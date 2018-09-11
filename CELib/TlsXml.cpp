
#include "stdafx.h"
#include "TlsXml.h"

#include <ctype.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#include "TlsDbg.h"
#define new DEBUG_NEW
#define malloc DEBUG_MALLOC
#define free DEBUG_FREE
#define realloc DEBUG_REALLOC
#endif

CTlsXmlScanner::EToken CTlsXmlScanner::SetError(char* pszErrorText)
{
	sprintf(m_pszError, "%s", pszErrorText);
	return TOK_ERROR;
}

CTlsXmlScanner::CTlsXmlScanner(CTlsIStream_Base* pIs)
	: m_pIs(pIs), m_eState(ST_NORMAL), m_iCurrLen(0), m_iCurrLine(1), m_iCurrPos(0)
    
{
	m_iPushBack=0;
	m_pszError[0] = '\0';
	m_szCurrToken[0] = '\0';
}

int  CTlsXmlScanner::GetChar()
{
  int i = m_pIs->ReadChar();
  if(i == '\r')
    m_iCurrLine++;
  return i;
}

void CTlsXmlScanner::PushBack(int i)
{
  m_pIs->PushBack(i);
  if(i == '\n')
    m_iCurrLine--;  
}

CTlsXmlScanner::EToken CTlsXmlScanner::Lexx()
		// get's the next token from is.
{
	m_iCurrLen = 0;
	while(true)
	{
		int i = GetChar();
		if(i == -1)
		{
			m_eCurrToken = TOK_EOF;
			m_szCurrToken[m_iCurrLen] = '\0';
			return m_eCurrToken;
		}
		char c = (char)i;
		
		if(m_eState == ST_NORMAL)
		{
			switch(c)
			{
			case '<':
				{
					i = GetChar();
					if(i == -1)
					{
						m_eCurrToken = TOK_ERROR;
						m_szCurrToken[m_iCurrLen] = '\0';
						return m_eCurrToken;
					}
					c = (char)i;
					if(c == '/')
					{
						m_eCurrToken = TOK_TAGEND;
						i = GetChar();
						c = (char)i;
						m_szCurrToken[0] = '\0';
						while(true)
						{
							m_szCurrToken[m_iCurrLen++] = c;
							i = GetChar();
							if(i == -1)
							{
								m_eCurrToken = TOK_ERROR;
								m_szCurrToken[m_iCurrLen] = '\0';
								return m_eCurrToken;
							}
							
							c = (char)i;
							if(c == '>')
							{
								m_szCurrToken[m_iCurrLen] = '\0';
								return m_eCurrToken;
							}
						}
					}
					else if(c == '?')
					{
						m_eCurrToken = TOK_TYPESTART;
						m_eState     = ST_TAG;
						m_szCurrToken[0] = '\0';
						m_iCurrLen = 0;
						while(true)
						{
							m_szCurrToken[m_iCurrLen++] = c;
							i = GetChar();
							if(i == -1)
							{
								m_eCurrToken = TOK_ERROR;
								m_szCurrToken[m_iCurrLen] = '\0';
								return m_eCurrToken;
							}
							
							c = (char)i;
							if(c == '>')
							{
								PushBack(c);
								m_szCurrToken[m_iCurrLen] = '\0';
								return m_eCurrToken;
							}
							if(c == ' ' || c == '\t' || c == '\n' || c == '\r')
							{
								m_szCurrToken[m_iCurrLen] = '\0';
								return m_eCurrToken;
							}
						}
					}
					else if(c == '!')
					{
						m_eCurrToken = TOK_COMMENT;
						//m_eState     = ST_TAG;
						m_szCurrToken[0] = '\0';
						m_iCurrLen = 0;
						
						int iState = 0;
						while(iState < 3)
						{
							i = GetChar();
							if(i == -1)
							{
								m_eCurrToken = TOK_ERROR;
								m_szCurrToken[0] = '\0';
								return m_eCurrToken;
							}
							c = (char)i;
							switch(iState)
							{
							case 0: 
								if(c == '-')
									iState = 1;
								break;
							case 1:
								if(c == '-')
									iState = 2;
								else
									iState = 0;
								break;
							case 2:
								if(c == '>')
									iState = 3;
								else
									iState = 0;
								break;
							}
						}
					}
					else
					{
						m_eCurrToken = TOK_TAGSTART;
						m_eState     = ST_TAG;
						m_szCurrToken[0] = '\0';
						m_iCurrLen = 0;
						while(true)
						{
							m_szCurrToken[m_iCurrLen++] = c;
							i = GetChar();
							if(i == -1)
							{
								m_eCurrToken = TOK_ERROR;
								m_szCurrToken[m_iCurrLen] = '\0';
								return m_eCurrToken;
							}
							
							c = (char)i;
							if(c == '>' || c == '/')
							{
								PushBack(c);
								m_szCurrToken[m_iCurrLen] = '\0';
								return m_eCurrToken;
							}
							if(c == ' ' || c == '\t' || c == '\n' || c == '\r')
							{
								m_szCurrToken[m_iCurrLen] = '\0';
								return m_eCurrToken;
							}
						}
					}
        }
        break;
        
      default:
		  {
			  m_szCurrToken[0] = '\0';
			  boolean bSpaceOnly = true;
			  while(true)
			  {
				  if(c != '\n' && c != '\r' && c != ' ' && c != '\t')
					  bSpaceOnly = false;
				  
				  if(c == '&')
				  {
					  char szBuf[10];
					  szBuf[0] = '&';
					  int  iBuf = 1;
					  while(true)
					  {
						  int c2 = GetChar();
						  szBuf[iBuf++] = c2;
						  if(c2 == -1)
							  break;
						  if(c2 == ';')
							  break;
						  if(c2 == '<')
						  {
							  PushBack(c2);
							  iBuf--;
							  break;
						  }
						  if(iBuf == 10)
							  break;
					  }
					  
					  if(strncmp(szBuf, "&lt", 3)==0)
						  m_szCurrToken[m_iCurrLen++] = '<';
					  else if(strncmp(szBuf, "&gt", 3)==0)
						  m_szCurrToken[m_iCurrLen++] = '>';
					  else if(strncmp(szBuf, "&quot", 5)==0)
						  m_szCurrToken[m_iCurrLen++] = '\"';
					  else if(strncmp(szBuf, "&apos", 5)==0)
						  m_szCurrToken[m_iCurrLen++] = '\'';
					  else if(strncmp(szBuf, "&amp", 4)==0)
						  m_szCurrToken[m_iCurrLen++] = '&';
					  else if(strncmp(szBuf, "&#", 2)==0)
						  m_szCurrToken[m_iCurrLen++] = atoi(szBuf+2);
					  else 
					  {
						  for(int i3 = 0; i3 < iBuf; i3++)
							  m_szCurrToken[m_iCurrLen++] = szBuf[i3];
					  }
					  i = GetChar();
				  }
				  else
				  {
					  m_szCurrToken[m_iCurrLen++] = c;
					  i = GetChar();
				  }
				  
				  c = (char)i;
				  
				  if(i == -1)
				  {
					  if(bSpaceOnly)
						  m_eCurrToken = TOK_EOF;
					  else
						  m_eCurrToken = TOK_TEXT;
					  
					  m_szCurrToken[m_iCurrLen] = '\0';
					  return m_eCurrToken;
				  }
				  
				  if(c == '<')
				  {
					  PushBack(c);
					  
					  if(bSpaceOnly)
						  break;
					  else
					  {
						  m_eCurrToken = TOK_TEXT;
						  m_szCurrToken[m_iCurrLen] = '\0';
						  return m_eCurrToken;
					  }
				  }
			  }
		  } //
      } // switch
    }
    else
    {
		switch(c)
		{
		case '?':
			{
				int i = GetChar();
				if(i == -1)
				{
					m_eCurrToken = TOK_ERROR;
				}
				else if(i == '>')
				{
					m_eCurrToken = TOK_TYPEEND;
					m_eState     = ST_NORMAL;
				}
				else
					m_eCurrToken = TOK_ERROR;
				
				return m_eCurrToken;
			}
			
		case '>':
			m_eCurrToken = TOK_TAGCLOSE;
			m_eState     = ST_NORMAL;
			return m_eCurrToken;
		case '=':
			m_eCurrToken = TOK_EQUALS;
			return m_eCurrToken;
		case '\"':
			{
				m_eCurrToken = TOK_STRING;
				m_szCurrToken[0] = '\0';
				while(true)
				{
					i = GetChar();
					c = (char)i;
					
					if(c == -1)
					{
						SetError(" \" erwartet");
						m_szCurrToken[m_iCurrLen] = '\0';
						return m_eCurrToken;
					}
					
					if(c == '\"')
					{
						m_szCurrToken[m_iCurrLen] = '\0';
						return m_eCurrToken;
					}
					
					m_szCurrToken[m_iCurrLen++] = c;
				}
			}
			// break;
			
		case '/':
			{
				m_eCurrToken = TOK_SLASH;
				return m_eCurrToken;
			}
			
		case ' ':
		case '\t':
		case '\r':
		case '\n':
			break;
			
		default:
			{
				if(isalpha(c))
				{
					m_eCurrToken = TOK_IDENT;
					m_szCurrToken[0] = '\0';
					while(true)
					{
						m_szCurrToken[m_iCurrLen++] = c;
						i = GetChar();
						c = (char)i;
						
						if(c == -1)
						{
							m_szCurrToken[m_iCurrLen] = '\0';
							return m_eCurrToken;
						}
						
						if(!isalnum(c))
						{
							PushBack(c);
							m_szCurrToken[m_iCurrLen] = '\0';
							return m_eCurrToken;
						}
					}
				}
				else
				{
					SetError("Unerwartetes Zeichen: " + c);
					m_szCurrToken[m_iCurrLen] = '\0';
					return m_eCurrToken;
				}
			}
			
		}
    }
  }
}

