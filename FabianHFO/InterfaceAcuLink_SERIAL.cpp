// InterfaceAcuLink_SERIAL.cpp : implementation file
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "InterfaceAcuLink_SERIAL.h"
#include "MVModel.h"
#include "TlsFile.h"
#include "TlsStream.h"

// Polynom für die CRC-16 Berechnung (nach CCITT)
//			X^16 + x^12 + x^5 + 1
// Dabei wird bei CRC16 immer implizit von x^16 ausgegangen,
// also: x^16+x^12+x^5+1 also 0b1.0001.0000.0010.0001 = 11021.
#define   POLYNOMIAL   0x1021
WORD crc_table[256];

int gMeasurementNkArray[ALINK_MSMNT_ARRAY] =
{
	1,	// ALINK_MSMNT_AVG_P_PEAK		 0	//Resolution: 0.1 (0 – 150 mbar bzw cmH2O) NV
	1,	// ALINK_MSMNT_AVG_P_MEAN		 1	//Resolution: 0.1 (-50.0 – 150.0 mbar bzw cmH2O)
	1,	// ALINK_MSMNT_AVG_PEEP			 2	//Resolution: 0.1 (-50.0 – 150.0 mbar bzw cmH2O)
	3,	// ALINK_MSMNT_AVG_T_INSP_PSV	 3	//Resolution: 0.001 (0.00 – 5.00 sec)
	1,	// ALINK_MSMNT_AVG_DYNCOMPL		 4	//Resolution: 0.1 (0.0 – 500.0 ml/mbar)
	1,	// ALINK_MSMNT_AVG_C20C			 5	//Resolution: 0.1 (0.0 – 50.0)
	1,	// ALINK_MSMNT_AVG_RESISTANCE	 6	//Resolution: 0.1 (0.0 – 500.0 mbar/lps)
	3,	// ALINK_MSMNT_AVG_MIN_VOL		 7	//Resolution: 0.001 (0.00 – 20.00 liter)
	0,	// ALINK_MSMNT_AVG_ANTEIL_RESP	 8	//Resolution: 1 (0 – 100 %)
	1,	// ALINK_MSMNT_AVG_TVE			 9	//Resolution: 0.1 (0.0 – 600.0 ml)
	1,	// ALINK_MSMNT_AVG_TVE_RESP		10	//Resolution: 0.1 (0.0 – 600.0 ml)
	1,	// ALINK_MSMNT_AVG_TVE_PATIENT	11 	//Resolution: 0.1 (0.0 – 600.0 ml)
	0,	// ALINK_MSMNT_AVG_LEAK			12	//Resolution: 1 (0 – 100 %)
	0,	// ALINK_MSMNT_AVG_FREQ			13	//Resolution: 1 (0 – 250 bpm)
	1,	// ALINK_MSMNT_AVG_TRIGGER		14	//Resolution: 0.1 (0.0 – 750.0 ml)
	1,	// ALINK_MSMNT_AVG_TVI			15	//Resolution: 0.1 (0.0 – 600.0 ml)
	1,	// ALINK_MSMNT_AVG_HFO_AMP		16	//Resolution: 0.1 (0 – 200 mbar bzw cmH2O)
	1,	// ALINK_MSMNT_AVG_TVE_HFO		17	//Resolution: 0.1 (0.0 – 200.0 ml)
	0,	// ALINK_MSMNT_AVG_DCO2			18	//Resolution: 1 (0 – 10000 ml^2/sec)
	0,	// ALINK_MSMNT_AVG_HFO_FREQ		19	//Resolution: 1 (0 – 20 hz)
	1,	// ALINK_MSMNT_OXY				20	//Resolution: 0.1 (21.0 - 100.0 %)
	1,	// ALINK_MSMNT_AVG_ETCO2		21	//Resolution: 0.1 (0 – 150.0 mmHg, 0 – 20.0 kPa, or 0 – 19.7 %) Modul
	3,	// ALINK_MSMNT_DEMAND_FLOW		22	//Resolution: 0.001 (4.0 - 20.0 l/min)
	3,	// ALINK_MSMNT_IFLOW			23	//Resolution: 0.001 (0.0 - 20.0 l/min)
	3	// ALINK_MSMNT_EFLOW			24	//Resolution: 0.001 (0.0 - 20.0 l/min)
};

