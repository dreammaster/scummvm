/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef LEGEND_EVENTS_H
#define LEGEND_EVENTS_H

#include "common/array.h"
#include "common/stack.h"
#include "graphics/screen.h"
#include "legend/messages.h"
#include "legend/gfx/screen.h"

namespace Legend {

#define FRAME_RATE 20
#define FRAME_DELAY (1000 / FRAME_RATE)

class Events;
class UIElement;

/**
 * Implements a thunk layer around an element's
 * bounds, allowing access to it as if it were
 * a simple Common::Rect, but any changes to it
 * will also be applied to a linked inner bounds
 */
struct Bounds {
private:
	UIElement *_owner;
	Common::Rect _bounds;
	Common::Rect &_innerBounds;
	int _borderXSize = 0, _borderYSize = 0;
public:
	const int16 &left;
	const int16 &top;
	const int16 &right;
	const int16 &bottom;
public:
	Bounds(UIElement *owner, Common::Rect &innerBounds);
	operator const Common::Rect &() const { return _bounds; }
	Bounds &operator=(const Common::Rect &r);
	void setBorderSize(size_t borderSize);
	void setBorderSize(size_t xSize, size_t ySize);
	int16 width() const { return _bounds.width(); }
	int16 height() const { return _bounds.height(); }
	bool isEmpty() const { return _bounds.isEmpty(); }

	Common::Point topLeft() const {
		return Common::Point(left, top);
	}
	Common::Point bottomRight() const {
		return Common::Point(right, bottom);
	}
};

/**
 * User interface element
 */
class UIElement {
	friend class Events;
private:
	int _timeoutCtr = 0;
protected:
	UIElement *_parent;
	Common::Array<UIElement *> _children;
	Common::Rect _innerBounds;
	Bounds _bounds;
	bool _needsRedraw = true;
	String _name;
protected:
	/**
	 * Set a delay countdown in seconds
	 */
	void delaySeconds(uint seconds);

	/**
	 * Set a delay countdown in frames
	 */
	void delayFrames(uint frames);

	/**
	 * Returns true if a delay is active
	 */
	bool isDelayActive() const {
		return _timeoutCtr != 0;
	}

	/**
	 * Cancels any active delay
	 */
	void cancelDelay() {
		_timeoutCtr = 0;
	}

	/**
	 * Called when an active timeout countdown expired
	 */
	virtual void timeout();

	/**
	 * Outer method for doing drawing
	 *
	 */
	virtual void drawElements();

private:
	/**
	 * Finds a view globally
	 */
	static UIElement *findViewGlobally(const String &name);
public:
	UIElement(const String &name, UIElement *uiParent);
	virtual ~UIElement() {}

	/**
	 * Returns true if the elements needs to be redrawn
	 */
	bool needsRedraw() const { return _needsRedraw; }

	/**
	 * Sets that the element needs to be redrawn
	 */
	void redraw();

	/**
	 * Focuses the element as the current view
	 */
	void focus();

	/**
	 * Closes the current view. The view must have been added
	 * via addView, so there's a remaining view afterwards
	 */
	void close();

	/*
	 * Returns true if the view is focused
	 */
	bool isFocused() const;

	/**
	 * Sets the focus to a new view
	 */
	void replaceView(UIElement *ui, bool replaceAllViews = false);
	void replaceView(const String &name, bool replaceAllViews = false);

	/**
	 * Adds a focused view to the view stack without replacing current one
	 */
	void addView(UIElement *ui);
	void addView(const String &name);
	void addView();
	void open() { addView(); }

	/**
	 * Returns a random number
	 */
	int getRandomNumber(int minNumber, int maxNumber);
	int getRandomNumber(int maxNumber);

	/**
	 * Sets the element's bounds
	 */
	virtual void setBounds(const Common::Rect &r) {
		_bounds = r;
	}

	/**
	 * Gets the element's bounds
	 */
	Common::Rect getBounds() const {
		return _bounds;
	}

	/**
	 * Returns a surface for drawing the element
	 */
	Graphics::ManagedSurface getSurface() const;

