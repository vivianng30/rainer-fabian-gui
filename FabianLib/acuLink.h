//=============================================================================
/** 
* \file acuLink.h
* \brief Defines of acuLink protocol.
* \author Rainer Kuehner
* \date 2013-01-21 Creation of file
* \date 2017-03-27 update, new values and SpO2
*
*  Notice:	- IVOI:es dürfen auch alles RED ALARMS sein. Sie dürfen allerdings nur 15 Zeichen lang sein 
*			  (und nicht 16 Zeichen wie bei den YELLOWs).
*			- es dürfen max. 25 ALARMs und INOPs sein
*			- Anzahl der RED und YELLOW ALARMs ist innerhalb der Alarmliste egal
*			- Beachte: max. Länge RED ALARM: 15,   YELLOW ALARM: 16
*			- nicht verwendete Wert/Para auf _NV setzen (ALINK_NOTVALID)!
*
*
*			- Flow value: ml/min -40000 bis +40000, +2048	-> 1 Punkt == 16ml/min
*															-> 0 ml/min == 2048
*			- Pressure: mbar, -500 bis 1500, +2048			-> 1 Punkt == 0,1 mbar
*															-> 0 mbar == 2048
*			- Volume: ml/min 0 bis 6000						-> 1 Punkt == 2ml/min, no negative values!
*															-> 0 ml/min == 0
*
*
*       Copyright (c) 2013 Ing.Buero Rainer Kuehner
*                      Illerkirchberg
*
**/
//=============================================================================
#pragma once

// Error Codes (ACL = AcuLink)
#define ACL_ERR_NONE							(UINT)0x00000000
#define ACL_ERR_LICENCE_WRONG					(UINT)0x00000001
#define ACL_ERR_SPECTABLE_NOT_FOUND				(UINT)0x00000002
#define ACL_ERR_CANNOT_OPEN_PORT				(UINT)0x00000004
#define ACL_ERR_CANNOT_START_WATCHER			(UINT)0x00000008
#define ACL_ERR_WAVE_WRITE_NOT_STARTED			(UINT)0x00000010
#define ACL_ERR_WAVE_BUFFER_UNDERRUN			(UINT)0x00000020
#define ACL_ERR_SPECTABLE_SETUP_FILE_NOT_FOUND	(UINT)0x00000040
#define ACL_ERR_INIFILE_NOT_FOUND				(UINT)0x00000080
#define ACU_LINK_ERROR_BUFFER_OVERFLOW			(UINT)0x00000100

// LinkTypes
#define ACL_LT_NONE				0
#define ACL_LT_SERIAL_ASCII		1
#define ACL_LT_SERIAL_IVOI		2
#define ACL_LT_ETHERNET			3

// ab Version 2.2
#define ACL_LT_SERIAL_WAVE		5
#define ACL_LT_ETHERNET_WAVE	6

// Default-Port auf dem im Ethernet gehört wird
#define DEFAULT_PORT	5150

//NEWACULINK
#define MAX_WAVES		5
#define WAVE_VOL		0
#define WAVE_PRESS		1
#define WAVE_FLOW		2
#define WAVE_CO2		3
#define WAVE_SPO2		4


#define NUL_CHAR	(unsigned char)0x00
#define SOH_CHAR	(unsigned char)0x01
#define STX_CHAR	(unsigned char)0x02
#define ETX_CHAR	(unsigned char)0x03
#define EOT_CHAR	(unsigned char)0x04
#define ENQ_CHAR	(unsigned char)0x05
#define ACK_CHAR	(unsigned char)0x06
#define HT_CHAR		(unsigned char)0x09
#define LF_CHAR		(unsigned char)0x0A
#define VT_CHAR		(unsigned char)0x0B
#define CR_CHAR		(unsigned char)0x0D
#define NAK_CHAR	(unsigned char)0x15
#define ESC_CHAR	(unsigned char)0x1B
#define FS_CHAR		(unsigned char)0x1C
#define GS_CHAR		(unsigned char)0x1D
#define RS_CHAR		(unsigned char)0x1E
#define US_CHAR		(unsigned char)0x1F

//acuLink Arrays
//#define WM_ACULINK_SHUTDOWN		WM_USER + 0x4000	//!< window message for shutdown acuLink

/**
*	\def ALINK_MSMNT_ARRAY 
*		max size array of measurements
*	\def ALINK_SETT_ARRAY  
*		max size array of settings
*	\def ALINK_ALARM_ARRAY 
*		max size array of alarms
 **/
//NEWACULINK
#define ALINK_MSMNT_ARRAY			40	//max size array of measurements
#define ALINK_SETT_ARRAY			80	//max size array of settings
#define ALINK_ALARM_ARRAY			80	//max size array of alarms

//Arraygröße bei neuen Einträgen anpassen (tatsächlich benutzte Werte)
/**
*	\def ALINK_SIZE_ALARMARRAY 
*		actual size array of alarms
*	\def ALINK_SIZE_SETTARRAY  
*		actual size array of settings
*	\def ALINK_SIZE_MSMNTARRAY 
*		actual size array of measurements
 **/
//NEWACULINK
#define ALINK_SIZE_ALARMARRAY		76	//actual size array of alarms
#define ALINK_SIZE_SETTARRAY		79	//actual size array of settings
#define ALINK_SIZE_MSMNTARRAY		34	//actual size array of measurements

/**
*	\def ALINK_UNUSED 
*		value UNUSED
*	\def ALINK_OUTOFRANGE 
*		value OUTOFRANGE
*	\def ALINK_NOTVALID 
*		value NOTVALID
*	\def ALINK_OFF 
*		value OFF
 **/
#define ALINK_UNUSED		-32767	//value UNUSED
#define ALINK_OUTOFRANGE	-32766	//value OUTOFRANGE
#define ALINK_NOTVALID		-32765	//value NOTVALID
#define ALINK_OFF			-32764	//value OFF