int gSettingNkArray[ALINK_SETT_ARRAY] = 
{
	0,	// ALINK_SETT_VENTRANGE				 0	//	1 = NEONATAL, 2 = PEDIATRIC
	-1,	// ALINK_SETT_VENT_MODE				 1	//	NONE=0,IPPV=1,SIPPV=2,SIMV=3,SIMVPSV=4,PSV=5,CPAP=6,NCPAP= 7,DUOPAP=8,HFO=9,SERVICE=15,
	1,	// ALINK_SETT_P_INSP				 2	//	Resolution: 0,1mbar bzw 0,1cmH2O
	1,	// ALINK_SETT_P_PSV					 3	//	Resolution: 0,1mbar bzw 0,1cmH2O
	1,	// ALINK_SETT_PEEP					 4	//	Resolution: 0,1mbar bzw 0,1cmH2O
	0,	// ALINK_SETT_OXY					 5	//	Resolution: 1%
	3,	// ALINK_SETT_INSP_FLOW				 6	//	Resolution: 0,001Liter, in HFO: 0 entspricht "auto"	ok
	3,	// ALINK_SETT_RISETIME				 7	//	Resolution: 0,001sec
	3,	// ALINK_SETT_EXH_FLOW				 8	//	Resolution: 0,001Liter
	3,	// ALINK_SETT_INSP_TIME				 9	//	Resolution: 0,001sec
	3,	// ALINK_SETT_EXH_TIME				10	//	Resolution: 0,001sec
	0,	// ALINK_SETT_FREQ					11	//	Resolution: 1/h
	1,	// ALINK_SETT_IERATIO_IPART			12	//	Resolution: 0,1
	1,	// ALINK_SETT_IERATIO_EPART			13	//	Resolution: 0,1
	1,	// ALINK_SETT_HF_AMPL				14	//	Resolution: 0,1mbar bzw 0,1cmH2O
	0,	// ALINK_SETT_HF_FREQ				15	//	Resolution: 1hz
	1,	// ALINK_SETT_HF_PMITT				16	//	Resolution: 0,1mbar bzw 0,1cmH2O
	1,	// ALINK_SETT_HF_PMEANREC			17	//	Resolution: 0,1mbar bzw 0,1cmH2O
	1,	// ALINK_SETT_VOLUMELIMIT			18	//	Resolution: 0,1ml
	1,	// ALINK_SETT_VOLUMEGARANT			19	//	Resolution: 0,1ml
	0,	// ALINK_SETT_FILTER_VOLGARANTY		20	//	Resolution: 1 breath
	0,	// ALINK_SETT_ABORT_CRITERIONPSV	21	//	Resolution: 1%
	0,	// ALINK_SETT_APNOE_TIME			22	//	Resolution: 1sec
	0,	// ALINK_SETT_BACKUP				23	//	Resolution: 1/min
	1,	// ALINK_SETT_TRIG_SCHWELLE			24	//	Resolution: 0,1
	0,	// ALINK_SETT_MODE_OPTION			25	//	Bit-Muster
	0,	// ALINK_SETT_MODE_OPTION2			26	//	Bit-Muster
	0,	// ALINK_SETT_POWERSTATE			27	//	0=Netz, 1=Battery
	0,	// ALINK_SETT_BATTERYSTATE_MINUTES	28	//	Resolution: 1 Minute
	0,	// ALINK_SETT_BATTERYSTATE_PERCENT	29	//	Resolution: 1 Prozent
	3,	// ALINK_SETT_ALIMIT_MVMAX			30	//	Resolution: 0,001Liter
	3,	// ALINK_SETT_ALIMIT_MVMIN			31	//	Resolution: 0,001Liter
	1,	// ALINK_SETT_ALIMIT_PIPMAX			32	//	Resolution: 0,1mbar bzw 0,1cmH2O
	1,	// ALINK_SETT_ALIMIT_PEEPMIN			33	//	Resolution: 0,1mbar bzw 0,1cmH2O
	0,	// ALINK_SETT_ALIMIT_FREQMAX		34	//	Resolution: 1/min
	0,	// ALINK_SETT_ALIMIT_LEAKMAX		35	//	Resolution: 1%
	0,	// ALINK_SETT_ALIMIT_DCO2MAX		36	//	Resolution: 1ml^2/sec oder ALINK_OFF
	0,	// ALINK_SETT_ALIMIT_DCO2MIN		37	//	Resolution: 1ml^2/sec oder ALINK_OFF
	1,	// ALINK_SETT_ALIMIT_ETCO2MAX		38	//	Resolution: 0,1mmHg oder 0,1kPa oder 0,1Vol%
	1,	// ALINK_SETT_ALIMIT_ETCO2MIN		39	//	Resolution: 0,1mmHg oder 0,1kPa oder 0,1Vol%
	0,	// ALINK_SETT_UNIT_PRESSURE			40	//	Einheit druck (0=mbar, 1=cmH2O......)
	0,	// ALINK_SETT_UNIT_CO2				41	//	Einheit co2 modul (0=mmHg, 1=kPas, 2=Vol%)
	0,	// ALINK_SETT_O2COMPENSATION_CO2	42	//	Resolution: 1%
	0,	// ALINK_SETT_BAROPRESSURE_CO2		43	//	Resolution: 1mmHg
	-44,	// ALINK_SETT_DEVICEID_CATEGORY		44	//Gerätenummer Teil1 (MAC, upper part): Spezialverarbeitung
	-45,	// ALINK_SETT_DEVICEID_VARIANT		45	//Gerätenummer Teil2 (MAC, lower part): Spezialverarbeitung
	0,	// ALINK_SETT_VERSION_MMI_MAJOR		46	//
	0,	// ALINK_SETT_VERSION_MMI_MINOR		47	//
	0,	// ALINK_SETT_VERSION_MMI_PATCH		48	//
	0,	// ALINK_SETT_VERSION_MMI_BUILD		49	//
	0,	// ALINK_SETT_VERSION_MONPIC_MAJOR	50	//
	0,	// ALINK_SETT_VERSION_MONPIC_MINOR	51	//
	0,	// ALINK_SETT_VERSION_MONPIC_PATCH	52	//
	0,	// ALINK_SETT_VERSION_CTRLPIC_MAJOR	53	//
	0,	// ALINK_SETT_VERSION_CTRLPIC_MINOR	54	//
	0,	// ALINK_SETT_VERSION_CTRLPIC_PATCH	55	//
	-56,	// ALINK_SETT_VERSION_ACULINK		56	// Versionsnummern: Spezialbehandlung
	0,	// ALINK_SETT_STATE_FLOWSENSOR		57	//
	0,	// ALINK_SETT_STATE_OXYSENSOR		58	//
	0,	// ALINK_SETT_PMANUAL				59	// neu!!!!!!!!!
	0,	// ALINK_SETT_PDUO					60	// neu!!!!!!!!!
	0,	// ALINK_SETT_FLUSHTIME			61	// neu!!!!!!!!!
	0,	// ALINK_SETT_O2FLUSH				62	// neu!!!!!!!!!

};


