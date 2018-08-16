// TrendData.cpp : implementation file
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "Trend.h"

#include <stdio.h>
#include <stdlib.h>

#include <time.h>

#include <iostream>

/**********************************************************************************************//**
 * Initializes a new instance of the Trend class
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	parameter1	The first parameter.
 * \param	parameter2	The second parameter.
 * \param	parameter3	The third parameter.
 **************************************************************************************************/

//IMPLEMENT_SERIAL(CTrend,CObject,1)

/**********************************************************************************************//**
 * CTrend
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

CTrend::CTrend()
{
	m_stTrendData.m_dtTimestamp.SetStatus(COleDateTime::null);

	//m_wBufCount=0;
	m_stTrendData.wBufCount=0;

	for(int i=0;i<SERIALZEBUFFER;i++)
	{
		//m_wBuffer[i]=0;
		m_stTrendData.wBuffer[i]=0;
	}

}

/**********************************************************************************************//**
 * Finalizes an instance of the CTrend class
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

CTrend::~CTrend(void)
{
	//if (m_wBuffer != NULL) {
	//	delete [] m_wBuffer;
	//	//m_wBuffer=NULL;
	//}
}

/**********************************************************************************************//**
 * Serialize this instance to the given stream
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param [in,out]	ar	The archive.
 **************************************************************************************************/

//void CTrend::Serialize(CArchive& ar)
//{
//	CObject::Serialize(ar);
//	if(ar.IsStoring())
//	{
//		//DEBUGMSG(TRUE, (TEXT("Serialize m_dtTimestamp %d\r\n"),m_dtTimestamp));
//
//		ar<<m_dtTimestamp;
//		ar<<m_wBufCount;
//		
//		for(int i=0;i<SERIALZEBUFFER;i++)
//		{
//			ar<<m_wBuffer[i];
//			//m_wBuffer[i]=0;
//		}
//
//		//m_wBufCount=0;
//	}
//	else
//	{
//		ar>>m_dtTimestamp;
//		ar>>m_wBufCount;
//
//		for(int i=0;i<SERIALZEBUFFER;i++)
//		{
//			ar>>m_wBuffer[i];
//		}
//	}
//}


bool CTrend::Serialize(CString szFile, eTrendSerialize state, UINT type)
{
	bool bRes=false;
	if(TREND_READ==state)
	{
		bRes=read(szFile);

		//if(TREND_PINSP==type)
		//{
		//	//TEST
		//	for(int i=0;i<SERIALZEBUFFER;i++)
		//	{
		//		DEBUGMSG(TRUE, (TEXT("trend buffer read i:%d val:%d\r\n"),i,m_stTrendData.wBuffer[i]));
		//	}
		//	DEBUGMSG(TRUE, (TEXT("trend buffer read count:%d time:%02d.%02d.%04d %02d:%02d:%02d\r\n"),m_stTrendData.wBufCount,
		//		m_stTrendData.m_dtTimestamp.GetDay(),
		//		m_stTrendData.m_dtTimestamp.GetMonth(),
		//		m_stTrendData.m_dtTimestamp.GetYear(),
		//		m_stTrendData.m_dtTimestamp.GetHour(),
		//		m_stTrendData.m_dtTimestamp.GetMinute(),
		//		m_stTrendData.m_dtTimestamp.GetSecond()));

		//}
		
	}
	else// TREND_WRITE
	{
		bRes=write(szFile);

		//if(TREND_PINSP==type)
		//{
		//	//TEST
		//	for(int i=0;i<SERIALZEBUFFER;i++)
		//	{
		//		DEBUGMSG(TRUE, (TEXT("trend buffer write i:%d val:%d\r\n"),i,m_stTrendData.wBuffer[i]));
		//	}

		//	DEBUGMSG(TRUE, (TEXT("trend buffer write count:%d time:%02d.%02d.%04d %02d:%02d:%02d\r\n"),m_stTrendData.wBufCount,
		//		m_stTrendData.m_dtTimestamp.GetDay(),
		//		m_stTrendData.m_dtTimestamp.GetMonth(),
		//		m_stTrendData.m_dtTimestamp.GetYear(),
		//		m_stTrendData.m_dtTimestamp.GetHour(),
		//		m_stTrendData.m_dtTimestamp.GetMinute(),
		//		m_stTrendData.m_dtTimestamp.GetSecond()));

		//}
		
	}
	return bRes;
}

//std::string CTrend::currentDateTime()
//{
//	time_t now = time(0);
//	struct tm tstruct;
//	char buf[80];
//	tstruct = *localtime(&now);
//	strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
//
//	return buf;
//}

