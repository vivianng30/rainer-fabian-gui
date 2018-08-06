/**********************************************************************************************//**
 * \file	MVEvent.cpp.
 *
 * Implements the mv event class
 **************************************************************************************************/

#include "StdAfx.h"
#include "MVEvent.h"

/**********************************************************************************************//**
 * Initializes a new instance of the CMVEvent class
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	ET	The et.
 **************************************************************************************************/

CMVEvent::CMVEvent(eET ET)
: m_ET(ET)
{

}

/**********************************************************************************************//**
 * Finalizes an instance of the CMVEvent class
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

CMVEvent::~CMVEvent(void)
{
}

/**********************************************************************************************//**
 * Get
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	A CMVEvent::eET.
 **************************************************************************************************/

CMVEvent::eET CMVEvent::GetET()
{
	return m_ET;
}



//##########################UI Events#####################################

/**********************************************************************************************//**
 * \brief Konstruktor für UI-Events
 * 
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	EventType	Type of the event.
 **************************************************************************************************/

CMVEventUI::CMVEventUI(int EventType)
: CMVEvent(ET_UIEVENT), m_EventType(EventType)
{
	m_bState=false;
	m_iData=0;
}

/**********************************************************************************************//**
 * \brief Konstruktor für UI-Events mit sonstigen bool-States
 * 
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	EventType	Type of the event.
 * \param	bState   	True to state.
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * \brief Konstruktor für UI-Events mit int-Datum
 * 
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	EventType	Type of the event.
 * \param	iData	 	The data.
 **************************************************************************************************/

CMVEventUI::CMVEventUI(int EventType, int iData)
: CMVEvent(ET_UIEVENT), m_EventType(EventType), m_iData(iData)
{
	m_bState=false;
}

/**********************************************************************************************//**
 * \brief Destruktor
 * 
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

CMVEventUI::~CMVEventUI()
{
}

/**********************************************************************************************//**
 * \brief UI-Event-Typ
 * 
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The event type.
 **************************************************************************************************/

int CMVEventUI::GetEventType()
{
	return m_EventType;
}

/**********************************************************************************************//**
 * \brief Nur bei Events mit States
 * 
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * \brief Nur bei Events mit Int-Datum gültig
 * 
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The i data.
 **************************************************************************************************/

int CMVEventUI::GetIData()
{
	return m_iData;
}


//##########################Matrix Events#####################################

/**********************************************************************************************//**
 * \brief Konstruktor für UI-Events
 * 
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	EventType	Type of the event.
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * \brief Destruktor
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

CMVEventMatrix::~CMVEventMatrix()
{
}

/**********************************************************************************************//**
 * \brief UI-Event-Typ
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The event type.
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * \brief Konstruktor für UI-Events
 * 
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	EventType	Type of the event.
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * \brief Destruktor
 * 
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

CMVEventControl::~CMVEventControl()
{
}

/**********************************************************************************************//**
 * \brief UI-Event-Typ
 * 
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The event type.
 **************************************************************************************************/

int CMVEventControl::GetEventType()
{
	return m_EventType;
}





//##########################System-Alarms#####################################

/**********************************************************************************************//**
 * \brief Konstruktor für UI-Events
 * 
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	EventType	Type of the event.
 **************************************************************************************************/

CMVEventAlarm::CMVEventAlarm(int EventType)
: CMVEvent(ET_ALARM), m_EventType(EventType)
{
}

/**********************************************************************************************//**
 * \brief Konstruktor für UI-Events mit sonstigen String-Daten
 * 
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	EventType	Type of the event.
 * \param	sData	 	The data.
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * \brief Destruktor
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

CMVEventAlarm::~CMVEventAlarm()
{
}

/**********************************************************************************************//**
 * \brief UI-Event-Typ
 * 
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The event type.
 **************************************************************************************************/

int CMVEventAlarm::GetEventType()
{
	return m_EventType;
}

/**********************************************************************************************//**
 * \brief Nur bei Events mit String-Daten gültig
 * 
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The data.
 **************************************************************************************************/

CStringW CMVEventAlarm::GetData()
{
	return m_sData;
}



//##########################Infotext#####################################

/**********************************************************************************************//**
 * \brief Konstruktor für UI-Events
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	EventType	Type of the event.
 **************************************************************************************************/

CMVEventInfotext::CMVEventInfotext(int EventType)
: CMVEvent(ET_INFOTEXT), m_EventType(EventType)
{
	m_iData=0;
}

/**********************************************************************************************//**
 * \brief Konstruktor für UI-Events mit sonstigen String-Daten
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	EventType	Type of the event.
 * \param	sData	 	The data.
 * \param	iData	 	The data.
 **************************************************************************************************/

CMVEventInfotext::CMVEventInfotext(int EventType, CStringW sData, int iData)
: CMVEvent(ET_INFOTEXT), m_EventType(EventType), m_sData(sData), m_iData(iData)
{
}

/**********************************************************************************************//**
 * \brief Destruktor
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

CMVEventInfotext::~CMVEventInfotext()
{
}

/**********************************************************************************************//**
 * \brief UI-Event-Typ
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The event type.
 **************************************************************************************************/

int CMVEventInfotext::GetEventType()
{
	return m_EventType;
}

/**********************************************************************************************//**
 * \brief Nur bei Events mit String-Daten gültig
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The data.
 **************************************************************************************************/

CStringW CMVEventInfotext::GetData()
{
	return m_sData;
}

/**********************************************************************************************//**
 * \brief Nur bei Events mit Int-Datum gültig
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The i data.
 **************************************************************************************************/

int CMVEventInfotext::GetIData()
{
	return m_iData;
}


//##########################Data Events#####################################

/**********************************************************************************************//**
 * \brief Konstruktor für UI-Events
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	EventType	Type of the event.
 **************************************************************************************************/

CMVEventData::CMVEventData(int EventType)
: CMVEvent(ET_DATAEVENT), m_EventType(EventType)
{
}

/**********************************************************************************************//**
 * \brief Destruktor
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

CMVEventData::~CMVEventData()
{
}

/**********************************************************************************************//**
 * \brief UI-Event-Typ
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The event type.
 **************************************************************************************************/

int CMVEventData::GetEventType()
{
	return m_EventType;
}