int gMeasurementArray[ALINK_MSMNT_ARRAY] = 
{
	ALINK_UNUSED, ALINK_UNUSED, ALINK_UNUSED, ALINK_UNUSED, ALINK_UNUSED, 
	ALINK_UNUSED, ALINK_UNUSED, ALINK_UNUSED, ALINK_UNUSED, ALINK_UNUSED,

	ALINK_UNUSED, ALINK_UNUSED, ALINK_UNUSED, ALINK_UNUSED, ALINK_UNUSED, 
	ALINK_UNUSED, ALINK_UNUSED, ALINK_UNUSED, ALINK_UNUSED, ALINK_UNUSED,

	ALINK_UNUSED, ALINK_UNUSED, ALINK_UNUSED, ALINK_UNUSED, ALINK_UNUSED, 
	ALINK_UNUSED, ALINK_UNUSED, ALINK_UNUSED, ALINK_UNUSED, ALINK_UNUSED,
};

int gSettingArray[ALINK_SETT_ARRAY] = 
{
	ALINK_UNUSED, ALINK_UNUSED, ALINK_UNUSED, ALINK_UNUSED, ALINK_UNUSED, 
	ALINK_UNUSED, ALINK_UNUSED, ALINK_UNUSED, ALINK_UNUSED, ALINK_UNUSED,

	ALINK_UNUSED, ALINK_UNUSED, ALINK_UNUSED, ALINK_UNUSED, ALINK_UNUSED, 
	ALINK_UNUSED, ALINK_UNUSED, ALINK_UNUSED, ALINK_UNUSED, ALINK_UNUSED,

	ALINK_UNUSED, ALINK_UNUSED, ALINK_UNUSED, ALINK_UNUSED, ALINK_UNUSED, 
	ALINK_UNUSED, ALINK_UNUSED, ALINK_UNUSED, ALINK_UNUSED, ALINK_UNUSED,

	ALINK_UNUSED, ALINK_UNUSED, ALINK_UNUSED, ALINK_UNUSED, ALINK_UNUSED, 
	ALINK_UNUSED, ALINK_UNUSED, ALINK_UNUSED, ALINK_UNUSED, ALINK_UNUSED,

	ALINK_UNUSED, ALINK_UNUSED, ALINK_UNUSED, ALINK_UNUSED, ALINK_UNUSED, 
	ALINK_UNUSED, ALINK_UNUSED, ALINK_UNUSED, ALINK_UNUSED, ALINK_UNUSED,

	ALINK_UNUSED, ALINK_UNUSED, ALINK_UNUSED, ALINK_UNUSED, ALINK_UNUSED, 
	ALINK_UNUSED, ALINK_UNUSED, ALINK_UNUSED, ALINK_UNUSED, ALINK_UNUSED,

	ALINK_UNUSED, ALINK_UNUSED, ALINK_UNUSED, ALINK_UNUSED, ALINK_UNUSED, 
	ALINK_UNUSED, ALINK_UNUSED, ALINK_UNUSED, ALINK_UNUSED, ALINK_UNUSED,
};

