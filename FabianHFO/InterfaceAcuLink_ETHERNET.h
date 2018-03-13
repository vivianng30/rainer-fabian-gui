#pragma once
#include "InterfaceAcuLink.h"
#include "AcuLink_DLL.h"


//#include <fstream>


// CInterfaceAcuLink_EXTERNAL

class CInterfaceAcuLink_ETHERNET : public CInterfaceAcuLink
{

public:
	CInterfaceAcuLink_ETHERNET();
	virtual ~CInterfaceAcuLink_ETHERNET();

	bool init();
	bool deinit();
	//HINSTANCE isAcuLink();
	int sendAllMeasurementData();
	void setParaData(UINT index, int val);
	void setAlarmData(UINT index, int val);

protected:
	int setMeasurementData(int idx, int iValue);

	
};


