//=============================================================================
/** 
* \class CInterfaceTerminalRemote
* 		 
* \brief Interface for remote terminal interface.
* 		 
* \author Rainer Kuehner
* \date 2017-10-23 Creation of file
*
**/
//=============================================================================
#pragma once
#include "InterfaceTerminal.h"

//terminal receive states
#define TERM_MSG_SOM							0x02

//from host
#define TERMINAL_MEASUREMENTS_BTB				0x00
#define TERMINAL_MEASUREMENTS_AVG				0x02
#define TERMINAL_WAVE_DATA						0x04
#define TERMINAL_VENT_MODE						0x05//'O'
#define TERMINAL_MODE_OPTION1					0x06//'R'
#define TERMINAL_MODE_OPTION2					0x07//')'
#define TERMINAL_HW_CONFIG						0x08//'K'
#define TERMINAL_VENT_RUNSTATE					0x09
#define TERMINAL_STATE_VLimit					0x0A
#define TERMINAL_STATE_VGarant					0x0B
#define TERMINAL_PARAM_VentRange				0x0C
#define TERMINAL_PARAM_IERatioHFO				0x0D
#define TERMINAL_MANBREATHrunning				0x0E
#define TERMINAL_PressureRiseCtrl				0x0F
#define TERMINAL_PARAM_HFOFreqRec				0x10
#define TERMINAL_PARAM_HFOFlow					0x11
#define TERMINAL_LeakCompensation				0x12
#define TERMINAL_TriggerOption					0x13
#define TERMINAL_FOToscillationState			0x14
#define TERMINAL_PARAM_PINSP					0x15
#define TERMINAL_PARAM_PEEP						0x16
#define TERMINAL_PARAM_PPSV						0x17
#define TERMINAL_PARAM_BPM						0x18
#define TERMINAL_PARAM_HFAmpl					0x19
#define TERMINAL_PARAM_HFAmplMax				0x1A
#define TERMINAL_PARAM_HFFreq					0x1B
#define TERMINAL_PARAM_O2						0x1C
#define TERMINAL_PARAM_IFlow					0x1D
#define TERMINAL_PARAM_EFlow					0x1E
#define TERMINAL_PARAM_Risetime					0x1F
#define TERMINAL_PARAM_ITime					0x20
#define TERMINAL_PARAM_ETime					0x21
#define TERMINAL_PARAM_HFPMean					0x22
#define TERMINAL_PARAM_HFPMeanRec				0x23
#define TERMINAL_PARAM_VLimit					0x24
#define TERMINAL_PARAM_VGarant					0x25
#define TERMINAL_PARAM_AbortCriterionPSV		0x26
#define TERMINAL_PARAM_TherapieFlow				0x27
#define TERMINAL_PARAM_Trigger					0x28
#define TERMINAL_PARAM_Flowmin					0x29
#define TERMINAL_PARAM_CPAP						0x2A
#define TERMINAL_PARAM_PManual					0x2B
#define TERMINAL_PARAM_Backup					0x2C
#define TERMINAL_PARAM_ITimeRec					0x2D
#define TERMINAL_PARAM_ETIMERec					0x2E
#define TERMINAL_PARAM_SPO2LOW					0x2F
#define TERMINAL_PARAM_SPO2HIGH					0x30
#define TERMINAL_PARAM_FIO2LOW					0x31
#define TERMINAL_PARAM_FIO2HIGH					0x32
#define TERMINAL_STATE_PRICO					0x33