int gAlarmArray[ALINK_ALARM_ARRAY] = 
{
	ALINK_UNUSED, ALINK_UNUSED, ALINK_UNUSED, ALINK_UNUSED, ALINK_UNUSED, 
	ALINK_UNUSED, ALINK_UNUSED, ALINK_UNUSED, ALINK_UNUSED, ALINK_UNUSED,

	ALINK_UNUSED, ALINK_UNUSED, ALINK_UNUSED, ALINK_UNUSED, ALINK_UNUSED, 
	ALINK_UNUSED, ALINK_UNUSED, ALINK_UNUSED, ALINK_UNUSED, ALINK_UNUSED,

	ALINK_UNUSED, ALINK_UNUSED, ALINK_UNUSED, ALINK_UNUSED, ALINK_UNUSED, 
	ALINK_UNUSED, ALINK_UNUSED, ALINK_UNUSED, ALINK_UNUSED, ALINK_UNUSED,

	ALINK_UNUSED, ALINK_UNUSED, ALINK_UNUSED, ALINK_UNUSED, ALINK_UNUSED, 
	ALINK_UNUSED, ALINK_UNUSED, ALINK_UNUSED, ALINK_UNUSED, ALINK_UNUSED,

	ALINK_UNUSED, ALINK_UNUSED, ALINK_UNUSED, ALINK_UNUSED, ALINK_UNUSED, 
	ALINK_UNUSED, ALINK_UNUSED, ALINK_UNUSED, ALINK_UNUSED, ALINK_UNUSED,

	ALINK_UNUSED, ALINK_UNUSED, ALINK_UNUSED, ALINK_UNUSED, ALINK_UNUSED, 
	ALINK_UNUSED, ALINK_UNUSED, ALINK_UNUSED, ALINK_UNUSED, ALINK_UNUSED,

	ALINK_UNUSED, ALINK_UNUSED, ALINK_UNUSED, ALINK_UNUSED, ALINK_UNUSED, 
	ALINK_UNUSED, ALINK_UNUSED, ALINK_UNUSED, ALINK_UNUSED, ALINK_UNUSED,
};

/*
 * Array aus Records für die Kurven (Waves)
*/

//DllWaveRecord gDllWaveRecords[TOTAL_MAX_WAVE_RECORDS] =
//{
//	// 100
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//
//	// 200
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//
//	// 300
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//
//	// 400
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//
//	// 500
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//
//	// 600
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//
//	// 700
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//
//	// 800
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//
//	// 900
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//
//	// 1000
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
//}; // WaveRecord gWaveRecords[MAX_WAVE_RECORDS]



// CInterfaceAcuLink_SERIAL


CInterfaceAcuLink_SERIAL::CInterfaceAcuLink_SERIAL()
{
	//            <     EXE - Version  ><DLL - Version>       
	m_iVersion = (1 << 24) | (0 << 16) | (0 << 8) | 0;
}

CInterfaceAcuLink_SERIAL::~CInterfaceAcuLink_SERIAL()
{
}



// CInterfaceAcuLink_SERIAL message handlers
// **************************************************************************
// 
// **************************************************************************
bool CInterfaceAcuLink_SERIAL::init()
{
	bool bRes=false;

	if(getModel()->getCONFIG()->GetPDMSprotocol()==ACL_RS232_DATA)
	{
		// initialisiere CRC Tabelle
		initCRC_Table();

		if (!m_comPort.Open(2,38400))
		{
			//theApp.getLog()->WriteLine(_T("#HFO:0009"));
			return false;
		}
	}
	else if(getModel()->getCONFIG()->GetPDMSprotocol()==ACL_RS232_WAVES)
	{
		// initialisiere CRC Tabelle
		initCRC_Table();

		if (!m_comPort.Open(2,115200))
		{
			//theApp.getLog()->WriteLine(_T("#HFO:0009"));
			return false;
		}
	}

	clearAlarmArray();

	setParaData(ALINK_SETT_VERSION_ACULINK,m_iVersion);

	return bRes;
}

// **************************************************************************
// 
// **************************************************************************
bool CInterfaceAcuLink_SERIAL::deinit()
{
	m_comPort.Close();
	return true;
}

//HINSTANCE CInterfaceAcuLink_SERIAL::isAcuLink()
//{
//	return AfxGetInstanceHandle();
//}

void CInterfaceAcuLink_SERIAL::setGraphData(SHORT iValVolume ,SHORT iValPressure ,SHORT iValFlow, SHORT iValCO2)
{

}



int CInterfaceAcuLink_SERIAL::setMeasurementData(int idx, int iValue)
{
	if ((idx >= 0) && (idx < ALINK_SIZE_MSMNTARRAY)) gMeasurementArray[idx] = iValue;
	return 0;
}

void CInterfaceAcuLink_SERIAL::setParaData(UINT idx, int iValue)
{
	if ((idx >= 0) && (idx < ALINK_SIZE_SETTARRAY)) gSettingArray[idx] = iValue;
}
int CInterfaceAcuLink_SERIAL::getParaData(int idx)
{
	return gSettingArray[idx];
}

void CInterfaceAcuLink_SERIAL::setAlarmData(UINT idx, int iValue)
{
	if ((idx >= 0) && (idx < ALINK_SIZE_ALARMARRAY)) gAlarmArray[idx] = iValue;
}
int CInterfaceAcuLink_SERIAL::getAlarmData(int idx)
{
	return gAlarmArray[idx];
}
int CInterfaceAcuLink_SERIAL::getMeasurementData(int idx)
{
	return gMeasurementArray[idx];
}

