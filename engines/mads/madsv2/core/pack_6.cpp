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

#include "mads/madsv2/core/general.h"
#include "mads/madsv2/core/implode.h"
#include "mads/madsv2/core/mem.h"
#include "mads/madsv2/core/keys.h"
#include "mads/madsv2/core/error.h"
#include "mads/madsv2/core/ems.h"
#include "mads/madsv2/core/pack.h"
#include "mads/madsv2/core/pack_1.h"
#include "mads/madsv2/core/pack_5.h"
#include "mads/madsv2/core/pack_d.h"

namespace MADS {
namespace MADSV2 {

byte *pack_special_buffer = NULL;
void (*(pack_special_function))() = NULL;

static void pack_activate(void) {
	if (pack_special_function)
		pack_special_function();
}

void pack_set_special_buffer(byte *buffer_address, void (*(special_function))()) {
	pack_special_buffer = buffer_address;
	pack_special_function = special_function;
}

/*
/*      pack_data()
/*
/*      Transfers a data packet from the specified source to the specified
/*      destination, using the specified packing strategy.
/*
/*              packing_flag    Specifies the packing strategy:
/*                              PACK_IMPLODE  (Compresses   data)
/*                              PACK_EXPLODE  (Decompresses data)
/*                              PACK_RAW_COPY (Copies data)
/*
/*              size            # of bytes to move
/*
/*              source_type     Specifies the source type:
/*                              FROM_DISK or FROM_MEMORY.
/*
/*              source          If FROM_DISK, then this is a FILE *handle.
/*                              If FROM_MEMORY, this is a far memory pointer.
/*
/*              dest_type       Specifies the destination type:
/*                              TO_DISK, TO_MEMORY, or TO_EMS.
/*
/*              dest            Same as "source" but for destination. For
/*                              TO_EMS, "dest" is a far pointer to an
/*                              EmsPtr structure.
/*
/*
/*      Example:
/*
/*              result =pack_data (PACK_EXPLODE, 132000,
/*                                 FROM_DISK, file_handle,
/*                                 TO_MEMORY, memory_pointer);
/*
/*              (Decompresses 132000 bytes from the already open
/*               disk file "file_handle", and writes it to memory
/*               far the specified address.  Size is always the
/*               uncompressed size of the data.  Result will be
/*               the # of bytes actually written -- 132000 if successful).
/*
*/
long fastcall pack_data(int packing_flag, long size,
	int source_type, void *source, int dest_type, void *dest) {
	int explode_mode = 0;
	long *loop_value;
	long *return_value;
	EmsPtr *ems_dest;
	int result;

	/* Select the read data routine */

	if (source_type == FROM_MEMORY) {
		pack_read_routine = pack_read_memory;
		pack_read_memory_ptr = (byte *)source;
	} else {
		/* FROM_DISK */
		pack_read_routine = pack_read_file;
		pack_read_file_handle = (Common::SeekableReadStream *)source;
	}

	/* Select the write data routine */

	if (dest_type == TO_EMS) {
		pack_write_routine = pack_write_ems;
		ems_dest = (EmsPtr *)dest;
		pack_ems_page_handle = ems_dest->handle;
		pack_ems_page_marker = ems_dest->page_marker;
		pack_ems_page_offset = ems_dest->page_offset;
	} else if (dest_type == TO_MEMORY) {
		pack_write_routine = pack_write_memory;
		pack_write_memory_ptr = (byte *)dest;
	} else {
		/* TO_DISK */
		pack_write_routine = pack_write_file;
		pack_write_file_handle = (Common::WriteStream *)dest;
	}

	/* Set up the packing parameters */

	pack_read_count = pack_write_count = 0;

	switch (packing_flag) {
	case PACK_IMPLODE:
		if (pack_strategy == PACK_PFAB) {
			pack_buffer_size = PACK_PFABCOMP_SIZE;
			if (pack_pFABcomp_routine == NULL) {
				error_report(ERROR_EXPLODER_NULL, SEVERE, MODULE_EXPLODER, packing_flag, pack_strategy);
			}
		} else {
			pack_buffer_size = PACK_IMPLODE_SIZE;
			if (pack_implode_routine == NULL) {
				error_report(ERROR_EXPLODER_NULL, SEVERE, MODULE_EXPLODER, packing_flag, pack_strategy);
			}
		}
		pack_read_size = size;                  /* Stop after reading "size" bytes  */
		pack_write_size = -1;                    /* Write as many bytes as necessary */
		loop_value = &pack_read_size;       /* Loop control is # bytes to read  */
		return_value = &pack_read_count;      /* Return value is # bytes read     */
		break;

	case PACK_EXPLODE:
		pack_read_size = -1;                    /* Read as many bytes as necessary  */
		pack_write_size = size;                  /* Stop after writing "size" bytes  */
		loop_value = &pack_write_size;      /* Loop control is # bytes to write */
		if (pack_strategy == PACK_PFAB) {
			if ((source_type == FROM_MEMORY) && (dest_type == FROM_MEMORY) &&
				(pack_pFABexp2_routine != NULL)) {
				return_value = &size; /* Fake return value */
				pack_buffer_size = PACK_PFABEXP2_SIZE;
				explode_mode = 2;
			} else if ((dest_type == TO_DISK) || (dest_type == TO_EMS)) {
				return_value = &pack_write_count;   /* Return value is # bytes written  */
				pack_buffer_size = PACK_PFABEXP0_SIZE;
				explode_mode = 0;
				if (pack_pFABexp0_routine == NULL) {
					error_report(ERROR_EXPLODER_NULL, SEVERE, MODULE_EXPLODER, packing_flag, pack_strategy);
				}
			} else {
				return_value = &size;               /* Fake return value for file-to_mem*/
				pack_buffer_size = PACK_PFABEXP1_SIZE;
				explode_mode = 1;
				if (pack_pFABexp1_routine == NULL) {
					error_report(ERROR_EXPLODER_NULL, SEVERE, MODULE_EXPLODER, packing_flag, pack_strategy);
				}
			}
		} else {
			return_value = &pack_write_count;
			pack_buffer_size = PACK_EXPLODE_SIZE;
			if (pack_explode_routine == NULL) {
				error_report(ERROR_EXPLODER_NULL, SEVERE, MODULE_EXPLODER, packing_flag, pack_strategy);
			}
		}
		break;

	case PACK_RAW_COPY:
	default:
		pack_buffer_size = PACK_RAW_COPY_SIZE;
		pack_read_size = size;                  /* Stop after reading "size" bytes   */
		pack_write_size = size;                  /* ... or after writing "size" bytes */
		loop_value = &pack_read_size;       /* Loop control is # bytes to read   */
		return_value = &pack_write_count;     /* Return value is # bytes written   */
		break;
	}

	/* Get memory for packing buffer if necessary */

	pack_buffer = NULL;

	if (pack_special_buffer == NULL) {
		pack_buffer = (byte *)mem_get_name(pack_buffer_size, "$pack$");
		if (pack_buffer == NULL) {
			*return_value = 0;
			goto done;
		}
	} else {
		pack_buffer = pack_special_buffer;
	}

	/* Keep moving records until we run out of data or die */

	if ((packing_flag == PACK_EXPLODE) && (dest_type == TO_MEMORY)) {
		result = pack_a_packet(packing_flag, explode_mode);
		if (result != CMP_NO_ERROR) {
			*return_value = 0;
			error_report(ERROR_EXPLODER_EXPLODED, SEVERE, MODULE_EXPLODER, (packing_flag * 1000) + explode_mode, result);
			goto done;
		}
	} else {
		while (*loop_value > 0) {
			if (pack_a_packet(packing_flag, false) != CMP_NO_ERROR) {
				*return_value = 0;
				error_report(ERROR_EXPLODER_EXPLODED, SEVERE, MODULE_EXPLODER, packing_flag, dest_type);
				goto done;
			}
		}
	}

	/* Free memory and go away */

done:
	if (pack_special_buffer == NULL) {
		if (pack_buffer != NULL) mem_free(pack_buffer);
	} else {
		pack_activate();
	}

	return *return_value;
}

} // namespace MADSV2
} // namespace MADS
