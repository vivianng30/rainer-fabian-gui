#pragma once
#include <windows.h>


class CBmp
{
public:
	CBmp(HINSTANCE hInstance,HDC hDC, int bmId);
	//CBmp(HINSTANCE hInstance,HDC hDC, int bmId, int iT);
	CBmp(HDC hDC,CStringW csFilename);
	CBmp(HDC hDC,CStringW csFilename,COLORREF crBkg,COLORREF crTransparent);
	~CBmp();

	void Draw(HDC hDC, int nX=0, int nY=0, DWORD dwCopyMode=SRCCOPY);
	inline int Width(){return m_bm.bmWidth;};
	inline int Height(){return m_bm.bmHeight;};
	inline HDC GetHDCPict(void){return m_hDCPic;};
	inline DWORD GetIDPict(void){return m_bmId;};
	bool IsBitmap(void);


protected:
	HBITMAP m_hbm;
	HBITMAP m_hbmPrev;
	BITMAP m_bm;
	HDC	m_hDCPic;
	bool m_bIsBitmap;
private:
	DWORD m_bmId;
};
