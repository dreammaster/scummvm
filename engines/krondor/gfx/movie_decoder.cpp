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

#include "graphics/screen.h"
#include "krondor/gfx/movie_decoder.h"
#include "krondor/core/vars.h"

namespace Krondor {
namespace Gfx {


static const unsigned int SAVE_BACKGROUND = 0x0020;
static const unsigned int DRAW_BACKGROUND = 0x0080;
static const unsigned int END_OF_PAGE = 0x0ff0;
static const unsigned int DELAY = 0x1020;
static const unsigned int SLOT_IMAGE = 0x1050;
static const unsigned int SLOT_PALETTE = 0x1060;
static const unsigned int SET_SCENE = 0x1110;
static const unsigned int SET_FRAME0 = 0x2000;
static const unsigned int SET_FRAME1 = 0x2010;
static const unsigned int FADE_OUT = 0x4110;
static const unsigned int FADE_IN = 0x4120;
static const unsigned int DRAW_WINDOW = 0xa100;
static const unsigned int DRAW_SPRITE0 = 0xa500;
static const unsigned int DRAW_SPRITE1 = 0xa510;
static const unsigned int DRAW_SPRITE2 = 0xa520;
static const unsigned int DRAW_SPRITE3 = 0xa530;
static const unsigned int READ_IMAGE = 0xb600;
static const unsigned int LOAD_SOUNDRESOURCE = 0xc020;
static const unsigned int SELECT_SOUND = 0xc030;
static const unsigned int DESELECT_SOUND = 0xc040;
static const unsigned int PLAY_SOUND = 0xc050;
static const unsigned int STOP_SOUND = 0xc060;
static const unsigned int LOAD_SCREEN = 0xf010;
static const unsigned int LOAD_IMAGE = 0xf020;
static const unsigned int LOAD_PALETTE = 0xf050;


bool MovieDecoder::loadFile(const Common::Path &filename) {
	_anim.load(filename.toString());
	_ttm.load(_anim.getAnimationData(1)._resource);
	loadMovie(&_ttm.getMovieTags());

	return true;
}

void MovieDecoder::loadMovie(const Common::Array<MovieTag *> *movie) {
	tagVec = movie;
	_surface.create(SCREEN_WIDTH, SCREEN_HEIGHT);

	_repeat = false;
	_playing = false;
	screenSlot = 0;
	soundSlot = &g_vars->_frpSx;
	Common::fill(imageSlot, imageSlot + MAX_IMAGE_SLOTS, nullptr);
	Common::fill(paletteSlot, paletteSlot + MAX_PALETTE_SLOTS, nullptr);
	backgroundImage = 0;
	backgroundImageDrawn = false;
	savedImage = 0;
	savedImageDrawn = false;
	currFrame = 0;
	currImage = 0;
	currPalette = 0;
	currTag = 0;
	currSound = 0;
	soundMap.clear();
	paletteSlot[currPalette] = new PaletteResource;
	paletteSlot[currPalette]->getActive();
	paletteActivated = false;
}

void MovieDecoder::start() {
	_playing = true;
	_nextFrameTime = 0;
}

void MovieDecoder::stop() {
	_playing = false;
	clear();
}

void MovieDecoder::clear() {
	delete screenSlot;
	screenSlot = nullptr;

	for (uint i = 0; i < MAX_IMAGE_SLOTS; i++)
		delete imageSlot[i];
	Common::fill(imageSlot, imageSlot + MAX_IMAGE_SLOTS, nullptr);

	for (uint i = 0; i < MAX_PALETTE_SLOTS; i++)
		delete paletteSlot[i];
	Common::fill(paletteSlot, paletteSlot + MAX_PALETTE_SLOTS, nullptr);

	delete backgroundImage;
	backgroundImage = nullptr;
	delete savedImage;
	savedImage = nullptr;
}

bool MovieDecoder::needsUpdate() {
	if (_playing && currTag == tagVec->size()) {
		if (looped) {
			currTag = 0;

			delete backgroundImage;
			backgroundImage = 0;
			delete savedImage;
			savedImage = 0;

		} else {
			_playing = false;
		}
	}

	uint32 currTime = g_system->getMillis();
	return _playing && (currTime >= _nextFrameTime);
}


const Graphics::ManagedSurface *MovieDecoder::decodeNextFrame() {
	uint32 currTime = g_system->getMillis();

	while (_playing && currTime >= _nextFrameTime) {
		MovieTag *mt = (*tagVec)[currTag];

		switch (mt->_code) {
		case SAVE_BACKGROUND:
			if (!backgroundImage)
				backgroundImage = new Graphics::ManagedSurface(SCREEN_WIDTH, SCREEN_HEIGHT);

			backgroundImage->blitFrom(_surface);
			break;
		case DRAW_BACKGROUND:
			if (backgroundImage) {
				_surface.blitFrom(*backgroundImage);
				backgroundImageDrawn = true;
			}
			break;
		case END_OF_PAGE:
			if (!paletteActivated) {
				paletteSlot[currPalette]->setActive();
				paletteActivated = true;
			}
			
			backgroundImageDrawn = false;
			savedImageDrawn = false;
			break;
		case DELAY:
			setDelay(mt->_data[0] * 10);
			break;
		case SLOT_IMAGE:
			setDelay(mt->_data[0]);
			break;
		case SLOT_PALETTE:
			currPalette = mt->_data[0];
			paletteActivated = false;
			break;
		case SET_SCENE:
			break;
		case SET_FRAME0:
		case SET_FRAME1:
			currFrame = mt->_data[1];
			break;
#if 0
		case FADE_OUT:
			paletteSlot[currPalette]->GetPalette()->FadeOut(mt->data[0], mt->data[1], 64 << (mt->data[2] & 0x0f), 2 << mt->data[3]);
			media->GetVideo()->Clear();
			paletteActivated = true;
			break;
		case FADE_IN:
			paletteSlot[currPalette]->GetPalette()->FadeIn(mt->data[0], mt->data[1], 64 << (mt->data[2] & 0x0f), 2 << mt->data[3]);
			paletteActivated = true;
			break;
		case DRAW_WINDOW:
			if ((backgroundImage) && (!backgroundImageDrawn))
			{
				backgroundImage->Draw(0, 0);
				backgroundImageDrawn = true;
			}
			if (imageSlot[currImage])
			{
				imageSlot[currImage]->GetImage(currFrame)->Draw(mt->data[0], mt->data[1], 0, 0, mt->data[2], mt->data[3]);
			}
			break;
		case DRAW_SPRITE3:
			if (currDelay > 0)
			{
				media->GetClock()->Delay(currDelay);
			}
		case DRAW_SPRITE2:
			if (currDelay > 0)
			{
				media->GetClock()->Delay(currDelay);
			}
		case DRAW_SPRITE1:
			if (currDelay > 0)
			{
				media->GetClock()->Delay(currDelay);
			}
		case DRAW_SPRITE0:
			if ((backgroundImage) && (!backgroundImageDrawn))
			{
				backgroundImage->Draw(0, 0);
				backgroundImageDrawn = true;
			}
			if ((savedImage) && (!savedImageDrawn))
			{
				savedImage->Draw(0, 0);
				savedImageDrawn = true;
			}
			if (imageSlot[mt->data[3]])
			{
				imageSlot[mt->data[3]]->GetImage(mt->data[2])->Draw(mt->data[0], mt->data[1], 0);
			}
			break;
		case READ_IMAGE:
			if (savedImage)
			{
				delete savedImage;
			}
			savedImage = new Image(mt->data[2], mt->data[3]);
			savedImage->Read(mt->data[0], mt->data[1]);
			savedImageDrawn = false;
			break;
		case LOAD_SOUNDRESOURCE:
			break;
		case SELECT_SOUND:
		{
			std::map<unsigned int, int>::iterator it = soundMap.find(mt->data[0]);
			if (it != soundMap.end())
			{
				if (it->second >= 0)
				{
					media->GetAudio()->StopSound(it->second);
				}
				soundMap.erase(it);
			}
			soundMap.insert(std::pair<unsigned int, int>(mt->data[0], -1));
		}
		break;
		case DESELECT_SOUND:
		{
			std::map<unsigned int, int>::iterator it = soundMap.find(mt->data[0]);
			if (it != soundMap.end())
			{
				if (it->second >= 0)
				{
					media->GetAudio()->StopSound(it->second);
				}
				soundMap.erase(it);
			}
		}
		break;
		case PLAY_SOUND:
		{
			std::map<unsigned int, int>::iterator it = soundMap.find(mt->data[0]);
			if (it != soundMap.end())
			{
				if (it->second >= 0)
				{
					media->GetAudio()->StopSound(it->second);
				}
				SoundData data = soundSlot->GetSoundData(it->first);
				it->second = media->GetAudio()->PlaySound(data.sounds[0]->GetSamples());
			}
		}
		break;
		case STOP_SOUND:
		{
			std::map<unsigned int, int>::iterator it = soundMap.find(mt->data[0]);
			if (it != soundMap.end())
			{
				if (it->second >= 0)
				{
					media->GetAudio()->StopSound(it->second);
				}
				soundMap.erase(it);
			}
		}
		break;
		case LOAD_SCREEN:
			if (screenSlot)
			{
				delete screenSlot;
			}
			mt->name[mt->name.length() - 1] = 'X';
			screenSlot = new ScreenResource;
			FileManager::GetInstance()->Load(screenSlot, mt->name);
			screenSlot->GetImage()->Draw(0, 0);
			break;
		case LOAD_IMAGE:
			if (imageSlot[currImage])
			{
				delete imageSlot[currImage];
			}
			mt->name[mt->name.length() - 1] = 'X';
			imageSlot[currImage] = new ImageResource;
			FileManager::GetInstance()->Load(imageSlot[currImage], mt->name);
			break;
		case LOAD_PALETTE:
			if (paletteSlot[currPalette])
			{
				delete paletteSlot[currPalette];
			}
			paletteSlot[currPalette] = new PaletteResource;
			FileManager::GetInstance()->Load(paletteSlot[currPalette], mt->name);
			paletteActivated = false;
			break;
#endif
		default:
			break;
		}

		currTag++;
		if (currTag == tagVec->size())
		{
			if (looped)
			{
				currTag = 0;
				if (backgroundImage)
				{
					delete backgroundImage;
					backgroundImage = 0;
				}
				if (savedImage)
				{
					delete savedImage;
					savedImage = 0;
				}
			} else
			{
				_playing = false;
			}
		}
	}

	return &_surface;
}

void MovieDecoder::setDelay(uint time) {
	_nextFrameTime = g_system->getMillis() + time;
}

} // namespace Gfx
} // namespace Krondor
