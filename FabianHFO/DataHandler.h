//=============================================================================
/** 
* \file DataHandler.h
* \class CDataHandler
* \brief Data-Controller of Model-View-Controller software architecture.
* \author Rainer Kuehner
* \date 2013-01-21 Creation of file
*
* The data controller handles all internal data and send commands to its associated 
* views to change the view's presentation of the model. These data are loaded and
* saved through class configuration.h into the FRAMs of the mainboard. The class
* provides the measured data from the Monitor PIC to other tasks through the
* Publisher-Subscriber model depending on the current ventilation mode.
* All val-ues are checked against ranges defined in the global files config.h and globDefs.h.
*
*       Copyright (c) 2013 Ing.Buero Rainer Kuehner
*                      Illerkirchberg
*
**/
//=============================================================================
#pragma once

#include "globDefs.h"
#include "ParaDataPRESET.h"
#include "ParaData.h"
#include "CircularBuffer.h"
#include "Trend.h"


class CMVModel;

class CDataHandler
{
public:

	static CDataHandler* getInstance();	
	static void destroyInstance();		

	void setExit();

	void init();	
	void start();	

	void checkVentRangeSettings();//EFLOW_CHECK
	void initTrend();

	WORD setBitOfWord(WORD x, unsigned int n);	
	BYTE setBitOfByte(BYTE x, unsigned int n);

	WORD getHWconfig();

	bool checkLicensing();
	void WriteLicenseFile();

	void setDemoLicense(eModule module);
	void delDemoLicense(eModule module, bool bReset);
	void endableDemoLicense(eModule module);
	void checkDemoLicense(eModule module);
	COleDateTime GetdemoTimestamp(eModule module);

	void checkDemoLicense_HFO();
	void checkDemoLicense_NMODE();
	void checkDemoLicense_VGUARANTY();
	void checkDemoLicense_VLIMIT();
	void checkDemoLicense_LUNGREC();
	void checkDemoLicense_TREND();
	void checkDemoLicense_THERAPY();
	void checkDemoLicense_PRICO();
	void checkDemoLicense_FOT();
	void checkDemoLicense_NIVTRIGGER();
	
	CString getFormattedEncryptKey(CStringA szEncryptedKey);

	CStringA encryptKey(eModule module);
	CStringA encryptDEMOKey(eModule module);
	
	void char2Hex(unsigned char ch, char* szHex);
	void hex2Char(char const* szHex, unsigned char& rch);
	void charStr2HexStr(unsigned char const* pucCharStr, char* pszHexStr, int iSize);
	void hexStr2CharStr(char const* pszHexStr, unsigned char* pucCharStr, int iSize);

	BYTE getPRICO_SPO2lowRange();
	void setPRICO_SPO2lowRange(BYTE SPO2low);
	BYTE getPRICO_SPO2highRange();
	void setPRICO_SPO2highRange(BYTE SPO2high);

	BYTE getPRICO_FIO2lowRange();
	void setPRICO_FIO2lowRange(BYTE FIO2low);
	BYTE getPRICO_FIO2highRange();
	void setPRICO_FIO2highRange(BYTE FIO2high);

	bool getPRICOState();
	void setPRICOon();
	void setPRICOoff();

	bool isHFOLicenseAvailable();
	void enableHFOLicense();
	void disableHFOLicense();
	bool isHFODemoLicAvailable();

	bool isNMODELicenseAvailable();
	void enableNMODELicense();
	void disableNMODELicense();
	bool isNMODEDemoLicAvailable();

	bool isVGUARANTLicenseAvailable();
	void enableVGUARANTLicense();
	void disableVGUARANTLicense();
	bool isVGUARANTDemoLicAvailable();

	bool isVLIMITLicenseAvailable();
	void enableVLIMITLicense();
	void disableVLIMITLicense();
	bool isVLIMITDemoLicAvailable();

	bool isLUNGRECLicenseAvailable();
	void enableLUNGRECLicense();
	void disableLUNGRECLicense();
	bool isLUNGRECDemoLicAvailable();

	bool isTRENDLicenseAvailable();
	void enableTRENDLicense();
	void disableTRENDLicense();
	bool isTRENDDemoLicAvailable();

	bool isTHERAPYLicenseAvailable();
	void enableTHERAPYLicense();
	void disableTHERAPYLicense();
	bool isTHERAPYDemoLicAvailable();

	bool isPRICOLicenseAvailable();
	void enablePRICOLicense();
	void disablePRICOLicense();
	bool isPRICODemoLicAvailable();

	bool isFOTLicenseAvailable();
	void enableFOTLicense();
	void disableFOTLicense();
	bool isFOTDemoLicAvailable();

	bool isNIVTRIGGERAvailable();
	bool isNIVTRIGGERLicenseAvailable();
	void enableNIVTRIGGERLicense();
	void disableNIVTRIGGERLicense();
	bool isNIVTRIGGERDemoLicAvailable();

	BOOL showFlowAC();

	void loadConfig();
	void loadIniConfiguration();
	void loadDiagrammColor();
	void loadNumerics();
	
	void setNumericINIstate(NUMERICINI* pbufNumeric,BYTE iBlock,CStringW szNum);
	void readNumericINIvalues(NUMERICINI* pbufNumeric,BYTE iBlock,BYTE num,CStringW szNum);
	eNumericType getNumericType(CStringW szVal);
	eNumericSize getNumericSize(CStringW szSize);

	CParaDataPRESET* PRESET();
	CParaData* PARADATA();
	
	void setMainboardData();
	void getMainboardData(MAINBOARD_DATA* pDataMainboard);

	void setPSVapnoe(bool bState);
	bool isPSVapnoe();

	void SetNurscallAlarm(bool state);
	bool isNurscallAlarm();

	void writeSPO2Buffer(const PBUFSPO2& pBufData);
	void writeSPO2waveData(SHORT iVal);
	SHORT readSPO2waveData();
	void writeSpO2SIQ(SHORT iSIQvalue);
	SHORT readSpO2SIQ();

	void writeCO2Buffer(const SHORT& pBufData);
	
	void writeDataBuffer(const PBUFSPI& pBufData);
	void writeCopyDataBuffer(const PBUFSPI& pBufData);
	void updateCopyDataBuffer();

	void setO2FlushDiff(BYTE iVal);
	void setO2Diff(BYTE iFlushVal);
	BYTE getO2Diff();

	void setPmeanRecDifference(int iVal);
	int getPmeanDifference();

