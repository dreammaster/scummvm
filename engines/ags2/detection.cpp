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

#include "base/plugins.h"
#include "common/config-manager.h"
#include "common/file.h"
#include "common/md5.h"
#include "common/str-array.h"
#include "common/util.h"
#include "common/punycode.h"
#include "ags2/detection.h"
#include "ags2/detection_tables.h"

namespace AGS2 {

static const char *const HEAD_SIG = "CLIB\x1a";
static const char *const TAIL_SIG = "CLIB\x1\x2\x3\x4SIGE";
#define HEAD_SIG_SIZE 5
#define TAIL_SIG_SIZE 12

/**
 * Detect the presence of an AGS game
 * TODO: This is a compact version of MFLUtil::ReadSigsAndVersion. I didn't
 * use the full version due to the complexities of including it when
 * plugins are enabled. In the future, though, it would be nice to figure
 * out, since the full version can handle not detecting on files that are
 * AGS, but only contain sounds, etc. rather than a game
 */
static bool isAGSFile(Common::File &f) {
	// Check for signature at beginning of file
	char buffer[16];
	if (f.read(buffer, HEAD_SIG_SIZE) == HEAD_SIG_SIZE &&
	        !memcmp(buffer, HEAD_SIG, HEAD_SIG_SIZE))
		return true;

	// Check for signature at end of EXE files
	f.seek(-TAIL_SIG_SIZE, SEEK_END);
	if (f.read(buffer, TAIL_SIG_SIZE) == TAIL_SIG_SIZE &&
	        !memcmp(buffer, TAIL_SIG, TAIL_SIG_SIZE))
		return true;

	return false;
}

} // namespace AGS2

const DebugChannelDef AGS2MetaEngineDetection::debugFlagList[] = {
	{AGS2::kDebugGraphics, "Graphics", "Graphics debug level"},
	{AGS2::kDebugPath, "Path", "Pathfinding debug level"},
	{AGS2::kDebugFilePath, "FilePath", "File path debug level"},
	{AGS2::kDebugScan, "Scan", "Scan for unrecognised games"},
	{AGS2::kDebugScript, "Script", "Enable debug script dump"},
	DEBUG_CHANNEL_END
};

AGS2MetaEngineDetection::AGS2MetaEngineDetection() : AdvancedMetaEngineDetection(AGS2::GAME_DESCRIPTIONS, AGS2::GAME_NAMES) {
	_flags = kADFlagCanPlayUnknownVariants;
}

DetectedGames AGS2MetaEngineDetection::detectGames(const Common::FSList &fslist, uint32 skipADFlags, bool skipIncomplete) {
	FileMap allFiles;

	if (fslist.empty())
		return DetectedGames();

	// Compose a hashmap of all files in fslist.
	composeFileHashMap(allFiles, fslist, (_maxScanDepth == 0 ? 1 : _maxScanDepth));

	// Run the detector on this
	ADDetectedGames matches = detectGame(fslist.begin()->getParent(), allFiles, Common::UNK_LANG, Common::kPlatformUnknown, "", skipADFlags, skipIncomplete);

	cleanupPirated(matches);

	bool foundKnownGames = false;
	DetectedGames detectedGames;
	for (uint i = 0; i < matches.size(); i++) {
		DetectedGame game = toDetectedGame(matches[i]);
		if (game.hasUnknownFiles) {
			// Check the game is an AGS game
			for (FilePropertiesMap::const_iterator it = game.matchedFiles.begin(); it != game.matchedFiles.end(); it++) {
				Common::File f;
				if (f.open(allFiles[it->_key]) && AGS2::isAGSFile(f)) {
					detectedGames.push_back(game);
					break;
				}
			}
		} else {
			detectedGames.push_back(game);
			foundKnownGames = true;
		}
	}

	return detectedGames;
}

REGISTER_PLUGIN_STATIC(AGS2_DETECTION, PLUGIN_TYPE_ENGINE_DETECTION, AGS2MetaEngineDetection);
