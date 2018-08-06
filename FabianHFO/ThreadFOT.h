/**********************************************************************************************//**
 * \file	ThreadFOT.h.
 *
 * Declares the thread fot class
 **************************************************************************************************/

#pragma once
#include "globDefs.h"

//typedef enum
//{
//	FOT_START,
//	FOT_VENTDELAY,
//	FOT_WAITVALIDDATA,
//	FOT_ACQUIREDATA,
//	FOT_RETRY,
//	FOT_CALCULATEFOT,
//} SequenceStatesFOT;

class CMVModel;
// CThreadFOT

class CThreadFOT
{

public:
	static CThreadFOT* getInstance();	
	static void destroyInstance();

	void loadHFO_FOTvalues();

	/**
	* @brief Start/Stop thread FOT
	**/
	void startFOTThread(void);
	void stopFOTThread( void );

	void writeFOTventdataBuffer(int iPRESSURE, double iFlow);
	void resetFOTventdataBuffer();

	BYTE getBufSizeFOTdisplay();
	void resetFOTdisplayBuffer();

	void calculateFOTdata(int i_osc_freq,WORD curPressure);

	void startFOT();
	void stopFOT();

	void startFOTconventional();
	void stopFOTconventional();

	bool isFOTrunning();

	void changeToFOTConvVentMode();
	void restoreFOTConvVentMode();
	void restoreFOTHFOVentMode();

	void startFOThfo();
	void stopFOThfo(/*bool bRestoreOld*/);

	CString getDateLastSequence();
	void resetDateLastSequence();
	
	WORD getOriginHFAMPLPara();

	void runStep();
	void continueSequence();
	void decreaseSequence();
	void repeatSequence();
	void resetLastDisplayBuf();

	bool isFOTDataValid();
	//bool isFOTsequenceRunning();
	bool isAcquireFOTData();
	BYTE getCurFOTsequence();

	//WORD getCurrentFOTPIP();
	WORD getCurrentFOTPEEP();
	WORD getCurrentFOTHFPmean();
	WORD getFOTHFPmeanStep();
	WORD getFOTPEEPStep();
	SHORT getFOToriginDiffPEEP_PINSP();
	SHORT getFOToriginDiffPEEP_PMAXVG();
	SHORT getFOToriginPINSPPara_IPPV();
	SHORT getFOToriginPMAXVGPara_IPPV();
	bool isFOToriginVGstateOn();
	
	bool isDecreasingSequence();

	SequenceStatesFOT getFOTstate();

	eRetryError getRetryERROR();

protected:
	CThreadFOT();
	virtual ~CThreadFOT();
	CMVModel *getModel();

private:
	bool doThread();
	void startThread();
	void stopThread();

	void calcParaFOTCONV();
	void setParaFOTCONV(bool bRetry);
	void setDecreaseParaFOTCONV(bool bIncreaseSeq);
	void calcDecreaseParaFOTCONV();

	void calcParaFOTHFO();
	void setParaFOTHFO(bool bRetry);
	void setDecreaseParaFOTHFO(bool bIncreaseSeq);
	void calcDecreaseParaFOTHFO();

	
	void startCalculation();
	bool isNextSequence();
	void startFOToscillation();
	void setDateLastSequence();

	void setRetry(eRetryError error);
	void resetRetryERROR();

	void setFOTstate(SequenceStatesFOT feState);
	void checkFOTvalidMeasurementData();
	void setFOTDataValid(bool validData, eRetryError error=RETRY_NONE);
	//void setFOTsequenceRunning(bool state);
	void increaseFOTsequence();
	void resetFOTsequence();

	//FOT functions
	void fnCalcZ(double** pp_Fourier, double** pp_PseudoInverse,double** pp_Flow,double** pp_Pressure,int i_osc_freq,int i_sample_freq,double *pOut);
	void fnConstructPseudoInverse(int i_osc_freq,int i_sample_freq,double** pp_Fourier,double** pp_PseudoInverse);
	void fnConstructFourier(int i_osc_freq,int i_sample_freq,double** pp_Fourier); 
	void fnInverse(double **pp_A,int n_order_A,double **pp_A_inv);
	void fnCofactor(double **pp_A,int n_order_A,double **pp_CoFactor);
	double fnDeterminant(double **pp_A,int n_order_A);
	void fnMinor(double** pp_A,int n_order_A,int row_skip,int col_skip,double** pp_Minor);
	void fnMultiplication(double **pp_A,int n_rows_A,int n_cols_A, double **pp_B, int n_cols_B, double **pp_C);
	void fnTranspose(double **pp_A,int n_rows_A,int n_cols_A,double **pp_A_Transpose);

public:
	static PBUFFOTcalc* m_pbufFOTdisplay;
	BYTE m_iCountFOTdisplay;

private:
	static CThreadFOT* theThreadFOT;//!< singleton of ThreadPRICO
	CMVModel *m_pModel;//!< the model

	CRITICAL_SECTION	csFOTthread;
	CRITICAL_SECTION	csFOTvalidData;//!< critical section for FOT data
	CRITICAL_SECTION	csFOTstate;//!< critical section for FOT state machine
	CRITICAL_SECTION	csFOTsequence;//!< critical section for FOT sequence
	CRITICAL_SECTION	csFOTcalcBuffer;//!< critical section for FOT sequence
	CRITICAL_SECTION	csFOTventBuffer;//!< critical section for FOT sequence
	CRITICAL_SECTION	csFOTdate;//!< critical section for FOT date

	friend UINT FOTThread(LPVOID pc);
	CWinThread*	m_pcwtFOTThread;
	DWORD FOTData(void);
	bool m_bDoFOTThread;
	HANDLE m_hThreadFOT;

	CStringW m_szLastCalculation;

	SequenceStatesFOT feFOTstate;

	eRetryError m_eRetryError;

	static PBUFFOTvent* m_pbufFOTventilation;
	WORD m_ibufCountFOTventilation;
	
	bool m_bFOTvalidData;

	//bool m_bFOTrunning;
	bool m_bFOTconvRunning;
	bool m_bFOThfoRunning;
	//bool m_bFOTsequenceRunning;
	
	SHORT m_iFOToriginPEEPPara_IPPV;
	SHORT m_iFOToriginPMAXVGPara_IPPV;
	SHORT m_iFOToriginPINSPPara_IPPV;
	SHORT m_iFOToriginPpsvPara;
	SHORT m_iFOToriginDiffPEEP_PINSP;
	SHORT m_iFOToriginDiffPEEP_Ppsv;
	SHORT m_iFOToriginDiffPEEP_PMAXVG;
	int m_iFOToriginPIPlowPEEPAlimitDiff;
	bool m_iFOToriginVGstate;

	eRatioIE m_eFOToriginHFIERatioPara;
	BYTE m_iFOToriginHFFreqPara;
	WORD m_iFOToriginHFAMPLPara;

	WORD m_iFOTPEEPStep;
	SHORT m_icurFOTPIPMAXVG;
	SHORT m_icurFOTPIP;
	SHORT m_icurFOTPSV;
	WORD m_icurFOTPEEP;

	WORD m_iFOTHFPmeanStep;
	WORD m_icurFOTHFPmean;
	BYTE m_iFOTsequence;
	BYTE m_iFOTdisplaySequence;
	WORD m_iCountFOTimer;
	bool m_bDecreasing;
	//bool m_bTestRetry;
};