	void setPEEP_PpsvDifference(int iDiff);
	int getPEEP_PpsvDifference();;

	void checkVGdependency();

	void setMessureDataAVGasTrendData();
	void setMessureDataAVG(BYTE type, INT val);
	INT getMessureDataAVG(BYTE type);
	void setMessureDataBTB(BYTE type, INT val);
	INT getMessureDataBTB(BYTE type);

	void checkLimits();

	SHORT getMessureDataPRESSURE();
	void setMessureDataPRESSURE(SHORT value);

	SHORT getMessureDataFLOW();				//Achtung, tatsächlicher Meß-Wert ist um Faktor 2 größer
	void setMessureDataFLOW(SHORT value);	//Achtung, tatsächlicher Meß-Wert ist um Faktor 2 größer
	SHORT getMessureDataVOLUME();

	void setMessureDataVOLUME(SHORT value);
	SHORT getAVGMessureDataPINSP();
	SHORT getBTBMessureDataPINSP();
	SHORT getAVGMessureDataPmitt();
	SHORT getBTBMessureDataPmitt();
	SHORT getAVGMessureDataPEEP();
	SHORT getBTBMessureDataPEEP();
	WORD getAVGMessureDataCompliance();
	WORD getBTBMessureDataCompliance();
	WORD getAVGMessureDataC20C();
	WORD getBTBMessureDataC20C();
	WORD getAVGMessureDataResistance();
	WORD getBTBMessureDataResistance();
	WORD getAVGMessureDataMV();
	WORD getBTBMessureDataMV();
	BYTE getAVGMessureDataPercent();
	BYTE getBTBMessureDataPercent();
	WORD getMessureDataTVE();
	WORD getMessureDataTVEresp();
	WORD getMessureDataTVEpat();
	SHORT getAppliedFiO2para();
	SHORT getMessureDataO2();
	void setMessureDataO2(SHORT value);
	BYTE getAVGMessureDataBPM();
	BYTE getBTBMessureDataBPM();
	BYTE getAVGMessureDataLeak();
	BYTE getBTBMessureDataLeak();
	WORD getAVGMessureDataHFAmpl();
	WORD getBTBMessureDataHFAmpl();
	BYTE getAVGMessureDataHFFreq();
	BYTE getBTBMessureDataHFFreq();
	WORD getAVGMessureDataTVEHFO();
	WORD getBTBMessureDataTVEHFO();
	WORD getAVGMessureDataDCO2();
	WORD getBTBMessureDataDCO2();
	WORD getAVGMessureDataTrigger();
	WORD getBTBMessureDataTrigger();
	WORD getAVGMessureDataITimePSV();
	WORD getBTBMessureDataITimePSV();
	WORD getMessureDataTVI();
	SHORT getAVGMessureDataETCO2();
	void resetCO2MessureData();
	BYTE getAVGMessureDataBPMco2();
	BYTE getBTBMessureDataBPMco2();
	int getAVGMessureDataFICO2();
	SHORT getAVGMessureDataSPO2();
	SHORT getAVGMessureDataSpO2PulseRate();
	SHORT getAVGMessureDataSpO2PerfusionIndex();

	void resetSPO2MessureData();

	void sendMessureDataVolumeCorrection();
	void setMessureDataVolumeCorrection(BYTE value);

	int calculateAutoAlarmlimitVgaranty();
	int calculateAutoAlarmlimitVlimit();
	int calculateAutoAlarmlimitMVmax();
	int calculateAutoAlarmlimitMVmin();
	int calculateAutoAlarmlimitPIPHIGH();
	int calculateAutoAlarmlimitPIPLOW();
	int calculateAutoAlarmlimitPEEPmin();
	int calculateAutoAlarmlimitBPMmax();
	int calculateAutoAlarmlimitLeakmax();
	int calculateAutoAlarmlimitApnoe();
	int calculateAutoAlarmlimitMAPmax();
	int calculateAutoAlarmlimitMAPmin();
	int calculateAutoAlarmlimitDCO2max();
	int calculateAutoAlarmlimitDCO2min();
	int calculateAutoAlarmlimitETCO2max();
	int calculateAutoAlarmlimitETCO2min();
	int calculateAutoAlarmlimitFICO2max();
	int calculateAutoAlarmlimitFICO2min();
	int calculateAutoAlarmlimitSPO2max();
	int calculateAutoAlarmlimitSPO2min();
	int calculateAutoAlarmlimitPulseRatemax();
	int calculateAutoAlarmlimitPulseRatemin();
	int calculateAutoAlarmlimitSPO2_PImin();
	int calculateAutoAlarmlimitSPO2_SIQmin();

	bool CheckIErelation(int iITime,int iBPM,int iETime);

	bool ConfirmItoBPMrelation(int iITime,int iBPM,int iETime);
	bool ConfirmItoErelation(int iITime,int iETime,int iBPM);

	bool CalculateETime_ITimeChanged(int fITime, bool bTemp);
	bool CalculateBPM_ITimeChanged(int fITime, bool bTemp);
	bool CalculateETime_BPMChanged(int iBpm, bool bTemp);
	bool CalculateBPM_ETimeChanged(int fETime, bool bTemp);

	bool InitialCalculateETime_ITimeChanged_TRIGGER(int fITime, bool bTemp);
	bool InitialCalculateETime_ITimeChanged_IPPV(int fITime, bool bTemp);
	bool InitialCalculateETime_ITimeChanged_NMODE(int fITime, bool bTemp);
	bool InitialCalculateBPM_ITimeChanged_TRIGGER(int fITime, bool bTemp);
	bool InitialCalculateBPM_ITimeChanged_IPPV(int fITime, bool bTemp);
	bool InitialCalculateBPM_ITimeChanged_NMODE(int fITime, bool bTemp);

	int GetCurrentTempITime();
	void SetCurrentTempITime(int val);

	int GetCurrentTempETime();
	void SetCurrentTempETime(int val);

	int GetSaveIERelationValue_ITIME();
	void SetSaveIERelationValue_ITIME(int val);
	int GetSaveIERelationValue_ETIME();
	void SetSaveIERelationValue_ETIME(int val);

	void SetCurrentRisetimePara(WORD val);
	WORD GetCurrentRisetimePara();

	void SetCurrentHFFlowPara(WORD val);
	WORD GetCurrentHFFlowPara();

	void SetCurrentIFlowPara(WORD val);
	WORD GetCurrentIFlowPara();

