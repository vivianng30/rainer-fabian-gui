#include "StdAfx.h"
#include "TlsIniFile.h"
#include "TlsFile.h"
#include <string.h>
#include "TlsStream.h"

// sample
//
// [SYSTEM]
//
// # ACHTUNG ACHTUNG ACHTUNG
// # vor und nach dem '='-Zeichen sind KEINE Leerzeichen erlaubt!!!!
//
// # Installtype: kernel, auto, manual, coldboot
// InstallType=kernel
//
// [KERNEL]
//
// # installation of nboot: yes, no
// kernel=yes
//
// # installation of nboot: yes, no
// nboot=yes

CTlsIniFile::CTlsIniFile(void)
{
}
CTlsIniFile::CTlsIniFile(const char * svDateiname)
{
	m_Inifile = this;

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


CTlsIniFile::~CTlsIniFile(void)
{
	m_Inifile = NULL;

	if (m_fiPtr != NULL)
	{
		fputws(L"\n",m_fiPtr);
		fclose(m_fiPtr);
	}
}

//=============================================================================
bool CTlsIniFile::Open()
{
	m_fiPtr = _wfopen(m_Filename,L"a+");

	if (m_fiPtr == NULL)
		return false;

	return true;
}
bool CTlsIniFile::Close()
{
	if (m_fiPtr != NULL)
	{
		fputws(L"\n",m_fiPtr);
		fclose(m_fiPtr);
	}

	return true;
}


bool CTlsIniFile::ReadIniGroup( FILE *fiPtr, CStringW sGroup) 
{ 
	unsigned int i, j; 
	char buf[ 256 ]; 
	char lName[ 256 ]; 
	CStringW sCurrGroup;

	for( ;; ) 
	{ 
		fgets( buf, 255, fiPtr ); 
		if( feof( fiPtr ) ) 
		{ 
			return FALSE; 
		} 

		if( buf[ 0 ] == '[' ) 
		{ 
			j = 0; 
			for( i = 1; i < strlen( buf )-1; i++ ) 
			{ 
				if( (isprint( buf[ i ] )) && (buf[ i ] != ']') ) 
				{ 
					lName[ j ] = buf[ i ]; 
					j++; 
				} 
				else 
				{ 
					break; 
				} 
			} // for 

			if( buf[ i ] == ']' ) 
			{ 
				lName[ j ] = '\0'; 
				sCurrGroup=lName;
				if( sCurrGroup==sGroup ) 
					//if( strcasecmp( lName, groupName ) == 0 ) 
				{ 
					return TRUE;  /* group found */ 
				} 
			} 
		} /* if == '[' */ 
	} /* for(;;) */ 
} 
 
//**************************************************************************
 
bool CTlsIniFile::ReadIniField( FILE *fiPtr, CStringW sFieldName, CStringW& sData) 
{ 
	char data[256];
	char buf[256];
	char *p; 
	int i; 

	CStringW sCurrField;

	for( ;; ) 
	{ 
		fgets( buf, 255, fiPtr ); 
		if( feof( fiPtr ) ) 
		{ 
			return FALSE; 
		} 

		switch( buf[ 0 ] )
		{ 
		case '[': // a new group beginns ==> field NOT found ==> return FALSE 
			return FALSE; 
		case '#':
		case ';': 
		case '\n': // it's a comment or empty line ==> read next line 
			continue; 
		default: 
			// this line contains a field ==> check it against 'fieldName'
			// NOTE: no ' ' is allowed before AND after '=' !!!!! 
			p = strtok( buf, "=" ); 
			if( p != NULL )
			{ 
				sCurrField=p;
				if( sCurrField == sFieldName ) 
				{ 
					// field was found ==> copy string after '=' into 'data' 
					p = strtok( NULL, "\0" ); 
					for( i = 0; isprint( p[ i ] ); i++ ) 
					{ 
						data[ i ] = p[ i ]; 
					} 
					data[ i ] = '\0'; 
					sData=data;
					return TRUE; 
				} 
			} 
			break; 
		} // switch 
	} // for(;;) 




} 
 
/**************************************************************************/ 

CStringW CTlsIniFile::ReadIniStr( CStringW sGroup, CStringW sField, CStringW sDef, CStringW sFileName ) 
{ 
	CStringW buffer=_T("");

	FILE *fiPtr;
	char szFileName[255];
	wcstombs(szFileName, sFileName.GetBuffer(0), 255);
	fiPtr = fopen( szFileName, "r+b" ); 
	if( fiPtr == NULL ) 
	{ 
		return sDef; 
	} 
 

	if( ReadIniGroup( fiPtr, sGroup ) ) 
	{ 
		if(ReadIniField( fiPtr, sField, buffer))
		{
			fclose( fiPtr ); 
			return buffer; 
		}
	}

	fclose( fiPtr );
	return sDef;
}

DWORD CTlsIniFile::ReadIniDWORD( CStringW sGroup, CStringW sField, DWORD iDef, CStringW sFileName ) 
{ 
	CStringW buffer=_T("");
	DWORD retValue=0;

	FILE *fiPtr;
	char szFileName[255];
	wcstombs(szFileName, sFileName.GetBuffer(0), 255);
	fiPtr = fopen( szFileName, "r+b" ); 
	if( fiPtr == NULL ) 
	{ 
		return iDef; 
	} 


	if( ReadIniGroup( fiPtr, sGroup ) ) 
	{ 
		if(ReadIniField( fiPtr, sField, buffer))
		{
			fclose( fiPtr ); 
			retValue = _wtoi(buffer);
			return retValue; 
		}
	}

	fclose( fiPtr );
	return iDef;
}

bool CTlsIniFile::WriteIniStr( CStringW sEntry ) 
{ 
	if( m_Inifile == NULL ) 
	{ 
		return false; 
	} 


	if (m_Inifile->m_fiPtr != NULL)
	{
		fputws(sEntry,m_Inifile->m_fiPtr);
		fputws(_T("\n"),m_Inifile->m_fiPtr);
		fflush(m_Inifile->m_fiPtr);
	}

	return true;
}