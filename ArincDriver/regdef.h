
#ifndef _PCI429_USERDEF_H_
#define _PCI429_USERDEF_H_

#include "..\ArincApi\arapi.h"
#include "..\ArincApi\arinc429.h"

#define COM422_PCI_MEMORY_BLOCK_NUM		4

#define COM422_PCI_BAR_REGS				0	// BAR0
#define COM422_PCI_BAR_PORT				1	// BAR1
#define COM422_PCI_BAR_SRAM				2	// BAR2
#define COM422_PCI_BAR_SRAM2			3	// BAR3

///////////////////////////////////////////////////////////////////////

#define ARINC_BUS_REG_OFFSET			0x10000

#define ARINC_BUS_RXREG_ADDR(_context_, _channel_, _regId_) \
	((PULONG)((_context_)->Bar[COM422_PCI_BAR_SRAM].Addr + \
	(ARINC_BUS_REG_OFFSET) + ARINC_RXREG_ADDR((_channel_), (_regId_))))

#define ARINC_BUS_TXREG_ADDR(_context_, _channel_, _regId_) \
	((PULONG)((_context_)->Bar[COM422_PCI_BAR_SRAM].Addr + \
	(ARINC_BUS_REG_OFFSET) + ARINC_TXREG_ADDR((_channel_), (_regId_))))

#define ARINC_BUS_USER_ADDR(_context_, _regId_) \
	((PULONG)((_context_)->Bar[COM422_PCI_BAR_SRAM].Addr + \
	(ARINC_BUS_REG_OFFSET) + ARINC_USER_ADDR((_regId_))))

#define ARINC_BUS_REG_ADDR(_context_, _offset_)  \
	((PULONG)((_context_)->Bar[COM422_PCI_BAR_SRAM].Addr + (_offset_)))

///////////////////////////////////////////////////////////////////////

#endif	// _PCI429_USERDEF_H_