	void SetFlowMinParadata(WORD val);
	void SetCurrentEFlowPara(WORD val);
	WORD GetCurrentEFlowPara();
	WORD GetFlowminParaData();

	void SetCurrentTherapieFlowParaData(WORD val);
	WORD GetCurrentTherapieFlowParaData();

	void ChangeVgarantState(eVgarantState state);
	eVgarantState GetVgarantState();

	void SetCurrentPmaxVolGPara(SHORT val);
	SHORT GetCurrentPmaxVolGPara();

	void SetCurrentPINSPPara(SHORT val);
	SHORT GetCurrentPINSPPara();

	SHORT GetActiveModePRESSUREPara();
	SHORT GetActiveModePINSPPara();
	SHORT GetActiveModePMAXVGPara();

	void SetCurrentPEEPPara(SHORT val);
	SHORT GetCurrentPEEPPara();

	void SetCurrentTriggerPara(BYTE val);
	BYTE GetCurrentTriggerPara();

	void SetDUOPAPtriggerAutoenableFlag(BYTE iPrevTRIGGERDUOPAPPara);
	BOOL GetDUOPAPtriggerAutoenableFlag();
	void ResetDUOPAPtriggerAutoenableFlag();

	WORD GetPrevTRIGGERDUOPAPPara();
	void SetTriggerDUOPAPenabled();
	void SetTriggerDUOPAPdisabled();
	bool GetTriggerDUOPAPenabled();

	void SetCurrentITimePara(WORD val);
	WORD GetCurrentITimePara();

	void SetCurrentETimePara(WORD val);
	WORD GetCurrentETimePara();

	void CalculateIERatioParts();

	int GetIERatioIpart();
	void SetIERatioIpart(int iVal);

	int GetIERatioEpart();
	void SetIERatioEpart(int iVal);

	void SetCurrentBPMPara(WORD val);
	WORD GetCurrentBPMPara();

	void SetCurrentO2Para(BYTE val);
	BYTE GetCurrentO2Para();

	void SetCurrentO2FlushPara(BYTE val);
	BYTE GetCurrentO2FlushPara();

	void SetCurrentPpsvPara(SHORT val);
	SHORT GetCurrentPpsvPara();

	void SetCurrentCPAPPara(SHORT val);

	void SetCurrentPManPara(SHORT val);

	void SetCurrentPMeanPara(WORD val);
	WORD GetCurrentPmeanPara();

	void SetCurrentHFFreqPara(BYTE val);
	BYTE GetCurrentHFFreqPara();

	void SetCurrentIERatioParaHFO(eRatioIE val);
	eRatioIE GetCurrentIERatioParaHFO();

	void SetCurrentHFAMPLmaxPara(WORD val);
	void SetCurrentHFAMPLPara(WORD val);
	WORD GetCurrentHFAMPLmaxPara();
	WORD GetCurrentHFAMPLPara();

	void SetCurrentITimeRecPara(WORD val);
	WORD GetCurrentITimeRecPara();

	void SetCurrentFreqRecPara(WORD val);
	WORD GetCurrentFreqRecPara();

	void SetCurrentHFPMeanRecPara(WORD val);
	WORD GetCurrentHFPMeanRecPara();

	int GetHF_ETIME_REC(int valFREQ);
	int CalculateHF_ETIME_REC(int valFREQ, int valITIME);

	void SetCurrentBackupPara(BYTE val);

	void SetCurrentVLimitParam(WORD val, bool bOn, bool bSend);
	WORD GetVLimitMinPara();
	WORD GetPrevVLimitMinPara();
	WORD GetCurrentVLimitPara();
	WORD GetPrevVLimitPara();
	bool IsCurrentModeVLimitStateOn();
	bool IsActiveModeVLimitStateOn();
	bool IsPrevVLimitStateOn();
	void SaveLastVLimitParam();
	WORD GetLastVLimitParam();

	void SetCurrentVGarantParamData(WORD val);
	WORD GetCurrentVGarantMinParaData();
	WORD GetPreviousVGarantMinParaData();
	WORD GetCurrentVGarantParaData();
	WORD GetPreviousVGarantParaData();
	bool IsCurrentModeVGarantStateOn();
	bool IsActiveModeVGarantStateOn();
	bool IsPrevVGarantStateOn();

	void setPpsvAsDeltaPEEPValue(BOOL bPPSVasDeltaPEEPValue);

	void setTriggerOptionCONV(eTriggereType type);
	eTriggereType getTriggerOptionCONV();
	void SetPrevTriggerOptionCONV(eTriggereType type);
	eTriggereType getPrevTriggerOptionCONV();

	/*void setTriggerOption_CPAP(eTriggereType type);
	eTriggereType getTriggerOption_CPAP();
	void SetPrevTriggerOption_CPAP(eTriggereType type);
	eTriggereType getPrevTriggerOption_CPAP();*/

	void setTriggerOptionDUOPAP(eTriggereType type);
	eTriggereType getTriggerOptionDUOPAP();

	void setTriggerOptionNCPAP(eTriggereType type);
	eTriggereType getTriggerOptionNCPAP();

	BYTE GetCurO2FlushTime();
	void SetCurO2FlushTime(BYTE iTime);

	int GetCurrentIERelation100();
	int GetCurrentEIRelation100();

	void setCurrentFOT_PMEANENDPara(WORD val);
	void setCurrentFOT_PMEANSTARTPara(WORD val);
	void setCurrentFOT_PEEPENDPara(WORD val);
	void setCurrentFOT_PEEPSTARTPara(WORD val);
	void setCurrentFOT_STEPSPara(WORD val);

	void checkFOTPmeanPara();
	void checkFOTPEEPPara();

	SHORT GetCurrentTherapieFlowMaxKey();
	SHORT GetCurrentPManualMaxKey();
	SHORT GetCurrentCPAPMaxKey();
	SHORT GetCurrentPInspMaxKey();
	SHORT GetCurrentPpsvMaxKey();
	SHORT GetCurrentPeepMaxKey();
	SHORT GetCurrentPeepMinKey();
	WORD GetCurrentVgarantMaxKey();
	WORD GetCurrentVlimitMaxKey();
	WORD GetCurrentIFlowMaxKey();
	WORD GetCurrentEFlowMaxKey();

	CStringW GetCurrentModeString();
	CStringW GetPrevModeString();

	eTubeSet GetTubeSet();
	void SetTubeSet(eTubeSet tube);