/**
*	\def ALINK_MSMNT_P_PEAK 
*		Peak Pressure, Resolution: 0.1 (0 – 150 mbar bzw cmH2O) NV
*	\def ALINK_MSMNT_P_MEAN 
*		Mean Pressure, Resolution: 0.1 (-50.0 – 150.0 mbar bzw cmH2O) IPPV / SIPPV /SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP / HFO
*	\def ALINK_MSMNT_PEEP 
*		PEEP, Resolution: 0.1 (-50.0 – 150.0 mbar bzw cmH2O) IPPV / SIPPV /SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP 
*	\def ALINK_MSMNT_T_INSP_PSV 
*		Inspiratory Time PSV, Resolution: 0.001 (0.00 – 5.00 sec) SIMV+PSV /PSV 
*	\def ALINK_MSMNT_DYNCOMPL 
*		Dynamic Compliance, Resolution: 0.1 (0.0 – 500.0 ml/mbar) IPPV / SIPPV /SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP / HFO
*	\def ALINK_MSMNT_C20C 
*		Overextension Index C20/C , Resolution: 0.1 (0.0 – 50.0) IPPV / SIPPV /SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP / HFO
*	\def ALINK_MSMNT_RESISTANCE 
*		Ventilatory Resistance , Resolution: 0.1 (0.0 – 500.0 mbar/lps) IPPV / SIPPV /SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP / HFO
*	\def ALINK_MSMNT_MIN_VOL 
*		Minute Volume , Resolution: 0.001 (0.00 – 20.00 liter) IPPV / SIPPV /SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP / HFO
*	\def ALINK_MSMNT_ANTEIL_RESP 
*		Share Minute Volume Respira-tor , Resolution: 1 (0 – 100 %) SIMV / SIMV+PSV 
*	\def ALINK_MSMNT_TVE 
*		Expiratory Mandatory Tidal Volume , Resolution: 0.1 (0.0 – 600.0 ml) IPPV / SIPPV /SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP 
*	\def ALINK_MSMNT_TVE_RESP 
*		Expiratory Mandatory Tidal Volume Respirator , Resolution: 0.1 (0.0 – 600.0 ml) SIMV/ SIMV+PSV 
*	\def ALINK_MSMNT_TVE_PATIENT 
*		Expiratory Mandatory Tidal Volume Patient , Resolution: 0.1 (0.0 – 600.0 ml) SIMV/ SIMV+PSV 
*	\def ALINK_MSMNT_LEAK 
*		Leakage, Resolution: 1 (0 – 100 %) IPPV / SIPPV / SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP / HFO
*	\def ALINK_MSMNT_FREQ 
*		Respiratory Rate , Resolution: 1 (0 – 250 bpm) IPPV / SIPPV / SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP 
*	\def ALINK_MSMNT_TRIGGER 
*		Trigger Volume/Trigger Flow (see MODE OPTION2 Bit 7), Resolution: 0.1 (0.0 – 750.0 ml) SIPPV /SIMV / SIMV+PSV / PSV 
*	\def ALINK_MSMNT_TVI 
*		Inspiratory Mandatory Tidal Volume Resolution: 0.1 (0.0 – 600.0 ml) IPPV / SIPPV / SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP / HFO
*	\def ALINK_MSMNT_HFO_AMP 
*		HF Amplitude , Resolution: 0.1 (0 – 200 mbar bzw cmH2O) HFO
*	\def ALINK_MSMNT_TVE_HFO 
*		HF Expiratory Mandatory Tidal Volume, Resolution: 0.1 (0.0 – 200.0 ml) HFO
*	\def ALINK_MSMNT_DCO2 
*		Gas Transport Coefficient, Resolution: 1 (0 – 10000 ml^2/sec) HFO
*	\def ALINK_MSMNT_HFO_FREQ 
*		HF Rate, Resolution: 1 (0 – 20 hz) HFO
*	\def ALINK_MSMNT_OXY 
*		FiO2, Resolution: 0.1 (21.0 - 100.0 %) IPPV / SIPPV / SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP / HFO
*	\def ALINK_MSMNT_ETCO2 
*		etCO2 (see ALINK_SETT_UNIT_CO2), Resolution: 0.1 (0 – 150.0 mmHg, 0 – 20.0 kPa, or 0 – 19.7 %) Modul
*	\def ALINK_MSMNT_DEMAND_FLOW 
*		Demand Flow, Resolution: 0.001 (4.0 - 20.0 l/min) CPAP
*	\def ALINK_MSMNT_IFLOW 
*		Inspiratory Flow, Resolution: 0.001 (0.0 - 20.0 l/min) DUOPAP 
*	\def ALINK_MSMNT_EFLOW 
*		Expiratory Flow, Resolution: 0.001 (0.0 - 20.0 l/min) NCPAP /DUOPAP / HFO
*	\def ALINK_MSMNT_TAU
*		Time Constant, Resolution: 0.01 (0.0 - 250.0 sec) IPPV / SIPPV /SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP / HFO
*	\def ALINK_MSMNT_FREQETCO2
*		Respiration Rate CO2 module, Resolution: 1 (0 – 150 bpm)
*	\def ALINK_MSMNT_SPO2
*		SpO2,  Resolution: 0.1 (0.0 - 100.0 %), only with enabled SpO2 module
*	\def ALINK_MSMNT_SPO2_PI
*		Perfusion Index (Signal Strength), Resolution: (0==no measurement, 0.02% to 20%)
*	\def ALINK_MSMNT_SPO2_PR
*		Pulse Rate, Resolution: (0 to 240 bpm)
*	\def ALINK_MSMNT_SPO2_SIQ
*		Pleth Variability Index (PVI), Resolution: 1, (0 to 100)
*	\def ALINK_MSMNT_RSBI
*		rapid shallow breathing index, Resolution: 0.1, (0 – 2500)
*	\def ALINK_MSMNT_FICO2
*		FiCO2 of CO2 modules, Resolution: 0,1mmHg (0–150.0 mmHg, 0–20.0 kPa, or 0–19.7%)
*	\def ALINK_MSMNT_VTEKG
*		Vte / kg, Resolution: 1 [0==off, 300-30000g]
 **/
