SETBIT		MACRO 	ad,dat
		mov	dptr,#ad
		movx	a,@dptr
		setb	acc.dat
		movx	@dptr,a
		ENDM
;
CLRBIT		MACRO	ad,dat
		mov	dptr,#ad
		movx	a,@dptr
		clr	acc.dat
		movx	@dptr,a
		ENDM
;
CLR_USB_IRQ	MACRO
		mov	a,EXIF
		clr	acc.4
		mov	EXIF,a
		ENDM

PUSHALL		MACRO
		push	dpl
		push 	dph
		push	dpl1
		push	dph1
		push	acc
		ENDM
;
POPALL		MACRO
		pop	acc
		pop	dph1
		pop	dpl1
		pop	dph
		pop	dpl
		ENDM