	void checkPRESETTriggerTubeDependency();
	void checkTriggerTubeDependency();
	void checkTriggerTubeDependency_DUOPAP();
	void checkTriggerTubeDependency_NCPAP();
	void checkTriggerTubeDependency_CPAP();
	void checkTriggerTubeDependency_IPPV();
	void checkTriggerTubeDependency_SIPPV();
	void checkTriggerTubeDependency_SIMV();
	void checkTriggerTubeDependency_HFO();

	bool IsFlowSensorStateOff();
	eFlowSensorState GetFlowSensorState();
	void SetFlowSensorState(eFlowSensorState state);
	void SetPrevFlowSensorState(eFlowSensorState prevState);
	eFlowSensorState GetPrevFlowSensorState();

	eOxySensorState GetO2SensorState();
	void SetO2SensorState(eOxySensorState state);

	eOxySensCalibrationstate GetOxySensorCalState();
	void SetOxySensorCalState(eOxySensCalibrationstate state);

	eFlowSensCalibrationstate GetFlowSensorCalState();
	void SetFlowSensorCalState(eFlowSensCalibrationstate state);

	bool GetO21SensorCalState();
	void SetO21SensorCalState(bool state);
	bool GetO100SensorCalState();
	void SetO100SensorCalState(bool state);

	bool IsFlowSensorCalibrating();
	void SetFlowSensorCalibrating(bool state);

	bool GetOxyCalRunning();
	void SetOxyCalRunning(bool state);
	void SetOxyHourglassRunning(bool state);
	bool isOxyHourglassRunning();

	void SetVentilBurnRunning(bool state);
	bool IsVentilBurnRunning();

	void SetExhalValvCalMode(bool state);
	bool GetExhalValvCalMode();

	DWORD IncreaseContrast();
	DWORD DecreaseContrast();
	DWORD IncreaseMaxContrast();
	DWORD DecreaseMaxContrast();
	DWORD GetCurContrastParm();

	void SetAlarmStateBytes(int iAlarmState);
	int GetAlarmStateBytes();
	void ResetAlarmStateBytes();

	void SetFlowsensorStateBytes(int iState);
	int GetFlowsensorStateBytes();
	void ResetFlowsensorStateBytes();

	void SetConPICChecksumError(bool state);
	bool GetConPICChecksumError();

	void SetControllerPIC_Checksum(int iChecksum);
	int GetControllerPIC_Checksum();

	void SetControllerPIC_Version(CStringW sz);
	CStringW GetControllerPIC_Version();

	void SetBlenderPIC_Version(CStringW sz);
	CStringW GetBlenderPIC_Version();

	void SetHFOPIC_Version(CStringW sz);
	CStringW GetHFOPIC_Version();

	void SetHFOPIC_Checksum(int iChecksum);
	int GetHFOPIC_Checksum();

	void SetControllerPIC_CALPRESSSCALE(int iVal);
	int GetControllerPIC_CALPRESSSCALE();
	
	void SetControllerPIC_CALPRESSOFFSET(int iVal);
	int GetControllerPIC_CALPRESSOFFSET();

	void saveOpTime();
	DWORD getOpTimeHFOMin(bool bSave);
	void setOpTimeHFOMin(UINT min);
	void resetOpTimeHFO();
	DWORD getOpTimeDeviceMin(bool bSave);
	void setOpTimeDeviceMin(UINT min);
	void resetOpTimeDevice();
	DWORD getOpTimeBatteryMin(bool bSave);
	void setOpTimeBatteryMin(UINT min);
	void resetOpTimeBattery();

	bool SetAccuSupply(bool state);
	bool IsAccuSupply();
	WORD GetAccuState();
	void CheckAkku(bool bRange=false);

	WORD GetBatteryStatMinutes();
	WORD GetBatteryStatPercent();

	void SetMSTATUS_ConductorPIC(int state);
	int GetMSTATUS_ConductorPIC();

	void SetMSTATUS_BlenderPIC(int state);
	int GetMSTATUS_BlenderPIC();

	UINT CheckLastTrendData();
	int UpdateTrendData(UINT type, COleDateTime dtTime);
	void DeleteAllTrendData();
	void SaveTrendData();

	void SetInspFlowData(int valueInsp);
	void SetExpFlowData(int valueExp);

	int GetInspFlowData();
	int GetExpFlowData();

	void SetDemandFlowData(int valueDem);
	int GetDemandFlowData();

	bool isPatientDataAvailable();
	void setPatientDataAvailable(bool bState);

	void SetPinspNotReachedFlag(bool state);
	bool GetPinspNotReachedFlag();

	void setSPIErrorCode(int bit, bool bIgnoreSilentState=false);
	void deleteSPIErrorCode();
	int getSPIErrorCode();

	void setCOMErrorCode(int iERRORcode);
	void deleteCOMErrorCode(int iERRORcode);
	void deleteAllCOMError();
	int getCOMErrorCode();
	bool setCOMLastSendError(CStringW szData);
	bool checkCOMlastSendError(CStringW szData);
	void checkCOMresetLastSendError(CStringW szData);

	void SetCO2ErrorCode(int bit, bool bIgnoreSilentState=false);
	void DeleteCO2ErrorCode(int bit);
	void DeleteCO2ErrorCode();
	int GetCO2ErrorCode();

	void SetHFOrunning(bool state);
	bool IsHFOrunning();

	int SetBit(int x, unsigned int n);
	int DeleteBit(int x, unsigned int n);

	bool IsGraphFreezed();
	void SetGraphFreezed(bool state);	

	void SetSavingTrendToUSBactiv();
	void SetSavingTrendToUSBinactiv();
	bool IsSavingTrendToUSBactiv();
	void LoadingTrendData(bool bState);
	bool IsTrendDataLoading();

	void ReadI2CWatchdogState();
	void ResetI2CWatchdogState();
	BYTE GetI2CWatchdogState();

	void SetDelTrendThread(bool state);
	bool getDelTrendThread();

	bool IsLEDdisplay();

	void SetVGarantParadata_TRIGGER(WORD val,bool bSend,bool bConfig);
	void SetVGarantParadata_IPPV(WORD val,bool bSend,bool bConfig);
	void SetHFVGarantParadata(WORD val,bool bSend,bool bConfig);
	void SetActiveModeVolumeGarantyState(bool bOn);

	void setVLIMITControlEnabled();
	void setVGARANTControlEnabled();

	void SetPresetModeVolumeGarantyState(bool bOn);
	void SetVGarantState_TRIGGER(bool bOn);
	void SetVGarantState_IPPV(bool bOn);
	void SetHFVGarantState(bool bOn);