int CInterfaceAcuLink_SERIAL::buildValue(char *sResult, int iVal, int iNk, int iLinkType)
{
	//char lsCodeBuf[16];
	char lsValBuf[16];
	//char lsPointBuf[16];

	strcpy(sResult, "");
	switch(iVal)
	{
	case ALINK_UNUSED:
		switch (iLinkType)
		{
		case ACL_RS232_DATA: return -1;
		case ACL_RJ45_DATA: sprintf(sResult, "%d", iVal); break;
		}
		break;

	case ALINK_OUTOFRANGE:
		switch (iLinkType)
		{
		case ACL_RS232_DATA: sprintf(sResult, "_OOR");
		case ACL_RJ45_DATA: sprintf(sResult, "%d", iVal); break;
		}
		break;

	case ALINK_NOTVALID:
		switch (iLinkType)
		{
		case ACL_RS232_DATA: sprintf(sResult, "_NV");
		case ACL_RJ45_DATA: sprintf(sResult, "%d", iVal); break;
		}
		break;

	case ALINK_OFF:
		switch (iLinkType)
		{
		case ACL_RS232_DATA: sprintf(sResult, "_OFF");
		case ACL_RJ45_DATA: sprintf(sResult, "%d", iVal); break;
		}
		break;

		// die Angabe der Nachkommastellen wird dazu benutzt, ein paar Parameter
		// besonders zu behandeln.
	default:
		switch(iNk)
		{
		case -44: // MAC, upper digits
			sprintf(sResult, "%06X", iVal);
			break;

		case -45: // MAC,  lower digits
			sprintf(sResult, "%06X", iVal);
			break;

		case -56: // AcuLink: EXE-Version (x,y), DLL-Version (m,n)
			sprintf(sResult, "%d.%d, %d.%d",
				(iVal >> 24) & 0x000000FF,
				(iVal >> 16) & 0x000000FF,
				(iVal >>  8) & 0x000000FF,
				(iVal & 0x000000FF));
			break;

		default: // Normalfall: setze einen '.' in den String ein
			sprintf(lsValBuf, "%d", iVal);			// Umwandlung in string
			setPoint(lsValBuf, sResult, iNk);	// '.' einsetzen
			break;
		} // switch(i)
		break;
	}

	return strlen(sResult);
}

int CInterfaceAcuLink_SERIAL::setPoint( char *s, char *news, int nk )
{
	int i, j;
	int vz = (s[0]=='-')||(s[0]=='+');
	char Vorz[20]=" ";

	if(nk <= 0)
	{
		strcpy(news, s);
		return 0;
	}

	Vorz[0]=s[0];

	if (vz)     // bei Vorzeichen: VZ merken, entfernen
	{
		for (i=1; s[i]!='\0'; i++)
			s[i-1]=s[i];
		s[i-1]='\0';
	}

	j = 0;
	if( strlen( s ) <=  nk )
	{
		for( i = strlen( s ), j = 0; i < nk + 1; i++ )
		{
			news[ j++ ] = '0';
		}
	}
	news[ j ] = '\0';
	strcat( news, s );
	for( i = strlen( news ), j = 0; j <= nk; i--, j++ )
	{
		news[ i+1 ] = news[ i ];
	}
	news[ i+1 ] = '.';

	if (vz)
	{
		strcat(Vorz, news);
		strcpy(news,Vorz);
	}

	return 1;
}
void CInterfaceAcuLink_SERIAL::initCRC_Table(void)
{
	WORD b, v;
	signed char i;

	for (b = 0; b <= 255; ++b)
	{
		for ( v = b << 8, i = 8; --i >= 0;)
		{
			if ( (v & 0x8000) != 0x0000 )
				v = ( v << 1 ) ^POLYNOMIAL;
			else
				v = v << 1;
		} // for v
		crc_table[b] = v;
	} // for b
}

WORD CInterfaceAcuLink_SERIAL::calculateCRC(unsigned char *msg, WORD  cnt)
{
	WORD crc_value = 0xFFFF;

	while (cnt-- != 0x0000)
		crc_value = (crc_value << 8) ^ crc_table[(crc_value >> 8) ^ *msg++];

	return( crc_value );
}

