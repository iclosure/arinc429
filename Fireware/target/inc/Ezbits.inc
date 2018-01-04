;
; EZ-USB Bit Assignments
;
; GLOBIEN
;
bSUDAV		equ	00000001b
bSOF		equ	00000010b
bSUDLD		equ	00000100b
bSUSP		equ	00001000b
bURES		equ	00010000b
;
; GLOBIEN
;
SUDAVIE		equ	0
SOFIE		equ	1
SUDLDIE		equ	2
SUSPIE		equ	3
URESIE		equ	4
;
; IREQ3
;
SUDAVIR		equ	0
SOFIR		equ	1
SUDLDIR		equ	2
SUSPIR		equ	3
URESIR		equ	4
;
; GLOBCTL (was AVBPSU)
;
AVEN		equ	0
BPEN		equ	1
BPPULSE		equ	2
BREAK		equ	3
;
; EP0-7 C&S registers
;	
Stall		equ	0
Busy		equ	1
bStall		equ	00000001b
bBusy		equ	00000010b
;
; EPCTL register
;
PR2IN		equ	0
PR4IN		equ	1
PR6IN		equ	2
PR2OUT		equ	3
PR4OUT		equ	4
PR6OUT		equ	5
ISOSEND0	equ	7
;
; IDBCTL register
;
DBUG		equ	0
MAN		equ	1
FRZ		equ	2
PPSTAT		equ	3
;
; USBCS Register
;
SIGRSUME	equ	0
RENUM		equ	1
DISCON		equ	2
  
