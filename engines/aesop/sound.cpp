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

#include "common/textconsole.h"
#include "aesop/system/vfx.h"
#include "aesop/files.h"
#include "aesop/mouse.h"
#include "aesop/gil2vfx.h"
#include "aesop/gil2vfxa.h"
#include "aesop/defs.h"
#include "aesop/rtsystem.h"
#include "aesop/rtmsg.h"
#include "aesop/rtres.h"
#include "aesop/rtlink.h"
#include "aesop/rt.h"
#include "aesop/rtmsg.h"
#include "aesop/sound.h"
#include "aesop/graphics.h"
#include "aesop/system/ail32.h"
//#include "dll.h"

namespace Aesop {

#define CFG_FN  "SOUND.CFG"
#define GTL_PFX "STDPATCH."

#define ROLAND_DRV_NAME "A32MT32.DLL"
#define PCSPKR_DRV_NAME "A32SPKR.DLL"
#define ADLIB_DRV_NAME  "A32ADLIB.DLL"
#define SBDIG_DRV_NAME  "A32SBDG.DLL"

#define EMSHCNT     15            // 15 EMS handles for 64K sound blocks
#define XMI_BUFSIZE 2048          // Size of reserved XMIDI sequence buffer

#define XMID_LA 0                 // LAPC-1/MT-32 in use
#define XMID_AD 1                 // Ad Lib in use
#define XMID_PC 2                 // PC speaker in use

UBYTE *PCM_storage;               // "Simulated EMS" memory for flat-model SFX

WORD XMI_device_type;             // _LA, _AD, or _PC

WORD PCM_active = 0;
WORD XMI_active = 0;

WORD sound_on;
WORD music_resident;

drvr_desc XMI_desc;

Common::Stream *gtl = nullptr;

HDRIVER hXMI, hPCM;
HSEQUENCE hSEQ;

BYTE XMI_fn[32];

void *XMI_driver;
HRES hXMI_buffer;
HRES hXMI_state;
HRES hXMI_cache;

UWORD EMS_offset[EMSHCNT];        // First free byte in each 64K block

WORD  SND_blk[64];                // EMS block of sound effect at index #
UWORD SND_off[64];                // EMS offset of sound effect at index #
UWORD SND_size[64];               // Size of sound effect at index #

// SSI MEL sound system config file
struct {
	WORD XMI_IO;
	WORD XMI_IRQ;
	WORD XMI_DMA;
	WORD XMI_DRQ;
	WORD XMI_CARDTYPE;

	WORD PCM_IO;
	WORD PCM_IRQ;
	WORD PCM_DMA;
	WORD PCM_DRQ;
	WORD PCM_CARDTYPE;

	WORD PCM_ENABLED;

	BYTE XMI_fn[14];
	BYTE PCM_fn[14];

