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
#include "aesop/vars.h"

namespace Aesop {

struct PHDR {
	uint16 static_size;
	ULONG imports;
	ULONG exports;
};

struct VALUELowHigh {
	uint16 val_l;
	uint16 val_h;
};

union VALUE {
	void *addr_flat;
	intptr_t val;
	VALUELowHigh lh;
};

ULONG current_this;
ULONG current_msg;
ULONG current_index;

ULONG stk_base;
ULONG stk_off;
ULONG objlist;

//*****************************************************************************
//*                                                                           *
//* Optimized high-level functions                                            *
//*                                                                           *
//*****************************************************************************

BYTE *RTD_lookup(HRES dictionary, const char *key) {
	error("TODO: RTD_lookup");
	/*
	RTD_lookup      PROC C USES es esi edi,\
			HRES:DWORD,Key:PTR

					LOCAL hash_size
					LOCAL tag_len
					LOCAL es32

					cld

					mov eax,[HRES]          ;*DS = dictionary base segment
					mov eax,[eax]           ;eax -> dictionary
					mov es32,eax            ;es32 -> dictionary

					xor edi,edi
					mov di,WORD PTR [eax]   ;EDI = dictionary's hash table modulus
					mov hash_size,edi

					mov esi,[Key]            ;compute hash function value for key
					mov eax,0
					mov ebx,0
					mov ecx,0
					mov edx,0
					mov edi,1

	__hash_loop:
					LCNT = 0                ;(partially unroll loop for speed)
					REPT 4

					mov bl,[esi+LCNT]
					or bl,bl
					jz __search
					inc edi
					add eax,ebx

					LCNT = LCNT + 1
					ENDM

					add esi,4
					jmp __hash_loop

	__search:       mov tag_len,edi         ;DI = length of tag string
					div hash_size           ;DX = hash function value for key

					mov edi,edx             ;edi = index into chain[]
					shl edi,2               ;*4 for ULONG chain[]

					add edi,es32            ;add dictionary base address

					mov eax,[edi+2]
					mov ecx,eax
					or ecx,eax              ;is chain for this value present?
					jz __not_found          ;no, exit w/error

					mov edi,es32
					add edi,ecx             ;EDI -> first chain entry

					mov edx,tag_len

	__find_match:   xor ecx,ecx
					mov cx,WORD PTR [edi]   ;fetch tag length
					add edi,2               ;add size of tag len word
					or ecx,ecx
					jz __not_found          ;if end of chain reached, exit w/error

					cmp ecx,edx             ;do string lengths match?
					je __len_matched        ;if not, don't bother checking strings

					add edi,ecx             ;add len of tag string
					xor eax,eax
					mov ax,WORD PTR [edi]   ;add len of def string
					add edi,eax
					add edi,2               ;add size of def len word
					jmp __find_match

	__len_matched:  mov ebx,edi
					mov esi,[Key]
					repe cmpsb
					je __found

					mov edi,ebx             ;recover pointer to tag string

					add edi,edx             ;add len of tag string
					xor eax,eax
					mov ax,WORD PTR [edi]   ;add len of def string
					add edi,eax
					add edi,2               ;add size of def len word
					jmp __find_match

	__not_found:    mov eax,0
					ret

	__found:        mov eax,edi             ;set DX:AX -> len-prefixed def string

					add eax,2               ;skip def's len prefix
					ret

	RTD_lookup      ENDP
	*/
}

void *RTD_first(void *dictionary) {
	error("TODO: RTD_first");
	/*
;*****************************************************************************
RTD_first       PROC C \
				USES es esi edi,\
		Base:PTR

				mov esi,[Base]
				xor eax,eax
				mov ax,WORD PTR [esi]
				add esi,2
				shl eax,2               ;skip hash table (max hash mod < 16K)
				add esi,eax

				cmp WORD PTR [esi],0    ;1st chain present?
				je __empty              ;no, dictionary must be empty

				mov eax,esi             ;else return pointer to tag_len in
				sub eax,[Base]          ;return offset from beginning of dict
				ret

__empty:        mov eax,0
				ret

RTD_first       ENDP
*/
}

void *RTD_iterate(void *base, void *cur, BYTE **tag, BYTE **def) {
	error("TODO: RTD_iterate");
	/*
	;*****************************************************************************
	RTD_iterate     PROC C \
					USES es esi edi,\
			Base:PTR,Cur:PTR,TagStr:PTR,DefStr:PTR

					mov ax,WORD PTR [Cur]
					or ax,WORD PTR [Cur+2]
					jz __end                ;nothing to iterate

					mov esi,[Cur]           ;get offset
					add esi,[Base]          ;add base address

					mov edi,[TagStr]

					xor eax,eax
					mov ax,WORD PTR [esi]
					add esi,2

					or eax,eax              ;end of chain? (tag_len == 0)
					jz __end_chain

	__read_tag:     mov [edi],esi

					mov edi,[DefStr]

					add esi,eax
					xor eax,eax
					mov ax,WORD PTR [esi];(assumes definition present)
					add esi,2
					mov [edi],esi

					add esi,eax

	__exit:         mov eax,esi             ;return pointer to next entry in chain
					sub eax,[Base]          ;return offset from beginning of dict
					ret

	__end_chain:
					xor eax,eax
					mov ax,WORD PTR [esi];end of chain; get len of 1st tag in
					add esi,2               ;next chain

					or eax,eax
					jnz __read_tag

	__end:          mov eax,0               ;no next chain; end of dictionary
					ret

	RTD_iterate     ENDP
	*/
}

//*****************************************************************************
//*                                                                           *
//* AESOP runtime interpreter                                                 *
//*                                                                           *
//*****************************************************************************
/*
OP_FETCH        MACRO                   ;Fetch next opcode
				xor ebx,ebx
				mov bl,BYTE PTR [esi]
				inc esi                 ;bump PC
				jmp op_dispatch[ebx*4]  ;do indirect jump to opcode handler
				ENDM

PS_DEREF        MACRO                   ;Set DS = new base program segment
				sub esi,ds32
				push eax
				mov eax,h_prg
				mov eax,[eax]
				mov ds32,eax
				pop eax
				add esi,ds32
				ENDM

OFF_THIS        = 2                     ;Offset of manifest THIS constant

GET_WORD        MACRO
				movzx esi,WORD PTR [esi];get Msg Handler Jump Offset in esi
				add esi,ds32            ;add base address of Msg Handler
				ENDM
*/

void RT_init(RTR_class *rtr, ULONG stack_size, HRES *rtObjList) {
	error("TODO: RT_init");
	/*
	;*****************************************************************************
	RT_init         PROC C USES es esi edi,\
			RTR:PTR,StkSize:DWORD,ObjectList:DWORD

					mov edi,[ObjectList]
					mov objlist,edi

					mov edi,[RTR]
					mov RTR_class,edi

					mov eax,[StkSize]
					push eax
					call mem_alloc
					add esp,4

					mov edi,eax                     ;edi = ptr to memory block

					mov stk_base,edi

					add edi,StkSize

					mov stk_off,edi

					ret
	RT_init         ENDP
	*/
}

void RT_shutdown() {
	error("TODO: RT_shutdown");
	/*
RT_shutdown     PROC C USES es esi edi

				push stk_base
				call mem_free
				add esp,4

				ret
RT_shutdown     ENDP
*/
}

void RT_arguments(void *base, ULONG size) {
	/*
RT_arguments    PROC C USES es esi edi,\
		Base:PTR,Bytes

				cld

				mov ecx,[Bytes]
				sub stk_off,ecx

				mov esi,[Base]
				mov edi,stk_off

				shr ecx,1
				rep movsw

				ret
RT_arguments    ENDP
*/
}

LONG RT_execute(ULONG index, ULONG msg_num, ULONG vector) {
	error("TODO: RT_execute");
/*
RT_execute      PROC C USES es esi edi,\
		Index,Message,Vector

				ASSUME edi:VALUE

				LOCAL h_prg             ;Handle to base SOP program
				LOCAL h_thunk           ;Handle to thunk
				LOCAL ptr_thunk         ;Pointer to thunk
				LOCAL h_instance        ;Handle to instance
				LOCAL fptr              ;Pointer to current AESOP stack frame
				LOCAL static_offset     ;Offset to static variable space
				LOCAL extern_offset     ;Offset to external reference fixups
				LOCAL cur_vector        ;Vector offset of current handler
				LOCAL ds32

				cld

				mov esi,[objlist]
				mov ebx,[Index]

				cmp bx,-1               ;sanity check
				je __no_vector

				mov current_index,ebx   ;save object # to debug variable

				shl ebx,2
				mov eax,[esi][ebx]      ;get instance handle

				cmp eax,-1              ;sanity check
				je __no_vector

				mov h_instance,eax      ;store it for quick recall
				mov ebx,[eax]           ;ebx = ptr to Instance

				mov ebx,[ebx]           ;get thunk handle from instance header
				mov h_thunk,ebx

				mov ebx,[ebx]           ;get thunk ptr from HD_entry
				mov ptr_thunk,ebx

				mov esi,[Vector]        ;explicit vector offset specified by
				cmp esi,-1              ;pass_message()?
				je  __n_pass            ;no, skip double check
				cmp esi,0000ffffh       ;pass_message()?
				jne __handle_msg        ;yes, call it directly

__n_pass:
				xor edx,edx
				mov dx,[ebx].THDR.max_msg    ;EDX = max
				xor eax,eax
				mov ax,[ebx].THDR.MV_list    ;do binary search on vect list
				add ebx,eax             ;ebx = PTR to MV_list

				cmp dx,-1
				je __no_vector          ;if no vectors exported, return -1

				mov eax,0               ;AX = min

				mov edi,[Message]       ;DI = message # to find
				mov current_msg,edi     ;save message # to debug variable

__find_vector:  mov ecx,eax             ;CX = mid*2 = (min+max)
				add ecx,edx
				and ecx,0fffffffeh      ;round down to 0 or multiple of 2

				mov esi,ecx             ;set SI = mid * SIZE MV_entry
				shl esi,2               ;(*2)*(*4)=(*8)

				cmp [ebx][esi].MV_entry.msg_num,di
				jg __greater
				je __equal

				shr ecx,1               ;entry < message, min=mid+1
				inc ecx
				mov eax,ecx

				cmp eax,edx
				jle __find_vector       ;if min <= max, continue search
				mov eax,-1
				mov edx,-1              ;else message vector not found,
				jmp __exit              ;return -1 to sender

__greater:      shr ecx,1               ;entry > message, max=mid-1
				dec ecx
				mov edx,ecx

				cmp eax,edx
				jle __find_vector       ;if min <= max, continue search
				mov edx,-1
__no_vector:    mov eax,-1              ;else message vector not found,
				jmp __exit              ;return -1 to sender

__equal:        add esi,SIZE MV_entry   ;now find lowest-level class which
										;handles this message
__find_first:   sub esi,SIZE MV_entry
				jz __MV_found
				cmp [ebx][esi-SIZE MV_entry].MV_entry.msg_num,di
				je __find_first

__MV_found:     add esi,ebx

__handle_msg:   mov ebx,esi
				sub ebx,ptr_thunk

				mov cur_vector,ebx      ;perform call through message vector

				xor ebx,ebx
				mov bx,[esi].MV_entry.SD_offset
				add ebx,ptr_thunk       ;ebx = Ptr to SOP descriptor in thunk
				mov esi,[esi].MV_entry.handler_offset

				mov eax,[ebx].SD_entry.SOP_handle
				mov h_prg,eax

				xor eax,eax
				mov ax,[ebx].SD_entry.static_base
				mov static_offset,eax

				xor eax,eax
				mov ax,[ebx].SD_entry.extern_base
				mov extern_offset,eax

				push h_prg
				push RTR_class
				call RTR_lock
				add esp,8

				mov ds32,0
				PS_DEREF                ;ESI -> MHDR

				mov edi,stk_off         ;EDI -> STK

				mov fptr,edi            ;FPTR = stack pointer at entry

				mov eax,[Index]         ;initialize manifest THIS var
				mov WORD PTR [edi-OFF_THIS],ax

				xor edx,edx
				mov dx,[esi].MHDR.auto_size  ;make room for auto variables
				sub edi,edx
				sub edi,SIZE VALUE      ;point to first free stack location

				add esi,SIZE MHDR       ;skip message handler header

				OP_FETCH                ;start execution

__exit:         ret

;-----------------------------------------------------------------------------
do_BRT:                                 ;BRanch if True

				ASSUME edi:VALUE

				mov ax,[edi].VALUE.val_l
				or ax,[edi].VALUE.val_h
				jz __skip

				GET_WORD
				OP_FETCH

__skip:         add esi,2
				OP_FETCH

;-----------------------------------------------------------------------------
do_BRF:                                 ;BRanch if False

				mov ax,[edi].VALUE.val_l
				or ax,[edi].VALUE.val_h
				jnz __skip_BRF

				GET_WORD
				OP_FETCH

__skip_BRF:     add esi,2
				OP_FETCH

;-----------------------------------------------------------------------------
do_BRA:                                 ;BRanch Always

				GET_WORD
				OP_FETCH

;-----------------------------------------------------------------------------
do_CASE:                                ;CASE selection

				mov eax,[edi]           ;get value of test expression

				xor ecx,ecx
				mov cx,WORD PTR [esi]   ;get # of cases to check
				add esi,2

				or ecx,ecx
				jz __branch             ;(no cases)

__for_case:     cmp eax,[esi]           ;find matching case value
				je __check_high
__next_case:    add esi,6
				dec ecx
				jnz __for_case
				jmp __branch            ;no matching cases, use default

__check_high:
										;already checked
				add esi,4

__branch:
				GET_WORD
				OP_FETCH

;-----------------------------------------------------------------------------
do_PUSH:                                ;PUSH 0 onto stack

				sub edi,SIZE VALUE

				mov [edi].VALUE.val,0

				OP_FETCH

;-----------------------------------------------------------------------------
do_DUP:                                 ;DUPlicate top of stack

				mov eax,[edi]
				sub edi,SIZE VALUE
				mov [edi],eax

				OP_FETCH

;-----------------------------------------------------------------------------
do_NOT:                                 ;Logical NOT (unary)

				mov ax,[edi].VALUE.val_l
				or ax,[edi].VALUE.val_h

				cmp ax,1                ;C=1 if !AX, else C=0
				mov ax,0
				adc ax,0                ;set AX=C

				mov [edi].VALUE.val_h,0
				mov [edi].VALUE.val_l,ax

				OP_FETCH

;-----------------------------------------------------------------------------
do_SETB:                                ;SET Boolean value (unary)

				mov ax,[edi].VALUE.val_l
				or ax,[edi].VALUE.val_h

				cmp ax,1                ;C=1 if !AX, else C=0
				mov ax,0
				sbb ax,-1               ;set AX=!C

				mov [edi].VALUE.val_h,0
				mov [edi].VALUE.val_l,ax

				OP_FETCH

;-----------------------------------------------------------------------------
do_NEG:                                 ;NEGate (unary)

				neg [edi].VALUE.val

				OP_FETCH

;-----------------------------------------------------------------------------
do_ADD:                                 ;ADD (binary)

				mov eax,[edi].VALUE.val

				add edi,SIZE VALUE

				add [edi].VALUE.val,eax

				OP_FETCH

;-----------------------------------------------------------------------------
do_SUB:                                 ;SUBtract (binary)

				mov eax,[edi].VALUE.val

				add edi,SIZE VALUE

				sub [edi].VALUE.val,eax

				OP_FETCH

;-----------------------------------------------------------------------------
do_MUL:                                 ;MULtiply (binary)

				mov eax,[edi].VALUE.val
				add edi,SIZE VALUE
				imul [edi].VALUE.val
				mov [edi].VALUE.val,eax

				OP_FETCH

;-----------------------------------------------------------------------------
do_DIV:                                 ;DIVide (binary)

				mov ebx,[edi].VALUE.val
				mov edx,0
				add edi,SIZE VALUE
				mov eax,[edi].VALUE.val
				idiv ebx
				mov [edi].VALUE.val,eax

				OP_FETCH

;-----------------------------------------------------------------------------
do_MOD:                                 ;MODulus (binary)

				mov ebx,[edi].VALUE.val
				add edi,SIZE VALUE

				mov edx,0
				mov eax,[edi].VALUE.val

				idiv ebx

				mov [edi].VALUE.val,edx

				OP_FETCH

;-----------------------------------------------------------------------------
do_EXP:                                 ;EXPonent (binary)

				mov eax,1

				xor ecx,ecx
				mov cx,[edi].VALUE.val_l
				add edi,SIZE VALUE

				cmp ecx,0
				je __EXP_done

				mov eax,[edi].VALUE.val

__EXP_loop:     dec ecx
				jz __EXP_done

				mov ebx,[edi].VALUE.val

				imul eax,ebx

				jmp __EXP_loop

__EXP_done:     mov [edi].VALUE.val,eax

				OP_FETCH

;-----------------------------------------------------------------------------
do_BAND:                                ;Bitwise AND (binary)

				mov eax,[edi].VALUE.val
				add edi,SIZE VALUE
				and [edi].VALUE.val,eax

				OP_FETCH

;-----------------------------------------------------------------------------
do_BOR:                                 ;Bitwise OR (binary)

				mov eax,[edi].VALUE.val
				add edi,SIZE VALUE
				or [edi].VALUE.val,eax

				OP_FETCH

;-----------------------------------------------------------------------------
do_XOR:                                 ;Bitwise XOR (binary)

				mov eax,[edi].VALUE.val
				add edi,SIZE VALUE
				xor [edi].VALUE.val,eax

				OP_FETCH

;-----------------------------------------------------------------------------
do_BNOT:                                ;Bitwise NOT (unary)

				not [edi].VALUE.val

				OP_FETCH

;-----------------------------------------------------------------------------
do_SHL:                                 ;SHift Left (binary)

				xor ecx,ecx
				mov cx,[edi].VALUE.val_l
				add edi,SIZE VALUE

				shl [edi].VALUE.val,cl

__SHL_done:     OP_FETCH

;-----------------------------------------------------------------------------
do_SHR:                                 ;SHift Right (binary)

				xor ecx,ecx
				mov cx,[edi].VALUE.val_l
				add edi,SIZE VALUE

				shr [edi].VALUE.val,cl

__SHR_done:     OP_FETCH

;-----------------------------------------------------------------------------
do_LT:                                  ;Less Than (binary)

				mov ecx,1

				mov eax,[edi+SIZE VALUE].VALUE.val
				cmp eax,[edi].VALUE.val
				jl __compare_done

				mov ecx,0

__compare_done: mov [edi+SIZE VALUE],ecx
				add edi,SIZE VALUE

				OP_FETCH

;-----------------------------------------------------------------------------
do_LE:                                  ;Less than or Equal (binary)

				mov ecx,1

				mov eax,[edi+SIZE VALUE].VALUE.val
				cmp eax,[edi].VALUE.val
				jle __cmp_done_LE

				mov ecx,0

__cmp_done_LE:  mov [edi+SIZE VALUE],ecx
				add edi,SIZE VALUE

				OP_FETCH

;-----------------------------------------------------------------------------
do_EQ:                                 ;EQual (binary)

				mov ecx,1

				mov eax,[edi+SIZE VALUE].VALUE.val
				cmp eax,[edi].VALUE.val
				je __cmp_done_EQ

				mov ecx,0

__cmp_done_EQ:  mov [edi+SIZE VALUE],ecx
				add edi,SIZE VALUE

				OP_FETCH

;-----------------------------------------------------------------------------
do_NE:                                ;Not Equal (binary)

				mov ecx,1

				mov eax,[edi+SIZE VALUE].VALUE.val
				cmp eax,[edi].VALUE.val
				jne __cmp_done_NE

				mov ecx,0

__cmp_done_NE:  mov [edi+SIZE VALUE],ecx
				add edi,SIZE VALUE

				OP_FETCH

;-----------------------------------------------------------------------------
do_GE:                                ;Greater than or Equal (binary)

				mov ecx,1

				mov eax,[edi+SIZE VALUE].VALUE.val
				cmp eax,[edi].VALUE.val
				jge __cmp_done_GE

				mov ecx,0

__cmp_done_GE:  mov [edi+SIZE VALUE],ecx
				add edi,SIZE VALUE

				OP_FETCH

;-----------------------------------------------------------------------------
do_GT:                               ;Greather Than (binary)

				mov ecx,1

				mov eax,[edi+SIZE VALUE].VALUE.val
				cmp eax,[edi].VALUE.val
				jg __cmp_done_GT

				mov ecx,0

__cmp_done_GT:  mov [edi+SIZE VALUE],ecx
				add edi,SIZE VALUE

				OP_FETCH

;-----------------------------------------------------------------------------
do_INC:                                 ;INCrement (unary)

				inc [edi].VALUE.val

				OP_FETCH

;-----------------------------------------------------------------------------
do_DEC:                                 ;DECrement (unary)

				dec [edi].VALUE.val

				OP_FETCH

;-----------------------------------------------------------------------------
do_SHTC:                                ;Load SHorT Constant (0-256)

				mov eax,0
				lodsb

				mov [edi].VALUE.val,eax

				OP_FETCH

;-----------------------------------------------------------------------------
do_INTC:                                ;Load INTeger Constant (256-64K)

				mov eax,0
				lodsw

				mov [edi].VALUE.val,eax

				OP_FETCH

;-----------------------------------------------------------------------------
do_LNGC:                                ;Load LoNG Constant (64K-4G)

				lodsd
				mov [edi].VALUE.val,eax

				OP_FETCH

;-----------------------------------------------------------------------------
do_RCRS:                                ;Reference Code ReSource address

				xor ebx,ebx
				mov bx,WORD PTR [esi]   ;get index into external offset list
				add esi,2

				add ebx,extern_offset   ;add offst in thnk for this SOP's list

				mov edx,h_thunk         ;ebx -> HD_entry
				add ebx,[edx]           ;ebx -> external reference list entry

				mov ebx,[ebx]           ;set EBX -> address of code resource

				mov [edi].VALUE.addr_flat,ebx ;jrl

				OP_FETCH

;-----------------------------------------------------------------------------
do_CALL:                                ;CALL code resource handler

				mov ebx,fptr            ;initialize global 'this' to allow
				xor eax,eax
				mov ax,WORD PTR [ebx-OFF_THIS]   ;array location in code resources
				mov current_this,eax

				xor ecx,ecx
				mov cl,BYTE PTR [esi]   ;get argument count
				inc esi

				mov edx,ecx             ;save # of args

				or ecx,ecx
				jz __call

__push_args:
				push [edi].VALUE.val
				add edi,SIZE VALUE
				dec ecx
				jz __call

				push [edi].VALUE.val
				add edi,SIZE VALUE
				dec ecx
				jz __call

				push [edi].VALUE.val
				add edi,SIZE VALUE
				dec ecx
				jz __call

				push [edi].VALUE.val
				add edi,SIZE VALUE
				dec ecx
				jz __call

				jmp __push_args

__call:         push edx                ;1st argument = # of arguments

				mov stk_off,edi         ;set up to create new stack frame

				call [edi].VALUE.addr_flat

				mov ecx,fptr             ;destroy stack frame
				mov stk_off,ecx

				mov [edi].VALUE.val,eax       ;copy returned value to AESOP stack

				pop edx                 ;pop # of args from CPU stack
				shl edx,2
				add esp,edx

				PS_DEREF
				OP_FETCH

;-----------------------------------------------------------------------------
do_SEND:                                ;SEND message

				xor ecx,ecx
				mov cl,BYTE PTR [esi]   ;get argument count

				mov ebx,edi             ;set up arguments in reverse order

				or ecx,ecx
				jz __send

__push_args_SEND:
				sub ebx,SIZE VALUE
				mov eax,[edi].VALUE.val
				mov [ebx].VALUE.val,eax
				add edi,SIZE VALUE
				dec ecx
				jz __send

				sub ebx,SIZE VALUE
				mov eax,[edi].VALUE.val
				mov [ebx].VALUE.val,eax
				add edi,SIZE VALUE
				dec ecx
				jz __send

				sub ebx,SIZE VALUE
				mov eax,[edi].VALUE.val
				mov [ebx].VALUE.val,eax
				add edi,SIZE VALUE
				dec ecx
				jz __send

				sub ebx,SIZE VALUE
				mov eax,[edi].VALUE.val
				mov [ebx].VALUE.val,eax
				add edi,SIZE VALUE
				dec ecx
				jz __send

				jmp __push_args_SEND

__send:         mov stk_off,ebx         ;set up to create new stack frame

				mov eax,-1
				push eax                ;push explicit vector offset (none)
				xor eax,eax
				mov ax,WORD PTR [esi+1] ;push message #
				push eax
				push [edi].VALUE.val    ;push instance handle

				call RT_execute
				add esp,12

				mov ecx,fptr            ;destroy stack frame
				mov stk_off,ecx

				mov [edi].VALUE.val,eax ;copy returned value to AESOP stack

				add esi,3               ;skip SEND operands

				PS_DEREF
				OP_FETCH

;-----------------------------------------------------------------------------
do_PASS:                                ;PASS message to parent class
				xor ecx,ecx
				mov cl,BYTE PTR [esi]   ;get argument count
				inc esi

				mov edx,h_thunk         ;edx -> HD_entry
				mov edx,[edx]           ;edx -> thunk

				mov ebx,cur_vector      ;get mem offset of current msg vector
				add ebx,SIZE MV_entry   ;index next higher-level handler

				cmp bx,[edx].THDR.SD_list  ;valid vector?
				jae __exit_PASS         ;no, past end of list

				add ebx,edx             ;ebx -> MV_entry
				mov eax,[Message]       ;same message type?
				cmp [ebx].MV_entry.msg_num,ax
				jne __exit_PASS         ;no, no higher-level handlers present

				push ebx                ;else push explicit vector offset

				mov ebx,edi             ;set up arguments in reverse order

				or ecx,ecx
				jz __pass

__push_args_PASS:
				REPT 3
				sub ebx,SIZE VALUE
				mov eax,[edi].VALUE.val
				mov [ebx].VALUE.val,eax
				add edi,SIZE VALUE
				dec ecx
				jz __pass
				ENDM
				jmp __push_args_PASS

__pass:         mov stk_off,ebx         ;set up to create new stack frame

				push [Message]          ;push message #
				push [Index]            ;push instance index
				call RT_execute
				add esp,12

				mov ecx,fptr            ;destroy stack frame
				mov stk_off,ecx

				mov [edi].VALUE.val,eax       ;copy returned value to AESOP stack

__exit_PASS:    PS_DEREF
				OP_FETCH

;-----------------------------------------------------------------------------
do_JSR:                                 ;Jump to SubRoutine

				lodsw                   ;AX = target offset

				push ds32
				push esi                ;push current PC
				push edi                ;push current SP
				push fptr               ;push current stack frame pointer

				xor esi,esi
				mov si,ax               ;set PC = target offset
				add esi,ds32

				mov fptr,edi            ;FPTR = stack pointer at entry

				mov eax,[Index]         ;initialize manifest THIS var
				mov [edi-OFF_THIS],ax

				xor edx,edx
				mov dx,[esi].MHDR.auto_size     ;make room for auto vars
				sub edi,edx                     ;make room for auto vars
				sub edi,SIZE VALUE

				add esi,SIZE MHDR       ;skip header

				OP_FETCH

;-----------------------------------------------------------------------------
do_RTS:                                 ;ReTurn from Subroutine

				mov eax,[edi].VALUE.val       ;top of stack = return value

				pop fptr                ;pop old stack frame pointer
				pop edi                 ;pop old SP
				pop esi                 ;pop old PC
				pop ds32

				mov [edi].VALUE.val,eax       ;establish value and return

				PS_DEREF
				OP_FETCH

;-----------------------------------------------------------------------------
do_AIM:                                 ;Array Index Multiply

				xor eax,eax
				mov ax,[edi].VALUE.val_l        ;max array index < 64K

				mul WORD PTR [esi]
				add esi,2

				add edi,SIZE VALUE

				add [edi].VALUE.val,eax

				OP_FETCH

;-----------------------------------------------------------------------------                ENDP
do_AIS:                                 ;Array Index Shift

				xor eax,eax
				mov ax,[edi].VALUE.val_l        ;max array index < 64K

				xor ecx,ecx
				mov cl,BYTE PTR [esi]
				inc esi

				shl eax,cl

				add edi,SIZE VALUE

				add [edi].VALUE.val,eax

				OP_FETCH

;-----------------------------------------------------------------------------
do_LTBA:                                ;Load Table Byte Array

				xor ebx,ebx
				mov bx,WORD PTR [esi]
				add esi,2
				add ebx,ds32

				movsx edx,[edi].VALUE.val_l
				add ebx,edx

				movsx eax,BYTE PTR [ebx]        ;jrl

				mov [edi].VALUE.val,eax

				OP_FETCH

;-----------------------------------------------------------------------------
do_LTWA:                                ;Load Table Word Array

				xor ebx,ebx
				mov bx,WORD PTR [esi]
				add esi,2
				add ebx,ds32

				movsx edx,[edi].VALUE.val_l
				add ebx,edx

				movsx eax,WORD PTR [ebx]           ;jrl

				mov [edi].VALUE.val,eax

				OP_FETCH

;-----------------------------------------------------------------------------
do_LTDA:                               ;Load Table Dword Array

				xor ebx,ebx
				mov bx,WORD PTR [esi]
				add esi,2
				add ebx,ds32

				movsx edx,[edi].VALUE.val_l
				add ebx,edx

				mov eax,[ebx]                      ;jrl

				mov [edi].VALUE.val,eax

				OP_FETCH

;-----------------------------------------------------------------------------
do_LETA:                               ;Load Effective Table Address

				xor ebx,ebx
				mov bx,WORD PTR [esi]
				add esi,2
				add ebx,ds32

				mov [edi].VALUE.addr_flat,ebx ;jrl

				OP_FETCH

;-----------------------------------------------------------------------------
do_LAB:                                ;Load Auto Byte

				mov ebx,fptr

				movsx edx,WORD PTR [esi]
				sub ebx,edx
				add esi,2

				movsx eax,BYTE PTR [ebx]

				mov [edi].VALUE.val,eax

				OP_FETCH

;-----------------------------------------------------------------------------
do_LAW:                                ;Load Auto Word

				mov ebx,fptr

				movsx edx,WORD PTR [esi]
				sub ebx,edx
				add esi,2

				movsx eax,WORD PTR [ebx]

				mov [edi].VALUE.val,eax

				OP_FETCH

;-----------------------------------------------------------------------------
do_LAD:                                ;Load Auto Dword

				mov ebx,fptr

				movsx edx,WORD PTR [esi]
				sub ebx,edx
				add esi,2

				mov eax,[ebx]

				mov [edi].VALUE.val,eax

				OP_FETCH

;-----------------------------------------------------------------------------
do_SAB:                                ;Store Auto Byte

				mov ebx,fptr

				movsx edx,WORD PTR [esi]
				sub ebx,edx
				add esi,2

				mov ax,[edi].VALUE.val_l

				mov [ebx],al

				OP_FETCH

;-----------------------------------------------------------------------------
do_SAW:                               ;Store Auto Word

				mov ebx,fptr

				movsx edx,WORD PTR [esi]
				sub ebx,edx
				add esi,2

				mov ax,[edi].VALUE.val_l

				mov [ebx],ax

				OP_FETCH

;-----------------------------------------------------------------------------
do_SAD:                              ;Store Auto Dword

				mov ebx,fptr

				movsx edx,WORD PTR [esi]
				sub ebx,edx
				add esi,2

				mov eax,[edi].VALUE.val

				mov [ebx],eax

				OP_FETCH

;-----------------------------------------------------------------------------                ENDP
do_LABA:                           ;Load Auto Byte Array

				mov ebx,fptr

				movsx edx,WORD PTR [esi]
				sub ebx,edx
				add esi,2

				movsx edx,[edi].VALUE.val_l
				add ebx,edx

				movsx eax,BYTE PTR [ebx]

				mov [edi].VALUE.val,eax

				OP_FETCH

;-----------------------------------------------------------------------------                ENDP
do_LAWA:                                ;Load Auto Word Array

				mov ebx,fptr

				movsx edx,WORD PTR [esi]
				sub ebx,edx
				add esi,2

				movsx edx,[edi].VALUE.val_l
				add ebx,edx

				movsx eax,WORD PTR [ebx]

				mov [edi].VALUE.val,eax

				OP_FETCH

;-----------------------------------------------------------------------------                ENDP
do_LADA:                               ;Load Auto Dword Array

				mov ebx,fptr

				movsx edx,WORD PTR [esi]
				sub ebx,edx
				add esi,2

				movsx edx,[edi].VALUE.val_l
				add ebx,edx

				mov eax,[ebx]

				mov [edi].VALUE.val,eax

				OP_FETCH

;-----------------------------------------------------------------------------                ENDP
do_SABA:                              ;Store Auto Byte Array

				mov eax,[edi].VALUE.val

				add edi,SIZE VALUE

				mov ebx,fptr

				movsx edx,WORD PTR [esi]
				sub ebx,edx
				add esi,2

				movsx edx,[edi].VALUE.val_l
				add ebx,edx

				mov [ebx],al            ;store data

				mov [edi].VALUE.val,eax       ;store copy of data over old offset

				OP_FETCH

;-----------------------------------------------------------------------------                ENDP
do_SAWA:                                ;Store Auto Word Array

				mov eax,[edi].VALUE.val

				add edi,SIZE VALUE

				mov ebx,fptr

				movsx edx,WORD PTR [esi]
				sub ebx,edx
				add esi,2

				movsx edx,[edi].VALUE.val_l
				add ebx,edx

				mov [ebx],ax            ;store data

				mov [edi].VALUE.val,eax       ;store copy of data over old offset

				OP_FETCH

;-----------------------------------------------------------------------------                ENDP
do_SADA:                                ;Store Auto Dword Array

				mov eax,[edi].VALUE.val

				add edi,SIZE VALUE

				mov ebx,fptr

				movsx edx,WORD PTR [esi]
				sub ebx,edx
				add esi,2

				movsx edx,[edi].VALUE.val_l
				add ebx,edx

				mov [ebx],eax           ;store data

				mov [edi].VALUE.val,eax       ;store copy of data over old offset

				OP_FETCH

;-----------------------------------------------------------------------------
do_LEAA:                                ;Load Effective Auto Address

				mov ebx,fptr

				movsx edx,WORD PTR [esi]
				sub ebx,edx
				add esi,2

				mov [edi].VALUE.addr_flat,ebx

				OP_FETCH

;-----------------------------------------------------------------------------
do_LSB:                                 ;Load Static Byte

				movzx ebx,WORD PTR [esi]
				add esi,2

				add ebx,static_offset   ;get index into static variable area

				mov edx,h_instance       ;edx -> HD_entry
				add ebx,[edx]            ;ebx -> variable

				movsx eax,BYTE PTR [ebx]

				mov [edi].VALUE.val,eax

				OP_FETCH

;-----------------------------------------------------------------------------
do_LSW:                                 ;Load Static Word

				movzx ebx,WORD PTR [esi]
				add esi,2

				add ebx,static_offset    ;get index into static variable area

				mov edx,h_instance       ;edx -> HD_entry
				add ebx,[edx]            ;ebx -> variable

				movsx eax,WORD PTR [ebx]

				mov [edi].VALUE.val,eax

				OP_FETCH

;-----------------------------------------------------------------------------
do_LSD:                                 ;Load Static Dword

				movzx ebx,WORD PTR [esi]
				add esi,2

				add ebx,static_offset   ;get index into static variable area

				mov edx,h_instance       ;edx -> HD_entry
				add ebx,[edx]            ;ebx -> variable

				mov eax,[ebx]

				mov [edi].VALUE.val,eax

				OP_FETCH

;-----------------------------------------------------------------------------
do_SSB:                                 ;Store Static Byte

				movzx ebx,WORD PTR [esi]
				add esi,2

				add ebx,static_offset    ;get index into static variable area

				mov edx,h_instance       ;edx -> HD_entry
				add ebx,[edx]            ;ebx -> variable

				mov ax,[edi].VALUE.val_l

				mov [ebx],al

				OP_FETCH

;-----------------------------------------------------------------------------
do_SSW:                                 ;Store Static Word

				movzx ebx,WORD PTR [esi]
				add esi,2

				add ebx,static_offset    ;get index into static variable area

				mov edx,h_instance       ;edx -> HD_entry
				add ebx,[edx]            ;ebx -> variable

				mov ax,[edi].VALUE.val_l

				mov [ebx],ax

				OP_FETCH

;-----------------------------------------------------------------------------
do_SSD:                                 ;Store Static Dword

				movzx ebx,WORD PTR [esi]
				add esi,2

				add ebx,static_offset   ;get index into static variable area

				mov edx,h_instance       ;edx -> HD_entry
				add ebx,[edx]            ;ebx -> variable

				mov eax,[edi].VALUE.val

				mov [ebx],eax

				OP_FETCH

;-----------------------------------------------------------------------------                ENDP
do_LSBA:                                ;Load Static Byte Array

				movzx ebx,WORD PTR [esi]
				add esi,2

				add ebx,static_offset   ;get index into static variable area

				mov edx,h_instance       ;edx -> HD_entry
				add ebx,[edx]            ;ebx -> variable

				movzx edx,[edi].VALUE.val_l
				add ebx,edx

				movsx eax,BYTE PTR [ebx]

				mov [edi].VALUE.val,eax

				OP_FETCH

;-----------------------------------------------------------------------------                ENDP
do_LSWA:                                ;Load Static Word Array

				movzx ebx,WORD PTR [esi]
				add esi,2

				add ebx,static_offset    ;get index into static variable area

				mov edx,h_instance       ;edx -> HD_entry
				add ebx,[edx]            ;ebx -> variable

				movzx edx,[edi].VALUE.val_l
				add ebx,edx

				movsx eax,WORD PTR [ebx]

				mov [edi].VALUE.val,eax

				OP_FETCH

;-----------------------------------------------------------------------------                ENDP
do_LSDA:                                ;Load Static Dword Array

				movzx ebx,WORD PTR [esi]
				add esi,2

				add ebx,static_offset   ;get index into static variable area

				mov edx,h_instance       ;edx -> HD_entry
				add ebx,[edx]            ;ebx -> variable

				movzx edx,[edi].VALUE.val_l
				add ebx,edx

				mov eax,[ebx]

				mov [edi].VALUE.val,eax

				OP_FETCH

;-----------------------------------------------------------------------------                ENDP
do_SSBA:                                ;Store Static Byte Array

				mov eax,[edi].VALUE.val

				add edi,SIZE VALUE

				movzx ebx,WORD PTR [esi]
				add esi,2

				add ebx,static_offset   ;get index into static variable area

				mov edx,h_instance       ;edx -> HD_entry
				add ebx,[edx]            ;ebx -> variable

				movzx edx,[edi].VALUE.val_l
				add ebx,edx

				mov [ebx],al            ;store data

				mov [edi].VALUE.val,eax       ;store copy of data over old offset

				OP_FETCH

;-----------------------------------------------------------------------------                ENDP
do_SSWA:                                ;Store Static Word Array

				mov eax,[edi].VALUE.val

				add edi,SIZE VALUE

				movzx ebx,WORD PTR [esi]
				add esi,2

				add ebx,static_offset   ;get index into static variable area

				mov edx,h_instance       ;edx -> HD_entry
				add ebx,[edx]            ;ebx -> variable

				movzx edx,[edi].VALUE.val_l
				add ebx,edx

				mov [ebx],ax            ;store data

				mov [edi].VALUE.val,eax       ;store copy of data over old offset

				OP_FETCH

;-----------------------------------------------------------------------------                ENDP
do_SSDA:                                ;Store Static Dword Array

				mov eax,[edi].VALUE.val

				add edi,SIZE VALUE

				movzx ebx,WORD PTR [esi]
				add esi,2

				add ebx,static_offset   ;get index into static variable area

				mov edx,h_instance       ;edx -> HD_entry
				add ebx,[edx]            ;ebx -> variable

				movzx edx,[edi].VALUE.val_l
				add ebx,edx

				mov [ebx],eax           ;store data

				mov [edi].VALUE.val,eax       ;store copy of data over old offset

				OP_FETCH

;-----------------------------------------------------------------------------
do_LESA:                                ;Load Effective Static Address

				movzx ebx,WORD PTR [esi];get offset address of array base
				add esi,2

				add ebx,static_offset    ;add static variable base address

				mov edx,h_instance       ;edx -> HD_entry
				add ebx,[edx]            ;ebx -> variable

				mov [edi].VALUE.addr_flat,ebx

				OP_FETCH

;-----------------------------------------------------------------------------
do_LXB:                                 ;Load eXtern Byte

				movzx ebx,WORD PTR [esi];get index into external offset list
				add esi,2

				add ebx,extern_offset   ;add base address for this SOP's list

				mov edx,h_thunk         ;ebx -> HD_entry
				add ebx,[edx]           ;ebx -> external list in thunk

				movzx ecx,WORD PTR [ebx];set CX->variable in instance

				movzx ebx,WORD PTR [edi];get index to reference
				shl ebx,2               ;*4 to index object list
				add ebx,objlist
				mov ebx,[ebx]           ;get instance handle to reference

				mov ebx,[ebx]           ;ebx -> instance

				add ebx,ecx             ;ebx -> variable in instance
				movsx eax,BYTE PTR [ebx];fetch the byte and save it

				mov [edi].VALUE.val,eax

				OP_FETCH

;-----------------------------------------------------------------------------
do_LXW:                                 ;Load eXtern Word

				movzx ebx,WORD PTR [esi];get index into external offset list
				add esi,2

				add ebx,extern_offset   ;add base address for this SOP's list

				mov edx,h_thunk         ;ebx -> HD_entry
				add ebx,[edx]           ;ebx -> extern list in thunk

				movzx ecx,WORD PTR [ebx];set CX->variable in instance

				movzx ebx,WORD PTR [edi];get index to reference
				shl ebx,2               ;*4 to index object list
				add ebx,objlist
				mov ebx,[ebx]           ;get instance handle to reference

				mov ebx,[ebx]           ;ebx -> instance

				add ebx,ecx             ;ebx -> variable in instance
				movsx eax,WORD PTR [ebx];fetch the word and save it

				mov [edi].VALUE.val,eax

				OP_FETCH

;-----------------------------------------------------------------------------
do_LXD:                                 ;Load eXtern Dword

				movzx ebx,WORD PTR [esi];get index into external offset list
				add esi,2

				add ebx,extern_offset   ;add base address for this SOP's list

				mov edx,h_thunk         ;ebx -> HD_entry
				add ebx,[edx]           ;ebx -> variable

				movzx ecx,WORD PTR [ebx];set CX->variable in instance

				movzx ebx,WORD PTR [edi];get index to reference
				shl ebx,2
				add ebx,objlist
				mov ebx,[ebx]           ;get instance handle to reference

				mov ebx,[ebx]           ;ebx -> instance

				add ebx,ecx             ;ebx -> variable
				mov eax,[ebx]           ;fetch the dword and save it

				mov [edi].VALUE.val,eax

				OP_FETCH

;-----------------------------------------------------------------------------
do_SXB:                                 ;Store eXtern Byte

				mov eax,[edi].VALUE.val       ;get value to store

				add edi,SIZE VALUE

				movzx ebx,WORD PTR [esi];get index into external offset list
				add esi,2

				add ebx,extern_offset   ;add base address for this SOP's list

				mov edx,h_thunk         ;ebx -> HD_entry
				add ebx,[edx]

				movzx ecx,WORD PTR [ebx];set CX->variable in instance

				movzx ebx,WORD PTR [edi];get index to reference
				shl ebx,2               ;*4 to index object list
				add ebx,objlist

				mov ebx,[ebx]           ;get instance handle to reference
				mov ebx,[ebx]           ;EBX -> element

				add ebx,ecx
				mov [ebx],al            ;store data

				mov [edi].VALUE.val,eax       ;store copy of data over old handle

				OP_FETCH

;-----------------------------------------------------------------------------
do_SXW:                                 ;Store eXtern Word

				mov eax,[edi].VALUE.val       ;get value to store

				add edi,SIZE VALUE

				movzx ebx,WORD PTR [esi];get index into external offset list
				add esi,2

				add ebx,extern_offset   ;add base address for this SOP's list

				mov edx,h_thunk         ;ebx -> HD_entry
				add ebx,[edx]

				movzx ecx,WORD PTR [ebx];set CX->variable in instance

				movzx ebx,WORD PTR [edi];get index to reference
				shl ebx,2               ;*4 to index object list
				add ebx,objlist
				mov ebx,[ebx]           ;get instance handle to reference

				mov ebx,[ebx]           ;EBX -> element

				add ebx,ecx
				mov [ebx],ax            ;store data

				mov [edi].VALUE.val,eax       ;store copy of data over old handle

				OP_FETCH

;-----------------------------------------------------------------------------
do_SXD:                                 ;Store eXtern Dword

				mov eax,[edi].VALUE.val       ;get value to store

				add edi,SIZE VALUE

				movzx ebx,WORD PTR [esi];get index into external offset list
				add esi,2

				add ebx,extern_offset   ;add base address for this SOP's list

				mov edx,h_thunk         ;ebx -> HD_entry
				add ebx,[edx]

				movzx ecx,WORD PTR [ebx];set CX->variable in instance

				movzx ebx,WORD PTR [edi];get index to reference
				shl ebx,2               ;*4 to index object list
				add ebx,objlist
				mov ebx,[ebx]           ;get instance handle to reference

				mov ebx,[ebx]           ;EBX -> element

				add ebx,ecx
				mov [ebx],eax           ;store data

				mov [edi].VALUE.val,eax       ;store copy of data over old handle

				OP_FETCH

;-----------------------------------------------------------------------------
do_LXBA:                                ;Load eXtern Byte Array

				movzx ebx,WORD PTR [esi];get index into external offset list
				add esi,2

				add ebx,extern_offset   ;add base address for this SOP's list

				mov edx,h_thunk         ;ebx -> HD_entry
				add ebx,[edx]

				movzx ecx,WORD PTR [ebx];set CX = array base in instance
				movzx edx,WORD PTR [edi+2];add array index
				add ecx,edx

				movzx ebx,WORD PTR [edi];get index to reference
				shl ebx,2               ;*4 to index object list
				add ebx,objlist
				mov ebx,[ebx]           ;get instance handle to reference

				mov ebx,[ebx]           ;EBX -> element

				add ebx,ecx
				movsx eax,BYTE PTR [ebx]

				mov [edi].VALUE.val,eax

				OP_FETCH

;-----------------------------------------------------------------------------
do_LXWA:                                ;Load eXtern Word Array

				movzx ebx,WORD PTR [esi];get index into external offset list
				add esi,2

				add ebx,extern_offset   ;add base address for this SOP's list

				mov edx,h_thunk         ;ebx -> HD_entry
				add ebx,[edx]

				movzx ecx,WORD PTR [ebx];set CX = array base in instance
				movzx edx,WORD PTR [edi+2];add array index
				add ecx,edx

				movzx ebx,WORD PTR [edi];get index to reference
				shl ebx,2               ;*4 to index object list
				add ebx,objlist
				mov ebx,[ebx]           ;get instance handle to reference

				mov ebx,[ebx]           ;EBX -> element

				add ebx,ecx
				movsx eax,WORD PTR [ebx]

				mov [edi].VALUE.val,eax

				OP_FETCH

;-----------------------------------------------------------------------------
do_LXDA:                                ;Load eXtern Dword Array

				movzx ebx,WORD PTR [esi];get index into external offset list
				add esi,2

				add ebx,extern_offset   ;add base address for this SOP's list

				mov edx,h_thunk         ;ebx -> HD_entry
				add ebx,[edx]

				movzx ecx,WORD PTR [ebx];set CX = array base in instance
				movzx edx,WORD PTR [edi+2];add array index
				add ecx,edx

				movzx ebx,WORD PTR [edi];get index to reference
				shl ebx,2               ;*4 to index object list
				add ebx,objlist
				mov ebx,[ebx]           ;get instance handle to reference

				mov ebx,[ebx]           ;EBX -> element

				add ebx,ecx
				mov eax,[ebx]

				mov [edi].VALUE.val,eax

				OP_FETCH

;-----------------------------------------------------------------------------
do_SXBA:                                ;Store eXtern Byte Array

				mov eax,[edi].VALUE.val       ;get value to store

				add edi,SIZE VALUE

				movzx ebx,WORD PTR [esi];get index into external offset list
				add esi,2

				add ebx,extern_offset   ;add base address for this SOP's list

				mov edx,h_thunk         ;ebx -> HD_entry
				add ebx,[edx]

				movzx ecx,WORD PTR [ebx];set CX = array base in instance
				movzx edx,WORD PTR [edi+2];add array index
				add ecx,edx

				movzx ebx,WORD PTR [edi];get index to reference
				shl ebx,2               ;*4 to index object list
				add ebx,objlist
				mov ebx,[ebx]             ;get instance handle to reference

				mov ebx,[ebx]           ;EBX -> element

				add ebx,ecx
				mov [ebx],al            ;store data

				mov [edi].VALUE.val,eax       ;store copy of data over old offset

				OP_FETCH

;-----------------------------------------------------------------------------
do_SXWA:                                ;Store eXtern Word Array

				mov eax,[edi].VALUE.val       ;get value to store

				add edi,SIZE VALUE

				movzx ebx,WORD PTR [esi];get index into external offset list
				add esi,2

				add ebx,extern_offset   ;add base address for this SOP's list

				mov edx,h_thunk         ;ebx -> HD_entry
				add ebx,[edx]

				movzx ecx,WORD PTR [ebx];set CX = array base in instance
				movzx edx,WORD PTR [edi+2];add array index
				add ecx,edx

				movzx ebx,WORD PTR [edi];get index to reference
				shl ebx,2               ;*4 to index object list
				add ebx,objlist
				mov ebx,[ebx]           ;get instance handle to reference

				mov ebx,[ebx]           ;EBX -> element

				add ebx,ecx
				mov [ebx],ax            ;store data

				mov [edi].VALUE.val,eax       ;store copy of data over old offset

				OP_FETCH

;-----------------------------------------------------------------------------
do_SXDA:                                ;Store eXtern Dword Array

				mov eax,[edi].VALUE.val       ;get value to store

				add edi,SIZE VALUE

				movzx ebx,WORD PTR [esi]             ;get index into external offset list
				add esi,2

				add ebx,extern_offset   ;add base address for this SOP's list

				mov edx,h_thunk         ;ebx -> HD_entry
				add ebx,[edx]

				movzx ecx,WORD PTR [ebx];set CX = array base in instance
				movzx edx,WORD PTR [edi+2];add array index
				add ecx,edx

				movzx ebx,WORD PTR [edi]          ;get index to reference
				shl ebx,2               ;*4 to index object list
				add ebx,objlist
				mov ebx,[ebx]           ;get instance handle to reference

				mov ebx,[ebx]           ;EBX -> element

				add ebx,ecx
				mov [ebx],eax           ;store data

				mov [edi].VALUE.val,eax       ;store copy of data over old offset

				OP_FETCH

;-----------------------------------------------------------------------------
do_LEXA:                                ;Load Effective eXtern Address

				movzx ebx,WORD PTR [esi];get index into external offset list
				add esi,2

				add ebx,extern_offset   ;add base address for this SOP's list

				mov edx,h_thunk         ;ebx -> HD_entry
				add ebx,[edx]

				movzx ecx,WORD PTR [ebx];set CX = array base in instance

				movzx ebx,WORD PTR [edi]          ;get index to reference
				shl ebx,2               ;*4 to index object list
				add ebx,objlist
				mov ebx,[ebx]             ;get instance handle to reference

				mov ebx,[ebx]           ;EBX -> element
				add ebx,ecx

				mov [edi].VALUE.val,ebx       ;store address

				OP_FETCH

;-----------------------------------------------------------------------------
do_SXAS:                                ;Set eXtern Array Source

				movzx eax,[edi].VALUE.val_l   ;get index
				add edi,SIZE VALUE

				mov [edi].VALUE.val_h,ax      ;merge index with source expr result

				OP_FETCH

;-----------------------------------------------------------------------------
do_LECA:                                ;Load Effective Code Address

				mov eax,0
				lodsw
				add eax,ds32

				mov [edi].VALUE.val,eax       ;set offset portion of address

				OP_FETCH

;-----------------------------------------------------------------------------
do_SOLE:                                ;Selector for Object List Entry

				movzx ebx,[edi].VALUE.val_l   ;get entry #
				shl ebx,2               ;*4 to index object list

				mov ecx,esi
				mov esi,[objlist]
				mov eax,[esi][ebx]      ;get instance handle from object list
				mov esi,ecx

				mov [edi].VALUE.val,eax       ;(-1 if no instance at list entry)

				OP_FETCH

;-----------------------------------------------------------------------------
do_END:                                 ;END of handler
				push h_prg
				call RTR_unlock
				add esp,4

				mov eax,[edi].VALUE.val       ;set EAX = return value

				ret

;-----------------------------------------------------------------------------
do_BRK:                                 ;BReaKpoint for debugging

				movzx eax,[edi].VALUE.val_l     ;set EAX = value at top stack

				int 3

				OP_FETCH

;-----------------------------------------------------------------------------

op_dispatch     LABEL DWORD             ;Vectors to opcode handlers

				dd do_BRT               ;BRanch if True
				dd do_BRF               ;BRanch if False
				dd do_BRA               ;BRanch Always
				dd do_CASE              ;CASE selection
				dd do_PUSH              ;PUSH 0 onto stack
				dd do_DUP               ;DUPlicate top of stack
				dd do_NOT               ;Logical NOT (unary)
				dd do_SETB              ;SET Boolean value (unary)
				dd do_NEG               ;NEGate (unary)
				dd do_ADD               ;ADD (binary)
				dd do_SUB               ;SUBtract (binary)
				dd do_MUL               ;MULtiply (binary)
				dd do_DIV               ;DIVide (binary)
				dd do_MOD               ;MODulus (binary)
				dd do_EXP               ;EXPonent (binary)
				dd do_BAND              ;Bitwise AND (binary)
				dd do_BOR               ;Bitwise OR (binary)
				dd do_XOR               ;Bitwise XOR (binary)
				dd do_BNOT              ;Bitwise NOT (unary)
				dd do_SHL               ;SHift Left (binary)
				dd do_SHR               ;SHift Right (binary)
				dd do_LT                ;Less Than (binary)
				dd do_LE                ;Less than or Equal (binary)
				dd do_EQ                ;EQual (binary)
				dd do_NE                ;Not Equal (binary)
				dd do_GE                ;Greater than or Equal (binary)
				dd do_GT                ;Greather Than (binary)
				dd do_INC               ;INCrement (unary)
				dd do_DEC               ;DECrement (unary)
				dd do_SHTC              ;Load SHorT Constant (0-256)
				dd do_INTC              ;Load INTeger Constant (256-64K)
				dd do_LNGC              ;Load LoNG Constant (64K-4G)
				dd do_RCRS              ;Reference Code ReSource address
				dd do_CALL              ;CALL code resource handler
				dd do_SEND              ;SEND message
				dd do_PASS              ;PASS message to parent class
				dd do_JSR               ;Jump to SubRoutine
				dd do_RTS               ;ReTurn from Subroutine
				dd do_AIM               ;Array Index Multiply
				dd do_AIS               ;Array Index Shift
				dd do_LTBA              ;Load Table Byte Array
				dd do_LTWA              ;Load Table Word Array
				dd do_LTDA              ;Load Table Dword Array
				dd do_LETA              ;Load Effective Table Address
				dd do_LAB               ;Load Auto Byte
				dd do_LAW               ;Load Auto Word
				dd do_LAD               ;Load Auto Dword
				dd do_SAB               ;Store Auto Byte
				dd do_SAW               ;Store Auto Word
				dd do_SAD               ;Store Auto Dword
				dd do_LABA              ;Load Auto Byte Array
				dd do_LAWA              ;Load Auto Word Array
				dd do_LADA              ;Load Auto Dword Array
				dd do_SABA              ;Store Auto Byte Array
				dd do_SAWA              ;Store Auto Word Array
				dd do_SADA              ;Store Auto Dword Array
				dd do_LEAA              ;Load Effective Auto Address
				dd do_LSB               ;Load Static Byte
				dd do_LSW               ;Load Static Word
				dd do_LSD               ;Load Static Dword
				dd do_SSB               ;Store Static Byte
				dd do_SSW               ;Store Static Word
				dd do_SSD               ;Store Static Dword
				dd do_LSBA              ;Load Static Byte Array
				dd do_LSWA              ;Load Static Word Array
				dd do_LSDA              ;Load Static Dword Array
				dd do_SSBA              ;Store Static Byte Array
				dd do_SSWA              ;Store Static Word Array
				dd do_SSDA              ;Store Static Dword Array
				dd do_LESA              ;Load Effective Static Address
				dd do_LXB               ;Load eXtern Byte
				dd do_LXW               ;Load eXtern Word
				dd do_LXD               ;Load eXtern Dword
				dd do_SXB               ;Store eXtern Byte
				dd do_SXW               ;Store eXtern Word
				dd do_SXD               ;Store eXtern Dword
				dd do_LXBA              ;Load eXtern Byte Array
				dd do_LXWA              ;Load eXtern Word Array
				dd do_LXDA              ;Load eXtern Dword Array
				dd do_SXBA              ;Store eXtern Byte Array
				dd do_SXWA              ;Store eXtern Word Array
				dd do_SXDA              ;Store eXtern Dword Array
				dd do_LEXA              ;Load Effective eXtern Address
				dd do_SXAS              ;Set eXtern Array Source
				dd do_LECA              ;Load Effective Code Address
				dd do_SOLE              ;Selector for Object List Entry
				dd do_END               ;END of handler
				dd do_BRK               ;BReaKpoint for debugging

				dd 256-(($-op_dispatch)/4) dup (invalid_opcode)

;-----------------------------------------------------------------------------

RT_execute      ENDP
*/
}

} // namespace Aesop
