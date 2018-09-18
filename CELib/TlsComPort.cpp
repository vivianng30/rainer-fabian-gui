
#include "stdafx.h"
#include "TlsComPort.h"
#include "TlsStream.h"

//********************************************************************
/* New IOControlCode values */
#include "WINIOCTL.h"  
#define FILE_DEVICE_COM     0x00008097
/* Get the current speed of the I2C bus */
#define IOCTL_COM_GET_HANGPARAM \
	(DWORD) CTL_CODE(FILE_DEVICE_COM, 0x800, METHOD_BUFFERED, FILE_ANY_ACCESS)
/* Set the current speed of the I2C bus */
#define IOCTL_COM_SET_HANGPARAM \
	(DWORD) CTL_CODE(FILE_DEVICE_COM, 0x801, METHOD_BUFFERED, FILE_ANY_ACCESS)
//********************************************************************

#define COM_LOG

CTlsComPort::CTlsComPort() 
: m_hCommPort(INVALID_HANDLE_VALUE)
{
	Baud = 0;
}

CTlsComPort::~CTlsComPort()
{
    Close();
}

DWORD CTlsComPort::Open(DWORD dwIndex,DWORD dwBaudrate)
{
	CStringW cs;
	cs.Format(_T("COM%d:"),dwIndex);
	//m_hCommPort= CreateFile(cs, GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ,NULL, OPEN_EXISTING, FILE_ATTRIBUTE_SYSTEM, NULL);
	//m_hCommPort= CreateFile(cs, GENERIC_READ|GENERIC_WRITE,0,NULL, OPEN_EXISTING, FILE_FLAG_NO_BUFFERING, NULL);
	m_hCommPort= CreateFile(cs, GENERIC_READ|GENERIC_WRITE,0,NULL, OPEN_EXISTING, NULL, NULL);

	DWORD theLastError;
	//m_hCommPort= CreateFile(cs, GENERIC_READ|GENERIC_WRITE,0,NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
	if (m_hCommPort == INVALID_HANDLE_VALUE)
	{
		/*CStringW csError;
		csError.Format(_T("CRS232: Can not init. RS232, error %d"),GetLastError());
		AfxMessageBox(csError);*/

		theLastError = ::GetLastError();

		return theLastError;
		//return INVALID_HANDLE_VALUE;
	}

	//workaround from F&S -> CheckForHang
	DWORD HangParam[2];
	if(!DeviceIoControl(m_hCommPort, IOCTL_COM_GET_HANGPARAM, NULL, 0, HangParam, sizeof(HangParam), NULL, NULL))
	{
		theLastError = ::GetLastError();
		//return theLastError;
	}

	HangParam[0]=400;
	HangParam[1]=403;
	if(!DeviceIoControl(m_hCommPort, IOCTL_COM_SET_HANGPARAM,HangParam, sizeof(HangParam), NULL, 0 , NULL, NULL))
	{
		theLastError = ::GetLastError();
		//return theLastError;
	}



	// Communication settings ***********************************************
	DCB dcb;
	GetCommState( m_hCommPort, &dcb );
	dcb.DCBlength   = sizeof( dcb );
	dcb.ByteSize	= 8;
	dcb.Parity		= NOPARITY;
	dcb.StopBits	= ONESTOPBIT;
	dcb.BaudRate	= dwBaudrate;
	dcb.fInX		= FALSE;
	dcb.fOutX		= FALSE;
	dcb.fOutxDsrFlow= FALSE;
	dcb.fOutxCtsFlow= FALSE;
	dcb.fRtsControl = RTS_CONTROL_DISABLE;
	dcb.fDtrControl	= 0;
	if(!SetCommState(m_hCommPort,&dcb))
	{
		/*CStringW csError;
		csError.Format(_T("CRS232: Can not SetCommState. RS232, error %d"),GetLastError());
		AfxMessageBox(csError);*/

		theLastError = ::GetLastError();

		return theLastError;
	}
	// Readfile nonblocking *************************************************
	COMMTIMEOUTS cto;						//blocking
	cto.ReadIntervalTimeout			= 1;	//MAXDWORD;
	cto.ReadTotalTimeoutMultiplier	= 5;	//MAXDWORD;
	cto.ReadTotalTimeoutConstant	= 10;	//READ_TOTAL_TIMEOUT;
	cto.WriteTotalTimeoutConstant	= 50;
	cto.WriteTotalTimeoutMultiplier	= 5;
	SetCommTimeouts(m_hCommPort,&cto);

	
	// **********************************************************************
	DWORD dwMask=EV_RXCHAR;

	SetCommMask(m_hCommPort,dwMask);

	return 0;
}