	BYTE dummy[32];
} MEL;

/****************************************************************************/
//
// Load a .DLL sound driver
//
/****************************************************************************/

static void *load_driver(BYTE *filename) {
	return nullptr;
#if 0
	void *DLL, *drvr;

	DLL = FILE_read(filename, NULL);

	if (DLL == NULL)
		return NULL;

	drvr = DLL_load(DLL, DLLMEM_ALLOC | DLLSRC_MEM, NULL);

	free(DLL);

	if (drvr == NULL)
		return NULL;

	return drvr;
#endif
}

/****************************************************************************/
//
// Standard routine for Global Timbre Library access
//
/****************************************************************************/

static void *load_global_timbre(ULONG bank, ULONG patch) {
	UWORD *timb_ptr;
	static UWORD len;

	// gtl file header entry structure
	static struct {
		BYTE patch;
		BYTE bank;
		ULONG offset;
	} GTL_hdr;

	if (!gtl)
		return NULL;      // if no gtl, return failure

	lseek(gtl, 0, SEEK_SET);         // else rewind to gtl header

	// search file for requested timbre
	do {
		read(gtl, &GTL_hdr, sizeof(GTL_hdr));
		if (GTL_hdr.bank == -1)
			return NULL;             // timbre not found, return NULL
	} while (((ULONG)GTL_hdr.bank != bank) ||
		((ULONG)GTL_hdr.patch != patch));

	lseek(gtl, GTL_hdr.offset, SEEK_SET);
	read(gtl, &len, 2);              // timbre found, read its length

	timb_ptr = (UWORD *)mem_alloc(len);     // allocate memory for timbre ..
	*timb_ptr = len;
	// and load it
	read(gtl, (timb_ptr + 1), len - 2);

	return timb_ptr;               // else return pointer to timbre
}


/****************************************************************************/
//
// Load a block of sound resources into the EMS range specified as starting
// and ending block handles (0-14)
//
// In practice, the valid block handles are:
//
// 0: COMMON sounds take blocks 0-8
// 9: LEVEL sounds can use blocks 9-15
//
// *array -> list of sound resource names to be loaded, terminated with a 
// null entry
//
// This function must not perform any resource cache manipulation, since
// it maintains a pointer to an array in movable memory!
//
/****************************************************************************/

#pragma off (unreferenced)
void load_sound_block(LONG argcnt, ULONG first_block, ULONG last_block, ULONG *array)
#pragma on (unreferenced)
{
	ULONG index;
	ULONG i, cur;
	ULONG size;
	ULONG end;
	ULONG res;

	if (!PCM_active)
		return;

	index = (first_block == BLK_COMMON) ? FIRST_COMMON : FIRST_LEVEL;

	for (i = first_block; i <= last_block; i++)
		EMS_offset[i] = 0;

	for (i = 0; (res = array[i]) != 0L; i++)
	{
		size = RTR_seek(RTR, res);
		if (size == 0L)
			abend(MSG_SRNF);

		for (cur = first_block; cur <= last_block; cur++)
		{
			end = (ULONG)EMS_offset[cur] + size - 1L;

			if (end < 65520L)
				break;
		}

		if (cur > last_block)
			abend(MSG_OOSSE);

		SND_blk[index] = cur;               // EMS_handle[cur];
		SND_off[index] = EMS_offset[cur];
		SND_size[index] = (UWORD)size;

		RTR_read_resource(RTR,
			PCM_storage + EMS_offset[cur] + (cur * 65536),
			size);

		EMS_offset[cur] += (UWORD)((size + 15L) & ~15L);

		index++;
	}
}

/****************************************************************************/
//
// Request a sound effect from the COMMON or LEVEL bank
//
/****************************************************************************/

#pragma off (unreferenced)
void sound_effect(LONG argcnt, ULONG index)
#pragma on (unreferenced)
{
	WORD ch;

	if (!PCM_active) return;
	if (!sound_on) return;

	for (ch = 0; ch < PHYSICAL; ch++)
		if (!PhysicalState(ch)) break;

	if (ch == PHYSICAL) return;

	SetChannel(SND_blk[index], SND_off[index], SND_size[index], ch, 1);
	SetActive(ch, ch);
	ChannelOn(ch);
}

/****************************************************************************/
//
// Play an XMIDI sequence, stopping any currently active sequence first
//
/****************************************************************************/

#pragma off (unreferenced)
void play_sequence(LONG argcnt, ULONG LA_version, ULONG AD_version, ULONG PC_version)
#pragma on (unreferenced)
{
	ULONG XMI_res;
	ULONG size;
	ULONG bank, patch, treq;
	void *timb;

	if (!XMI_active) return;
	if (!music_resident) return;
	if (!sound_on) return;

	switch (XMI_device_type)
	{
	case XMID_LA: XMI_res = LA_version; break;
	case XMID_PC: XMI_res = PC_version; break;
	default:      XMI_res = AD_version; break;
	}

	if (hSEQ != -1)
	{
		if (AIL_sequence_status(hXMI, hSEQ) != SEQ_DONE)
			AIL_stop_sequence(hXMI, hSEQ);

		AIL_release_sequence_handle(hXMI, hSEQ);
	}

	size = RTR_seek(RTR, XMI_res);
	RTR_read_resource(RTR, RTR_addr(hXMI_buffer), size);

	hSEQ = AIL_register_sequence(hXMI, RTR_addr(hXMI_buffer), 0,
		RTR_addr(hXMI_state), NULL);

	while ((treq = AIL_timbre_request(hXMI, hSEQ)) != (ULONG)-1)
	{
		bank = treq / 256; patch = treq % 256;

		timb = load_global_timbre(bank, patch);
		if (timb != NULL)
		{
			AIL_install_timbre(hXMI, bank, patch, timb);
			mem_free(timb);
		} else
			abend(MSG_TPNF, bank, patch);
	}

	AIL_start_sequence(hXMI, hSEQ);
}

/****************************************************************************/
//
// Load and initialize music system in preparation for sequence playback
//
/****************************************************************************/

void load_music(void) {
	LONG tsize;

	if ((!XMI_active) || (!sound_on) || (music_resident)) return;

	XMI_driver = load_driver(XMI_fn);

	hXMI = AIL_register_driver(XMI_driver);

	if (!AIL_detect_device(hXMI, XMI_desc.default_IO, XMI_desc.default_IRQ,
		XMI_desc.default_DMA, XMI_desc.default_DRQ))
	{
		mem_free(XMI_driver);
		return;
	}

	AIL_init_driver(hXMI, XMI_desc.default_IO, XMI_desc.default_IRQ,
		XMI_desc.default_DMA, XMI_desc.default_DRQ);

	hXMI_state = RTR_alloc(RTR, AIL_state_table_size(hXMI),
		DA_FIXED | DA_PRECIOUS);

	hXMI_buffer = RTR_alloc(RTR, XMI_BUFSIZE, DA_FIXED | DA_PRECIOUS);

	hXMI_cache = (HRES)-1;
	tsize = AIL_default_timbre_cache_size(hXMI);

	if (tsize)
	{
		hXMI_cache = RTR_alloc(RTR, tsize, DA_FIXED | DA_PRECIOUS);
		AIL_define_timbre_cache(hXMI, RTR_addr(hXMI_cache), (UWORD)tsize);
	}

	hSEQ = -1;
	music_resident = 1;
}

/****************************************************************************/
//
// Shut down and unload music system to conserve heap resources during game
//
/****************************************************************************/

void unload_music(void) {
	LONG i;

	if ((!XMI_active) || (!music_resident)) return;

	if (hSEQ != -1)
	{
		if (AIL_sequence_status(hXMI, hSEQ) != SEQ_DONE)
		{
			AIL_stop_sequence(hXMI, hSEQ);

			if (XMI_device_type == XMID_LA)
				for (i = 0; i < 60; i++)
					VFX_wait_vblank_leading();
		}

		AIL_release_sequence_handle(hXMI, hSEQ);
		hSEQ = -1;
	}

	AIL_shutdown_driver(hXMI, MSG_AIL);
	AIL_release_driver_handle(hXMI);

	if (hXMI_cache != (ULONG)-1)
		RTR_free(RTR, hXMI_cache);

	RTR_free(RTR, hXMI_buffer);
	RTR_free(RTR, hXMI_state);
	mem_free(XMI_driver);

	music_resident = 0;
}

/****************************************************************************/
//
// Turn sound effects & music on/off
//
/****************************************************************************/

#pragma off (unreferenced)
void set_sound_status(LONG argcnt, ULONG status)
#pragma on (unreferenced)
{
	if (!(PCM_active || XMI_active)) return;

	if (status)
		sound_on = 1;
	else
	{
		if ((XMI_active) && (music_resident))
			if (hSEQ != -1)
			{
				if (AIL_sequence_status(hXMI, hSEQ) != SEQ_DONE)
					AIL_stop_sequence(hXMI, hSEQ);

				AIL_release_sequence_handle(hXMI, hSEQ);
				hSEQ = -1;
			}

		if (PCM_active)
		{
			InActive(0);
			InActive(1);
			InActive(2);
			InActive(3);
		}

		sound_on = 0;
	}
}

/****************************************************************************/
//
// Shut down audio resources and release all EMS memory used
// 
// Note: Does not release resource cache blocks used for sound drivers, etc.
// 
/****************************************************************************/

void shutdown_sound(void) {
	if (!(PCM_active || XMI_active)) return;

	if (PCM_active)
	{
		StopMod();

		AIL_shutdown_driver(hPCM, MSG_AIL);
		AIL_release_driver_handle(hPCM);
	}

	if (XMI_active)
	{
		if (music_resident)
		{
			AIL_shutdown_driver(hXMI, MSG_AIL);
			AIL_release_driver_handle(hXMI);
		}

		if (gtl)
			close(gtl);
	}

	PCM_active = XMI_active = music_resident = 0;
}

/****************************************************************************/
//
// Initialize audio resources
//
// Load requested drivers (specified in config file) and set global flags
// to indicate presence of PCM / XMI sound
//
// This routine uses debug(1, ) to report nonfatal errors, so it should be 
// called before the graphics system is initialized unless the errprompt
// argument is set to 0 to inhibit informational messages
//
/****************************************************************************/

#pragma off (unreferenced)
void init_sound(LONG argcnt, ULONG errprompt)
#pragma on (unreferenced)
{
	WORD PCM_requested, XMI_requested;
	BYTE PCM_fn[32];
	BYTE GTL_fn[32];
	WORD PCM_IO = 0;
	WORD PCM_IRQ = 0;
	WORD XMI_IO = 0;
	BYTE *PCMdrvr;
	drvr_desc *desc;
	void *sndwrk;

	if (PCM_active || XMI_active) return;

	PCM_requested = 0;
	XMI_requested = 0;
	music_resident = 0;
	gtl = nullptr;

	if (FILE_read(CFG_FN, &MEL) == NULL)
	{
		if (errprompt)
			error(MSG_NO_CFG);

		return;
	}

	MEL.XMI_fn[13] = 0;
	MEL.PCM_fn[13] = 0;

	if (!scumm_strnicmp(MEL.PCM_fn, "SB", 2))
		Common::strcpy_s(MEL.PCM_fn, SBDIG_DRV_NAME);

	if (!scumm_stricmp(MEL.XMI_fn, "ADLIB.ADV"))
		Common::strcpy_s(MEL.XMI_fn, ADLIB_DRV_NAME);
	else
		if (!scumm_stricmp(MEL.XMI_fn, "MT32MPU.ADV"))
			Common::strcpy_s(MEL.XMI_fn, ROLAND_DRV_NAME);
		else
			Common::strcpy_s(MEL.XMI_fn, PCSPKR_DRV_NAME);

	if (MEL.XMI_CARDTYPE != 113)
	{
		Common::strcpy_s(XMI_fn, MEL.XMI_fn);
		XMI_IO = MEL.XMI_IO;
		XMI_requested = 1;
	}

	if ((MEL.PCM_CARDTYPE != 113) && (MEL.PCM_ENABLED))
	{
		Common::strcpy_s(PCM_fn, MEL.PCM_fn);
		PCM_IO = MEL.PCM_IO;
		PCM_IRQ = MEL.PCM_IRQ;
		PCM_requested = 1;
	}

	if (PCM_requested)
	{
		if ((PCM_storage = (UBYTE *)malloc(15 * 65536)) == NULL)      // memory avail?
		{
			if (errprompt)
				debug(1, MSG_NO_EMS);
		} else
		{
			if ((PCMdrvr = (BYTE *)load_driver(PCM_fn)) != NULL)
			{
				hPCM = AIL_register_driver(PCMdrvr);
				desc = AIL_describe_driver(hPCM);

				desc->default_IO = PCM_IO;
				desc->default_IRQ = PCM_IRQ;

				if (AIL_detect_device(hPCM, desc->default_IO, desc->default_IRQ,
					desc->default_DMA, desc->default_DRQ))
				{
					AIL_init_driver(hPCM, desc->default_IO, desc->default_IRQ,
						desc->default_DMA, desc->default_DRQ);

					sndwrk = RTR_addr(RTR_alloc(RTR, ModSizeNeeded(),
						DA_FIXED | DA_PRECIOUS));

					if (StartMod(hPCM, sndwrk, PCM_storage) != -1)
					{
						InActive(0);
						InActive(1);
						InActive(2);
						InActive(3);

						PCM_active = 1;
					}
				}
			}
		}
	}

	if (XMI_requested)
	{
		if (!scumm_stricmp(XMI_fn, ROLAND_DRV_NAME))
			XMI_device_type = XMID_LA;
		else if (!scumm_stricmp(XMI_fn, PCSPKR_DRV_NAME))
			XMI_device_type = XMID_PC;
		else
			XMI_device_type = XMID_AD;

		if ((XMI_driver = load_driver(XMI_fn)) != NULL)
		{
			hXMI = AIL_register_driver(XMI_driver);
			desc = AIL_describe_driver(hXMI);

			desc->default_IO = XMI_IO;

			if (AIL_detect_device(hXMI, desc->default_IO, desc->default_IRQ,
				desc->default_DMA, desc->default_DRQ))
			{
				XMI_desc = *desc;

				Common::strcpy_s(GTL_fn, GTL_PFX);
				Common::strcat_s(GTL_fn, XMI_desc.data_suffix);

				gtl = open(GTL_fn, O_RDONLY | O_BINARY);

				XMI_active = 1;
			}

			AIL_release_driver_handle(hXMI);
		}

		mem_free(XMI_driver);
	}

	sound_on = 1;

	if (errprompt && XMI_requested && (!XMI_active))
		error(MSG_NO_XMI);

	if (errprompt && PCM_requested && (!PCM_active))
		error(MSG_NO_PCM);
}

} // namespace Aesop
