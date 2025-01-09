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

#include "aesop/gil2vfxa.h"

namespace Aesop {

/*
BASCALC         MACRO pt_x,pt_y
				LOCAL mul_320,end_calc
				mov edi,pt_x
				sub edi,x1_p1           ;;normalize against parent's left X
				mov edx,pt_y
				sub edx,y1_p1           ;;normalize against parent's top Y
				mov eax,width_p1
				cmp eax,320
				je mul_320
				mul edx                 ;;multiply Y by buffer width
				add edi,eax
				jmp end_calc
mul_320:        xchg dl,dh              ;;fast mult for screen or staging page
				add edi,edx
				shr edx,2
				add edi,edx             ;;DI=X+320*Y (256*Y+64*Y)
end_calc:
				ENDM

				;
				;BASCALC_2: DI -> address of point (x,y) in secondary window
				;Disposable: AX,DX
				;

BASCALC_2       MACRO pt_x,pt_y
				LOCAL mul_320,end_calc
				mov edi,pt_x
				sub edi,x1_p2           ;;normalize against parent's left X
				mov edx,pt_y
				sub edx,y1_p2           ;;normalize against parent's top Y
				mov eax,width_p2
				cmp eax,320
				je mul_320
				mul edx                 ;;multiply Y by buffer width
				add edi,eax
				jmp end_calc
mul_320:        xchg dl,dh              ;;fast mult for screen or staging page
				add edi,edx
				shr edx,2
				add edi,edx             ;;DI=X+320*Y (256*Y+64*Y)
end_calc:
				ENDM

				;
				;SETWND: Copy window and parent to primary window area
				;Disposable: AX,BX,CX
				;

SETWND          MACRO handle
				LOCAL find_parent,parent_found
				mov ebx,handle
				mov ecx,[ebx].PANE.x0
				mov x1_1,ecx
				mov eax,[ebx].PANE.y0
				mov y1_1,eax
				mov eax,[ebx].PANE.x1
				mov x2_1,eax
				sub eax,ecx
				inc eax
				mov width_1,eax
				mov eax,[ebx].PANE.y1
				mov y2_1,eax
				sub eax,y1_1
				inc eax
				mov height_1,eax

				mov ebx,[ebx].PANE.win
				mov eax,[ebx].WINDOW.wnd_y0
				mov y1_p1,eax
				mov eax,[ebx].WINDOW.wnd_y1
				mov y2_p1,eax
				sub eax,y1_p1
				inc eax
				mov height_p1,eax
				mov ecx,[ebx].WINDOW.wnd_x0
				mov x1_p1,ecx
				mov eax,[ebx].WINDOW.wnd_x1
				mov x2_p1,eax
				sub eax,ecx
				inc eax
				mov width_p1,eax

				ENDM
				;
				;SETWND2: Copy window and parent to primary window area
				;Disposable: AX,BX,CX
				;

SETWND2         MACRO handle
				LOCAL find_parent,parent_found
				mov ebx,handle
				mov ecx,[ebx].PANE.x0
				mov x1_2,ecx
				mov eax,[ebx].PANE.y0
				mov y1_2,eax
				mov eax,[ebx].PANE.x1
				mov x2_2,eax
				sub eax,ecx
				inc eax
				mov width_2,eax
				mov eax,[ebx].PANE.y1
				mov y2_2,eax
				sub eax,y1_2
				inc eax
				mov height_2,eax

				mov ebx,[ebx].PANE.win
				mov eax,[ebx].WINDOW.wnd_y0
				mov y1_p2,eax
				mov eax,[ebx].WINDOW.wnd_y1
				mov y2_p2,eax
				sub eax,y1_p2
				inc eax
				mov height_p2,eax
				mov ecx,[ebx].WINDOW.wnd_x0
				mov x1_p2,ecx
				mov eax,[ebx].WINDOW.wnd_x1
				mov x2_p2,eax
				sub eax,ecx
				inc eax
				mov width_p2,eax

				ENDM

				;
				;SET_TW: Make locally-accessible copy of current text window
				;Disposable: CX,ES,SI,DI; sets DF=0
				;

SET_TW          MACRO
				cld
				mov esi,twptr
				mov edi,OFFSET tw
				mov ecx,(SIZE textwindow)/4
				rep movsd
				IF (SIZE textwindow) AND 2
				movsw
				ENDIF
				IF (SIZE textwindow) AND 1
				movsb
				ENDIF
				ENDM
*/

void GIL2VFXA_print_buffer(PANE *pane, LONG line_number) {
/*
GIL2VFXA_print_buffer    PROC C USES es esi edi, \
				twpane:PPANE,LineNum

				mov eax,[twpane]
				mov twpptr,eax

				SET_TW
				SETWND twpptr

				mov eax,[LineNum]
				mov pending_flag,eax    ;non-zero if pending lines
				dec eax
				mov pending_lines,eax   ;# of lines still pending
				mov eop_reached,0       ;goes true if cout() hits end-of-page

				mov ebx,tw.txtbuf
				push ebp
				xor ebp,ebp

__do_line:      cmp pending_flag,0      ;was linecount parameter nonzero?
				je __update_tw          ;no
				cmp eop_reached,1       ;was end-of-page reached in cout()?
				jae __done              ;yes, terminate buffer dump

__update_tw:
				SET_TW                  ;update local copy of text window data
				mov esi,ebp
				mov line_beg,ebp

				cmp BYTE PTR [ebx][esi],0
				je __eol_found
__find_eol:     mov al,[esi][ebx]       ;find end of first "word"
				inc esi
				or al,al                ;end of string?
				jz __eol_found
				cmp al,10               ;line feed?
				je __eol_found
				cmp al,' '              ;space?
				jne __find_eol
				dec esi                 ;yes, back up one
__eol_found:    mov line_end,esi

				mov edi,tw.htab         ;now find bounds of entire line....
				mov esi,ebp
__lscan:        xor eax,eax
				mov al,[ebx][esi]
				or al,al
				jz __end_scan
				cmp al,10
				je __end_scan
				cmp al,' '
				jne __same_word
				mov line_end,esi
__same_word:    inc esi
				push ebx
				push eax
				push tw.font
				call VFX_character_width
				add esp,8
				pop ebx
				add edi,eax
				mov ecx,x2_1
				inc ecx
				cmp edi,ecx
				jg __w_remaining
				jmp __lscan
__end_scan:     inc esi
				mov line_end,esi
__w_remaining:  mov eax,x2_1
				sub eax,tw.htab
				inc eax                 ;w_win = # of pixels remaining in
				mov w_win,eax           ;window line
				mov w_lin,0
				mov esi,line_beg
__sum_chars:    cmp esi,line_end        ;sum total width of line in w_lin
				je __write_line
				xor eax,eax
				mov al,[ebx][esi]
				push ebx
				push eax
				push tw.font
				call VFX_character_width
				add esp,8
				pop ebx
				add w_lin,eax
				inc esi
				jmp __sum_chars

__write_line:   cmp pending_flag,0      ;line counter active?
				je __not_pending        ;no, continue normally
				cmp pending_lines,0     ;any pending lines?
				je __not_pending        ;no, write this one and continue
				dec pending_lines       ;yes, dec counter and fetch next line
				mov ebp,line_end
				cmp BYTE PTR [ebx][esi-1],0
				je __done               ;(if there IS a next line)
				jmp __next_line

__not_pending:  mov edi,tw.justify
				shl edi,2
;                and edi,7
				mov esi,line_beg
				jmp output_handlers[edi]

__handler_return:
				mov esi,line_end        ;do cr/lf after line if necessary
				mov ebp,esi
				mov ebx,tw.txtbuf
				mov al,[ebx][esi-1]     ;look at last character printed
				or al,al                ;end of buffer, exit
				je __done
				cmp al,10
				je __do_line            ;explicit linefeed, already printed
				push ebx
				mov ebx,10
				push ebx
;                push twpptr
;                invoke GIL2VFXA_cout,twpptr,ebx        ;else do line feed
				call GIL2VFX_cout       ;else do line feed
				add esp,4
				pop ebx
__next_line:    mov esi,ebp
__spc_clip:     cmp BYTE PTR [ebx][esi],' '
				jne __go_nxtlin         ;and skip next line's trailing spaces
				inc esi
				jmp __spc_clip
__go_nxtlin:    mov ebp,esi
				jmp __do_line

__done:         mov pending_flag,0      ;turn off cout() override flag
				pop ebp
				ret

;-----------------------------------------------------------------------------
out_l:          mov esi,line_beg        ;justify left margin
__do_char:      cmp esi,line_end
				jae __donel
				mov ebx,tw.txtbuf
				movzx eax,BYTE PTR [ebx][esi]
				or al,al
				jz __donel
				xor ah,ah
				push eax
				call GIL2VFX_cout
				add esp,4
				inc esi
				jmp __do_char
__donel:        jmp __handler_return

;-----------------------------------------------------------------------------
out_c:          mov eax,w_win           ;center line in window
				sub eax,w_lin
				jbe out_l
				cmp BYTE PTR [ebx][esi],0
				je out_l
				inc eax
				shr eax,1
				mov ebx,twptr
				add [ebx].textwindow.htab,eax
				jmp out_l

;-----------------------------------------------------------------------------
out_r:          mov ecx,x1_1            ;justify right margin
				mov eax,w_lin
				cmp eax,w_win
				ja __set_htab
				cmp BYTE PTR [ebx][esi],0
				je __set_htab
				mov ecx,x2_1
				sub ecx,w_lin
				inc ecx
__set_htab:     mov ebx,twptr
				mov [ebx].textwindow.htab,ecx
				jmp out_l

;-----------------------------------------------------------------------------
out_f:          mov eax,w_lin           ;justify both margins
				cmp eax,w_win
				jae out_l
				mov esi,line_end
				cmp BYTE PTR [ebx][esi-1],0
				je out_l
				cmp BYTE PTR [ebx][esi-1],10
				je out_l                ;don't fill last or truncated line
__lead_spc:     mov esi,line_beg
				cmp BYTE PTR [ebx][esi],' '
				jne __first_word
				mov ebx,' '
				push ebx
;                push twpptr
;                invoke GIL2VFXA_cout,twpptr,ebx
				call GIL2VFX_cout
				add esp,4

				mov ebx,' '
				push ebx
				push tw.font
				call VFX_character_width
				add esp,8

				mov ebx,tw.txtbuf
				sub w_lin,eax
				inc line_beg
				jmp __lead_spc
__first_word:   mov esi,line_beg        ;write first word normally...
				movzx eax,BYTE PTR [ebx][esi]
				cmp al,' '
				je __get_pad
				xor ah,ah
				mov edi,eax
				push edi
;                push twpptr
;                invoke GIL2VFXA_cout,twpptr,edi
				call GIL2VFX_cout
				add esp,4

				push edi
				push tw.font
				call VFX_character_width
				add esp,8

				sub w_lin,eax
				mov ebx,tw.txtbuf
				inc line_beg
				jmp __first_word
__get_pad:      mov ebx,twptr           ;get # of pixels left to pad
				mov eax,x2_1
				sub eax,[ebx].textwindow.htab
				inc eax
				sub eax,w_lin
				mov ebx,tw.txtbuf
				xor ecx,ecx
				mov esi,line_beg
__wcount:       cmp esi,line_end        ;get # of words left in line
				jae __calc_pad
				cmp BYTE PTR [ebx][esi],' '
				jne __next_wc
				inc ecx
__next_wc:      inc esi
				jmp __wcount
__calc_pad:     or ecx,ecx
				je out_l
				or ch,ch
				jne out_l
				div cl                  ;divide to calculate padding
				mov quotient,al         ;per word
				mov remainder,ah

				mov esi,line_beg         ;dump the line....
__do_charf:     mov ebx,tw.txtbuf
				cmp esi,line_end
				jae __donef
				movzx eax,BYTE PTR [ebx][esi]
				or al,al
				jz __donef
				xor ah,ah
				push eax
				push eax
;                push twpptr
;                invoke GIL2VFXA_cout,twpptr,eax
				call GIL2VFX_cout
				add esp,4

				pop eax
				cmp al,' '              ;end of word?
				jne __next_char
				mov al,quotient         ;yes, bump htab for padding
				mov ebx,twptr
				add [ebx].textwindow.htab,eax
				cmp remainder,0
				je __next_char
				inc [ebx].textwindow.htab
				dec remainder
__next_char:    inc esi
				jmp __do_charf
__donef:        jmp __handler_return

;-----------------------------------------------------------------------------

output_handlers dd out_l,out_r,out_c,out_f

GIL2VFXA_print_buffer    ENDP
*/
}

void GIL2VFX_hook_INT9() {
}

void GIL2VFX_unhook_INT9() {
}

} // namespace Aesop