void CTlsComPort::Close()
{
    if (m_hCommPort != INVALID_HANDLE_VALUE) 
    {
        FlushFileBuffers(m_hCommPort);
        SetEndOfFile(m_hCommPort);
       
        CloseHandle(m_hCommPort);
        m_hCommPort = INVALID_HANDLE_VALUE;
    }
}
BOOL CTlsComPort::FlushBuffer()
{
	BOOL rc = false;
	if (m_hCommPort != INVALID_HANDLE_VALUE) 
    {
        //rc = FlushFileBuffers(m_hCommPort);
		rc = PurgeComm(m_hCommPort, PURGE_RXABORT | PURGE_RXCLEAR /*| PURGE_TXABORT | PURGE_TXCLEAR*/);
    }

	return rc;
}

DWORD CTlsComPort::Write(CStringW UniString)
{
    int   Len   = UniString.GetLength();
    char  *pStr = (char *) calloc(Len+1, 1);
    DWORD	BytesToWrite;
    DWORD	BytesWritten;

    
    BytesToWrite = wcstombs(pStr ,UniString, Len);
    
    BytesWritten = Write(pStr ,BytesToWrite);
    
    free (pStr);
    
    return (BytesWritten);
}

DWORD CTlsComPort::Write(const void *pData, DWORD len)
{
    DWORD dwBytesWritten;
    DWORD Status=0;
    
    if(m_hCommPort == INVALID_HANDLE_VALUE) 
        return 0;
    
    do {
        GetCommMask(m_hCommPort, &Status);
    } while( (Status & EV_TXEMPTY) != 0);
    
    
    if(WriteFile (m_hCommPort,pData,len,&dwBytesWritten,NULL))
    {
//#ifdef COM_LOG
//        CTlsOStream_File file(_T("\\comout.txt"), true);
//        file.Write(_T(">\r\n"));
//        file.Write((char*)pData, len);
//        file.Write(_T(">\r\n"));
//#endif
    }
    else
    {
        int x = GetLastError();
        CStringW error;
        error.Format(L"Comport Write Error %d\n",x);
//#ifdef COM_LOG
        CTlsOStream_File file(_T("\\FFSDISK\\comout.txt"), true);
        file.Write(_T(">\r\n"));
        file.Write(error);
        file.Write(_T(">\r\n"));
//#endif
        Close();
        
        return 0;
    }
    
    return dwBytesWritten;
}
DWORD CTlsComPort::Read(void *pData , DWORD iData)
{
    //ASSERT(pData);
    
    if(m_hCommPort == INVALID_HANDLE_VALUE) 
        return 0;
    
    
    DWORD iRead = 0;
    
    ReadFile(m_hCommPort, pData, iData , &iRead, NULL);
    
    if(iRead > 0) 
    {
//#ifdef COM_LOG
//        CTlsOStream_File file(_T("\\comin.txt"), true);
//        file.Write(_T(">\r\n"));
//        file.Write((char*)pData, iRead);
//        file.Write(_T(">\r\n"));
//#endif
    }
    
    return iRead;
}

DWORD CTlsComPort::ReadLine(char *pData , DWORD iData)
{
    DWORD iRead = 0;
    
    if(m_hCommPort == INVALID_HANDLE_VALUE) 
        return 0;
    
    if(!pData)
        return MAXDWORD;
    
    while(true)
    {
        DWORD iLen = 0;
        ReadFile(m_hCommPort, pData+iRead, 1 , &iLen, NULL);
        if(iLen == 0)
            break;
        if(pData[iRead] == '\n')
            break;
        iRead++;
        if(iRead > iData)
            break;
    }
    pData[iRead+1] = '\0';
    
    return iRead;
}

