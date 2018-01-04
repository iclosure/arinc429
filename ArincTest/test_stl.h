
#pragma once

#include <stdio.h>
#include <vector>
#include <list>

#ifdef _DEBUG
#pragma comment (lib, "../Debug/arapi")
#else
#pragma comment (lib, "../Release/arapi")
#endif

//#define test_push_back		1
//#define test_pop_back			1
//#define test_front			1
//#define test_erase			1
//#define test_pop_front		1
#define test_clear				1
//#define test_empty			1

//! 
typedef struct _AR_RX_MSG
{
	UCHAR channel;
	DWORD data;
	DWORD timetag;	// us
	 
} AR_RX_MSG;

typedef std::vector<AR_RX_MSG> AR_RX_MSG_VECTOR;
typedef std::list<AR_RX_MSG> AR_RX_MSG_LIST;

int test_stl()
{
	printf("\n--------  begin to test arinc api --------\n\n");

	PreciTimer preciTimer;

	double vstime, vetime;
	double lstime, letime;

	AR_RX_MSG_VECTOR vectTemp;
	vectTemp.clear();

	AR_RX_MSG_LIST listTemp;
	listTemp.clear();

	printf("\n--------  begin to test push_back --------\n\n");

#ifdef test_push_back
	vectTemp.clear();
	listTemp.clear();
	for (int i = 0; i < 1024; i++)
	{
		AR_RX_MSG item;
		item.data = i;
		vstime = preciTimer.valueUS();
		vectTemp.push_back(item);
		vetime = preciTimer.valueUS();

		lstime = preciTimer.valueUS();
		listTemp.push_back(item);
		letime = preciTimer.valueUS();

		printf("index: %d, vector: %f vs list: %f\n", i, vetime - vstime, letime - lstime);
	}
#endif  // test_push_back

	printf("\n--------  begin to test pop_back --------\n\n");

#ifdef test_pop_back
	vectTemp.clear();
	listTemp.clear();
	for (int i = 0; i < 1024; i++)
	{
		AR_RX_MSG item;
		item.data = i;
		vectTemp.push_back(item);
		listTemp.push_back(item);
	}

	for (int i = 0; i < 1024; i++)
	{
		vstime = preciTimer.valueUS();
		vectTemp.pop_back();
		vetime = preciTimer.valueUS();

		lstime = preciTimer.valueUS();
		listTemp.pop_back();
		letime = preciTimer.valueUS();

		printf("index: %d, vector: %f vs list: %f\n", i, vetime - vstime, letime - lstime);
	}
#endif  // test_pop_back

	printf("\n--------  begin to test front --------\n\n");

#ifdef test_front
	vectTemp.clear();
	listTemp.clear();
	for (int i = 0; i < 1024; i++)
	{
		AR_RX_MSG item;
		item.data = i;
		vectTemp.push_back(item);
		listTemp.push_back(item);
	}

	AR_RX_MSG temp;
	for (int i = 0; i < 1024; i++)
	{
		vstime = preciTimer.valueUS();
		temp = vectTemp[0];
		vetime = preciTimer.valueUS();

		lstime = preciTimer.valueUS();
		temp = listTemp.front();
		letime = preciTimer.valueUS();

		printf("index: %d, vector: %f vs list: %f\n", i, vetime - vstime, letime - lstime);
	}
#endif  // test_front

	printf("\n--------  begin to test erase --------\n\n");

#ifdef test_erase
	vectTemp.clear();
	listTemp.clear();
	for (int i = 0; i < 1024; i++)
	{
		AR_RX_MSG item;
		item.data = i;
		vectTemp.push_back(item);
		listTemp.push_back(item);
	}

	for (int i = 0; i < 1024; i++)
	{
		vstime = preciTimer.valueUS();
		vectTemp.erase(vectTemp.begin());
		vetime = preciTimer.valueUS();

		lstime = preciTimer.valueUS();
		listTemp.erase(listTemp.begin());
		letime = preciTimer.valueUS();

		printf("index: %d, vector: %f vs list: %f\n", i, vetime - vstime, letime - lstime);
	}
#endif  // test_erase

	printf("\n--------  begin to test pop_front --------\n\n");

#ifdef test_pop_front
	vectTemp.clear();
	listTemp.clear();
	for (int i = 0; i < 1024; i++)
	{
		AR_RX_MSG item;
		item.data = i;
		vectTemp.push_back(item);
		listTemp.push_back(item);
	}

	for (int i = 0; i < 1024; i++)
	{
		vstime = preciTimer.valueUS();
		vectTemp.erase(vectTemp.begin());
		vetime = preciTimer.valueUS();

		lstime = preciTimer.valueUS();
		listTemp.pop_front();
		letime = preciTimer.valueUS();

		printf("index: %d, vector: %f vs list: %f\n", i, vetime - vstime, letime - lstime);
	}
#endif  // test_pop_front

	printf("\n--------  begin to test clear --------\n\n");

#ifdef test_clear
	vectTemp.clear();
	listTemp.clear();
	for (int i = 0; i < 10240; i++)
	{
		AR_RX_MSG item;
		item.data = i;
		vectTemp.push_back(item);
		listTemp.push_back(item);
	}

	vstime = preciTimer.valueUS();
	vectTemp.clear();
	vetime = preciTimer.valueUS();

	lstime = preciTimer.valueUS();
	listTemp.clear();
	letime = preciTimer.valueUS();

	printf("vector: %f vs list: %f\n", vetime - vstime, letime - lstime);
#endif  // test_clear

	printf("\n--------  begin to test empty --------\n\n");

#ifdef test_empty
	vectTemp.clear();
	listTemp.clear();
	for (int i = 0; i < 1024; i++)
	{
		AR_RX_MSG item;
		item.data = i;
		vectTemp.push_back(item);
		listTemp.push_back(item);
	}

	for (int i = 0; i < 1024; i++)
	{
		vstime = preciTimer.valueUS();
		vectTemp.empty();
		vetime = preciTimer.valueUS();

		lstime = preciTimer.valueUS();
		listTemp.empty();
		letime = preciTimer.valueUS();

		printf("index: %d, vector: %f vs list: %f\n", i, vetime - vstime, letime - lstime);
	}

#endif  // test_empty

	//vectTemp.erase(vectTemp.begin());

	return 0;
}
