/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

//=============================================================================
//
// SDL software renderer filter. Technically a non-op, as SDL_Renderer
// does the job.
//
//=============================================================================

#ifndef AGS_ENGINE_GFX_SDL_RENDERER_FILTER_H
#define AGS_ENGINE_GFX_SDL_RENDERER_FILTER_H

#include "ags/shared/gfx/gfxfilter_scaling.h"

namespace AGS3 {
namespace AGS {
namespace Engine {
namespace ALSW {

class SDLRendererGfxFilter : public ScalingGfxFilter {
public:
	virtual const GfxFilterInfo &GetInfo() const;

	static const GfxFilterInfo FilterInfo;
};

} // namespace ALSW
} // namespace Engine
} // namespace AGS
} // namespace AGS3

#endif