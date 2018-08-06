

#ifndef __XMLSCAN_H
#define __XMLSCAN_H

#include <stdio.h>
//#include <iostream.h>
//#include <string.h>
//#include "CEStd/bstream.h"
#include "TlsBlob.h"
#include "TlsStream.h"
//#include "afxwin.h"

#define strncasecmp strnicmp
#define strcasecmp stricmp

#define MAX_ERRORLEN 255
#define MAX_LINELEN 255
#define MAX_IDENTLEN 50

class CTlsXmlScanner
{
	CTlsIStream_Base* m_pIs;

	// +++ for error-handling +++
  int m_iPushBack;
	int m_iCurrLine;
	int m_iCurrPos;
	char m_pszError[MAX_ERRORLEN];

	// +++ internal state +++
	enum EState {ST_NORMAL, ST_TAG};
	EState m_eState;

public:
	// +++ tokens +++
	enum EToken {TOK_EOF, TOK_ERROR, TOK_TAGSTART, TOK_TAGEND, TOK_TEXT, TOK_TAGCLOSE, 
               TOK_IDENT, TOK_EQUALS, TOK_STRING, TOK_SLASH, TOK_TYPESTART, TOK_TYPEEND, TOK_COMMENT};

	enum eTokenType {TT_BS, TT_ES, TT_ATT};

private:
	EToken m_eCurrToken;
	char m_szCurrToken[MAX_LINELEN];
	int  m_iCurrLen;
	double m_dValue;

  int  GetChar();
  void PushBack(int i);

	EToken SetError(char* pszErrorText);
	EToken Lexx();

public:
	CTlsXmlScanner(CTlsIStream_Base* pIs);

	// +++ interface of the scanner +++
	void	ClearError() { m_pszError[0] = '\0'; }
	char*	GetErrorText() { return m_pszError;}
	
	int GetCurrentLine() { return m_iCurrLine; } 
	int GetCurrentPos() { return m_iCurrPos; }

	EToken	NextToken()	{ m_eCurrToken = Lexx();	return m_eCurrToken; }
		// get's the next token from is.

	EToken	CurrToken() { return m_eCurrToken; }
		// get's the current token

	char*	CurrText() { return m_szCurrToken; }
	int		CurrTextLen() { return m_iCurrLen; }
	double	CurrNumber() { return m_dValue; }

	void DumpTokens(CTlsOStream_Base* pOs);
	void WriteXML(CTlsOStream_Base* pOs);
	
	bool Parse(CTlsXmlScanner& scanner, eTokenType& Type, CStringW& Key, CStringW& Value);
};

class CTlsXmlWriter
{
  CTlsOStream_Base* m_pOs;
  bool m_bBeautify;
  int  m_iIndent;
  bool m_bStart;

  bool m_bOpen;
  CStringW sCurrTag;

  void Indent();

public:
  CTlsXmlWriter(CTlsOStream_Base* pOs, bool bBeautify = true);
  ~CTlsXmlWriter();

  void OpenTag(char* pszTagName, bool bFinish = true);
  void OpenTag(CStringW szTagName, bool bFinish = true);
    // writes '<pszTagName'
  void WriteAttr(char* pszAttr, const char* pszValue);
    // writes 'pszAttr="pszValue"'
  void WriteAttr(char* pszAttr, CStringW pszValue);
    // writes 'pszAttr="pszValue"'
  void WriteAttr(char* pszAttr, int iValue);
    // writes 'pszAttr="pszValue"'
  void WriteAttr(char* pszAttr, double dValue);
    // writes 'pszAttr="pszValue"'
  void EndTag();
    // Writes '>'

  void WriteText(char* pszText);
    // Insert Tag-Body as Text
  void WriteText(int iText);
    // Insert Tag-Body as Text
  void WriteText(double dText);
    // Insert Tag-Body as Text
  CTlsOStream_Base* InsertBinary();
    // Insert Tag-Body as Binary
    // please don't write '<' or '>' !!

  void CloseTag(char* pszTagName=NULL, bool bInline = false);
  void CloseTag(CStringW szTagName=_T(""), bool bInline = false);
    // Writes '</pszTagName>' or '/>' depending on preceding calls...

  void WriteTextTag(CStringW szTagName, CStringW sValue);
  void WriteTextTag(char* pszTagName, CStringW sValue);
  void WriteTextTag(char* pszTagName, char* pszValue);
  void WriteTextTag(char* pszTagName, int iValue);
  void WriteTextTag(CStringW szTagName, int iValue);
  void WriteTextTag(char* pszTagName, double dValue);
  void WriteTextTag(CStringW szTagName, double dValue);
};

#endif
/*=============================================================================
    E O F
=============================================================================*/
