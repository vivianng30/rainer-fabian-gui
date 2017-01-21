#include "StdAfx.h"
#include "MVEvent.h"

CMVEvent::CMVEvent(eET ET)
: m_ET(ET)
{

}

CMVEvent::~CMVEvent(void)
{
}

CMVEvent::eET CMVEvent::GetET()
{
	return m_ET;
}



//##########################UI Events#####################################


//=============================================================================
//
//\brief Konstruktor für UI-Events
//
//=============================================================================
CMVEventUI::CMVEventUI(int EventType)
: CMVEvent(ET_UIEVENT), m_EventType(EventType)
{
	m_bState=false;
	m_iData=0;
}

//=============================================================================
//
//\brief Konstruktor für UI-Events mit sonstigen bool-States
//
//=============================================================================
CMVEventUI::CMVEventUI(int EventType, bool bState)
: CMVEvent(ET_UIEVENT), m_EventType(EventType), m_bState(bState)
{
	m_iData=0;
}


//=============================================================================
//
//\brief Konstruktor für UI-Events mit sonstigen String-Daten
//
//=============================================================================
//CMVEventUI::CMVEventUI(int EventType, CStringW sData)
//: CMVEvent(ET_UIEVENT), m_EventType(EventType), m_sData(sData)
//{
//}

//=============================================================================
//
//\brief Konstruktor für UI-Events mit int-Datum
//
//=============================================================================
CMVEventUI::CMVEventUI(int EventType, int iData)
: CMVEvent(ET_UIEVENT), m_EventType(EventType), m_iData(iData)
{
	m_bState=false;
}

//=============================================================================
//
//\brief Destruktor
//
//=============================================================================
CMVEventUI::~CMVEventUI()
{
}

//=============================================================================
//
//\brief UI-Event-Typ
//
//=============================================================================
int CMVEventUI::GetEventType()
{
	return m_EventType;
}

//=============================================================================
//
//\brief Nur bei Events mit States
//
//=============================================================================
bool CMVEventUI::GetState()
{
	return m_bState;
}

//=============================================================================
//
//\brief Nur bei Events mit String-Daten gültig
//
//=============================================================================
//CStringW CMVEventUI::GetData()
//{
//	return m_sData;
//}

//=============================================================================
//
//\brief Nur bei Events mit Int-Datum gültig
//
//=============================================================================
int CMVEventUI::GetIData()
{
	return m_iData;
}


//##########################Matrix Events#####################################

//=============================================================================
//
//\brief Konstruktor für UI-Events
//
//=============================================================================
CMVEventMatrix::CMVEventMatrix(int EventType)
: CMVEvent(ET_MATRIX), m_EventType(EventType)
{
}


//=============================================================================
//
//\brief Konstruktor für UI-Events mit sonstigen String-Daten
//
//=============================================================================
//CMVEventMatrix::CMVEventMatrix(int EventType, CStringW sData)
//: CMVEvent(ET_MATRIX), m_EventType(EventType), m_sData(sData)
//{
//}

//=============================================================================
//
//\brief Konstruktor für UI-Events mit int-Datum
//
//=============================================================================
//CMVEventMatrix::CMVEventMatrix(int EventType, int iData)
//: CMVEvent(ET_MATRIX), m_EventType(EventType), m_iData(iData)
//{
//}

//=============================================================================
//
//\brief Destruktor
//
//=============================================================================
CMVEventMatrix::~CMVEventMatrix()
{
}

//=============================================================================
//
//\brief UI-Event-Typ
//
//=============================================================================
int CMVEventMatrix::GetEventType()
{
	return m_EventType;
}



//=============================================================================
//
//\brief Nur bei Events mit String-Daten gültig
//
//=============================================================================
//CStringW CMVEventMatrix::GetData()
//{
//	return m_sData;
//}

//=============================================================================
//
//\brief Nur bei Events mit Int-Datum gültig
//
//=============================================================================
//int CMVEventMatrix::GetIData()
//{
//	return m_iData;
//}



//##########################Control Events#####################################

//=============================================================================
//
//\brief Konstruktor für UI-Events
//
//=============================================================================
CMVEventControl::CMVEventControl(int EventType)
: CMVEvent(ET_CONTROL), m_EventType(EventType)
{
}


