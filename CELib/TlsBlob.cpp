#include "stdafx.h"
#include "TlsBlob.h"
#include "TlsLanguage.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#include "TlsDbg.h"
#define new DEBUG_NEW
#define malloc(a) DEBUG_MALLOC(a)
#define free(a) DEBUG_FREE(a)
#define realloc(a, b) DEBUG_REALLOC(a,b)
#endif

CStringW CTlsInt::Format(int d)
{
	CStringW s;
	s.Format(L"%d", d);
	return s;
}

CStringW CTlsCurr::Format(double d)
{
    CStringW s;
    s.Format(L"%f", d);
    s.Replace('.', ',');
    return s;
}

double CTlsFloat::ParseSystemFloat(CStringW s)
{
    float fp;
    swscanf( s, L"%f", &fp );
	
    return fp;
}

double CTlsFloat::ParseUserFloat(CStringW s)
{
    s.Replace(',', '.');
    return ParseSystemFloat(s);
}

CStringW CTlsFloat::FormatUserFloat(double d)
{
    CStringW s;
    s.Format(L"%.2f", d);
    s.Replace('.', ',');
    return s;
}

CStringW CTlsFloat::FormatSystemFloat(double d)
{
    CStringW s;
    s.Format(L"%f", d);
    return s;
}

double CTlsFloat::Round(double val, int dp)
//double CTlsFloat::Round(double Zahl, int Stellen)
{
	int modifier = 1;
	for (int i = 0; i < dp; ++i)
		modifier *= 10;

	if(val < 0.0)
		return (ceil(val * modifier - 0.5) / modifier);
	return (floor(val * modifier + 0.5) / modifier);

	//double v[] = { 1, 10, 1e2, 1e3, 1e4, 1e5, 1e6, 1e7, 1e8 };  // mgl. verlängern 
	//return floor(Zahl * v[Stellen] + 0.5) / v[Stellen]; 


}

CStringW CTlsString::RemoveLeadingZeroes(CStringW s)
{
	if(s != "")
	{
		while(s.GetAt(0) == '0')
			s = s.Right(s.GetLength()-1);
	}
	return s;
}

CTlsBlob::Buffer::Buffer()
: m_pBuffer(NULL), m_iBuffer(NULL), m_iRefCnt(0)
{
}

CTlsBlob::Buffer::Buffer(int iLen)
: m_pBuffer(NULL), m_iBuffer(NULL), m_iRefCnt(0)
{
	Resize(iLen);
}

CTlsBlob::Buffer::Buffer(const BYTE* pData, int iLen)
: m_pBuffer(NULL), m_iBuffer(NULL), m_iRefCnt(0)
{
	Resize(iLen);
	memcpy(m_pBuffer, pData, iLen);
}

CTlsBlob::Buffer::~Buffer()
{
	if(m_pBuffer)
	{
		free(m_pBuffer);
	}
}

//=============================================================================
//
//\brief Referenzzähler erhöhen
//
//=============================================================================
void CTlsBlob::Buffer::AddRef()
{
	m_iRefCnt++;
}

//=============================================================================
//
//brief Referenzzähler decrementieren, bei =0 wird der Buffer gelöscht
//
//=============================================================================
void CTlsBlob::Buffer::Release()
{
	m_iRefCnt--;
	if(!m_iRefCnt)
		delete this;
}

//=============================================================================
//
//\brief Stand des Referenzzählers
//
//=============================================================================
int CTlsBlob::Buffer::GetRefCnt()
{
	return m_iRefCnt;
}

//=============================================================================
//
//\brief Resize the Buffer
//\param iLen New Length
//\return true if success
//
//  the buffer will keep it's data. 
//  the new memory is not initialized
//
//=============================================================================
bool CTlsBlob::Buffer::Resize(int iLen)
{
	if(iLen == 0)
	{
		if(m_pBuffer)
		{
			free(m_pBuffer);
		}
		
		m_pBuffer = NULL;
		m_iBuffer = 0;
		return true;
	}

	

	
	if(m_pBuffer == NULL)
	{
		m_pBuffer = (BYTE*)malloc(iLen+1);
	}
	else
	{
		BYTE *oldbuffer =m_pBuffer;
		if((m_pBuffer = (BYTE*)realloc(m_pBuffer, iLen+1))==NULL)
			free( oldbuffer );  // free original block
	}
	
	if(!m_pBuffer)
		return false;
	
	m_iBuffer = iLen;
	m_pBuffer[iLen] = '\0';
	return true;
}

//=============================================================================
//
//\brief Resize to 0
//\return true if success
//
//=============================================================================
bool CTlsBlob::Buffer::Clear()
{
	return Resize(0);
}

//=============================================================================
//
//\brief Fill the Buffer
//\param c the fill-char
//
//=============================================================================
void CTlsBlob::Buffer::Fill(char c)
{
	if(m_pBuffer)
		memset(m_pBuffer, c, m_iBuffer);
}

