
#if !defined(__TLSBLOB_H__)
#define __TLSBLOB_H__

#include "stdafx.h"

class CTlsInt
{
public:
    static CStringW Format(int d);
};

class CTlsCurr
{
public:
    static CStringW Format(double d);
};

class CTlsFloat
{
public:
    static double ParseSystemFloat(CStringW s);
    static double ParseUserFloat(CStringW s);
    static CStringW FormatUserFloat(double d);
    static CStringW FormatSystemFloat(double d);
	static double Round(double val, int dp);
};

class CTlsString
{
public:
	static CStringW RemoveLeadingZeroes(CStringW s);
};

//===========================================================================//
class CTlsBlob
{
  class Buffer
  {
    BYTE* m_pBuffer;
    int   m_iBuffer;
    int   m_iRefCnt;
  public:
    Buffer();
    Buffer(int iLen);
    Buffer(const BYTE* pData, int iLen);
    ~Buffer();

    void AddRef();
    void Release();
    int GetRefCnt();
    bool Resize(int iLen);
    bool Clear();
    void Fill(char c);
    Buffer* Clone();
    BYTE* GetBuffer();
    int   GetBufferLen() const;
    void Append(const BYTE* pBuffer, int iBufLen);
  };

  Buffer* m_pBuffer;

public:
  CTlsBlob();
  CTlsBlob(int iLen);
  CTlsBlob(const BYTE* p, int iLen);
  CTlsBlob(const CStringW& rString);
  CTlsBlob(const CTlsBlob& CTlsBlob);
  ~CTlsBlob();

  bool   Resize(int iLen);
  bool   Clear();
  void   Fill(char c = '\0');
  void   Set(const BYTE* pBuffer, int iBufLen);

  bool   Unify(); // Create unique Buffer for this instance (copy on write...)
  BYTE* GetBuffer();
  const BYTE* GetBufferC() const;
  int    GetBufferLen() const;
  CStringW GetString();

  bool   Compare(const CTlsBlob& CTlsBlob);
  bool   Append(const CTlsBlob& CTlsBlob);
  bool   Append(const unsigned char* pszBuf, int iLen);

  CTlsBlob& operator=(const CTlsBlob& CTlsBlob);
  bool   operator==(const CTlsBlob& CTlsBlob) const;

  CTlsBlob* Clone();
};

//
// \brief Internal Class for Enumerations
//

template <class T> class REnumeration; // forward-declaration for friend...

template <class T> class REnumeration_base
{
protected:
  virtual      ~REnumeration_base() {}
  virtual bool HasMoreElements()=0;
  virtual T    NextElement()=0;

  friend class REnumeration<T>;
};

//===========================================================================//
//
//\brief 
// 
// e.g.:
// <code>
// REnumeration e<char*> = p->GetElements();<BR>
//while(e.HasMoreElements())<BR>
// {<BR>
//   char* pszCurr = e.NextElement();<BR>
//   // .. do something ..<BR>
// }<BR>
// </code>
//
//===========================================================================//
template <class T> class REnumeration
{
	REnumeration_base<T>* m_pEnum;

public:
	// +++ constructor's & destructor +++
	REnumeration();
	REnumeration(REnumeration_base<T>* pBase);
	~REnumeration();

	// +++ iterator-functions +++
	bool HasMoreElements();
	T NextElement();

	// +++ operators +++
	void operator=(REnumeration_base<T>* pBase);
};

// +++ constructor's & destructor +++
template <class T>
inline REnumeration<T>::REnumeration()
  : m_pEnum(NULL)
{}

template <class T>
inline REnumeration<T>::REnumeration(REnumeration_base<T>* pBase)
  : m_pEnum(pBase)
{}

template <class T>
inline REnumeration<T>::~REnumeration()
{
  delete m_pEnum;
}

// +++ iterator-functions +++
template <class T>
inline bool REnumeration<T>::HasMoreElements()
{
  return m_pEnum ? m_pEnum->HasMoreElements() : false;
}

template <class T>
inline T REnumeration<T>::NextElement()
{
  return m_pEnum ? m_pEnum->NextElement() : T();
}

// +++ operators +++
template <class T>
inline void REnumeration<T>::operator=(REnumeration_base<T>* pBase)
{
  if(m_pEnum)
    delete m_pEnum;
  m_pEnum = pBase;
}

#endif
/*=============================================================================
    E O F
=============================================================================*/
