#include "stdafx.h"
#include "device.h"

int CDevice::getConfig(int item, unsigned long* value)
{
	if (!this->isOpen())
	{
		return ARS_WINRTFAIL;
	}

	int status;

	if (ARU_RX_CH01_BIT_RATE <= item && item <= ARU_RX_CH16_BIT_RATE)		// ��ȡ����ͨ��������
	{
		status = getBaudRate(item - ARU_RX_CH01_BIT_RATE, ARINC_CH_RX, value);
	}
	else if (ARU_TX_CH01_BIT_RATE <= item && item <= ARU_TX_CH16_BIT_RATE)	// ��ȡ����ͨ��������
	{
		status = getBaudRate(item - ARU_TX_CH01_BIT_RATE, ARINC_CH_TX, value);
	}
	else if (ARU_RX_CH01_PARITY <= item && item <= ARU_RX_CH16_PARITY)		// ��ȡ����ͨ��У�鷽ʽ
	{
		status = getParity(item - ARU_RX_CH01_PARITY, ARINC_CH_RX, value);
	}
	else if (ARU_TX_CH01_PARITY <= item && item <= ARU_TX_CH16_PARITY)		// ��ȡ����ͨ��У�鷽ʽ
	{
		status = getParity(item - ARU_TX_CH01_PARITY, ARINC_CH_TX, value);
	}
	else if (ARU_CH01_DECODER <= item && item <= ARU_CH16_DECODER)			// ��ȡ����ͨ����Ź��˿���
	{
		status = getDecoder(item - ARU_CH01_DECODER, value);
	}
	else if (ARU_CH01_SDI <= item && item <= ARU_CH16_SDI)					// ��ȡ����ͨ����Ź���
	{
		status = getSDI(item - ARU_CH01_SDI, value);
	}
	else if (ARU_CH01_LOOPBACK <= item && item <= ARU_CH16_LOOPBACK)		// ��ȡ����ͨ���ջ����Ա�־
	{
		status = getLookBack(item - ARU_CH01_LOOPBACK, value);
	}
	else if (ARU_INTERNAL_WRAP == item)										// ��ȡ���з���ͨ���ջ����Ա�־
	{
		status = getLookBack(-1, value);
	}
	else																	// ѡ����Ч
	{
		status = ARS_INVHARCMD;
	}

	return status;
}

int CDevice::numRChans()
{
	if (!this->isOpen())
	{
		return ARS_BRDNOTLOAD;
	}

	ULONG chNum = 0;

	readRegister(ARINC_USER_REG_RCHNUM, &chNum, ARINC_REG_ADDR_USER);

	return (int)chNum;
}

int CDevice::numTChans()
{
	if (!this->isOpen())
	{
		return ARS_BRDNOTLOAD;
	}

	ULONG chNum = 0;

	readRegister(ARINC_USER_REG_TCHNUM, &chNum, ARINC_REG_ADDR_USER);

	return (int)chNum;
}

int CDevice::getLatest(int channel, int label, void *data, char *seq_num)
{
	if (!this->isOpen())
	{
		return ARS_WINRTFAIL;
	}

	return ARS_WINRTFAIL;
}

// ��ȡArinc429������
int CDevice::getBaudRate(int channel, int trx, unsigned long* value)
{
	ULONG regAddr = ARINC_CORE_ADDR(channel, trx, ARINC_REG_CTRL_ID);
	ULONG regVal, chReg;

	readRegister(regAddr, &regVal);
	readRegister(ARINC_USER_REG_BAUDGEAR, &chReg, ARINC_REG_ADDR_USER);

	int _channel = (ARINC_CH_TX == trx) ? channel : (channel + 16);
	int baudBitPos = (ARINC_CH_TX == trx) ? ARINC_TCTRL_BAUD : ARINC_RCTRL_BAUD;

	switch ((GET_REG_BIT(regVal, baudBitPos) << 1) | (GET_REG_BIT_VAL(chReg, _channel)))
	{
	case 0x00:
	case 0x01: *value = AR_100K; break;
	case 0x02: *value = AR_12_5K; break;
	case 0x03: *value = AR_50K; break;
	}

	return ARS_NORMAL;
}

// @brief ��ȡArinc429У�鷽ʽ
// @param channel
// @param trx
// @param value
int CDevice::getParity(int channel, int trx, unsigned long* value)
{
	ULONG regAddr = ARINC_CORE_ADDR(channel, trx, ARINC_REG_CTRL_ID);
	ULONG regVal;

	readRegister(regAddr, &regVal);

	ULONG enParityBitPos = (ARINC_CH_TX == trx) ? ARINC_TCTRL_EN32ND_PARITY : ARINC_RCTRL_EN32ND_PARITY;
	ULONG parityBitPos = (ARINC_CH_TX == trx) ? ARINC_TCTRL_PARITY : ARINC_RCTRL_PARITY;

	if (GET_REG_BIT_VAL(regVal, enParityBitPos))
	{
		*value = GET_REG_BIT_VAL(regVal, parityBitPos) ? AR_EVEN : AR_ODD;
	}
	else
	{
		*value = AR_OFF;
	}

	return ARS_NORMAL;
}

// @brief ��ȡArinc429��Ź��˿���
// @param channel 
// @param value 
int CDevice::getDecoder(int channel, unsigned long* value)
{
	ULONG regAddr = ARINC_RXREG_ADDR(channel, ARINC_REG_CTRL_ID);
	ULONG regVal;

	readRegister(regAddr, &regVal);

	*value = GET_REG_BIT_VAL(regVal, ARINC_RCTRL_DECODER);

	switch (*value)
	{
	case 0: *value = AR_OFF; break;
	case 1: *value = AR_ON; break;
	}

	return ARS_NORMAL;
}

// @brief ��ȡArinc429��Ź���
// @param channel 
// @param value 
int CDevice::getSDI(int channel, unsigned long* value)
{
	ULONG regAddr = ARINC_RXREG_ADDR(channel, ARINC_REG_CTRL_ID);
	ULONG regVal;

	readRegister(regAddr, &regVal);

	*value = GET_REG_BITS_VAL(regVal, RCTRL_SDI_OFFSET, RCTRL_SDI_MASK);

	return ARS_NORMAL;
}

// @brief ��ȡArinc429�ջ����Ա�־
// @param channel 
// @param value 
int CDevice::getLookBack(int channel, unsigned long* value)
{
	if (channel < 0)	// ��ʾ��ȡ���з���ͨ���ıջ����Ա�־
	{
		channel = 0;
	}

	ULONG regAddr = ARINC_TXREG_ADDR(channel, ARINC_REG_CTRL_ID);
	ULONG regVal;

	readRegister(regAddr, &regVal);

	*value = GET_REG_BIT_VAL(regVal, ARINC_TCTRL_LOOPBACK);
	
	switch (*value)
	{
	case 0: *value = AR_WRAP_OFF; break;
	case 1: *value = AR_WRAP_ON; break;
	}

	return ARS_NORMAL;
}

// ��ȡλ��ʱ����
int CDevice::getBitsOfRegister(int channel, int trx, ULONG* value, ULONG offset, ULONG mask)
{
	ULONG regAddr = ARINC_CORE_ADDR(channel, trx, ARINC_REG_CTRL_ID);
	ULONG regVal;

	readRegister(regAddr, &regVal);
	
	*value = (regVal & mask) >> offset;

	return ARS_NORMAL;
}
