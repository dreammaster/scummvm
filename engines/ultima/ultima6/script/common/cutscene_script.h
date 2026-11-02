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

#ifndef NUVIE_SCRIPT_COMMON_CUTSCENE_SCRIPT_H
#define NUVIE_SCRIPT_COMMON_CUTSCENE_SCRIPT_H

#include "common/array.h"
#include "ultima/ultima6/script/script_cutscene.h"

namespace Ultima {
namespace Ultima6 {
namespace Scripts {

/**
 * Base for scripted cutscenes such as intros and endings. Owns every sprite and
 * image the script creates and releases them when the script is destroyed.
 */
class CutsceneScript {
public:
	typedef Common::Array<Common::Array<CSImage *> > ImageTable;

private:
	struct Firework {
		CSSprite *_sprite;
		int _colour;
		int _frame;
	};

	Common::Array<CSSprite *> _sprites;
	Common::Array<CSImage *> _images;

	Firework createFirework(const ImageTable &images);
	int updateFireworks(Common::Array<Firework> &fireworks, const ImageTable &images);

protected:
	ScriptCutscene *_cutscene;
	bool _shouldExit;

	CutsceneScript();
	virtual ~CutsceneScript();

	static int randomRange(int low, int high);

	CSSprite *newSprite(CSImage *image, int x, int y, bool visible);
	void moveToFront(CSSprite *sprite);
	static void setOpacity(CSSprite *sprite, int opacity);
	void setClip(CSSprite *sprite, int x, int y, int w, int h);

	CSImage *newImage(uint16 w, uint16 h, int fill = -1);
	CSImage *copyImage(const CSImage *image);
	bool loadImages(const char *filename, ImageTable &images);

	/** Takes ownership of images loaded elsewhere */
	void adoptImages(const Common::Array<CSImage *> &images);

	static uint16 imageWidth(const CSImage *image);
	static void blit(CSImage *dest, const CSImage *src, uint16 x, uint16 y);

	int mouseX() const;
	int mouseY() const;
	static void showMouseCursor(bool show);
	static bool engineShouldQuit();
	void playSfx(uint16 sfxId, bool async = false);
	void musicPlay(const char *filename, uint16 songNum);
	void musicStop();

	void canvasUpdate();
	int inputPoll(bool pollMouseMotion = false);
	int waitForInput();

	/** Returns true if there was input. With no cycle count, only checks once */
	bool pollForKeyOrButton(int cycles = -1);
	bool pollForEsc(int cycles = -1);

	void fadeIn(int speed = 3);
	void fadeOut(int speed = 3);

	static const uint PLAYER_COUNT = 9;
	void updatePlayers(CSSprite *const *players, const ImageTable &images);
	CSSprite *createPlayerSprite(CSImage *image, int x, int y);
	CSSprite *createSprite(CSImage *image, int x, int y);
	void fireworks(const ImageTable &images, CSSprite *logo);
};

} // End of namespace Scripts
} // End of namespace Ultima6
} // End of namespace Ultima

#endif
