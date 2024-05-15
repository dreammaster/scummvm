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

#ifndef KRONDOR_GFX_MOVIE_DECODER_H
#define KRONDOR_GFX_MOVIE_DECODER_H

#include "video/video_decoder.h"
#include "krondor/res/animation_resource.h"
#include "krondor/res/image_resource.h"
#include "krondor/res/movie_resource.h"
#include "krondor/res/palette_resource.h"
#include "krondor/res/screen_resource.h"
#include "krondor/res/sound_resource.h"

namespace Krondor {
namespace Gfx {

static const uint MAX_IMAGE_SLOTS = 4;
static const uint MAX_PALETTE_SLOTS = 4;

class MovieDecoder : public Video::VideoDecoder {
private:
	AnimationResource _anim;
	MovieResource _ttm;
	bool _repeat = false;

private:
	Common::Array<MovieTag *> *tagVec = nullptr;
	ScreenResource *screenSlot = nullptr;
	SoundResource *soundSlot = nullptr;
	ImageResource *imageSlot[MAX_IMAGE_SLOTS];
	PaletteResource *paletteSlot[MAX_PALETTE_SLOTS];
	Graphics::ManagedSurface *backgroundImage = nullptr;
	bool backgroundImageDrawn = false;
	Graphics::ManagedSurface *savedImage = nullptr;
	bool savedImageDrawn = false;
	uint currFrame = 0;
	uint currImage = 0;
	uint currPalette = 0;
	uint currTag = 0;
	uint currDelay = 0;
	uint currSound = 0;
	Common::HashMap<uint, int> soundMap;
	bool paletteActivated = false;
	bool playing = false;
	bool looped = false;
	bool delayed = false;

	void loadMovie(const Common::Array<MovieTag *> *movie);
	//void playTag(MediaToolkit *media);

public:
	bool loadFile(const Common::Path &filename) override;
	bool loadStream(Common::SeekableReadStream *stream) override;

	bool isRepeat() const {
		return _repeat;
	}
	void setRepeat(bool repeat) {
		_repeat = repeat;
	}
};

} // namespace Gfx
} // namespace Krondor

#endif
