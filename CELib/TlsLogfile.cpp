
#include "stdafx.h"
#include "TlsLogfile.h"
#include "TlsFile.h"
#include "TlsTime.h"
//#include "TlsLoglist.h"

//=============================================================================
CTlsLogfile::CTlsLogfile(const char * svDateiname)
{
	m_bCopyLog=false;
	m_Logfile = this;

	//maxfiles = 10;
	//maxsize = 1024 * 50;
	m_maxfiles = 10;
	m_maxsize = 1024 * 200;

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

	Open();
}

CTlsLogfile::CTlsLogfile(CString svDateiname)
{
	m_bCopyLog=false;
	m_Logfile = this;

	//maxfiles = 10;
	//maxsize = 1024 * 50;
	m_maxfiles = 10;
	m_maxsize = 1024 * 200;

	//überprüft ob das zielverzeichnis vorhanden ist
	//CString sTo = CString(svDateiname);
	LPCTSTR dst = svDateiname;

	wchar_t *pos2 =  const_cast<LPTSTR>(wcsrchr(dst,L'\\'));
	int pos = pos2 - dst; 

	CString folder;
	folder = svDateiname.Mid(0,pos);

	if (!CTlsFile::Exists(folder))
	{
		CTlsFile::MkDir(folder);
	}


	m_Filename = svDateiname;

	Open();
}

//=============================================================================
bool CTlsLogfile::Open()
{
	//stream = _wfopen(m_Filename,L"a+",ccs=UNICODE);
	//stream = _wfopen( m_Filename,L"a+,ccs=UTF-8");
	//stream = _wfopen( m_Filename,L"a+,ccs=UNICODE");
	stream = _wfopen(m_Filename,L"a+");

	if (stream == NULL)
		return false;

	return true;
}

//=============================================================================
bool CTlsLogfile::Close()
{
	if (stream != NULL)
	{
		//fputws(L"\n",stream);
		fclose(stream);
	}

	return true;
}

//=============================================================================
bool CTlsLogfile::Save()
{
	m_bCopyLog=true;
	Close();
	Open();
	m_bCopyLog=false;

	return true;
}

void CTlsLogfile::copyLog(bool state)
{
	m_bCopyLog=state;
}
//=============================================================================
CTlsLogfile::~CTlsLogfile()
{
	m_Logfile = NULL;

	if (stream != NULL)
	{
		fputws(L"\n",stream);
		fclose(stream);
	}
}

//=============================================================================
bool CTlsLogfile::CheckFileSize()
{
	if(!m_bCopyLog)
	{
		if (m_Logfile == NULL)
			return false;

		if (m_Logfile->stream != NULL)
		{

			fpos_t pos;

			if (fgetpos(m_Logfile->stream, &pos) != 0)
				return false;


			if (pos > m_Logfile->m_maxsize)
			{
				WritePlainLine(_T("***FILESIZE END***"),false);
				m_Logfile->svLastDatum = "";

				// muss beendet werden
				fclose(m_Logfile->stream);

				//löscht das letzte file

				CStringW pattern = m_Logfile->GetFilePattern();

				if (pattern.GetLength() > 0)
				{
					// benennt die files um
					for (int x = m_Logfile->m_maxfiles; x > 1 ; x--)
					{
						CStringW nameold;
						CStringW namenew;

						namenew.Format(pattern, x);
						nameold.Format(pattern, x -1);

						if (CTlsFile::Exists(nameold))
						{
							CTlsFile::Delete(namenew);
							if (!CTlsFile::Rename(nameold,namenew,true))
							{
								AfxMessageBox( _T("Failure1: Rename ") + nameold);
							}

						}
					}

					CStringW namenew;
					CStringW nameold(m_Logfile->m_Filename);
					//CStringW nameold = m_Logfile->m_Filename;
					namenew.Format(pattern, 1);

					CTlsFile::Delete(namenew);
					if (!CTlsFile::Rename(nameold, namenew,true))
					{
						AfxMessageBox( _T("Failure2: Rename ") + nameold);
					}

					//CTlsFile::Delete(m_Logfile->m_Filename);

					return m_Logfile->Open();
				}
			}

		}
	}
	

	return false;
}

//=============================================================================
bool CTlsLogfile::WritePlainLine(CStringW inhalt, bool bCheckFileSize)
{
	

	if(!m_bCopyLog)
	{
		if (m_Logfile == NULL)
			return false;

		if(bCheckFileSize)
			CheckFileSize();

		if (m_Logfile->stream != NULL)
		{
			fputws(inhalt,m_Logfile->stream);
			fputws(_T("\n"),m_Logfile->stream);
			fflush(m_Logfile->stream);
		}
	}
	

	return true;
}

//void CTlsLogfile::copyLog(bool state)
//{
//	m_bCopyLog=state;
//}

//=============================================================================
bool CTlsLogfile::WriteLine(CStringW inhalt, bool bDate)
{
	if(!m_bCopyLog)
	{
		CStringW temp=_T("");

		if(bDate)
		{
			SYSTEMTIME t;
			GetLocalTime(&t);

			temp.Format(TEXT("%02d/%02d %02d:%02d:%02d: %s"),t.wMonth,t.wDay, t.wHour, t.wMinute, t.wSecond,inhalt);

			return WritePlainLine(temp);
		}
		else
		{
			return WritePlainLine(inhalt);
		}
	}
	return false;
}

//=============================================================================
CStringW CTlsLogfile::GetContent(int num)
{
	FILE* s = stream;

	if (num != 0)
	{
		CString pattern = GetFilePattern();
		CString name;
		name.Format(pattern,num);

		s = _wfopen(name,L"r");

		if (s==NULL)
			return L"";
	}

	CString temp(L"");

	if (s != NULL)
	{
		fpos_t pos;
		pos = 0;

		fsetpos(s, &pos);

		wchar_t line[100];

		while (!feof(s))
		{
			if (fgetws( line, 100, s ) == NULL)
			{
				break;
			}
			else
			{
				temp += CString(line);
			}
		}
	}

	if (num != 0)
		fclose(s);

	return temp;
}

//=============================================================================
CStringW CTlsLogfile::GetFilePattern()
{
	if (m_Logfile)
	{
		//char* p = strrchr(m_Logfile->m_Filename, '.');
		wchar_t* p = const_cast<LPTSTR>(wcsrchr(m_Logfile->m_Filename, '.'));

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

/*=============================================================================
E O F
=============================================================================*/