void CTlsXmlScanner::DumpTokens(CTlsOStream_Base* pOs)
{
  while(true)
  {
    switch(NextToken())
    {
    case TOK_EOF:
      pOs->Write("TOK_EOF");
      pOs->Write("\n");
      return;
    case TOK_ERROR:
      pOs->Write("TOK_ERROR ");
      pOs->Write(GetErrorText());
      return;
    case TOK_TAGSTART:
      pOs->Write("TOK_TAGSTART ");
      pOs->Write(CurrText());
      break;
    case TOK_TAGEND:
      pOs->Write("TOK_TAGEND ");
      pOs->Write(CurrText());
      break;
    case TOK_TYPESTART:
      pOs->Write("TOK_TYPESTART ");
      pOs->Write(CurrText());
      break;
    case TOK_TYPEEND:
      pOs->Write("TOK_TYPEEND ");
      pOs->Write(CurrText());
      break;
    case TOK_TEXT:
      pOs->Write("TOK_TEXT ");
      pOs->Write(CurrText());
      break;
    case TOK_TAGCLOSE:
      pOs->Write("TOK_TAGCLOSE");
      break;
    case TOK_IDENT:
      pOs->Write("TOK_IDENT ");
      pOs->Write(CurrText());
      break;
    case TOK_EQUALS:
      pOs->Write("TOK_EQUALS");
      break;
    case TOK_STRING:
      pOs->Write("TOK_STRING ");
      pOs->Write(CurrText());
      break;
    }
    pOs->Write("\n");
  }
}

