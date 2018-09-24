
#if !defined(__TLS_STOPWATCH_H__)
#define __TLS_STOPWATCH_H__

class CTlsTime  
{
	CTime m_time;
	
public:
	CTlsTime();
	CTlsTime(CTime time);
	CTlsTime(int iYear, int iMonth, int iDay, int iHour=0, int iMinute=0, int iSecond=0);
	virtual ~CTlsTime();
	
	int GetYear();
	int GetMonth();
	int GetDay();
	int GetHour();
	int GetMinute();
	int GetSecond();
	
	CStringW GetDispDate();
	CStringW GetDispTime(bool bSecond = true);
	CStringW GetDispDateTime();
	CStringW GetExpDate();
	CStringW GetExpTime();
	CStringW GetExpDateTime();
	
	static CTlsTime ParseSystemDateTime(CStringW sTime);
    static CTlsTime ParseJavaDateTime(CStringW sTime);
    CStringW GetJavaDateTime();
	void Add(int iDays, int iHours=0, int iMinutes=0, int iSeconds=0); 
    void Substract(int iDays, int iHours=0, int iMinutes=0, int iSeconds=0);
	
	CTime GetCTime() { return m_time; }
	static CTime   GetTimeStamp();
	static CTlsTime TS();
	
	static CTlsTime   GetNullTime() { return CTlsTime (1970,1,1,0,0,0); }
	
	static CTimeSpan Diff(CTlsTime& tsFirst, CTlsTime& tsSecond);

	static CTlsTime UTC2Local(CTlsTime time);
	static CTlsTime Local2UTC(CTlsTime time);

	CTlsTime& operator=(CTlsTime& time);
	CTlsTime& operator=(CTime time);

	bool operator==(const CTlsTime& time) const;
	bool operator!=(const CTlsTime& time) const;
	
	bool operator<(const CTlsTime& time) const;
};

#endif
/*=============================================================================
    E O F
=============================================================================*/
