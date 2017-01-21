// Logfile.h: interface for the Logfile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGFILE_H__72502C54_C715_4FE4_9FFC_C4EB1296C824__INCLUDED_)
#define AFX_LOGFILE_H__72502C54_C715_4FE4_9FFC_C4EB1296C824__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class Logfile  
{
public:
	

	 bool WriteLine(CStringW inhalt);
	
	 bool CheckDate();
	 void writeDate();


	//static void copyLogToUSB(bool state);

	Logfile(const char * svDateiname);
	//Logfile(const Logfile& rhs);

	bool Open();
	bool Close();
	virtual ~Logfile();

	void StartLogfileThread(void);
	void StopLogfileThread( void );


private:
	CString GetFilePattern();
	//CString GetContent(int num);
	 bool CheckFileSize();
	 bool WritePlainLine(CStringW inhalt);
	// DWORD writeFile(CStringW inhalt);


protected:
	CString svLastDatum;

	CList<CStringW, CStringW&>  msgLog;

	CRITICAL_SECTION csLog;

	 Logfile* m_Logfile;

	FILE* stream;
	CString m_Filename;
	int maxfiles;
	int maxsize;

	friend UINT CWriteLogThread(LPVOID pc);
	CWinThread*	m_pcwtWriteLogThread;
	DWORD writeLogfile(void);
	bool m_bDoWriteLogfile;
	HANDLE m_hThreadWriteLog;

	bool m_bExit;
};

#endif // !defined(AFX_LOGFILE_H__72502C54_C715_4FE4_9FFC_C4EB1296C824__INCLUDED_)
