#pragma once

class CTlsIniFile
{
public:
	CTlsIniFile(void);
	CTlsIniFile(const char * svDateiname);
	~CTlsIniFile(void);

	bool Open();
	bool Close();

	static CStringW ReadIniStr( CStringW sGroup, CStringW sField, CStringW sDef, CStringW sFileName );
	static DWORD ReadIniDWORD( CStringW sGroup, CStringW sField, DWORD iDef, CStringW sFileName );
	static bool ReadIniField( FILE *fiPtr, CStringW sFieldName, CStringW& sData);
	static bool ReadIniGroup( FILE *fiPtr, CStringW sGroup);
	bool WriteIniStr( CStringW sEntry );

protected:
	CTlsIniFile* m_Inifile;
	CString m_Filename;
	FILE* m_fiPtr;
};
