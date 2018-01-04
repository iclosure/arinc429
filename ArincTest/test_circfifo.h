
#pragma once

#include <stdio.h>
#include <Windows.h>

typedef struct _CIRCFIFO_PARAM
{
	hr::circfifo<ARINC_WORD_MSG, 20> circfifo;
	KMutex mutex;
} CIRCFIFO_PARAM, *PCIRCFIFO_PARAM;

DWORD WINAPI workRead(LPVOID pUser)
{
	PCIRCFIFO_PARAM circfifoParam = reinterpret_cast<PCIRCFIFO_PARAM>(pUser);

	while (TRUE)
	{
		if (!circfifoParam->circfifo.empty())
		{
			circfifoParam->mutex.lock();
			ARINC_WORD_MSG msg = circfifoParam->circfifo.take();
			circfifoParam->mutex.unlock();
			printf("receive: %d, timetag: %I64d\n", msg.data, msg.timestamp);
		}

		Sleep(600);
	}
}

DWORD WINAPI workWrite(LPVOID pUser)
{
	DWORD index = 0xFF, data = 0;
	PCIRCFIFO_PARAM circfifoParam = reinterpret_cast<PCIRCFIFO_PARAM>(pUser);

	while (TRUE)
	{
		while (index--)
		{
			ARINC_WORD_MSG msg = { 0 };
			msg.data = data++;

			circfifoParam->mutex.lock();
			circfifoParam->circfifo.push_back(msg);
			circfifoParam->mutex.unlock();

			printf("send: %d, timetag: %ld\n", msg.data, msg.timestamp);

			Sleep(300);
		}

		Sleep(500);
	}
}

int test_circfifo()
{
	printf("\n--------  begin to test circfifo --------\n\n");

	HANDLE _hThreadRead;
	HANDLE _hThreadWrite;
	CIRCFIFO_PARAM circfifoParam;

	_hThreadRead = CreateThread(NULL, NULL, workRead, &circfifoParam, 0, 0);
	_hThreadWrite = CreateThread(NULL, NULL, workWrite, &circfifoParam, 0, 0);

	getchar();

	CloseHandle(_hThreadRead);
	CloseHandle(_hThreadWrite);

	getchar();

	return 1;
}