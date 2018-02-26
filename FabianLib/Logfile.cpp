// Logfile.cpp: implementation of the Logfile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Logfile.h"
#include "TlsFile.h"

/**********************************************************************************************//**
 * Initializes a new instance of the Logfile class
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	svDateiname	The sv dateiname.
 **************************************************************************************************/

Logfile::Logfile(const char * svDateiname)
{
	InitializeCriticalSection(&csLog);
	
	m_Logfile = this;
	m_Logfile->svLastDatum = "";

	maxfiles = 10;
	maxsize = 1024 * 200;
	
	stream = NULL;

	//überprüft ob das zielverzeichnis vorhanden ist
	CString sTo = CString(svDateiname);
	LPCTSTR dst = sTo;

	
	wchar_t *pos2 =  const_cast<LPTSTR>(wcsrchr(dst,L'\\'));
	int pos = pos2 - dst; 
	
	CString folder;
	folder = sTo.Mid(0,pos);
	
	if (!CTlsFile::Exists(folder))
	{
		CTlsFile::MkDir(folder);
	}

	
	m_Filename = svDateiname;

	m_pcwtWriteLogThread=NULL;
	m_bDoWriteLogfile=false;
	m_hThreadWriteLog=INVALID_HANDLE_VALUE;

	StartLogfileThread();
	m_bExit=false;
}

/**********************************************************************************************//**
 * Opens this instance
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool Logfile::Open()
{
	stream = _wfopen(m_Filename,L"a+");

	if (stream == NULL)
		return false;

	return true;
}

/**********************************************************************************************//**
 * Closes this instance
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool Logfile::Close()
{

	if (stream != NULL)
	{
		fclose(stream);
	}
	stream = NULL;
	return true;
}

/**********************************************************************************************//**
 * Exits this instance
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

void Logfile::Exit()
{
	m_bExit=true;
	StopLogfileThread();
}

/**********************************************************************************************//**
 * Finalizes an instance of the Logfile class
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

Logfile::~Logfile()
{
	if(m_pcwtWriteLogThread!=NULL)
	{
		delete m_pcwtWriteLogThread;
		m_pcwtWriteLogThread=NULL;

		if(m_hThreadWriteLog!=INVALID_HANDLE_VALUE)
		{
			CloseHandle(m_hThreadWriteLog);
			m_hThreadWriteLog=INVALID_HANDLE_VALUE;
		}
	}

	
	if (stream != NULL)
	{
		fputws(L"\n",stream);
		fclose(stream);
	}
	

	m_Logfile = NULL;

	EnterCriticalSection(&csLog);
	msgLog.RemoveAll();
	LeaveCriticalSection(&csLog);

	DeleteCriticalSection(&csLog);

}

/**********************************************************************************************//**
 * Determines if we can check file size
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool Logfile::CheckFileSize()
{
	if (m_Logfile == NULL)
		return false;

	if (m_Logfile->stream != NULL)
	{
		
		long _lFileSize = ftell(m_Logfile->stream);
		if (_lFileSize == -1) {  /* Handle Error */}
		if ( _lFileSize > m_Logfile->maxsize)
		{
			fputws(_T("***FILESIZE END***"),m_Logfile->stream);
			m_Logfile->svLastDatum = "";

			// muss beendet werden
			fclose(m_Logfile->stream);

			//löscht das letzte file
			CString pattern = m_Logfile->GetFilePattern();

			if (pattern.GetLength() > 0)
			{
				// benennt die files um
				for (int x = m_Logfile->maxfiles; x > 1 ; x--)
				{
					CString nameold;
					CString namenew;

					namenew.Format(pattern, x);
					nameold.Format(pattern, x -1);

					if (CTlsFile::Exists(nameold))
					{
						CTlsFile::Delete(namenew);
						if (!CTlsFile::Rename(nameold,namenew,true))
						{
							
						}

					}
				}

				CString namenew;
				CString nameold = m_Logfile->m_Filename;
				namenew.Format(pattern, 1);

				CTlsFile::Delete(namenew);
				if (!CTlsFile::Rename(nameold, namenew,true))
				{
					
				}

				return m_Logfile->Open();
			}
		}
	}

	return false;
}

