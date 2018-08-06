/**********************************************************************************************//**
 * \file	CDCBuffer.h.
 *
 * Declares the cdc buffer class
 **************************************************************************************************/

#ifndef CDCBUFFER_H
#define CDCBUFFER_H

#include <exception>

class CDCBuffer
{
public:
	CDCBuffer()
	{
		m_bValid = FALSE;
		m_hOldBitmap = NULL;
		m_hBitmap = NULL;
		m_nBitmapID = 0;
		m_sizeBitmap.cx = m_sizeBitmap.cy = 0;
	}

	~CDCBuffer()
	{
		DeleteObject();
	}

	void DeleteObject()
	{
		if (m_hOldBitmap)
			m_dcMemory.SelectObject(m_hOldBitmap);
		m_hOldBitmap = NULL;

		if (m_hBitmap)
			::DeleteObject(m_hBitmap);
		m_hBitmap = NULL;

		m_bValid = FALSE;
	}

	BOOL LoadBitmap(UINT nBitmapID, CDC * pDC)
	{
		BOOL ok = FALSE;

		DeleteObject();

		ASSERT(pDC);
		ASSERT(nBitmapID);

		HINSTANCE hInstResource = AfxFindResourceHandle(MAKEINTRESOURCE(nBitmapID), RT_BITMAP);

		m_hBitmap = (HBITMAP)::LoadImage(hInstResource, MAKEINTRESOURCE(nBitmapID),
							IMAGE_BITMAP, 0, 0, 0);
		ASSERT(m_hBitmap);

		if (m_hBitmap && pDC)
		{
			m_dcMemory.DeleteDC();

			if (m_dcMemory.CreateCompatibleDC(pDC))
			{
				m_hOldBitmap = (HBITMAP)::SelectObject(m_dcMemory.m_hDC, m_hBitmap);
				m_nBitmapID = nBitmapID;
				m_bValid = TRUE;

				m_sizeBitmap.cx = m_sizeBitmap.cy = 0;

				BITMAP bm;
				if (GetBitmap(&bm))
				{
					m_sizeBitmap.cx = bm.bmWidth;
					m_sizeBitmap.cy = bm.bmHeight;
				}
				else
				{
					
				}

				ok = TRUE;
			}
			else
			{
				
			}
		}

		return ok;
	}

	BOOL AttachBitmap(HBITMAP hBitmap, CDC * pDC)
	{
		BOOL ok = FALSE;

		DeleteObject();

		ASSERT(hBitmap);

		m_hBitmap = hBitmap;

		if (m_hBitmap && pDC)
		{
			m_dcMemory.DeleteDC();

			if (m_dcMemory.CreateCompatibleDC(pDC))
			{
				m_hOldBitmap = (HBITMAP)::SelectObject(m_dcMemory.m_hDC, m_hBitmap);
				m_nBitmapID = 0;
				m_bValid = TRUE;
				ok = TRUE;
			}
		}

		return ok;
	}

	BOOL GetBitmap(BITMAP * pBM)
	{
		BOOL ok = FALSE;
		if (m_bValid && m_hBitmap)
		{
			ok = ::GetObject(m_hBitmap, sizeof(BITMAP), pBM);
		}
		return ok;
	}

	BOOL IsValid()
	{
		return m_bValid;
	}

	HBITMAP		m_hBitmap;
	CDC			m_dcMemory;
	CSize		m_sizeBitmap;

protected:
	HBITMAP		m_hOldBitmap;
	UINT		m_nBitmapID;
	BOOL		m_bValid;
};

#endif //CDCBUFFER_H
