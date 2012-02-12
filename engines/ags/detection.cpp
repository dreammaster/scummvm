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

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

/* Based on the Adventure Game Studio source code, copyright 1999-2011 Chris Jones,
 * which is licensed under the Artistic License 2.0.
 */

#include "base/plugins.h"
#include "engines/advancedDetector.h"
#include "ags/detection_tables.h"

class AGSMetaEngineDetection : public AdvancedMetaEngineDetection {
public:
	AGSMetaEngineDetection() : AdvancedMetaEngineDetection(AGS::GAME_DESCRIPTIONS, sizeof(AGS::AGSGameDescription), AGS::GAMES) {
		_maxScanDepth = 3;
	}

	const char *getEngineId() const override {
		return "ags";
	}

	const char *getName() const override {
		return "AGS";
	}

	const char *getOriginalCopyright() const override {
		return "Copyright (C) Chris Jones";;
	}
};


REGISTER_PLUGIN_STATIC(AGS_DETECTION, PLUGIN_TYPE_ENGINE_DETECTION, AGSMetaEngineDetection);
