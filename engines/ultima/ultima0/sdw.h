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

#ifndef ULTIMA_ULTIMA0_SDW_H
#define ULTIMA_ULTIMA0_SDW_H

#include "common/str.h"
#include "graphics/managed_surface.h"
#include "ultima/ultima0/akalabeth.h"

namespace Ultima {
namespace Ultima0 {

namespace SDLWrapper {

#define DEFAULT_SCX		(1024)					// Default Screen Size and Depth
#define DEFAULT_SCY		(768)
#define DEFAULT_SCBPP	(0)
#define MAXSOUND        (16)                    // Maximum number of sounds

#define SDWASSERT(x)	if (!(x)) SDWERROR()	// ASSERT and ERROR macros
#define SDWERROR()	FatalError(__LINE__,__FILE__)

int Main(int argc, char *argv[]);				// Main program must be defined.
char *Name(char *Name);		    			    // Return Window Caption Name must be defined

void SetSpeed(int n);
void FatalError(int Line, const Common::String &file);	// Prototyping

int GameClock(void);				            // No point in wrapping this, it just goes to SDL_GetTicks()
int SystemClock(void);                          // This one is unaffected by the game speed.

int ReadStick(int &A, int &B, int &dx, int &dy);   // Read a joystick
int MouseClick(int &x, int &y);				    // Read a mouse select - and - click
int ExitKey(void);
int GetKey(void);

#define SDL_Surface Graphics::ManagedSurface

struct SDL_Joystick {
};
struct SDL_AudioSpec {
};


class Rect {
public:
	Rect() {
	}							    // Constructors
	Rect(int x1, int y1, int x2, int y2) {
		Left = x1; Top = y1; Right = x2; Bottom = y2;
	}
	int Left, Top, Right, Bottom;				// The rectangle
};

class Surface {
public:
	Surface(int x = 0, int y = 0, int Trans = 0, int UseDisplay = 0, const char *File = nullptr);
	~Surface();

	void SetColour(int r, int g, int b);
	void SetColour() {
		SetColour(-1, -1, -1);
	}
	void SetColour(int Col) {
		Colour = Col;
	}
	unsigned int GetColour(void) {
		return Colour;
	}
	int  Width(void) {
		return xSize;
	}
	int  Height(void) {
		return ySize;
	}
	void SetOrigin(int x = 0, int y = 0) {
		xOrigin = x; yOrigin = y;
	}
	void SetScale(int x = 256, int y = 256) {
		xScale = x; yScale = y;
	}

	void Plot(int x1, int y1);
	void FillRect(int x1 = 0, int y1 = 0, int x2 = 0, int y2 = 0);
	void FillRect(const Rect &r) {
		FillRect(r.Left, r.Top, r.Right, r.Bottom);
	}
	void FrameRect(int x1 = 0, int y1 = 0, int x2 = 0, int y2 = 0);
	void FrameRect(const Rect &r) {
		FrameRect(r.Left, r.Top, r.Right, r.Bottom);
	}
	void FillEllipse(int x1 = 0, int y1 = 0, int x2 = 0, int y2 = 0);
	void FillEllipse(const Rect &r) {
		FillEllipse(r.Left, r.Top, r.Right, r.Bottom);
	}
	void FrameEllipse(int x1 = 0, int y1 = 0, int x2 = 0, int y2 = 0);
	void FrameEllipse(const Rect &r) {
		FrameEllipse(r.Left, r.Top, r.Right, r.Bottom);
	}
	void Line(int x1 = 0, int y1 = 0, int x2 = 0, int y2 = 0);

	void Copy(Surface &Target, const Rect &SrcRect, int x = 0, int y = 0);
	void Copy(const Rect &SrcRect, int x = 0, int y = 0);
	void Copy(Surface &Target, int x = 0, int y = 0);
	void Copy(int x = 0, int y = 0);