//=============================================================================
void CTlsXmlScanner::WriteXML(CTlsOStream_Base* pOs)
{
  while(true)
  {
    switch(NextToken())
    {
    case TOK_EOF:
      //pOs->Write("TOK_EOF");
      pOs->Write("\n");
      return;
    case TOK_ERROR:
      pOs->Write("TOK_ERROR ");
      pOs->Write(GetErrorText());
      return;
    case TOK_TAGSTART:
      pOs->Write("<");
      pOs->Write(CurrText());
      break;
    case TOK_TAGEND:
      pOs->Write("</");
      pOs->Write(CurrText());
	  pOs->Write(">");
      break;
    case TOK_TYPESTART:
      pOs->Write("TOK_TYPESTART ");
      pOs->Write(CurrText());
      break;
    case TOK_TYPEEND:
      pOs->Write("TOK_TYPEEND ");
      pOs->Write(CurrText());
      break;
    case TOK_TEXT:
      //pOs->Write("TOK_TEXT ");
      pOs->Write(CurrText());
      break;
    case TOK_TAGCLOSE:
      pOs->Write(">");
      break;
    case TOK_IDENT:
      pOs->Write("TOK_IDENT ");
      pOs->Write(CurrText());
      break;
    case TOK_EQUALS:
      pOs->Write("TOK_EQUALS");
      break;
    case TOK_STRING:
      pOs->Write("TOK_STRING ");
      pOs->Write(CurrText());
      break;
    }
    //pOs->Write("\n");
  }
}

