;//-----------------------------------------------------------------------------
;//   File:      Fx.inc
;//   Contents:   EZ-USB FX register declarations
;//
;//   Copyright (c) 1999 Cypress Semiconductor, All rights reserved
;//-----------------------------------------------------------------------------
;//-----------------------------------------------------------------------------
;// Fx Related Register Assignments
;//-----------------------------------------------------------------------------

AINDATA     XDATA    07800h ; // FIFO A Read Data
AINBC       XDATA    07801h ; // FIFO A In Byte Count
AINPF       XDATA    07802h ; // FIFO A In Programmable Flag
AINPFPIN    XDATA    07803h ; // FIFO A In Pin Progr Flag
BINDATA     XDATA    07805h ; // FIFO B Read Data
BINBC       XDATA    07806h ; // FIFO B In Byte Count
BINPF       XDATA    07807h ; // FIFO B In Programmable Flag
BINPFPIN    XDATA    07808h ; // FIFO B In Pin Progr Flag

ABINTF      XDATA    0780Ah ; // Input FIFO A/B Flags & Toggle
ABINIE      XDATA    0780Bh ; // Input FIFO A/B Int Enables

ABINIRQ     XDATA   0780Ch ; // Input FIFO A/B Int Requests

AOUTDATA    XDATA   0780Eh ; // FIFO A Out Write Data
AOUTBC      XDATA   0780Fh ; // FIFO A Out Byte Count
AOUTPF      XDATA   07810h ; // FIFO A Out Programmable Flag
AOUTPFPIN   XDATA   07811h ; // FIFO A Out Pin Progr Flag
BOUTDATA    XDATA   07813h ; // FIFO B Out Write Data
BOUTBC      XDATA   07814h ; // FIFO B Out Byte Count
BOUTPF      XDATA   07815h ; // FIFO B Out Programmable Flag
BOUTPFPIN   XDATA   07816h ; // FIFO B Out Pin Progr Flag

ABOUTCS     XDATA   07818h ; // Output FIFO A/B Flags & Toggle
ABOUTIE     XDATA   07819h ; // Output FIFO A/B Int Enables
ABOUTIRQ    XDATA   0781Ah ; // Output FIFO A/B Int Requests

ABSETUP     XDATA   0781Ch ; // FIFO A/B Setup
ABPOLAR     XDATA   0781Dh ; // FIFO signal polarities
ABFLUSH     XDATA   0781Eh ; // Reset all FIFO Flags


WFSELECT    XDATA   07824h ;
IDLE_CS     XDATA   07825h ;
IDLE_CTLOUT   XDATA   07826h ;
CTLOUTCFG      XDATA   07827h ;
GPIFADRL           XDATA   0782Ah ;
GPIFADRH    XDATA   0782Bh ;
AINTC      XDATA   0782Ch ;
AOUTTC      XDATA   0782Dh ;
ATRIG      XDATA   0782Eh ;
BINTC      XDATA   07830h ;
BOUTTC      XDATA   07831h ;
BTRIG      XDATA   07832h ;
SGLDATH      XDATA   07834h ;
SGLDATLTRIG       XDATA   07835h ;
SGLDATLNTRIG     XDATA   07836h ;
READY      XDATA   07838h ;
ABORT       XDATA   07839h ;
GENIE      XDATA   0783Bh ;
;// for both GENIE and GENIRQ
;#define bmDMADONE     bmBIT2   
;#define   bmGPIFWF     bmBIT1
;#define   bmGPIFDONE     bmBIT0

GENIRQ      XDATA   0783Ch ;
OUTD              XDATA   07841h ;
PINSD      XDATA   07842h ;
OED         XDATA   07843h ;
OUTE              XDATA   07845h ;
PINSE      XDATA   07846h ;
OEE         XDATA   07847h ;
PORTSETUP       XDATA   07849h ;

IFCONFIG      XDATA   0784Ah ; // Interface Configuration
PORTACF2      XDATA   0784Bh ; // PORTA Alternate Config #2
PORTCCF2      XDATA   0784Ch ; // PORTC Alternate Config #2

DMASRC        XDATA   0784Fh ;
;//DMASRCH      XDATA   0784Fh ; // DMA Source Address (H)
;//DMASRCL      XDATA   07850h ; // DMA Source Address (L)
DMADEST      XDATA   07851h ;
;//DMADESTH      XDATA   07851h ; // DMA Destination Address (H)
;//DMADESTL      XDATA   07852h ; // DMA Destination Address (L)
DMALEN      XDATA   07854h ; // DMA Transfer Length
DMAGO      XDATA   07855h ; // DMA Start and Status

DMABURST        XDATA   07857h ; // DMA Burst Modeh ; for ext DMA transfers

DMAEXTFIFO    XDATA   07858h ;
INT4IVEC        XDATA   0785Dh ; // Interrupt 4 AutoVector
INT4SETUP    XDATA   0785Eh ; // Interrupt 4 Setup

WFDESC  XDATA   07900h ;

I2CMODE    XDATA   07FA7h ;      // STOPIE and 400KHZ bits
IBNIRQ    XDATA   07FB0h ;     // IN-bulk-NAK IRQ
IBNIEN    XDATA   07FB1h ;         // IN-bulk-NAK Int enable

