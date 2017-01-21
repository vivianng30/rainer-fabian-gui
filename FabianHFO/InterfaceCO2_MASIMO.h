#pragma once
#include "InterfaceCO2.h"

class CInterfaceCO2_MASIMO :
	public CInterfaceCO2
{
public:
	CInterfaceCO2_MASIMO(void);
	virtual ~CInterfaceCO2_MASIMO(void);

	int Init(BYTE com);
	int Deinit();
	void StartupSequenz();
	void InitializeSequenz();
	bool PerformMsg();

	void resetStateBytes();
	void setStateBytes(BYTE state);
	BYTE getStateBytes();
	bool isStateOk();

	void setExtendedStateBytes(BYTE state);
	BYTE getExtendedStateBytes();
	bool isExtendedStateOk();
};
