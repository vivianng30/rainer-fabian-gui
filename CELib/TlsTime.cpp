
#include "stdafx.h"
#include "TlsTime.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#include "TlsDbg.h"
#define new DEBUG_NEW
#define malloc DEBUG_MALLOC
#define free DEBUG_FREE
#define realloc DEBUG_REALLOC
#endif

CTlsTime::CTlsTime()
: m_time(GetNullTime().m_time)
{

}

CTlsTime::CTlsTime(CTime time)
: m_time(time)
{
}

CTlsTime::CTlsTime(int iYear, int iMonth, int iDay, int iHour, int iMinute, int iSecond)
: m_time(iYear, iMonth, iDay, iHour, iMinute, iSecond)
{
}

CTlsTime::~CTlsTime()
{

}

int CTlsTime::GetYear()
{
	return m_time.GetYear();
}

int CTlsTime::GetMonth()
{
	return m_time.GetMonth();
}

int CTlsTime::GetDay()
{
	return m_time.GetDay();
}

int CTlsTime::GetHour()
{
	return m_time.GetHour();
}

int CTlsTime::GetMinute()
{
	return m_time.GetMinute();
}

int CTlsTime::GetSecond()
{
	return m_time.GetSecond();
}

CStringW CTlsTime::GetDispDate()
{
  if(m_time == GetNullTime().GetCTime())
    return L"";

  CStringW sRet;
	sRet.Format(TEXT("%02d.%02d.%04d"), m_time.GetDay(), m_time.GetMonth(), m_time.GetYear());
  return sRet;
}

CStringW CTlsTime::GetDispTime(bool bSecond)
{
  if(m_time == GetNullTime().GetCTime())
    return L"";

  CStringW sRet;
  if(bSecond)
	sRet.Format(TEXT("%02d:%02d:%02d"), m_time.GetHour(), m_time.GetMinute(), m_time.GetSecond());
  else
	sRet.Format(TEXT("%02d:%02d"), m_time.GetHour(), m_time.GetMinute());
  return sRet;
}

CStringW CTlsTime::GetDispDateTime()
{
  if(m_time == GetNullTime().GetCTime())
    return L"";

  return GetDispDate() + L" " + GetDispTime();
}

CStringW CTlsTime::GetExpDate()
{
  if(m_time == GetNullTime().GetCTime())
    return L"";

  CStringW sRet;
	sRet.Format(TEXT("%04d%02d%02d"), m_time.GetYear(), m_time.GetMonth(), m_time.GetDay());
  return sRet;
}

CStringW CTlsTime::GetExpTime()
{
  if(m_time == GetNullTime().GetCTime())
    return L"";

  CStringW sRet;
	sRet.Format(TEXT("%02d%02d%02d"), m_time.GetHour(), m_time.GetMinute(), m_time.GetSecond());
  return sRet;
}

CStringW CTlsTime::GetExpDateTime()
{
  if(m_time == GetNullTime().GetCTime())
    return L"";

  return GetExpDate() + GetExpTime();
}

CStringW CTlsTime::GetJavaDateTime()
{
    if(m_time == GetNullTime().GetCTime())
        return L"";
    
    CStringW sDate;
    sDate.Format(TEXT("%04d-%02d-%02d"), m_time.GetYear(), m_time.GetMonth(), m_time.GetDay());
    
    CStringW sTime;
    sTime.Format(TEXT("%02d:%02d:%02d"), m_time.GetHour(), m_time.GetMinute(), m_time.GetSecond());
    
    return sDate + L" " + sTime;
}

