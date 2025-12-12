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

#ifndef AESOP_AIL32_H
#define AESOP_AIL32_H

namespace Aesop {

//
// MetaWare support
//

#ifdef __HIGHC__
#define _CC(_REVERSE_PARMS | _NEAR_CALL)
#pragma Global_aliasing_convention("_%r");
#pragma Align_members(1)

#ifndef FAR_PTR_DEF
#define FAR_PTR_DEF

typedef struct   // Union used to build far pointers under MetaWare
{
   unsigned off;
   unsigned short seg;
}
fp_ovl;

typedef union
{
   _Far void *ptr;
   fp_ovl part;
}
FARPTR;

#endif
#endif

//
// End of MetaWare conditionals
//

#define SEQ_STOPPED 0       /* equates for AIL_sequence_status()         */
#define SEQ_PLAYING 1
#define SEQ_DONE 2

#define DAC_STOPPED 0       /* equates for AIL_sound_buffer_status()     */
#define DAC_PAUSED 1        /*             AIL_VOC_playback_status()     */
#define DAC_PLAYING 2
#define DAC_DONE 3

#define XMIDI_DRVR 3        /* equates for drvr_desc.drvr_type values    */
#define DSP_DRVR 2

typedef int HTIMER;         /* handle to timer                           */
typedef int HDRIVER;        /* handle to driver                          */
typedef int HSEQUENCE;      /* handle to XMIDI sequence                  */

typedef struct
{
   unsigned min_API_version;
   unsigned drvr_type;
   char data_suffix[4];
   void *dev_name_table;
   int default_IO;
   int default_IRQ;
   int default_DMA;
   int default_DRQ;
   int service_rate;
   unsigned display_size;
}  
drvr_desc;

#ifdef __HIGHC__           /* MetaWare uses different far pointer format */

typedef struct
{
   unsigned pack_type;
   unsigned sample_rate;
   FARPTR sel_data;
   unsigned seg_data;
   unsigned len;
}
sound_buff;

#else

typedef struct
{
   unsigned pack_type;
   unsigned sample_rate;
   void *sel_data;
   unsigned seg_data;
   unsigned len;
}
sound_buff;

#endif

/********************/
/*                  */
/* Process services */
/*                  */
/********************/

void AIL_startup(void);
void AIL_shutdown(char *signoff_msg);
HTIMER AIL_register_timer(void *callback_fn);
void AIL_set_timer_period(HTIMER timer, unsigned long microseconds);
void AIL_set_timer_frequency(HTIMER timer, unsigned long hertz);
void AIL_set_timer_divisor(HTIMER timer, unsigned PIT_divisor);
unsigned AIL_interrupt_divisor(void);
void AIL_start_timer(HTIMER timer);
void AIL_start_all_timers(void);
void AIL_stop_timer(HTIMER timer);
void AIL_stop_all_timers(void);
void AIL_release_timer_handle(HTIMER timer);
void AIL_release_all_timers(void);

/*************************/
/*                       */
/* Installation services */
/*                       */
/*************************/

HDRIVER AIL_register_driver(void *driver_base_addr);
void AIL_release_driver_handle(HDRIVER driver);
drvr_desc * AIL_describe_driver(HDRIVER driver);
unsigned AIL_detect_device(HDRIVER driver, unsigned IO_addr, 
    unsigned IRQ, unsigned DMA, unsigned DRQ);
void AIL_init_driver(HDRIVER driver, unsigned IO_addr, 
    unsigned IRQ, unsigned DMA, unsigned DRQ);
void AIL_shutdown_driver(HDRIVER driver, char *signoff_msg);
                    
/********************************/
/*                              */
/* Digital performance services */
/*                              */
/********************************/

#ifdef __HIGHC__           /* MetaWare uses different far pointer format */

unsigned AIL_index_VOC_block(HDRIVER driver, _Far void *VOC_sel, 
   unsigned VOC_seg, unsigned block_marker, sound_buff *buff);
void AIL_format_VOC_file(HDRIVER driver, _Far void *VOC_sel, int
   block_marker);
void AIL_play_VOC_file(HDRIVER driver, _Far void *VOC_sel,
   unsigned VOC_seg, int block_marker);

#else

unsigned AIL_index_VOC_block(HDRIVER driver, void *VOC_sel, 
   unsigned VOC_seg, unsigned block_marker, sound_buff *buff);
void AIL_format_VOC_file(HDRIVER driver, void *VOC_sel, int
   block_marker);
void AIL_play_VOC_file(HDRIVER driver, void *VOC_sel,
   unsigned VOC_seg, int block_marker);

#endif

void AIL_register_sound_buffer(HDRIVER driver, unsigned buffer_num,
   sound_buff *buff);
void AIL_format_sound_buffer(HDRIVER driver, sound_buff *buff);
unsigned AIL_sound_buffer_status(HDRIVER driver, unsigned buffer_num);
unsigned AIL_VOC_playback_status(HDRIVER driver);

void AIL_start_digital_playback(HDRIVER driver);
void AIL_stop_digital_playback(HDRIVER driver);
void AIL_pause_digital_playback(HDRIVER driver);
void AIL_resume_digital_playback(HDRIVER driver);
void AIL_set_digital_playback_volume(HDRIVER driver, unsigned volume);
unsigned AIL_digital_playback_volume(HDRIVER driver);
void AIL_set_digital_playback_panpot(HDRIVER driver, unsigned panpot);
unsigned AIL_digital_playback_panpot(HDRIVER driver);

/******************************/
/*                            */
/* XMIDI performance services */
/*                            */
/******************************/

unsigned AIL_state_table_size(HDRIVER driver);
HSEQUENCE AIL_register_sequence(HDRIVER driver, void *FORM_XMID,
   unsigned sequence_num, void *state_table, void *controller_table);
void AIL_release_sequence_handle(HDRIVER driver, HSEQUENCE sequence);

unsigned AIL_default_timbre_cache_size(HDRIVER driver);
void AIL_define_timbre_cache(HDRIVER driver, void *cache_addr,
   unsigned cache_size);                     
unsigned AIL_timbre_request(HDRIVER driver, HSEQUENCE sequence);
unsigned AIL_timbre_status(HDRIVER driver, int bank, int patch);
void AIL_install_timbre(HDRIVER driver, int bank, int patch, 
   void *src_addr);
void AIL_protect_timbre(HDRIVER driver, int bank, int patch);
void AIL_unprotect_timbre(HDRIVER driver, int bank, int patch);

void AIL_start_sequence(HDRIVER driver, HSEQUENCE sequence);
void AIL_stop_sequence(HDRIVER driver, HSEQUENCE sequence);
void AIL_resume_sequence(HDRIVER driver, HSEQUENCE sequence);
unsigned AIL_sequence_status(HDRIVER driver, HSEQUENCE sequence);
unsigned AIL_relative_volume(HDRIVER driver, HSEQUENCE sequence);
unsigned AIL_relative_tempo(HDRIVER driver, HSEQUENCE sequence);
void AIL_set_relative_volume(HDRIVER driver, HSEQUENCE sequence,
   unsigned percent, unsigned milliseconds);
void AIL_set_relative_tempo(HDRIVER driver, HSEQUENCE sequence,
   unsigned percent, unsigned milliseconds);
int AIL_controller_value(HDRIVER driver, HSEQUENCE sequence,
   unsigned channel, unsigned controller_num);
void AIL_set_controller_value(HDRIVER driver, HSEQUENCE sequence,
   unsigned channel, unsigned controller_num, unsigned value);
unsigned AIL_channel_notes(HDRIVER driver, HSEQUENCE sequence,
   unsigned channel);
unsigned AIL_beat_count(HDRIVER driver, HSEQUENCE sequence);
unsigned AIL_measure_count(HDRIVER driver, HSEQUENCE sequence);
void AIL_branch_index(HDRIVER driver, HSEQUENCE sequence, 
   unsigned marker_number);

void AIL_send_channel_voice_message(HDRIVER driver, unsigned status, 
   unsigned data_1, unsigned data_2);
void AIL_send_sysex_message(HDRIVER driver, unsigned addr_a, 
   unsigned addr_b, unsigned addr_c, void *data, unsigned size, 
   unsigned delay);
void AIL_write_display(HDRIVER driver, char *string);
void AIL_install_callback(HDRIVER driver, 
   void *callback_fn);
void AIL_cancel_callback(HDRIVER driver);

unsigned AIL_lock_channel(HDRIVER driver);
void AIL_map_sequence_channel(HDRIVER driver, HSEQUENCE sequence,
   unsigned sequence_channel, unsigned physical_channel);
unsigned AIL_true_sequence_channel(HDRIVER driver, HSEQUENCE sequence,
   unsigned sequence_channel);
void AIL_release_channel(HDRIVER driver, unsigned channel);
  
//
// MetaWare support
//

#ifdef __HIGHC__
#pragma Global_aliasing_convention();
#endif

} // namespace AESOP

#endif
