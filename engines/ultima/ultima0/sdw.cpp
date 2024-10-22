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

#include "common/system.h"
#include "common/textconsole.h"
#include "ultima/ultima0/sdw.h"

namespace Ultima {
namespace Ultima0 {

using namespace SDLWrapper;

static SDL_Surface *Display;					// The SDL Display screen
static int Joystick;							// Do we have a joystick or do we mimic it ?
static SDL_Joystick *JoyPtr;					// Pointer to a joystick object
static int GameSpeed = 120;						// Scales the system clock.
static SDL_AudioSpec AudioFmt;                  // Audio system specification
static AudioObject *SoundList[MAXSOUND];     // Audio objects in use.

static uint32 _GetPixel(SDL_Surface *Surface, int x, int y);

static void SoundInitialise();
static void SoundTerminate();
static void SoundCallBack(void *Data,byte *Stream,int Length);

//	**************************************************************************************************************************
//
//										Called when ERROR or ASSERT failes
//
//	**************************************************************************************************************************

void SDLWrapper::FatalError(int Line, const Common::String &file) {
	error("Fatal Error at line %d of %s", Line, file.c_str());
}

//	**************************************************************************************************************************
//
//								Set the game running speed, percent of normal
//
//	**************************************************************************************************************************

void SDLWrapper::SetSpeed(int n)
{
    GameSpeed = n;
}


//	**************************************************************************************************************************
//
//												Constructor for a surface
//
//	**************************************************************************************************************************

Surface::Surface(int x, int y, int Trans, int UseDisplay, const char *File) {
	IsTransparent = Trans;						// Save transparency flag
	IsDisplay = UseDisplay;						// Save physical display flag
	TransColour = 0;
	SetScale();									// Reset scale and origin
	SetOrigin();

	if (File == nullptr) {
		// Not loading a bitmap
		if (x == 0) x = Display->w;				// if x = y = 0 use whole screen
		if (y == 0) y = Display->h;
		xSize = x; ySize = y;					// Store width and height

		if (IsDisplay == 0) {
			sSurface = new Graphics::ManagedSurface(x, y);
			assert(Display->format.bpp() == 8);

			// A horrible transparency colour
			TransColour = sSurface->format.RGBToColor(220, 20, 130);

			SetColour();						// Set the colour to the default (normally black)
			FillRect();							// Erase the surface
		} else {
			// Use the display surface
			sSurface = Display;
			SetColour();
		}
	} else {
		// Loading a bitmp
		error("TODO");
	}
}

//	**************************************************************************************************************************
//
//												Destructor for a surface
//
//	**************************************************************************************************************************


Surface::~Surface() {
	delete sSurface;
}

//	**************************************************************************************************************************
//
//													Translate Points
//
//	**************************************************************************************************************************

void Surface::PointProcess(int &x,int &y)
{
	x = ((x * xScale) >> 8) + xOrigin;

	y = ((y * yScale) >> 8) + yOrigin;
}

//	**************************************************************************************************************************
//
//													Set drawing colour
//
//	**************************************************************************************************************************

void Surface::SetColour(int r,int g,int b) {
	if (r >= 0 && g >= 0 && b >= 0)
		// Setting a colour normally
		Colour = sSurface->format.RGBToColor(r, g, b);
	else {											// Default behaviour
		if (IsTransparent) {
			// Transparency, or not.
			Colour = TransColour;
		} else
			SetColour(0,0,0);
	}
}

//	**************************************************************************************************************************
//
//													  Single Pixel Plot
//
//	**************************************************************************************************************************

void Surface::Plot(int x1,int y1) {
	sSurface->hLine(x1, y1, x1, Colour);
}

//	**************************************************************************************************************************
//
//													Fill a solid rectangle
//
//	**************************************************************************************************************************

void Surface::FillRect(int x1, int y1, int x2, int y2) {
	sSurface->fillRect(Common::Rect(x1, y1, x2, y2), Colour);
}

//	**************************************************************************************************************************
//
//													    Frame a rectangle
//
//	**************************************************************************************************************************

void Surface::FrameRect(int x1, int y1, int x2, int y2) {
	sSurface->frameRect(Common::Rect(x1, y1, x2, y2), Colour);
}

//	**************************************************************************************************************************
//
//								Sort coordinates , x and y into order. Check they are in range
//
//	**************************************************************************************************************************

void Surface::SortAndValidate(int &x1, int &y1, int &x2, int &y2) {
	if (x1 > x2) SWAP(x1, x2);
	if (y1 > y2) SWAP(y1, y2);
}

//	**************************************************************************************************************************
//
//								Copying from one surface to another : Copying is ALWAYS Physical Coordinates
//
//	(1)	From this to a target, given a source rectangle and optional position
//	(2) From this to the display, given a source rectangle and optional position

//	(3) From this to the display, whole surface, optional position
//	(4) From this to a target, whole surface, optional position
//
//	**************************************************************************************************************************

static void _SurfaceCopier(Graphics::ManagedSurface *Src, Graphics::ManagedSurface *Tgt,
	int left,int top,int right,int bottom, int x,int y);

void Surface::Copy(Surface &Target, const Rect &SrcRect, int x, int y) {
	_SurfaceCopier(sSurface, Target.sSurface,
		SrcRect.Left, SrcRect.Top, SrcRect.Right, SrcRect.Bottom, x, y);
}

void Surface::Copy(const Rect &SrcRect, int x, int y) {
	_SurfaceCopier(sSurface, Display,
		SrcRect.Left,SrcRect.Top,SrcRect.Right,SrcRect.Bottom,x,y);
}

void Surface::Copy(int x,int y) {
	_SurfaceCopier(sSurface, Display, 0,0,xSize-1,ySize-1,x,y);
}

void Surface::Copy(Surface &Target,int x,int y) {
	_SurfaceCopier(sSurface, Target.sSurface, 0, 0, xSize - 1, ySize - 1, x, y);
}

static void _SurfaceCopier(SDL_Surface *Src,SDL_Surface *Tgt,
		int left,int top,int right,int bottom, int x,int y) {
	Tgt->blitFrom(*Src, Common::Rect(left, top, right, bottom),
		Common::Point(x, y));
}


//	**************************************************************************************************************************
//
//									These routines draw filled and outline ellipses

//
//	**************************************************************************************************************************

// This one draws two points or a line per vertical slice of ellipse
static void _EllipsePoint(SDL_Surface *s, int x,int y,int w,uint32 c,int Solid) {
	Common::Rect rc;

	if (Solid) {
		rc = Common::Rect(x, y, x + w, y + 1);
		s->fillRect(rc, c);
	} else {
		rc = Common::Rect(x, y, x + 1, y + 1);
		s->fillRect(rc, c);

		rc.moveTo(x + w, y);
		s->fillRect(rc, c);
	}
}

// Bresenham Algorithm
static void _DrawEllipse(SDL_Surface *s, int mx, int my, int a, int b,
		uint32 c,int Solid) {
	int x,mx1,mx2,my1,my2;
	int aq,bq,dx,dy,r,rx,ry;

	_EllipsePoint(s, mx - a, my, a*2, c, Solid);

	mx1 = mx - a;   my1 = my;
	mx2 = mx + a;   my2 = my;


	aq = a * a;bq = b * b;

	dx = aq * 2;dy = bq * 2;
	r = a * bq;
	rx = r * 2;
	ry = 0;
	x = a;

	while (x > 0)
	{
	    if (r > 0)

		{
			my1++;my2--;
			ry = ry+dx;
			r = r - ry;
		}
	    if (r <= 0)
		{
			x--;
			mx1++;mx2--;
			rx = rx - dy;
			r = r + rx;

		}

		_EllipsePoint(s,mx1,my1,mx2-mx1,c,Solid);
		_EllipsePoint(s,mx1,my2,mx2-mx1,c,Solid);
	}
}

void Surface::FillEllipse(int x1,int y1,int x2,int y2)
{
	if (x2 == 0) x2 = xSize-1;					// Default values for the bottom corner
	if (y2 == 0) y2 = ySize-1;
	PointProcess(x1,y1);
	PointProcess(x2,y2);
	SortAndValidate(x1,y1,x2,y2);				// Sort the 2 corners so they become left,top,right,bottom
	_DrawEllipse(sSurface,	// Draw filled ellipse
					(x1+x2)/2,(y1+y2)/2,
					(x2-x1)/2,(y2-y1)/2,
					(uint32)Colour,1);
}

void Surface::FrameEllipse(int x1,int y1,int x2,int y2)
{
	if (x2 == 0) x2 = xSize-1;					// Default values for the bottom corner
	if (y2 == 0) y2 = ySize-1;
	PointProcess(x1,y1);
	PointProcess(x2,y2);
	SortAndValidate(x1,y1,x2,y2);				// Sort the 2 corners so they become left,top,right,bottom
	_DrawEllipse(sSurface,	// Draw outline ellipse
					(x1+x2)/2,(y1+y2)/2,
					(x2-x1)/2,(y2-y1)/2,
					(uint32)Colour,0);
}


//	**************************************************************************************************************************
//
//								This is a Bresenham Line Algorithm for drawing lines
//
//	**************************************************************************************************************************

static void _LinePixel(SDL_Surface *s,int x,int y,int c) {
	if (x >= 0 && y >= 0 && x < s->w && y < s->h) {
		s->hLine(x, y, x, c);
	}
}

void Surface::Line(int x1,int y1,int x2,int y2) {
	int x,y,dx,dy,s1,s2;
	int i,dp,temp,swap=0;

	PointProcess(x1,y1);
	PointProcess(x2,y2);

	x = x1;y = y1;
	_LinePixel(sSurface,x1,y1,Colour);
	dx=abs(x2-x1);
	dy=abs(y2-y1);

	if   (x2<x1) s1=-1;
	else if(x2>x1) s1=1;
	else s1=0;

	if   (y2<y1) s2=-1;
	else if(y2>y1) s2=1;
	else s2=0;

	dp = 2*dy-dx;
	if (dy > dx)
	{
		temp=dx;dx=dy;dy=temp;swap=1;
	}
	for(i = 1;i <= dx;i++)
	{
		if (dp < 0)
		{
			 if (swap)

				 _LinePixel(sSurface,x,y=y+s2,Colour);
			 else
				 _LinePixel(sSurface,x=x+s1,y,Colour);
			 dp = dp + 2*dy;
		}
		else
		{
			_LinePixel(sSurface,x=x+s1,y=y+s2,Colour);
			dp = dp+2*dy-2*dx;
		}
	}
}

//	**************************************************************************************************************************

//
//									An 8x8 pixel font, just alphanumeric characters
//
//	**************************************************************************************************************************

static const byte font1[2048] = {
	124,130,186,162,186,130,124,0,126,129,165,129,165,153,129,126,
	126,129,165,129,153,165,129,126,108,246,246,254,124,56,16,0,
	16,56,124,254,124,56,16,0,16,56,84,254,84,16,56,0,
	56,124,254,254,108,16,56,0,16,24,20,20,48,112,96,0,
	254,254,254,238,254,254,254,0,236,138,138,170,170,170,236,0,
	142,136,136,140,136,136,232,0,174,170,170,234,170,170,174,0,
	238,136,136,204,136,136,136,0,238,138,138,142,140,138,234,0,
	62,34,62,34,102,238,204,0,16,84,40,198,40,84,16,0,
	240,248,252,254,252,248,240,0,30,62,126,254,126,62,30,0,
	16,56,124,16,124,56,16,0,238,238,238,238,238,0,238,0,
	254,68,68,68,68,68,68,0,126,128,188,198,122,2,252,0,
	0,0,0,0,255,255,0,0,16,56,124,16,124,56,16,254,
	16,56,124,254,56,56,56,0,56,56,56,254,124,56,16,0,
	16,24,252,254,252,24,16,0,16,48,126,254,126,48,16,0,



	144,72,36,18,36,72,144,0,18,36,72,144,72,36,18,0,
	16,40,68,146,40,68,130,0,130,68,40,146,68,40,16,0,
	0,0,0,0,0,0,0,0,16,16,16,16,16,0,16,0,

	40,40,40,0,0,0,0,0,68,254,68,68,68,254,68,0,
	16,126,144,124,18,252,16,0,66,164,72,16,36,74,132,0,
	56,68,56,112,138,132,122,0,16,16,32,0,0,0,0,0,
	8,16,16,16,16,16,8,0,32,16,16,16,16,16,32,0,
	16,84,56,254,56,84,16,0,16,16,16,254,16,16,16,0,
	0,0,0,0,0,16,16,32,0,0,0,254,0,0,0,0,
	0,0,0,0,0,0,16,0,2,4,8,16,32,64,128,0,
	124,130,130,130,130,130,124,0,240,16,16,16,16,16,254,0,
	252,2,2,124,128,128,254,0,252,2,2,28,2,2,252,0,
	130,130,130,126,2,2,2,0,254,128,252,2,2,2,252,0,
	126,128,252,130,130,130,124,0,252,2,2,2,2,2,2,0,
	124,130,130,124,130,130,124,0,126,130,130,126,2,2,252,0,
	0,0,0,16,0,0,16,0,0,0,0,16,0,0,16,32,
	8,16,32,64,32,16,8,0,0,0,0,254,0,254,0,0,
	64,32,16,8,16,32,64,0,56,68,4,8,16,0,16,0,
	60,66,154,170,156,64,62,0,124,130,130,254,130,130,130,0,
	252,130,130,252,130,130,252,0,124,130,128,128,128,130,124,0,
	252,130,130,130,130,130,252,0,254,128,128,240,128,128,254,0,
	254,128,128,240,128,128,128,0,124,130,128,142,130,130,124,0,
	130,130,130,254,130,130,130,0,254,16,16,16,16,16,254,0,
	62,2,2,2,130,130,124,0,130,132,136,240,136,132,130,0,
	128,128,128,128,128,128,254,0,252,146,146,146,146,146,146,0,
	130,194,162,146,138,134,130,0,124,130,130,130,130,130,124,0,
	252,130,130,252,128,128,128,0,124,130,130,130,138,134,126,0,
	252,130,130,252,130,130,130,0,126,128,128,124,2,2,252,0,
	254,16,16,16,16,16,16,0,130,130,130,130,130,130,124,0,
	130,130,68,68,40,40,16,0,130,130,130,146,146,146,108,0,
	130,68,40,16,40,68,130,0,130,130,130,126,2,2,252,0,
	254,4,8,16,32,64,254,0,56,32,32,32,32,32,56,0,
	128,64,32,16,8,4,2,0,56,8,8,8,8,8,56,0,
	16,40,68,130,0,0,0,0,0,0,0,0,0,0,0,255,
	32,32,16,0,0,0,0,0,0,0,56,68,124,68,68,0,
	0,0,120,68,120,68,120,0,0,0,60,64,64,64,60,0,
	0,0,120,68,68,68,120,0,0,0,124,64,112,64,124,0,
	0,0,124,64,112,64,64,0,0,0,60,64,76,68,60,0,
	0,0,68,68,124,68,68,0,0,0,124,16,16,16,124,0,
	0,0,28,4,4,68,56,0,0,0,68,72,112,72,68,0,
	0,0,64,64,64,64,124,0,0,0,120,84,84,84,84,0,
	0,0,120,68,68,68,68,0,0,0,56,68,68,68,56,0,
	0,0,120,68,120,64,64,0,0,0,56,68,68,76,54,0,
	0,0,120,68,120,68,68,0,0,0,60,64,56,4,120,0,
	0,0,124,16,16,16,16,0,0,0,68,68,68,68,56,0,
	0,0,68,68,40,40,16,0,0,0,68,68,84,108,68,0,
	0,0,68,40,16,40,68,0,0,0,68,68,60,4,120,0,
	0,0,124,8,16,32,124,0,8,16,16,32,16,16,8,0,
	16,16,16,0,16,16,16,0,32,16,16,8,16,16,32,0,
	80,40,0,0,0,0,0,0,0,16,40,68,130,130,254,0,
	254,254,254,254,254,254,254,0,0,0,0,0,0,254,254,0,
	0,0,124,124,124,124,124,0,0,0,0,0,0,0,124,0,
	128,128,128,128,128,128,128,0,0,64,64,64,64,64,64,0,
	16,24,28,30,28,24,16,0,16,48,112,240,112,48,16,0,
	62,30,30,62,114,224,64,0,4,14,156,248,240,240,248,0,
	64,224,114,62,30,30,62,0,248,240,240,248,156,14,4,0,
	56,68,130,130,130,68,56,0,56,124,254,254,254,124,56,0,
	0,124,68,68,68,124,0,0,0,124,124,124,124,124,0,0,
	0,60,110,126,112,126,60,0,0,60,118,126,14,126,60,0,
	0,60,126,106,126,126,106,0,0,60,126,86,126,126,86,0,
	0,0,0,24,24,0,0,0,0,0,24,60,60,24,0,0,

	0,12,52,36,36,108,72,0,0,0,0,0,0,0,0,0,
	60,126,198,231,255,224,126,60,60,126,227,231,255,7,126,60,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,52,118,118,94,126,60,0,0,60,110,126,112,126,60,0,
	0,60,126,122,110,110,44,0,0,60,126,14,126,118,60,0,
	0,0,0,0,0,0,0,0,126,126,126,126,60,0,0,0,
	0,15,31,31,31,31,15,0,126,127,127,127,127,127,63,0,
	0,0,0,60,126,126,126,126,126,126,126,126,126,126,126,126,
	0,63,127,127,127,127,127,126,126,127,127,127,127,127,127,126,


	0,240,248,248,248,248,240,0,126,254,254,254,254,254,252,0,
	0,255,255,255,255,255,255,0,126,255,255,255,255,255,255,0,
	0,252,254,254,254,254,254,126,126,254,254,254,254,254,254,126,
	0,255,255,255,255,255,255,126,126,255,255,255,255,255,255,126,
	0,0,63,63,48,55,52,52,0,0,255,255,0,255,0,0,
	0,0,248,248,24,216,88,88,88,88,88,88,88,88,88,88,
	88,216,24,248,248,0,0,0,0,255,0,255,255,0,0,0,
	52,55,48,63,63,0,0,0,52,52,52,52,52,52,52,52,
	0,0,0,31,24,24,24,24,0,0,0,255,0,0,0,0,
	0,0,0,240,48,48,48,48,48,48,48,48,48,48,48,48,
	48,48,48,240,0,0,0,0,0,0,0,255,0,0,0,0,
	24,24,24,31,0,0,0,0,24,24,24,24,24,24,24,24,
	136,34,136,34,136,34,136,34,85,170,85,170,85,170,85,170,
	68,170,68,170,68,170,68,170,51,102,204,153,51,102,204,153,
	204,102,51,153,204,102,51,153,199,143,31,62,124,248,241,227,
	227,241,248,124,62,31,143,199,174,128,186,2,234,8,171,32,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,


	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
};


//	**************************************************************************************************************************
//
//										Check if pixel is set in character
//
//	**************************************************************************************************************************

static int _FONTPixelSet(const byte *Data,int x,int y)
{
    if (x < 0 || y < 0 || x > 7 || y > 7) return 0;

    return (Data[y] & (0x80 >> x)) ? 1 : 0;
}

//	**************************************************************************************************************************

//
//					Draw an angled line - this stops the squared corners on diagonals showing
//
//	**************************************************************************************************************************

static void _FONTAngleDraw(SDL_Surface *s, Common::Rect *rc,int w,int h,uint32 Colour) {
    int i,m;

    m = abs(w);if (abs(h) > m) m = abs(h);
    for (i = 0; i < m; i++) {
		Common::Rect rc3(rc->width(), rc->height());
		rc3.moveTo(rc->left + w * i / m, rc->top + h * i / m);
		s->fillRect(rc3, Colour);
    }
}

//	**************************************************************************************************************************
//
//							Draw an 8x8 pixellated character (for fonts), does some rounding etc.
//
//	**************************************************************************************************************************

static void _FONTChar(SDL_Surface *s, Common::Rect *rc,uint32 Colour,int Char)
{
    int x,y,w,h;
    const byte *GfxData;
    Common::Rect rc2;
    w = rc->width() / 8;
	h = rc->height() / 8;					// work out the box sizes
    SDWASSERT(w != 0 && h != 0);				// At least one pixel !
    if (Char == ' ') return;					// Don't do anything for spaces
	GfxData = font1+Char*8;
    for (x = 0;x < 8;x++)						// Work through the 64 pixel array
        for (y = 0;y < 8;y++)
			// If set.
            if (_FONTPixelSet(GfxData,x,y)) {
                rc2.left = rc->left+rc->width()*x/8;		// Calculate the bounding rectangle
                rc2.top = rc->top+rc->height()*y/8;
				rc2.setWidth(rc->left + rc->width() * (x + 1) / 8 - rc2.left);

				rc2.setHeight(rc->top+rc->height()*(y+1)/8-rc2.top);
				s->fillRect(rc2, Colour);		// Draw an pixel there

												// Neaten the diagonals
                if (_FONTPixelSet(GfxData,x,y+1) == 0 &&
                    _FONTPixelSet(GfxData,x-1,y) == 0 &&
                    _FONTPixelSet(GfxData,x-1,y+1) != 0)
                            _FONTAngleDraw(s,&rc2,-w,h,Colour);

                if (_FONTPixelSet(GfxData,x,y+1) == 0 &&
                    _FONTPixelSet(GfxData,x+1,y) == 0 &&
                    _FONTPixelSet(GfxData,x+1,y+1) != 0)
                            _FONTAngleDraw(s,&rc2,w,h,Colour);
            }
}

//	**************************************************************************************************************************
//
//								Draw a single character using the bevelled font
//
//	**************************************************************************************************************************

void Surface::Char(int x1,int y1,int x2,int y2,char c)
{
	Common::Rect rc;
	if (x2 == 0) x2 = xSize-1;					// Default values for the bottom corner
	if (y2 == 0) y2 = ySize-1;
	PointProcess(x1,y1);
	PointProcess(x2,y2);
	SortAndValidate(x1,y1,x2,y2);				// Sort the 2 corners so they become left,top,right,bottom

	rc.left = x1;
	rc.top = y1;						// Set up bounding rectangle
	rc.setWidth(x2 - x1 + 1);
	rc.setHeight(y2 - y1 + 1);

	_FONTChar(sSurface,&rc,(uint32)Colour,c);
}


//	**************************************************************************************************************************
//
//										Draw a string using the bevelled font
//
//	**************************************************************************************************************************


void Surface::String(int x1, int y1, int x2, int y2, const char *s) {
	int i,n;
	if (s == nullptr) SDWERROR();					// Don't pass me nullptr !
	if (*s == '\0') return;						// Empty string.
	if (x2 == 0) x2 = xSize-1;					// Default values for the bottom corner
	if (y2 == 0) y2 = ySize-1;
	PointProcess(x1,y1);
	PointProcess(x2,y2);
	SortAndValidate(x1,y1,x2,y2);				// Sort the 2 corners so they become left,top,right,bottom
	n = strlen(s);								// Length of string
	for (i = 0;i < n;i++)
		Char(x1+i*(x2-x1)/n,y1,x1+(i+1)*(x2-x1)/n,y2,s[i]);
}

//	**************************************************************************************************************************
//
//											Pixel Reader (from the SDL Documentation)
//
//	**************************************************************************************************************************

static uint32 _GetPixel(SDL_Surface *Surface, int x, int y) {
	int bpp;
	byte *p;
	uint32 r = 0;

    bpp = Surface->format.bytesPerPixel;
	p = (byte *)Surface->getBasePtr(x, y);

    switch(bpp) {
	case 1:
		r = *p;
		break;

	case 2:
		r = *(uint16 *)p;
		break;

	case 4:
		r =  *(uint32 *)p;
		break;

	default:
		break;
    }

	return r;
}

//	**************************************************************************************************************************
//
//											Flip a surface (Double Buffering)
//
//	**************************************************************************************************************************

void Surface::Flip() {
	g_ultima->_screen->blitFrom(*Display);
	g_ultima->_screen->update();
}

//	**************************************************************************************************************************
//

//												  Mirroring functions
//
//	**************************************************************************************************************************

void Surface::HorizontalMirror(int x1,int y1,int x2,int y2)
{
	int x,y;
	uint32 c1,c2;
	Common::Rect rc;
	SDL_Surface *s = sSurface;
	if (x2 == 0) x2 = xSize-1;					// Default values for the bottom corner
	if (y2 == 0) y2 = ySize-1;
	PointProcess(x1,y1);
	PointProcess(x2,y2);
	SortAndValidate(x1,y1,x2,y2);				// Sort the 2 corners so they become left,top,right,bottom
	rc.setWidth(1);
	rc.setHeight(1);

	for (y = y1;y <= y2;y++)
		for (x = 0;x < (x2-x1)/2;x++) {
			c1 = _GetPixel(s,x1+x,y);
			c2 = _GetPixel(s,x2-x,y);

			rc.left = x1+x;rc.top = y;

			s->fillRect(rc, c2);

			rc.left = x2-x;
			s->fillRect(rc, c1);
		}
}

void Surface::VerticalMirror(int x1,int y1,int x2,int y2)
{
	int x,y;
	uint32 c1,c2;
	Common::Rect rc;
	SDL_Surface *s = sSurface;
	if (x2 == 0) x2 = xSize-1;					// Default values for the bottom corner
	if (y2 == 0) y2 = ySize-1;
	PointProcess(x1,y1);
	PointProcess(x2,y2);
	SortAndValidate(x1,y1,x2,y2);				// Sort the 2 corners so they become left,top,right,bottom

	rc.setWidth(1); rc.setHeight(1);
	for (x = x1;x <= x2;x++)
		for (y = 0;y < (y2-y1)/2;y++)
		{
			c1 = _GetPixel(s,x,y1+y);
			c2 = _GetPixel(s,x,y2-y);
			rc.left = x;rc.top = y1+y;
			s->fillRect(rc, c2);
			rc.top = y2-y;
			s->fillRect(rc, c1);
		}
}


//	**************************************************************************************************************************
//
//							Function returns system timer, game timer
//
//	**************************************************************************************************************************

int SDLWrapper::GameClock() {
	return (int)g_ultima->getTicks() * GameSpeed/100;
}

int SDLWrapper::SystemClock() {
	return (int)g_ultima->getTicks();
}

//	**************************************************************************************************************************
//
//														Timer Class
//
//	**************************************************************************************************************************

Timer::Timer(int TimeOut)					// Create it
{
	StartClock = GameClock();				// When we started
	EventTime = TimeOut;						// When the timer times out
	ResetTimer();								// Set the end time
}

void Timer::ResetTimer(int t)				// Reset it. If default param (0) use same time as before
{
    if (t != 0) EventTime = t;
	EndClock = GameClock() + EventTime;
}

unsigned int Timer::Elapsed()			// Time elapsed since creation
{
	return GameClock() - StartClock;
}

int Timer::TimedOut()					// Has the timer timed out
{
	int t = (GameClock() > EndClock);
    if (t) ResetTimer(0);
    return t;
}

void Timer::WaitTimer()					// Wait for the timer to time out

{
	while (!TimedOut()) {}
}

//	**************************************************************************************************************************
//
//							Read a joystick - if there is one - or emulate it from the keyboard
//
//	**************************************************************************************************************************

static int _Process(int Pos)					// Converts an analogue position to a digital directional value
{
	if (abs(Pos) < 1024) return 0;
	return (Pos < 0) ? -1 : 1;
}

int SDLWrapper::ReadStick(int &A,int &B,int &dx,int &dy)
{
#ifdef TODO
	Common::Event e;
	int r;
	while (SDL_PollEvent(&e)) {}				// Process the event queue
	if (Joystick && 0)         					// Joystick available ? (ID#0)
	{
		SDL_JoystickUpdate();					// Update everything
		dx = _Process(SDL_JoystickGetAxis(JoyPtr,0));
		dy = _Process(SDL_JoystickGetAxis(JoyPtr,1));
		A = r = (SDL_JoystickGetButton(JoyPtr,0) != 0);
		B = (SDL_JoystickGetButton(JoyPtr,1) != 0);
	}
	else										// No joystick, use keyboard (arrow keys Z and X)
	{
		byte *Key = SDL_GetKeyState(nullptr);
		dx = dy = 0;							// ZXCF or Arrow keys
		if (Key[SDLK_LEFT])		dx = -1;
        if (Key[SDLK_RIGHT])    dx = 1;
		if (Key[SDLK_DOWN])		dy = 1;
		if (Key[SDLK_UP])  		dy = -1;
												// A & B are G and H or 1 and 2 on the keypad
		A = r = (Key[SDLK_LCTRL] != 0);
		B = (Key[SDLK_LALT] != 0);

	}
	return r;
#endif
	return 0;
}

//	**************************************************************************************************************************
//
//							            Game Over Key Pressed ?
//
//	**************************************************************************************************************************

int SDLWrapper::ExitKey()
{
    SDL_Event e;

    byte *Key;
	while (SDL_PollEvent(&e)) {}				// Process the event queue
    Key = SDL_GetKeyState(nullptr);
    return (Key[SDLK_ESCAPE] != 0);
}

int SDLWrapper::GetKey()
{
	SDL_Event e;
	int Key = -1;
	
	while (Key < 0)
	{
		while (SDL_PollEvent(&e))
			if (e.type == SDL_KEYDOWN)
				Key = e.key.keysym.sym;
	}
	return Key;
}

//	**************************************************************************************************************************
//
//							Display Pointer, Follow Mouse around, Wait for Click.
//
//	**************************************************************************************************************************

int SDLWrapper::MouseClick(int &x,int &y)
{
	int n;
	SDL_Event e;
	SDL_ShowCursor(SDL_ENABLE);					// Bring the cursor back
	while (n = SDL_GetMouseState(&x,&y),n != 0)	// Wait for no buttons
								while (SDL_PollEvent(&e)) {}

	while (n = SDL_GetMouseState(&x,&y),n == 0)	// Wait for a button press
	{

		while (SDL_PollEvent(&e)) {}			// Process mouse events
	}
	SDL_ShowCursor(SDL_DISABLE);				// Hide mouse cursor
	n = n & SDL_BUTTON(1);						// Return true if left pressed, false otherwise
	return n;
}

//	**************************************************************************************************************************
//
//                      Mix data from an audio object into the Stream - used by callback function
//
//	**************************************************************************************************************************

void AudioObject::CopyStream(void *Stream,int Reqd)
{
    int Qty = Reqd;                             // Bytes to copy
    if (SoundOn == 0) return;
    if (Qty > Length-Position)                  // Can't use more than are available
    {
        if (LoopSound)
            Position = 0;
        else
            Qty = Length-Position;
    }
    if (Qty != 0)                               // Some data ?
    {
        byte *Audio = (byte *)Data;
        Audio = Audio + Position;
        SDL_MixAudio((byte *)Stream,Audio,Qty, // If so, mix it in.
                                    SDL_MIX_MAXVOLUME);
        Position = Position + Qty;
    }
}

//	**************************************************************************************************************************
//
//                          Attach and Detach objects from the internal sound object list
//
//	**************************************************************************************************************************

void AudioObject::Attach()
{
    int i = 0;
    for (i = 0;i < MAXSOUND;i++)                // Look for empty slot
        if (SoundList[i] == nullptr)               // If found, copy reference in
        {
            SoundList[i] = this;
            return;
        }
}

void AudioObject::Detach()
{
    for (int i = 0;i < MAXSOUND;i++)            // Look for it and delete it.

            if (SoundList[i] == this) SoundList[i] = nullptr;
}

//	**************************************************************************************************************************
//
//                                                  Load a .WAV file
//
//	**************************************************************************************************************************

void AudioWave::Load(char *File)
{
    SDL_AudioSpec Wave;
    byte *WaveData;
    uint32 Size;
    SDL_AudioCVT Cvt;

    if (SDL_LoadWAV(File,&Wave,                 // Read the wave in
                            &WaveData,&Size) == nullptr)
    {
        fprintf(stderr,"Failed to load %s\n",File);
        SDWERROR();
    }
    SDL_BuildAudioCVT(&Cvt,                     // Prepare to conver it to the current format
                      Wave.format,Wave.channels,Wave.freq,
                      AudioFmt.format,AudioFmt.channels,AudioFmt.freq);

    Cvt.buf = (byte *)malloc(Size * Cvt.len_mult);      // Allocate memory for converted WAV
    if (Cvt.buf == nullptr) SDWERROR();

    memcpy(Cvt.buf,WaveData,Size);              // Copy data in
    Cvt.len = Size;
    SDL_ConvertAudio(&Cvt);                     // Convert it
    SDL_FreeWAV(WaveData);
    SDL_LockAudio();                            // Data into Audio structure
    Data = Cvt.buf;
    Length = Cvt.len_cvt;
    Position = 0;
    SDL_UnlockAudio();
}

//	**************************************************************************************************************************
//
//							                          Create a beep.
//
//	**************************************************************************************************************************

void AudioBeep::CreateBeep(int sPitch,int sLength)
{
    int Size = AudioFmt.freq * sLength / 1000;
    SDWASSERT(AudioFmt.format = AUDIO_S16);     // We must be using 16 bit signed here ?
    Uint16 *Wave = (Uint16 *)malloc(Size*2);    // Allocate a buffer for it
    CreateWave(Wave,Size,sPitch);               // Create the wave
    SDL_LockAudio();                            // Copy data into structure
    Data = Wave;
    Length = Size*2;
    Position = 0;
    SDL_UnlockAudio();
}

void AudioObject::Write(int Pos,int Dat)
{
    if (Pos >= 0 && Pos < Length/2)
    {
        Uint16 *Wave = (Uint16 *)Data;
        Wave[Pos] = Dat;
    }
}

//	**************************************************************************************************************************
//
//                          Wave Creators for beeps and hisses - one square wave, one white noise
//
//	**************************************************************************************************************************

void AudioBeep::CreateWave(void *Data,int Size,int sPitch)
{
    Uint16 *Wave = (Uint16 *)Data;
    int PCount = 0;
    int PValue = 32700;
    int PMax = 0;
    if (sPitch > 0) PMax = AudioFmt.freq / sPitch / 2;
    for (int i = 0;i < Size;i++)
    {
        Wave[i] = (sPitch == 0 ? rand() : PValue);
        if (++PCount == PMax)
        {
            PCount = 0;PValue = -PValue;
        }
    }
}


//	**************************************************************************************************************************
//
//							                    Mixing Callback function
//
//	**************************************************************************************************************************


static void SoundCallBack(void *Data,byte *Stream,int Length)
{
    Data = Data;                                // Stops nagging :)
    for (int i = 0;i < MAXSOUND;i++)            // Work through all the sounds
    {
        if (SoundList[i] != nullptr)               // Call the handler function to copy them.
                    SoundList[i]->CopyStream(Stream,Length);
    }
}

//	**************************************************************************************************************************
//
//							                Initialise Sound System
//
//	**************************************************************************************************************************

static void SoundInitialise()
{
    for (int i = 0;i < MAXSOUND;i++)            // Erase all sound objects
                            SoundList[i] = nullptr;
    AudioFmt.freq = 22050;                      // Frequency of reproduction
    AudioFmt.format = AUDIO_S16;                // 16 bit sound
    AudioFmt.channels = 1;                      // No of channels
    AudioFmt.samples = 1024;                    // Sample length
    AudioFmt.callback = SoundCallBack;       // Callback function
    AudioFmt.userdata = nullptr;                   // Don't use this :)
    if (SDL_OpenAudio(&AudioFmt,nullptr) < 0)      // Open the audio devices
                                    SDWERROR(); // ... or not.
    SDL_PauseAudio(0);                          // Turn the audio system on.
}

//	**************************************************************************************************************************
//
//							                Terminate sound systems
//
//	**************************************************************************************************************************

static void SoundTerminate()
{
        SDL_CloseAudio();                       // Close the audio devices
}

} // namespace Ultima0
} // namespace Ultima
