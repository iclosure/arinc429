#include "stdafx.h"
#include "device.h"

int CDevice::setConfig(int item, int value)
{
	if (!this->isOpen())
	{
		return ARS_WINRTFAIL;
	}

	int status;

	if (ARU_RECV_RATE == item)												// 设置所有接收通道波特率
	{
		status = setBaudRate(-1, ARINC_CH_RX, value);
	}
	else if (ARU_RX_CH01_BIT_RATE <= item && item <= ARU_RX_CH16_BIT_RATE)	// 设置接收通道波特率
	{
		status = setBaudRate(item - ARU_RX_CH01_BIT_RATE, ARINC_CH_RX, value);
	}
	else if (ARU_XMIT_RATE == item)											// 设置所有发送通道波特率
	{
		status = setBaudRate(-1, ARINC_CH_TX, value);
	}
	else if (ARU_TX_CH01_BIT_RATE <= item && item <= ARU_TX_CH16_BIT_RATE)	// 设置发送通道波特率
	{
		status = setBaudRate(item - ARU_TX_CH01_BIT_RATE, ARINC_CH_TX, value);
	}
	else if (ARU_RX_CH01_PARITY <= item && item <= ARU_RX_CH16_PARITY)		// 设置发送通道校验方式
	{
		status = setParity(item - ARU_RX_CH01_PARITY, ARINC_CH_RX, value);
	}
	else if (ARU_TX_CH01_PARITY <= item && item <= ARU_TX_CH16_PARITY)		// 设置接收通道校验方式
	{
		status = setParity(item - ARU_TX_CH01_PARITY, ARINC_CH_TX, value);
	}
	else if (ARU_PARITY == item)											// 设置所有发送、接收通道校验方式
	{
		status = setParity(-1, -1, value);
	}
	else if (ARU_CH01_DECODER <= item && item <= ARU_CH16_DECODER)			// 设置接收通道标号过滤开关
	{
		status = setDecoder(item - ARU_CH01_DECODER, value);
	}
	else if (ARU_CH01_SDI <= item && item <= ARU_CH16_SDI)					// 设置接收通道标号过滤
	{
		status = setSDI(item - ARU_CH01_SDI, value);
	}
	else if (ARU_CH01_LOOPBACK <= item && item <= ARU_CH16_LOOPBACK)		// 设置发送通道闭环测试标志
	{
		status = setLookBack(item - ARU_CH01_LOOPBACK, value);
	}
	else if (ARU_INTERNAL_WRAP == item)										// 设置所有发送通道闭环测试标志
	{
		status = setLookBack(-1, value);
	}
	else																	// 选项无效
	{
		status = ARS_INVHARCMD;
	}

	return status;
}

int CDevice::labelFilter(int channel, int label, int action)
{
	if (!this->isOpen())
	{
		return ARS_WINRTFAIL;
	}

	if (channel < 0 || channel >= m_rxChNum)
	{
		return ARS_INVARG;		// 返回无效通道标志
	}

	if (label >= ARINC_LABEL_NUM && label != ARU_ALL_LABELS)
	{
		return ARS_INVHARVAL;	// 无效的配置值
	}

	return setLabelFilter(channel, label, action);
}

// @brief 设置Arinc429波特率
// @param channel
// @param trx
// @param value
int CDevice::setBaudRate(int channel, int trx, int value)
{
	ULONG regAddr, regVal, chReg, chBitPos;
	ULONG baudBitPos = (ARINC_CH_TX == trx) ? ARINC_TCTRL_BAUD : ARINC_RCTRL_BAUD;

	if (channel < 0)		// 设置所有通道波特率
	{
		int chNum = (ARINC_CH_TX == trx) ? m_txChNum : m_rxChNum;

		for (int i = 0; i < chNum; i++)
		{
			chBitPos = (ARINC_CH_TX == trx) ? i : (i + 16);
			regAddr = ARINC_CORE_ADDR(i, trx, ARINC_REG_CTRL_ID);
			readRegister(regAddr, &regVal);
			readRegister(ARINC_USER_REG_BAUDGEAR, &chReg, ARINC_REG_ADDR_USER);

			switch (value)
			{
			case AR_12_5K:
				CLR_REG_BIT(chReg, chBitPos);
				SET_REG_BIT(regVal, baudBitPos);
				break;
			case AR_50K:
				SET_REG_BIT(chReg, chBitPos);
				SET_REG_BIT(regVal, baudBitPos);
				break;
			case AR_100K:
				SET_REG_BIT(chReg, chBitPos);
				CLR_REG_BIT(regVal, baudBitPos);
				break;
			default: return ARS_INVHARVAL;
			}

			writeRegister(regAddr, regVal);
			writeRegister(ARINC_USER_REG_BAUDGEAR, chReg, ARINC_REG_ADDR_USER);
		}
	}
	else
	{
		chBitPos = (ARINC_CH_TX == trx) ? channel : (channel + 16);
		regAddr = ARINC_CORE_ADDR(channel, trx, ARINC_REG_CTRL_ID);
		readRegister(regAddr, &regVal);
		readRegister(ARINC_USER_REG_BAUDGEAR, &chReg, ARINC_REG_ADDR_USER);

		switch (value)
		{
		case AR_12_5K:
			CLR_REG_BIT(chReg, chBitPos);
			SET_REG_BIT(regVal, baudBitPos);
			break;
		case AR_50K:
			SET_REG_BIT(chReg, chBitPos);
			SET_REG_BIT(regVal, baudBitPos);
			break;
		case AR_100K:
			SET_REG_BIT(chReg, chBitPos);
			CLR_REG_BIT(regVal, baudBitPos);
			break;
		default: return ARS_INVHARVAL;
		}

		writeRegister(regAddr, regVal);
		writeRegister(ARINC_USER_REG_BAUDGEAR, chReg, ARINC_REG_ADDR_USER);
	}

	return ARS_NORMAL;
}

