
#ifndef ARINC429_REG_H
#define ARINC429_REG_H

/*! -------------------- definitions for arinc429 ------------------------*/

/**/
#define ARINC_BYTE_INDEX_CORE	(0)
#define ARINC_BYTE_INDEX_USER	(2)

/**/
#define ARINC_REG_DATA_ID		(0)
#define ARINC_REG_CTRL_ID		(1)
#define ARINC_REG_STAT_ID		(2)
#define ARINC_REG_LABEL_ID		(3)

/**/
#define ARINC_CH_RX				(0)
#define ARINC_CH_TX				(1)

#define CORE429_CPU_ADDR(_channel_, _txr_, _regId_, _byteId_)  \
	(((_channel_) << 5) + ((_txr_) << 4) + ((_regId_) << 2) + (_byteId_))

#define ARINC_CORE_ADDR(_channel_, _txr_, _regId_)  \
	CORE429_CPU_ADDR((_channel_), (_txr_), (_regId_), (ARINC_BYTE_INDEX_CORE))

#define ARINC_USER_ADDR(_regId_)  \
	((((_regId_) << 2) + 2) + (ARINC_BYTE_INDEX_USER))

#define ARINC_RXREG_ADDR(_channel_, _regId_)  \
	ARINC_CORE_ADDR((_channel_), (ARINC_CH_RX), (_regId_))

#define ARINC_TXREG_ADDR(_channel_, _regId_)  \
	ARINC_CORE_ADDR((_channel_), (ARINC_CH_TX), (_regId_))

/* Rx Control Register */
#define ARINC_RCTRL_BAUD			0	/*Data rate: 0=100kbps;1=12.5 or 50 kbps*/
#define ARINC_RCTRL_ENLABEL			1	/*Label compare: 0=disable;1=enable*/
#define ARINC_RCTRL_EN32ND_PARITY	2	/*0=32nd bit is data;1=32nd bit is parity*/
#define ARINC_RCTRL_PARITY			3	/*Parity:0=odd;1=even*/
#define ARINC_RCTRL_DECODER			4	/**/
#define ARINC_RCTRL_SDI9			5	/**/
#define ARINC_RCTRL_SDI10			6	/**/
#define ARINC_RCTRL_RELOAD_LABELL	7	/**/

#define RCTRL_BAUD_OFFSET			0
#define RCTRL_BAUD_MASK				0x00000001UL
#define RCTRL_ENLABEL_OFFSET		1
#define RCTRL_ENLABEL_MASK			0x00000002UL
#define RCTRL_EN32ND_PARITY_OFFSET	2
#define RCTRL_EN32ND_PARITY_MASK	0x00000004UL
#define RCTRL_PARITY_OFFSET			3
#define RCTRL_PARITY_MASK			0x00000008UL
#define RCTRL_DECODER_OFFSET		4
#define RCTRL_DECODER_MASK			0x00000010UL
#define RCTRL_SDI_OFFSET			5
#define RCTRL_SDI_MASK				0x00000060UL
#define RCTRL_RELOAD_LABELL_OFFSET	7
#define RCTRL_RELOAD_LABELL_MASK	0x00000080UL

/* Rx Status Register */
#define ARINC_RSTAT_FIFO_EMPTY		0	/*0=not empty; 1=empty*/
#define ARINC_RSTAT_FIFO_HALF		1	/*0=less than half full or programmed level;
										  1=half full or programmed level*/
#define ARINC_RSTAT_FIFO_FULL		2	/*0=not full;1=full*/

/* Tx Control Register */
#define ARINC_TCTRL_BAUD			0	/*Data rate: 0=100kbps;1=12.5 or 50 kbps*/
#define ARINC_TCTRL_LOOPBACK		1	/*Label compare: 0=disable;1=enable*/
#define ARINC_TCTRL_EN32ND_PARITY	2	/*0=32nd bit is data;1=32nd bit is parity*/
#define ARINC_TCTRL_PARITY			3	/*Parity:0=odd;1=even*/

#define TCTRL_BAUD_OFFSET			0
#define TCTRL_BAUD_MASK				0x00000001UL
#define TCTRL_LOOPBACKL_OFFSET		1
#define TCTRL_LOOPBACK_MASK			0x00000002UL
#define TCTRL_EN32ND_PARITY_OFFSET	2
#define TCTRL_EN32ND_PARITY_MASK	0x00000004UL
#define TCTRL_PARITY_OFFSET			3
#define TCTRL_PARITY_MASK			0x00000008UL

/* Tx Status Register */
#define ARINC_TSTAT_FIFO_EMPTY		0	/*0=not empty; 1=empty*/
#define ARINC_TSTAT_FIFO_HALF		1	/*0=less than half full or programmed level;
										  1=half full or programmed level*/
#define ARINC_TSTAT_FIFO_FULL		2	/*0=not full;1=full*/

/* -----------------------  user's self-definition register address ------------------- */

#define ARINC_USER_REG_BAUDGEAR		0x0C  /* set baud-rate gear (100/12.5 and 100/50)*/
#define ARINC_USER_REG_INTSTAT		0x10  /* interrupt status */
#define ARINC_USER_REG_RCHNUM		0x14  /* get number of receiving channel */
#define ARINC_USER_REG_TCHNUM		0x18  /* get number of sending channel */
#define ARINC_USER_REG_RESETFIFO	0x1C  /* reset hardware (clear fifo and so on) */
#define ARINC_USER_REG_LABEL0		0x20  /* enable or disable the label 0x00 */
#define ARINC_USER_REG_TIMECNT		0x24  /* get the startup time counter in hardware */

#define ARINC_CODE_RESET_OFF		0	/* reset hardware and stop transmit */
#define ARINC_CODE_RESET_ON			1	/* reset hardware and start transmit */
#define ARINC_CODE_RESET_CLOSE		2	/* close device (used for USB429 Device) */

/* -------------------------------------------------------------------- */

#endif  /* ARINC429_REG_H */
