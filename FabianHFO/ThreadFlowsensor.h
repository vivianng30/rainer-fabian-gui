/**********************************************************************************************//**
 * \file	ThreadFlowsensor.h.
 *
 * Declares the thread flowsensor class
 **************************************************************************************************/

#pragma once

class CMVModel;
// CThreadFlowsensor

class CThreadFlowsensor
{

public:
	static CThreadFlowsensor* getInstance();	
	static void destroyInstance();

	

	/**
	* @brief Start/Stop thread for flow sensor state.
	**/
	void startFlowsensorThread(void);
	void stopFlowsensorThread( void );

	void setCheckFlowsensorData(bool bState);
protected:
	CThreadFlowsensor();
	~CThreadFlowsensor();
	CMVModel *getModel();
private:
	static CThreadFlowsensor* theThreadFlowsensor;//!< singleton of ThreadFlowsensor
	CMVModel *m_pModel;//!< the model

	static friend UINT FlowsensorThread(LPVOID pc);
	CWinThread*	m_pcwtFlowsensorThread;
	DWORD FlowsensorData(void);
	bool m_bDoFlowsensorThread;
	bool m_bCheckFlowsensorData;
	HANDLE m_hThreadFlowsensor;
};
