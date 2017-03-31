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

	//eVentMode getActiveVentMode();


	bool checkLicensing();
	void WriteLicenseFile();

	void setDemoLicense(eModule module);
	void delDemoLicense(eModule module, bool bReset);
	void endableDemoLicense(eModule module);
	//void checkDemoLicense();
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
	void setPRICOon();//PRICO04
	void setPRICOoff();//PRICO04

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

	//bool isHFOManBreathAvailable();PMAN1

	BOOL showFlowAC();

	void loadConfig();
	void loadIniConfiguration();
	void loadDiagrammColor();
	void loadNumerics();
	//void loadNumericData();
	void setNumericINIstate(NUMERICINI* pbufNumeric,BYTE iBlock,CStringW szNum);
	void readNumericINIvalues(NUMERICINI* pbufNumeric,BYTE iBlock,BYTE num,CStringW szNum);
	eNumericType getNumericType(CStringW szVal);
	eNumericSize getNumericSize(CStringW szSize);

	

	CParaDataPRESET* PRESET();
	CParaData* PARADATA();
	
	void setMainboardData();
	void getMainboardData(MAINBOARD_DATA* pDataMainboard);

	/*bool modeIsPSVmode();
	bool activeModeIsVM_PSV();
	bool activeModeHasPSV();
	
	bool activeModeIsTriggerMode();
	bool activeModeIsHFO();
	bool activeModeIsNMODE();
	bool activeModeIsCPAP();
	bool activeModeIsIPPV();
	bool curModeIsVolumeSupportMode();*/

	void setPSVapnoe(bool bState);
	bool isPSVapnoe();

	void SetNurscallAlarm(bool state);
	bool isNurscallAlarm();

	/*void writeEMGBuffer(const PBUFEMG& pBufData);
	void writeEMGwaveData(SHORT iVal);
	SHORT readEMGwaveData();*/


	void writeSPO2Buffer(const PBUFSPO2& pBufData);
	void writeSPO2waveData(SHORT iVal);
	SHORT readSPO2waveData();
	void writeSpO2SIQ(SHORT iSIQvalue);
	SHORT readSpO2SIQ();
	//void setSpO2InterpolationValues(SPO2_INTERPOLATION* pStruct);

	//void writeCO2Buffer(const PBUFCO2& pBufData);
	void writeCO2Buffer(const SHORT& pBufData);
	
	void writeDataBuffer(const PBUFSPI& pBufData);
	void writeCopyDataBuffer(const PBUFSPI& pBufData);
	void updateCopyDataBuffer();

	//void updateFOTDataBuffer();

	//void setPRICO_O2(BYTE iCalcFiO2);//PRICO04
	void setO2FlushDiff(BYTE iVal);
	void setO2Diff(BYTE iFlushVal);
	BYTE getO2Diff();

	void setPmeanRecDifference(int iVal);
	//void setPmeanDiff(int iVal);
	int getPmeanDifference();

	void setPEEP_PpsvDifference(int iDiff);
	int getPEEP_PpsvDifference();;
	
	//void VentModeChanged(eVentMode state);
	void checkVGdependency();//rku, VGVL check

	//messure data
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
	//void setAVGMessureDataPINSP(SHORT value);
	SHORT getBTBMessureDataPINSP();
	//void setBTBMessureDataPINSP(SHORT value);

	SHORT getAVGMessureDataPmitt();
	//void setAVGMessureDataPmitt(SHORT value);
	SHORT getBTBMessureDataPmitt();
	//void setBTBMessureDataPmitt(SHORT value);

	SHORT getAVGMessureDataPEEP();
	//void setAVGMessureDataPEEP(SHORT value);
	SHORT getBTBMessureDataPEEP();
	//void setBTBMessureDataPEEP(SHORT value);

	WORD getAVGMessureDataCompliance();
	//void setAVGMessureDataCompliance(WORD value);
	WORD getBTBMessureDataCompliance();
	//void setBTBMessureDataCompliance(WORD value);

	WORD getAVGMessureDataC20C();
	//void setAVGMessureDataC20C(WORD value);
	WORD getBTBMessureDataC20C();
	//void setBTBMessureDataC20C(WORD value);

	WORD getAVGMessureDataResistance();
	//void setAVGMessureDataResistance(WORD value);
	WORD getBTBMessureDataResistance();
	//void setBTBMessureDataResistance(WORD value);

	WORD getAVGMessureDataMV();
	//void setAVGMessureDataMV(WORD value);
	WORD getBTBMessureDataMV();
	//void setBTBMessureDataMV(WORD value);

	BYTE getAVGMessureDataPercent();
	//void setAVGMessureDataPercent(BYTE value);
	BYTE getBTBMessureDataPercent();
	//void setBTBMessureDataPercent(BYTE value);

	WORD getMessureDataTVE();
	//WORD getAVGMessureDataTVE();
	//void setAVGMessureDataTVE(WORD value);
	//WORD getBTBMessureDataTVE();
	//void setBTBMessureDataTVE(WORD value);

	WORD getMessureDataTVEresp();
	//WORD getAVGMessureDataTVEresp();
	//void setAVGMessureDataTVEresp(WORD value);
	//WORD getBTBMessureDataTVEresp();
	//void setBTBMessureDataTVEresp(WORD value);

	WORD getMessureDataTVEpat();
	//WORD getAVGMessureDataTVEpat();
	//void setAVGMessureDataTVEpat(WORD value);
	//WORD getBTBMessureDataTVEpat();
	//void setBTBMessureDataTVEpat(WORD value);

    /*BYTE getAVGMessureDataBPM();
	void setAVGMessureDataBPM(BYTE value);
	BYTE getBTBMessureDataBPM();
	void setBTBMessureDataBPM(BYTE value);*/
	SHORT getAppliedFiO2para();
	//SHORT getMessureDataO2calindependent();
	SHORT getMessureDataO2();
	void setMessureDataO2(SHORT value);

	BYTE getAVGMessureDataBPM();
	BYTE getBTBMessureDataBPM();

	BYTE getAVGMessureDataLeak();
	//void setAVGMessureDataLeak(BYTE value);
	BYTE getBTBMessureDataLeak();
	//void setBTBMessureDataLeak(BYTE value);

	WORD getAVGMessureDataHFAmpl();
	//void setAVGMessureDataHFAmpl(WORD value);
	WORD getBTBMessureDataHFAmpl();
	//void setBTBMessureDataHFAmpl(WORD value);

	BYTE getAVGMessureDataHFFreq();
	//void setAVGMessureDataHFFreq(BYTE value);
	BYTE getBTBMessureDataHFFreq();
	//void setBTBMessureDataHFFreq(BYTE value);

	WORD getAVGMessureDataTVEHFO();
	//void setAVGMessureDataTVEHFO(WORD value);
	WORD getBTBMessureDataTVEHFO();
	//void setBTBMessureDataTVEHFO(WORD value);

	WORD getAVGMessureDataDCO2();
	//void setAVGMessureDataDCO2(WORD value);
	WORD getBTBMessureDataDCO2();
	//void setBTBMessureDataDCO2(WORD value);

	WORD getAVGMessureDataTrigger();
	//void setAVGMessureDataTrigger(WORD value);
	WORD getBTBMessureDataTrigger();
	//void setBTBMessureDataTrigger(WORD value);

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
	void SetEFlowParaData(WORD val);
	WORD GetEFlowParaData();
	WORD GetFlowminParaData();

	void SetCurrentTherapieFlowParaData(WORD val);
	WORD GetCurrentTherapieFlowParaData();

	void ChangeVgarantState(eVgarantState state);
	eVgarantState GetVgarantState();

	void SetCurrentPmaxVolGPara(SHORT val);
	SHORT GetCurrentPmaxVolGPara();
	
	void SetCurrentPINSPPara(SHORT val);
	SHORT GetCurrentPINSPPara();
	
	SHORT GetActiveModePRESSUREPara();//newVG
	SHORT GetActiveModePINSPPara();//newVG
	SHORT GetActiveModePMAXVGPara();//newVG
	
	void SetCurrentPEEPPara(SHORT val);
	SHORT GetCurrentPEEPPara();
	

	void SetCurrentTriggerPara(BYTE val);
	BYTE GetCurrentTriggerPara();
	void SetNMODEtriggerAutoenableFlag(BYTE iPrevTRIGGERnmodePara);
	BOOL GetNMODEtriggerAutoenableFlag();
	void ResetNMODEtriggerAutoenableFlag();
	WORD GetPrevTRIGGERnmodePara();

	void SetTriggerNMODEenabled();
	void SetTriggerNMODEdisabled();
	bool GetTriggerNMODEenabled();

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

	void SetCurrentVGarantParamData(WORD val);
	WORD GetCurrentVGarantMinParaData();
	WORD GetPreviousVGarantMinParaData();
	WORD GetCurrentVGarantParaData();
	WORD GetPreviousVGarantParaData();
	bool IsCurrentModeVGarantStateOn();
	bool IsActiveModeVGarantStateOn();
	bool IsPrevVGarantStateOn();

	void SaveLastVLimitParam();
	WORD GetLastVLimitParam();

	void setPpsvAsDeltaPEEPValue(BOOL bPPSVasDeltaPEEPValue);
	
	void setTriggerOptionCONV(eTriggereType type);
	eTriggereType getTriggerOptionCONV();
	void SetPrevTriggerOptionCONV(eTriggereType type);
	eTriggereType getPrevTriggerOptionCONV();

	void setTriggerOptionNMODE(eTriggereType type);
	eTriggereType getTriggerOptionNMODE();
	
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

	CStringW GetCurrentModeString();
	CStringW GetPrevModeString();

	eTubeSet GetTubeSet();
	void SetTubeSet(eTubeSet tube);

	void checkTriggerTubeDependency();

	//on/off
	bool IsFlowSensorStateOff();
	eFlowSensorState GetFlowSensorState();
	void SetFlowSensorState(eFlowSensorState state);

	void SetPrevFlowSensorState(eFlowSensorState prevState);
	eFlowSensorState GetPrevFlowSensorState();

	eOxySensorState GetO2SensorState();
	void SetO2SensorState(eOxySensorState state);

	//calibrating, defect etc
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
	
	//newSPI
	void setSPIErrorCode(int bit, bool bIgnoreSilentState=false);
	void deleteSPIErrorCode();
	int getSPIErrorCode();

	void setCOMErrorCode(int iERRORcode/*,int iSerialCommand*/);//newVG
	void deleteCOMErrorCode(int iERRORcode);//newVG
	void deleteAllCOMError();//newVG
	int getCOMErrorCode();//newVG
	bool setCOMLastSendError(CStringW szData);//newVG;
	bool checkCOMlastSendError(CStringW szData);//newVG;
	void checkCOMresetLastSendError(CStringW szData);//newVG;

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
	bool getDelTrendThread();//rkuNEWFIX

	bool IsLEDdisplay();

	//new paradata
	void SetVGarantParadata_TRIGGER(WORD val,bool bSend,bool bConfig);
	void SetVGarantParadata_IPPV(WORD val,bool bSend,bool bConfig);
	void SetHFVGarantParadata(WORD val,bool bSend,bool bConfig);
	void SetActiveModeVolumeGarantyState(bool bOn);
	void setVLIMITControlEnabled();//EFLOW1
	void setVGARANTControlEnabled();//EFLOW1
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

	void SetTriggerCONVParadata(BYTE value, bool bSend=true);
	void SetTriggerNMODEParadata(BYTE value, bool bSend=true);

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
	void SerializeAllTrends(bool bIncreaseFileNum);//rkuNEWFIX
	bool DeserializeTrend(UINT type, BYTE fileNum);
	bool DeserializeTempTrend(UINT type, WORD fileNum);

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

	void setAcuLinkStarted();
	bool isAcuLinkStarted();

	void enableProcPressureCal60();
	void disableProcPressureCal60();
	bool isProcPressureCal60enabled();

	void setRemainCO2PumpTime(DWORD time);
	DWORD getRemainCO2PumpTime();

	void setFOToscillationState(bool bOsciRun);
	bool getFOToscillationState();

	void SetBodyweight(WORD weightGramm, bool bLog);
	WORD GetBodyweight();

	double getAmpCorFactor(BYTE iFreq);

