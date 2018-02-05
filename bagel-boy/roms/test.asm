	SECTION "Org $00", HOME[$00]
RST_00:
	jp	$100
	
	SECTION "Org $08", HOME[$08]
RST_08	jp	$100

	SECTION "Org $18", HOME[$18]
RST_18:
	jp	$100
	
	SECTION "Org $20", HOME[$20]
RST_20	jp	$100

	SECTION "Org $30", HOME[$30]
RST_30	jp	$100

	SECTION "Org $38", HOME[$38]
RST_38:
	jp	$100

	SECTION "V-Blank IRQ Vector", HOME[$40]
VBL_VECT:
	reti

	SECTION "LCD IRQ Vector", HOME[$48]
LCD_VECT:
	reti

	SECTION "Timer IRQ Vector", HOME[$50]
TIMER_VECT:
	reti

	SECTION "Serial IRQ Vector", HOME[$58]
SERIAL_VECT:
	reti

	SECTION "Joypad IRQ Vector", HOME[$60]
JOYPAD_VECT:
	reti

	SECTION "Start", HOME[$100]
	nop
	jp Start

	DB	$CE,$ED,$66,$66,$CC,$0D,$00,$0B,$03,$73,$00,$83,$00,$0C,$00,$0D
	DB	$00,$08,$11,$1F,$88,$89,$00,$0E,$DC,$CC,$6E,$E6,$DD,$DD,$D9,$99
	DB	$BB,$BB,$67,$63,$6E,$0E,$EC,$CC,$DD,$DC,$99,$9F,$BB,$B9,$33,$3E

	DB "BagelBytes", 0
	
	DB $00

	DB $00

	DB $00

	DB $19

	DB $01

	DB $00

	DB $00

	DB $33

	DB $00
	
	DB $00

	DW $00

	SECTION "Program Start", HOME[$0150]
Start::
	ld a, $00
        call Test_Inc_A
        jp Hang_CPU

Test_Inc_A::
        inc a
        cp $ff
        call Test_Inc_A
        ret z
        
Hang_CPU::
    jp Hang_CPU