	void HorizontalMirror(int x1 = 0, int y1 = 0, int x2 = 0, int y2 = 0);
	void VerticalMirror(int x1 = 0, int y1 = 0, int x2 = 0, int y2 = 0);

	void Char(int x1 = 0, int y1 = 0, int x2 = 0, int y2 = 0, char c = ' ');
	void Char(const Rect &r, char c) {
		Char(r.Left, r.Top, r.Right, r.Bottom, c);
	}
	void String(int x1 = 0, int y1 = 0, int x2 = 0, int y2 = 0, const char *s = "");
	void String(Rect &r, const char *s) {
		String(r.Left, r.Top, r.Right, r.Bottom, s);
	}

	void Flip(void);


protected:
	void SortAndValidate(int &x1, int &y1, int &x2, int &y2);
	void PointProcess(int &x1, int &y1);

private:
	Graphics::ManagedSurface *sSurface = nullptr;	// Underlying Graphics::Surface
	int xSize = 0, ySize = 0;				// Surface size (physical)
	uint Colour = 0;						// Drawing colour
	int IsTransparent = 0;					// Set if transparent
	uint TransColour = 0;					// Transparency drawing colour
	int IsDisplay = 0;						// Set if is the physical display object
	int xOrigin = 0, yOrigin = 0;			// Mobile origin and scaling
	int xScale = 0, yScale = 0;
};

// A surface but with transparency
class TransparentSurface : public Surface {
public:
	TransparentSurface(int x = 0, int y = 0) : Surface(x, y, 1, 0, nullptr) {
	}
};

class BitmapSurface : public Surface		    // A surface with a bitmap on it, one solid, one transparent
{
public:
	BitmapSurface(char *File) : Surface(0, 0, 0, 0, File) {
	}
};

class TransparentBitmapSurface : public Surface
{
public:
	TransparentBitmapSurface(char *File) : Surface(0, 0, 1, 0, File) {
	}
};

class DisplaySurface : public Surface		    // The actual physical display
{
public:
	DisplaySurface(int x = 0, int y = 0) : Surface(0, 0, 0, 1, nullptr) {
	}
};

class Timer                                     // A simple timer
{
public:
	Timer(int TimeOut = 0);
	void ResetTimer(int t = 0);
	unsigned int Elapsed(void);
	int TimedOut(void);
	void WaitTimer(void);

private:
	int StartClock;
	int EndClock;
	int EventTime;
};

class AudioObject                               // An audio object
{
public:
	AudioObject() {
		Data = nullptr; Position = Length = 0; Attach(); SoundOn = 0; LoopSound = 0;
	}
	~AudioObject() {
		Detach(); if (Data != nullptr) free(Data);
	}
	void CopyStream(void *Stream, int Reqd);
	void Play(void) {
		Position = 0; SoundOn = 1;
	}
	void PlayLoop(void) {
		Position = 0; SoundOn = 1; LoopSound = 1;
	}
	void Stop(void) {
		SoundOn = 0; LoopSound = 0;
	}
	int  Size(void) {
		return Length / 2;
	}
	void Write(int Pos, int Dat);
protected:
	void Attach(void);
	void Detach(void);
	void *Data;
	int Position;
	int Length;
	int SoundOn;
	int LoopSound;
};

class AudioWave : public AudioObject
{
public:
	AudioWave(char *File) : AudioObject() {
		Load(File);
	}
protected:
	void Load(char *File);
};

class AudioBeep : public AudioObject
{
public:
	AudioBeep(int p, int l) : AudioObject() {
		CreateBeep(p, l);
	}
protected:
	void CreateWave(void *Data, int Size, int sPitch);
	void CreateBeep(int sPitch, int sLength);
};

class AudioNoise : public AudioBeep
{
public:
	AudioNoise(int l) : AudioBeep(0, l) {
	}
};

} // namespace SDLWrapper
} // namespace Ultima1
} // namespace Ultima

#endif