private:
	//singleton
	static CDataHandler* theDataHandler;//!< singleton of data handler

protected:
	CDataHandler(void);
	virtual ~CDataHandler(void);

	CMVModel *getModel();
	
	DWORD GetContrast(void );
	void SetContrast(DWORD dwValue );

public:
	CRITICAL_SECTION	csTrend;//!< critical section for tend data
	CRITICAL_SECTION	csTrendFileData;//!< critical section for file of trend data
	CRITICAL_SECTION	csDelTrendThread;//!< critical section for deleteing trend data //rkuNEWFIX
	CRITICAL_SECTION	csOpTime;//!< critical section for operating time
	CRITICAL_SECTION	csSPIDataBuffer;//!< critical section for SPI buffer
	CRITICAL_SECTION	csCopyDataBuffer;//!< critical section for copy of graph buffer
	CRITICAL_SECTION	csVentDataBuffer;//!< critical section for graph buffer
	CRITICAL_SECTION	csCO2DataBuffer;//!< critical section for ETCO2 buffer
	CRITICAL_SECTION	csSPO2DataBuffer;//!< critical section for SPO2 buffer
	CRITICAL_SECTION	csSavedBreath;//!< critical section for saved breath buffer


	static CircularBuffer<PBUFSPI> m_rbufVent;//!< ring buffer for a copy of graph data for freeze/scaling
	static CircularBuffer<PBUFSPI> m_rbufCopy;//!< ring buffer for graph data
	static CircularBuffer<PBUFSPI> m_rbufSPI;//!< ring buffer for SPI graph data
	static CircularBuffer<SHORT> m_rbufCO2;//!< ring buffer for ETCO2 data
	static CircularBuffer<PBUFSPO2> m_rbufSPO2;//!< ring buffer for SPO2 data
	


	static PBUFSPI* m_pbufSavedBreath;
	int m_iSizeSavedBreath;
	
	static LPNUMERICINI m_pbufNumericIPPV;
	static LPNUMERICINI m_pbufNumericSIPPV;
	static LPNUMERICINI m_pbufNumericSIMV;
	static LPNUMERICINI m_pbufNumericSIMVPSV;
	static LPNUMERICINI m_pbufNumericPSV;
	static LPNUMERICINI m_pbufNumericCPAP;
	static LPNUMERICINI m_pbufNumericHFO;
	static LPNUMERICINI m_pbufNumericNCPAP;
	static LPNUMERICINI m_pbufNumericDUOPAP;
	static LPNUMERICINI m_pbufNumericTHERAPY;
	static LPNUMERICINI m_pbufNumericFLOWOFFCONV;
	static LPNUMERICINI m_pbufNumericFLOWOFFCPAP;
	static LPNUMERICINI m_pbufNumericFLOWOFFHFO;
	
	CTrend m_cTrendTemporary;