// @brief 设置Arinc429校验方式
// @param channel
// @param trx
// @param value
int CDevice::setParity(int channel, int trx, int value)
{
	ULONG regAddr, regValue;
	ULONG enParityBitPos = (ARINC_CH_TX == trx) ? ARINC_TCTRL_EN32ND_PARITY : ARINC_RCTRL_EN32ND_PARITY;
	ULONG parityBitPos = (ARINC_CH_TX == trx) ? ARINC_TCTRL_PARITY : ARINC_RCTRL_PARITY;

	if (channel < 0)			// 设置所有发送、接收通道校验方式
	{
		struct {
			int ChNum;
			int trx;
		} chns[2] = {
			{ m_rxChNum, ARINC_CH_RX }, { m_txChNum, ARINC_CH_TX }
		};

		for (int i = 0; i < 2; i++)
		{
			for (int j = 0; j < chns[i].ChNum; j++)
			{
				regAddr = ARINC_CORE_ADDR(j, chns[i].trx, ARINC_REG_CTRL_ID);
				readRegister(regAddr, &regValue);

				switch (value)				// 译码
				{
				case AR_OFF:
				//case AR_NONE:
					CLR_REG_BIT(regValue, enParityBitPos);
					break;
				case AR_ON:
					SET_REG_BIT(regValue, enParityBitPos);
					break;
				case AR_ODD:
					SET_REG_BIT(regValue, enParityBitPos);
					CLR_REG_BIT(regValue, parityBitPos);
					break;
				case AR_EVEN:
					SET_REG_BIT(regValue, enParityBitPos);
					SET_REG_BIT(regValue, parityBitPos);
					break;
				default: return ARS_INVHARVAL;	// 无效的值
				}

				writeRegister(regAddr, regValue);
			}
		}
	}
	else
	{
		regAddr = ARINC_CORE_ADDR(channel, trx, ARINC_REG_CTRL_ID);
		readRegister(regAddr, &regValue);
		
		switch (value)					// 译码
		{
		case AR_OFF:
		//case AR_NONE:
			CLR_REG_BIT(regValue, enParityBitPos);
			break;
		case AR_ON:
			SET_REG_BIT(regValue, enParityBitPos);
			break;
		case AR_ODD:
			SET_REG_BIT(regValue, enParityBitPos);
			CLR_REG_BIT(regValue, parityBitPos);
			break;
		case AR_EVEN:
			SET_REG_BIT(regValue, enParityBitPos);
			SET_REG_BIT(regValue, parityBitPos);
			break;
		default: return ARS_INVHARVAL;	// 无效的值
		}

		writeRegister(regAddr, regValue);
	}

	return ARS_NORMAL;
}

// @brief 设置Arinc429标号过滤开关
// @param value 
// @param channel 
int CDevice::setDecoder(int channel, int value)
{
	switch (value)
	{
	case AR_OFF: value = 0; break;
	case AR_ON: value = 1; break;
	default:
		return ARS_INVHARVAL;	// 无效的值
	}

	return setBitsOfRegister(channel, ARINC_CH_RX, value, RCTRL_DECODER_OFFSET, RCTRL_DECODER_MASK);
}

// @brief 设置Arinc429标号过滤
// @param value 
// @param channel 
int CDevice::setSDI(int channel, int value)
{
	return setBitsOfRegister(channel, ARINC_CH_RX, value, RCTRL_SDI_OFFSET, RCTRL_SDI_MASK);
}