void CInterfaceAcuLink_SERIAL::sendTestWaveData() 
{
	unsigned char faSendBuf[405];
	//DWORD   fdwSendBufLen;
	DWORD dwBytesWRitten=0;

	/*int i=0;
	for(i = 0; i < 405; i++)
	{
		faSendBuf[i++] = 'x';
	}

	dwBytesWRitten=m_comPort.Write(faSendBuf,405);

	if(dwBytesWRitten<64 && m_bWriteERROR)
	{
		CString sz;
		sz.Format(_T("sendTestData: %d"), dwBytesWRitten);
		theApp.getLog()->WriteLine(sz);
		m_bWriteERROR=false;
	}*/

	int i=0;
	int j=0;
	for(i = 0; i < 7; i++)
	{
		for(j = 0; j < 64; j++)
		{
			faSendBuf[j++] = 'x';
		}

		dwBytesWRitten=m_comPort.Write(faSendBuf,64);

		if(dwBytesWRitten<64 && m_bShowAcuLinkError)
		{
			CString sz;
			sz.Format(_T("sendTestData: %d"), dwBytesWRitten);
			theApp.getLog()->WriteLine(sz);
			m_bShowAcuLinkError=false;
		}
	}

}
int CInterfaceAcuLink_SERIAL::sendMeasurementData()//CAcuLinkApp::BuildMsg()
{
	fillMeasurementData();
	
	unsigned char faSendBuf[4096];
	DWORD   fdwSendBufLen = 0;
	DWORD dwBytesWRitten=0;
	int i;
	char ls[16];
	// int liBufLen;
	int liVal;
	char lsCodeBuf[16];
	// char lsValBuf[16];
	char lsPointBuf[16];
	int idx;
	int liStartDataIdx = 0;
	int liEndDataIdx = 0;
	int liDataLen = 0;
	WORD lwCrc = 0x0000;

	idx = 0;

	// Startzeichen
	faSendBuf[idx++] = SOH_CHAR;

	// Platzhalter für die Länge, wird später eingetragen
	for(i = 0; i < 5; i++)
	{
		faSendBuf[idx++] = 'x';
	}

	faSendBuf[idx++] = STX_CHAR;
	liStartDataIdx = idx; // erstes Zeichen nach STX !

	for (i = 0; i < ALINK_MSMNT_ARRAY; i++)
	{
		liVal = getMeasurementData(i);            // hole den i-ten Parameter
		if (buildValue(lsPointBuf, liVal, gMeasurementNkArray[i], fiLinkType) >= 0)
		{
			sprintf(lsCodeBuf, "%02d=%s", i, lsPointBuf);
			memcpy(&(faSendBuf[idx]), lsCodeBuf, strlen(lsCodeBuf));
			idx += strlen(lsCodeBuf);
			faSendBuf[idx++] = '|';
		}
	} // for i 

	for (i = 0; i < ALINK_SETT_ARRAY; i++)
	{
		liVal = getParaData(i);            // hole den i-ten Parameter
		if (i == 5)
		{
			int kk = 2;
		}
		if (buildValue(lsPointBuf, liVal, gSettingNkArray[i], fiLinkType) >= 0)
		{
			sprintf(lsCodeBuf, "%03d=%s", i+100, lsPointBuf);
			memcpy(&(faSendBuf[idx]), lsCodeBuf, strlen(lsCodeBuf));
			idx += strlen(lsCodeBuf);
			faSendBuf[idx++] = '|';
		}
	} // for i 

	for (i = 0; i < ALINK_ALARM_ARRAY; i++)
	{
		liVal = getAlarmData(i);            // hole den i-ten Parameter
		// bei Alarmen gibt es kein Nachkomma-Stellen
		if (buildValue(lsPointBuf, liVal, -1, fiLinkType) >= 0)
		{
			sprintf(lsCodeBuf, "%03d=%s", i+200, lsPointBuf);
			memcpy(&(faSendBuf[idx]), lsCodeBuf, strlen(lsCodeBuf));
			idx += strlen(lsCodeBuf);
			faSendBuf[idx++] = '|';
		}
	} // for i 

	// am Ende steht zur Zeit ein (überflüssiges) '|'
	if (faSendBuf[idx-1] == '|')
		idx--;

	faSendBuf[idx++] = ETX_CHAR; // idx bei erstem Zeichen nach ETX !
	liEndDataIdx = idx;

	// Framebody mit den Daten ist fertig
	// fürs Empfangen ...
	//        Position von ETX checken:    (pos(STX) + 1) + liDataLen 
	//                            oder:    pos(STX) + liDataLen + 1
	liDataLen = liEndDataIdx - liStartDataIdx - 1;    
	sprintf(ls, "%05d", liDataLen);
	memcpy(&(faSendBuf[1]), ls, strlen(ls));    

	// Prüfsumme: CRC-16 (nach CCITT)
	lwCrc = calculateCRC(&(faSendBuf[liStartDataIdx]), liDataLen);
	sprintf(ls, "%04X", lwCrc);
	memcpy(&(faSendBuf[idx]), ls, strlen(ls));    
	idx += strlen(ls);

	faSendBuf[idx++] = EOT_CHAR;

	// TEST TEST TEST
	// sprintf(__msg, "BuildMsg: End, idx=%d", idx);
	// theApp.pLogMsg(__msg);

	int iCount=0;
	if(idx>=64)
	{
		for(iCount = 0; iCount+64 <= idx; )
		{
			dwBytesWRitten=m_comPort.Write(&(faSendBuf[iCount]),64);

			if(dwBytesWRitten!=64)
			{
				CString sz;
				sz.Format(_T("sendAcuLink: %d"), dwBytesWRitten);
				theApp.getLog()->WriteLine(sz);
			}
			iCount+=64;
		}
		int iRest=idx-iCount;
		if(iRest>0)
		{
			dwBytesWRitten=m_comPort.Write(&(faSendBuf[iCount]),iRest);

			if(dwBytesWRitten!=iRest)
			{
				CString sz;
				sz.Format(_T("sendAcuLink2: %d"), dwBytesWRitten);
				theApp.getLog()->WriteLine(sz);
			}
		}
	}
	else
	{

	}

	/*theApp.getLog()->WriteLine(_T("1"));
	dwBytesWRitten=m_comPort.Write(faSendBuf,idx);
	theApp.getLog()->WriteLine(_T("2"));

	if(dwBytesWRitten<idx)
	{
		CString sz;
		sz.Format(_T("sendTestData: %d"), dwBytesWRitten);
		theApp.getLog()->WriteLine(sz);
	}*/

	//for(i = 0; i <= idx; i++)
	//{
	//	/*if(idx>=64)
	//	{

	//	}*/
	//	dwBytesWRitten=m_comPort.Write(faSendBuf,1);

	//	if(dwBytesWRitten==0)
	//	{
	//		CString sz;
	//		sz.Format(_T("sendTestData: %d"), dwBytesWRitten);
	//		theApp.getLog()->WriteLine(sz);
	//	}
	//}

	//dwBytesWRitten=m_comPort.Write(faSendBuf,405);

	//return idx;

	return 1;
}