//=============================================================================
bool CTlsXmlScanner::Parse(CTlsXmlScanner& scanner, eTokenType& Type, CStringW& Key, CStringW& Value)
{
	switch(scanner.CurrToken())
	{
	case CTlsXmlScanner::TOK_TAGSTART:
		{
			Key = scanner.CurrText();
			Key.MakeLower();

			if(scanner.NextToken() == CTlsXmlScanner::TOK_SLASH)
			{
				// <Key /^> NT
				scanner.NextToken();
				scanner.NextToken();
				Type = TT_ATT;
				Value = "";
			}
			else if(scanner.CurrToken() == CTlsXmlScanner::TOK_TAGEND)
			{
				// leeres Attribut
				Type = TT_ATT;
				scanner.NextToken();
				Value = "";
			}
			else
			{ 
				scanner.NextToken();

				switch(scanner.CurrToken())
				{
				case CTlsXmlScanner::TOK_TEXT:
					{
						Value = scanner.CurrText();
						Type = TT_ATT;
                
						scanner.NextToken(); // TOK_TAGEND
						scanner.NextToken();
					}
					break;
				case CTlsXmlScanner::TOK_TAGEND:
					{
						Value = scanner.CurrText();
						Type = TT_ATT;
                
						scanner.NextToken();
					}
					break;
				default:
					{
						Type = TT_BS;
					}
					break;
				}
				
			}
		}
		break;
	case CTlsXmlScanner::TOK_TAGEND:
		{
			Type = TT_ES;
			scanner.NextToken();
		}
		break;
	default:
		SetError("Fehler Parse");
		return false;
	}
	
    return true;
}

//=============================================================================
CTlsXmlWriter::CTlsXmlWriter(CTlsOStream_Base* pOs, bool bBeautify)
  : m_pOs(pOs), m_bBeautify(bBeautify), m_iIndent(0), m_bStart(false), m_bOpen(false)
{
}

//=============================================================================
CTlsXmlWriter::~CTlsXmlWriter()
{
}

//=============================================================================
void CTlsXmlWriter::Indent()
{
  for(int i = 0; i < m_iIndent; i++)
    m_pOs->Write('\t');
}

//=============================================================================
void CTlsXmlWriter::OpenTag(char* pszTagName, bool bFinish)
{
  sCurrTag = pszTagName;

  if(m_bBeautify && !m_bStart)
    m_pOs->Write("\r\n");
  if(m_bBeautify)
    Indent();
  m_pOs->Write('<');
  m_pOs->Write(pszTagName);
  m_iIndent++;

  m_bStart = false;
  m_bOpen = true;

  if(bFinish)
  {
    m_pOs->Write('>');
    m_bOpen = false;
  }
}

void CTlsXmlWriter::OpenTag(CStringW szTagName, bool bFinish)
{
	sCurrTag = szTagName;

	if(m_bBeautify && !m_bStart)
		m_pOs->Write("\r\n");
	if(m_bBeautify)
		Indent();
	m_pOs->Write('<');
	m_pOs->Write(szTagName);
	m_iIndent++;

	m_bStart = false;
	m_bOpen = true;

	if(bFinish)
	{
		m_pOs->Write('>');
		m_bOpen = false;
	}
}

//=============================================================================
void CTlsXmlWriter::WriteAttr(char* pszAttr, const char* pszValue)
{
  m_pOs->Write(' ');
  m_pOs->Write(pszAttr);
  m_pOs->Write("=\"");
  m_pOs->Write(pszValue);
  m_pOs->Write("\"");
}

//=============================================================================
void CTlsXmlWriter::WriteAttr(char* pszAttr, CStringW sValue)
{
  m_pOs->Write(' ');
  m_pOs->Write(pszAttr);
  m_pOs->Write("=\"");
  m_pOs->Write(sValue);
  m_pOs->Write("\"");
}

//=============================================================================
void CTlsXmlWriter::WriteAttr(char* pszAttr, int iValue)
{
  char szBuf[20];
  sprintf(szBuf, "%i", iValue);
  m_pOs->Write(' ');
  m_pOs->Write(pszAttr);
  m_pOs->Write("=");
  m_pOs->Write("\"");
  m_pOs->Write(szBuf);
  m_pOs->Write("\"");
}

//=============================================================================
void CTlsXmlWriter::WriteAttr(char* pszAttr, double dValue)
{
  char szBuf[20];
  sprintf(szBuf, "%d", dValue);
  m_pOs->Write(' ');
  m_pOs->Write(pszAttr);
  m_pOs->Write("=");
  m_pOs->Write("\"");
  m_pOs->Write(szBuf);
  m_pOs->Write("\"");
}

//=============================================================================
//
//\brief Writes '>'
//
//=============================================================================
void CTlsXmlWriter::EndTag()
{
  m_pOs->Write('>');
  m_iIndent--;
  m_bOpen = false;
}

