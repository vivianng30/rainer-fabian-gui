
#include "stdafx.h"
#include "TlsFile.h"
#include "TlsStream.h"

//=============================================================================
CTlsFile::CTlsFile()
{
}

//=============================================================================
CTlsFile::~CTlsFile()
{

}

//=============================================================================
//
//\brief Erstellt ein Verzeichnis
//
//=============================================================================
bool CTlsFile::MkDir(CStringW sDirectory)
{
  return CreateDirectory(sDirectory, NULL) != FALSE;
}

//=============================================================================
//
//\brief Prüft, ob die Datei existiert
//
//=============================================================================
bool CTlsFile::Exists(CStringW sPath)
{
  bool bAvailable = false;

  switch(GetFileAttributes(sPath))
  {
  case -1: bAvailable = false; break;
  case FILE_ATTRIBUTE_DIRECTORY: bAvailable = true; break;
  default: bAvailable = true; break; // Langt für unsere Zwecke
  }

  return bAvailable;
}

//=============================================================================
//
// \brief Verschiebt eine Datei
// \param sFrom
// \param sTo
// \param bOverwrite
//
//=============================================================================
bool CTlsFile::Rename(CStringW sFrom, CStringW sTo, bool)
{
  return MoveFile(sFrom, sTo) != 0;
}

//=============================================================================
//
// \brief Kopiert eine Datei
// \param sFrom       Quelldatei
// \param sTo         Zieldatei
// \param bOverwrite  Auch schreibgeschützte Datei überschreiben
//
//=============================================================================
bool CTlsFile::Copy(CStringW sFrom, CStringW sTo, bool bOverwrite)
{
    if(!CTlsFile::Exists(sFrom))
        return false;

    if(bOverwrite)
    {
        if(CTlsFile::Exists(sTo))
        {
            TRY {
                CFileStatus Status; 
                if(CFile::GetStatus(sTo, Status))
                {
                    Status.m_attribute &= ~CFile::readOnly;
                    CFile::SetStatus(sTo, Status);
                }
            } CATCH (CFileException, e) {
                return false;
            }
            END_CATCH
        }
    }

    return CopyFile(sFrom, sTo, false) != 0;
}

//=============================================================================
//
// \brief Löscht einen Datei
//
//  Falls ein Schreibschutz gesetzt ist, wird dieser vorher zurückgesetzt
//
//=============================================================================
bool CTlsFile::Delete(CStringW sFilename)
{
    if(CTlsFile::Exists(sFilename))
    {
        TRY {
            CFileStatus Status; 
            if(CFile::GetStatus(sFilename, Status))
            {
                Status.m_attribute &= ~CFile::readOnly;
                CFile::SetStatus(sFilename, Status);
            }
        } CATCH (CFileException, e) {
            return false;
        }
        END_CATCH
    }
    
    return DeleteFile(sFilename) != 0; 
}



//=============================================================================
//
// \brief Löscht ein Verzeichnis
//
//  Falls das Verzeichnis Dateien und/oder Unterverzeichnisse enthält,
//  werde diese rekursiv gelöscht.
//
//=============================================================================
bool CTlsFile::DeleteDir(CStringW sFilename)
{
    if(sFilename == "")  // Ignorieren wir (Das löscht sonst das ganze Device...)
        return false; 

	

    WIN32_FIND_DATA FileData;
    HANDLE hSearch = ::FindFirstFile(sFilename + L"\\*.*", &FileData);
    if(hSearch != INVALID_HANDLE_VALUE)
    {
        do
        {
            CStringW sFile = sFilename + L"\\" + FileData.cFileName;
			Delete(sFile);
            DeleteDir(sFile);    
			Sleep(0);
        } while(::FindNextFile(hSearch, &FileData));

		FindClose(hSearch);
    }
	


    return RemoveDirectory(sFilename) != 0;
}


//=============================================================================
//
// \brief Liefert die Dateigröße in Bytes
//
//=============================================================================
int CTlsFile::GetSize(CStringW sPath)
{
  int iSize = 0;
  if(!Exists(sPath))
	  return -1;

  HANDLE hFile = CreateFile(CStringW(sPath), 
                            GENERIC_READ, 
                            FILE_SHARE_READ | FILE_SHARE_WRITE, 
                            NULL, 
                            OPEN_EXISTING, 
                            0, 
                            NULL);
  if(hFile != INVALID_HANDLE_VALUE)
  {
    iSize = GetFileSize(hFile, NULL);
    CloseHandle(hFile);
  }

  return iSize;
}

//=============================================================================
//
// \brief Liefert einen eindeutigen Dateinamen
//
// <Path>/<Prefix>200307311022<nummer><postfix>
//
//=============================================================================
CStringW CTlsFile::GetUniqueFileName(CStringW sPath, CStringW sPrefix, CStringW sPostfix)
{
  static int _LastImportTimeCounter = 0;
	static CStringW _LastImportTime = L"";

	SYSTEMTIME systime;
	GetSystemTime(&systime);
	CTime ct = CTime(systime);


	//CTime ct = CTime::GetCurrentTime();
	CStringW zeit;

	zeit.Format(L"%.4d%.2d%.2d%.2d%.2d%.2d",
		ct.GetYear(),
		ct.GetMonth(),
		ct.GetDay(),
		ct.GetHour(),
		ct.GetMinute(),
		ct.GetSecond());

	if(zeit == _LastImportTime)
		_LastImportTimeCounter++;
	else
	{
		_LastImportTime = zeit;	
		_LastImportTimeCounter = 0;
	}

	CStringW dateiname;
	dateiname.Format(L"%s\\%s#%.4d%s",sPath,zeit,_LastImportTimeCounter,sPostfix);

  return dateiname;
}

//=============================================================================
//
//\brief Liefert die Dateiversion
//
//=============================================================================
CStringW CTlsFile::GetFileVersion(CStringW sPath)
{
	CStringW version = L"-";

	char* m_pVersionInfo;

	DWORD dwVersionInfoSize;
	DWORD dwZero = 0;  

	dwVersionInfoSize = GetFileVersionInfoSize(sPath.GetBuffer(0), &dwZero);
	m_pVersionInfo = NULL;

	if (dwVersionInfoSize)
	{
		m_pVersionInfo = new char[dwVersionInfoSize];
		if (GetFileVersionInfo(sPath.GetBuffer(0), 0, dwVersionInfoSize, m_pVersionInfo))
		{
			TCHAR fv[] = L"FileVersion";
			for (UINT i = 0; i < dwVersionInfoSize; ++i)
			{
				if (memcmp(&m_pVersionInfo[i], fv, sizeof(fv)) == 0)
				{
					if( !(TCHAR)m_pVersionInfo[i+sizeof(fv)])
						i += sizeof( TCHAR);
					version = (TCHAR*)&m_pVersionInfo[i+sizeof(fv)];
				}
			}
		}
		delete[] m_pVersionInfo;
	}

	version.Remove(' ');
	version.Replace(',', '.');

	return version;
}

/*=============================================================================
    E O F
=============================================================================*/
