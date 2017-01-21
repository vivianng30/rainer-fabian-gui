#pragma once
#include "interface.h"
#include "globDefs.h"
#include "serial.h"
#include "serialEx.h"
//#include <fstream>

//typedef struct tagTERMMSG
//{
//	BYTE bySize;
//	BYTE byBuffer[32];
//}TERMMSG, *LPTERMMSG;

//terminal receive states
#define TERM_MSG_SOM							0x02


#define TERM_GET_MEASUREMENTS_ONCE_BTB			0x00
#define TERM_GET_MEASUREMENTS_CONTINIOUS_BTB	0x01
#define TERM_GET_MEASUREMENTS_ONCE_AVG			0x02
#define TERM_GET_MEASUREMENTS_CONTINIOUS_AVG	0x03
#define TERM_GET_WAVE_DATA						0x04
#define TERM_GET_VENT_MODE						0x05//'O'
#define TERM_GET_MODE_OPTION					0x06//'R'
#define TERM_GET_MODE_OPTION2					0x07//')'
//#define TERM_GET_HW_CONFIG						0x08//'K'

#define TERM_GET_PARAM_IFlow					0x0A
#define TERM_GET_PARAM_EFlow					0x0B
#define TERM_GET_PARAM_TherapieFlow				0x0C
#define TERM_GET_PARAM_Risetime					0x0D
#define TERM_GET_PARAM_PEEP						0x0E
#define TERM_GET_PARAM_PINSP					0x0F
#define TERM_GET_PARAM_ITime					0x10
#define TERM_GET_PARAM_ETime					0x11
#define TERM_GET_PARAM_BPM						0x12
#define TERM_GET_PARAM_O2						0x13
#define TERM_GET_PARAM_VLimit					0x14
#define TERM_GET_PARAM_VGarant					0x15
#define TERM_GET_PARAM_PPSV						0x16
#define TERM_GET_PARAM_Trigger					0x17
#define TERM_GET_PARAM_AbortCriterionPSV		0x18
#define TERM_GET_PARAM_Flowmin					0x19
#define TERM_GET_PARAM_CPAP						0x1A
#define TERM_GET_PARAM_PManual					0x1B
#define TERM_GET_PARAM_Backup					0x1C
#define TERM_GET_PARAM_CPAPnmode				0x1D
#define TERM_GET_PARAM_PManualnmode				0x1E
#define TERM_GET_PARAM_ITimeNMODE				0x1F
#define TERM_GET_PARAM_ETimeNMODE				0x20
#define TERM_GET_PARAM_BPMNMODE					0x21
#define TERM_GET_PARAM_HFAmpl					0x22
#define TERM_GET_PARAM_HFFreq					0x23
#define TERM_GET_PARAM_HFPMean					0x24
#define TERM_GET_PARAM_ITimeRec					0x25
#define TERM_GET_PARAM_ETIMERec					0x26
#define TERM_GET_PARAM_FreqRec					0x27
#define TERM_GET_PARAM_HFPMeanRec				0x28
#define TERM_GET_PARAM_HFFlow					0x29
#define TERM_GET_PARAM_HFVGarant				0x2A



#define TERM_STOP_CONTINIOUS_MEASUREMENTS		0x30
#define TERM_STOP_WAVE_DATA						0x31
//#define TERM_SET_VENT_MODE						0x32
//#define TERM_SET_MODE_OPTION1					0x33
//#define TERM_SET_MODE_OPTION2					0x34

