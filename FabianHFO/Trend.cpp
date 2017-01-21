// TrendData.cpp : implementation file
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "Trend.h"

//CircularBuffer<BYTE> CTrend::m_rbufByteTrend(MAXSIZE_TREND_BUFFER);
//CircularBuffer<BYTE> CTrend::m_rbufByteTrend(512);

//#define IDS_CONFIG_DATA_FILE _T("\\FFSDISK\\test1.dat")

IMPLEMENT_SERIAL(CTrend,CObject,1)
// CTrend
CTrend::CTrend()
{
	m_dtTimestamp.SetStatus(COleDateTime::null);

	//WORD size = sizeof(m_dtTimestamp);
	//WORD size = sizeof(COleDateTime);

	m_wBufCount=0;
	//m_wBuffer= new WORD[SERIALZEBUFFER];

	for(int i=0;i<SERIALZEBUFFER;i++)
	{
		m_wBuffer[i]=0;
	}

}
CTrend::~CTrend(void)
{
	if (m_wBuffer != NULL) {
		delete [] m_wBuffer;
		//m_wBuffer=NULL;
	}
}

void CTrend::Serialize(CArchive& ar)
{
	CObject::Serialize(ar);
	if(ar.IsStoring())
	{
		/*WORD size = sizeof(m_dtTimestamp);
		size += sizeof(m_wBufCount);
		size += sizeof(m_wBuffer);*/

		//DEBUGMSG(TRUE, (TEXT("Serialize m_dtTimestamp %d\r\n"),m_dtTimestamp));

		ar<<m_dtTimestamp;
		ar<<m_wBufCount;
		
		for(int i=0;i<SERIALZEBUFFER;i++)
		{
			ar<<m_wBuffer[i];
			//m_wBuffer[i]=0;
		}

		//m_wBufCount=0;
	}
	else
	{
		ar>>m_dtTimestamp;
		ar>>m_wBufCount;

		for(int i=0;i<SERIALZEBUFFER;i++)
		{
			ar>>m_wBuffer[i];
		}
	}
}




WORD CTrend::WriteBuffer(WORD val, COleDateTime dtTimestamp)
{
	if(m_wBufCount>=SERIALZEBUFFER)
	{
		//ERROR sollte nie vorkommen, da m_wBufCount bei >=SERIALZEBUFFER serialisiert werden sollte (DataHandler) und dabei auf 0 gesetzt wird
		m_wBufCount=0;
	}

	m_wBuffer[m_wBufCount]=val;
	m_wBufCount++;

	SetTimestamp(dtTimestamp);
	
	return m_wBufCount;
}

WORD CTrend::GetBufferValue(UINT pos)
{
	return m_wBuffer[pos];
}

COleDateTime CTrend::GetTimestamp()
{
	//CStringW szTime = m_dtTimestamp.Format(LOCALE_NOUSEROVERRIDE,LANG_USER_DEFAULT);
	return m_dtTimestamp;
}
//void CTrend::SetTimestamp()
//{
//	SYSTEMTIME st;
//	GetLocalTime(&st);
//	m_dtTimestamp=COleDateTime(st);
//
//	//m_dtTimestamp= COleDateTime::GetCurrentTime();
//
//	//CStringW szTime = dtTest.Format(LOCALE_NOUSEROVERRIDE,LANG_USER_DEFAULT);
//	CStringW szTime2 = m_dtTimestamp.Format(LOCALE_NOUSEROVERRIDE,LANG_USER_DEFAULT);
//	DEBUGMSG(TRUE, (TEXT("SetTimestamp1 %s\r\n"),szTime2));
//}
void CTrend::SetTimestamp(COleDateTime timestamp)
{
	m_dtTimestamp=timestamp;
	//CStringW szTime = m_dtTimestamp.Format(LOCALE_NOUSEROVERRIDE,LANG_USER_DEFAULT);
	//DEBUGMSG(TRUE, (TEXT("SetTimestamp2 %s\r\n"),szTime));
}


WORD CTrend::GetBufferCount()
{
	return m_wBufCount;
}
void CTrend::SetBufferCount(WORD count)
{
	m_wBufCount=count;
}

void CTrend::ResetBuffer()
{
	m_wBufCount=0;

	for(int i=0;i<SERIALZEBUFFER;i++)
	{
		m_wBuffer[i]=0;
	}
}

//void CTrend::SerializeTrend(CFile* pFile,UINT nMode)
//{
//	CFile archivDatei;
//	archivDatei.Open(_T("\\FFSDISK\\archiv.dat"),CFile::modeCreate | CFile::modeWrite);
//
//	CArchive objektArchiv(&archivDatei, CArchive::store);
//
//	if(objektArchiv.IsStoring())
//	{
//		objektArchiv << m_cTendPinsp;
//	}
//	else
//	{
//
//	}
//
//	
//
//	objektArchiv.Close();
//	archivDatei.Close();
//
//}
//void CTrend::DeserializeTrend()
//{
//	CFile archivDatei;
//	archivDatei.Open(_T("\\FFSDISK\\archiv.dat"),CFile::modeRead);
//
//	CArchive objektArchiv(&archivDatei, CArchive::load);
//
//	objektArchiv >> m_cTendPinsp;
//
//	objektArchiv.Close();
//	archivDatei.Close();
//
//}