#define ALINK_MSMNT_P_PEAK			0	//Peak Pressure, Resolution: 0.1 (0 – 150 mbar bzw cmH2O) NV
#define ALINK_MSMNT_P_MEAN			1	//Mean Pressure, Resolution: 0.1 (-50.0 – 150.0 mbar bzw cmH2O)	IPPV / SIPPV /SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP / HFO
#define ALINK_MSMNT_PEEP			2	//PEEP, Resolution: 0.1 (-50.0 – 150.0 mbar bzw cmH2O)	IPPV / SIPPV /SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP 
#define ALINK_MSMNT_T_INSP_PSV		3	//Inspiratory Time PSV, Resolution: 0.001 (0.00 – 5.00 sec)				SIMV+PSV /PSV 
#define ALINK_MSMNT_DYNCOMPL		4	//Dynamic Compliance, Resolution for acuLink: 0.1 (0.0 – 500.0 ml/mbar), Resolution from PIC: 0.01 !!!	IPPV / SIPPV /SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP / HFO
#define ALINK_MSMNT_C20C			5	//Overextension Index C20/C , Resolution: 0.1 (0.0 – 50.0)						IPPV / SIPPV /SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP / HFO
#define ALINK_MSMNT_RESISTANCE		6	//Ventilatory Resistance , Resolution: 0.1 (0.0 – 500.0 mbar/lps)			IPPV / SIPPV /SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP / HFO
#define ALINK_MSMNT_MIN_VOL			7	//Minute Volume , Resolution: 0.001 (0.00 – 20.00 liter)			IPPV / SIPPV /SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP / HFO
#define ALINK_MSMNT_ANTEIL_RESP		8	//Share Minute Volume Respira-tor , Resolution: 1 (0 – 100 %)							SIMV / SIMV+PSV 
#define ALINK_MSMNT_TVE				9	//Expiratory Mandatory Tidal Volume , Resolution: 0.1 (0.0 – 600.0 ml)					IPPV / SIPPV /SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP 
#define ALINK_MSMNT_TVE_RESP		10	//Expiratory Mandatory Tidal Volume Respirator , Resolution: 0.1 (0.0 – 600.0 ml)					SIMV/ SIMV+PSV  / PSV
#define ALINK_MSMNT_TVE_PATIENT		11 	//Expiratory Mandatory Tidal Volume Patient , Resolution: 0.1 (0.0 – 600.0 ml)					SIMV/ SIMV+PSV  / PSV
#define ALINK_MSMNT_LEAK			12	//Leakage, Resolution: 1 (0 – 100 %)							IPPV / SIPPV / SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP / HFO
#define ALINK_MSMNT_FREQ			13	//Respiratory Rate , Resolution: 1 (0 – 250 bpm)						IPPV / SIPPV / SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP 
#define ALINK_MSMNT_TRIGGER			14	//Trigger Volume/Trigger Flow (see MODE OPTION2 Bit 7), Resolution: 0.1 (0.0 – 750.0 ml)					SIPPV /SIMV / SIMV+PSV / PSV 
#define ALINK_MSMNT_TVI				15	//Inspiratory Mandatory Tidal Volume Resolution: 0.1 (0.0 – 600.0 ml)					IPPV / SIPPV / SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP / HFO
#define ALINK_MSMNT_HFO_AMP			16	//HF Amplitude , Resolution: 0.1 (0 – 200 mbar bzw cmH2O)			HFO
#define ALINK_MSMNT_TVE_HFO			17	//HF Expiratory Mandatory Tidal Volume, Resolution: 0.1 (0.0 – 200.0 ml)					HFO
#define ALINK_MSMNT_DCO2			18	//Gas Transport Coefficient, Resolution: 1 (0 – 10000 ml^2/sec)				HFO
#define ALINK_MSMNT_HFO_FREQ		19	//HF Rate, Resolution: 1 (0 – 20 hz)							HFO
#define ALINK_MSMNT_OXY				20	//FiO2, Resolution: 0.1 (21.0 - 100.0 %)					IPPV / SIPPV / SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP / HFO
#define ALINK_MSMNT_ETCO2			21	//etCO2 (see ALINK_SETT_UNIT_CO2), Resolution: 0.1 (0 – 150.0 mmHg, 0 – 20.0 kPa, or 0 – 19.7 %) Modul
#define ALINK_MSMNT_DEMAND_FLOW		22	//Demand Flow, Resolution: 0.001 (4.0 - 20.0 l/min)				CPAP
#define ALINK_MSMNT_IFLOW			23	//Inspiratory Flow, Resolution: 0.001 (0.0 - 20.0 l/min)				DUOPAP 
#define ALINK_MSMNT_EFLOW			24	//Expiratory Flow, Resolution: 0.001 (0.0 - 20.0 l/min)				NCPAP /DUOPAP / HFO
//NEWACULINK
#define ALINK_MSMNT_TAU				25 //Time Constant, Resolution: 0.01, 0 – 250 (sec) IPPV / SIPPV /SIMV / SIMV+PSV / PSV / CPAP /NCPAP /DUOPAP / HFO			
#define ALINK_MSMNT_FREQETCO2		26 //Respiration Rate CO2 module, Resolution: 1 (0 – 150 bpm)	
#define ALINK_MSMNT_SPO2			27 //SpO2, Resolution: 0.1 (0.0 - 100.0 %), only with enabled SpO2 module		
#define ALINK_MSMNT_SPO2_PI			28 //Perfusion Index (Signal Strength), Resolution: 0.001, (0==no measurement, 0.02% to 20%)		
#define ALINK_MSMNT_SPO2_PR			29 //Pulse Rate, Resolution: 1, (0 to 240 bpm)		
#define ALINK_MSMNT_SPO2_SIQ		30 //Pleth Variability Index (PVI), Resolution: 1, (0 to 100)		
#define ALINK_MSMNT_RSBI			31 //rapid shallow breathing index, Resolution: 0.1, (0 – 2500) SIMV/ SIMV+PSV / PSV
#define ALINK_MSMNT_FICO2			32 //FiCO2 of CO2 modules, Resolution: 0,1mmHg (0–150.0 mmHg, 0–20.0 kPa, or 0–19.7%)
#define ALINK_MSMNT_VTEKG			33 //Vte / kg [g], Resolution: 0.1 [0==off, 300-30000g]
//!!!!!!!!!!!!!!! ACHTUNG - Verknüpfung zu den Meßwerten der Numerikblöcke !!!!!!



