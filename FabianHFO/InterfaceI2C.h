//=============================================================================
/** 
* \class CInterfaceI2C
* 		 
* \brief Interface for I2C bus.
* 		 
* \author Rainer Kuehner
* \date 2017-10-23 Creation of file
*
**/
//=============================================================================
#pragma once
#include "interface.h"
#include "globDefs.h"


#include "i2cio.h"

#define NI2CHANDLE HANDLE

class CInterfaceI2C : public CInterface
{
public:
	

	static CInterfaceI2C* GetInstance();
	static void DestroyInstance();

	bool Init(bool bStartup);
	bool Deinit();
	bool ReinitI2C();
	bool SetI2Cerror(BYTE iAdresse,bool bStartup=false);
	void ResetI2Cerror();
	void SetI2CErrorCode(int bit);
	void DeleteI2CErrorCode(int bit);
	void DeleteI2CErrorCode();
	int GetI2CErrorCode();

	void SetI2C_FRAMavailability(BOOL state);
	BOOL IsI2C_FRAMavailability();

	void SetI2C_ACCUavailability(BOOL state);
	BOOL IsI2C_ACCUavailability();

	void SetI2C_MCPavailability(BOOL state);
	BOOL IsI2C_MCPavailability();

	bool ScanForDevice(BYTE bDeviceAddress,bool bStartup);
	void scanForDeviceMCP(bool bStartup);

	bool InitMCP_ControlerPIC();
	//bool InitMCP_MonitorPIC(BYTE iVersionMainboard);
	bool InitMCP_HfoPIC();
	

	//bool InitMCP_HfoPIC();
	void SetMCPstate(eMCPstate state);
	eMCPstate GetMCPstate();
	void SetMCPwatchdog();
	BYTE GetByteMCPwatchdog();

	void SetFRAMstate(bool state);
	void SetACCUstate(bool state);
	bool GetACCUstate();
	//void SetTRENDstate(bool state);

	bool Erase64FRAM();
	bool SetFRAMDefaultFactoryConfig();

	bool SetFRAMDefaultFactoryValues();

	/*bool SaveHospitalSettings();
	bool LoadHospitalSettings();
	bool WriteHospitalSettingsToUSB();*/

	//WORD ReadWord(BYTE bDeviceAddress);
	//BYTE ReadByte(BYTE bDeviceAddress);

	/*WORD ReadWordMCP();
	BYTE ReadByteMCP();*/

	//WORD ReadWordFRAM(BYTE chReg1, BYTE chReg2);
	//BYTE ReadByteFRAM();
	//BYTE ReadByteFRAM(WORD wReg);
	//bool WriteByteFRAM(WORD wReg, BYTE bData);
	
	/*void SetNextRegAddressTrend(UINT iSize, bool bSaveAddress);
	void SetNextSlaveAddressTrend(bool bSaveAddress);
	void SaveCurrentAddressTrend();*/

	//bool ReadTrendDateForward(PBUFTREND pBuffer, bool bSaveAddress=true);
	//bool ReadTrendDate(PBUFTREND* pBuffer, bool bSaveAddress=true);
	//bool WriteTrendDate(PBUFTREND pBuffer/*BYTE byPINSP,int iPMEAN,BYTE byFIO2,WORD wVTE,WORD wCOMPL,WORD wCO2*/, bool bSaveAddress=true);
	//bool FillTrendDate(UINT size);
	//bool FillByteTrendDate(NI2CHANDLE hOpenContext, 
	//							BYTE chDevAddr, 
	//							WORD wReg,
	//							/*BYTE* pBuffer,*/
	//							UINT iBufferSize);
	//bool Set9ByteTrendDate(	NI2CHANDLE hOpenContext, 
	//							BYTE chDevAddr, 
	//							WORD wReg,
	//							BYTE* pBuffer,
	//							UINT iBufferSize);
	//bool Get9ByteTrendDate(NI2CHANDLE hOpenContext, 
	//							BYTE chDevAddr, 
	//							WORD wReg,
	//							BYTE* pBuffer,
	//							UINT iBufferSize);

	//FRAM
	BYTE ReadConfigByte(WORD wReg);
	bool WriteConfigByte(WORD wReg, BYTE bData);
	SHORT ReadConfigWord(WORD wReg);
	bool WriteConfigWord(WORD wReg, SHORT bData);

	//AKKU
	//DWORD ReadAccuBytes(WORD wReg,BYTE *pBuffer,WORD bufSize);
	SHORT ReadAccuWord(WORD wReg);
	bool WriteAccuWord(WORD wReg, SHORT bData);
	

	WORD ReadMCP(void);
	BYTE ReadMCP_Port0(void);
	BYTE ReadMCP_Port1(void);
	/*bool WriteMCP_Port0(BYTE bData);
	bool WriteMCP_Port1(BYTE bData);*/

