#pragma once
#include "interface.h"
#include "globDefs.h"
//#include "AcuLink_DLL.h"
#include "acuLink.h"


class CInterfaceAcuLink : public CInterface
{


public:
	static CInterfaceAcuLink* getInstance(BYTE iLinkType);
	static void destroyInstance();

	virtual bool init()=0;
	virtual bool deinit()=0;

	//virtual HINSTANCE isAcuLink()=0;
	virtual int sendMeasurementData()=0;
	virtual int setMeasurementData(int idx, int iValue)=0;
	virtual void setParaData(UINT index, int val)=0;
	virtual void setAlarmData(UINT index, int val)=0;

	virtual void setGraphData(SHORT iValVolume ,SHORT iValPressure ,SHORT iValFlow, SHORT iValCO2, SHORT iValSPO2);
	virtual int setLanguage(UINT uiLang);
	virtual int setNetPort(UINT uiPort);
	
	//virtual int setAcuLinkError(UINT uiErrorCode);
	virtual UINT getAcuLinkError();
	virtual int setLinkType(int iLinkType);


	//EXTERNAL only
	virtual void setShutdown(UINT uiVal){};
	virtual void setProducerStarted(bool bState){};
	virtual CString getExeVersion(){return _T("");};
	virtual CString getDllVersion(){return _T("");};

	
	void clearAlarmArray();


protected:
	CInterfaceAcuLink(void);
	virtual ~CInterfaceAcuLink(void);

	void fillMeasurementData();

protected:

	
	UINT m_iVersion;
	static int		fiLinkType;

	//singleton
	static CInterfaceAcuLink* theAcuLinkInterface;

	static HINSTANCE _hAcuLink;
	bool m_bShowAcuLinkError;

	


};
