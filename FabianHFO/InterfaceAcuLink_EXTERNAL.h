#pragma once
#include "InterfaceAcuLink.h"
#include "AcuLink_DLL.h"


//#include <fstream>

typedef INT (CALLBACK* LPSETINTDLLFUNC)(INT);
typedef INT (CALLBACK* LPGETINTDLLFUNC)();

typedef INT (CALLBACK* LP_SET_ARRAY_DATA_DLLFUNC)(INT, INT);
typedef INT (CALLBACK* LP_GET_ARRAY_DATA_DLLFUNC)(INT);

typedef INT (CALLBACK* LP_SET_WAVE_RECORD_DLLFUNC)(SHORT,SHORT,SHORT,SHORT,SHORT);

typedef VOID (CALLBACK* LP_SET_BOOL_DLLFUNC)(BOOL);

typedef UINT (CALLBACK* LP_GETACLERROR_DLLFUNC)(VOID);
typedef UINT (CALLBACK* LP_SETACLERROR_DLLFUNC)(VOID);

typedef INT (CALLBACK* LPSETUINTDLLFUNC)(UINT);
typedef UINT (CALLBACK* LPGETUINTDLLFUNC )();

// CInterfaceAcuLink_EXTERNAL

class CInterfaceAcuLink_EXTERNAL : public CInterfaceAcuLink
{

public:
	CInterfaceAcuLink_EXTERNAL();
	virtual ~CInterfaceAcuLink_EXTERNAL();

	bool init();
	bool deinit();
	//HINSTANCE isAcuLink();

	CString getExeVersion();
	CString getDllVersion();

	void setShutdown(UINT uiVal);
	void setProducerStarted(bool bState);

	UINT getAcuLinkError();
	void setGraphData(SHORT iValVolume ,SHORT iValPressure ,SHORT iValFlow, SHORT iValCO2, SHORT iValSPO2);
	int setLanguage(UINT uiLang);
	int setNetPort(UINT uiPort);
	int setLinkType(int iLinkType);
	int sendMeasurementData();
	int setMeasurementData(int idx, int iValue);
	void setParaData(UINT index, int val);
	void setAlarmData(UINT index, int val);

protected:
	static LPSETUINTDLLFUNC pSetShutdownFunc;
	static LPGETUINTDLLFUNC pCheckShutdownFunc;

	static LPSETINTDLLFUNC pSetAcuLinkVersion;
	static LPGETINTDLLFUNC pGetAcuLinkVersion;

	static LP_SET_ARRAY_DATA_DLLFUNC pSetMeasurementData;
	static LP_GET_ARRAY_DATA_DLLFUNC pGetMeasurementData;

	static LP_SET_ARRAY_DATA_DLLFUNC pSetSettingData;
	static LP_GET_ARRAY_DATA_DLLFUNC pGetSettingData;

	static LP_SET_ARRAY_DATA_DLLFUNC pSetAlarmData;
	static LP_GET_ARRAY_DATA_DLLFUNC pGetAlarmData;

	static LP_SET_ARRAY_DATA_DLLFUNC pSayHelloStr;

	static LP_SET_WAVE_RECORD_DLLFUNC pSetWaveRecord;

	static LP_SET_BOOL_DLLFUNC pSetProducerStarted;

	static LP_GETACLERROR_DLLFUNC pGetAclError;
	static LPSETINTDLLFUNC pSetLinkTypeFunc;
	static LPSETUINTDLLFUNC pSetNetPortFunc;
	static LPSETUINTDLLFUNC pSetLanguageFunc;

	static CStringW m_szFileAcuLink;
	CString m_szVersionExe;
	CString m_szVersionDll;
	
};