#define TERMINAL_GET_MEASUREMENTS_ONCE_BTB			0x00
#define TERMINAL_GET_MEASUREMENTS_CONTINIOUS_BTB	0x01
#define TERMINAL_GET_MEASUREMENTS_ONCE_AVG			0x02
#define TERMINAL_GET_MEASUREMENTS_CONTINIOUS_AVG	0x03
#define TERMINAL_GET_WAVE_DATA						0x04
#define TERMINAL_GET_VENT_MODE						0x05//'O'
#define TERMINAL_GET_MODE_OPTION1					0x06//'R'
#define TERMINAL_GET_MODE_OPTION2					0x07//')'
//#define TERM_GET_HW_CONFIG						0x08//'K'
#define TERMINAL_GET_VENT_RUNSTATE					0x09
#define TERMINAL_GET_STATE_VLimit					0x0A
#define TERMINAL_GET_STATE_VGarant					0x0B
#define TERMINAL_GET_PARAM_VentRange				0x0C
#define TERMINAL_GET_PARAM_IERatioHFO				0x0D
#define TERMINAL_GET_MANBREATHrunning				0x0E
#define TERMINAL_GET_PressureRiseCtrl				0x0F
#define TERMINAL_GET_PARAM_HFOFreqRec				0x10
#define TERMINAL_GET_PARAM_HFOFlow					0x11
#define TERMINAL_GET_LeakCompensation				0x12
#define TERMINAL_GET_TriggerOption					0x13
#define TERMINAL_GET_FOToscillationState			0x14
#define TERMINAL_GET_PARAM_PINSP					0x15
#define TERMINAL_GET_PARAM_PEEP						0x16
#define TERMINAL_GET_PARAM_PPSV						0x17
#define TERMINAL_GET_PARAM_BPM						0x18
#define TERMINAL_GET_PARAM_HFAmpl					0x19
#define TERMINAL_GET_PARAM_HFAmplMax				0x1A
#define TERMINAL_GET_PARAM_HFFreq					0x1B
#define TERMINAL_GET_PARAM_O2						0x1C
#define TERMINAL_GET_PARAM_IFlow					0x1D
#define TERMINAL_GET_PARAM_EFlow					0x1E
#define TERMINAL_GET_PARAM_Risetime					0x1F
#define TERMINAL_GET_PARAM_ITime					0x20
#define TERMINAL_GET_PARAM_ETime					0x21
#define TERMINAL_GET_PARAM_HFPMean					0x22
#define TERMINAL_GET_PARAM_HFPMeanRec				0x23
#define TERMINAL_GET_PARAM_VLimit					0x24
#define TERMINAL_GET_PARAM_VGarant					0x25
#define TERMINAL_GET_PARAM_AbortCriterionPSV		0x26
#define TERMINAL_GET_PARAM_TherapieFlow				0x27
#define TERMINAL_GET_PARAM_Trigger					0x28
#define TERMINAL_GET_PARAM_Flowmin					0x29
#define TERMINAL_GET_PARAM_CPAP						0x2A
#define TERMINAL_GET_PARAM_PManual					0x2B
#define TERMINAL_GET_PARAM_Backup					0x2C
#define TERMINAL_GET_PARAM_ITimeRec					0x2D
#define TERMINAL_GET_PARAM_ETIMERec					0x2E
#define TERMINAL_GET_PARAM_SPO2LOW					0x2F
#define TERMINAL_GET_PARAM_SPO2HIGH					0x30
#define TERMINAL_GET_PARAM_FIO2LOW					0x31
#define TERMINAL_GET_PARAM_FIO2HIGH					0x32
#define TERMINAL_GET_STATE_PRICO					0x33




#define TERMINAL_STOP_CONTINIOUS_MEASUREMENTS		0x50
#define TERMINAL_STOP_WAVE_DATA						0x51
#define TERMINAL_SET_VENT_MODE						0x52
//#define TERM_SET_MODE_OPTION1					0x53
//#define TERM_SET_MODE_OPTION2					0x54
#define TERMINAL_SET_VENT_RUNSTATE				0x55
#define TERMINAL_SET_STATE_VLimit				0x56
#define TERMINAL_SET_STATE_VGarant				0x57
#define TERMINAL_SET_PARAM_VentRange			0x58
#define TERMINAL_SET_PARAM_IERatioHFO			0x59
#define TERMINAL_SET_MANBREATHrunning			0x5A
#define TERMINAL_SET_PressureRiseCtrl			0x5B
#define TERMINAL_SET_PARAM_HFOFreqRec			0x5C
#define TERMINAL_SET_PARAM_HFOFlow				0x5D
#define TERMINAL_SET_LeakCompensation			0x5E
//#define TERMINAL_SET_TriggerOption				0x5F
#define TERMINAL_SET_PARAM_PINSP				0x60
#define TERMINAL_SET_PARAM_PEEP					0x61
#define TERMINAL_SET_PARAM_PPSV					0x62
#define TERMINAL_SET_PARAM_BPM					0x63
#define TERMINAL_SET_PARAM_HFAmpl				0x64
#define TERMINAL_SET_PARAM_HFAmplMax			0x65
#define TERMINAL_SET_PARAM_HFFreq				0x66
#define TERMINAL_SET_PARAM_O2					0x67
#define TERMINAL_SET_PARAM_IFlow				0x68
#define TERMINAL_SET_PARAM_EFlow				0x6A
#define TERMINAL_SET_PARAM_RiseTime				0x6B
#define TERMINAL_SET_PARAM_ITime				0x6C
#define TERMINAL_SET_PARAM_ETime				0x6D
#define TERMINAL_SET_PARAM_HFPMean				0x6E
#define TERMINAL_SET_PARAM_HFPMeanRec			0x6F
#define TERMINAL_SET_PARAM_VLimit				0x70
#define TERMINAL_SET_PARAM_VGarant				0x71
#define TERMINAL_SET_PARAM_AbortCriterionPSV	0x72
#define TERMINAL_SET_PARAM_TherapieFlow			0x73
#define TERMINAL_SET_PARAM_Trigger				0x74
#define TERMINAL_SET_PARAM_Flowmin				0x75
#define TERMINAL_SET_PARAM_CPAP					0x76
#define TERMINAL_SET_PARAM_PManual				0x77
#define TERMINAL_SET_PARAM_Backup				0x78
#define TERMINAL_SET_PARAM_ITimeRec				0x79
#define TERMINAL_SET_PARAM_O2_FLUSH				0x7A
#define TERMINAL_SET_PARAM_SPO2LOW				0x7B
#define TERMINAL_SET_PARAM_SPO2HIGH				0x7C
#define TERMINAL_SET_PARAM_FIO2LOW				0x7D
#define TERMINAL_SET_PARAM_FIO2HIGH				0x7E
#define TERMINAL_SET_STATE_PRICO				0x7F


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