/**
*	\def ALINK_SETT_VENTRANGE 
*		Patient Range: 1 = NEONATAL, 2 = PEDIATRIC
*	\def ALINK_SETT_VENT_MODE 
*		Ventilator Mode: NONE=0,IPPV=1,SIPPV=2,SIMV=3,SIMVPSV=4,PSV=5,CPAP=6,NCPAP= 7,DUOPAP=8,HFO=9,SERVICE=15,
*	\def ALINK_SETT_P_INSP 
*		Inspiratory Pressure, Resolution: 0,1mbar bzw 0,1cmH2O
*	\def ALINK_SETT_P_PSV 
*		Inspiratory Pressure PSV, Resolution: 0,1mbar bzw 0,1cmH2O
*	\def ALINK_SETT_PEEP 
*		PEEP/CPAP, Resolution: 0,1mbar bzw 0,1cmH2O
*	\def ALINK_SETT_OXY 
*		FiO2, Resolution: 1%
*	\def ALINK_SETT_INSP_FLOW 
*		Inspiratory Flow, Resolution: 0,001Liter, in HFO: 0 entspricht "auto"	ok
*	\def ALINK_SETT_PLATEAU 
*		Plateau, Resolution: 1%
*	\def ALINK_SETT_EXH_FLOW 
*		Expiratory Flow / Flow in O2-Therapy mode, Resolution: 0,001Liter
*	\def ALINK_SETT_INSP_TIME 
*		Inspiratory Time, Resolution: 0,001sec
*	\def ALINK_SETT_EXH_TIME 
*		Expiratory Time, Resolution: 0,001sec
*	\def ALINK_SETT_FREQ 
*		Rate, Resolution: 1/h
*	\def ALINK_SETT_IERATIO_IPART 
*		I–Part TI :TE, Resolution: 0,1
*	\def ALINK_SETT_IERATIO_EPART 
*		E–Part TI :TE, Resolution: 0,1
*	\def ALINK_SETT_HF_AMPL 
*		HF Amplitude, Resolution: 0,1mbar bzw 0,1cmH2O
*	\def ALINK_SETT_HF_FREQ 
*		HF Rate, Resolution: 1hz
*	\def ALINK_SETT_HF_PMITT 
*		Mean Airway Pressure HFO, Resolution: 0,1mbar bzw 0,1cmH2O
*	\def ALINK_SETT_HF_PMEANREC 
*		Mean Airway Pressure HFO Lung Recruitment, Resolution: 0,1mbar bzw 0,1cmH2O
*	\def ALINK_SETT_VOLUMELIMIT 
*		Volume Limit, Resolution: 0,1ml
*	\def ALINK_SETT_VOLUMEGARANT 
*		Volume Guarantee, Resolution: 0,1ml
*	\def ALINK_SETT_FILTER_VOLGARANTY 
*		Averaging Volume Guarantee, Resolution: 1 breath
*	\def ALINK_SETT_ABORT_CRITERIONPSV 
*		Termination Criteria PSV, Resolution: 1%
*	\def ALINK_SETT_APNOE_TIME 
*		Apnea Time, Resolution: 1sec
*	\def ALINK_SETT_BACKUP 
*		Backup Frequency, Resolution: 1/min
*	\def ALINK_SETT_TRIG_SCHWELLE 
*		Trigger Sensitivity, Resolution: 0,1
*	\def ALINK_SETT_MODE_OPTION 
*		Mode Option 1: 
*		Bit 0: Start/Stop (Beatmung angehalten=1)
*		Bit 1: Status Volumengarantie (an=1)
*		Bit 2: Status Volumenlimit (an=1)
*		Bit 3: Bereich: 0=NEONATAL, 1 = PEDIATRIC
*		Bit 4: - internal use only-
*		Bit 5: - internal use only-
*		Bit 6: - internal use only-
*		Bit 7: Trigger-Modus (0=Volumetrigger, 1=Flowtrigger)
*		Bit 8: - internal use only-
*		Bit 9: - internal use only-
*		Bit 10+11: Schlauchset für NCPAP/DUOPAP: 00=InfantFlow, 10=MediJet, Rest notdef
*		Bit 12+13: I:E Ratio HFO: 00=1:3, 10=1:2, 01=1:1, Rest notdef
*		Bit 14: - internal use -
*	\def ALINK_SETT_MODE_OPTION2 
*		Mode Option 2:
*		Bit 0+1:	Kurvenform: 00=I-Flow, 01=Dreiekck, 10=AutoIFlow (PFG), Rest notdef
*		Bit 2:		HFO-Recruitment 1==on
*	\def ALINK_SETT_POWERSTATE 
*		Powerstate, 0=Netz, 1=Battery
*	\def ALINK_SETT_BATTERYSTATE_MINUTES 
*		Remaining Battery Time, Resolution: 1 Minute
*	\def ALINK_SETT_BATTERYSTATE_PERCENT 
*		Remaining Battery Percent, Resolution: 1 Prozent
*	\def ALINK_SETT_ALIMIT_MVMAX 
*		Limit MV high, Resolution: 0,001Liter
*	\def ALINK_SETT_ALIMIT_MVMIN 
*		Limit MV low, Resolution: 0,001Liter
*	\def ALINK_SETT_ALIMIT_PIPMAX 
*		Limit P peak high, Resolution: 0,1mbar bzw 0,1cmH2O
*	\def ALINK_SETT_ALIMIT_PEEPMIN 
*		Limit P peak  low, Resolution: 0,1mbar bzw 0,1cmH2O
*	\def ALINK_SETT_ALIMIT_FREQMAX 
*		Limit Rate, Resolution: 1/min
*	\def ALINK_SETT_ALIMIT_LEAKMAX 
*		Limit Leakage, Resolution: 1%
*	\def ALINK_SETT_ALIMIT_DCO2MAX 
*		Limit Gas Transport Coefficient high, Resolution: 1ml^2/sec oder ALINK_OFF
*	\def ALINK_SETT_ALIMIT_DCO2MIN 
*		Limit Gas Transport Coefficient low, Resolution: 1ml^2/sec oder ALINK_OFF
*	\def ALINK_SETT_ALIMIT_ETCO2MAX 
*		Limit etCO2 high, Resolution: 0,1mmHg oder 0,1kPa oder 0,1Vol%
*	\def ALINK_SETT_ALIMIT_ETCO2MIN 
*		Limit etCO2 low, Resolution: 0,1mmHg oder 0,1kPa oder 0,1Vol%
*	\def ALINK_SETT_UNIT_PRESSURE 
*		Measuring Unit Pressure (0=mbar, 1=cmH2O......)
*	\def ALINK_SETT_UNIT_CO2 
*		Measuring Unit CO2 (0=mmHg, 1=kPas, 2=Vol%)
*	\def ALINK_SETT_O2COMPENSATION_CO2 
*		Compensation, Resolution: 1%
*	\def ALINK_SETT_BAROPRESSURE_CO2 
*		Baropressure CO2, Resolution: 1mmHg
*	\def ALINK_SETT_DEVICEID_CATEGORY 
*		device ID category
*	\def ALINK_SETT_DEVICEID_VARIANT 
*		device ID variant
*	\def ALINK_SETT_VERSION_MMI_MAJOR 
*		version MMI major
*	\def ALINK_SETT_VERSION_MMI_MINOR 
*		version MMI minor
*	\def ALINK_SETT_VERSION_MMI_PATCH 
*		version MMI patch
*	\def ALINK_SETT_VERSION_MMI_BUILD 
*		version MMI build
*	\def ALINK_SETT_VERSION_MONPIC_MAJOR 
*		version Monitor PIC major
*	\def ALINK_SETT_VERSION_MONPIC_MINOR 
*		version Monitor PIC minor
*	\def ALINK_SETT_VERSION_MONPIC_PATCH 
*		version Monitor PIC patch
*	\def ALINK_SETT_VERSION_CTRLPIC_MAJOR 
*		version Controller PIC minor
*	\def ALINK_SETT_VERSION_CTRLPIC_MINOR 
*		version Controller PIC major 
*	\def ALINK_SETT_VERSION_CTRLPIC_PATCH 
*		version Controller PIC patch
*	\def ALINK_SETT_VERSION_ACULINK 
*		version ACULINK
*	\def ALINK_SETT_STATE_FLOWSENSOR 
*		state of flow sensor, 0=off,1=on
*	\def ALINK_SETT_STATE_OXYSENSOR 
*		state of oxy sensor, 0=off,1=on
*	\def ALINK_SETT_PMANUAL 
*		Pressure Manual Breath (CPAP/HFO), Resolution: 0.1mbar bzw 0.1cmH2O
*	\def ALINK_SETT_PDUO 
*		Pressure Manual Breath (DUO-PAP/NCPAP), Resolution: 0.1mbar bzw 0.1cmH2O
*	\def ALINK_SETT_FLUSHTIME 
*		FiO2 flush time, Resolution: 1sec
*	\def ALINK_SETT_O2FLUSH 
*		FiO2 flush, Resolution: 1%
*	\def ALINK_SETT_PERSID
*		PersID for identification
*	\def ALINK_SETT_ALIMIT_PRICO_SPO2LOW
*		SpO2 low limit range, Resolution: 1 (0 - 100)
*	\def ALINK_SETT_ALIMIT_PRICO_SPO2HIGH
*		SpO2 high limit range, Resolution: 1 (0 - 100)
*	\def ALINK_SETT_ALIMIT_PRICO_FIO2LOW
*		FiO2 low limit range, Resolution: 1 (0 - 100)
*	\def ALINK_SETT_ALIMIT_PRICO_FIO2HIGH
*		FiO2 low limit range, Resolution: 1 (0 - 100)
*	\def ALINK_SETT_ALIMIT_SPO2SIQMIN
*		Pleth Variability Index (PVI), Resolution: 1, (0 to 100)
*	\def ALINK_SETT_ALIMIT_SPO2MAX
*		Resolution: 0.1 (0.0 - 100.0 %)
*	\def ALINK_SETT_ALIMIT_SPO2MIN	
*		Resolution: 0.1 (0.0 - 100.0 %)
*	\def ALINK_SETT_ALIMIT_SPO2PIMIN
*		Perfusion Index, Resolution: 0.001, (0 to 20.000)
*	\def ALINK_SETT_ALIMIT_PIPMIN
*		Limit P peak low, Resolution: 0.1, (0.0- 150.0)
*	\def ALINK_SETT_ALIMIT_FICO2MAX	
*		Resolution: 0,1mmHg (0–150.0 mmHg, 0–20.0 kPa, or 0–19.7%)
*	\def ALINK_SETT_ALIMIT_FICO2MIN	
*		Resolution: 0,1mmHg (0–150.0 mmHg, 0–20.0 kPa, or 0–19.7%)
*	\def ALINK_SETT_ALIMIT_PULSERATEMAX
*		Resolution: 1, (0 to 240 bpm)
*	\def ALINK_SETT_ALIMIT_PULSERATEMIN
*		Resolution: 1, (0 to 240 bpm)
*	\def ALINK_SETT_ALIMIT_PMEANMAX
*		Resolution: 0.1, (0 to 100.0 bpm)
*	\def ALINK_SETT_ALIMIT_PMEANMIN
*		Resolution: 0.1, (0 to 100.0 bpm)
 **/
