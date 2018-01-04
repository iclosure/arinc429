
#include "stdafx.h"
#include "device_stack.h"

//#pragma data_seg (".shared_arapi")
CDeviceStack* _g_deviceStack = NULL;
//#pragma data_seg ()

//#pragma comment(linker, "/SECTION:.shared_arapi,RWS")

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

int __crd_api ar_board_num()
{
	return _g_deviceStack->deviceCount();
}

int  __crd_api ar_board_type(int board)
{
	IF_BOARD_IDOVR_RETURN_INVBOARD(board);

	return (*_g_deviceStack)[board]->boardType();
}

/* ---------------------------------------------------------
	                         通用函数
   --------------------------------------------------------- */

int __crd_api ar_set_userhandle(UserHandle pUser)
{
	for (int i = 0; i < _g_deviceStack->deviceCount(); i++)
	{
		(*_g_deviceStack)[i]->setUserHandler(pUser);
	}

	return (_g_deviceStack->deviceCount() > 0) ? ARS_NORMAL : ARS_INVBOARD;
}

int __crd_api ar_loadslv(int board, int base_reg, int base_port, int ram_size)
{
	IF_BOARD_IDOVR_RETURN_INVBOARD(board);
	
	return (*_g_deviceStack)[board]->loadslv(base_reg, base_port, ram_size);
}

int __crd_api ar_go(int board)
{
	IF_BOARD_IDOVR_RETURN_INVBOARD(board);

	return (*_g_deviceStack)[board]->go();
}

int __crd_api ar_close(int board)
{
	IF_BOARD_IDOVR_RETURN_INVBOARD(board);

	return (*_g_deviceStack)[board]->close();
}

int __crd_api ar_reset(int board)
{
	IF_BOARD_IDOVR_RETURN_INVBOARD(board);

	return (*_g_deviceStack)[board]->reset();
}

/* ---------------------------------------------------------
	                    ARINC429 函数
   --------------------------------------------------------- */

int __crd_api ar_putword(int board, int channel, unsigned long data)
{
	return (*_g_deviceStack)[board]->putWord(channel, data);
}

int __crd_api ar_getnext(int board, int channel, unsigned long* data)
{
	return (*_g_deviceStack)[board]->getNext(channel, data);
}

int __crd_api ar_getnextt(int board, int channel, unsigned long *data, void *timetag)
{
	return (*_g_deviceStack)[board]->getNextt(channel, data, timetag);
}

int __crd_api ar_set_config(int board, int item, int value)
{
	IF_BOARD_IDOVR_RETURN_INVBOARD(board);

	return (*_g_deviceStack)[board]->setConfig(item, value);
}

int __crd_api ar_get_config(int board, int item, int* value)
{
	IF_BOARD_IDOVR_RETURN_INVBOARD(board);

	return (*_g_deviceStack)[board]->getConfig(item, value);
}

int __crd_api ar_label_filter(int board, int channel, int label, int action)
{
	IF_BOARD_IDOVR_RETURN_INVBOARD(board);

	return (*_g_deviceStack)[board]->labelFilter(channel, label, action);
}

void __crd_api ar_get_latest(int board, int channel, int label, void *data, int *seq_num)
{
	if (_g_deviceStack->deviceCount() == 0 ||
		board < 0 ||
		board >= _g_deviceStack->deviceCount())
	{
		return;
	}

	(*_g_deviceStack)[board]->getLatest(channel, label, data, seq_num);
}

int __crd_api ar_num_rchans(int board)
{
	IF_BOARD_IDOVR_RETURN_INVBOARD(board);

	return (*_g_deviceStack)[board]->numRChans();
}

int __crd_api ar_num_xchans(int board)
{
	IF_BOARD_IDOVR_RETURN_INVBOARD(board);
	
	return (*_g_deviceStack)[board]->numTChans();
}

int __crd_api ar_putwords(int board, int channel, unsigned long* buffer, int size, int* length)
{
	return (*_g_deviceStack)[board]->putWords(channel, buffer, size, length);
}

int __crd_api ar_getwords(int board, int channel, unsigned long* buffer, int size, int* length)
{
	return (*_g_deviceStack)[board]->getWords(channel, buffer, size, length);
}

int __crd_api ar_getwordst(int board, int channel, ARINC_WORD_MSG* buffer, int size, int* length)
{
	return (*_g_deviceStack)[board]->getWordst(channel, buffer, size, length);
}
