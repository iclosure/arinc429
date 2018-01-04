NAME		FXjmpt

extrn		code (ISR_Sudav, ISR_Sof, ISR_Sutok, ISR_Susp, ISR_Ures, ISR_IBN, ISR_Ep0in, ISR_Ep0out, ISR_Ep1in, ISR_Ep1out, ISR_Ep2in, ISR_Ep2out, ISR_Ep3in, ISR_Ep3out, ISR_Ep4in, ISR_Ep4out, ISR_Ep5in, ISR_Ep5out, ISR_Ep6in, ISR_Ep6out, ISR_Ep7in, ISR_Ep7out)
extrn		code (ISR_AINPF, ISR_BINPF, ISR_AOUTPF, ISR_BOUTPF, ISR_AINEF, ISR_BINEF, ISR_AOUTEF, ISR_BOUTEF, ISR_AINFF, ISR_BINFF, ISR_AOUTFF, ISR_BOUTFF, ISR_GPIFDONE, ISR_GPIFWF, ISR_DMADONE)
public		USB_AutoVector, USB_Jump_Table, INT4_AutoVector, INT4_Jump_Table
;------------------------------------------------------------------------------
; Interrupt Vectors
;------------------------------------------------------------------------------
		CSEG	AT 43H
USB_AutoVector	equ	$ + 2
		ljmp	USB_Jump_Table	; Autovector will replace byte 45
		CSEG	AT 53H
INT4_AutoVector	equ	$ + 2
		ljmp	INT4_Jump_Table	; Autovector will replace byte 55
;------------------------------------------------------------------------------
; USB Jump Table
;------------------------------------------------------------------------------
?PR?USB_JUMP_TABLE?USBJT	segment	code page	; Place jump table on a page boundary
		RSEG 	?PR?USB_JUMP_TABLE?USBJT	; autovector jump table
USB_Jump_Table:	
		ljmp	ISR_Sudav	; Setup Data Available
		db	0		
		ljmp	ISR_Sof	; Start of Frame
		db	0
		ljmp	ISR_Sutok	; Setup Data Loading
		db	0
		ljmp	ISR_Susp	; Global Suspend
		db 	0
		ljmp	ISR_Ures	; USB Reset  	
		db	0
		ljmp	ISR_IBN 	; Reserved
		db	0
		ljmp	ISR_Ep0in	; End Point 0 In
		db	0
		ljmp	ISR_Ep0out	; End Point 0 Out
		db	0
		ljmp	ISR_Ep1in	; End Point 1 In
		db	0
		ljmp	ISR_Ep1out	; End Point 1 Out
		db	0
		ljmp	ISR_Ep2in
		db	0
		ljmp	ISR_Ep2out
		db	0
		ljmp	ISR_Ep3in
		db	0
		ljmp	ISR_Ep3out
		db	0
		ljmp	ISR_Ep4in
		db	0
		ljmp	ISR_Ep4out
		db	0
		ljmp	ISR_Ep5in
		db	0
		ljmp	ISR_Ep5out
		db	0
		ljmp	ISR_Ep6in
		db	0
		ljmp	ISR_Ep6out
		db	0
		ljmp	ISR_Ep7in
		db	0
		ljmp	ISR_Ep7out
		db	0

		org USB_Jump_Table + 0x80

INT4_Jump_Table:
		ljmp	ISR_AINPF
		db	0
		ljmp	ISR_BINPF
		db	0
		ljmp	ISR_AOUTPF
		db	0
		ljmp	ISR_BOUTPF
		db	0
		ljmp	ISR_AINEF
		db	0
		ljmp	ISR_BINEF
		db	0
		ljmp	ISR_AOUTEF
		db	0
		ljmp	ISR_BOUTEF
		db	0
		ljmp	ISR_AINFF
		db	0
		ljmp	ISR_BINFF
		db	0
		ljmp	ISR_AOUTFF
		db	0
		ljmp	ISR_BOUTFF
		db	0
		ljmp	ISR_GPIFDONE
		db	0
		ljmp	ISR_GPIFWF
		db	0
		ljmp	ISR_DMADONE

		end
