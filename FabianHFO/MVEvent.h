#pragma once

class CMVEvent
{
public:
	enum eET { ET_DATAEVENT, ET_ALARM, ET_UIEVENT, ET_MATRIX, ET_INFOTEXT, ET_CONTROL };

	CMVEvent(eET ET);
	virtual ~CMVEvent(void);

	eET GetET();

public:
	

private:
	eET m_ET;
};



//===========================================================================//
//
//\brief Klasse für ein Scan-Event
//
//===========================================================================//

//class CMVEventScan : public CMVEvent  
//{
//	CStringW m_sBarcode;
//	CStringW m_sSource;
//	long    m_lLabelType;
//
//public:
//	CMVEventScan(CStringW sBarcode, CStringW sSource, long lLabelType);
//	virtual ~CMVEventScan();
//
//	CStringW GetBarcode();
//	CStringW GetSource();
//	long    GetLabelType();
//
//};

//===========================================================================//
//
//\brief Klasse für ein Systemtasten-Event
//
//===========================================================================//
class CMVEventUI : public CMVEvent  
{
public:
	enum eEvents { 
		//EV_INITIALIZE,
		EV_RELOAD_CONFIG,
		/*EV_BN_MENU_1,    
		EV_BN_MENU_2,    
		EV_BN_MENU_3,     
		EV_BN_MENU_4,
		EV_BN_MENU_5,*/
		/*EV_BN_MENU_IPPV,
		EV_BN_MENU_SIPPV,
		EV_BN_MENU_SIMV,
		EV_BN_MENU_SIMVPSV,
		EV_BN_MENU_PSV,
		EV_BN_MENU_HFO,
		EV_BN_MENU_CPAP,
		EV_BN_MENU_NCPAP,
		EV_BN_MENU_DUOPAP,*/
		//EV_BN_MENU_VOLUME,
		EV_PATDATA_CHANGED,
		EV_BN_MENU_10,
		EV_BN_SETUP1,    
		EV_BN_SETUP2,     
		EV_BN_SETUP3,    
		EV_BN_SETUP4,
		EV_BN_SETUP5,
		EV_BN_SET_SERVICE_MODE,
		EV_BN_DEL_SERVICE_MODE,
		EV_BN_SERVICE_SYSTEM,
		EV_BN_SERVICE_DISPLAY,
		EV_BN_SERVICE_CALIBR,
		EV_BN_SERVICE_FLOW,
		EV_BN_SERVICE_ACCU,
		EV_BN_SERVICE_VALVES,
		EV_BN_SERVICE_MONITOR,
		EV_BN_SERVICE_IF,
		EV_BN_SERVICE_SETTINGS,
		EV_BN_SERVICE_NETWORK,
		EV_BN_SERVICE_LICENSING,
		EV_BN_SERVICE_MAINTENANCE,
		EV_BN_SERVICE_BACK,
		EV_BN_IPPV,
		EV_BN_SIPPV,
		EV_BN_SIMV,
		EV_BN_SIMVPSV,
		EV_BN_PSV,
		EV_BN_HFO,
		EV_BN_CPAP,
		EV_BN_NCPAP,
		EV_BN_DUOPAP,
		EV_BN_THERAPIE,
		EV_BN_GRAPH_WAVE,    
		EV_BN_GRAPH_LOOP,
		EV_BN_GRAPH_CO2,
		EV_BN_GRAPH_SPO2,
		EV_BN_GRAPH_FOT,
		//EV_BN_GRAPH_EMG,
		//EV_BN_GRAPH_CLFIO2,
		EV_BN_GRAPH_TREND,     
		EV_BN_GRAPH_FREEZED,
		EV_BN_GRAPH_CONTINUE,
		EV_BN_GRAPH_SAVE,
		EV_BN_TREND1,
		EV_BN_TREND2,
		EV_BN_TREND3,
		EV_BN_TREND4,
		EV_BN_TREND5,
		EV_BN_NEXT_GRAPH,
		EV_GRAPH_SETCURSOR,
		EV_GRAPH_DELCURSOR,
		EV_TREND_SETCURSOR,
		EV_TREND_DELCURSOR,
		//EV_BN_ALIMIT1,    
		//EV_BN_ALIMIT2,     
		//EV_BN_ALIMIT3,     
		EV_BN_ALIMIT4,
		EV_BN_ALIMIT5,
		//EV_BN_SYSALARM5,
		//EV_BN_SYSCONF1,
		//EV_BN_SYSCONF2,
		//EV_BN_SYSCONF3,
		//EV_BN_SYSCONF4,
		EV_BN_NUMERIC,	
		EV_BN_MENUBUTTON_OPEN,
		EV_BN_MENUBUTTON_CLOSE,
		EV_BN_MENU_MOVELEFT,
		EV_BN_MENU_MOVERIGHT,
		//EV_BN_SCREENLOCK_MAINVIEW,
		EV_BN_SCREENLOCK,
		EV_BN_SCREENFREE,
		//EV_BN_CAL_FLOWOFF,    
		//EV_BN_CAL_FLOWON,
		/*EV_BN_CAL_O2OFF,    
		EV_BN_CAL_O2ON,*/
		//EV_BN_CAL_FLOW,    
		//EV_BNDWN_CAL_FLOW,    
		//EV_BNUP_CAL_FLOW,
		/*EV_BN_CAL_O2,    
		EV_BNDWN_CAL_O2,    
		EV_BNUP_CAL_O2,*/
		//EV_BN_CAL_O2_21,   
		//EV_BNDWN_CAL_O2_21,   
		//EV_BNUP_CAL_O2_21,    
		//EV_BN_CAL_O2_100,   
		//EV_BNDWN_CAL_O2_100,    
		//EV_BNUP_CAL_O2_100,
		EV_ALIMIT_BN_BPM_ON,
		EV_ALIMIT_BN_BPM_OFF,
		EV_ALIMIT_BN_LEAK_ON,
		EV_ALIMIT_BN_LEAK_OFF,
		EV_ALIMIT_BN_APNOE_ON,
		EV_ALIMIT_BN_APNOE_OFF,
		EV_PARABN_IFLOW,
		EV_PARABN_EFLOW,
		EV_PARABN_PEEP,
		EV_PARABN_PINSP,
		EV_PARABN_PMAXVOLG,
		EV_PARABN_PMAN,
		EV_PARABN_PPSV,
		EV_PARABN_PDUO,
		EV_PARABN_VLIMIT,
		EV_PARABN_VLIMIT_ENABLED,
		EV_PARABN_VLIMIT_DISABLED,
		EV_PARABN_VLIMIT_AUTO_ON,
		EV_PARABN_VLIMIT_AUTO_OFF,
		/*EV_PINSP_INCREASE,
		EV_PINSP_DECREASE,*/
		EV_VOLUMEGARANT_ON,
		EV_VOLUMEGARANT_OFF,
		EV_VOLUMEGARANT_PRESET_ON,
		EV_VOLUMEGARANT_PRESET_OFF,
		EV_PARABN_VOLUMEGARANT_SELECTED_ON,
		EV_PARABN_VOLUMEGARANT_SELECTED_OFF,
		EV_PARABN_VOLUMEGARANT_PRESET_SELECTED_ON,
		EV_PARABN_VOLUMEGARANT_PRESET_SELECTED_OFF,
		EV_PARABN_VOLUMEGARANT_AUTO_ON,
		EV_PARABN_VOLUMEGARANT_AUTO_OFF,
		EV_PARABN_HFFREQ,
		EV_PARABN_HFFREQREC,
		EV_PARABN_HFAMPL,
		EV_PARABN_PMITT,
		EV_PARABN_IERATIO,
		EV_PARABN_TRIGGER,
		EV_PARABN_BPM,
		EV_PARABN_ITIME,
		EV_PARABN_ETIME,
		EV_PARABN_BACKUP,
		EV_PARABN_O2,
		EV_PARABN_MINFLOW,
		EV_PARABN_VGARANT,
		EV_PARABN_O2FLUSH,
		EV_PARABN_CPAP,
		EV_PARABN_FOT,
		EV_BN_SYSAL1,    
		EV_BN_SYSAL2,     
		EV_BN_SYSAL3,    
		EV_BN_SYSAL4,
		EV_LANGUAGE,
		
	};

private:
	int             m_EventType;
	bool			m_bState;
	/*CStringW         m_sData;*/
	int             m_iData;

public:
	CMVEventUI(int EventType);
	CMVEventUI(int EventType, bool bState);
	/*CMVEventUI(int EventType, CStringW sData);*/
	CMVEventUI(int EventType, int iData);