bool CTrend::write(CString szFile)
{
	CStringA charstr(szFile);

	FILE *outfile;
     
    // open file for writing
    //outfile = fopen ("trend.dat", "w");
	outfile = fopen ((const char *)charstr, "w");
	if (outfile == NULL)
    {
        //fprintf(stderr, "\nError opend file\n");
        return false;
    }
 
    // write struct to file
    fwrite (&m_stTrendData, sizeof(struct TrendData), 1, outfile);
     
	if(fwrite == 0) 
	{
		//error
		int iStop=0;
	}
   /* if(fwrite != 0) 
        printf("contents to file written successfully !\n");
    else
        printf("error writing file !\n");*/

	fclose(outfile);

	return true;
}
bool CTrend::read(CString szFile)
{
	CStringA charstr(szFile);

	FILE *infile;
    //struct person input;
     
    // Open person.dat for reading
    infile = fopen ((const char *)charstr, "r");
    if (infile == NULL)
    {
        return false;
    }
     
    // read file contents till end of file
   /* while(fread(&m_stTrendData, sizeof(struct TrendData), 1, infile))
        printf ("id = %d name = %s %s\n", input.id,
        input.fname, input.lname);*/
	fread(&m_stTrendData, sizeof(struct TrendData), 1, infile);

	fclose(infile);

	return true;
}

/**********************************************************************************************//**
 * Writes a buffer
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	val		   	The value.
 * \param	dtTimestamp	The dt timestamp.
 *
 * \return	A WORD.
 **************************************************************************************************/

WORD CTrend::WriteBuffer(WORD val, COleDateTime dtTimestamp)
{
	if(m_stTrendData.wBufCount>=SERIALZEBUFFER)
	{
		//ERROR sollte nie vorkommen, da m_wBufCount bei >=SERIALZEBUFFER serialisiert werden sollte (DataHandler) und dabei auf 0 gesetzt wird
		m_stTrendData.wBufCount=0;
	}

	m_stTrendData.wBuffer[m_stTrendData.wBufCount]=val;
	//DEBUGMSG(TRUE, (TEXT("trend buffer count:%d val:%d\r\n"),m_stTrendData.wBufCount,val));


	m_stTrendData.wBufCount++;

	

	SetTimestamp(dtTimestamp);

	return m_stTrendData.wBufCount;

	//if(m_wBufCount>=SERIALZEBUFFER)
	//{
	//	//ERROR sollte nie vorkommen, da m_wBufCount bei >=SERIALZEBUFFER serialisiert werden sollte (DataHandler) und dabei auf 0 gesetzt wird
	//	m_wBufCount=0;
	//	m_stTrendData.wBufCount=0;
	//}

	//m_wBuffer[m_wBufCount]=val;
	//m_wBufCount++;

	//m_stTrendData.wBuffer[m_wBufCount]=val;
	//m_stTrendData.wBufCount=m_wBufCount;

	//SetTimestamp(dtTimestamp);
	//
	//return m_wBufCount;
}

/**********************************************************************************************//**
 * Gets buffer value
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	pos	The position.
 *
 * \return	The buffer value.
 **************************************************************************************************/

WORD CTrend::GetBufferValue(UINT pos)
{
	return m_stTrendData.wBuffer[pos];
	//return m_wBuffer[pos];
}

/**********************************************************************************************//**
 * Gets the timestamp
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	The timestamp.
 **************************************************************************************************/

COleDateTime CTrend::GetTimestamp()
{
	//CStringW szTime = m_dtTimestamp.Format(LOCALE_NOUSEROVERRIDE,LANG_USER_DEFAULT);
	return m_stTrendData.m_dtTimestamp;
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

/**********************************************************************************************//**
 * Sets a timestamp
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	timestamp	The timestamp.
 **************************************************************************************************/

void CTrend::SetTimestamp(COleDateTime timestamp)
{
	m_stTrendData.m_dtTimestamp=timestamp;

	//timestamp.ParseDateTime(m_stTrendData.strTimestamp);

	/*sprintf(m_stTrendData.strTimestamp, "%02d.%02d.%04d %02d:%02d:%02d", 
		m_dtTimestamp.GetDay(),
		m_dtTimestamp.GetMonth(),
		m_dtTimestamp.GetYear(),
		m_dtTimestamp.GetHour(),
		m_dtTimestamp.GetMinute(),
		m_dtTimestamp.GetSecond());*/
	//CStringW szTime = m_dtTimestamp.Format(LOCALE_NOUSEROVERRIDE,LANG_USER_DEFAULT);
	//DEBUGMSG(TRUE, (TEXT("SetTimestamp2 %s\r\n"),szTime));
}

/**********************************************************************************************//**
 * Gets buffer count
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	The buffer count.
 **************************************************************************************************/

WORD CTrend::GetBufferCount()
{
	return m_stTrendData.wBufCount;
	//return m_wBufCount;
}

/**********************************************************************************************//**
 * Sets buffer count
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	count	Number of.
 **************************************************************************************************/

void CTrend::SetBufferCount(WORD count)
{
	//m_wBufCount=count;
	m_stTrendData.wBufCount=count;
}

/**********************************************************************************************//**
 * Resets the buffer
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CTrend::ResetBuffer()
{
	m_stTrendData.m_dtTimestamp.SetStatus(COleDateTime::null);
	m_stTrendData.wBufCount=0;

	for(int i=0;i<SERIALZEBUFFER;i++)
	{
		m_stTrendData.wBuffer[i]=0;
	}
}


