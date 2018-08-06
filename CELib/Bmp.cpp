#include "StdAfx.h"
#include "Bmp.h"

/***** Global/Extern *******************************************************/
extern bool PreloadBitmap(HINSTANCE hInstance,HDC hDC, int bmid, HBITMAP *phBm, HBITMAP *hbmPrev, HDC *phDCPic);
extern bool PreloadBitmap(HDC hDC,CStringW csFilename, HBITMAP *phBm, HBITMAP *hbmPrev, HDC *phDCPic);
extern bool PreloadBitmap(HDC hDC,CStringW csFilename, HBITMAP *phBm, HBITMAP *hbmPrev,HDC *phDCPic, COLORREF crBkg, COLORREF crTransparent);


// Load bitmap from resource
CBmp::CBmp(HINSTANCE hInstance,HDC hDC, int bmId)
{
	m_hDCPic=NULL;

	m_bIsBitmap=false;
	if(PreloadBitmap(hInstance,hDC, bmId, &m_hbm, &m_hbmPrev, &m_hDCPic))
	{
		GetObject(m_hbm, sizeof(BITMAP), (LPSTR) &m_bm);
		m_bIsBitmap=true;
	}
	m_bmId=bmId;
}

// Load bitmap from resource
//CBmp::CBmp(HINSTANCE hInstance,HDC hDC, int bmId, int iT)
//{
//
//	m_bIsBitmap=false;
//
//	m_hbm=::LoadBitmap(hInstance, MAKEINTRESOURCE(bmId));
//	if(m_hbm)
//	{
//		GetObject(m_hbm, sizeof(BITMAP), (LPSTR) &m_bm);
//		m_bIsBitmap=true;
//	}
//	m_bmId=bmId;
//}
// Load bitmap from file
CBmp::CBmp(HDC hDC,CStringW csFilename)
{
	m_hDCPic=NULL;

	m_bIsBitmap=false;
	if(PreloadBitmap(hDC, csFilename, &m_hbm, &m_hbmPrev, &m_hDCPic))
	{
		GetObject(m_hbm, sizeof(BITMAP), (LPSTR) &m_bm);
		m_bIsBitmap=true;
	}
	m_bmId=0;
}
// Load bitmap from file (regard "cr" as transparent)
CBmp::CBmp(HDC hDC,CStringW csFilename,COLORREF crBkg,COLORREF crTransparent)
{
	m_hDCPic=NULL;

	m_bIsBitmap=false;
	if(PreloadBitmap(hDC, csFilename, &m_hbm,&m_hbmPrev,&m_hDCPic,crBkg,crTransparent))
	{
		GetObject(m_hbm, sizeof(BITMAP), (LPSTR) &m_bm);
		m_bIsBitmap=true;
	}
	m_bmId=0;
}
CBmp::~CBmp()
{
	if(m_hDCPic)
	{
		SelectObject(m_hDCPic,m_hbmPrev);

		//CDC::FromHandle(m_hDCPic)->DeleteDC();
		DeleteDC(m_hDCPic);
	}
	
	if(m_hbm)
	{
		//SelectObject(m_hDCPic,m_hbmPrev);
		//CBitmap::FromHandle(m_hbm)->DeleteObject();
		DeleteObject(m_hbm);
	}
	
}

// **************************************************************************
// 
// **************************************************************************
void CBmp::Draw(HDC hDC, int nX, int nY,DWORD dwCopyMode)
{
	BitBlt(hDC, nX, nY, m_bm.bmWidth, m_bm.bmHeight, m_hDCPic, 0, 0,dwCopyMode);
}

// **************************************************************************
// 
// **************************************************************************
bool CBmp::IsBitmap(void)
{
	return  m_bIsBitmap;
}

// **************************************************************************
// 
// **************************************************************************
//void CImage::Rotate()
//{
//	CDC sourceDC, destDC;
//	sourceDC.CreateCompatibleDC( NULL );
//	destDC.CreateCompatibleDC( NULL );
//
//	// Get logical coordinates
//	BITMAP bm;
//	::GetObject( (HBITMAP)bitmap, sizeof( bm ), &bm );
//
//	// First we determine the dimensions of the new bitmap.
//	// width = height and heigth = width
//
//	int w = bm.bmHeight;
//	int h = bm.bmWidth;
//
//
//	// Create a bitmap to hold the result
//	HBITMAP hbmResult = ::CreateCompatibleBitmap(CClientDC(NULL), w, h);
//
//	HBITMAP hbmOldSource = (HBITMAP)::SelectObject( sourceDC.m_hDC, (HBITMAP)bitmap );
//	HBITMAP hbmOldDest = (HBITMAP)::SelectObject( destDC.m_hDC, hbmResult );
//
//	// Draw the background color before we change mapping mode
//	HBRUSH hbrBack = CreateSolidBrush(0x00FFFFFF);
//	HBRUSH hbrOld = (HBRUSH)::SelectObject( destDC.m_hDC, hbrBack );
//	destDC.PatBlt( 0, 0, w, h, PATCOPY );
//	::DeleteObject( ::SelectObject( destDC.m_hDC, hbrOld ) );
//
//
//	// Now do the actual rotating - a pixel at a time
//	// Computing the destination point for each source point
//	// will leave a few pixels that do not get covered
//	// So we use a reverse transform - e.i. compute the source point
//	// for each destination point
//
//	for( int y = 0; y < bm.bmHeight; y++ )
//	{
//		for( int x = 0; x < bm.bmWidth; x++ )
//		{
//			destDC.SetPixel(y, h - x - 1, sourceDC.GetPixel(x,y));
//		}
//	}
//
//	// Restore DCs
//	::SelectObject( sourceDC.m_hDC, hbmOldSource );
//	::SelectObject( destDC.m_hDC, hbmOldDest );
//
//	Update(hbmResult);
//	origSize = hbmResult;
//}

//BOOL CImage::Update(HBITMAP hBM)
//{
//	HGDIOBJ hOld;
//	BOOL ret = TRUE;
//	// Keep the origBitmap handle.
//	// Delete the GDI object only if it is not the original one.
//	if (((hOld = bitmap.Detach()) != origBitmap) && (hOld != origSize)) {
//		if (hOld != 0)
//			ret &= DeleteObject(hOld);
//	}
//
//	// Attach the new bimap handle to the bitmap object.
//	ret &= bitmap.Attach(hBM);
//	CSize size = GetSize();
//	imageInfo.height = size.cy;
//	imageInfo.width = size.cx;
//	if (imageInfo.width % 8)
//		imageInfo.bytesPerLine = (imageInfo.width / 8 + 1);
//	else
//		imageInfo.bytesPerLine = imageInfo.width / 8; 
//
//	imageInfo.fourBytes = (((imageInfo.bytesPerLine - 1) / 4) + 1) * 4;
//
//	return ret;
//}


