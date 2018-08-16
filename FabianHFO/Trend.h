/**********************************************************************************************//**
 * \file	Trend.h.
 *
 * Declares the trend class
 **************************************************************************************************/

#pragma once
#include <iostream>
#include "CircularBuffer.h"
#include "globDefs.h"


// CTrend command target

//class CircularBuffer;



class CTrend //: public CObject
{
	//DECLARE_SERIAL(CTrend)
public:
	//virtual void Serialize(CArchive& ar);
	bool Serialize(CString szFile, eTrendSerialize state, UINT type);
	
	CTrend();
	virtual ~CTrend(void);

	//DECLARE_SERIAL(CTrend)

	WORD WriteBuffer(WORD val, COleDateTime dtTimestamp);
	WORD GetBufferValue(UINT pos);

	COleDateTime GetTimestamp();
	void SetTimestamp(COleDateTime timestamp);

	WORD GetBufferCount();
	void SetBufferCount(WORD count);
	void ResetBuffer();

	

protected:

	bool write(CString szFile);
	bool read(CString szFile);

	/*SYSTEMTIME stTime;
		GetLocalTime(&stTime);
		COleDateTime dtCurrentTime(stTime);*/
	//std::string currentDateTime();

	typedef struct
	{
		COleDateTime m_dtTimestamp;
		//char strTimestamp[80];
		WORD	wBuffer[SERIALZEBUFFER];
		WORD	wBufCount;
	}TrendData;

	TrendData m_stTrendData;

	//COleDateTime m_dtTimestamp;
	
	//WORD	m_wBuffer[SERIALZEBUFFER];
	//WORD	m_wBufCount;
};


