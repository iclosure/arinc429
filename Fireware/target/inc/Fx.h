//-----------------------------------------------------------------------------
//	File:		Fx.h
//	Contents:	EZ-USB register declarations and bit mask definitions.
//
//	Copyright (c) 1999 Cypress Semiconductor, All rights reserved
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Fx Related Register Assignments
//-----------------------------------------------------------------------------

#ifdef ALLOCATE_EXTERN
#define EXTERN
#define _AT_ _at_
#else
#define EXTERN extern
#define _AT_ ;/ ## /
#endif

EXTERN xdata volatile BYTE AINDATA		_AT_	0x7800; // FIFO A Read Data
EXTERN xdata volatile BYTE AINBC		_AT_	0x7801; // FIFO A In Byte Count
EXTERN xdata volatile BYTE AINPF		_AT_	0x7802; // FIFO A In Programmable Flag
EXTERN xdata volatile BYTE AINPFPIN		_AT_	0x7803; // FIFO A In Pin Progr Flag
EXTERN xdata volatile BYTE BINDATA		_AT_	0x7805; // FIFO B Read Data
EXTERN xdata volatile BYTE BINBC		_AT_	0x7806; // FIFO B In Byte Count
EXTERN xdata volatile BYTE BINPF		_AT_	0x7807; // FIFO B In Programmable Flag
EXTERN xdata volatile BYTE BINPFPIN		_AT_	0x7808; // FIFO B In Pin Progr Flag

EXTERN xdata volatile BYTE ABINTF		_AT_	0x780A; // Input FIFO A/B Flags & Toggle
EXTERN xdata volatile BYTE ABINIE		_AT_	0x780B; // Input FIFO A/B Int Enables
#define bmAINPF		0x20
#define bmAINEF		0x10
#define bmAINFF		0x08
#define bmBINPF		0x04
#define bmBINEF		0x02
#define bmBINFF		0x01

EXTERN xdata volatile BYTE ABINIRQ		_AT_	0x780C; // Input FIFO A/B Int Requests

EXTERN xdata volatile BYTE AOUTDATA		_AT_	0x780E; // FIFO A Out Write Data
EXTERN xdata volatile BYTE AOUTBC		_AT_	0x780F; // FIFO A Out Byte Count
EXTERN xdata volatile BYTE AOUTPF		_AT_	0x7810; // FIFO A Out Programmable Flag
EXTERN xdata volatile BYTE AOUTPFPIN		_AT_	0x7811; // FIFO A Out Pin Progr Flag
EXTERN xdata volatile BYTE BOUTDATA		_AT_	0x7813; // FIFO B Out Write Data
EXTERN xdata volatile BYTE BOUTBC		_AT_	0x7814; // FIFO B Out Byte Count
EXTERN xdata volatile BYTE BOUTPF		_AT_	0x7815; // FIFO B Out Programmable Flag
EXTERN xdata volatile BYTE BOUTPFPIN   		_AT_	0x7816; // FIFO B Out Pin Progr Flag

EXTERN xdata volatile BYTE ABOUTCS		_AT_	0x7818; // Output FIFO A/B Flags & Toggle
EXTERN xdata volatile BYTE ABOUTIE		_AT_	0x7819; // Output FIFO A/B Int Enables
EXTERN xdata volatile BYTE ABOUTIRQ		_AT_	0x781A; // Output FIFO A/B Int Requests
#define bmAOUTPF	0x20
#define bmAOUTEF	0x10
#define bmAOUTFF	0x08
#define bmBOUTPF	0x04
#define bmBOUTEF	0x02
#define bmBOUTFF	0x01


EXTERN xdata volatile BYTE ABSETUP		_AT_	0x781C; // FIFO A/B Setup
EXTERN xdata volatile BYTE ABPOLAR		_AT_	0x781D; // FIFO signal polarities
EXTERN xdata volatile BYTE ABFLUSH		_AT_	0x781E; // Reset all FIFO Flags


