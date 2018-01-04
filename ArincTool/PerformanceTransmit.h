#pragma once

#include <vector>
#include <deque>

typedef struct _BOARD_PER_CHANNEL
{
	unsigned long WriteCount;
	unsigned long ReadCount;

	unsigned long LastWriteCount;
	unsigned long LastReadCount;
	std::deque<unsigned long> SampleWriteCount;
	std::deque<unsigned long> SampleReadCount;

} BOARD_PER_CHANNEL, *PBOARD_PER_CHANNEL;

typedef struct _BOARD_PER_CONFIG
{
	int BoardCount;
	int Id;
	int BaudRate;
	int Loopback;
	int Parity;
	std::vector<BOARD_PER_CHANNEL> Channel;

	int SamplePeriod;

} BOARD_PER_CONFIG, *PBOARD_PER_CONFIG;

class CPerformanceTransmit : public KPairThread
{
public:
	CPerformanceTransmit();
	~CPerformanceTransmit();

	bool load();
	bool open();
	void reset();
	void close();
	bool start(bool stopRecv);
	bool stop(bool stopRecv);

	void enableSend(BOOL enable = TRUE);
	void enableRecv(BOOL enable = TRUE);
	unsigned long getWriteCount(int channel);
	unsigned long getReadCount(int channel);
	bool isEmptyDeltaWriteCount(int channel);
	bool isEmptyDeltaReadCount(int channel);
	long getDeltaWriteCount(int channel);
	long getDeltaReadCount(int channel);

	BOARD_PER_CONFIG & getBoardConfig() { return m_boardConfig; }

protected:
	inline bool workFirst();
	inline bool workSecond();

	static void WINAPI MmTimerEventCount(PVOID dwUser);

private:
	BOOL m_cancelSend;
	BOOL m_cancelRecv;
	BOARD_PER_CONFIG m_boardConfig;
	KMutex m_mutex;
	MmTimer m_mmTimer;
};