	void SetVLimitParadata_TRIGGER(WORD val, bool bOn,bool bSend);
	void SetVLimitParadata_IPPV(WORD val, bool bOn,bool bSend);

	void SetETIMEParadata_TRIGGER(WORD value, bool bSend=true);
	void SetETIMEParadata_IPPV(WORD value, bool bSend=true);
	void SetETIMENMODEParadata(WORD value, bool bSend=true);

	void SetBPMParadata_TRIGGER(WORD value, bool bSend);
	void SetBPMParadata_IPPV(WORD value, bool bSend);
	void SetBPMNMODEParadata(WORD value, bool bSend);

	void SetTriggerCPAPParadata(BYTE value, bool bSend=true);
	void SetTriggerCONVParadata(BYTE value, bool bSend=true);
	void SetTriggerDUOPAPParadata(BYTE value, bool bSend=true);
	void SetTriggerNCPAPParadata(BYTE value, bool bSend=true);

	void SetITimeParadata_TRIGGER(WORD value, bool bSend=true);
	void SetITimeParadata_IPPV(WORD value, bool bSend=true);
	void SetITimeNMODEParadata(WORD value, bool bSend=true);

	void SetO2Paradata(BYTE value, bool bSend=true);
	void SetO2FlushParadata(BYTE value);

	void SetRisetimeParadata_TRIGGER(WORD value, bool bSend=true);
	void SetRisetimeParadata_IPPV(WORD value, bool bSend=true);

	void SetHFFlowParadata(WORD value, bool bSend=true);

	void SetIFlowParadata_TRIGGER(WORD value, bool bSend=true);
	void SetIFlowParadata_IPPV(WORD value, bool bSend=true);

	void SetEFlowParadata_TRIGGER(WORD value, bool bSend=true);
	void SetEFlowParadata_IPPV(WORD value, bool bSend=true);
	void SetFlowminParadata(WORD value, bool bSend=true);
	void SetTherapieFLOWParadata(WORD value, bool bSend);

	void SetPmaxVolGParadata_TRIGGER(SHORT value, bool bSend);
	void SetPmaxVolGParadata_IPPV(SHORT value, bool bSend);

	void SetPINSPParadata_TRIGGER(SHORT value, bool bSend);
	void SetPINSPParadata_IPPV(SHORT value, bool bSend);

	void SetPEEPParadata_TRIGGER(SHORT value, bool bSend=true);
	void SetPEEPParadata_IPPV(SHORT value, bool bSend=true);

	void SetFOTconv_AMPLITUDEParadata(WORD value, bool bSend);
	void SetFOTconv_FREQParadata(WORD value, bool bSend);
	void SetFOTconv_STEPSParadata(WORD value, bool bSend);
	void SetFOTconv_PEEPSTARTParadata(WORD value, bool bSend);
	void SetFOTconv_PEEPENDParadata(WORD value, bool bSend);
	void SetFOThfo_AMPLITUDEParadata(WORD value, bool bSend);
	void SetFOThfo_FREQParadata(WORD value, bool bSend);
	void SetFOThfo_STEPSParadata(WORD value, bool bSend);
	void SetFOThfo_PMEANSTARTParadata(WORD value, bool bSend);
	void SetFOThfo_PMEANENDParadata(WORD value, bool bSend);
	
	void SetPpsvParadata(SHORT value, bool bSend=true);
	void SetCPAPParadata(SHORT value, bool bSend=true);
	void SetCPAPNMODEParadata(SHORT value, bool bSend=true);
	void SetPManualHFOParadata(SHORT value, bool bSend=true);
	void SetPManualCPAPParadata(SHORT value, bool bSend=true);
	void SetPManualNMODEParadata(SHORT value, bool bSend=true);
	void SetHFPMeanParadata(WORD value, bool bSend=true);
	void SetBackupParadata(BYTE value, bool bSend=true);
	void SetIERatioParadataHFO(eRatioIE ratio, bool bSend=true);
	void SetHFFreqParadata(BYTE value, bool bSend=true);
	void SetITimeRecParadata(WORD value, bool bSend=true);
	void SetFreqRecParadata(WORD value, bool bSend=true);
	void SetHFPMeanRecParadata(WORD value, bool bSend=true);
	void SetHFAMPLmaxParadata(WORD value, bool bSend);
	void SetHFAMPLParadata(WORD value, bool bSend);
	void SetVLimitParamdata_TRIGGER(WORD val, bool bOn,bool bSend);
	void SetVLimitParamdata_IPPV(WORD val, bool bOn,bool bSend);

	void setStatus2(SHORT state);
	SHORT getStatus2();

	void SerializeTrend(UINT type, bool bIncreaseFileNum);
	void SerializeAllTrends(bool bIncreaseFileNum);
	bool DeserializeTrend(UINT type, BYTE fileNum);
	bool DeserializeTempTrend(UINT type, WORD fileNum);

	void saveTempTrends();

	BYTE getCountNumericIPPV();
	BYTE getCountNumericSIPPV();
	BYTE getCountNumericSIMV();
	BYTE getCountNumericSIMVPSV();
	BYTE getCountNumericPSV();
	BYTE getCountNumericCPAP();
	BYTE getCountNumericHFO();
	BYTE getCountNumericNCPAP();
	BYTE getCountNumericDUOPAP();
	BYTE getCountNumericTHERAPY();
	BYTE getCountNumericFLOWOFFCONV();
	BYTE getCountNumericFLOWOFFCPAP();
	BYTE getCountNumericFLOWOFFHFO();

	COLORREF getGraphColor_PRESSURE();
	COLORREF getGraphColor_FLOW();
	COLORREF getGraphColor_VOLUME();
	COLORREF getGraphColor_ETCO2();
	COLORREF getGraphColor_SPO2();
	COLORREF getGraphColor_LOOP();
	COLORREF getGraphColor_SAVEDLOOP();
	COLORREF getGraphColor_TRIGGER();

	int getSizeSavedBreath();
	void setSizeSavedBreath(int iSize);

	void enableProcPressureCal60();
	void disableProcPressureCal60();
	bool isProcPressureCal60enabled();

	void setRemainCO2PumpTime(DWORD time);
	DWORD getRemainCO2PumpTime();

	void setFOToscillationState(bool bOsciRun);
	bool getFOToscillationState();

	void setBodyweight(WORD weightGramm, bool bLog);
	WORD getBodyweight();