//=============================================================================
void CTlsXmlWriter::WriteText(char* pszText)
{
  while(*pszText)
  {
    char c = *pszText;
    switch(c)
    {
    case '<':
      m_pOs->Write("&lt;");
      break;

    case '>':
      m_pOs->Write("&gt;");
      break;

    case '\"':
      m_pOs->Write("&quot;");
      break;

    case '\'':
      m_pOs->Write("&apos;");
      break;

    case '&':
      m_pOs->Write("&amp;");
      break;

    case 'ß':
    case 'ä':
    case 'ö':
    case 'ü':
    case 'Ä':
    case 'Ö':
    case 'Ü':
      {
      int u = (unsigned char)c;
      char szBuf[20];
      sprintf(szBuf, "&#%u;", u);
      m_pOs->Write(szBuf);
      }
      break;

    default:
      m_pOs->Write(c);
      break;
    }
    pszText++;
  }
}

//=============================================================================
//
//\brief Insert Tag-Body as Text
//
//=============================================================================
void CTlsXmlWriter::WriteText(int iText)
{
  char szBuf[20];
  sprintf(szBuf, "%i", iText);
  m_pOs->Write(szBuf);
}

//=============================================================================
//
//\brief Insert Tag-Body as Text
//
//=============================================================================
void CTlsXmlWriter::WriteText(double dText)
{
  char szBuf[20];
  sprintf(szBuf, "%d", dText);
  m_pOs->Write(szBuf);
}

//=============================================================================
CTlsOStream_Base* CTlsXmlWriter::InsertBinary()
{
  return m_pOs;
}

//=============================================================================
//
//\brief Writes '</pszTagName>' or '/>' depending on preceding calls...
//
//=============================================================================
void CTlsXmlWriter::CloseTag(char* pszTagName, bool bInline)
{
  if(m_bOpen)
  {
      m_iIndent--;
      m_pOs->Write("/>");
      m_bOpen = false;
      return;
  }

  if(m_bBeautify && !m_bStart && !bInline)
  {
    m_pOs->Write("\r\n");
    m_iIndent--;
    Indent();
    m_iIndent++;
  }

  m_pOs->Write("</");
  m_pOs->Write(pszTagName);
  m_pOs->Write('>');
  m_iIndent--;
}

void CTlsXmlWriter::CloseTag(CStringW szTagName, bool bInline)
{
	if(m_bOpen)
	{
		m_iIndent--;
		m_pOs->Write("/>");
		m_bOpen = false;
		return;
	}

	if(m_bBeautify && !m_bStart && !bInline)
	{
		m_pOs->Write("\r\n");
		m_iIndent--;
		Indent();
		m_iIndent++;
	}

	m_pOs->Write("</");
	m_pOs->Write(szTagName);
	m_pOs->Write('>');
	m_iIndent--;
}

//=============================================================================
void CTlsXmlWriter::WriteTextTag(CStringW szTagName, CStringW sValue)
{
  OpenTag(szTagName);
  CTlsBlob bl(sValue.GetLength());
  int iLen = wcstombs((char*)bl.GetBuffer(), sValue, sValue.GetLength());
  if(bl.GetBuffer())
    WriteText((char*)bl.GetBuffer());
  CloseTag(szTagName, true);
}

void CTlsXmlWriter::WriteTextTag(char* pszTagName, CStringW sValue)
{
	OpenTag(pszTagName);
	CTlsBlob bl(sValue.GetLength());
	int iLen = wcstombs((char*)bl.GetBuffer(), sValue, sValue.GetLength());
	if(bl.GetBuffer())
		WriteText((char*)bl.GetBuffer());
	CloseTag(pszTagName, true);
}

//=============================================================================
void CTlsXmlWriter::WriteTextTag(char* pszTagName, char* pszValue)
{
  OpenTag(pszTagName);
  WriteText(pszValue);
  CloseTag(pszTagName, true);
}

//=============================================================================
void CTlsXmlWriter::WriteTextTag(char* pszTagName, int iValue)
{
  OpenTag(pszTagName);
  WriteText(iValue);
  CloseTag(pszTagName, true);
}
void CTlsXmlWriter::WriteTextTag(CStringW szTagName, int iValue)
{
	OpenTag(szTagName);
	WriteText(iValue);
	CloseTag(szTagName, true);
}

//=============================================================================
void CTlsXmlWriter::WriteTextTag(char* pszTagName, double dValue)
{
  OpenTag(pszTagName);
  WriteText(dValue);
  CloseTag(pszTagName, true);
}
void CTlsXmlWriter::WriteTextTag(CStringW szTagName, double dValue)
{
	OpenTag(szTagName);
	WriteText(dValue);
	CloseTag(szTagName, true);
}

/*=============================================================================
    E O F
=============================================================================*/
