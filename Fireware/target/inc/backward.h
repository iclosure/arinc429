//////////////////////////////////////////////////////////////////////
//
//	backward.h -- Maintain backward compatability with OLD reg names
//
//	Author: SteveK
//
//  Copyright (c) 1998, Anchor Chips, Inc.
//  May be modified and reused ONLY in systems containing an Anchor Chips device.
//  See the license agreement for more details.
//
//////////////////////////////////////////////////////////////////////

// Rename the random control structures
#define	IN07IRQ	EPINIRQ
#define	OUT07IRQ	EPOUTIRQ
#define	IN07IEN	EPINIEN
#define	OUT07IEN	EPOUTIEN
#define	USBIEN	GLOBIEN
#define	USBBAV	GLOBCTL
#define	USBFRAMEL	FRML
#define	USBFRAMEH	FRMH
#define	USBPAIR	EPCTL
#define	IN07VAL	IEPVAL
#define	OUT07VAL	OEPVAL
#define	INISOVAL	ISOIVAL
#define	OUTISOVAL	ISOOVAL

//  Rename the iso start addresses
#define	IN8ADDR	EP8INST
#define	IN9ADDR	EP9INST
#define	IN10ADDR	EP10INST
#define	IN11ADDR	EP11INST
#define	IN12ADDR	EP12INST
#define	IN13ADDR	EP13INST
#define	IN14ADDR	EP14INST
#define	IN15ADDR	EP15INST

#define	OUT8ADDR	EP8OUTST
#define	OUT9ADDR	EP9OUTST
#define	OUT10ADDR	EP10OUTST
#define	OUT11ADDR	EP11OUTST
#define	OUT12ADDR	EP12OUTST
#define	OUT13ADDR	EP13OUTST
#define	OUT14ADDR	EP14OUTST
#define	OUT15ADDR	EP15OUTST

//	 Rename the control	status bytes
// #define	IN0CS	EP0INCS
// #define	IN1CS	EP1INCS
// #define	IN2CS	EP2INCS
// #define	IN3CS	EP3INCS
// #define	IN4CS	EP4INCS
// #define	IN5CS	EP5INCS
// #define	IN6CS	EP6INCS
// #define	IN7CS	EP7INCS
#define	IN8CS	EP8INCS
#define	IN9CS	EP9INCS
#define	IN10CS	EP10INCS
#define	IN11CS	EP11INCS
#define	IN12CS	EP12INCS
#define	IN13CS	EP13INCS
#define	IN14CS	EP14INCS
#define	IN15CS	EP15INCS

#define	OUT8CS	EP8OUTCS
#define	OUT9CS	EP9OUTCS
#define	OUT10CS	EP10OUTCS
#define	OUT11CS	EP11OUTCS
#define	OUT12CS	EP12OUTCS
#define	OUT13CS	EP13OUTCS
#define	OUT14CS	EP14OUTCS
#define	OUT15CS	EP15OUTCS
//#define	OUT0CS	EP0OUTCS
//#define	OUT1CS	EP1OUTCS
//#define	OUT2CS	EP2OUTCS
//#define	OUT3CS	EP3OUTCS
//#define	OUT4CS	EP4OUTCS
//#define	OUT5CS	EP5OUTCS
//#define	OUT6CS	EP6OUTCS
//#define	OUT7CS	EP7OUTCS
    
//	 Rename the ISO Byte counts
#define	OUT8BCH	EP8BCH
#define	OUT9BCH	EP9BCH
#define	OUT10BCH	EP10BCH
#define	OUT11BCH	EP11BCH
#define	OUT12BCH	EP12BCH
#define	OUT13BCH	EP13BCH
#define	OUT14BCH	EP14BCH
#define	OUT15BCH	EP15BCH

#define	OUT8BCL	EP8BCL
#define	OUT9BCL	EP9BCL
#define	OUT10BCL	EP10BCL
#define	OUT11BCL	EP11BCL
#define	OUT12BCL	EP12BCL
#define	OUT13BCL	EP13BCL
#define	OUT14BCL	EP14BCL
#define	OUT15BCL	EP15BCL
//		 Rename the Bulk Byte counts
//#define	IN0BC	EP0INBC
//#define	IN1BC	EP1INBC
//#define	IN2BC	EP2INBC
//#define	IN3BC	EP3INBC
//#define	IN4BC	EP4INBC
//#define	IN5BC	EP5INBC
//#define	IN6BC	EP6INBC
//#define	IN7BC	EP7INBC
//
//#define	OUT0BC	EP0OUTBC
//#define	OUT1BC	EP1OUTBC
//#define	OUT2BC	EP2OUTBC
//#define	OUT3BC	EP3OUTBC
//#define	OUT4BC	EP4OUTBC
//#define	OUT5BC	EP5OUTBC
//#define	OUT6BC	EP6OUTBC
//#define	OUT7BC	EP7OUTBC