CTlsTime CTlsTime::ParseSystemDateTime(CStringW sTime)
{
    // 'YYYYMMDDHHMMSS'
    int iYear = 0;
    int iMonth = 0;
    int iDay = 0;
    int iHour = 0;
    int iMinute = 0;
    int iSecond = 0;
    if(sTime.GetLength() == 14 || sTime.GetLength() == 8)
    {
        iYear = _wtoi(sTime.Mid(0, 4));
        iMonth = _wtoi(sTime.Mid(4, 2));
        iDay = _wtoi(sTime.Mid(6,2));
    }
    if(sTime.GetLength() == 14)
    {
        iHour = _wtoi(sTime.Mid(8, 2));
        iMinute = _wtoi(sTime.Mid(10, 2));
        iSecond = _wtoi(sTime.Mid(12,2));
    }

	if(iYear == 0)
		return GetNullTime();

    return CTime(iYear, iMonth, iDay, iHour, iMinute, iSecond);
}

CTlsTime CTlsTime::ParseJavaDateTime(CStringW sTime)
{
    // 'YYYY-MM-DD HH:MM:SS'
    int iYear = 0;
    int iMonth = 0;
    int iDay = 0;
    int iHour = 0;
    int iMinute = 0;
    int iSecond = 0;
    if(sTime.GetLength() == 19 || sTime.GetLength() == 10)
    {
        iYear = _wtoi(sTime.Mid(0, 4));
        iMonth = _wtoi(sTime.Mid(5, 2));
        iDay = _wtoi(sTime.Mid(8,2));
    }
    if(sTime.GetLength() == 19)
    {
        iHour = _wtoi(sTime.Mid(11, 2));
        iMinute = _wtoi(sTime.Mid(14, 2));
        iSecond = _wtoi(sTime.Mid(17,2));
    }

	if(iYear == 0)
		return GetNullTime();

    return CTime(iYear, iMonth, iDay, iHour, iMinute, iSecond);
}

void CTlsTime::Add(int iDays, int iHours, int iMinutes, int iSeconds)
{
  m_time += CTimeSpan(iDays, iHours, iMinutes, iSeconds);
}

void CTlsTime::Substract(int iDays, int iHours, int iMinutes, int iSeconds)
{
  m_time -= CTimeSpan(iDays, iHours, iMinutes, iSeconds);
}

CTime CTlsTime::GetTimeStamp()
{
	SYSTEMTIME systime;
	GetLocalTime(&systime);

	return CTime(systime); 

  //return CTime::GetCurrentTime(); 
}

CTlsTime CTlsTime::TS()
{
  return GetTimeStamp();
}

CTimeSpan CTlsTime::Diff(CTlsTime& tsFirst, CTlsTime& tsSecond)
{
	if(   tsFirst == GetNullTime()
	   || tsSecond == GetNullTime())
	   return CTimeSpan(0, 0, 0, 0);

	return tsSecond.GetCTime() - tsFirst.GetCTime();
}

CTlsTime CTlsTime::UTC2Local(CTlsTime time)
{
	CTlsTime local = time;
	TIME_ZONE_INFORMATION zone;

	if(GetTimeZoneInformation(&zone) == TIME_ZONE_ID_DAYLIGHT )
	{
		local.Add(0, 0, -zone.DaylightBias, 0);
	}
	
    local.Substract(0, 0, zone.Bias, 0);

    return local;
}

CTlsTime CTlsTime::Local2UTC(CTlsTime time)
{
	CTlsTime temp = time;

	TIME_ZONE_INFORMATION zone;
	GetTimeZoneInformation(&zone);
	temp.Add(0, 0, zone.Bias, 0);

	return temp;
}

CTlsTime& CTlsTime::operator=(CTlsTime& time)
{
	m_time = time.m_time;

	return (*this);
}

CTlsTime& CTlsTime::operator=(CTime time)
{
	m_time = time;

	return (*this);
}

bool CTlsTime::operator==(const CTlsTime& time) const
{
	return (m_time == time.m_time) != 0;
}

bool CTlsTime::operator!=(const CTlsTime& time) const
{
	return (m_time != time.m_time) != 0;
}

bool CTlsTime::operator<(const CTlsTime& time) const
{
	if((*this) == GetNullTime() || time == GetNullTime())
		return false;
	
	return (m_time < time.m_time) != 0;
}

/*=============================================================================
    E O F
=============================================================================*/
