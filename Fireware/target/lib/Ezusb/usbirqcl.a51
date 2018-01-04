NAME		USBIRQCL
PUBLIC		EZUSB_IRQ_CLEAR

$include (..\..\inc\ezregs.inc)

EZUSB		segment	code

		rseg	EZUSB		
EZUSB_IRQ_CLEAR:
		mov	a,#0efH
		anl	EXIF,a
		ret
		end
