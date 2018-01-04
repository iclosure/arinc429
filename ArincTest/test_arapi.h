
#pragma once

#include "..\ArincApi\arapi.h"
#include <stdio.h>
#include <Windows.h>

#pragma comment(lib, "arapi.lib")

DWORD WINAPI workSend(LPVOID pUser)
{
	DWORD index = 0x1FF;
	DWORD data = 0;

	while(TRUE)
	{
		while (index--)
		{
			while (ar_putword(0, 0, data) != ARS_NORMAL)
			{
				Sleep(10);
			}

			if ((++data % 0xFF) == 0)
			{
				printf("%4X\n", data);
			}
		}

		Sleep(10);
	}
}

DWORD WINAPI workRecv(LPVOID pUser)
{

	DWORD data = 0, timetag = 0;
	while(TRUE)
	{
		while (TRUE)
		{
			while (ar_getnextt(0, 0, &data, &timetag) != ARS_GOTDATA)
			{
				Sleep(10);
			}

			printf("receive: %4X, timetag: %ld\n", data, timetag);
		}

		Sleep(10);
	}
}

HANDLE _hThreadSend;
HANDLE _hThreadRecv;

int test_arapi()
{
	printf("\n--------  begin to test arinc api --------\n\n");

	short board = 0;
	short channel = 0;

	short status;


	status = ar_loadslv(board, 0, 0, 0);
	status = ar_reset(board);

	status = ar_set_config(board, ARU_INTERNAL_WRAP, AR_ON);
	status = ar_set_config(board, ARU_PARITY, AR_OFF);
	status = ar_set_config(board, ARU_XMIT_RATE, AR_100K);
	status = ar_set_config(board, ARU_RECV_RATE, AR_100K);
	status = ar_go(board);

	ULONG index = 0x2FFF;
	ULONG data;

	ULONG regValTx = ar_num_xchans(board);
	ULONG regValRx = ar_num_rchans(board);

	//! 
	printf("\n--------  test ar_getnext --------\n");

	_hThreadSend = CreateThread(NULL, NULL, workSend, NULL, 0, 0);
	_hThreadRecv = CreateThread(NULL, NULL, workRecv, NULL, 0, 0);

	getchar();

	CloseHandle(_hThreadSend);
	CloseHandle(_hThreadRecv);

	printf("\nMax num: %4X\n", index);

	//! 
	printf("\n--------  test ar_getnextt --------\n");

	index = 0xFFF;
	data = 0;
	while (index--)
	{
		while (ar_putword(board, channel, data) != ARS_NORMAL)
		{

		}

		if ((++data % 0xFF) == 0)
		{
			printf("%4X\n", data);
		}
	}

	index = 0;
	ULONG timetag = 0;
	while (ar_getnextt(board, channel, &data, &timetag) == ARS_GOTDATA)
	{
		printf("data: %4X, timetag: %4ld (* 0.1ms)\n", data, timetag);
		if (data != index++)
		{
			printf("------ ERROR ------\n");
		}
	}

	ar_reset(board);

	printf("\n--------  finish to test arinc api --------\n\n");

	getchar();

	return 0;
}