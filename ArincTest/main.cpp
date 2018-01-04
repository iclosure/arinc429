// arapi_test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "test_stl.h"
#include "test_mmtimer.h"
#include "test_arapi.h"
#include "test_circfifo.h"
#include "test_multi_channel.h"

void UserHandleTest()
{  
	unsigned long data;
	if(ar_getnext(0, 0, &data) == ARS_GOTDATA) {
		printf("Thread:  channel%02X--->: %08X\n", 0, data);	
	}
}

int _tmain(int argc, _TCHAR* argv[])
{ 
	//! test multi-channel
	test_multi_channel();
	return 0;

	//! test stl module
	//test_circfifo();

	//! test stl module
	//test_stl();

	//! test mmtimer module
	//test_mmtimer();

	//! test arapi library
	//test_arapi();

	unsigned long reg = 0;
	unsigned long send = 0;
    short status; 
	short board = 0;
	short channel = 0;
	unsigned long data = 0;
	status = ar_loadslv(0, 0, 0, 0);
	status = ar_reset(0);
	status = ar_set_config(0, ARU_INTERNAL_WRAP, AR_ON);
	//status = ar_set_config(0, ARU_TX_CH01_BIT_RATE, AR_50K);
	status = ar_set_config(0, ARU_XMIT_RATE, AR_50K);
	status = ar_set_config(0, ARU_RECV_RATE, AR_50K);
	//status = ar_set_config(0, ARU_TX_CH01_BIT_RATE, AR_50K);
	//status = ar_set_config(0, ARU_RX_CH01_BIT_RATE, AR_12_5K);
	status = ar_go(0);

	ar_set_userhandle(UserHandleTest);

	int count = 0;
	int chans;
	chans = ar_num_rchans(0);
	chans = ar_num_xchans(0);
	for(channel = 0;channel<2;channel++){
		int k = 0x1000;
		while (k--) {

			while (ar_putword(0, channel, send) != ARS_NORMAL) {
			}
			send++;
			if (send % 100 == 0) 
				printf("%08X\n", send);
		}

		
// 		while (ar_getnext(0, channel, &data) == ARS_GOTDATA) {
// 			printf("channel%02X--->:", channel);
// 			printf("%08X\n", data);
// 			if (data != count) {
// 				printf("------ ERROR -------\n");
// 			}
// 			count++;		
// 		}
	}
	printf("%x\n", count);
	printf("-----------------------------\n");
	getchar();
	status = ar_close(0);
	return 0;
}