EXTERN xdata volatile BYTE WFSELECT		_AT_	0x7824;
EXTERN xdata volatile BYTE IDLE_CS		_AT_	0x7825;
EXTERN xdata volatile BYTE IDLE_CTLOUT	        _AT_	0x7826;
EXTERN xdata volatile BYTE CTLOUTCFG		_AT_	0x7827;
EXTERN xdata volatile BYTE GPIFADRL	     	_AT_	0x782A;
EXTERN xdata volatile BYTE GPIFADRH	 _AT_	0x782B;
EXTERN xdata volatile BYTE AINTC		_AT_	0x782C;
EXTERN xdata volatile BYTE AOUTTC		_AT_	0x782D;
EXTERN xdata volatile BYTE ATRIG		_AT_	0x782E;
EXTERN xdata volatile BYTE BINTC		_AT_	0x7830;
EXTERN xdata volatile BYTE BOUTTC		_AT_	0x7831;
EXTERN xdata volatile BYTE BTRIG		_AT_	0x7832;
EXTERN xdata volatile BYTE SGLDATH		_AT_	0x7834;
EXTERN xdata volatile BYTE SGLDATLTRIG	 	_AT_	0x7835;
EXTERN xdata volatile BYTE SGLDATLNTRIG  	_AT_	0x7836;
EXTERN xdata volatile BYTE READY		_AT_	0x7838;
EXTERN xdata volatile BYTE ABORT 		_AT_	0x7839;
EXTERN xdata volatile BYTE GENIE		_AT_	0x783B;
// for both GENIE and GENIRQ
#define bmDMADONE	  bmBIT2	
#define	bmGPIFWF	  bmBIT1
#define	bmGPIFDONE	  bmBIT0

EXTERN xdata volatile BYTE GENIRQ		_AT_	0x783C;
EXTERN xdata volatile BYTE OUTD		     	_AT_	0x7841;
EXTERN xdata volatile BYTE PINSD		_AT_	0x7842;
EXTERN xdata volatile BYTE OED			_AT_	0x7843;
EXTERN xdata volatile BYTE OUTE		     	_AT_	0x7845;
EXTERN xdata volatile BYTE PINSE		_AT_	0x7846;
EXTERN xdata volatile BYTE OEE			_AT_	0x7847;
EXTERN xdata volatile BYTE PORTSETUP	 	_AT_	0x7849;

EXTERN xdata volatile BYTE IFCONFIG		_AT_	0x784A; // Interface Configuration
								// bmBIT0 IF0
								// bmBIT1 IF1
								// bmBIT2 BUS16
								// bmBIT3 GSTATE
								// bmBIT7 52One
EXTERN xdata volatile BYTE PORTACF2		_AT_	0x784B; // PORTA Alternate Config #2
								// bmBIT4 SLWR
								// bmBIT5 SLRD
EXTERN xdata volatile BYTE PORTCCF2		_AT_	0x784C; // PORTC Alternate Config #2
								// bmBIT0 RDY0
								// bmBIT1 RDY1
								// bmBIT3 RDY3
								// bmBIT4 CTRL1
								// bmBIT5 CTRL3
								// bmBIT6 CTRL4
								// bmBIT7 CTRL5

EXTERN xdata volatile WORD DMASRC		_AT_	0x784F;
//EXTERN xdata volatile BYTE DMASRCH		_AT_	0x784F; // DMA Source Address (H)
//EXTERN xdata volatile BYTE DMASRCL		_AT_	0x7850; // DMA Source Address (L)
EXTERN xdata volatile WORD DMADEST		_AT_	0x7851;
//EXTERN xdata volatile BYTE DMADESTH		_AT_	0x7851; // DMA Destination Address (H)
//EXTERN xdata volatile BYTE DMADESTL		_AT_	0x7852; // DMA Destination Address (L)
EXTERN xdata volatile BYTE DMALEN		_AT_	0x7854; // DMA Transfer Length
EXTERN xdata volatile BYTE DMAGO		_AT_	0x7855; // DMA Start and Status

EXTERN xdata volatile BYTE DMABURST	     _AT_	0x7857; // DMA Burst Mode; for ext DMA transfers
								// bmBIT0 BW
								// bmBIT1 BR
								// bmBIT2 DSTR0
								// bmBIT3 DSTR1
								// bmBIT4 DSTR2
EXTERN xdata volatile BYTE DMAEXTFIFO	 _AT_	0x7858;
EXTERN xdata volatile BYTE INT4IVEC	     _AT_	0x785D; // Interrupt 4 AutoVector
EXTERN xdata volatile BYTE INT4SETUP	 _AT_	0x785E; // Interrupt 4 Setup
#define		INT4SFC		   bmBIT2	
#define		INT4_INTERNAL  bmBIT1
#define		INT4_AV4EN	   bmBIT0

// New bits in USBBAV
#define		INT2SFC		   bmBIT4
EXTERN xdata volatile BYTE WFDESC[0x80]  _AT_	0x7900;

EXTERN xdata volatile BYTE I2CMODE	 _AT_	0x7FA7;		// STOPIE and 400KHZ bits
EXTERN xdata volatile BYTE IBNIRQ	 _AT_	0x7FB0;  	// IN-bulk-NAK IRQ
EXTERN xdata volatile BYTE IBNIEN	 _AT_	0x7FB1;         // IN-bulk-NAK Int enable