// CInterfaceTerminalRemote

class CInterfaceTerminalRemote : public CInterfaceTerminal
{
	DECLARE_DYNAMIC(CInterfaceTerminalRemote)

public:
	CInterfaceTerminalRemote();
	virtual ~CInterfaceTerminalRemote();

	//virtuals abstract
	bool Init();
	bool OpenCOM();
	void OnEvent (EEvent eEvent, EError eError);

	//virtuals
	void sendMessurementData();

protected:
	bool PerformMsg();
	

	void sendData_Cmd(BYTE command);
	void sendData_BYTE(BYTE command, BYTE val) ;
	void sendData_SHORT(BYTE command, SHORT val);

	void setParam_PINSP(WORD iVal);
	void setParam_PEEP(WORD iVal);
	void setParam_PPSV(WORD iVal);
	void setParam_BPM(WORD iVal);
	void setParam_HFAmpl(WORD iVal);
	void setParam_HFAmplMax(WORD iVal);
	void setParam_HFFreq(WORD iVal);
	void setParam_FiO2(BYTE iVal);
	void setParam_IFlow(WORD iVal);
	void setParam_EFlow(WORD iVal);
	void setParam_RiseTime(WORD iVal);
	void setParam_ITime(WORD iVal);
	void setParam_ETime(WORD iVal);
	void setParam_HFPMean(WORD iVal);
	void setParam_HFPMeanRec(WORD iVal);
	void setParam_VLimit(WORD iVal);
	void setParam_VGarant(WORD iVal);
	void setParam_AbortCriterionPSV(BYTE iVal);
	void setState_VLimit(BYTE iVal);
	void setState_VGarant(BYTE iVal);
	void setVent_RunState(BYTE iVal);
	void setParam_IERatioHFO(BYTE iVal);
	void set_MANBREATHrunning(BYTE iVal);
	void set_PressureRiseCtrl(BYTE iVal);
	void setParam_HFOFreqRec(WORD iVal);
	void setParam_HFOFlow(WORD iVal);
	void setLeakCompensation(BYTE iVal);
	void setParam_TherapieFlow(WORD iVal);
	void setParam_Trigger(BYTE iVal);
	void setParam_Flowmin(WORD iVal);
	void setParam_CPAP(WORD iVal);
	void setParam_PManual(WORD iVal);
	void setParam_Backup(BYTE iVal);
	void setParam_ITimeRec(WORD iVal);
	//void setParam_ETimeRec(WORD iVal);
	void setParam_O2_FLUSH(BYTE iVal);
	void setParam_SPO2LOW(BYTE iVal);
	void setParam_SPO2HIGH(BYTE iVal);
	void setParam_FIO2LOW(BYTE iVal);
	void setParam_FIO2HIGH(BYTE iVal);
	void setState_PRICO(BYTE iVal);

	void setVentMode(BYTE iVal);
	void setVentRange(BYTE state);

	void sendModeOption1();
	void sendModeOption2();

protected:
	ReceiverStatesTerminal	feRecvState;

	BYTE	fwReadChkSum;
	BYTE	fwCalcChkSum;
	DWORD	fdwRecvBufLen;
	BYTE	faRecvBuf[256];
	BYTE	faDataBuf[256];
	WORD	fwRecvDataCnt;
	WORD	fwRecvDataBufCnt;
	BYTE	fwRecvDataSize;
	BYTE	fwRecvDataCode;

	bool m_bMEASUREMENTS_BTB;
	bool m_bMEASUREMENTS_AVG;

	

	//DECLARE_MESSAGE_MAP()
};