#define ALINK_SETT_VENTRANGE			0	//Patient Range: 1 = NEONATAL, 2 = PEDIATRIC
#define ALINK_SETT_VENT_MODE			1	//Ventilator Mode: NONE=0,IPPV=1,SIPPV=2,SIMV=3,SIMVPSV=4,PSV=5,CPAP=6,NCPAP= 7,DUOPAP=8,HFO=9,THERAPY=10, SERVICE=15,
#define ALINK_SETT_P_INSP				2	//Inspiratory Pressure, Resolution: 0,1mbar bzw 0,1cmH2O
#define ALINK_SETT_P_PSV				3	//Inspiratory Pressure PSV, Resolution: 0,1mbar bzw 0,1cmH2O
#define ALINK_SETT_PEEP					4	//PEEP/CPAP, Resolution: 0,1mbar bzw 0,1cmH2O
#define ALINK_SETT_OXY					5	//FiO2, Resolution: 1%
#define ALINK_SETT_INSP_FLOW			6	//Inspiratory Flow, Resolution: 0,001Liter, in HFO: 0 entspricht "auto"	ok
#define ALINK_SETT_RISETIME				7	//RiseTime, Resolution: 0,001sec ///////Plateau, Resolution: 1%
#define ALINK_SETT_EXH_FLOW				8	//Expiratory Flow / Flow in O2-Therapy mode, Resolution: 0,001Liter
#define ALINK_SETT_INSP_TIME			9	//Inspiratory Time, Resolution: 0,001sec
#define ALINK_SETT_EXH_TIME				10	//Expiratory Time, Resolution: 0,001sec
#define ALINK_SETT_FREQ					11	//Rate, Resolution: 1/h
#define ALINK_SETT_IERATIO_IPART		12	//I–Part TI :TE, Resolution: 0,1
#define ALINK_SETT_IERATIO_EPART		13	//E–Part TI :TE, Resolution: 0,1
#define ALINK_SETT_HF_AMPL				14	//HF Amplitude, Resolution: 0,1mbar bzw 0,1cmH2O
#define ALINK_SETT_HF_FREQ				15	//HF Rate, Resolution: 1hz
#define ALINK_SETT_HF_PMITT				16	//Mean Airway Pressure HFO, Resolution: 0,1mbar bzw 0,1cmH2O
#define ALINK_SETT_HF_PMEANREC			17	//Mean Airway Pressure HFO Lung Recruitment, Resolution: 0,1mbar bzw 0,1cmH2O
#define ALINK_SETT_VOLUMELIMIT			18	//Volume Limit, Resolution: 0,1ml
#define ALINK_SETT_VOLUMEGARANT			19	//Volume Guarantee, Resolution: 0,1ml
#define ALINK_SETT_FILTER_VOLGARANTY	20	//Averaging Volume Guarantee, Resolution: 1 breath
#define ALINK_SETT_ABORT_CRITERIONPSV	21	//Termination Criteria PSV, Resolution: 1%
#define ALINK_SETT_APNOE_TIME			22	//Apnea Time, Resolution: 1sec
#define ALINK_SETT_BACKUP				23	//Backup Frequency, Resolution: 1/min
#define ALINK_SETT_TRIG_SCHWELLE		24	//Trigger Sensitivity, Resolution: 0,1
#define ALINK_SETT_MODE_OPTION			25	//Bit 0: Start/Stop (Beatmung angehalten=1)
											//Bit 1: Status Volumengarantie (an=1)
											//Bit 2: Status Volumenlimit (an=1)
											//Bit 3: Bereich: 0=NEONATAL, 1 = PEDIATRIC
											//Bit 4: Flow sensor calibration running
											//Bit 5: O2 compensation enabled
											//Bit 6: Exhalation  valve calibration running
											//Bit 7: Trigger-Modus (0=Volumetrigger, 1=Flowtrigger)
											//Bit 8: Calibration process 21% O2 running
											//Bit 9: Calibration process 100% O2 running
											//Bit 10+11: 	Tubeset for NCPAP/DUOPAP:
											//				xxxx 00xx xxxx xxxx	= InfantFlow
											//				xxxx 01xx xxxx xxxx	= MediJet
											//				xxxx 10xx xxxx xxxx	=InfantFlowLP
											//				others notdef
											// Bit 12+13: 	I:E Ratio HFO:
											//				xx00 xxxx xxxx xxxx	= 1:3
											//				xx01 xxxx xxxx xxxx	= 1:2
											//				xx10 xxxx xxxx xxxx	= 1:1
											//				others notdef
											// Bit 14: 	- internal use -
											// Bit 15: manual breath running
											//
#define ALINK_SETT_MODE_OPTION2			26	//Bit 0+1:	Pressure rise control: 
											//				xxxx xx00	= I-Flow
											//				xxxx xx01	= 01=Ramp
											//				xxxx xx10	= AutoIFlow
											//				others notdef
											//Bit 2:	HFO-Recruitment 1==on
											//Bit 3:	HFO flow 1==on
											//Bit 4: not used
											//Bit 5: Bias flow, 0==internal, 1 == external
											//Bit 6+7+8:  Trigger mode
											//				xxx0 00xx xxxx	= volume trigger
											//				xxx0 01xx xxxx	= flow trigger
											//				xxx0 10xx xxxx	= pressure trigger
											//				xxx0 xxxx xxxx	= for future setting
											//Bit 9: FOT oscillation running
											//Bit 10+11: Leak compensation
											//				00xx xxxx xxxx	= off
											//				01xx xxxx xxxx	= low
											//				10xx xxxx xxxx	= middle
											//				11xx xxxx xxxx	= high
