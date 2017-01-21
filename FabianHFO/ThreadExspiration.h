#pragma once


class CMVModel;
// CThreadExspiration

class CThreadExspiration
{

public:
	static CThreadExspiration* getInstance();	
	static void destroyInstance();

	

	/**
	* @brief Start/Stop threads for exspiration state.
	**/
	void startExspirationThread(void);
	void stopExspirationThread( void );

protected:
	CThreadExspiration();
	~CThreadExspiration();
	CMVModel *getModel();
private:
	static CThreadExspiration* theThreadExspiration;//!< singleton of CThreadExspiration
	CMVModel *m_pModel;//!< the model

	friend UINT ExspirationThread(LPVOID pc);
	CWinThread*	m_pcwtExspirationThread;
	DWORD Exspirationstart(void);
	bool m_bDoExspirationThread;
	//DWORD m_dwLastExspirationData;
	HANDLE m_hThreadExspiration;
};