	/**
	 * Clear the surface
	 */
	virtual void clearSurface();

	/**
	 * Draws the element
	 */
	virtual void draw();

	/**
	 * Called for game frame ticks
	 */
	virtual bool tick();

	/**
	 * Called when the bounds are canged
	 */
	virtual void metricsChanged() {}

	/**
	 * Find a view by name
	 */
	virtual UIElement *findView(const String &name);

	/**
	 * Handles events
	 */
	#define MESSAGE(NAME) \
	protected: \
		virtual bool msg##NAME(const NAME##Message &e) { \
			for (Common::Array<UIElement *>::iterator it = _children.begin(); \
					it != _children.end(); ++it) { \
				if ((*it)->msg##NAME(e)) return true; \
			} \
			return false; \
		} \
	public: \
		bool send(const String &viewName, const NAME##Message &msg) { \
			UIElement *view = UIElement::findViewGlobally(viewName); \
			assert(view); \
			return view->msg##NAME(msg); \
		} \
		bool send(const NAME##Message &msg) { \
			return send("Root", msg); \
		} \

	MESSAGE(Focus);
	MESSAGE(Unfocus);
	MESSAGE(Keypress);
	MESSAGE(MouseDown);
	MESSAGE(MouseUp);
	MESSAGE(MouseWheel);
	MESSAGE(MouseDrag);
	MESSAGE(Action);
	MESSAGE(Game);
	MESSAGE(Value);
	MESSAGE(Text);
	#undef MESSAGE
};

/**
 * Main events and view manager
 */
class Events : public UIElement {
private:
	Graphics::Screen *_screen = nullptr;
	Common::Stack<UIElement *> _views;
	enum DragState {
		DS_UNPRESSED, DS_PRESSED, DS_DRAGGING
	};
	DragState _dragState = DS_UNPRESSED;
	Common::Point _dragStartPos;

protected:
	/**
	 * Process an event
	 */
	void processEvent(Common::Event &ev);

	/**
	 * Overrides events we want to only go to the focused view
	 */
	#define MESSAGE(NAME) \
		bool msg##NAME(const NAME##Message &e) override { \
			return !_views.empty() ? focusedView()->msg##NAME(e) : false; \
		}
	MESSAGE(Action);
	MESSAGE(Focus);
	MESSAGE(Unfocus);
	MESSAGE(Keypress);
	MESSAGE(MouseDown);
	MESSAGE(MouseUp);
	MESSAGE(MouseWheel);
	MESSAGE(MouseDrag);
	MESSAGE(Text);
#undef MESSAGE
public:
	Events();
	virtual ~Events();

	/**
	 * Main game loop
	 */
	virtual void runGame(Gfx::Screen *screen);

	/**
	 * Sets the focus to a new view
	 */
	void replaceView(UIElement *ui, bool replaceAllViews = false);
	void replaceView(const String &name, bool replaceAllViews = false);

	/**
	 * Adds a focused view to the view stack without replacing current one
	 */
	void addView(UIElement *ui);
	void addView(const String &name);

	/**
	 * Clears the view list
	 */
	void clearViews();

	/**
	 * Pops a view from the view stack
	 */
	void popView();

	/**
	 * Returns the currently focused view, if any
	 */
	UIElement *focusedView() const {
		return _views.empty() ? nullptr : _views.top();
	}

	/**
	 * Returns the view prior to the current view, if any
	 */
	UIElement *priorView() const {
		return _views.size() < 2 ? nullptr :
			_views[_views.size() - 2];
	}

	/**
	 * Returns true if a view of a given name is present
	 * at all in the visible view stack
	 */
	bool isPresent(const String &name) const;

	Graphics::Screen *getScreen() const {
		return _screen;
	}

	void drawElements() {
		if (!_views.empty())
			focusedView()->drawElements();
	}

	/**
	 * Checks whether a keypress is pending
	 */
	bool isKeypressPending() const;

	void draw() override {}

	bool tick() override {
		return !_views.empty() ? focusedView()->tick() : false;
	}
};

extern Events *g_events;

} // namespace Legend

#endif