#define TERM_SET_PARAM_IFlow					0x40
#define TERM_SET_PARAM_EFlow					0x41
#define TERM_SET_PARAM_TherapieFlow				0x42
#define TERM_SET_PARAM_RiseTime					0x43
#define TERM_SET_PARAM_PEEP						0x44
#define TERM_SET_PARAM_PINSP					0x45
#define TERM_SET_PARAM_ITime					0x46
#define TERM_SET_PARAM_ETime					0x47
#define TERM_SET_PARAM_BPM						0x48
//#define TERM_SET_PARAM_VLimit					0x4A
//#define TERM_SET_PARAM_VGarant					0x4B
#define TERM_SET_PARAM_PPSV						0x4C
#define TERM_SET_PARAM_Trigger					0x4D
#define TERM_SET_PARAM_AbortCriterionPSV		0x4E
#define TERM_SET_PARAM_Flowmin					0x4F
#define TERM_SET_PARAM_CPAP						0x50
#define TERM_SET_PARAM_PManual					0x51
#define TERM_SET_PARAM_Backup					0x52
#define TERM_SET_PARAM_CPAPnmode				0x53
#define TERM_SET_PARAM_PManualnmode				0x54
#define TERM_SET_PARAM_ITimeNMODE				0x55
#define TERM_SET_PARAM_ETimeNMODE				0x56
#define TERM_SET_PARAM_BPMNMODE					0x57
#define TERM_SET_PARAM_HFAmpl					0x58
#define TERM_SET_PARAM_HFFreq					0x59
#define TERM_SET_PARAM_HFPMean					0x5A
#define TERM_SET_PARAM_ITimeRec					0x5B
#define TERM_SET_PARAM_ETIMERec					0x5C
#define TERM_SET_PARAM_FreqRec					0x5D
#define TERM_SET_PARAM_HFPMeanRec				0x5E
#define TERM_SET_PARAM_HFFlow					0x5F
#define TERM_SET_PARAM_HFVGarant				0x60
#define TERM_SET_PARAM_O2						0x61
#define TERM_SET_PARAM_O2_FLUSH					0x62

//from host
#define TERM_MEASUREMENTS_BTB				0x00
#define TERM_MEASUREMENTS_AVG				0x02
#define TERM_WAVE_DATA						0x04
#define TERM_VENT_MODE						0x05//'O'
#define TERM_MODE_OPTION					0x06//'R'
#define TERM_MODE_OPTION2					0x07//')'
#define TERM_HW_CONFIG						0x08//'K'

#define TERM_PARAM_IFlow					0x0A
#define TERM_PARAM_EFlow					0x0B
#define TERM_PARAM_TherapieFlow				0x0C
#define TERM_PARAM_Risetime					0x0D
#define TERM_PARAM_PEEP						0x0E
#define TERM_PARAM_PINSP					0x0F
#define TERM_PARAM_ITime					0x10
#define TERM_PARAM_ETime					0x11
#define TERM_PARAM_BPM						0x12
#define TERM_PARAM_O2						0x13
#define TERM_PARAM_VLimit					0x14
#define TERM_PARAM_VGarant					0x15
#define TERM_PARAM_PPSV						0x16
#define TERM_PARAM_Trigger					0x17
#define TERM_PARAM_AbortCriterionPSV		0x18
#define TERM_PARAM_Flowmin					0x19
#define TERM_PARAM_CPAP						0x1A
#define TERM_PARAM_PManual					0x1B
#define TERM_PARAM_Backup					0x1C
#define TERM_PARAM_CPAPnmode				0x1D
#define TERM_PARAM_PManualnmode				0x1E
#define TERM_PARAM_ITimeNMODE				0x1F
#define TERM_PARAM_ETimeNMODE				0x20
#define TERM_PARAM_BPMNMODE					0x21
#define TERM_PARAM_HFAmpl					0x22
#define TERM_PARAM_HFFreq					0x23
#define TERM_PARAM_HFPMean					0x24
#define TERM_PARAM_ITimeRec					0x25
#define TERM_PARAM_ETIMERec					0x26
#define TERM_PARAM_FreqRec					0x27
#define TERM_PARAM_HFPMeanRec				0x28
#define TERM_PARAM_HFFlow					0x29
#define TERM_PARAM_HFVGarant				0x2A

#define TERM_PARAM_NOSUPPORT				0xFD
#define TERM_PARAM_ACKN						0xFE
#define TERM_PARAM_OUTOFFRANGE				0xFF

typedef enum
{
	TERM_STATE_SOM,
	TERM_STATE_DATA_SIZE,
	TERM_STATE_CODE,
	TERM_STATE_DATA,
	//TERM_STATE_DATA1,
	//TERM_STATE_DATA2,
	TERM_STATE_CHKSUM,
	TERM_STATE_EOM,
} ReceiverStatesTerminal;