//=============================================================================
//
//\brief Append pBuffer to the Blob
//\param pBuffer the Data
//\param iBufLen Length of pBuffer
//
//=============================================================================
void CTlsBlob::Buffer::Append(const BYTE* pBuffer, int iBufLen)
{
	int iOffset = m_iBuffer;
	Resize(m_iBuffer + iBufLen);
	memcpy(m_pBuffer+iOffset, pBuffer, iBufLen);
}

//=============================================================================
//
//\brief Create a identical Copy of the Buffer
//\return the new Buffer
//
//=============================================================================
CTlsBlob::Buffer* CTlsBlob::Buffer::Clone()
{
	return new Buffer(m_pBuffer, m_iBuffer);
}

BYTE* CTlsBlob::Buffer::GetBuffer()
{
	return m_pBuffer;
}

int CTlsBlob::Buffer::GetBufferLen() const
{
	return m_iBuffer;
}

CTlsBlob::CTlsBlob()
: m_pBuffer(NULL)
{
}

CTlsBlob::CTlsBlob(int iLen)
{
	m_pBuffer = new Buffer(iLen);
	m_pBuffer->AddRef();
}

CTlsBlob::CTlsBlob(const BYTE* p, int iLen)
{
	m_pBuffer = new Buffer(p, iLen);
	m_pBuffer->AddRef();
}

CTlsBlob::CTlsBlob(const CTlsBlob& rBlob)
{
	m_pBuffer = rBlob.m_pBuffer;
	
	if(m_pBuffer)
		m_pBuffer->AddRef();
}

CTlsBlob::CTlsBlob(const CStringW& rString)
: m_pBuffer(NULL)
{
	CStringW s = rString;
	Resize(s.GetLength());
	int iLen = wcstombs((char*)GetBuffer(), rString, s.GetLength());
	Resize(iLen);
}

CTlsBlob::~CTlsBlob()
{
	if(m_pBuffer)
		m_pBuffer->Release();
}

bool CTlsBlob::Resize(int iLen)
{
	if(!m_pBuffer)
	{
		m_pBuffer = new Buffer(iLen);
		m_pBuffer->AddRef();
		
		if(m_pBuffer->GetBufferLen() != iLen)
		{
			//TlsMessageBox(L"CTlsBlob::Resize() Alloc failed!!", MB_OK);
			return false; // alloc failed !!
		}
		
		return m_pBuffer != NULL;
	}
	
	return m_pBuffer->Resize(iLen);
}

bool CTlsBlob::Clear()
{
	return m_pBuffer ? m_pBuffer->Clear() : NULL;
}

void CTlsBlob::Fill(char c)
{
	if(m_pBuffer)
		m_pBuffer->Fill(c);
}

void CTlsBlob::Set(const BYTE* pBuffer, int iBufLen)
{
	if(m_pBuffer)
		m_pBuffer->Release();
	
	m_pBuffer = new Buffer(pBuffer, iBufLen);
	m_pBuffer->AddRef();
}

bool CTlsBlob::Append(const unsigned char* pBuffer, int iLen)
{
	Unify();
	
	if(!m_pBuffer)
		Set(pBuffer, iLen);
	else
		m_pBuffer->Append(pBuffer, iLen);
	
	return true;
}

bool CTlsBlob::Append(const CTlsBlob& bl)
{
	return Append(bl.GetBufferC(), bl.GetBufferLen());
}

bool CTlsBlob::Unify()
{
	if(  !m_pBuffer 
		|| (m_pBuffer->GetRefCnt() == 1))
		return true;
	
	Buffer* pBuffer = m_pBuffer->Clone();
	m_pBuffer->Release();
	m_pBuffer = pBuffer;
	m_pBuffer->AddRef();
	
	return m_pBuffer != NULL;
}

BYTE* CTlsBlob::GetBuffer()
{
	return m_pBuffer ? m_pBuffer->GetBuffer() : NULL;
}

const BYTE* CTlsBlob::GetBufferC() const
{
	return m_pBuffer ? m_pBuffer->GetBuffer() : NULL;
}

int CTlsBlob::GetBufferLen() const
{
	return m_pBuffer ? m_pBuffer->GetBufferLen() : 0;
}

CTlsBlob& CTlsBlob::operator=(const CTlsBlob& bl)
{
	if(bl.m_pBuffer)
		bl.m_pBuffer->AddRef();
	
	if(m_pBuffer)
		m_pBuffer->Release();
	
	m_pBuffer = bl.m_pBuffer;
	
	return *this;
}

//=============================================================================
//
//\brief comparison-operator
//\param CTlsBlob compare-blob
//\return true if equal
//
//=============================================================================
bool CTlsBlob::operator==(const CTlsBlob& bl) const
{
	if(GetBufferLen() != bl.GetBufferLen())
		return false;
	
	return memcmp(GetBufferC(), bl.GetBufferC(), GetBufferLen()) == 0;
}

CStringW CTlsBlob::GetString()
{
	CStringW s;
	int iLen = mbstowcs(s.GetBuffer(GetBufferLen()), (char*)GetBuffer(), GetBufferLen());
	s.ReleaseBuffer(iLen);
	return s;
}

CTlsBlob* CTlsBlob::Clone()
{
	return new CTlsBlob(GetBuffer(), GetBufferLen());
}

/*=============================================================================
    E O F
=============================================================================*/