	virtual ~CMVEventUI();

	int             GetEventType();
	bool			GetState();
	/*CStringW         GetData();*/
	int             GetIData();
};



class CMVEventMatrix : public CMVEvent  
{
public:
	enum eEvents { 
		EV_BN_MATRIX_HOME,
		EV_BN_MATRIX_MENU,
		EV_BN_MATRIX_ALARMLIMITS,
		EV_BN_MATRIX_GRAPHS,
		EV_BN_MATRIX_TREND,
		EV_BN_MATRIX_CURSOR_LEFT,
		EV_BN_MATRIX_CURSOR_RIGHT,
		EV_BN_MATRIX_CURSOR_UP,
		EV_BN_MATRIX_CURSOR_DOWN,
		EV_BN_MATRIX_CURSOR_MOVEXUP,
		EV_BN_MATRIX_CURSOR_MOVEXDOWN,
		EV_BN_MATRIX_CURSOR_MOVEYLEFT,
		EV_BN_MATRIX_CURSOR_MOVEYRIGHT,
		EV_BN_MATRIX_SERVICE,
		//EV_BN_MATRIX_O2FLUSH,
		//EV_BN_MATRIX_MANUALBREATH_START,
		//EV_BN_MATRIX_MANUALBREATH_STOP,
		//EV_BN_MATRIX_START_STOP,
		EV_BN_MATRIX_ALARM_SILENT,
		EV_BN_MATRIX_SYSALARM_SILENT,
		EV_BN_MATRIX_STARTSTOP_DOWN,
		EV_BN_MATRIX_STARTSTOP_UP,
		EV_BN_MATRIX_ONOFF_DOWN,
		EV_BN_MATRIX_ONOFF_UP
	};

private:
	int             m_EventType;
	/*CStringW         m_sData;
	int             m_iData;*/

public:
	CMVEventMatrix(int EventType);
	/*CMVEventUI(int EventType, CStringW sData);
	CMVEventUI(int EventType, int iData);*/

