/**********************************************************************************************//**
 * \file	SoundPlayer.h.
 *
 * Declares the sound player class
 **************************************************************************************************/

#pragma once

#include "globDefs.h"
//#include "MVModel.h"
//#include "TlsLogfile.h"
//#include "TlsLoglist.h"
//#include "TlsTime.h"

class CMVModel;



class CSoundPlayer
{
	//SOUNDS of System
	enum eSystemSound {
		NONE	= 0,
		SINGLE_BEEP	= 1,
		DOUBLE_BEEP	= 2
	};

public:
	
	

	
	static CSoundPlayer* GetInstance();
	static void DestroyInstance();

	void SetSystemSound(eSystemSound sound, bool loop=false);
	void SetAlarmVolume(eAlarmLoudness state);
	eAlarmLoudness GetAlarmVolume();

	void SetPIFSound(ePIFSound sound);

	//void SetPICSound(eSystemSound sound, bool loop=false);

	void Init();

protected:

	CSoundPlayer(void);
	virtual ~CSoundPlayer(void);

	CMVModel *getModel();
	
private:
	//singleton
	static CSoundPlayer* theSoundPlayer;


protected:
	CMVModel *m_pModel;

	//eAlarmLoudness m_eAlarmVolume;

	//bool m_bSig;

	//BYTE m_bySignal[1];
	BYTE m_bySignal;
	BYTE m_byVolume;
	BYTE m_byAlarm;
	
};

