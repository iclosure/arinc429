
#include "stdafx.h"
#include "device_stack.h"

//#pragma data_seg (".shared_wtapi")
CDeviceStack* _g_deviceStack = NULL;
//#pragma data_seg ()

//#pragma comment(linker, "/SECTION:.shared_wtapi,RWS")

#define IF_BOARD_IDOVR_RETURN_INVBOARD(_board_)  \
	if (_g_deviceStack->deviceCount() == 0 || \
		_board_ < 0 || \
		_board_ >= _g_deviceStack->deviceCount()) \
	{ \
		return ARS_INVBOARD; \
	}

DWORD __DeviceInitialize()
{
	if (NULL == _g_deviceStack)
	{
		_g_deviceStack = new CDeviceStack();
	}

	return TRUE;
}

DWORD __DeviceCleanup()
{
	if (_g_deviceStack != NULL)
	{
		delete _g_deviceStack;
		_g_deviceStack = NULL;
	}

	return TRUE;
}

/* ---------------------------------------------------------
	                         通用函数
   --------------------------------------------------------- */

int __crd_api SetUserHandle(UserHandle pUser)
{
	for (int i = 0; i < _g_deviceStack->deviceCount(); i++)
	{
		(*_g_deviceStack)[i]->setUserHandler(pUser);
	}

	return (_g_deviceStack->deviceCount() > 0) ? ARS_NORMAL : ARS_INVBOARD;
}

short __crd_api ar_loadslv(short board, unsigned long base_reg, int base_port, unsigned short ram_size)
{
	IF_BOARD_IDOVR_RETURN_INVBOARD(board);
	
	return (*_g_deviceStack)[board]->loadslv(base_reg, base_port, ram_size);
}

short __crd_api ar_go(short board)
{
	IF_BOARD_IDOVR_RETURN_INVBOARD(board);

	return (*_g_deviceStack)[board]->go();
}

short __crd_api ar_close(short board)
{
	IF_BOARD_IDOVR_RETURN_INVBOARD(board);

	return (*_g_deviceStack)[board]->close();
}

short __crd_api ar_reset(short board)
{
	IF_BOARD_IDOVR_RETURN_INVBOARD(board);

	return (*_g_deviceStack)[board]->reset();
}

/* ---------------------------------------------------------
	                    ARINC429 函数
   --------------------------------------------------------- */

short __crd_api ar_putword(short board, short channel, unsigned long data)
{
	return (*_g_deviceStack)[board]->putWord(channel, data);
}

short __crd_api ar_getnext(short board, short channel, unsigned long* data)
{
	return (*_g_deviceStack)[board]->getNext(channel, data);
}

short __crd_api ar_getnextt(short board, short channel, unsigned long *data, void *timetag)
{
	return (*_g_deviceStack)[board]->getNextt(channel, data, timetag);
}

short __crd_api ar_set_config(short board, short item, unsigned long value)
{
	IF_BOARD_IDOVR_RETURN_INVBOARD(board);

	return (*_g_deviceStack)[board]->setConfig(item, value);
}

short __crd_api ar_get_config(short board, short item, unsigned long *value)
{
	IF_BOARD_IDOVR_RETURN_INVBOARD(board);

	return (*_g_deviceStack)[board]->getConfig(item, value);
}

short __crd_api ar_label_filter(short board, short channel, unsigned short label, short action)
{
	IF_BOARD_IDOVR_RETURN_INVBOARD(board);

	return (*_g_deviceStack)[board]->labelFilter(channel, label, action);
}

void __crd_api ar_get_latest(short board, short channel, unsigned short label, void *data, char *seq_num)
{
	if (_g_deviceStack->deviceCount() == 0 ||
		board < 0 ||
		board >= _g_deviceStack->deviceCount())
	{
		return;
	}

	(*_g_deviceStack)[board]->getLatest(channel, label, data, seq_num);
}

short __crd_api ar_num_rchans(short board)
{
	IF_BOARD_IDOVR_RETURN_INVBOARD(board);

	return (*_g_deviceStack)[board]->numRChans();
}

short __crd_api ar_num_xchans(short board)
{
	IF_BOARD_IDOVR_RETURN_INVBOARD(board);
	
	return (*_g_deviceStack)[board]->numTChans();
}