/**********************************************************************************************//**
 * Writes a plain line
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	inhalt	The inhalt.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool Logfile::WritePlainLine(CStringW inhalt)
{
	EnterCriticalSection(&csLog);
	msgLog.AddTail(inhalt);
	LeaveCriticalSection(&csLog);

	return true;
}

/**********************************************************************************************//**
 * Writes the date
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

void Logfile::writeDate()
{
	if(m_Logfile != NULL)
	{
		SYSTEMTIME st;
		GetLocalTime(&st);

		COleDateTime dtTime(st);
		CStringW svDateNow;
		svDateNow.Format(L"%d.%d.%d",dtTime.GetDay(),dtTime.GetMonth(),dtTime.GetYear());
		m_Logfile->svLastDatum = svDateNow;

		WritePlainLine(_T("STARTDATE"));
		WritePlainLine(m_Logfile->svLastDatum);
	}
}

/**********************************************************************************************//**
 * Determines if we can check date
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool Logfile::CheckDate()
{
	bool bChanged=false;
	
	if(m_Logfile != NULL)
	{
		SYSTEMTIME st;
		GetLocalTime(&st);

		COleDateTime dtTime(st);
		CStringW svDateNow;
		svDateNow.Format(L"%d.%d.%d",dtTime.GetDay(),dtTime.GetMonth(),dtTime.GetYear());
		CString svDateLast=m_Logfile->svLastDatum;
		
		if (svDateNow != svDateLast)
		{
			bChanged=true;			
			m_Logfile->svLastDatum = svDateNow;

			WritePlainLine(_T("---DATE---"));
			WritePlainLine(m_Logfile->svLastDatum);
		}	
	}

	return bChanged;
}

/**********************************************************************************************//**
 * Writes a line
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	inhalt	The inhalt.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool Logfile::WriteLine(CStringW inhalt)
{

	bool bRes=true;

	if(m_bExit)
		return false;

	SYSTEMTIME st;
	GetLocalTime(&st);

	CString temp;
	temp.Format(TEXT("%02d:%02d:%02d: %s"), st.wHour, st.wMinute, st.wSecond, inhalt);

	bRes= WritePlainLine(temp);

	Sleep(0);

	return bRes;
}



//CString Logfile::GetContent(int num)
//{
//	FILE* s = stream;
//
//	if (num != 0)
//	{
//		CString pattern = GetFilePattern();
//		CString name;
//		name.Format(pattern,num);
//
//		s = _wfopen(name,L"r");
//
//		if (s==NULL)
//			return L"";
//	}
//
//	CString temp(L"");
//
//	if (s != NULL)
//	{
//		fpos_t pos;
//		pos = 0;
//		
//		fsetpos(s, &pos);
//
//		wchar_t line[100];
//
//		while (!feof(s))
//		{
//			if (fgetws( line, 100, s ) == NULL)
//			{
//				break;
//			}
//			else
//			{
//				temp += CString(line);
//			}
//		}
//	}
//
//	if (num != 0)
//		fclose(s);
//
//	return temp;
//}

/**********************************************************************************************//**
 * Gets file pattern
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \return	The file pattern.
 **************************************************************************************************/

CString Logfile::GetFilePattern()
{
	if (m_Logfile)
	{
		//wchar_t* p = const_cast<LPTSTR>(wcsrchr(m_Logfile->m_Filename, '.'));
		const wchar_t* p = wcsrchr(m_Logfile->m_Filename, '.');

		if (p != NULL)
		{
			//holt sich das filepattern
			int extpos = p - m_Logfile->m_Filename;

			CString pattern(m_Logfile->m_Filename);
			pattern.Insert(extpos,L"%d");
			
			return pattern;
		}
	}


	return L"";
}

/**********************************************************************************************//**
 * Starts logfile thread
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

void Logfile::StartLogfileThread( void )
{
	
	m_bDoWriteLogfile=true;

	if(m_pcwtWriteLogThread!=NULL)
	{
		delete m_pcwtWriteLogThread;
		m_pcwtWriteLogThread=NULL;

		if(m_hThreadWriteLog!=INVALID_HANDLE_VALUE)
		{
			CloseHandle(m_hThreadWriteLog);
			m_hThreadWriteLog=INVALID_HANDLE_VALUE;
		}
	}

	//DEBUGMSG(TRUE, (TEXT("StartSerialThread4\r\n")));

	m_pcwtWriteLogThread=AfxBeginThread(CWriteLogThread,this,THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED);
	m_hThreadWriteLog=m_pcwtWriteLogThread->m_hThread;
	m_pcwtWriteLogThread->m_bAutoDelete = FALSE; 
	m_pcwtWriteLogThread->ResumeThread();

	
}

/**********************************************************************************************//**
 * Stops logfile thread
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

void Logfile::StopLogfileThread( void )
{
	if(m_bDoWriteLogfile)
	{
		m_bDoWriteLogfile=false;
		//DEBUGMSG(TRUE, (TEXT("StopSerialThread 5\r\n")));
		if (WaitForSingleObject(m_pcwtWriteLogThread->m_hThread,500) == WAIT_TIMEOUT)
		{
			DEBUGMSG(TRUE, (TEXT("StopLogfileThread 1 !!!!!!!!!!!!!!\r\n")));

			if(!TerminateThread(m_pcwtWriteLogThread,0))
			{

			}
		}
	}



}

/**********************************************************************************************//**
 * Writes a log thread
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	pc	The PC.
 *
 * \return	An UINT.
 **************************************************************************************************/

static UINT CWriteLogThread( LPVOID pc )
{
	((Logfile*)pc)->writeLogfile();
	return TRUE;
}

/**********************************************************************************************//**
 * Writes the logfile
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \return	A DWORD.
 **************************************************************************************************/

DWORD Logfile::writeLogfile(void) 
{
	CeSetThreadPriority(m_pcwtWriteLogThread->m_hThread,250);//PRICO04

	do
	{
		EnterCriticalSection(&csLog);

		POSITION pos = msgLog.GetHeadPosition();
		if(!pos)
		{
			LeaveCriticalSection(&csLog);
		}
		else
		{
			Open();
			if (m_Logfile->stream != NULL)
			{
				fputws(msgLog.GetAt(pos),m_Logfile->stream);
				fputws(L"\n",m_Logfile->stream);
				msgLog.RemoveAt(pos);
			}
			LeaveCriticalSection(&csLog);

			CheckFileSize();
			Close();
		}

		Sleep(50);

	}while(m_bDoWriteLogfile);

	
	return 0;
}