private:

	CMVModel *m_pModel;//!< the model

	CRITICAL_SECTION	csFOTosciState;//!< critical section for FOT oscillation
	CRITICAL_SECTION	csMainboardData;//!< critical section mainboard data
	CRITICAL_SECTION	csMessureDataAVG;//!< critical section  for measured averaga data 
	CRITICAL_SECTION	csMessureDataBTB;//!< critical section  for measured breath to breath data 
	CRITICAL_SECTION	csMessureData;//!< critical section  for measure data 
	CRITICAL_SECTION	csAlarmStateBytes;//!< critical section for alarm state
	CRITICAL_SECTION	csFlowsensorStateBytes;//!< critical section for sensor state
	CRITICAL_SECTION	csFlowsensorState;//!< critical section for flow sensor calibration
	CRITICAL_SECTION	csOxyCal;//!< critical section for oxy calibration
	CRITICAL_SECTION	csOxyState;//!< critical section for oxy sensor state
	
	static CParaDataPRESET* m_PRESET;	//!< Structure for parameter values of preset mode
	static CParaData* m_PARADATA;		//!< Structure for parameter values of ventilation modes
	
	CTrend m_cTendPINSP;	//!< Trend of PINSP
	CTrend m_cTendPMEAN;	//!< Trend of PMEAN
	CTrend m_cTendFIO2;	//!< Trend of FIO2
	CTrend m_cTendVTE;	//!< Trend of VTE
	CTrend m_cTendCOMPLIANCE;	//!< Trend of COMPLIANCE
	CTrend m_cTendCO2HFO;	//!< Trend of CO2HFO
	CTrend m_cTendMV;	//!< Trend of MV
	CTrend m_cTendHFAMP;	//!< Trend of HFAMP
	CTrend m_cTendRSBI;
	CTrend m_cTendShareMVmand;
	CTrend m_cTendResistance;
	CTrend m_cTendLeak;
	CTrend m_cTendSpO2;
	CTrend m_cTendSpO2PI;
	CTrend m_cTendEtCO2;
	CTrend m_cTendSpO2PR;
	CTrend m_cTendFrequency;
	
	CStringW m_szCOMlastSendError;
	CStringW m_szLicenseFile;
	
	MAINBOARD_DATA		m_dataMainboard;
	MESSURE_DATA		m_dataMessure;

	static INT* m_pbufMessureAVG;
	static INT* m_pbufMessureBTB;

	eFlowSensorState m_ePrevFlowSensorState;
	eFlowSensorState m_eFlowSensorState;
	eOxySensorState m_eO2SensorState;

	eOxySensCalibrationstate m_eOxySensorCalState;
	eFlowSensCalibrationstate m_eFlowSensorCalState;
	
	/*ULONGLONG volatile m_ullOpTimeDevTickCountStarted;
	ULONGLONG volatile m_ullOpTimeBattTickCountStarted;
	ULONGLONG volatile m_ullOpTimeHFOTickCountStarted;*/

	DWORD m_dwOpTimeDevTickCountStarted;
	DWORD m_dwOpTimeBattTickCountStarted;
	DWORD m_dwOpTimeHFOTickCountStarted;
	DWORD m_dwRemainCO2PumpTime;
	
	int m_iCountDIOERROR;
	int m_iAlarmState;
	int m_iFlowsensorState;
	int m_iConductorCALPRESSSCALE;
	int m_iConductorCALPRESSOFFSET;
	int m_iTempITime;
	int m_iTempETime;
	int m_iSaveIERelationValue_ITIME;
	int m_iSaveIERelationValue_ETIME;
	int m_iMSTATUS_ConPIC;
	int m_iMSTATUS_BlenderPIC;
	int m_iCOMErrorCommandBits; //32 Bit
	UINT m_iSPIErrorCodeBits;
	UINT m_iCOMErrorCodeBits; //32 Bit
	UINT m_iCO2ErrorCodeBits;
	
	WORD m_wACCU_DAT_STATE;
	WORD m_wBattStatMinutes;
	WORD m_wBattStatPercents;
	
	BYTE m_iNumericIPPVcount;
	BYTE m_iNumericSIPPVcount;
	BYTE m_iNumericSIMVcount;
	BYTE m_iNumericSIMVPSVcount;
	BYTE m_iNumericPSVcount;
	BYTE m_iNumericCPAPcount;
	BYTE m_iNumericHFOcount;
	BYTE m_iNumericNCPAPcount;
	BYTE m_iNumericDUOPAPcount;
	BYTE m_iNumericTHERAPYcount;

	BYTE m_iNumericFLOWOFFCONVcount;
	BYTE m_iNumericFLOWOFFCPAPcount;
	BYTE m_iNumericFLOWOFFHFOcount;

	BYTE m_chFilenumPINSP;
	BYTE m_chFilenumPMEAN;
	BYTE m_chFilenumFIO2;
	BYTE m_chFilenumVTE;
	BYTE m_chFilenumCOMPLIANCE;
	BYTE m_chFilenumCO2HFO;
	BYTE m_chFilenumMV;
	BYTE m_chFilenumHFAMP;
	BYTE m_chFilenumRSBI;
	BYTE m_chFilenumShareMVmand;
	BYTE m_chFilenumResistance;
	BYTE m_chFilenumLeak;
	BYTE m_chFilenumSpO2;
	BYTE m_chFilenumSPO2PI;
	BYTE m_chFilenumEtCO2;
	BYTE m_chFilenumSpO2PR;
	BYTE m_chFilenumFrequency;
	
	bool m_bVGUARANTDemoLicAvailable;
	bool m_bVLIMITDemoLicAvailable;
	bool m_bHFODemoLicAvailable;
	bool m_bNMODEDemoLicAvailable;
	bool m_bLUNGRECDemoLicAvailable;
	bool m_bTRENDDemoLicAvailable;
	bool m_bTHERAPYDemoLicAvailable;
	bool m_bPRICODemoLicAvailable;
	bool m_bFOTDemoLicAvailable;

	bool m_bVGUARANTLicenseAvailable;
	bool m_bVLIMITLicenseAvailable;
	bool m_bHFOLicenseAvailable;
	bool m_bNMODELicenseAvailable;
	bool m_bLUNGRECLicenseAvailable;
	bool m_bTRENDLicenseAvailable;
	bool m_bTHERAPYLicenseAvailable;
	bool m_bPRICOLicenseAvailable;
	bool m_bFOTLicenseAvailable;
	bool m_bPRICOrunning;
	bool m_bAcuLinkStarted;
	bool m_bDoDelTrendThread;
	bool m_bConPICChecksumError;
	bool m_bAccuSupply;
	bool m_bShowBatteryEmpty;
	bool m_bShowBattery60;
	bool m_bShowBattery30;
	bool m_bShowBatteryState15;
	bool m_bI2CError;
	bool m_bExhalValvCalMode;
	bool m_bO21SensorCalState;
	bool m_bO100SensorCalState;
	bool m_bFlowSensorCalState;
	bool m_bOxyCalRunning;
	bool m_bOxyHourglassRunning;
	bool m_bVentilBurnRunning;
	bool m_bPinspNotReached;
	bool m_bPatientDataAvailable;
	bool m_bVolGarantyRunning;
	bool m_bPSVapnoe;
	bool m_bNurscallAlarm;
	
	eVgarantState m_eCurVgarantState;
	//bool m_bResendPinsp;
	
	//bool m_bSendVolumeCorrection;


	//*******Trends***********//
	/*CTlsTrendFile *m_tfilePINSP_Byte;
	CTlsTrendFile *m_tfilePMEAN_Byte;
	CTlsTrendFile *m_tfileFIO2_Byte;
	CTlsTrendFile *m_tfileVTE_Word;
	CTlsTrendFile *m_tfileCOMPLIANCE_Word;
	CTlsTrendFile *m_tfileCO2HFO_Word;*/
	

	//COleDateTime m_dtTimeLastTrend;
	
	/*CStringParser	m_strTrend1;
	CStringParser	m_strTrend2;
	CStringParser	m_strTrend3;*/

	DWORD m_iContrastParm;

	UINT m_iTrendCnt;
	double m_fTrendData_Pmean;
	double m_fTrendData_FiO2;
	double m_fTrendData_Pinsp;
	double m_fTrendData_Vte;
	double m_fTrendData_Compliance;
	double m_fTrendData_CO2HFO;
	double m_fTrendData_MV;
	double m_fTrendData_HFAMP;
	double m_fTrendData_RSBI;
	double m_fTrendData_ShareMVmand;
	double m_fTrendData_Resistance;
	double m_fTrendData_Leak;
	double m_fTrendData_SpO2;
	double m_fTrendData_PI;
	double m_fTrendData_etCO2;
	double m_fTrendData_SpO2PR;
	double m_fTrendData_Frequency;

	BYTE m_iO2Difference;
	int m_iPmeanDifference;
	int m_iPEEP_PPSVdifference;

	bool m_bHFOrunning;
	bool m_bFreezedGraphs;
	bool m_bSavingTrendToUSB;
	bool m_bTrendsLoading;
	//bool m_bFOTrunning;
	//bool m_bFOTsequenceRunning;
	
	WORD m_iLastVGarantParam;
	WORD m_iLastHFVGarantParam;
	WORD m_iLastVLimitParam;

	WORD m_wHardwareConfig;

	BYTE m_byI2CWatchdogState;

	UINT m_nCountDelTrends;

	
	bool m_bLEDdisplay;

	int m_iIERatioIpart;
	int m_iIERatioEpart;

	SHORT m_Status2;

	//SHORT m_iOldOxyValue;

	BOOL m_bShowFlowAC;

	SHORT m_iSPO2waveData;
	SHORT m_iSPO2_SIQ;
	//SHORT m_iEMGwaveData;

	COLORREF m_crGraphColor_PRESSURE;
	COLORREF m_crGraphColor_FLOW;
	COLORREF m_crGraphColor_VOLUME;
	COLORREF m_crGraphColor_ETCO2;
	COLORREF m_crGraphColor_SPO2;
	COLORREF m_crGraphColor_LOOP;
	COLORREF m_crGraphColor_SAVEDLOOP;
	COLORREF m_crGraphColor_TRIGGER;

	bool m_bExit;

	int m_iProcPressureCal60;
	bool m_bProcPressureCal60;

	bool m_bFOToscillationState;

	bool m_bNMODEtriggerAutoEnable;
	BYTE m_iPrevTRIGGERnmodePara;
	bool m_bTriggerNMODEenabled;

	WORD m_iBodyweightGramm;

	eTriggereType m_ePrevConvTrigger;
};