// ****************************************************************************

/*
 *	Idx			Zeichen
 *	  0			ESC-CHAR (Startzeichen)
 *	  1 .. 4	CRC (hex)
 *	  5 .. 7    Länge (dezimal)
 *	  8 ....	Daten
*/

#define IDX_START		0
#define IDX_CRC			1
#define IDX_LEN			3
#define HEADER_LEN 		5

//int CInterfaceAcuLink_SERIAL::sendWaveData()//CAcuLinkApp::BuildAdMsg()
//{
//	int i;
//	char ls[32];
//	int idx;
//	WORD _wCrc = 0x0000;
//	int _iError = 0;
//	int _iWaveIdx = 0;
//	int _wDataLen = 0;
//	DllWaveRecord _oWaveSample;
//	//unsigned char faSendBuf[1200];
//	unsigned char faSendBuf[4096];
//	DWORD   fdwSendBufLen = 0;
//
//	//serial.fdwSendBufLen = 0;
//	idx = 0;
//
//	// Platzhalter für den Header einrichten
//	// werden später eingetragen
//	for (i = 0; i < HEADER_LEN; i++)
//	{
//		faSendBuf[idx++] = 'x';
//	}
//
//	// Startzeichen
//	faSendBuf[IDX_START] = ESC_CHAR;
//
//	idx = HEADER_LEN;  // 0 basiert !!
//
//	i = 0;
//
//	// lese 1 Sample aus dem SampleArray
//	_iError = pGetSingleWaveRecordFunc(&(_oWaveSample));
//	while (_iError == 0)
//	{
//		i++;
//
//		// hänge die wave-samples an den Sendebuffer
//		for (_iWaveIdx = 0; _iWaveIdx < MAX_WAVES; _iWaveIdx++)
//		{	
//			if (_iError == 0)
//			{
//				faSendBuf[idx++] = (unsigned char)((_oWaveSample.SampleWaves[_iWaveIdx] >> 8) & 0x00FF);
//				faSendBuf[idx++] = (unsigned char)(_oWaveSample.SampleWaves[_iWaveIdx] & 0x00FF);
//			}
//			else
//			{
//				if (idx > 2) // ... nicht den Buffer kaputt machen ...
//				{
//					faSendBuf[idx] = faSendBuf[idx-2];
//					idx++;
//					faSendBuf[idx] = faSendBuf[idx-2];
//					idx++;
//				}
//			}
//		} // for (_iWaveIdx ...)
//
//		// hole nächsten Satz Samples :)
//		_iError = pGetSingleWaveRecordFunc(&(_oWaveSample));
//	} // while
//
//	//sprintf(ls, "%d entries", i);
//	//pLogMsg(ls);
//
//	// Länge eintragen
//	_wDataLen = idx - HEADER_LEN;
//	faSendBuf[IDX_LEN] = (unsigned char)((_wDataLen >> 8) & 0x00FF);
//	faSendBuf[IDX_LEN + 1] = (unsigned char)(_wDataLen & 0x00FF);
//	//sprintf(ls, "%02X", idx);
//	//memcpy(&(serial.faSendBuf[IDX_LEN]), ls, strlen(ls));	
//
//	// Prüfsumme: CRC-16 (nach CCITT)
//	_wCrc = calculateCRC(&(faSendBuf[IDX_LEN + 2]), idx);
//	faSendBuf[IDX_CRC] = (unsigned char)((_wCrc >> 8) & 0x00FF);
//	faSendBuf[IDX_CRC + 1] = (unsigned char)(_wCrc & 0x00FF);
//	//sprintf(ls, "%04X", lwCrc);
//	//memcpy(&(serial.faSendBuf[IDX_CRC]), ls, strlen(ls));	
//
//	return idx;
//}