#define ALINK_SETT_POWERSTATE			27	//Powerstate, 0=Netz, 1=Battery
#define ALINK_SETT_BATTERYSTATE_MINUTES	28	//Remaining Battery Time, Resolution: 1 Minute
#define ALINK_SETT_BATTERYSTATE_PERCENT	29	//Remaining Battery Percent, Resolution: 1 Prozent
#define ALINK_SETT_ALIMIT_MVMAX			30	//Limit MV high, Resolution: 0,001Liter
#define ALINK_SETT_ALIMIT_MVMIN			31	//Limit MV low, Resolution: 0,001Liter
#define ALINK_SETT_ALIMIT_PIPMAX		32	//Limit P peak high, Resolution: 0,1mbar bzw 0,1cmH2O
#define ALINK_SETT_ALIMIT_PEEPMIN		33	//Limit P peak  low, Resolution: 0,1mbar bzw 0,1cmH2O
#define ALINK_SETT_ALIMIT_FREQMAX		34	//Limit Rate, Resolution: 1/min
#define ALINK_SETT_ALIMIT_LEAKMAX		35	//Limit Leakage, Resolution: 1%
#define ALINK_SETT_ALIMIT_DCO2MAX		36	//Limit Gas Transport Coefficient high, Resolution: 1ml^2/sec oder ALINK_OFF
#define ALINK_SETT_ALIMIT_DCO2MIN		37	//Limit Gas Transport Coefficient low, Resolution: 1ml^2/sec oder ALINK_OFF
#define ALINK_SETT_ALIMIT_ETCO2MAX		38	//Limit etCO2 high, Resolution: 0,1mmHg oder 0,1kPa oder 0,1Vol%
#define ALINK_SETT_ALIMIT_ETCO2MIN		39	//Limit etCO2 low, Resolution: 0,1mmHg oder 0,1kPa oder 0,1Vol%
#define ALINK_SETT_UNIT_PRESSURE		40	//Measuring Unit Pressure (0=mbar, 1=cmH2O......)
#define ALINK_SETT_UNIT_CO2				41	//Measuring Unit CO2 (0=mmHg, 1=kPas, 2=Vol%)
#define ALINK_SETT_O2COMPENSATION_CO2	42	//O2 Compensation, Resolution: 1%
#define ALINK_SETT_BAROPRESSURE_CO2		43	//Baropressure CO2, Resolution: 1mmHg
#define ALINK_SETT_DEVICEID_CATEGORY	44	//device ID category
#define ALINK_SETT_DEVICEID_VARIANT		45	//device ID variant
#define ALINK_SETT_VERSION_MMI_MAJOR	46	//version MMI major
#define ALINK_SETT_VERSION_MMI_MINOR	47	//version MMI minor
#define ALINK_SETT_VERSION_MMI_PATCH	48	//version MMI patch
#define ALINK_SETT_VERSION_MMI_BUILD	49	//version MMI build
#define ALINK_SETT_VERSION_MONPIC_MAJOR	50	//version Monitor PIC major
#define ALINK_SETT_VERSION_MONPIC_MINOR	51	//version Monitor PIC minor
#define ALINK_SETT_VERSION_MONPIC_PATCH	52	//version Monitor PIC patch
#define ALINK_SETT_VERSION_CTRLPIC_MAJOR	53	//version Controller PIC minor
#define ALINK_SETT_VERSION_CTRLPIC_MINOR	54	//version Controller PIC major 
#define ALINK_SETT_VERSION_CTRLPIC_PATCH	55	//version Controller PIC patch
#define ALINK_SETT_VERSION_ACULINK			56	//version ACULINK
#define ALINK_SETT_STATE_FLOWSENSOR			57	//state of flow sensor, 0=off,1=on
#define ALINK_SETT_STATE_OXYSENSOR			58	//state of oxy sensor, 0=off,1=on
#define ALINK_SETT_PMANUAL					59	//Pressure Manual Breath (CPAP/HFO), Resolution: 0.1mbar bzw 0.1cmH2O
#define ALINK_SETT_PDUO						60	//Pressure Manual Breath (DUO-PAP/NCPAP), Resolution: 0.1mbar bzw 0.1cmH2O
#define ALINK_SETT_FLUSHTIME				61	//FiO2 flush time, Resolution: 1sec
#define ALINK_SETT_O2FLUSH					62	//FiO2 flush, Resolution: 1%
#define ALINK_SETT_PERSID					63	//PersID for identification
//NEWACULINK
#define ALINK_SETT_ALIMIT_PRICO_SPO2LOW			64 //SpO2 low limit range, Resolution: 1 (0 - 100)
#define ALINK_SETT_ALIMIT_PRICO_SPO2HIGH		65 //SpO2 high limit range, Resolution: 1 (0 - 100)
#define ALINK_SETT_ALIMIT_PRICO_FIO2LOW			66 //FiO2 low limit range, Resolution: 1 (0 - 100)
#define ALINK_SETT_ALIMIT_PRICO_FIO2HIGH 		67 //FiO2 low limit range, Resolution: 1 (0 - 100)
#define ALINK_SETT_ALIMIT_SPO2SIQMIN			68 //Pleth Variability Index (PVI), Resolution: 1, (0 to 100)
#define ALINK_SETT_ALIMIT_SPO2MAX				69 //SpO2 high limit, Resolution: 0.1 (0.0 - 100.0 %)
#define ALINK_SETT_ALIMIT_SPO2MIN				70 //SpO2 low limit, Resolution: 0.1 (0.0 - 100.0 %)
#define ALINK_SETT_ALIMIT_SPO2PIMIN				71 //Perfusion Index (Signal Strength), Resolution: 0.001, (0==no measurement, 0.02% to 20%)
#define ALINK_SETT_ALIMIT_PIPMIN				72 //Limit P peak low, Resolution: 0.1, (0.0- 150.0)
#define ALINK_SETT_ALIMIT_FICO2MAX				73 //FiCO2 high limit, , Resolution: 0,1mmHg (0–150.0 mmHg, 0–20.0 kPa, or 0–19.7%)
#define ALINK_SETT_ALIMIT_FICO2MIN				74 //FiCO2 low limit, , Resolution: 0,1mmHg (0–150.0 mmHg, 0–20.0 kPa, or 0–19.7%)
#define ALINK_SETT_ALIMIT_PULSERATEMAX			75 //Pulse Rate high limit, Resolution: 1, (0 to 240 bpm)
#define ALINK_SETT_ALIMIT_PULSERATEMIN			76 //Pulse Rate low limit, Resolution: 1, (0 to 240 bpm)
#define ALINK_SETT_ALIMIT_PMEANMAX				77 //not implemented yet in Fabian!!!!! Pmean high limit, Resolution: 1, (0 to 240 bpm)
#define ALINK_SETT_ALIMIT_PMEANMIN				78 //not implemented yet in Fabian!!!!! Pmean low limit, Resolution: 1, (0 to 240 bpm)


