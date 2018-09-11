/**********************************************************************************************//**
 * \file	ThreadMonitor.h.
 *
 * Declares the thread monitor class
 **************************************************************************************************/

#pragma once



class CMVModel;
// CThreadMonitor

class CThreadMonitor
{

public:
	static CThreadMonitor* getInstance();	
	static void destroyInstance();

	

	/**
	* @brief Start/Stop thread for SPI (Monitor PIC) communication.
	**/
	void startMonitorThread(void);
	void stopMonitorThread( void );

protected:
	CThreadMonitor();
	~CThreadMonitor();
	CMVModel *getModel();
private:
	static CThreadMonitor* theThreadMonitor;//!< singleton of ThreadMonitor
	CMVModel *m_pModel;//!< the model

	static friend UINT MonitorThread(LPVOID pc);
	CWinThread*	m_pcwtMonitorThread;
	DWORD MonitorData(void);
	bool m_bDoMonitorThread;
	HANDLE m_hThreadMonitor;
};