//=============================================================================
//
//\brief Konstruktor für UI-Events mit sonstigen String-Daten
//
//=============================================================================
//CMVEventControl::CMVEventControl(int EventType, CStringW sData)
//: CMVEvent(ET_CONTROL), m_EventType(EventType), m_sData(sData)
//{
//}

//=============================================================================
//
//\brief Konstruktor für UI-Events mit int-Datum
//
//=============================================================================
//CMVEventControl::CMVEventControl(int EventType, int iData)
//: CMVEvent(ET_CONTROL), m_EventType(EventType), m_iData(iData)
//{
//}

//=============================================================================
//
//\brief Destruktor
//
//=============================================================================
CMVEventControl::~CMVEventControl()
{
}

//=============================================================================
//
//\brief UI-Event-Typ
//
//=============================================================================
int CMVEventControl::GetEventType()
{
	return m_EventType;
}





//##########################System-Alarms#####################################

//=============================================================================
//
//\brief Konstruktor für UI-Events
//
//=============================================================================
CMVEventAlarm::CMVEventAlarm(int EventType)
: CMVEvent(ET_ALARM), m_EventType(EventType)
{
}


//=============================================================================
//
//\brief Konstruktor für UI-Events mit sonstigen String-Daten
//
//=============================================================================
CMVEventAlarm::CMVEventAlarm(int EventType, CStringW sData)
: CMVEvent(ET_ALARM), m_EventType(EventType), m_sData(sData)
{
}

//=============================================================================
//
//\brief Konstruktor für UI-Events mit int-Datum
//
//=============================================================================
//CMVEventUI::CMVEventUI(int EventType, int iData)
//: CMVEvent(ET_UIEVENT), m_EventType(EventType), m_iData(iData)
//{
//}

//=============================================================================
//
//\brief Destruktor
//
//=============================================================================
CMVEventAlarm::~CMVEventAlarm()
{
}

//=============================================================================
//
//\brief UI-Event-Typ
//
//=============================================================================
int CMVEventAlarm::GetEventType()
{
	return m_EventType;
}



//=============================================================================
//
//\brief Nur bei Events mit String-Daten gültig
//
//=============================================================================
CStringW CMVEventAlarm::GetData()
{
	return m_sData;
}



//##########################Infotext#####################################

//=============================================================================
//
//\brief Konstruktor für UI-Events
//
//=============================================================================
CMVEventInfotext::CMVEventInfotext(int EventType)
: CMVEvent(ET_INFOTEXT), m_EventType(EventType)
{
	m_iData=0;
}


//=============================================================================
//
//\brief Konstruktor für UI-Events mit sonstigen String-Daten
//
//=============================================================================
CMVEventInfotext::CMVEventInfotext(int EventType, CStringW sData, int iData)
: CMVEvent(ET_INFOTEXT), m_EventType(EventType), m_sData(sData), m_iData(iData)
{
}


//=============================================================================
//
//\brief Destruktor
//
//=============================================================================
CMVEventInfotext::~CMVEventInfotext()
{
}

//=============================================================================
//
//\brief UI-Event-Typ
//
//=============================================================================
int CMVEventInfotext::GetEventType()
{
	return m_EventType;
}



//=============================================================================
//
//\brief Nur bei Events mit String-Daten gültig
//
//=============================================================================
CStringW CMVEventInfotext::GetData()
{
	return m_sData;
}

//=============================================================================
//
//\brief Nur bei Events mit Int-Datum gültig
//
//=============================================================================
int CMVEventInfotext::GetIData()
{
	return m_iData;
}


//##########################Data Events#####################################

//=============================================================================
//
//\brief Konstruktor für UI-Events
//
//=============================================================================
CMVEventData::CMVEventData(int EventType)
: CMVEvent(ET_DATAEVENT), m_EventType(EventType)
{
}



//=============================================================================
//
//\brief Destruktor
//
//=============================================================================
CMVEventData::~CMVEventData()
{
}

//=============================================================================
//
//\brief UI-Event-Typ
//
//=============================================================================
int CMVEventData::GetEventType()
{
	return m_EventType;
}

