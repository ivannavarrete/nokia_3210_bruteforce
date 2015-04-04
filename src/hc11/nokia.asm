
; motorola.asm
;
; MCU: 68HC11A1
; mode: bootstrap
;
; This program is used to brute force the 'security code' on a nokia 3210
; phone. This is done by stepping through each key, generating the corresponding
; pulses on the appropriate ports.
;
; The security code on the 3210 is 5 digits. The total number of keys is then:
;
;	10 + 10^2 + 10^3 + 10^4 + 10^5 = 111110
;
; There is nothing sacred with the 3210. It could be replaced by any phone,
; or pretty much anything else for that matter. All this program does is
; generates pulses on ports. Got nothin to do with phones whatsoever.

; Implementation notes:
; Since this program is transferred to the MCU at every restart, it uses only
; the 256 bytes of RAM that the bootstrap code downloads. Note that we need some
; space for the stack at the top of the RAM. Thus, the code plus stack must not
; exceed 256 bytes.


PORTA		equ	$0000
PORTB		equ	$0004

; Since we have 11 buttons (and one diode), we use two ports.
; PORTB
BTN_0		equ	$01
BTN_1		equ	$02
BTN_2		equ	$04
BTN_3		equ	$08
BTN_4		equ	$10
BTN_5		equ	$20
BTN_6		equ	$40
BTN_7		equ	$80
; PORTA
BTN_8		equ	$08
BTN_9		equ $10
BTN_ENTER	equ	$20
DIODE		equ	$40



	org $0000
start:
;	jsr		init
;	jsr		get_key_block
	jsr		brute_force
	jsr		end


;=== init ======================================================================
; Init SCI and ports.
;===============================================================================
;init:
;	ldx		#$1000			; X points to registers
;	rts


;=== get_key_block =============================================================
; Get a keyspace block from the PC.
;===============================================================================
;get_key_block:

;	rts


;=== brute_force ===============================================================
; Test all keys in the keyspace block.
;===============================================================================
brute_force:
	ldy		#5001			; test this many keys per round
_try_again:
	jsr		send_key
	jsr		next_key
	dey
	bne		_try_again

	rts


;=== next_key ==================================================================
; Calculate the next key, and store it in RAM.
;===============================================================================
next_key:
	psha
	pshb
	pshx
	pshy
	
	; point Y to least significant digit
	ldab	key_size
_inc:
	ldy		#key
	aby
	dey
	; get digit
	ldaa	0,Y
	inca
	cmpa	#9
	ble		_store
	; OK, we have an overflow ..
	clr		0,Y				; set this digit to 0
	decb					; decrement key_size counter
	bne		_inc
	; oops, we've tested all keys with this key size ..
	inc		key_size		; increment key size
	ldab	key_size
	cmpb	#10				; can't test keys with more than 10 digits
	ble		_zero
	jsr		end
	; zero the key
_zero:
	ldy		#$0000
	sty		key
	sty		key+2
	sty		key+4
	sty		key+6
	sty		key+8
	bra		_done

_store:
	staa	0,Y

_done:
	puly
	pulx
	pulb
	pula
	rts


;=== send_key ==================================================================
; Send key to phone.
;===============================================================================
send_key:
	psha
	pshb
	pshy

	ldy		#key
	ldaa	key_size
	beq		_exit

_next_digit:
	; get a digit from key
	ldab	0,Y
	; convert to appropriate pulse
	; depending on digit, we send a pulse to either PORTA or PORTB
	pshx
	ldx		#btn_table
	abx							; if AccB is > 9 then we're screwed
	cmpb	#$07
	bgt		_porta
_portb:
	ldab	0,X
	pulx
	stab	PORTB,X				; press button
	bra		_dly
_porta:
	ldab	0,X
	pulx
	stab	PORTA,X				; press button
_dly:
	jsr		sdelay
	clrb
	stab	PORTA,X				; release button
	stab	PORTB,X
	jsr		sdelay

	iny							; point to next digit
	deca						; one more digit done
	bne		_next_digit

_exit:	
	; press the 'enter' button
	jsr		sdelay				; extra delays for this button ...
	ldaa	#BTN_ENTER
	staa	PORTA,X
	jsr		sdelay
	clr		PORTA,X
	jsr		sdelay
	jsr		sdelay

	puly
	pulb
	pula
	rts


;=== ldelay/sdelay =============================================================
; long/short delay.
;===============================================================================
ldelay:
	pshx
	ldx		#$FFFF
	bra		_sdelay1

sdelay:
	pshx
	ldx		#$3800
_sdelay1:
	dex
	bne		_sdelay1
	pulx
	rts


;=== end =======================================================================
; Flash a diode.
;===============================================================================
end:
	clra
	staa	PORTB,X
_flash:
	staa	PORTA,X
	jsr		ldelay
	bset	PORTA,X DIODE
	jsr		ldelay
	bra		_flash


;===============================================================================
; keys tested:
;	0-979999
;
; keys confirmed:
;	0-79999
;
; possible anomalies detected in keys:
;	50000-52999		; strong
;	53000-54999		; weak
;	70000-74999		; very weak
;	75000-79999		; strong
;	80000-84999		; weak (1)
;===============================================================================
key_size:		fcb	5
key:			fcb 8, 5, 0, 0, 0, 0, 0, 0, 0, 0 

btn_table:
btn_0:			fcb	#BTN_0 
btn_1:			fcb	#BTN_1
btn_2:			fcb	#BTN_2
btn_3:			fcb	#BTN_3
btn_4:			fcb	#BTN_4
btn_5:			fcb	#BTN_5
btn_6:			fcb	#BTN_6
btn_7:			fcb	#BTN_7
btn_8:			fcb	#BTN_8
btn_9:			fcb	#BTN_9


; This is needed to properly parse the s-file..
	org $FF
code_end:		fcb	0