/**
*	\def ALINK_ALARM_ENABLED 
*		Alarm status: AS_NONE=0, AS_ACTIVE=1, AS_SIGNALED=2
*	\def ALINK_ALARM_Accu_Empty 
*		Alarm Accu Empty 
*	\def ALINK_ALARM_SysFail_ChecksumConPIC 
*		Alarm Checksum error Controller PIC
*	\def ALINK_ALARM_SysFail_ChecksumMonPIC 
*		Alarm  Checksum error Monitor PIC
*	\def ALINK_ALARM_SysFail_RELAIS_DEFECT 
*		Alarm Relais defect
*	\def ALINK_ALARM_SysFail_P_IN_MIXER 
*		Alarm Input pressure blender
*	\def ALINK_ALARM_SysFail_OVERPRESSURE 
*		Alarm Excess pressure
*	\def ALINK_ALARM_SysFail_VOLTAGE 
*		Alarm Voltage monitoring
*	\def ALINK_ALARM_SysFail_IF_SPI 
*		Alarm SPI interface
*	\def ALINK_ALARM_SysFail_IF_DIO 
*		Alarm DIO interface
*	\def ALINK_ALARM_SysFail_IF_COM 
*		Alarm COM interface
*	\def ALINK_ALARM_SysFail_IF_I2C 
*		Alarm I2C interface
*	\def ALINK_ALARM_SysFail_IF_PIF 
*		Alarm PIF (Parallel interface) interface
*	\def ALINK_ALARM_SysFail_IF_ACULINK 
*		Alarm AcuLink interface
*	\def ALINK_ALARM_SysFail_OUTOFMEMORY 
*		Alarm Low physical memory
*	\def ALINK_ALARM_SysFail_Fan 
*		Alarm Coolingfan defect
*	\def ALINK_ALARM_SysFail_IF_CO2 
*		Alarm CO2 interface
*	\def ALINK_ALARM_SysFail_MIXER 
*		Alarm Blender defect
*	\def ALINK_ALARM_Accu_Defect 
*		Alarm Accu defect
*	\def ALINK_ALARM_SysAl_P_IN_O2 
*		Alarm Oxygen supply pressure
*	\def ALINK_ALARM_SysAl_P_IN_AIR 
*		Alarm Air supply pressure
*	\def ALINK_ALARM_SysAl_TUBE_OCCLUSION 
*		Alarm Tube Occlusion
*	\def ALINK_ALARM_DISCONNECTION 
*		Alarm Patient disconnected
*	\def ALINK_ALARM_TUBUSBLOCKED 
*		Alarm check ET tube
*	\def ALINK_ALARM_Sens_FLOW_SENSOR_DEFECT 
*		Alarm Flow sensor defect
*	\def ALINK_ALARM_Sens_FLOW_SENSOR_CLEANING 
*		Alarm Clean flow sensor
*	\def ALINK_ALARM_Sens_FLOW_SENSOR_NOTCONNECTED 
*		Alarm Flow sensor not connected
*	\def ALINK_ALARM_Sens_O2_SENSOR_DEFECT 
*		Alarm Oxygen sensor defect
*	\def ALINK_ALARM_Sens_O2_SENSOR_USED 
*		Alarm Oxygen sensor used up
*	\def ALINK_ALARM_Sens_O2_VALUE_INCORRECT 
*		Alarm O2 value out of range
*	\def ALINK_ALARM_Sens_O2_NOT_CALIBRATED 
*		Alarm O2 sensor not calibrated
*	\def ALINK_ALARM_PatAl_MVmax 
*		Alarm High minute volume
*	\def ALINK_ALARM_PatAl_MVmin 
*		Alarm Low minute volume
*	\def ALINK_ALARM_PatAl_Pmax 
*		Alarm Pressure too high
*	\def ALINK_ALARM_PatAl_PminLow 
*		Alarm Pressure too low
*	\def ALINK_ALARM_PatAl_PminHigh 
*		Alarm High PEEP alarm
*	\def ALINK_ALARM_PatAl_BPMmax 
*		Alarm High breath rate
*	\def ALINK_ALARM_PatAl_Leakmax 
*		Alarm High tube leak
*	\def ALINK_ALARM_PatAl_Apnoe 
*		Alarm Apnea alarm
*	\def ALINK_ALARM_PatAl_DCO2max 
*		Alarm High DCO2 alarm
*	\def ALINK_ALARM_PatAl_DCO2min 
*		Alarm Low DCO2 alarm
*	\def ALINK_ALARM_PatAl_ETCO2max 
*		Alarm High ETCO2 alarm
*	\def ALINK_ALARM_PatAl_ETCO2min 
*		Alarm Low ETCO2 alarm
*	\def ALINK_ALARM_SysLimit_Pinsp_NotReached 
*		Alarm Inspiratory pressure not reached
*	\def ALINK_ALARM_SysLimit_Vlimitted 
*		Alarm Tidal Volume limited
*	\def ALINK_ALARM_SysLimit_Vgaranty 
*		Alarm Tidal Volume not reached
*	\def ALINK_ALARM_Accu_POWER 
*		Alarm Power failure
*	\def ALINK_ALARM_Accu_60 
*		Alarm Charge battery (<60min)
*	\def ALINK_ALARM_Accu_30 
*		Alarm Charge battery (<30min)
*	\def ALINK_ALARM_Accu_15 
*		Alarm Charge battery (<15min)
*	\def ALINK_ALARM_Nebulizer_Disconnection 
*		Alarm Nebulizer disconnected
*	\def ALINK_ALARM_Nebulizer_SysError 
*		Alarm Nebulizer ERROR
*	\def ALINK_ALARM_CO2_MODULE_NOTCONNECTED 
*		Alarm CO2 module disconnected
*	\def ALINK_ALARM_CO2_FILTERLINE_NOTCONNECTED 
*		Alarm CO2 FilterLine disconnected
*	\def ALINK_ALARM_CO2_CHECKSAMPLINGLINE 
*		Alarm check CO2 sampling line
*	\def ALINK_ALARM_CO2_CHECKAIRWAYADAPTER 
*		Alarm check CO2 airway adapter
*	\def ALINK_ALARM_CO2_SENSORFAULTY 
*		Alarm CO2 sensor faulty
*	\def ALINK_ALARM_PRICO_FiO2max
*		Alarm high FiO2 (PRICO)
*	\def ALINK_ALARM_PRICO_FiO2min
*		Alarm low FiO2 (PRICO)
*	\def ALINK_ALARM_Sens_PRICO_FiO2outOfRange
*		Alarm FiO2 out of range (PRICO)
*	\def ALINK_ALARM_Sens_SPO2_MODULE_NOTCONNECTED
*		Alarm SpO2 module not connected
*	\def ALINK_ALARM_Sens_SPO2_SENSORFAULTY
*		Alarm SpO2 sensor faulty
*	\def ALINK_ALARM_Sens_SPO2_CHECKSENSOR
*		Alarm SpO2 check sensor
*	\def ALINK_ALARM_PatAl_SPO2_SIQmin
*		SpO2 low SIQ
*	\def ALINK_ALARM_PatAl_SPO2max
*		Alarm high SpO2
*	\def ALINK_ALARM_PatAl_SPO2min
*		Alarm low SpO2
*	\def ALINK_ALARM_PatAl_SPO2_PImin
*		Alarm low SpO2 PI
*	\def ALINK_ALARM_PatAl_PIPmin
*		Alarm P peak low
*	\def ALINK_ALARM_PatAl_FICO2max	
*		Alarm FiCO2 high
*	\def ALINK_ALARM_PatAl_FICO2min
*		Alarm FiCO2 low
*	\def ALINK_ALARM_PatAl_PulseRatemax
*		Alarm Pulse Rate high
*	\def ALINK_ALARM_PatAl_PulseRatemin
*		Alarm Pulse Rate low
*	\def ALINK_ALARM_PatAl_PMEANmax
*		Alarm Pmean high
*	\def ALINK_ALARM_PatAl_PMEANmin
*		Alarm Pmean high
 **/
