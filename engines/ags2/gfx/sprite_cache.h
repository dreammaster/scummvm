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

#ifndef AGS2_GFX_SPRITE_CACHE_H
#define AGS2_GFX_SPRITE_CACHE_H

#include "common/file.h"
#include "ags2/common/wgt2allg.h"

namespace AGS2 {

// We can't rely on offsets[slot]==0 because when the engine is running
// this is changed to reference the Bluecup sprite. Therefore we need
// a definite way of knowing whether the sprite existed in the sprite file.
#define SPRCACHEFLAG_DOESNOTEXIST 1

class SpriteCache {
public:
	SpriteCache(long maxElements);

	int  initFile(const char *);
	int  loadSprite(int);
	void seekToSprite(int index);
	void precache(int);				// preloads and locks in memory
	void set(int, block);
	void setNonDiscardable(int, block);
	void removeSprite(int, bool);
	void removeOldest();
	void reset();					// wipes all data 
	void init();
	void changeMaxSize(long);
	int  enlargeTo(long);
	void removeAll();				// removes all items from the cache
	int  findFreeSlot();
	int  saveToFile(const char *, int lastElement, bool compressOutput);
	int  doesSpriteExist(int index);
	void detachFile();
	int  attachFile(const char *);

	block operator[] (int index);

	long *offsets = nullptr;
	long sprite0InitialOffset = 0;
	long elements = 0;				// size of offsets/images arrays
	block *images = nullptr;
	int *sizes = nullptr;
	byte *flags = nullptr;
	Common::SeekableReadStream *ff = nullptr;
	bool spritesAreCompressed = false;
	long cachesize = 0;				// size in bytes of currently cached images
	int *mrulist = nullptr, *mrubacklink = nullptr;
	int liststart = 0, listend = 0;
	int lastLoad = 0;
	long maxCacheSize = 0;
	long lockedSize = 0;			// size in bytes of currently locked images

private:
	void compressSprite(block sprite, Common::WriteStream *ooo);
	bool loadSpriteIndexFile(int expectedFileID, long spr_initial_offs, short numspri);
};

} // namespace AGS2

#endif
