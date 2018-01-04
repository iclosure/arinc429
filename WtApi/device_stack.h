#pragma once

#include "device_pci.h"
#include "device_usb.h"
#include <vector>
#include <iostream>

class CDeviceStack
{
public:
	CDeviceStack()
	{
		int count;

		count = CPCIDevice(-1).deviceCount();
		for (int i = 0; i < count; i++)
		{
			m_deviceStack.push_back(new CPCIDevice(i));
		}
		
		count = CUSBDevice(-1).deviceCount();
		for (int i = 0; i < count; i++)
		{
			m_deviceStack.push_back(new CUSBDevice(i));
		}
	}

	~CDeviceStack()
	{
		for (size_t i = 0; i < m_deviceStack.size(); i++)
		{
			delete m_deviceStack[i];
		}

		m_deviceStack.clear();
	}

	int deviceCount() const { return m_deviceStack.size(); }
	CDevice* operator[](int index) const { return m_deviceStack[index]; }

private:
	std::vector<CDevice*>	m_deviceStack;
};
