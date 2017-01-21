#pragma once

#include "afxwin.h"
#include "FabianHFO.h"
#include "TlsComPort.h"
#include "DIO.h"
#include "dio_sdk.h"
#include "fs_driverinfo.h"

extern CEvent g_eventGraphData;
//extern CEvent g_eventSPIData;
extern CEvent g_eventNewMonitorData;
extern CEvent g_eventNewAlarmData;
extern CEvent g_eventNewFlowsensorData;
extern CEvent g_eventExspiration;


class CMVModel;

class CInterface
{
public:
	

	
public:
	CInterface(void);
	virtual ~CInterface(void);

	bool isSafeTickCountDelayExpired(DWORD oldTickCount, UINT delay=0);//used for (dwLastTickCount+DELAY<getTickCount64())

	virtual CMVModel *getModel();

private:

	CMVModel *m_pModel;


};
