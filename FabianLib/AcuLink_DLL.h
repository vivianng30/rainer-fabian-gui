// AcuLink_DLL.h : Hauptheaderdatei für die AcuLink_DLL-DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "'stdafx.h' vor dieser Datei für PCH einschließen"
#endif

#include "resource.h"
#include "acuLink.h"

#ifndef EXPORT 
#ifdef __cplusplus 
#define EXPORT extern "C" __declspec (dllexport) 
#endif 
#endif 

//// Error Codes (ACL = AcuLink)
//#define ACL_ERR_NONE							(UINT)0x00000000
//#define ACL_ERR_LICENCE_WRONG					(UINT)0x00000001
//#define ACL_ERR_SPECTABLE_NOT_FOUND				(UINT)0x00000002
//#define ACL_ERR_CANNOT_OPEN_PORT				(UINT)0x00000004
//#define ACL_ERR_CANNOT_START_WATCHER			(UINT)0x00000008
//#define ACL_ERR_WAVE_WRITE_NOT_STARTED			(UINT)0x00000010
//#define ACL_ERR_WAVE_BUFFER_UNDERRUN			(UINT)0x00000020
//#define ACL_ERR_SPECTABLE_SETUP_FILE_NOT_FOUND	(UINT)0x00000040
//#define ACL_ERR_INIFILE_NOT_FOUND				(UINT)0x00000080
//#define ACU_LINK_ERROR_BUFFER_OVERFLOW			(UINT)0x00000100
//
//// LinkTypes
//#define ACL_LT_NONE				0
//#define ACL_LT_SERIAL_ASCII		1
//#define ACL_LT_SERIAL_IVOI		2
//#define ACL_LT_ETHERNET			3
//
//// ab Version 2.2
//#define ACL_LT_SERIAL_WAVE		5
//#define ACL_LT_ETHERNET_WAVE	6
//
//// Default-Port auf dem im Ethernet gehört wird
//#define DEFAULT_PORT	5150
//
//
//#define MAX_WAVES		4
//#define WAVE_VOL		0
//#define WAVE_PRESS		1
//#define WAVE_FLOW		2
//#define WAVE_CO2		3


#define MAX_WAVES		4
#define MAX_SAMPLES		4

#define MAX_WAVE_RECORDS	1000

typedef struct T_DLL_WAVE_RECORD
{
	short	SampleWaves[MAX_WAVES];
} DllWaveRecord;

// CAcuLink_DLLApp
// Siehe AcuLink_DLL.cpp für die Implementierung dieser Klasse
//

class CAcuLink_DLLApp : public CWinApp
{
public:

	CAcuLink_DLLApp(); // Konstruktor

	int IncWriteIdx();
	int IncReadIdx();
	int FindStartLogCounter(int iInitCounter, int iMaxCounter);

	static EXPORT int SetAcuLinkVersion (int iValue);
	static EXPORT int GetAcuLinkVersion ();
	static EXPORT int SetMeasurementData (int idx, int wValue);
	static EXPORT int GetMeasurementData (int idx);
	static EXPORT int SetSettingData (int idx, int wValue);
	static EXPORT int GetSettingData (int idx);
	static EXPORT int SetAlarmData (int idx, int wValue);
	static EXPORT int GetAlarmData (int idx);
	static EXPORT int GetAcuLinkLastError(void);

	static EXPORT CString SayHelloStr (CString strName);
	
	static EXPORT int  SetWaveRecord(short shVol, short shPress, 
											short shFlow, short shCO2);

	static EXPORT int GetWaveRecord(DllWaveRecord p[MAX_SAMPLES]);
	static EXPORT int GetSingleWaveRecord(DllWaveRecord *p);

	static EXPORT int  SetupProducer(WORD wMaxWaveRecords, WORD wReadStartThreshold);
	static EXPORT void SetProducerStarted(bool bFlag);
	static EXPORT bool GetProducerStarted(void);
	static EXPORT void SetWriteGo(bool bFlag);
	static EXPORT bool GetWriteGo(void);
	static EXPORT int  SetupConsumer(void);
	static EXPORT void SetConsumerStarted(bool bFlag);
	static EXPORT bool GetConsumerStarted(void);
	static EXPORT void SetReadGo(bool bFlag);
	static EXPORT bool GetReadGo(void);

	static EXPORT int SetLinkType(int iLinkType);
	static EXPORT int GetLinkType();

	static EXPORT int SetAclError(UINT uiErrorCode);
	static EXPORT UINT GetAclError();

	static EXPORT int SetNetPort(UINT uiPort);
	static EXPORT UINT GetNetPort();

	static EXPORT int SetLanguage(UINT uiPort);
	static EXPORT UINT getLANGUAGE();

	static EXPORT int LogMsg(char *s);

	static EXPORT int SetShutdown(UINT uiVal);
	static EXPORT UINT CheckShutdown();

private:
	int fiAcuLinkError;
	
	//static int LogMsg(char *s);

// Überschreibungen
public:

	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

// ****************************************************************************

class CCounter
{
public:
	/*
	int GetCounter();
	void SetCounter(int iValue);
	*/

	// Konstruktor
	explicit CCounter(int _iCounter);
private:
	int fiCounter;
};
