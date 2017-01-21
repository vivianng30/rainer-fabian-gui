// SoundPlayer.cpp : implementation file
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "SoundPlayer.h"
#include "MVModel.h"
//#include "Library/TlsRegistry.h"

//#include <mmsystem.h>

#define MASK_SIGBIT	0x10

CSoundPlayer* CSoundPlayer::theSoundPlayer=0;

CSoundPlayer::CSoundPlayer(void)
{
	

	//m_eAlarmVolume=getModel()->getCONFIG()->GetAlarmVolume();
	//m_eAlarmVolume=ALARMMEDIUM;
	m_byVolume=ALARMMEDIUM;
	m_byAlarm=0;
	m_bySignal=0;

	m_pModel = NULL;

}

CSoundPlayer::~CSoundPlayer(void)
{
	

	
}

// **************************************************************************
// 
// **************************************************************************
CMVModel *CSoundPlayer::getModel()
{
	if(m_pModel==NULL)
		m_pModel=CMVModel::GetInstance();
	return m_pModel;
}

// **************************************************************************
// 
// **************************************************************************
CSoundPlayer* CSoundPlayer::GetInstance()
{
	if(theSoundPlayer == 0)
	{
		theSoundPlayer = new CSoundPlayer;
	}
	return theSoundPlayer;
}

// **************************************************************************
// 
// **************************************************************************
void CSoundPlayer::DestroyInstance()
{

	if(theSoundPlayer != NULL)
	{
		delete theSoundPlayer;
		theSoundPlayer = NULL;
	}
}



void CSoundPlayer::Init()
{
	BYTE data[1];
	DWORD dwBytesToWrite=1;
	data[0] = 0;
	
	getModel()->getPIF()->SendData(data, dwBytesToWrite);

	m_byVolume=(BYTE)getModel()->getCONFIG()->GetAlarmVolume();
}


void CSoundPlayer::SetSystemSound(eSystemSound sound, bool loop)
{
	switch(sound)
	{
	case NONE:
		{
			PlaySound(NULL, NULL, 0);
		}
		break;
	case SINGLE_BEEP:
		{
			if(loop)
				PlaySound(_T("\\FFSDISK\\beep.wav"), NULL, SND_ASYNC | SND_LOOP | SND_FILENAME);
			else
				PlaySound(_T("\\FFSDISK\\beep.wav"), NULL, SND_ASYNC | SND_FILENAME);
		}
		break;
	case DOUBLE_BEEP:
		{
			if(loop)
				PlaySound(_T("\\FFSDISK\\Chip2.wav"), NULL, SND_FILENAME | SND_LOOP | SND_ASYNC);
			else
				PlaySound(_T("\\FFSDISK\\Chip2.wav"), NULL, SND_FILENAME | SND_ASYNC);
		}
		break;
	default:
		break;
	}
}

void CSoundPlayer::SetAlarmVolume(eAlarmLoudness state)
{
	m_byVolume=(BYTE)state;

	BYTE data[1];
	DWORD dwBytesToWrite=1;
	data[0] = m_byVolume;
	int iBytesWritten=getModel()->getPIF()->SendData(data, dwBytesToWrite);
	DEBUGMSG(TRUE, (TEXT("SetAlarmVolume %d\r\n"),m_byVolume));
	if(iBytesWritten!=dwBytesToWrite)
	{
		theApp.getLog()->WriteLine(_T("#HFO:0261"));
		//getModel()->getALARMHANDLER()->SetAlarm_IF_PIF(_T(""));
		if(AfxGetApp())
			AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_IF_PIF);
	}

	//m_eAlarmVolume=state;
	/*unsigned long volume=0x00000000;

	switch(m_eAlarmVolume)
	{
	case ALARMSILENT:
	{
	volume=0x33333333;
	}
	break;
	case ALARMMEDIUM:
	{
	volume=0x99999999;
	}
	break;
	case ALARMLOUD:
	{
	volume=0xFFFFFFFF;
	}
	break;
	}
	waveOutSetVolume(HWAVEOUT WAVE_MAPPER,volume);*/

	getModel()->getCONFIG()->SetAlarmVolume(state);
}

// **************************************************************************
// 
// **************************************************************************
eAlarmLoudness CSoundPlayer::GetAlarmVolume()
{
	return (eAlarmLoudness)m_byVolume;
}

// **************************************************************************
// 
// **************************************************************************
void CSoundPlayer::SetPIFSound(ePIFSound sound)
{
	BYTE data[1];
	DWORD dwBytesToWrite=1;
	switch(sound)
	{
	case PIF_NONE:
		{
			m_byAlarm=0;

			//data[0] = 0;
			data[0] = m_bySignal + m_byVolume;
			int iBytesWritten=getModel()->getPIF()->SendData(data, dwBytesToWrite);
			if(iBytesWritten!=dwBytesToWrite)
			{
				theApp.getLog()->WriteLine(_T("#HFO:0261"));
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_IF_PIF);
			}
		}
		break;
	case PIF_SIGNAL:
		{
			m_bySignal = m_bySignal ^ MASK_SIGBIT;
			//data[0] = m_bySignal + m_byVolume + m_byAlarm;
			data[0] = m_bySignal + m_byVolume + m_byAlarm;

			int iBytesWritten=getModel()->getPIF()->SendData(data, dwBytesToWrite);
			//DEBUGMSG(TRUE, (TEXT("PIF_SIGNAL %d\r\n"),data[0]));
			if(iBytesWritten!=dwBytesToWrite)
			{
				theApp.getLog()->WriteLine(_T("#HFO:0261"));
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_IF_PIF);
			}
		}
		break;
	case PIF_ALARM1:
		{
			m_byAlarm = 0x1;
			data[0] = m_bySignal + m_byVolume + m_byAlarm;

			int iBytesWritten=getModel()->getPIF()->SendData(data, dwBytesToWrite);
			if(iBytesWritten!=dwBytesToWrite)
			{
				theApp.getLog()->WriteLine(_T("#HFO:0261"));
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_IF_PIF);
			}
		}
		break;
	case PIF_ALARM2:
		{
			m_byAlarm = 0x2;
			data[0] = m_bySignal + m_byVolume + m_byAlarm;

			int iBytesWritten=getModel()->getPIF()->SendData(data, dwBytesToWrite);
			if(iBytesWritten!=dwBytesToWrite)
			{
				theApp.getLog()->WriteLine(_T("#HFO:0261"));
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_IF_PIF);
			}
		}
		break;
	case PIF_ALARM3:
		{
			m_byAlarm = 0x3;
			data[0] = m_bySignal + m_byVolume + m_byAlarm;

			int iBytesWritten=getModel()->getPIF()->SendData(data, dwBytesToWrite);
			if(iBytesWritten!=dwBytesToWrite)
			{
				theApp.getLog()->WriteLine(_T("#HFO:0261"));
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_IF_PIF);
			}
		}
		break;
	case PIF_SHUTDOWN:
		{
			m_byAlarm = 0x20;
			data[0] = m_bySignal + m_byVolume + m_byAlarm;

			int iBytesWritten=getModel()->getPIF()->SendData(data, dwBytesToWrite);
			if(iBytesWritten!=dwBytesToWrite)
			{
				theApp.getLog()->WriteLine(_T("#HFO:0262"));
			}
		}
		break;
	default:
		break;
	}
}