	double getAmpCorFactor(BYTE iFreq);

private:
	//singleton
	static CDataHandler* theDataHandler;	///< singleton of data handler

protected:

	CDataHandler(void);
	virtual ~CDataHandler(void);

	CMVModel *getModel();

	DWORD GetContrast(void );
	void SetContrast(DWORD dwValue );

public:
	CRITICAL_SECTION	csTrend;	///< critical section for tend data
	CRITICAL_SECTION	csTrendUpdate;	///< critical section for tend update
	CRITICAL_SECTION	csTrendFileData;	///< critical section for file of trend data
	CRITICAL_SECTION	csDelTrendThread;   ///< critical section for deleteing trend data //rkuNEWFIX
	CRITICAL_SECTION	csOpTime;   ///< critical section for operating time
	CRITICAL_SECTION	csSPIDataBuffer;	///< critical section for SPI buffer
	CRITICAL_SECTION	csCopyDataBuffer;   ///< critical section for copy of graph buffer
	CRITICAL_SECTION	csVentDataBuffer;   ///< critical section for graph buffer
	CRITICAL_SECTION	csCO2DataBuffer;	///< critical section for ETCO2 buffer
	CRITICAL_SECTION	csSPO2DataBuffer;   ///< critical section for SPO2 buffer
	CRITICAL_SECTION	csSavedBreath;  ///< critical section for saved breath buffer

	static CircularBuffer<PBUFSPI> m_rbufVent;  ///< ring buffer for a copy of graph data for freeze/scaling
	static CircularBuffer<PBUFSPI> m_rbufCopy;  ///< ring buffer for graph data
	static CircularBuffer<PBUFSPI> m_rbufSPI;   ///< ring buffer for SPI graph data
	static CircularBuffer<SHORT> m_rbufCO2; ///< ring buffer for ETCO2 data
	static CircularBuffer<PBUFSPO2> m_rbufSPO2; ///< ring buffer for SPO2 data
	
	static PBUFSPI* m_pbufSavedBreath;  ///< The pbuf saved breath
	int m_iSizeSavedBreath; ///< The size saved breath
	
	static LPNUMERICINI m_pbufNumericIPPV;  ///< The pbuf numeric ippv
	static LPNUMERICINI m_pbufNumericSIPPV; ///< The pbuf numeric sippv
	static LPNUMERICINI m_pbufNumericSIMV;  ///< The pbuf numeric simv
	static LPNUMERICINI m_pbufNumericSIMVPSV;   ///< The pbuf numeric simvpsv
	static LPNUMERICINI m_pbufNumericPSV;   ///< The pbuf numeric psv
	static LPNUMERICINI m_pbufNumericCPAP;  ///< The pbuf numeric cpap
	static LPNUMERICINI m_pbufNumericHFO;   ///< The pbuf numeric hfo
	static LPNUMERICINI m_pbufNumericNCPAP; ///< The pbuf numeric ncpap
	static LPNUMERICINI m_pbufNumericDUOPAP;	///< The pbuf numeric duopap
	static LPNUMERICINI m_pbufNumericTHERAPY;   ///< The pbuf numeric therapy
	static LPNUMERICINI m_pbufNumericFLOWOFFCONV;   ///< The pbuf numeric flowoffconv
	static LPNUMERICINI m_pbufNumericFLOWOFFCPAP;   ///< The pbuf numeric flowoffcpap
	static LPNUMERICINI m_pbufNumericFLOWOFFHFO;	///< The pbuf numeric flowoffhfo
	
	CTrend m_cTrendTemporary;   ///< The trend temporary

private:

	CMVModel *m_pModel; ///< the model

	CRITICAL_SECTION	csFOTosciState; ///< critical section for FOT oscillation
	CRITICAL_SECTION	csMainboardData;	///< critical section mainboard data
	CRITICAL_SECTION	csMessureDataAVG;   ///< critical section  for measured averaga data
	CRITICAL_SECTION	csMessureDataBTB;   ///< critical section  for measured breath to breath data
	CRITICAL_SECTION	csMessureData;  ///< critical section  for measure data
	CRITICAL_SECTION	csAlarmStateBytes;  ///< critical section for alarm state
	CRITICAL_SECTION	csFlowsensorStateBytes; ///< critical section for sensor state
	CRITICAL_SECTION	csFlowsensorState;  ///< critical section for flow sensor calibration
	CRITICAL_SECTION	csOxyCal;   ///< critical section for oxy calibration
	CRITICAL_SECTION	csOxyState; ///< critical section for oxy sensor state
	
	static CParaDataPRESET* m_PRESET;   ///< Structure for parameter values of preset mode
	static CParaData* m_PARADATA;   ///< Structure for parameter values of ventilation modes
	
	CTrend m_cTendPINSP;	///< Trend of PINSP
	CTrend m_cTendPMEAN;	///< Trend of PMEAN
	CTrend m_cTendFIO2; ///< Trend of FIO2
	CTrend m_cTendVTE;  ///< Trend of VTE
	CTrend m_cTendCOMPLIANCE;   ///< Trend of COMPLIANCE
	CTrend m_cTendCO2HFO;   ///< Trend of CO2HFO
	CTrend m_cTendMV;   ///< Trend of MV
	CTrend m_cTendHFAMP;	///< Trend of HFAMP
	CTrend m_cTendRSBI; ///< The tend rsbi
	CTrend m_cTendShareMVmand;  ///< The tend share m vmand
	CTrend m_cTendResistance;   ///< The tend resistance
	CTrend m_cTendLeak; ///< The tend leak
	CTrend m_cTendSpO2; ///< The second tend sp o
	CTrend m_cTendSpO2PI;   ///< The tend sp o 2 pi
	CTrend m_cTendEtCO2;	///< The second tend et co
	CTrend m_cTendSpO2PR;   ///< The tend sp o 2 pr
	CTrend m_cTendFrequency;	///< The tend frequency
	
	CStringW m_szCOMlastSendError;  ///< The co mlast send error
	CStringW m_szLicenseFile;   ///< The license file
	
	MAINBOARD_DATA		m_dataMainboard;	///< The data mainboard
	MESSURE_DATA		m_dataMessure;  ///< The data messure

	static INT* m_pbufMessureAVG;   ///< The pbuf messure a vg
	static INT* m_pbufMessureBTB;   ///< The pbuf messure btb