//int CInterfaceAcuLink_SERIAL::setWaveRecord(short shVol, short shPress, short shFlow, short shCO2) //CAcuLink_DLLApp::SetWaveRecord(...)
//{
//	AFX_MANAGE_STATE(AfxGetStaticModuleState());
//
//	if (gbWriteGo == false)
//		return ACL_ERR_WAVE_WRITE_NOT_STARTED;
//
//	gwWaveRecordsWriteIdx++;
//	if (gwWaveRecordsWriteIdx >= gwMaxWaveRecords)
//	{
//		gwWaveRecordsWriteIdx = 0;
//	}
//
//	if (gwWaveRecordsWriteIdx == gwWaveRecordsReadIdx)
//	{
//		theApp.fiAcuLinkError = 0; //ACU_LINK_ERROR_BUFFER_OVERFLOW;
//		LogMsg("WI reaches RI !!");
//		return 0;
//	}
//
//	if (gbReadGo == false)
//	{
//		if (gwWaveRecordsWriteIdx == gwWriteAheadIdx)
//		{
//			// Schwelle zum Starten des Auslesens erreicht
//			gbReadGo = true;
//			LogMsg("ReadGo = true");
//		}
//	}
//
//	// Index richtig, und speichern
//	gDllWaveRecords[gwWaveRecordsWriteIdx].SampleWaves[WAVE_VOL]   = shVol;
//	gDllWaveRecords[gwWaveRecordsWriteIdx].SampleWaves[WAVE_PRESS] = shPress;
//	gDllWaveRecords[gwWaveRecordsWriteIdx].SampleWaves[WAVE_FLOW]  = shFlow;
//	gDllWaveRecords[gwWaveRecordsWriteIdx].SampleWaves[WAVE_CO2]   = shCO2;
//
//	return 0;
//}

// ****************************************************************************

//int CInterfaceAcuLink_SERIAL::getWaveRecord(DllWaveRecord p[MAX_SAMPLES])//CAcuLink_DLLApp::GetWaveRecord(...)
//{
//	int i;
//	int _ccSample;
//	WORD _wDist;
//	WORD _wStep;
//
//	AFX_MANAGE_STATE(AfxGetStaticModuleState());
//
//	for( i = 0; i < MAX_SAMPLES; i++ )
//	{
//		p[i].SampleWaves[WAVE_VOL] = 0;
//		p[i].SampleWaves[WAVE_PRESS] = 0;
//		p[i].SampleWaves[WAVE_FLOW] = 0;
//		p[i].SampleWaves[WAVE_CO2] = 0;
//	}
//	_wStep = 0;
//
//	if (gbReadGo == false)
//		return 0;
//
//	// Schrittweite aus Distanz wi und ri neu berechnen
//	_wStep = 1;
//	/*
//	if( gwWaveRecordsWriteIdx > gwWaveRecordsReadIdx )
//	{
//		_wDist = (gwWaveRecordsWriteIdx - gwWaveRecordsReadIdx) / MAX_SAMPLES;
//	}
//	else
//	{
//		_wDist = (gwMaxWaveRecords - (gwWaveRecordsReadIdx - gwWaveRecordsWriteIdx)) / MAX_SAMPLES;
//	}
//	*/
//	
//	if( gwWaveRecordsWriteIdx > gwWaveRecordsReadIdx )
//	{
//		_wDist = gwWaveRecordsWriteIdx - gwWaveRecordsReadIdx;
//	}
//	else
//	{
//		_wDist = (gwMaxWaveRecords - (gwWaveRecordsReadIdx - gwWaveRecordsWriteIdx));
//	}
//
//	if( _wDist > gwDistance )
//	{
//		_wStep = 2;
//	}
//
//	if( _wDist < MAX_SAMPLES )
//	{
//		_wStep = 0;
//	}
//
//	// lese 4 Samples aller Waves, wenn mindestens 1 Wave angefragt wird
//	// Samples liegen für jede ms vor, ich brauche Werte für 32ms, 
//	// also nehme ich jedes 8. Sample aus dem Samplearray
//	for( _ccSample = 0; _ccSample < MAX_SAMPLES; _ccSample++)
//	{
//		for (i = 0; i < _wStep; i++)
//		{
//			gwWaveRecordsReadIdx++;
//			if (gwWaveRecordsReadIdx >= gwMaxWaveRecords)
//			{
//				gwWaveRecordsReadIdx = 0;
//			}
//
//			gwWriteAheadIdx++;
//			if (gwWriteAheadIdx >= gwMaxWaveRecords)
//			{
//				gwWriteAheadIdx = 0;
//			}
//
//			if (gwWaveRecordsReadIdx == gwWaveRecordsWriteIdx)
//			{
//				gbReadGo = false; // wieder warten bis WriteAhead erreicht ist!
//				LogMsg("RI reaches WI => ReadGo = false => WriteAhead");
//
//				theApp.fiAcuLinkError = ACL_ERR_WAVE_BUFFER_UNDERRUN;
//				return -1;
//			}
//		} // for i
//
//		// Index stimmt, fülle Rückgabestruktur
//		p[_ccSample].SampleWaves[WAVE_VOL]		= gDllWaveRecords[gwWaveRecordsReadIdx].SampleWaves[WAVE_VOL];
//		p[_ccSample].SampleWaves[WAVE_PRESS]	= gDllWaveRecords[gwWaveRecordsReadIdx].SampleWaves[WAVE_PRESS];
//		p[_ccSample].SampleWaves[WAVE_FLOW]		= gDllWaveRecords[gwWaveRecordsReadIdx].SampleWaves[WAVE_FLOW];
//		p[_ccSample].SampleWaves[WAVE_CO2]		= gDllWaveRecords[gwWaveRecordsReadIdx].SampleWaves[WAVE_CO2];
//	} // for _ccSample
//
//	return 0;
//}