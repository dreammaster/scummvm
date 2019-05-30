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

#include "glk/tads/os_glk.h"
#include "glk/tads/tads3/t3std.h"
#include "glk/tads/tads3/vmconsol.h"

namespace Glk {
namespace TADS {
namespace TADS3 {

int CVmFormatter::formatter_more_mode() const {
    return false;
}

int CVmFormatter::get_init_html_target() const {
    return false;
}

void CVmFormatterMain::start_html_in_os() {
}

void CVmFormatterMain::end_html_in_os() {
}

int CVmFormatterMain::get_os_line_wrap() {
    return true;
}

void CVmConsole::show_con_more_prompt(VMG0_) {
    os_more_prompt();
}

} // End of namespace TADS3
} // End of namespace TADS
} // End of namespace Glk
