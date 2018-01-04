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
#define EPINIRQ IN07IRQ
#define EPOUTIRQ OUT07IRQ
#define EPINIEN IN07IEN
#define EPOUTIEN OUT07IEN
#define GLOBIEN USBIEN
#define GLOBCTL USBBAV
#define FRML USBFRAMEL
#define FRMH USBFRAMEH
#define EPCTL USBPAIR
#define IEPVAL IN07VAL
#define OEPVAL OUT07VAL
#define ISOIVAL INISOVAL
#define ISOOVAL OUTISOVAL

//  Rename the iso start addresses
#define EP8INST IN8ADDR
#define EP9INST IN9ADDR
#define EP10INST IN10ADDR
#define EP11INST IN11ADDR
#define EP12INST IN12ADDR
#define EP13INST IN13ADDR
#define EP14INST IN14ADDR
#define EP15INST IN15ADDR

#define EP8OUTST OUT8ADDR
#define EP9OUTST OUT9ADDR
#define EP10OUTST OUT10ADDR
#define EP11OUTST OUT11ADDR
#define EP12OUTST OUT12ADDR
#define EP13OUTST OUT13ADDR
#define EP14OUTST OUT14ADDR
#define EP15OUTST OUT15ADDR

//	 Rename the control	status bytes
// #define EP0INCS IN0CS
// #define EP1INCS IN1CS
// #define EP2INCS IN2CS
// #define EP3INCS IN3CS
// #define EP4INCS IN4CS
// #define EP5INCS IN5CS
// #define EP6INCS IN6CS
// #define EP7INCS IN7CS
#define EP8INCS IN8CS
#define EP9INCS IN9CS
#define EP10INCS IN10CS
#define EP11INCS IN11CS
#define EP12INCS IN12CS
#define EP13INCS IN13CS
#define EP14INCS IN14CS
#define EP15INCS IN15CS

#define EP8OUTCS OUT8CS
#define EP9OUTCS OUT9CS
#define EP10OUTCS OUT10CS
#define EP11OUTCS OUT11CS
#define EP12OUTCS OUT12CS
#define EP13OUTCS OUT13CS
#define EP14OUTCS OUT14CS
#define EP15OUTCS OUT15CS
//#define EP0OUTCS OUT0CS
//#define EP1OUTCS OUT1CS
//#define EP2OUTCS OUT2CS
//#define EP3OUTCS OUT3CS
//#define EP4OUTCS OUT4CS
//#define EP5OUTCS OUT5CS
//#define EP6OUTCS OUT6CS
//#define EP7OUTCS OUT7CS
    
//	 Rename the ISO Byte counts
#define EP8BCH OUT8BCH
#define EP9BCH OUT9BCH
#define EP10BCH OUT10BCH
#define EP11BCH OUT11BCH
#define EP12BCH OUT12BCH
#define EP13BCH OUT13BCH
#define EP14BCH OUT14BCH
#define EP15BCH OUT15BCH

#define EP8BCL OUT8BCL
#define EP9BCL OUT9BCL
#define EP10BCL OUT10BCL
#define EP11BCL OUT11BCL
#define EP12BCL OUT12BCL
#define EP13BCL OUT13BCL
#define EP14BCL OUT14BCL
#define EP15BCL OUT15BCL
//		 Rename the Bulk Byte counts
//#define EP0INBC IN0BC
//#define EP1INBC IN1BC
//#define EP2INBC IN2BC
//#define EP3INBC IN3BC
//#define EP4INBC IN4BC
//#define EP5INBC IN5BC
//#define EP6INBC IN6BC
//#define EP7INBC IN7BC
//
//#define EP0OUTBC OUT0BC
//#define EP1OUTBC OUT1BC
//#define EP2OUTBC OUT2BC
//#define EP3OUTBC OUT3BC
//#define EP4OUTBC OUT4BC
//#define EP5OUTBC OUT5BC
//#define EP6OUTBC OUT6BC
//#define EP7OUTBC OUT7BC

//	 Rename the ISOOUTER register
#define ISOOUTERR ISOERR
    
//	 Rename the iso EP in and EP out buffers
#define EP8OUTD OUT8DATA
#define EP9OUTD OUT9DATA
#define EP10OUTD OUT10DATA
#define EP11OUTD OUT11DATA
#define EP12OUTD OUT12DATA
#define EP13OUTD OUT13DATA
#define EP14OUTD OUT14DATA
#define EP15OUTD OUT15DATA

#define EP8IND IN8DATA
#define EP9IND IN9DATA
#define EP10IND IN10DATA
#define EP11IND IN11DATA
#define EP12IND IN12DATA
#define EP13IND IN13DATA
#define EP14IND IN14DATA
#define EP15IND IN15DATA
    
//	 Rename the EP in and EP out buffers.  This needs to go last, because
//	 it is a subset of many other names
#define EP0OUT OUT0BUF
#define EP1OUT OUT1BUF
#define EP2OUT OUT2BUF
#define EP3OUT OUT3BUF
#define EP4OUT OUT4BUF
#define EP5OUT OUT5BUF
#define EP6OUT OUT6BUF
#define EP7OUT OUT7BUF

#define EP0IN IN0BUF
#define EP1IN IN1BUF
#define EP2IN IN2BUF
#define EP3IN IN3BUF
#define EP4IN IN4BUF
#define EP5IN IN5BUF
#define EP6IN IN6BUF
#define EP7IN IN7BUF

/* End point control offsets */
#define	EP0IN_ID IN0BUF_ID
#define	EP1IN_ID IN1BUF_ID
#define	EP2IN_ID IN2BUF_ID
#define	EP3IN_ID IN3BUF_ID
#define	EP4IN_ID IN4BUF_ID
#define	EP5IN_ID IN5BUF_ID
#define	EP6IN_ID IN6BUF_ID
#define	EP7IN_ID IN7BUF_ID
#define	EP0OUT_ID OUT0BUF_ID
#define	EP1OUT_ID OUT1BUF_ID
#define	EP2OUT_ID OUT2BUF_ID
#define	EP3OUT_ID OUT3BUF_ID
#define	EP4OUT_ID OUT4BUF_ID
#define	EP5OUT_ID OUT5BUF_ID
#define	EP6OUT_ID OUT6BUF_ID
#define	EP7OUT_ID OUT7BUF_ID


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
