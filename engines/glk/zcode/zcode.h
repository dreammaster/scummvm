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

#ifndef GLK_ZCODE_ZCODE
#define GLK_ZCODE_ZCODE

#include "glk/glk_api.h"

namespace Glk {
namespace ZCode {

/**
 * ZCode interpreter using Frotz
 */
class ZCode : public GlkAPI {
	friend class FrotzScreen;
public:
	/**
	 * Constructor
	 */
	ZCode(OSystem *syst, const GlkGameDescription &gameDesc);

	/**
	 * Destructor
	 */
	~ZCode() override;

	/**
	 * Returns the running interpreter type
	 */
	InterpreterType getInterpreterType() const override { return INTERPRETER_FROTZ; }

	/**
	 * Execute the game
	 */
	void runGame() override;

	/**
	 * Loading method not used for Frotz sub-engine
	 */
	Common::Error readSaveData(Common::SeekableReadStream *rs) override { return Common::kReadingFailed; }

	/**
	 * Saving method not used for Frotz sub-engine
	 */
	Common::Error writeGameData(Common::WriteStream *ws) override { return Common::kWritingFailed; }
};

extern ZCode *g_vm;

} // End of namespace ZCode
} // End of namespace Glk

#endif