	virtual ~CMVEventMatrix();

	int             GetEventType();
	/*CStringW         GetData();
	int             GetIData();*/
};



class CMVEventControl : public CMVEvent  
{
public:
	enum eEvents { 
		//EV_CONTROL_COM_M_CHECKSUM,
		//EV_CONTROL_COM_M_STATUS,
		EV_CONTROL_COM_ERROR,
		//EV_CONTROL_CHECKSUM_ERROR,
		EV_CONTROL_STARTUP_SUCCESS,
		EV_CONTROL_SETLASTVENTMODE,
		EV_CONTROL_FLOWSENSORSTATE,
		EV_CONTROL_O2SENSORSTATE,
		EV_CONTROL_O2CALSTART,
		EV_CONTROL_O2CALSTOP,
		/*EV_CONTROL_O2CAL100START,
		EV_CONTROL_O2CAL100STOP,*/
		EV_CONTROL_START_VENTILATION,
		EV_CONTROL_STOP_VENTILATION,
		EV_CONTROL_STANDBY,
		//EV_CONTROL_EXHALVALVCAL,
		EV_CONTROL_SET_MODE_EXHALVALVCAL,
		EV_CONTROL_DEL_MODE_EXHALVALVCAL,
		EV_CONTROL_START_FOT_CONVENTIONAL,
		//EV_CONTROL_STOP_FOT_CONVENTIONAL,
		EV_CONTROL_RESTORE_FOT_CONVENTIONAL,
		EV_CONTROL_POWERSTATE_CHANGED,
		EV_CONTROL_SYSTEMSTATE_CHANGED,
		EV_CONTROL_SYSFAIL,
		EV_CONTROL_START_MANBREATH,
		EV_CONTROL_STOP_MANBREATH,
		EV_CONTROL_START_NEBULIZER,
		EV_CONTROL_STOP_NEBULIZER,
		EV_CONTROL_FLOWSENSOR_CALIBRATED
	};

private:
	int             m_EventType;
	/*CStringW         m_sData;
	int             m_iData;*/

public:
	CMVEventControl(int EventType);
	/*CMVEventControl(int EventType, CStringW sData);
	CMVEventControl(int EventType, int iData);*/

	virtual ~CMVEventControl();

	int             GetEventType();
	/*CStringW         GetData();
	int             GetIData();*/
};