	eFlowSensorState m_ePrevFlowSensorState;	///< State of the previous flow sensor
	eFlowSensorState m_eFlowSensorState;	///< State of the flow sensor
	eOxySensorState m_eO2SensorState;   ///< State of the o 2 sensor

	eOxySensCalibrationstate m_eOxySensorCalState;  ///< State of the oxy sensor calendar
	eFlowSensCalibrationstate m_eFlowSensorCalState;	///< State of the flow sensor calendar
	
	eVgarantState m_eCurVgarantState;   ///< The current vgarant state

	eTriggereType m_ePrevTrigger_CONV;   ///< The previous convert trigger
	//eTriggereType m_ePrevTrigger_CPAP;

	double m_fTrendData_Pmean;  ///< The f trend data pmean
	double m_fTrendData_FiO2;   ///< The second f trend data fi o
	double m_fTrendData_Pinsp;  ///< The f trend data pinsp
	double m_fTrendData_Vte;	///< The f trend data vte
	double m_fTrendData_Compliance; ///< The f trend data compliance
	double m_fTrendData_CO2HFO; ///< The f trend data co 2 hfo
	double m_fTrendData_MV; ///< The f trend data mv
	double m_fTrendData_HFAMP;  ///< The f trend data hfamp
	double m_fTrendData_RSBI;   ///< The f trend data rsbi
	double m_fTrendData_ShareMVmand;	///< The f trend data share m vmand
	double m_fTrendData_Resistance; ///< The f trend data resistance
	double m_fTrendData_Leak;   ///< The f trend data leak
	double m_fTrendData_SpO2;   ///< The second f trend data sp o
	double m_fTrendData_PI; ///< The f trend data pi
	double m_fTrendData_etCO2;  ///< The second f trend data et co
	double m_fTrendData_SpO2PR; ///< The f trend data sp o 2 pr
	double m_fTrendData_Frequency;  ///< The f trend data frequency

	COLORREF m_crGraphColor_PRESSURE;   ///< The carriage return graph color pressure
	COLORREF m_crGraphColor_FLOW;   ///< The carriage return graph color flow
	COLORREF m_crGraphColor_VOLUME; ///< The carriage return graph color volume
	COLORREF m_crGraphColor_ETCO2;  ///< The second carriage return graph color etco
	COLORREF m_crGraphColor_SPO2;   ///< The second carriage return graph color spo
	COLORREF m_crGraphColor_LOOP;   ///< The carriage return graph color loop
	COLORREF m_crGraphColor_SAVEDLOOP;  ///< The carriage return graph color savedloop
	COLORREF m_crGraphColor_TRIGGER;	///< The carriage return graph color trigger

	DWORD m_dwOpTimeDevTickCountStarted;	///< The operation time development tick count started
	DWORD m_dwOpTimeBattTickCountStarted;   ///< The operation time batt tick count started
	DWORD m_dwOpTimeHFOTickCountStarted;	///< The operation time hfo tick count started
	DWORD m_dwRemainCO2PumpTime;	///< The remain co 2 pump time
	DWORD m_iContrastParm;  ///< The contrast parameter
	
	int m_iCountDIOERROR;   ///< The count dioerror
	int m_iAlarmState;  ///< State of the alarm
	int m_iFlowsensorState; ///< State of the flowsensor
	int m_iConductorCALPRESSSCALE;  ///< The conductor calpressscale
	int m_iConductorCALPRESSOFFSET; ///< The conductor calpressoffset
	int m_iTempITime;   ///< The temporary i time
	int m_iTempETime;   ///< The temporary e time
	int m_iSaveIERelationValue_ITIME;   ///< The i save IE relation value itime
	int m_iSaveIERelationValue_ETIME;   ///< The i save IE relation value etime
	int m_iMSTATUS_ConPIC;  ///< The i mstatus con picture
	int m_iMSTATUS_BlenderPIC;  ///< The i mstatus blender picture
	int m_iCOMErrorCommandBits; ///< 32 Bit
	int m_iPmeanDifference; ///< The pmean difference
	int m_iPEEP_PPSVdifference; ///< The i peep pps vdifference
	int m_iIERatioIpart;	///< The IE ratio ipart
	int m_iIERatioEpart;	///< The IE ratio epart
	
	
	UINT m_iSPIErrorCodeBits;   ///< The spi error code bits
	UINT m_iCOMErrorCodeBits;   ///< 32 Bit
	UINT m_iCO2ErrorCodeBits;   ///< The co 2 error code bits
	UINT m_iTrendCnt;   ///< Number of trends
	UINT m_nCountDelTrends; ///< The count delete trends

	SHORT m_Status2;	///< The second status
	SHORT m_iSPO2waveData;  ///< Information describing the spo 2wave
	
	SHORT m_sTempTrendData_Pmean;
	WORD m_wTempTrendData_Pinsp;
	WORD m_wTempTrendData_FiO2;
	WORD m_wTempTrendData_Vte;
	WORD m_wTempTrendData_Compliance;
	WORD m_wTempTrendData_CO2HFO;
	WORD m_wTempTrendData_MV;
	WORD m_wTempTrendData_HFAMP;
	WORD m_wTempTrendData_RSBI;
	WORD m_wTempTrendData_ShareMVmand;
	WORD m_wTempTrendData_Resistance;
	WORD m_wTempTrendData_Leak;
	WORD m_wTempTrendData_SpO2;
	WORD m_wTempTrendData_PI;
	WORD m_wTempTrendData_etCO2;
	WORD m_wTempTrendData_SpO2PR;
	WORD m_wTempTrendData_Frequency;
	COleDateTime m_dtTempTimestamp;

	WORD m_wACCU_DAT_STATE; ///< State of the w a ccu dat
	WORD m_wBattStatMinutes;	///< The batt stat in minutes
	WORD m_wBattStatPercents;   ///< The batt stat percents
	WORD m_iLastVGarantParam;   ///< The last v garant parameter
	WORD m_iLastHFVGarantParam; ///< The last hfv garant parameter
	WORD m_iLastVLimitParam;	///< The last v limit parameter
	WORD m_iBodyweightGramm;	///< The bodyweight gramm
	WORD m_wHardwareConfig; ///< The hardware configuration
	