//	 Rename the ISOOUTER register
#define	ISOERR	ISOOUTERR
    
//	 Rename the iso EP in and EP out buffers
#define	OUT8DATA	EP8OUTD
#define	OUT9DATA	EP9OUTD
#define	OUT10DATA	EP10OUTD
#define	OUT11DATA	EP11OUTD
#define	OUT12DATA	EP12OUTD
#define	OUT13DATA	EP13OUTD
#define	OUT14DATA	EP14OUTD
#define	OUT15DATA	EP15OUTD

#define	IN8DATA	EP8IND
#define	IN9DATA	EP9IND
#define	IN10DATA	EP10IND
#define	IN11DATA	EP11IND
#define	IN12DATA	EP12IND
#define	IN13DATA	EP13IND
#define	IN14DATA	EP14IND
#define	IN15DATA	EP15IND
    
//	 Rename the EP in and EP out buffers.  This needs to go last, because
//	 it is a subset of many other names
#define	OUT0BUF	EP0OUT
#define	OUT1BUF	EP1OUT
#define	OUT2BUF	EP2OUT
#define	OUT3BUF	EP3OUT
#define	OUT4BUF	EP4OUT
#define	OUT5BUF	EP5OUT
#define	OUT6BUF	EP6OUT
#define	OUT7BUF	EP7OUT

#define	IN0BUF	EP0IN
#define	IN1BUF	EP1IN
#define	IN2BUF	EP2IN
#define	IN3BUF	EP3IN
#define	IN4BUF	EP4IN
#define	IN5BUF	EP5IN
#define	IN6BUF	EP6IN
#define	IN7BUF	EP7IN

#define IN0CS	(*((char xdata *) 0x7FB4))	
#define IN0BC	(*((char xdata *) 0x7FB5))	
#define IN1CS	(*((char xdata *) 0x7FB6))	
#define IN1BC	(*((char xdata *) 0x7FB7))	
#define IN2CS	(*((char xdata *) 0x7FB8))	
#define IN2BC	(*((char xdata *) 0x7FB9))	
#define IN3CS	(*((char xdata *) 0x7FBA))	
#define IN3BC	(*((char xdata *) 0x7FBB))	
#define IN4CS	(*((char xdata *) 0x7FBC))	
#define IN4BC	(*((char xdata *) 0x7FBD))	
#define IN5CS	(*((char xdata *) 0x7FBE))	
#define IN5BC	(*((char xdata *) 0x7FBF))	
#define IN6CS	(*((char xdata *) 0x7FC0))	
#define IN6BC	(*((char xdata *) 0x7FC1))	
#define IN7CS	(*((char xdata *) 0x7FC2))	
#define IN7BC	(*((char xdata *) 0x7FC3))	

#define OUT0CS	(*((char xdata *) 0x7FC4))	
#define OUT0BC	(*((char xdata *) 0x7FC5))	
#define OUT1CS	(*((char xdata *) 0x7FC6))	
#define OUT1BC	(*((char xdata *) 0x7FC7))	
#define OUT2CS	(*((char xdata *) 0x7FC8))	
#define OUT2BC	(*((char xdata *) 0x7FC9))	
#define OUT3CS	(*((char xdata *) 0x7FCA))	
#define OUT3BC	(*((char xdata *) 0x7FCB))	
#define OUT4CS	(*((char xdata *) 0x7FCC))	
#define OUT4BC	(*((char xdata *) 0x7FCD))	
#define OUT5CS	(*((char xdata *) 0x7FCE))	
#define OUT5BC	(*((char xdata *) 0x7FCF))	
#define OUT6CS	(*((char xdata *) 0x7FD0))	
#define OUT6BC	(*((char xdata *) 0x7FD1))	
#define OUT7CS	(*((char xdata *) 0x7FD2))	
#define OUT7BC	(*((char xdata *) 0x7FD3))	
