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
 * Initializes a new instance of the CTrend class.
 *
 * @author	Rainer Kühner
 * @date	16.08.2018
 **************************************************************************************************/

CTrend::CTrend()
{
	m_stTrendData.m_dtTimestamp.SetStatus(COleDateTime::null);

	m_stTrendData.wBufCount=0;

	for(int i=0;i<SERIALZEBUFFER;i++)
	{
		m_stTrendData.wBuffer[i]=0;
	}

}

/**********************************************************************************************//**
 * Finalizes an instance of the CTrend class.
 *
 * @author	Rainer Kühner
 * @date	16.08.2018
 **************************************************************************************************/

CTrend::~CTrend(void)
{

}

bool CTrend::Serialize(CString szFile, eTrendSerialize state, UINT type)
{
	bool bRes=false;
	if(TREND_READ==state)
	{
		bRes=read(szFile);

		//if(TREND_PINSP==type)
		//{
		//	//TEST
		//	/*for(int i=0;i<SERIALZEBUFFER;i++)
		//	{
		//		DEBUGMSG(TRUE, (TEXT("trend buffer read i:%d val:%d\r\n"),i,m_stTrendData.wBuffer[i]));
		//	}*/
		//	DEBUGMSG(TRUE, (TEXT("trend buffer read count:%d time:%02d.%02d.%04d %02d:%02d:%02d\r\n"),m_stTrendData.wBufCount,
		//		m_stTrendData.m_dtTimestamp.GetDay(),
		//		m_stTrendData.m_dtTimestamp.GetMonth(),
		//		m_stTrendData.m_dtTimestamp.GetYear(),
		//		m_stTrendData.m_dtTimestamp.GetHour(),
		//		m_stTrendData.m_dtTimestamp.GetMinute(),
		//		m_stTrendData.m_dtTimestamp.GetSecond()));
		//	DEBUGMSG(TRUE, (TEXT("trend buffer file :%s\r\n"),szFile));
		//}
		
	}
	else// TREND_WRITE
	{
		bRes=write(szFile);

		//if(TREND_PINSP==type)
		//{
		//	//TEST
		//	/*for(int i=0;i<SERIALZEBUFFER;i++)
		//	{
		//		DEBUGMSG(TRUE, (TEXT("trend buffer write i:%d val:%d\r\n"),i,m_stTrendData.wBuffer[i]));
		//	}*/

		//	DEBUGMSG(TRUE, (TEXT("trend buffer write count:%d time:%02d.%02d.%04d %02d:%02d:%02d\r\n"),m_stTrendData.wBufCount,
		//		m_stTrendData.m_dtTimestamp.GetDay(),
		//		m_stTrendData.m_dtTimestamp.GetMonth(),
		//		m_stTrendData.m_dtTimestamp.GetYear(),
		//		m_stTrendData.m_dtTimestamp.GetHour(),
		//		m_stTrendData.m_dtTimestamp.GetMinute(),
		//		m_stTrendData.m_dtTimestamp.GetSecond()));
		//	DEBUGMSG(TRUE, (TEXT("trend buffer file :%s\r\n"),szFile));
		//}
	}
	return bRes;
}

/**********************************************************************************************//**
 * Writes the given size file.
 *
 * @author	Rainer Kühner
 * @date	16.08.2018
 *
 * @param	szFile	The file to write.
 *
 * @return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CTrend::write(CString szFile)
{
	CStringA charstr(szFile);

	FILE *outfile;
     
    // open file for writing
	outfile = fopen ((const char *)charstr, "w");
	if (outfile == NULL)
    {
        //fprintf(stderr, "\nError opend file\n");
        return false;
    }
 
    // write struct to file
    fwrite (&m_stTrendData, sizeof(struct TrendData), 1, outfile);
     
	//if(fwrite == 0) 
	//{
	//	//error
	//	int iStop=0;
	//}
   /* if(fwrite != 0) 
        printf("contents to file written successfully !\n");
    else
        printf("error writing file !\n");*/

	fclose(outfile);

	return true;
}

/**********************************************************************************************//**
 * Reads the given size file.
 *
 * @author	Rainer Kühner
 * @date	16.08.2018
 *
 * @param	szFile	The file to read.
 *
 * @return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CTrend::read(CString szFile)
{
	CStringA charstr(szFile);

	FILE *infile;
     
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
}


WORD CTrend::GetBufferValue(UINT pos)
{
	return m_stTrendData.wBuffer[pos];
}

/**********************************************************************************************//**
 * Gets the timestamp.
 *
 * @author	Rainer Kühner
 * @date	16.08.2018
 *
 * @return	The timestamp.
 **************************************************************************************************/

COleDateTime CTrend::GetTimestamp()
{
	return m_stTrendData.m_dtTimestamp;
}

/**********************************************************************************************//**
 * Sets a timestamp.
 *
 * @author	Rainer Kühner
 * @date	16.08.2018
 *
 * @param	timestamp	The timestamp.
 **************************************************************************************************/

void CTrend::SetTimestamp(COleDateTime timestamp)
{
	m_stTrendData.m_dtTimestamp=timestamp;
}

/**********************************************************************************************//**
 * Gets buffer count.
 *
 * @author	Rainer Kühner
 * @date	16.08.2018
 *
 * @return	The buffer count.
 **************************************************************************************************/

WORD CTrend::GetBufferCount()
{
	return m_stTrendData.wBufCount;
}


void CTrend::SetBufferCount(WORD count)
{
	m_stTrendData.wBufCount=count;
}

/**********************************************************************************************//**
 * Resets the buffer.
 *
 * @author	Rainer Kühner
 * @date	16.08.2018
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


