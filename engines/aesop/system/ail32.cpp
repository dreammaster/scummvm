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

#include "aesop/system/ail32.h"

namespace Aesop {

drvr_desc driverDesc;

HDRIVER AIL_register_driver(void *driver_base_addr) {
	return (HDRIVER)0;
}

void AIL_shutdown_driver(HDRIVER hXMI, int) {
}

void AIL_release_driver_handle(HDRIVER hXMI) {
}

ULONG AIL_detect_device(HDRIVER driver, ULONG IO_addr,
		ULONG IRQ, ULONG DMA, ULONG DRQ) {
	return 0;
}

drvr_desc *AIL_describe_driver(HDRIVER driver) {
	return &driverDesc;
}

void AIL_init_driver(HDRIVER driver, ULONG IO_addr,
		ULONG IRQ, ULONG DMA, ULONG DRQ) {
}

ULONG AIL_state_table_size(HDRIVER driver) {
	return 1;
}

ULONG AIL_default_timbre_cache_size(HDRIVER driver) {
	return 1;
}

void AIL_define_timbre_cache(HDRIVER driver, void *cache_addr,
	ULONG cache_size) {
}


bool PhysicalState(int channel) {
	return false;
}

void SetChannel(WORD blk, UWORD offset, UWORD size, int channel, int one) {
}

void SetActive(WORD channel1, WORD channel2) {
}

void ChannelOn(WORD channel) {
}

int AIL_sequence_status(HDRIVER hXMI, HSEQUENCE hSEQ) {
	return SEQ_DONE;
}

void AIL_stop_sequence(HDRIVER hXMI, HSEQUENCE hSEQ) {
}

void AIL_release_sequence_handle(HDRIVER hXMI, HSEQUENCE hSEQ) {
}

void InActive(WORD channel) {
}

HSEQUENCE AIL_register_sequence(HDRIVER driver, void *FORM_XMID,
		ULONG sequence_num, void *state_table, void *controller_table) {
	return 0;
}

ULONG AIL_timbre_request(HDRIVER driver, HSEQUENCE sequence) {
	return 0;
}

void AIL_install_timbre(HDRIVER driver, int bank, int patch,
	void *src_addr) {
}

void AIL_start_sequence(HDRIVER driver, HSEQUENCE sequence) {
}

HRES StartMod(HDRIVER hPCM, void *sndwrk, void *pcmStorage) {
	return 1;
}

void PollMod() {
}

void StopMod() {
}

ULONG ModSizeNeeded() {
	return 1;
}

} // namespace Aesop