DWORD CTlsComPort::ReadCommand(char *pData , DWORD iData)
{
	DWORD iRead = 0;

	if(m_hCommPort == INVALID_HANDLE_VALUE) 
		return 0;

	if(!pData)
		return MAXDWORD;

	while(true)
	{
		DWORD iLen = 0;
		ReadFile(m_hCommPort, pData+iRead, 1 , &iLen, NULL);
		if(iLen == 0)
			break;
		if(pData[iRead] == 'q')
			break;

		iRead++;
		if(iRead > iData)
			break;
	}
	pData[iRead+1] = '\0';
//#ifdef COM_LOG
//	CTlsOStream_File file(_T("\\com.txt"), true);
//	file.Write(L">\r\n");
//	file.Write((char*)pData, iRead);
//	file.Write(L">\r\n");
//#endif


	return iRead;
}
int CTlsComPort::ReadCO2Command(BYTE *pData , DWORD iData)
{
	
	if(m_hCommPort == INVALID_HANDLE_VALUE) 
		return 0;

	if(!pData)
		return -1;

	DWORD iLen = 0;
	DWORD iRead = 0;

	DWORD timeout = 0;
	
	/*DWORD dwStart = GetTickCount();
	DWORD dwDiff=0;*/

	ReadFile(m_hCommPort, pData+iRead, 1 , &iLen, NULL);


	/*while(iLen == 0 && timeout+20 > GetTickCount())
	{
		ReadFile(m_hCommPort, pData+iRead, 1 , &iLen, NULL);
	}*/
	if(iLen == 0)
 		return 0;

	timeout = GetTickCount();
	if((BYTE)pData[iRead]>=0x80 && (BYTE)pData[iRead]<=0xFF)//commando starts
	{
		iRead++;
		iLen = 0;
		//while(iLen == 0 && timeout+30 > getTickCount64())
		while(iLen == 0 && false==isSafeTickCountDelayExpired(timeout, 30))
		{
			ReadFile(m_hCommPort, pData+iRead, 1 , &iLen, NULL);
		}
		
		if(iLen == 0)
			return 0;
		char NBF=pData[iRead];
		iRead++;

		//if(NBF>0)
		//{
		//	ReadFile(m_hCommPort, pData+iRead, NBF , &iLen, NULL);
		//	iRead+=iLen;

		//	if(iLen!=NBF)
		//	{
		//		//error
		//		int iSTop=0;
		//	}
		//}


		timeout = GetTickCount();

		//while(NBF>0 && timeout+500 > getTickCount64())
		while(NBF>0 && false==isSafeTickCountDelayExpired(timeout, 500))
		{
			ReadFile(m_hCommPort, pData+iRead, 1 , &iLen, NULL);
			if(iLen == 0)
			{
				Sleep(0);
				continue;
			}

			NBF--;
			iRead++;
			if(iRead > iData)
				break;
		}

		if(NBF>0)
			return 0;
		
	}
	else
		return 0;
	

	return iRead;
}

bool CTlsComPort::Write(CTlsBlob &data)
{
    DWORD timeout = GetTickCount();
    
    const char * temp = (const char *)data.GetBuffer();
    int size = data.GetBufferLen();
    int pos = 0;
    DWORD written = 0;
    
    while (pos != size) 
    {
        written = Write(temp + pos,size - pos);
        
        if (written == 0) 
        {
			//if (timeout + 10000 < getTickCount64()) 
			if(isSafeTickCountDelayExpired(timeout, 10000)) 
            {
                return false;
            }
            
            Sleep(10);
        }
        else
            timeout = GetTickCount();
        
        
        pos += written;
    }
    
    return true;	
}

DWORD CTlsComPort::Read(CTlsBlob &data)
{
    DWORD timeout = GetTickCount();
    
    char * temp = (char *)data.GetBuffer();
    int size = data.GetBufferLen();
    int pos = 0;
    DWORD readed = 0;
    
    while (pos != size) 
    {
        readed = Read(temp + pos, size - pos);
        
        if (readed == 0) 
        {
			//if (timeout + 1000 < getTickCount64()) 
			if(isSafeTickCountDelayExpired(timeout, 1000)) 
			{
                return pos;
			}
            
            Sleep(10);
        }
        else
            timeout = GetTickCount();
        
        pos += readed;
    }
    
    return pos;	
}
bool CTlsComPort::isSafeTickCountDelayExpired(DWORD oldTickCount, UINT delay)////used to check if old tick count plus delay is still lower than actual tickCount, (dwLastTickCount+DELAY<getTickCount64())
{
	bool bExpired=false;
	DWORD curTickCount=GetTickCount();

	if(curTickCount<oldTickCount)//overrun
	{
		DWORD diff=0xFFFFFFFF-oldTickCount;

		if(diff+delay<curTickCount)
		{
			bExpired=true;
		}
	}
	else
	{
		if(oldTickCount+delay<curTickCount)
		{
			bExpired=true;
		}
	}
	return bExpired;
}


/*=============================================================================
    E O F
=============================================================================*/
