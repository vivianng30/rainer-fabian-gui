#pragma once
#include <iostream>
#include "CircularBuffer.h"
#include "globDefs.h"

// CTrend command target

//class CircularBuffer;



class CTrend : public CObject
{
	//DECLARE_SERIAL(CTrend)
public:
	virtual void Serialize(CArchive& ar);
	//void Serialize(CArchive& ar);
	
	CTrend();
	virtual ~CTrend(void);

	DECLARE_SERIAL(CTrend)

	WORD WriteBuffer(WORD val, COleDateTime dtTimestamp);
	WORD GetBufferValue(UINT pos);

	COleDateTime GetTimestamp();
	//void SetTimestamp();
	void SetTimestamp(COleDateTime timestamp);

	WORD GetBufferCount();
	void SetBufferCount(WORD count);
	void ResetBuffer();

protected:
	COleDateTime m_dtTimestamp;
	
	//WORD	*m_wBuffer;
	WORD	m_wBuffer[SERIALZEBUFFER];
	WORD	m_wBufCount;

	/*typedef struct tagTREND
	{
		COleDateTime m_dtTimestamp;
		WORD	m_wBuffer[SERIALZEBUFFER];
		WORD	m_wBufCount;
	}MYTREND;

	MYTREND m_MyTrend;*/

 
};