	BYTE m_iNumericIPPVcount;   ///< The numeric ipp vcount
	BYTE m_iNumericSIPPVcount;  ///< The numeric sipp vcount
	BYTE m_iNumericSIMVcount;   ///< The numeric simulation vcount
	BYTE m_iNumericSIMVPSVcount;	///< The numeric simvps vcount
	BYTE m_iNumericPSVcount;	///< The numeric ps vcount
	BYTE m_iNumericCPAPcount;   ///< The numeric cpa pcount
	BYTE m_iNumericHFOcount;	///< The numeric hf ocount
	BYTE m_iNumericNCPAPcount;  ///< The numeric ncpa pcount
	BYTE m_iNumericDUOPAPcount; ///< The numeric duopa pcount
	BYTE m_iNumericTHERAPYcount;	///< The numeric therap ycount

	BYTE m_iNumericFLOWOFFCONVcount;	///< The numeric flowoffcon vcount
	BYTE m_iNumericFLOWOFFCPAPcount;	///< The numeric flowoffcpa pcount
	BYTE m_iNumericFLOWOFFHFOcount; ///< The numeric flowoffhf ocount

	BYTE m_chFilenumPINSP;  ///< The filenum pinsp
	BYTE m_chFilenumPMEAN;  ///< The filenum pmean
	BYTE m_chFilenumFIO2;   ///< The second filenum fio
	BYTE m_chFilenumVTE;	///< The filenum vte
	BYTE m_chFilenumCOMPLIANCE; ///< The filenum compliance
	BYTE m_chFilenumCO2HFO; ///< The filenum co 2 hfo
	BYTE m_chFilenumMV; ///< The filenum mv
	BYTE m_chFilenumHFAMP;  ///< The filenum hfamp
	BYTE m_chFilenumRSBI;   ///< The filenum rsbi
	BYTE m_chFilenumShareMVmand;	///< The filenum share m vmand
	BYTE m_chFilenumResistance; ///< The filenum resistance
	BYTE m_chFilenumLeak;   ///< The filenum leak
	BYTE m_chFilenumSpO2;   ///< The second filenum sp o
	BYTE m_chFilenumSPO2PI; ///< The filenum spo 2 pi
	BYTE m_chFilenumEtCO2;  ///< The second filenum et co
	BYTE m_chFilenumSpO2PR; ///< The filenum sp o 2 pr
	BYTE m_chFilenumFrequency;  ///< The filenum frequency

	BYTE m_iO2Difference;   ///< The o 2 difference
	BYTE m_byI2CWatchdogState;  ///< State of the by i 2 c watchdog
	BYTE m_iPrevTRIGGERDUOPAPPara;   ///< The previous trigge rnmode para
	
	bool m_bVGUARANTDemoLicAvailable;   ///< True if vguarant demo lic available
	bool m_bVLIMITDemoLicAvailable; ///< True if vlimit demo lic available
	bool m_bHFODemoLicAvailable;	///< True if hfo demo lic available
	bool m_bNMODEDemoLicAvailable;  ///< True if nmode demo lic available
	bool m_bLUNGRECDemoLicAvailable;	///< True if lungrec demo lic available
	bool m_bTRENDDemoLicAvailable;  ///< True if trend demo lic available
	bool m_bTHERAPYDemoLicAvailable;	///< True if therapy demo lic available
	bool m_bPRICODemoLicAvailable;  ///< True if prico demo lic available
	bool m_bFOTDemoLicAvailable;	///< True if fot demo lic available
	bool m_bNIVTRIGGERDemoLicAvailable; ///< True if nivtrigger demo lic available

	bool m_bVGUARANTLicenseAvailable;   ///< True if vguarant license available
	bool m_bVLIMITLicenseAvailable; ///< True if vlimit license available
	bool m_bHFOLicenseAvailable;	///< True if hfo license available
	bool m_bNMODELicenseAvailable;  ///< True if nmode license available
	bool m_bLUNGRECLicenseAvailable;	///< True if lungrec license available
	bool m_bTRENDLicenseAvailable;  ///< True if trend license available
	bool m_bTHERAPYLicenseAvailable;	///< True if therapy license available
	bool m_bPRICOLicenseAvailable;  ///< True if prico license available
	bool m_bFOTLicenseAvailable;	///< True if fot license available
	bool m_bNIVTRIGGERLicenseAvailable; ///< True if nivtrigger license available

	bool m_bPRICOrunning;   ///< True to pric orunning
	bool m_bDoDelTrendThread;   ///< True to do delete trend thread
	bool m_bConPICChecksumError;	///< True to con picture checksum error
	bool m_bAccuSupply; ///< True to accu supply
	bool m_bShowBatteryEmpty;   ///< True to show, false to hide the battery empty
	bool m_bShowBattery60;  ///< True to show, false to hide the battery 60
	bool m_bShowBattery30;  ///< True to show, false to hide the battery 30
	bool m_bShowBatteryState15; ///< True to show, false to hide the battery state 15
	bool m_bI2CError;   ///< True to i 2 c error
	bool m_bExhalValvCalMode;   ///< True to enable exhal valv calendar mode, false to disable it
	bool m_bO21SensorCalState;  ///< True to o 21 sensor calendar state
	bool m_bO100SensorCalState; ///< True to o 100 sensor calendar state
	bool m_bFlowSensorCalState; ///< True to flow sensor calendar state
	bool m_bOxyCalRunning;  ///< True to oxy calendar running
	bool m_bOxyHourglassRunning;	///< True to oxy hourglass running
	bool m_bVentilBurnRunning;  ///< True to ventil burn running
	bool m_bPinspNotReached;	///< True if pinsp not reached
	bool m_bPatientDataAvailable;   ///< True if patient data available
	bool m_bVolGarantyRunning;  ///< True to volume garanty running
	bool m_bPSVapnoe;   ///< True to ps vapnoe
	bool m_bNurscallAlarm;  ///< True to nurscall alarm
	bool m_bHFOrunning; ///< True to hf orunning
	bool m_bFreezedGraphs;  ///< True to freezed graphs
	bool m_bSavingTrendToUSB;   ///< True to saving trend to USB
	bool m_bTrendsLoading;  ///< True to trends loading
	bool m_bLEDdisplay; ///< True to le ddisplay
	bool m_bExit;   ///< True to exit
	bool m_bProcPressureCal60;  ///< True to proc pressure calibration 60
	bool m_bFOToscillationState;	///< True to fo toscillation state
	bool m_bDUOPAPtriggerAutoEnable; ///< True if nmod etrigger automatic enable
	bool m_bTriggerDUOPAPenabled;	///< True if trigger nmod eenabled

	bool m_bTrendUpdateRunning;
	bool m_bInitialSaveTrend;

	BOOL m_bShowFlowAC; ///< True to show, false to hide the flow a c

	
	

	
};