#define ALINK_ALARM_ENABLED							0
#define ALINK_ALARM_Accu_Empty						1  
#define ALINK_ALARM_SysFail_ChecksumConPIC			2
#define ALINK_ALARM_SysFail_ChecksumMonPIC			3
#define ALINK_ALARM_SysFail_RELAIS_DEFECT			4 
//#define ALINK_ALARM_SysFail_P_PROXIMAL				5 
#define ALINK_ALARM_SysFail_P_IN_MIXER				6 
#define ALINK_ALARM_SysFail_OVERPRESSURE			7 
#define ALINK_ALARM_SysFail_VOLTAGE					8
#define ALINK_ALARM_SysFail_IF_SPI					9
#define ALINK_ALARM_SysFail_IF_DIO					10
#define ALINK_ALARM_SysFail_IF_COM					11
#define ALINK_ALARM_SysFail_IF_I2C					12
#define ALINK_ALARM_SysFail_IF_PIF					13
#define ALINK_ALARM_SysFail_IF_ACULINK				14
#define ALINK_ALARM_SysFail_OUTOFMEMORY				15 
#define ALINK_ALARM_SysFail_Fan						16
#define ALINK_ALARM_SysFail_IF_CO2					17
#define ALINK_ALARM_SysFail_MIXER					18
#define ALINK_ALARM_Accu_Defect						19
#define ALINK_ALARM_SysAl_P_IN_O2					20  
#define ALINK_ALARM_SysAl_P_IN_AIR					21  
#define ALINK_ALARM_SysAl_TUBE_OCCLUSION			22 
//#define ALINK_ALARM_SysAl_P_EXSPIRATIONTUBE			22 
//#define ALINK_ALARM_SysAl_P_INSPIRATIONTUBE			23
#define ALINK_ALARM_DISCONNECTION					24
#define ALINK_ALARM_TUBUSBLOCKED					25
#define ALINK_ALARM_Sens_FLOW_SENSOR_DEFECT			26 
#define ALINK_ALARM_Sens_FLOW_SENSOR_CLEANING		27
#define ALINK_ALARM_Sens_FLOW_SENSOR_NOTCONNECTED	28 
#define ALINK_ALARM_Sens_O2_SENSOR_DEFECT			29 
#define ALINK_ALARM_Sens_O2_SENSOR_USED				30
#define ALINK_ALARM_Sens_O2_VALUE_INCORRECT			31 
#define ALINK_ALARM_Sens_O2_NOT_CALIBRATED			32
#define ALINK_ALARM_PatAl_MVmax						33
#define ALINK_ALARM_PatAl_MVmin						34 
#define ALINK_ALARM_PatAl_Pmax						35 
#define ALINK_ALARM_PatAl_PminLow					36 
#define ALINK_ALARM_PatAl_PminHigh					37 
#define ALINK_ALARM_PatAl_BPMmax					38 
#define ALINK_ALARM_PatAl_Leakmax					39 
#define ALINK_ALARM_PatAl_Apnoe						40 
#define ALINK_ALARM_PatAl_DCO2max					41 
#define ALINK_ALARM_PatAl_DCO2min					42 
#define ALINK_ALARM_PatAl_ETCO2max					43 
#define ALINK_ALARM_PatAl_ETCO2min					44 
#define ALINK_ALARM_SysLimit_Pinsp_NotReached		45  
#define ALINK_ALARM_SysLimit_Vlimitted				46
#define ALINK_ALARM_SysLimit_Vgaranty				47
#define ALINK_ALARM_Accu_POWER						48
#define ALINK_ALARM_Accu_60							49  
#define ALINK_ALARM_Accu_30							50  
#define ALINK_ALARM_Accu_15							51
#define ALINK_ALARM_Nebulizer_Disconnection			52  
#define ALINK_ALARM_Nebulizer_SysError				53
#define ALINK_ALARM_CO2_MODULE_NOTCONNECTED			54
#define ALINK_ALARM_CO2_FILTERLINE_NOTCONNECTED		55
#define ALINK_ALARM_CO2_CHECKSAMPLINGLINE			56
#define ALINK_ALARM_CO2_CHECKAIRWAYADAPTER			57
#define ALINK_ALARM_CO2_SENSORFAULTY				58
//NEWACULINK
#define ALINK_ALARM_PRICO_FiO2max					59
#define ALINK_ALARM_PRICO_FiO2min					60
#define ALINK_ALARM_Sens_PRICO_FiO2outOfRange		61
#define ALINK_ALARM_Sens_SPO2_MODULE_NOTCONNECTED	62
#define ALINK_ALARM_Sens_SPO2_SENSORFAULTY			63
#define ALINK_ALARM_Sens_SPO2_CHECKSENSOR			64
#define ALINK_ALARM_PatAl_SPO2_SIQmin				65
#define ALINK_ALARM_PatAl_SPO2max					66
#define ALINK_ALARM_PatAl_SPO2min					67
#define ALINK_ALARM_PatAl_SPO2_PImin				68
#define ALINK_ALARM_PatAl_PIPmin					69
#define ALINK_ALARM_PatAl_FICO2max					70
#define ALINK_ALARM_PatAl_FICO2min					71
#define ALINK_ALARM_PatAl_PulseRatemax				72
#define ALINK_ALARM_PatAl_PulseRatemin				73
#define ALINK_ALARM_PatAl_PMEANmax					74//not implemented yet in Fabian!!!!!
#define ALINK_ALARM_PatAl_PMEANmin					75//not implemented yet in Fabian!!!!!

//NEWACULINK
typedef struct WAVE_RECORD_T
{
	short	fshVol;
	short	fshPress;
	short	fshFlow;
	short	fshCO2;
	short	fshSPO2;
} WaveRecord;