// @brief 设置Arinc429闭环测试标志
// @param value 
// @param channel 
int CDevice::setLookBack(int channel, int value)
{
	switch (value)
	{
	case AR_OFF:
	case AR_WRAP_OFF: value = 0; break;
	case AR_ON:
	case AR_WRAP_ON: value = 1; break;
	default:
		return ARS_INVHARVAL;	// 无效的值
	}

	if (channel < 0)	// 表示设置所有发送通道的闭环测试标志
	{
		for (int i = 0; i < m_txChNum; i++)
		{
			setBitsOfRegister(i, ARINC_CH_TX, value, TCTRL_LOOPBACKL_OFFSET, TCTRL_LOOPBACK_MASK);
		}
	}
	else
	{
		setBitsOfRegister(channel, ARINC_CH_TX, value, TCTRL_LOOPBACKL_OFFSET, TCTRL_LOOPBACK_MASK);
	}

	return ARS_NORMAL;
}

// @brief 设置Arinc429...
// @param channel 
// @param label 
// @param action 
int CDevice::setLabelFilter(int channel, int label, int action)
{
	switch (action)
	{
	case ARU_FILTER_OFF: action = FALSE; break;
	case ARU_FILTER_ON: action = TRUE; break;
	default:
		return ARS_INVHARVAL;		// 无效的值
	}

	ULONG labelAddr = ARINC_RXREG_ADDR(channel, ARINC_REG_LABEL_ID);

	if (ARU_ALL_LABELS == label)	// process all labels
	{
		enableLabel0(channel, !action);

		setBitsOfRegister(channel, ARINC_CH_RX, 0, RCTRL_ENLABEL_OFFSET, RCTRL_ENLABEL_MASK);	// disable label compare
		setBitsOfRegister(channel, ARINC_CH_RX, 1, RCTRL_RELOAD_LABELL_OFFSET, RCTRL_RELOAD_LABELL_MASK);	// reset the read and write counter

		for (int i = 0; i < ARINC_LABEL_NUM; i++)
		{
			m_labelTables[channel][i] = action ? 0 : UCHAR(i);
			writeRegister(labelAddr, ULONG(m_labelTables[channel][i]));
		}

		setBitsOfRegister(channel, ARINC_CH_RX, 0, RCTRL_RELOAD_LABELL_OFFSET, RCTRL_RELOAD_LABELL_MASK);	// 
		setBitsOfRegister(channel, ARINC_CH_RX, action, RCTRL_ENLABEL_OFFSET, RCTRL_ENLABEL_MASK);	// if action is FALSE, disable label compare
	}
	else if (label < ARINC_LABEL_NUM)
	{
		if (0 == label)
		{
			enableLabel0(channel, !action);
		}

		setBitsOfRegister(channel, ARINC_CH_RX, 0, RCTRL_ENLABEL_OFFSET, RCTRL_ENLABEL_MASK);	// disable label compare
		setBitsOfRegister(channel, ARINC_CH_RX, 1, RCTRL_RELOAD_LABELL_OFFSET, RCTRL_RELOAD_LABELL_MASK);	// reset the read and write counter

		int enableCount = 0;
		for (int i = 0; i < ARINC_LABEL_NUM; i++)
		{
			m_labelTables[channel][i] = (i == label) ? (action ? 0 : UCHAR(label)) : m_labelTables[channel][i];
			writeRegister(labelAddr, ULONG(m_labelTables[channel][i]));
			enableCount += (i == 0) ? GET_REG_BIT_VAL(m_enableLabel0, channel) : (m_labelTables[channel][i] ? 1 : 0);
		}

		setBitsOfRegister(channel, ARINC_CH_RX, 0, RCTRL_RELOAD_LABELL_OFFSET, RCTRL_RELOAD_LABELL_MASK);	// 
		setBitsOfRegister(channel, ARINC_CH_RX, 1, RCTRL_ENLABEL_OFFSET, RCTRL_ENLABEL_MASK);	// if action is TRUE, disable label compare

		if (enableCount == ARINC_LABEL_NUM)
		{
			setLabelFilter(channel, ARU_ALL_LABELS, ARU_FILTER_OFF);
		}
	}
	else
	{
		return ARS_INVHARVAL;			// invalid input value
	}
	
	return ARS_NORMAL;
}

// 设置控制寄存器的指定位
int CDevice::setBitsOfRegister(int channel, int trx, ULONG value, ULONG offset, ULONG mask)
{
	if (value > (mask >> offset))
	{
		return ARS_INVHARVAL;			// invalid input value
	}

	ULONG regAddr = ARINC_CORE_ADDR(channel, trx, ARINC_REG_CTRL_ID);
	ULONG regVal;

	readRegister(regAddr, &regVal);

	CONFIG_REG_BITS_ULONG(regVal, offset, mask, value);

	writeRegister(regAddr, regVal);

	return ARS_NORMAL;
}
