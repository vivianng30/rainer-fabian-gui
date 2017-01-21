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
	int sendMeasurementData();
	int setMeasurementData(int idx, int iValue);
	void setParaData(UINT index, int val);
	void setAlarmData(UINT index, int val);



	

protected:
	
	
};