//EXTERN xdata volatile BYTE FASTXFR		 _AT_	0x7FE2; // Fast XFer; for external DMA transfers
								// bmBIT0 WMOD0
								// bmBIT1 WMOD1
								// bmBIT2 WPOL
								// bmBIT3 RMOD0
								// bmBIT4 RMOD1
								// bmBIT5 RPOL
								// bmBIT6 FBLK
								// bmBIT7 FISO
                                                                
// The TUFT provides 8 memory mapped control registers to configure the FIFO tests
//EXTERN xdata volatile BYTE TUFT_SEL		_AT_	0x7900; // TUFT Register Addr Select
//EXTERN xdata volatile BYTE TUFT_SEL		_AT_	0x8000; // TUFT Register Addr Select
//EXTERN xdata volatile BYTE TUFT_CONF_A	_AT_	0x8001; // Configure FIFO A test
//								// bmBIT0 FA_ENA_COUNT
//								// bmBIT1 FA_CONTINUOUS
//								// bmBIT2 FA_ENA_RELOAD
//								// bmBIT3 FA_DIR
//								// bmBIT4 FA_HANDSHAKE
//								// bmBIT5 FA_EAGER
//EXTERN xdata volatile BYTE TUFT_CONF_B	_AT_	0x8002; // Configure FIFO B test
//								// bmBIT0 FA_ENA_COUNT
//								// bmBIT1 FA_CONTINUOUS
//								// bmBIT2 FA_ENA_RELOAD
//								// bmBIT3 FA_DIR
//								// bmBIT4 FA_HANDSHAKE
//								// bmBIT5 FA_EAGER
//EXTERN xdata volatile BYTE TUFT_MODE	_AT_	0x8003; // Strobe, FIFO test Modes
//								// bmBIT 1:0 STROBE_ACTIVE
//								// bmBIT 2:3 STROBE_INACTIVE
//								// bmBIT4 16_BIT_FIFO_MODE 
//							       	// bmBIT5 16_BIT_BUS_MODE 
//EXTERN xdata volatile BYTE COUNT_DOWN_A	_AT_	0x8004; // PreLoad Count Down Value
//EXTERN xdata volatile BYTE COUNT_DOWN_B	_AT_	0x8005; // PreLoad Count Down Value
//EXTERN xdata volatile BYTE COUNT_WRAP	_AT_	0x8006; // PreLoad Wrap Count Value

//EXTERN xdata volatile BYTE CPUCS		_AT_	0x7F92; // TBD: I2C Mode
	#define CPUCS_48MHZ  bmBIT3				
	#define CPUCS_CLKINV bmBIT2
//EXTERN xdata volatile BYTE CONFIG0		_AT_	0x7FA6; // TBD: Config0
								// bmBIT0 400KHZ
//EXTERN xdata volatile BYTE I2CMODE		_AT_	0x7FA7; // TBD: I2C Mode
								// bmBIT0 400KHZ
								// bmBIT1 STOPIE
                                
                                
// Note:  These registers will not work until
// bit 0 in PORTSETUP is set.
sfr	IOA	= 0x80;
sfr	IOB	= 0x90;
sfr	IOC	= 0xA0;
sfr	IOD	= 0xB0;
sfr	IOE	= 0xB1;

sfr	SOEA	= 0xB2;
sfr	SOEB	= 0xB3;
sfr	SOEC	= 0xB4;
sfr	SOED	= 0xB5;
sfr	SOEE	= 0xB6;

// Note -- Requires bit 4 in USBBAV to be set!
sfr	INT2CLR	= 0xA1;

// Note -- Requires bit 2 in INT4SETUP to be set!
sfr	INT4CLR	= 0xA2;

#ifdef AGGRESSIVELY_USE_TNG_FEATURES
// Note these features will not work unless they are enabled via the 
// USBBAV, INT4SETUP and PORTSETUP are initialized to enable them.
	#define OUTA IOA
	#define OUTB IOB
	#define OUTC IOC
	#define OUTD IOD
	#define OUTE IOE
	#define PINSA IOA
	#define PINSB IOB
	#define PINSC IOC
	#define PINSD IOD
	#define PINSE IOE

	#define OEA SOEA
	#define	OEB	SOEB
	#define	OEC	SOEC
	#define	OED	SOED
	#define	OEE	SOEE

	// Globally change USBIRQ clearing to INT2CLR clearing
	#undef USBIRQ
	#define USBIRQ INT2CLR
#endif

#undef EXTERN
#undef _AT_
