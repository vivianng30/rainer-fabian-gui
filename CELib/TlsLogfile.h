
class CTlsLogfile  //: public CTlsLoglistListener
{
public:
	CStringW GetFilePattern();
	CStringW GetContent(int num);
	CStringW svLastDatum;

	/*static */bool WritePlainLine(CStringW inhalt, bool bCheckFileSize=true);
	/*static */bool WriteLine(CStringW inhalt, bool bDate=true);
	//void copyLog(bool state);

	/*static */bool CheckFileSize();

	//CTlsLogfile(const char * svPath, const char * svDateiname, int maxfiles = 1, int maxsize = 100);
	CTlsLogfile(const char * svDateiname);
	CTlsLogfile(CString svDateiname);
	bool Open();
	bool Close();
	bool Save();
	void copyLog(bool state);
	virtual ~CTlsLogfile();

protected:
	//virtual void AddMessage(CStringW sMessage);

protected:
	/*static */CTlsLogfile* m_Logfile;
	bool m_bCopyLog;

	FILE* stream;
	//char m_Filename[256];
	CString m_Filename;
	int m_maxfiles;
	int m_maxsize;
};

/*=============================================================================
E O F
=============================================================================*/
