#pragma once
//#include "MVViewHandler.h"
//#include "MVModel.h"
#include "TlsBlob.h"

extern CEvent eventDoTestTimerFunctions;

typedef struct tagParaVALUE
{
	int iParaBtnID;
	int iUpperLimit;
	int iLowerLimit;
	int iStep;
	//TCHAR tText[16];
}PARAVALUE, *LPPARAVALUE;

class CMVModel;

class CAcuTestFrame
{
	//singleton
	static CAcuTestFrame* theAcuTestFrame;

	CAcuTestFrame(void);
	~CAcuTestFrame(void);

public:
	static CAcuTestFrame* GetInstance();
	static void DestroyInstance();
	//friend class CMVViewHandler;
	//friend class CMVModel;

	void startTest();

	void testParaBtn();
	void serialData(CStringW szData);
	bool checkSerialData(int iBtnID, int iVal);

	CMVModel *getModel();
protected:
	//void StartTestTimerThread(void);
	//void StopTestTimerThread(void);

protected:
	CMVModel* m_pModel;

	//CString m_szSerialData;
	CTlsBlob m_blSerialData;
	UINT m_iWaitForComand;
	//int m_numtasks;
	//struct task_tag
	//{
	//	DWORD tickcount;
	//	DWORD step;
	//	//void (*f)(void);
	//	eTimeFunc func;
	//} task[MAXTASKS];

	/*friend UINT CTestTimerThread(LPVOID pc);
	CWinThread*	m_pcwtTestTimerThread;
	DWORD DoTestTimerFunctions(void);
	bool m_bDoTestTimerFunctionsThread;
	HANDLE m_hThreadTestTimerFunctions;*/
};