	bool WriteNextTrendDate(BYTE byPINSP,BYTE byPMEAN,BYTE byFIO2,WORD wVTE,WORD wCOMPL,WORD wCO2);
	//bool ReadTrendDate();
	

	CStringW GetDriverInfo();

private:
	//singleton
	static CInterfaceI2C* theI2CInterface;
	static CRITICAL_SECTION m_csI2C;

protected:
	CInterfaceI2C(void);
	virtual ~CInterfaceI2C(void);

	void StartI2CThread(void);
	void StopI2CThread( void );

	int SetBit(int x, unsigned int n);
	int DeleteBit(int x, unsigned int n);

	/*bool InitMCP_MonitorPIC_MB3();
	bool InitMCP_MonitorPIC_MB4();*/

	bool InitMCP_HfoPIC_MB3();
	bool InitMCP_HfoPIC_MB4();

	bool InitMCP_ControlerPIC_MB3();
	bool InitMCP_ControlerPIC_MB4();

	bool SetRegister8(BYTE chDevAddr, BYTE chReg, BYTE chValue);
	BYTE GetRegister8(BYTE chDevAddr, BYTE chReg);
	WORD GetRegister16(BYTE chDevAddr, BYTE chReg1, BYTE chReg2);

	bool SetConfigRegister8(BYTE chDevAddr, WORD wReg, BYTE chValue);
	BYTE GetConfigRegister8(BYTE chDevAddr, WORD wReg);
	bool SetConfigRegister16(BYTE chDevAddr, WORD wReg, SHORT wValue);
	SHORT GetConfigRegister16(BYTE chDevAddr, WORD wReg, bool bIgnoreAlarm=false);
	//DWORD GetConfigRegisterBytes(NI2CHANDLE hOpenContext, BYTE chDevAddr, WORD wReg, BYTE *pBuffer,WORD bufSize);

	//SHORT GetConfigRegisterAccu24(NI2CHANDLE hOpenContext, BYTE chDevAddr, WORD wReg);

	bool SetByteFRAMRegister(BYTE chDevAddr, WORD wReg, BYTE chValue);
	BYTE GetByteFRAMRegister(BYTE chDevAddr, WORD wReg);
	//BYTE GetByteFRAMCurRegister(NI2CHANDLE hOpenContext, BYTE chDevAddr);
	//WORD GetWordFRAMRegister(NI2CHANDLE hOpenContext, BYTE chDevAddr, WORD wReg);
	//WORD GetFRAMRegister(NI2CHANDLE hOpenContext, BYTE chDevAddr, WORD wReg, BYTE *pBuffer, UINT bufSizeToRead);

	
	//void LogError(CStringW sError);

	//bool WriteI2C(BYTE* pData, WORD wSize, WORD wOffset);
	//bool ReadI2C(BYTE* pData, WORD wSize, WORD wOffset);

	//bool ResizeBuffer(int iLen);

	static friend UINT CI2CThread(LPVOID pc);
	CWinThread*	m_pcwtI2CThread;
	DWORD I2CData(void);
	bool m_bDoI2CThread;
	HANDLE m_hI2CThread;
	
	bool m_bReadI2C;
	bool m_bReadByte;
	bool m_bWriteByte;
	BYTE m_chDevAddr;
	WORD m_wRegister;
	BYTE m_chRegister1;
	BYTE m_chRegister2;
	BYTE m_chValue;
	SHORT m_wValue;

	BYTE m_chReturn1;
	BYTE m_chReturn2;

	bool m_bI2CerrorPending;

	/*GetByteFRAMRegister(BYTE chDevAddr, WORD wReg);
	GetConfigRegister16(BYTE chDevAddr, WORD wReg); ACCU, CONFIG
	GetConfigRegister8(BYTE chDevAddr, WORD wReg);


	GetRegister8(BYTE chDevAddr, BYTE chReg); MCP
	GetRegister16(BYTE chDevAddr, BYTE chReg1, BYTE chReg2); MCP*/

protected:

	HANDLE m_hI2C;

	DWORD m_dwLastI2CError;
	
	BYTE m_byGP1_ACCESS;

	bool m_bWatchdog;
	//bool m_bMCP;
	eMCPstate m_eMCPstate;
	bool m_bFRAM;
	bool m_bACCU;

	bool m_bLog;

	WORD m_wUseForUDP;

	BYTE m_byCurDevAddr; 
	WORD m_wCurReg;

	//BYTE m_iMainBoard;

	bool m_bI2CError;
	bool m_bAlarmSet;

	BOOL m_bI2C_FRAMavailability;
	BOOL m_bI2C_ACCUavailability;
	BOOL m_bI2C_MCPavailability;
	UINT m_iI2CErrorCodeBits;
};