//typedef enum
//{
//	TERM_NONE,
//	//TERM_DELLACA_AMPLITUDE,xxxxxxxxxxx//a
//	//TERM_DELLACA_FREQUENCY,xxxxxxxxxxx//f
//	TERM_S_MODE_OPTION2,	//'('
//	TERM_M_MODE_OPTION2,	//')'
//	TERM_S_PLATEAU,	//'['
//	TERM_M_PLATEAU,	//']'
//	TERM_S_HF_PMEAN_RECRUIT,	//'!'
//	TERM_M_HF_PMEAN_RECRUIT,	//'?'
//	/*TERM_SET_BLENDER_BIT,	//'<'
//	TERM_CLEAR_BLENDER_BIT,	//'='
//	TERM_SET_FG_BIT,	// '>'*/
//	TERM_S_EXP_FLOW,	//'a'
//	TERM_M_EXP_FLOW,	//'A'
//	TERM_M_DEMAND_FLOW,	//'b'
//	TERM_M_CAL_PRESS_SCALE,	//'B'
//	TERM_S_P_PSV,	//'c'
//	TERM_M_P_PSV,	//'C'
//	TERM_S_OXY_RATIO,	//'d'
//	TERM_M_OXY_RATIO,	//'D'
//	TERM_S_EXP_TIME,	//'e'
//	TERM_M_EXP_TIME,	//'E'
//	TERM_S_HF_FREQ,	//'f'
//	TERM_M_HF_FREQ,	//'F'
//	//TERM_S_CAL_PRESS_PROX,	//'g'
//	TERM_M_CAL_PRESS_OFFSET,	//'G'
//	//TERM_M_BLENDER_VERSION,	//'H'
//	//TERM_S_FG_OXY_CORR,	//'i'
//	TERM_M_BLENDER_STATUS,	//'I'
//	TERM_S_HF_MEAN_AIRWAY_PRESS,	//'j'
//	TERM_M_HF_MEAN_AIRWAY_PRESS,	//'J'
//	TERM_S_HW_CONFIG,	//'k'
//	TERM_M_HW_CONFIG,	//'K'
//	TERM_S_DELLACA_AMP_FREQ,	//'l'
//	TERM_M_STATUS,	//'L'
//	TERM_S_HF_AMPLITUDE,	//'m'
//	TERM_M_HF_AMPLITUDE,	//'M'
//	TERM_S_MAN_BREATH,	//'n'
//	//TERM_M_CHECKSUM,	//'N'
//	TERM_S_VENT_MODE,	//'o'
//	TERM_M_VENT_MODE,	//'O'
//	TERM_S_MAN_BREATH_STOP,	//'p'
//	TERM_EOC, //end of command
//	TERM_S_MODE_OPTION,	//'r'
//	TERM_M_MODE_OPTION,	//'R'
//	//TERM_C_VOL_CORR,	//'s'
//	//TERM_S_AUTOFLOW_CORR,	//'S'
//	TERM_S_P_INSP,	//'t'
//	TERM_M_P_INSP,	//'T'
//	TERM_S_INSP_FLOW,	//'u'
//	TERM_M_INSP_FLOW,	//'U'
//	TERM_S_APNEA_TIME,	//'v'
//	TERM_M_APNEA_TIME,	//'V'
//	TERM_S_INSP_TIME,	//'w'
//	TERM_M_INSP_TIME,	//'W'
//	TERM_M_EXP_FLOW_NMODE,	//'x'
//	TERM_M_INSP_FLOW_NMODE,//'X'
//	//TERM_M_VERSION,	//'Y'
//	TERM_S_PEEP,	//'z'
//	TERM_M_PEEP	//'Z'
//} ReceiverStatesTerminal;

typedef struct tagTERMMSG
{
	CStringW szMessage;
}TERMMSG, *LPTERMMSG;

