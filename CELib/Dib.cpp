#include "StdAfx.h"
/*****************************************************************************

THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.

Copyright  1999  Microsoft Corporation.  All Rights Reserved.


Module Name:

dib.cpp

Abstract:

This file contains the source code of the "Meteors" application, which
will be used to illustrate part of the functionality of Palm-size PC. The
application demonstrates the implementation of a simple sprite engine in the
form of an outer-space meteor shoot-em-up game.

This is sample code and is freely distributable.

****************************************************************************/
#include <windows.h>
#include <afx.h>
bool PreloadBitmap(HINSTANCE hInstance,HDC hDC, int bmid, HBITMAP *phBm, HBITMAP *hbmPrev, HDC *phDCPic)
{
	HBITMAP	hTempBitmap;
	HDC hdcTemp;
	BITMAP bm;
	//
	// now here is some strange looking code that warrants some
	// explanation.  we basically create two compatible display contexts,
	// select the bitmap into one of them and bitblt it into the second.
	// why? so the color conversion only happens once.  in the future,
	// when we blt from the second display context (the one we keep with
	// the sprite), no color conversion happens and so the bitblt will be
	// faster.  we could remove this code and it may not appear that the app
	// is any faster, but in that case the Windows CE OS is caching the
	// color translation for us - if ever that cache blows, the animation will
	// drag.
	// 

	//
	// load the bitmap and select it into a temporary DC
	hTempBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(bmid));
	if(!hTempBitmap)
		return false;
	GetObject(hTempBitmap, sizeof(BITMAP), (LPSTR) &bm);
	hdcTemp = CreateCompatibleDC(hDC);
	HBITMAP hbmTempPrev=(HBITMAP)SelectObject(hdcTemp, hTempBitmap);

	//
	// create a compatible bitmap and select it into another DC
	//
	*phBm = CreateCompatibleBitmap(hDC, bm.bmWidth, bm.bmHeight);
	*phDCPic = CreateCompatibleDC(hDC);
	*hbmPrev=(HBITMAP)SelectObject(*phDCPic, *phBm);

	//
	// now blt our bitmap into it
	//
	BitBlt(*phDCPic, 0, 0, bm.bmWidth, bm.bmHeight, hdcTemp, 0, 0, SRCCOPY);

	//
	// get rid of the trash
	//
	SelectObject(hdcTemp, hbmTempPrev);
	DeleteObject(hTempBitmap);
	DeleteDC(hdcTemp);
	return true;
}
/////////////////////////////////////////////////////////////////////////////
bool PreloadBitmap(HDC hDC,CStringW csFilename, HBITMAP *phBm, HBITMAP *hbmPrev, HDC *phDCPic)
{
	HBITMAP	hTempBitmap;
	HDC hdcTemp;
	BITMAP bm;

	// Load the bitmap and select it into a temporary DC
	hTempBitmap = SHLoadDIBitmap(csFilename);
	if(!hTempBitmap)
		return false;
	GetObject(hTempBitmap, sizeof(BITMAP), (LPSTR) &bm);
	hdcTemp = CreateCompatibleDC(hDC);
	HBITMAP hbmTempPrev=(HBITMAP)SelectObject(hdcTemp, hTempBitmap);

	// Create a compatible bitmap and select it into another DC
	*phBm = CreateCompatibleBitmap(hDC, bm.bmWidth, bm.bmHeight);
	*phDCPic = CreateCompatibleDC(hDC);
	*hbmPrev=(HBITMAP)SelectObject(*phDCPic, *phBm);

	// BitBlt the bitmap the DC
	BitBlt(*phDCPic, 0, 0, bm.bmWidth, bm.bmHeight, hdcTemp, 0, 0, SRCCOPY);

	// Delete contruction 
	SelectObject(hdcTemp, hbmTempPrev);
	DeleteObject(hTempBitmap);
	DeleteDC(hdcTemp);
	return true;
}
/////////////////////////////////////////////////////////////////////////////
#include <Afxwin.h>
bool PreloadBitmap(HDC hDC,CStringW csFilename, HBITMAP *phBm, HBITMAP *hbmPrev, HDC *phDCPic, COLORREF crBkg, COLORREF crTransparent)
{
	HBITMAP	hTempBitmap;
	HDC hdcTemp;
	BITMAP bm;

	// Load the bitmap and select it into a temporary DC
	hTempBitmap = SHLoadDIBitmap(csFilename);
	if(!hTempBitmap)
		return false;
	GetObject(hTempBitmap, sizeof(BITMAP), (LPSTR) &bm);
	hdcTemp = CreateCompatibleDC(hDC);
	HBITMAP hbmTempPrev=(HBITMAP)SelectObject(hdcTemp, hTempBitmap);

	// Create a compatible bitmap and select it into another DC
	*phBm = CreateCompatibleBitmap(hDC, bm.bmWidth, bm.bmHeight);
	*phDCPic = CreateCompatibleDC(hDC);
	*hbmPrev=(HBITMAP)SelectObject(*phDCPic, *phBm);

	// BitBlt the bitmap trans. into the DC
	CBrush cbr(crBkg);
	RECT rc={ 0, 0, bm.bmWidth, bm.bmHeight}; 
	FillRect(*phDCPic,&rc,(HBRUSH)cbr);
	TransparentBlt(*phDCPic, 0, 0, bm.bmWidth, bm.bmHeight, hdcTemp, 0, 0, bm.bmWidth, bm.bmHeight, crTransparent);

	// Delete contruction
	SelectObject(hdcTemp, hbmTempPrev);
	DeleteObject(hTempBitmap);
	//DeleteObject(cbr);
	DeleteDC(hdcTemp);
	return true;
}

