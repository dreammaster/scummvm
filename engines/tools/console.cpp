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

#include "common/config-manager.h"
#include "common/file.h"
#include "common/fs.h"
#include "common/textconsole.h"
#include "tools/console.h"

namespace Tools {

static void addHeaders(const Common::String &namespaceStr, const Common::String &fname) {
	Common::File src;
	Common::DumpFile dest;
	Common::StringArray lines;

	if (!src.open(fname))
		error("Could not open for reading - %s", fname.c_str());
	if (!dest.open(fname))
		error("Could not open for writing - %s", fname.c_str());

	while (!src.eos())
		lines.push_back(src.readLine());

	// Remove any initial comment block (i.e. old GPL headers)
	while (!lines.empty() && lines[0].empty())
		lines.remove_at(0);
	if (!lines.empty() && lines[0].hasPrefix("/*")) {
		Common::String l;
		do {
			l = lines[0];
			l.trim();
			lines.remove_at(0);
		} while (!lines.empty() && !l.hasSuffix("*/"));
	}
	while (!lines.empty() && (lines[0].empty() || lines[0].hasPrefix("//")))
		lines.remove_at(0);

	// Add in namespace. The file is scanned to find the last #include
	// line, and the namespace opening is put after that
	int idx = lines.size() - 1;
	while (idx > 0 && !lines[idx].hasPrefixIgnoreCase("#include"))
		--idx;
	lines.insert_at(idx + 1, Common::String::format(
		"namespace %s {", namespaceStr.c_str()));
	lines.insert_at(idx + 1, "");
	lines.push_back(Common::String::format("} // namespace %s", namespaceStr.c_str()));

	// In any #include line, add in a folder prefix, which is presumed
	// to be the lowercase of the namespace
	Common::String folderName = namespaceStr;
	folderName.toLowercase();
	for (int i = 0; i <= idx; ++i) {
		if (lines[i].hasPrefixIgnoreCase("#include \""))
			lines[i] = Common::String::format("#include \"%s/%s",
				folderName.c_str(), lines[i].c_str() + 10);
	}


	// Add in GPL header
	const char *const LINES[20] = {
		"/* ScummVM - Graphic Adventure Engine",
		" *",
		" * ScummVM is the legal property of its developers, whose names",
		" * are too numerous to list here. Please refer to the COPYRIGHT",
		" * file distributed with this source distribution.",
		" *",
		" * This program is free software: you can redistribute it and/or modify",
		" * it under the terms of the GNU General Public License as published by",
		" * the Free Software Foundation, either version 3 of the License, or",
		" * (at your option) any later version.",
		" *",
		" * This program is distributed in the hope that it will be useful,",
		" * but WITHOUT ANY WARRANTY; without even the implied warranty of",
		" * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the",
		" * GNU General Public License for more details.",
		" *",
		" * You should have received a copy of the GNU General Public License",
		" * along with this program.  If not, see <http://www.gnu.org/licenses/>.",
		" *",
		" */"
	};
	for (int i = 0; i < 20; ++i)
		lines.insert_at(i, LINES[i]);

	// Finally write out file
	for (uint i = 0; i < lines.size(); ++i) {
		dest.writeString(lines[i]);
		dest.writeByte('\n');
	}

	src.close();
	dest.close();
}

Console::Console() : GUI::Debugger() {
	registerCmd("headers",   WRAP_METHOD(Console, Cmd_headers));
}

Console::~Console() {
}

bool Console::Cmd_headers(int argc, const char **argv) {
	Common::FSNode folder(ConfMan.get("path"));
	Common::FSList fslist;
	const char *ns = (argc > 1) ? argv[1] : "Namespace1";

	if (folder.getChildren(fslist, Common::FSNode::kListFilesOnly)) {
		for (uint i = 0; i < fslist.size(); ++i) {
			const Common::String fname = fslist[i].getName();
			if (fname.hasSuffixIgnoreCase(".c") ||
					fname.hasSuffixIgnoreCase(".cpp") ||
					fname.hasSuffixIgnoreCase(".h"))
				debugPrintf("Processing %s..\n", fname.c_str());
				addHeaders(ns, fname);
		}

		debugPrintf("Done\n");
	} else {
		debugPrintf("Error\n");
	}

	return true;
}

} // End of namespace Tools