class CMVEventAlarm : public CMVEvent  
{
public:
	enum eEvents { 
		EV_ALARMSTATE_CHANGED,
		//EV_ALARM_ACTIVATED,
		EV_ALARM_REACTIVATED,
		//EV_FLOWSENSORSTATE_CHANGED,
		//EV_ALARM_PATIENT_SILENT,
		//EV_ALARM_TUBUSBLOCKED,
		//EV_ALARM_CANCELED,
		//EV_ALARM_SYSALARM,
		/*EV_ALARM_TEST1,
		EV_ALARM_P_IN_O2,
		EV_ALARM_P_IN_AIR,
		EV_ALARM_P_EXSPIRATION,
		EV_ALARM_P_INSPIRATION,
		EV_ALARM_O2_SENSOR_DEFECT,
		EV_ALARM_O2_SENSOR_USED,
		EV_ALARM_O2_VALUE_INCORRECT,
		EV_ALARM_RELAIS_DEFECT,
		EV_ALARM_P_PROXIMAL,
		EV_ALARM_P_IN_MIXER,
		EV_ALARM_VOLTAGE,
		EV_ALARM_ERROR_1,
		EV_ALARM_ERROR_2,
		EV_SYSALARM_CLOSED,*/
		//patientenalarme
		/*EV_ALARM_DISCONNECTION,
		EV_ALARM_MVMAX,
		EV_ALARM_MVMIN,
		EV_ALARM_PMAX,
		EV_ALARM_PMIN,
		EV_ALARM_BPM,
		EV_ALARM_LEAK,
		EV_ALARM_APNOE,
		EV_ALARM_MVMAX_CANCELED,
		EV_ALARM_MVMIN_CANCELED,
		EV_ALARM_PMAX_CANCELED,
		EV_ALARM_PMIN_CANCELED,
		EV_ALARM_BPM_CANCELED,
		EV_ALARM_LEAK_CANCELED,
		EV_ALARM_APNOE_CANCELED,
		EV_PATALARM_CLOSED*/
	};

private:
	int             m_EventType;
	CStringW         m_sData;
	/*int             m_iData;*/

public:
	CMVEventAlarm(int EventType);
	CMVEventAlarm(int EventType, CStringW sData);
	/*CMVEventUI(int EventType, int iData);*/

	virtual ~CMVEventAlarm();

	int             GetEventType();
	CStringW         GetData();
	/*int             GetIData();*/
};



//class CMVEventAlarmPatient : public CMVEvent  
//{
//public:
//	enum eEvents { 
//		//EV_ALARM_PATIENT,
//		EV_ALARM_DISCONNECTION,
//		EV_ALARM_MVMAX,
//		EV_ALARM_MVMIN,
//		EV_ALARM_PMAX,
//		EV_ALARM_PMIN,
//		EV_ALARM_BPM,
//		EV_ALARM_LEAK,
//		EV_ALARM_APNOE,
//		EV_ALARM_MVMAX_CANCELED,
//		EV_ALARM_MVMIN_CANCELED,
//		EV_ALARM_PMAX_CANCELED,
//		EV_ALARM_PMIN_CANCELED,
//		EV_ALARM_BPM_CANCELED,
//		EV_ALARM_LEAK_CANCELED,
//		EV_ALARM_APNOE_CANCELED,
//		EV_ALARM_CLOSED
//	};
//
//private:
//	int             m_EventType;
//	CStringW         m_sData;
//	/*int             m_iData;*/
//
//public:
//	CMVEventAlarmPatient(int EventType);
//	CMVEventAlarmPatient(int EventType, CStringW sData);
//	/*CMVEventUI(int EventType, int iData);*/
//
//	virtual ~CMVEventAlarmPatient();
//
//	int             GetEventType();
//	CStringW         GetData();
//	/*int             GetIData();*/
//};


class CMVEventInfotext : public CMVEvent  
{
public:
	enum eEvents { 
		EV_TEXT,
		EV_TIMETEXT,
		EV_CLEAR_TEXT
	};

private:
	int             m_EventType;
	CStringW         m_sData;
	int             m_iData;

public:
	CMVEventInfotext(int EventType);
	CMVEventInfotext(int EventType, CStringW sData, int iData);
	//CMVEventInfotext(int EventType, int iData);

	virtual ~CMVEventInfotext();

	int             GetEventType();
	CStringW         GetData();
	int             GetIData();
};


class CMVEventData : public CMVEvent  
{
public:
	enum eEvents { 
		EV_DATA_ALARMLIMITS_CHANGED,
		EV_DATA_CALCULATE_ALARMLIMITS
	};

private:
	int             m_EventType;
	/*CStringW         m_sData;
	int             m_iData;*/

public:
	CMVEventData(int EventType);
	/*CMVEventUI(int EventType, CStringW sData);
	CMVEventUI(int EventType, int iData);*/

	virtual ~CMVEventData();

	int             GetEventType();
	/*CStringW         GetData();
	int             GetIData();*/
};
