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

#ifndef AESOP_SYSTEM_AIL32_H
#define AESOP_SYSTEM_AIL32_H

#include "common/scummsys.h"
#include "aesop/defs.h"

namespace Aesop {

#define PHYSICAL 8

#define SEQ_DONE 1
//define PCM_IO 1
//define PCM_IRQ 2

typedef int HDRIVER;
typedef int HSEQUENCE;

struct drvr_desc {
	ULONG default_IO = 0;
	ULONG default_IRQ = 0;
	ULONG default_DMA = 0;
	ULONG default_DRQ = 0;
	const char *data_suffix = ".dat";
};

extern HDRIVER AIL_register_driver(void *driver_base_addr);
extern void AIL_shutdown_driver(HDRIVER hXMI, const char *);
extern void AIL_release_driver_handle(HDRIVER hXMI);
extern ULONG AIL_detect_device(HDRIVER driver, ULONG IO_addr,
	ULONG IRQ, ULONG DMA, ULONG DRQ);
extern drvr_desc *AIL_describe_driver(HDRIVER driver);
extern void AIL_init_driver(HDRIVER driver, ULONG IO_addr,
	ULONG IRQ, ULONG DMA, ULONG DRQ);
extern ULONG AIL_state_table_size(HDRIVER driver);
extern ULONG AIL_default_timbre_cache_size(HDRIVER driver);
extern void AIL_define_timbre_cache(HDRIVER driver, void *cache_addr,
	ULONG cache_size);


extern bool PhysicalState(int channel);
extern void SetChannel(WORD blk, UWORD offset, UWORD size, WORD channel, WORD one);
extern void SetActive(WORD channel1, WORD channel2);
extern void ChannelOn(WORD channel);
extern int AIL_sequence_status(HDRIVER hXMI, HSEQUENCE hSEQ);
extern void AIL_stop_sequence(HDRIVER hXMI, HSEQUENCE hSEQ);
extern void AIL_release_sequence_handle(HDRIVER hXMI, HSEQUENCE hSEQ);
extern void InActive(WORD channel);
extern HSEQUENCE AIL_register_sequence(HDRIVER driver, void *FORM_XMID,
	ULONG sequence_num, void *state_table, void *controller_table);
extern ULONG AIL_timbre_request(HDRIVER driver, HSEQUENCE sequence);
extern void AIL_install_timbre(HDRIVER driver, int bank, int patch,
	void *src_addr);
extern void AIL_start_sequence(HDRIVER driver, HSEQUENCE sequence);

extern HRES StartMod(HDRIVER hPCM, void *sndwrk, void *pcmStorage);
extern void PollMod();
extern void StopMod();
extern ULONG ModSizeNeeded();

} // namespace Aesop

#endif
