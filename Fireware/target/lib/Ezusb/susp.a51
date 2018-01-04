NAME		SUSP
PUBLIC		EZUSB_SUSP

$include (..\..\inc\ezregs.inc)

EZUSB		segment	code

		rseg	EZUSB		
EZUSB_SUSP:	mov	dptr,#USBCS	; Clear the Wake Source bit in
		movx	a,@dptr		; the USBCS register
		orl	a,#80h
		movx	@dptr,a

		orl	PCON,#00000001b	; Place the processor in idle

		nop			; Insert some meaningless instruction
		nop			; fetches to insure that the processor
		nop			; suspends and resumes before RET
		nop
		nop
er_end:		ret
		end