class CInterfaceTerminal : public CInterface, public CSerialEx
{
	//typedef struct tagMESSAGE
	//{
	//	//bool bWaitForAckn;
	//	CStringW szMessage;
	//}MESSAGE;

public:
	

	static CInterfaceTerminal* GetInstance();
	static void DestroyInstance();

	bool Init();
	bool Deinit();
	bool OpenCOM();

	bool PerformMsg();
	void OnEvent (EEvent eEvent, EError eError);
	void send(LPTERMINALMSG msg);

	void setWaveDataActive();
	bool isWaveDataActive();
	bool isMeasurementDataActive();

	void sendMessurementData();

	void sendData_Cmd(BYTE command);
	void sendData_BYTE(BYTE command, BYTE val) ;
	void sendData_SHORT(BYTE command, SHORT val);

	//DWORD sendTerminalCommand(CStringW data);

	void sendWaveData(SHORT  iValPressure,SHORT  iValFlow, SHORT  iCO2) ;//FOT-TERM1
	void sendWaveData(SHORT  iValPressure,SHORT  iValFlow,SHORT  iValVolume,SHORT  iValCO2);
	//void SendToTerminal(SHORT  iValPressure,SHORT  iValFlow);
	/*void SendBuffer(	BYTE CO2_command=0, 
						BYTE Anzahlbytes=0, 
						BYTE byte0=0, 
						BYTE byte1=0, 
						BYTE byte2=0, 
						BYTE byte3=0, 
						BYTE byte4=0);*/
	//void SendBuffer(BYTE command, BYTE sizeBytes);
	void sendBuffer(LPTERMINALMSG msg);
protected:
	CInterfaceTerminal(void);
	virtual ~CInterfaceTerminal(void);

	

	void StartTerminalThread(void);
	void StopTerminalThread( void );

	void sendModeOption();
	void sendModeOption2();

	//void AddChecksum(short num, BYTE buf[]);

private:
	//Singelton
	static CInterfaceTerminal* theTerminalInterface;

protected:
	
	/*friend UINT CTerminalThread(LPVOID pc);
	CWinThread*	m_pcwtTerminalThread;
	DWORD TerminalData(void);
	bool m_bDoTerminalThread;
	HANDLE m_hThreadTerminal;*/

	/*friend UINT CTerminalReceiveThread(LPVOID pc);
	CWinThread*	m_pcwtTerminalReceiveThread;
	DWORD ReceiveTerminalData(void);
	bool m_bDoTerminalReceiveThread;
	HANDLE m_hThreadTerminalReceive;*/


	friend UINT CTerminalSendThread(LPVOID pc);
	CWinThread*	m_pcwtTerminalSendThread;
	DWORD SendTerminalData(void);
	bool m_bDoTerminalSendThread;
	HANDLE m_hThreadTerminalSend;

protected:	

	CTlsComPort m_comPort;

	static CList<LPTERMINALMSG, LPTERMINALMSG>  MSGSend;
	//static CList<LPTERMMSG, LPTERMMSG>  MSGSendBuf;
	//static CList<CStringW, CStringW&>  MSGSend;
	CRITICAL_SECTION	  csMSGSend;
	CRITICAL_SECTION	  csEnableSend;

	//SHORT m_byBuffer[32];
	//BYTE m_byBuffer[32];
	UINT m_iCntBuf;

	
	BYTE	fwReadChkSum;
	BYTE	fwCalcChkSum;
	DWORD	fdwRecvBufLen;
	BYTE	faRecvBuf[256];
	BYTE	faDataBuf[256];
	WORD	fwRecvDataCnt;
	WORD	fwRecvDataBufCnt;
	BYTE	fwRecvDataSize;
	BYTE	fwRecvDataCode;

	BYTE	faSendDataBuf[256];
	BYTE	fwSendDataSize;

	bool m_bStarted;
	bool m_bSendWaveData;

	ReceiverStatesTerminal	feRecvState;


	BYTE m_byDellaceAmp;
	BYTE m_byDellaceFreq;

	bool m_bMEASUREMENTS_BTB;
	bool m_bMEASUREMENTS_AVG;

	bool m_bSendContiniousMeasurements;
	

};
