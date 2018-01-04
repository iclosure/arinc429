#include "stdafx.h"
#include "device.h"

int CDevice::getConfig(int item, unsigned long* value)
{
	if (!this->isOpen())
	{
		return ARS_WINRTFAIL;
	}

	int status;

	if (ARU_RX_CH01_BIT_RATE <= item && item <= ARU_RX_CH16_BIT_RATE)		// 获取接收通道波特率
	{
		status = getBaudRate(item - ARU_RX_CH01_BIT_RATE, ARINC_CH_RX, value);
	}
	else if (ARU_TX_CH01_BIT_RATE <= item && item <= ARU_TX_CH16_BIT_RATE)	// 获取发送通道波特率
	{
		status = getBaudRate(item - ARU_TX_CH01_BIT_RATE, ARINC_CH_TX, value);
	}
	else if (ARU_RX_CH01_PARITY <= item && item <= ARU_RX_CH16_PARITY)		// 获取发送通道校验方式
	{
		status = getParity(item - ARU_RX_CH01_PARITY, ARINC_CH_RX, value);
	}
	else if (ARU_TX_CH01_PARITY <= item && item <= ARU_TX_CH16_PARITY)		// 获取接收通道校验方式
	{
		status = getParity(item - ARU_TX_CH01_PARITY, ARINC_CH_TX, value);
	}
	else if (ARU_CH01_DECODER <= item && item <= ARU_CH16_DECODER)			// 获取接收通道标号过滤开关
	{
		status = getDecoder(item - ARU_CH01_DECODER, value);
	}
	else if (ARU_CH01_SDI <= item && item <= ARU_CH16_SDI)					// 获取接收通道标号过滤
	{
		status = getSDI(item - ARU_CH01_SDI, value);
	}
	else if (ARU_CH01_LOOPBACK <= item && item <= ARU_CH16_LOOPBACK)		// 获取发送通道闭环测试标志
	{
		status = getLookBack(item - ARU_CH01_LOOPBACK, value);
	}
	else if (ARU_INTERNAL_WRAP == item)										// 获取所有发送通道闭环测试标志
	{
		status = getLookBack(-1, value);
	}
	else																	// 选项无效
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

// 获取Arinc429波特率
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

// @brief 获取Arinc429校验方式
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

// @brief 获取Arinc429标号过滤开关
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

// @brief 获取Arinc429标号过滤
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

// @brief 获取Arinc429闭环测试标志
// @param channel 
// @param value 
int CDevice::getLookBack(int channel, unsigned long* value)
{
	if (channel < 0)	// 表示获取所有发送通道的闭环测试标志
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

// 获取位定时配置
int CDevice::getBitsOfRegister(int channel, int trx, ULONG* value, ULONG offset, ULONG mask)
{
	ULONG regAddr = ARINC_CORE_ADDR(channel, trx, ARINC_REG_CTRL_ID);
	ULONG regVal;

	readRegister(regAddr, &regVal);
	
	*value = (regVal & mask) >> offset;

	return ARS_NORMAL;
}
