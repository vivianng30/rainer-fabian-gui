// BitmapSlider.cpp : implementation file
//

#include "stdafx.h"
#include "BitmapSlider.h"

#ifdef _DEBUG

/**********************************************************************************************//**
 * A macro that defines new
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 **************************************************************************************************/

#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__; ///< this file[]
#endif

/**********************************************************************************************//**
 * Initializes a new instance of the CBitmapSlider class
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 **************************************************************************************************/

CBitmapSlider::CBitmapSlider():
m_nMax(100),m_nMin(0),m_nPos(0),m_nPage(20),m_nWidth(0),m_nHeight(0),m_nThumbWidth(0),m_nThumbHeight(0),m_nMarginLeft(0),m_nMarginRight(0),m_nMarginTop(0),m_nMarginBottom(0),
m_nThumbBgX(-1),m_nThumbBgY(-1),m_nMouseOffset(0),m_bVertical(FALSE),m_bChannelActive(FALSE),m_bThumbActive(FALSE),m_bTransparentChannel(FALSE),m_bTransparentThumb(FALSE),
m_bThumb(FALSE),m_bChannel(FALSE),m_bLButtonDown(FALSE),m_bFocus(FALSE),m_bFocusRect(FALSE),m_bDrawFocusRect(TRUE),m_bEnable(TRUE)
{

}

/**********************************************************************************************//**
 * Finalizes an instance of the CBitmapSlider class
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 **************************************************************************************************/

CBitmapSlider::~CBitmapSlider()
{
}


BEGIN_MESSAGE_MAP(CBitmapSlider, CStatic)
	//{{AFX_MSG_MAP(CBitmapSlider)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_GETDLGCODE()
	ON_WM_KEYDOWN()
	ON_WM_KILLFOCUS()
	ON_WM_SETFOCUS()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************************************//**
 * Executes the erase bkgnd action
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 *
 * \param [in,out]	pDC	If non-null, the device-context.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

BOOL CBitmapSlider::OnEraseBkgnd(CDC*) 
{	
	// Do not erase background for the transparency effect
	return TRUE;
}

/**********************************************************************************************//**
 * Draw channel and thumb
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 **************************************************************************************************/

void CBitmapSlider::OnPaint() 
{
	CPaintDC dcOrigin(this);

	// "Flicker Free Drawing In MFC" by Keith Rule
	CMemDC dc( &dcOrigin, &m_rect, m_bTransparentChannel );

	CDC dcMem;
	dcMem.CreateCompatibleDC( &dc );

	CBitmap *pbmTmp=NULL;

	// Delete focus rectangle for transparent channel
	if( m_bFocusRect && ( m_bTransparentChannel || !m_bChannel ) ) {

		dc.DrawFocusRect( m_rect );
		m_bFocusRect = FALSE;
	}

	// Draw channel
	if( m_bChannel ) {

		pbmTmp = dcMem.SelectObject( &m_bmChannel );

		// There is a bitmap for active channel
		if( m_bChannelActive && m_bEnable ) {

			// Vertical slider
			if( m_bVertical ) {

				// Lower part
				DrawBitmap( &dc, 0, Pos2Pixel(m_nPos),
					m_nWidth, m_nHeight - Pos2Pixel(m_nPos),
					&dcMem, 0, Pos2Pixel(m_nPos),
					&m_bmChannelActiveMask, m_bTransparentChannel );

				dcMem.SelectObject( &m_bmChannelActive );

				// Upper part
				DrawBitmap( &dc, 0, 0, m_nWidth, Pos2Pixel(m_nPos),
					&dcMem, 0, 0, &m_bmChannelMask, m_bTransparentChannel );

			// Horizontal slider
			} else {

				// Right side
				DrawBitmap( &dc, Pos2Pixel(m_nPos), 0,
					m_nWidth - Pos2Pixel(m_nPos), m_nHeight,
					&dcMem, Pos2Pixel(m_nPos), 0, 
					&m_bmChannelActiveMask, m_bTransparentChannel );

				dcMem.SelectObject( &m_bmChannelActive );

				// Left side
				DrawBitmap( &dc, 0, 0, Pos2Pixel(m_nPos), m_nHeight,
					&dcMem, 0, 0, &m_bmChannelMask, m_bTransparentChannel );
			}

		// Only one bitmap for channel
		} else {

			DrawBitmap( &dc, 0, 0, m_nWidth, m_nHeight,
				&dcMem, 0, 0, &m_bmChannelMask, m_bTransparentChannel );

		}

		dcMem.SelectObject( pbmTmp );
	}

	// If there is a bitmap to restore background image of a thumb
	if( m_nThumbBgX != -1 ) {

		RestoreBackground(
			&dc, m_nThumbBgX, m_nThumbBgY,
			m_nThumbWidth, m_nThumbHeight, &m_bmThumbBg );

		m_nThumbBgX = -1;
	}

	// Draw thumb
	if( m_bThumb && m_bEnable ) {

		if( m_bThumbActive && m_bLButtonDown )
			pbmTmp = dcMem.SelectObject( &m_bmThumbActive ); // Active thumb
		else
			pbmTmp = dcMem.SelectObject( &m_bmThumb ); // Normal thumb

		// Vertical slider
		if( m_bVertical ) {

			// Background image is need to be restored
			if( m_bTransparentChannel || !m_bChannel ) {

				m_nThumbBgX = m_nMarginLeft;
				m_nThumbBgY = Pos2Pixel(m_nPos) - m_nThumbHeight/2;

				CopyBackground(
					&dc, m_nThumbBgX, m_nThumbBgY,
					m_nThumbWidth, m_nThumbHeight, &m_bmThumbBg );
			}

			DrawBitmap(
				&dc, m_nMarginLeft, Pos2Pixel(m_nPos) - m_nThumbHeight/2,
				m_nThumbWidth, m_nThumbHeight,
				&dcMem, 0, 0, &m_bmThumbMask, m_bTransparentThumb );

		// Horizontal slider
		} else {

			// Background image is need to be restored
			if( m_bTransparentChannel || !m_bChannel ) {

				m_nThumbBgX = Pos2Pixel(m_nPos) - m_nThumbWidth/2;
				m_nThumbBgY = m_nMarginTop;

				CopyBackground(
					&dc, m_nThumbBgX, m_nThumbBgY,
					m_nThumbWidth, m_nThumbHeight, &m_bmThumbBg );
			}

			DrawBitmap(
				&dc, Pos2Pixel(m_nPos) - m_nThumbWidth/2, m_nMarginTop,
				m_nThumbWidth, m_nThumbHeight,
				&dcMem, 0, 0, &m_bmThumbMask, m_bTransparentThumb );

		} // if horizontal

		dcMem.SelectObject( pbmTmp );

	} // if draw thumb

	// Draw focus rectangle
	if( m_bDrawFocusRect && m_bFocus && m_bEnable ) {

		dc.DrawFocusRect( m_rect );
		m_bFocusRect = TRUE;
	}

	dcMem.DeleteDC();
}

/**********************************************************************************************//**
 * Sets the maximum range for the slider.
 * 
 * Parameters:
 *  	[IN]	nMax
 *  			Maximum position for the slider.
 *  	[IN]	bRedraw
 *  			TRUE to redraw after the range is set. FALSE to only change maximum position.
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 *
 * \param	nMax   	The maximum.
 * \param	bRedraw	True to redraw.
 **************************************************************************************************/

void CBitmapSlider::SetRangeMax(int nMax, BOOL bRedraw)
{
	m_nMax = nMax;
	if( bRedraw )
	{
		Invalidate();
		UpdateWindow();
	}
}

/**********************************************************************************************//**
 * Sets the minimum range for the slider.
 * 
 * Parameters:
 *  	[IN]	nMin
 *  			Minimum position for the slider.
 *  	[IN]	bRedraw
 *  			TRUE to redraw after the range is set. FALSE to only change minimum position.
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 *
 * \param	nMin   	The minimum.
 * \param	bRedraw	True to redraw.
 **************************************************************************************************/

void CBitmapSlider::SetRangeMin(int nMin, BOOL bRedraw)
{
	m_nMin = nMin;
	if( bRedraw )
	{
		Invalidate();
		UpdateWindow();
	}
}

/**********************************************************************************************//**
 * Sets the range (minimum and maximum positions) for the slider.
 * 
 * Parameters:
 *  	[IN]	nMin
 *  			Minimum position for the slider.
 *  	[IN]	nMax
 *  			Maximum position for the slider.
 *  	[IN]	bRedraw
 *  			TRUE to redraw after the range is set. FALSE to only change the range.
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 *
 * \param	nMin   	The minimum.
 * \param	nMax   	The maximum.
 * \param	bRedraw	True to redraw.
 **************************************************************************************************/

void CBitmapSlider::SetRange(int nMin, int nMax, BOOL bRedraw)
{
	SetRangeMin( nMin, FALSE );
	SetRangeMax( nMax, bRedraw );
}

/**********************************************************************************************//**
 * Sets the current position of the slider.
 * 
 * Parameters:
 *  	[IN]	nPos
 *  			Specifies the new slider position.
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 *
 * \param	nPos	The position.
 **************************************************************************************************/

void CBitmapSlider::SetPos(int nPos)
{
	m_nPos = nPos;

	// Boundary check
	if( m_nPos > m_nMax )
		m_nPos = m_nMax;
	if( m_nPos < m_nMin )
		m_nPos = m_nMin;

	Invalidate();
	UpdateWindow();
}

/**********************************************************************************************//**
 * Sets the size of the page for a control.
 * 
 * Parameters:
 *  	[IN]	nSize
 *  			The new page size of the control.
 * 
 * Return value:
 *  	The previous page size.
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 *
 * \param	nSize	The size.
 *
 * \return	An int.
 **************************************************************************************************/

int CBitmapSlider::SetPageSize(int nSize)
{
	int nRet = m_nPage;

	m_nPage = nSize;

	return nRet;
}

/**********************************************************************************************//**
 * Sets the left, top, right, and bottom margins for a control
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 *
 * \param	nLeft  	The left.
 * \param	nTop   	The top.
 * \param	nRight 	The right.
 * \param	nBottom	The bottom.
 **************************************************************************************************/

void CBitmapSlider::SetMargin(int nLeft, int nTop, int nRight, int nBottom )
{
	SetMarginLeft( nLeft );
	SetMarginTop( nTop );
	SetMarginRight( nRight );
	SetMarginBottom( nBottom );
}

/**********************************************************************************************//**
 * Enables or disables control.
 * 
 *  	[IN]	bEnable
 *  			TRUE to enable control. FALSE to disable control.
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 *
 * \param	bEnable	True to enable, false to disable.
 **************************************************************************************************/

void CBitmapSlider::Enable(BOOL bEnable)
{
	m_bEnable = bEnable;

	// If control is disabled during dragging
	if( !m_bEnable && m_bLButtonDown ) {

		m_bLButtonDown = FALSE;
		ReleaseCapture();
	}

	Invalidate();
	UpdateWindow();
}

/**********************************************************************************************//**
 * Specify whether draw focus rectangle or not.
 * 
 *  	[IN]	bDraw
 *  			TRUE to draw focus rectangle. FALSE to hide focus rectangle.
 * 
 *  	[IN]	bRedraw
 *  			TRUE to redraw status is changed. FALSE to only change the status.
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 *
 * \param	bDraw  	True to draw.
 * \param	bRedraw	True to redraw.
 **************************************************************************************************/

void CBitmapSlider::DrawFocusRect(BOOL bDraw, BOOL bRedraw)
{
	m_bDrawFocusRect = bDraw;

	if( bRedraw )
	{
		Invalidate();
		UpdateWindow();
	}
}

/**********************************************************************************************//**
 * Load bitmaps for a channel
 * 
 * Parameters:
 *  	[IN]	nChannelID
 *  			ID number of the bitmap resource of the channel.
 *  	[IN]	nActiveID
 *  			ID number of the bitmap resource of the active channel.
 *  	[IN]	bTransparent
 *  			TRUE to apply transparency effect. FALSE to display normal bitmap.
 *  	[IN]	clrpTransColor
 *  			RGB color to treat as transparent.
 *  	[IN]	iTransPixelX
 *  			Logical x-coordinate of a point. It's color will be treated as transparent.
 *  	[IN]	iTransPixelY
 *  			Logical y-coordinate of a point. It's color will be treated as transparent.
 * 
 * Return value:
 *  	TRUE
 *  		Function succeedes.
 *  	FALSE
 *  		Function failes to load bitmaps.
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 *
 * \param	nChannelID	  	Identifier for the channel.
 * \param	nActiveID	  	Identifier for the active.
 * \param	bTransparent  	True to transparent.
 * \param	clrpTransColor	The clrp transaction color.
 * \param	iTransPixelX  	Zero-based index of the transaction pixel x coordinate.
 * \param	iTransPixelY  	Zero-based index of the transaction pixel y coordinate.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

BOOL CBitmapSlider::SetBitmapChannel(
	UINT nChannelID, UINT nActiveID , BOOL bTransparent,
	COLORREF clrpTransColor, int iTransPixelX, int iTransPixelY )
{
	// This control will not have any bitmap for channel
	if( !nChannelID ) {

		m_bChannel = FALSE;

		m_bmChannel.DeleteObject();
		m_bmChannelMask.DeleteObject();
		m_bmChannelActive.DeleteObject();
		m_bmChannelActiveMask.DeleteObject();
	
		return TRUE;
	}

	// load a bitmap
	m_bmChannel.DeleteObject();

	if( !m_bmChannel.LoadBitmap( nChannelID ) )
		return FALSE;

	// Prepare mask for transparency effect.
	if( bTransparent ) {

		PrepareMask( &m_bmChannel, &m_bmChannelMask,
			clrpTransColor, iTransPixelX, iTransPixelY );
	}

	// Load a bitmap for active state.
	if( nActiveID ) {

		m_bmChannelActive.DeleteObject();

		if( !m_bmChannelActive.LoadBitmap( nActiveID ) ) {

			m_bmChannel.DeleteObject();
			if( bTransparent )
				m_bmChannelMask.DeleteObject();

			return FALSE;
		}

		if( bTransparent ) {

			PrepareMask( &m_bmChannelActive, &m_bmChannelActiveMask,
				clrpTransColor, iTransPixelX, iTransPixelY );
		}
		
		m_bChannelActive = TRUE;

	// There is no bitmap for active state.
	} else
		m_bChannelActive = FALSE;

	// Get size of bitmap.
	BITMAP	bitmap;
	m_bmChannel.GetBitmap( &bitmap );

	m_nWidth = bitmap.bmWidth;
	m_nHeight = bitmap.bmHeight;

	// Compare size
	if( m_bChannelActive ) {

		BITMAP	bitmap;
		m_bmChannelActive.GetBitmap( &bitmap );

		ASSERT( m_nWidth == bitmap.bmWidth && m_nHeight == bitmap.bmHeight );
	}

	// Change size of control as same as the bitmap.
	SetWindowPos(NULL, 0, 0, m_nWidth, m_nHeight, SWP_NOZORDER | SWP_NOMOVE);

	GetClientRect( &m_rect );

	m_bTransparentChannel = bTransparent;
	m_bChannel = TRUE;

	return TRUE;
}

/**********************************************************************************************//**
 * Load bitmaps for a thumb
 * 
 * Parameters:
 *  	[IN]	nThumbID
 *  			ID number of the bitmap resource of the thumb
 *  	[IN]	nActiveID
 *  			ID number of the bitmap resource of the active thumb
 *  	[IN]	bTransparent
 *  			TRUE to apply transparency effect FALSE to display normal bitmap
 *  	[IN]	clrpTransColor
 *  			RGB color to treat as transparent
 *  	[IN]	iTransPixelX
 *  			Logical x-coordinate of a point. It's color will be treated as transparent
 *  	[IN]	iTransPixelY
 *  			Logical y-coordinate of a point. It's color will be treated as transparent
 * 
 * Return value:
 *  	TRUE
 *  		Function succeedes.
 *  	FALSE
 *  		Function failes to load bitmaps.
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 *
 * \param	nThumbID	  	Identifier for the thumb.
 * \param	nActiveID	  	Identifier for the active.
 * \param	bTransparent  	True to transparent.
 * \param	clrpTransColor	The clrp transaction color.
 * \param	iTransPixelX  	Zero-based index of the transaction pixel x coordinate.
 * \param	iTransPixelY  	Zero-based index of the transaction pixel y coordinate.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

BOOL CBitmapSlider::SetBitmapThumb(
	UINT nThumbID, UINT nActiveID, BOOL bTransparent,
	COLORREF clrpTransColor, int iTransPixelX, int iTransPixelY )
{
	// This control will not have bitmap
	if( !nThumbID ) {

		m_bThumb = FALSE;

		m_bmThumb.DeleteObject();
		m_bmThumbMask.DeleteObject();
		m_bmThumbActive.DeleteObject();
		m_bmThumbActiveMask.DeleteObject();
		m_bmThumbBg.DeleteObject();

		return TRUE;
	}

	// load a bitmap
	m_bmThumb.DeleteObject();

	if( !m_bmThumb.LoadBitmap( nThumbID ) )
		return FALSE;

	// Prepare mask for transparency effect.
	if( bTransparent ) {

		PrepareMask( &m_bmThumb, &m_bmThumbMask,
			clrpTransColor, iTransPixelX, iTransPixelY );
	}

	// Load a bitmap for active state.
	if( nActiveID ) {

		m_bmThumbActive.DeleteObject();

		if( !m_bmThumbActive.LoadBitmap( nActiveID ) ) {

			m_bmThumb.DeleteObject();
			if( bTransparent )
				m_bmThumbMask.DeleteObject();

			return FALSE;
		}

		if( bTransparent ) {

			PrepareMask( &m_bmThumbActive, &m_bmThumbActiveMask,
				clrpTransColor, iTransPixelX, iTransPixelY );
		}
		
		m_bThumbActive = TRUE;

	// There is no bitmap for active state.
	} else
		m_bThumbActive = FALSE;

	// Get size of the bitmap
	BITMAP	bitmap;
	m_bmThumb.GetBitmap( &bitmap );

	m_nThumbWidth = bitmap.bmWidth;
	m_nThumbHeight = bitmap.bmHeight;

	// Get size of the control if there was no bitmap for channel.
	if( !m_bChannel ) {

		GetClientRect( &m_rect );
		m_nHeight = m_rect.Height();
		m_nWidth = m_rect.Width();
	}

	ASSERT( m_nThumbWidth <= m_nWidth && m_nThumbHeight <= m_nHeight );

	// Compare size
	if( m_bThumbActive ) {

		BITMAP	bitmap;
		m_bmThumbActive.GetBitmap( &bitmap );

		ASSERT(
			m_nThumbWidth == bitmap.bmWidth &&
			m_nThumbHeight == bitmap.bmHeight );
	}

	// Set attributes
	m_bTransparentThumb = bTransparent;
	m_bThumb = TRUE;

	return TRUE;
}

/**********************************************************************************************//**
 * OnLButtonDown
 * 
 * Dragging is started
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 *
 * \param	nFlags	The flags.
 * \param	point 	The point.
 **************************************************************************************************/

void CBitmapSlider::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if( !m_bEnable )
		return;

	SetCapture();
	SetFocus();

	m_bLButtonDown = TRUE;

	// If mouse button is clicked on the thumb,
	// capture the coordinates of mouse pointer and center of thumb
	// and calculate distance between them.
	if( m_bVertical ) {

		if( abs( point.y - Pos2Pixel( m_nPos ) ) <= m_nThumbHeight / 2 )
			m_nMouseOffset = point.y - Pos2Pixel( m_nPos );
		else
			m_nMouseOffset = 0;

	} else {

		if( abs( point.x - Pos2Pixel( m_nPos ) ) <= m_nThumbWidth / 2 )
			m_nMouseOffset = point.x - Pos2Pixel( m_nPos );
		else
			m_nMouseOffset = 0;
	}

	OnMouseMove( nFlags, point );
	Invalidate();
	UpdateWindow();

	CStatic::OnLButtonDown(nFlags, point);
}

/**********************************************************************************************//**
 * OnMouseMove
 * 
 * During dragging
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 *
 * \param	nFlags	The flags.
 * \param	point 	The point.
 **************************************************************************************************/

void CBitmapSlider::OnMouseMove(UINT nFlags, CPoint point) 
{
	if( !m_bLButtonDown || !m_bEnable )
		return;

	int nPixel;

	// Boundary check
	if( m_bVertical ) {

		nPixel = point.y - m_nMouseOffset;

		if( nPixel > m_nHeight - m_nMarginBottom - m_nThumbHeight/2 )
			nPixel = m_nHeight - m_nMarginBottom - m_nThumbHeight/2;

		if( nPixel < m_nMarginTop + m_nThumbHeight/2 )
			nPixel = m_nMarginTop + m_nThumbHeight/2;

	} else {

		nPixel = point.x - m_nMouseOffset;

		if( nPixel < m_nMarginLeft + m_nThumbWidth/2 )
			nPixel = m_nMarginLeft + m_nThumbWidth/2;

		if( nPixel > m_nWidth - m_nMarginRight - m_nThumbWidth/2 )
			nPixel = m_nWidth - m_nMarginRight - m_nThumbWidth/2;
	}

	// Apply change
	if( Pos2Pixel(m_nPos) != nPixel ) {

		SetPos( Pixel2Pos( nPixel ) );

		::PostMessage(
			GetParent()->GetSafeHwnd(), WM_BITMAPSLIDER_MOVING,
			GetDlgCtrlID(), m_nPos );
	}

	CStatic::OnMouseMove(nFlags, point);
}

/**********************************************************************************************//**
 * OnLButtonUp
 * 
 * Dragging is finished
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 *
 * \param	nFlags	The flags.
 * \param	point 	The point.
 **************************************************************************************************/

void CBitmapSlider::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if( !m_bEnable )
		return;

	ReleaseCapture();
	m_bLButtonDown = FALSE;

	Invalidate();
	UpdateWindow();

	::PostMessage(
		GetParent()->GetSafeHwnd(),	WM_BITMAPSLIDER_MOVED,
		GetDlgCtrlID(), m_nPos );

	CStatic::OnLButtonUp(nFlags, point);
}

/**********************************************************************************************//**
 * Calculate point of thumb from position value
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 *
 * \param	nPos	The position.
 *
 * \return	An int.
 **************************************************************************************************/

int CBitmapSlider::Pos2Pixel(int nPos)
{
	if( m_bVertical ) {

		return
			m_nMarginTop + m_nThumbHeight/2 +
			(int)(
			( m_nHeight - m_nMarginTop - m_nMarginBottom - m_nThumbHeight ) *
			((double) ( nPos - m_nMin ) / ( m_nMax - m_nMin ) )
			);

	} else {

		return (int)(
			( m_nWidth - m_nMarginLeft - m_nMarginRight - m_nThumbWidth ) *
			((double) ( nPos - m_nMin ) / ( m_nMax - m_nMin ) )
			) + m_nMarginLeft + m_nThumbWidth/2;
	}
}

/**********************************************************************************************//**
 * Calculate position value from point of mouse
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 *
 * \param	nPixel	The pixel.
 *
 * \return	An int.
 **************************************************************************************************/

int CBitmapSlider::Pixel2Pos(int nPixel)
{
	if( m_bVertical ) {

		return (int)(
			m_nMin +
			(double)( nPixel - m_nMarginTop - m_nThumbHeight/2) /
			( m_nHeight - m_nMarginBottom - m_nMarginTop - m_nThumbHeight ) *
			( m_nMax - m_nMin )
			);

	} else {

		return (int)(
			m_nMin +
			(double)( nPixel - m_nMarginLeft - m_nThumbWidth/2 ) /
			( m_nWidth - m_nMarginLeft - m_nMarginRight - m_nThumbWidth ) *
			( m_nMax - m_nMin )
			);
	}
}

/**********************************************************************************************//**
 * Copy background image to bitmap
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 *
 * \param [in,out]	pDC	   	If non-null, the device-context.
 * \param 		  	nXSrc  	The x coordinate source.
 * \param 		  	nYSrc  	The y coordinate source.
 * \param 		  	nWidth 	The width.
 * \param 		  	nHeight	The height.
 * \param [in,out]	pBmDst 	If non-null, the bm destination.
 **************************************************************************************************/

void CBitmapSlider::CopyBackground(
	CDC *pDC, int nXSrc, int nYSrc, int nWidth, int nHeight, CBitmap *pBmDst)
{
	pBmDst->DeleteObject();
	pBmDst->CreateCompatibleBitmap( pDC, nWidth, nHeight );

	CDC memDC;

	memDC.CreateCompatibleDC( pDC );
	CBitmap *pBmTmp = memDC.SelectObject( pBmDst );

	memDC.BitBlt( 0, 0, nWidth, nHeight, pDC, nXSrc, nYSrc, SRCCOPY );

	memDC.SelectObject( pBmTmp );
	memDC.DeleteDC();
}

/**********************************************************************************************//**
 * Restore background image from bitmap
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 *
 * \param [in,out]	pDC	   	If non-null, the device-context.
 * \param 		  	nXDst  	The x coordinate destination.
 * \param 		  	nYDst  	The y coordinate destination.
 * \param 		  	nWidth 	The width.
 * \param 		  	nHeight	The height.
 * \param [in,out]	pBmSrc 	If non-null, the bm source.
 **************************************************************************************************/

void CBitmapSlider::RestoreBackground(
	CDC *pDC, int nXDst, int nYDst, int nWidth, int nHeight, CBitmap *pBmSrc)
{
	CDC memDC;

	memDC.CreateCompatibleDC( pDC );
	CBitmap *pBmTmp = memDC.SelectObject( pBmSrc );

	pDC->BitBlt( nXDst, nYDst, nWidth, nHeight, &memDC, 0, 0, SRCCOPY );

	memDC.SelectObject( pBmTmp );
	memDC.DeleteDC();
}

/**********************************************************************************************//**
 * DrawBitmap
 * 
 * It's for code readability
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 *
 * \param [in,out]	pDC				If non-null, the device-context.
 * \param 		  	xStart			The start.
 * \param 		  	yStart			The start.
 * \param 		  	wWidth			The width.
 * \param 		  	wHeight			The height.
 * \param [in,out]	pTmpDC			If non-null, the temporary device-context.
 * \param 		  	xSource			Source for the.
 * \param 		  	ySource			Source for the.
 * \param [in,out]	bmMask			If non-null, the bm mask.
 * \param 		  	bTransparent	True to transparent.
 **************************************************************************************************/

void CBitmapSlider::DrawBitmap(
	CDC *pDC, int xStart, int yStart, int wWidth, int wHeight,
	CDC *pTmpDC, int xSource, int ySource, CBitmap *bmMask, BOOL bTransparent )
{
	if( bTransparent ) {

		DrawTransparentBitmap(
			pDC, xStart, yStart,
			wWidth, wHeight,
			pTmpDC, xSource, ySource, bmMask );

	} else {

		pDC->BitBlt( xStart, yStart,
			wWidth, wHeight,
			pTmpDC, xSource, ySource, SRCCOPY );
	}
}

/**********************************************************************************************//**
 * PrepareMask
 * 
 * "Drawing Transparent Bitmap with ease with on the fly masks in MFC" By Raja Segar
 * 
 * I changed default clrpTransColor value from NULL(black) to 0xFF000000(not RGB color)
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 *
 * \param [in,out]	pBmpSource	  	If non-null, the bitmap source.
 * \param [in,out]	pBmpMask	  	If non-null, the bitmap mask.
 * \param 		  	clrpTransColor	The clrp transaction color.
 * \param 		  	iTransPixelX  	Zero-based index of the transaction pixel x coordinate.
 * \param 		  	iTransPixelY  	Zero-based index of the transaction pixel y coordinate.
 **************************************************************************************************/

void CBitmapSlider::PrepareMask(
	CBitmap *pBmpSource, CBitmap *pBmpMask,
	COLORREF clrpTransColor, int iTransPixelX, int iTransPixelY)
{
	BITMAP bm;

	// Get the dimensions of the source bitmap
	pBmpSource->GetObject(sizeof(BITMAP), &bm);

	// Create the mask bitmap
	pBmpMask->DeleteObject();
	pBmpMask->CreateBitmap( bm.bmWidth, bm.bmHeight, 1, 1, NULL);

	// We will need two DCs to work with. One to hold the Image
	// (the source), and one to hold the mask (destination).
	// When blitting onto a monochrome bitmap from a color, pixels
	// in the source color bitmap that are equal to the background
	// color are blitted as white. All the remaining pixels are
	// blitted as black.

	CDC hdcSrc, hdcDst;

	hdcSrc.CreateCompatibleDC(NULL);
	hdcDst.CreateCompatibleDC(NULL);

	// Load the bitmaps into memory DC
	CBitmap* hbmSrcT = (CBitmap*) hdcSrc.SelectObject(pBmpSource);
	CBitmap* hbmDstT = (CBitmap*) hdcDst.SelectObject(pBmpMask);

	// Dynamically get the transparent color
	COLORREF clrTrans;
	if (clrpTransColor == 0xFF000000)
	{
		// User did not specify trans color so get it from bmp
		clrTrans = hdcSrc.GetPixel(iTransPixelX, iTransPixelY);
	}
	else
	{
		clrTrans = clrpTransColor;
	}


	// Change the background to trans color
	COLORREF clrSaveBk  = hdcSrc.SetBkColor(clrTrans);

	// This call sets up the mask bitmap.
	hdcDst.BitBlt(0,0,bm.bmWidth, bm.bmHeight, &hdcSrc,0,0,SRCCOPY);

	// Now, we need to paint onto the original image, making
	// sure that the "transparent" area is set to black. What
	// we do is AND the monochrome image onto the color Image
	// first. When blitting from mono to color, the monochrome
	// pixel is first transformed as follows:
	// if  1 (black) it is mapped to the color set by SetTextColor().
	// if  0 (white) is is mapped to the color set by SetBkColor().
	// Only then is the raster operation performed.

	COLORREF clrSaveDstText = hdcSrc.SetTextColor(RGB(255,255,255));
	hdcSrc.SetBkColor(RGB(0,0,0));

	hdcSrc.BitBlt(0,0,bm.bmWidth, bm.bmHeight, &hdcDst,0,0,SRCAND);

	// Clean up by deselecting any objects, and delete the
	// DC's.
	hdcDst.SetTextColor(clrSaveDstText);

	hdcSrc.SetBkColor(clrSaveBk);
	hdcSrc.SelectObject(hbmSrcT);
	hdcDst.SelectObject(hbmDstT);

	hdcSrc.DeleteDC();
	hdcDst.DeleteDC();
}

/**********************************************************************************************//**
 * DrawTransparentBitmap
 * 
 * "Drawing Transparent Bitmap with ease with on the fly masks in MFC" By Raja Segar
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 *
 * \param [in,out]	pDC	   	If non-null, the device-context.
 * \param 		  	xStart 	The start.
 * \param 		  	yStart 	The start.
 * \param 		  	wWidth 	The width.
 * \param 		  	wHeight	The height.
 * \param [in,out]	pTmpDC 	If non-null, the temporary device-context.
 * \param 		  	xSource	Source for the.
 * \param 		  	ySource	Source for the.
 * \param [in,out]	bmMask 	If non-null, the bm mask.
 **************************************************************************************************/

void CBitmapSlider::DrawTransparentBitmap(
	CDC *pDC, int xStart, int yStart, int wWidth, int wHeight,
	CDC *pTmpDC, int xSource, int ySource, CBitmap *bmMask )
{
	// We are going to paint the two DDB's in sequence to the destination.
	// 1st the monochrome bitmap will be blitted using an AND operation to
	// cut a hole in the destination. The color image will then be ORed
	// with the destination, filling it into the hole, but leaving the
	// surrounding area untouched.

	CDC hdcMem;
	hdcMem.CreateCompatibleDC(NULL);

	CBitmap* hbmT = hdcMem.SelectObject(bmMask);

	pDC->BitBlt( xStart, yStart, wWidth, wHeight, &hdcMem,
		xSource, ySource, SRCAND);

	// Also note the use of SRCPAINT rather than SRCCOPY.

	pDC->BitBlt(xStart, yStart, wWidth, wHeight, pTmpDC,
		xSource, ySource,SRCPAINT);

	// Now, clean up.
	hdcMem.SelectObject(hbmT);
	hdcMem.DeleteDC();
}

/**********************************************************************************************//**
 * To get keyboard input
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 *
 * \return	An UINT.
 **************************************************************************************************/

UINT CBitmapSlider::OnGetDlgCode() 
{
	if( GetKeyState(VK_TAB) >= 0 ) {

		return  DLGC_WANTALLKEYS;
	}
	
	return CStatic::OnGetDlgCode();
}

/**********************************************************************************************//**
 * Handling keyboard input
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 *
 * \param	nChar  	The character.
 * \param	nRepCnt	Number of reps.
 * \param	nFlags 	The flags.
 **************************************************************************************************/

void CBitmapSlider::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if( !m_bEnable )
		return;

	switch( nChar ) {

	// Left & up
	case VK_LEFT :
	case VK_UP :

		SetPos( m_nPos-1 );
		break;

	// Right & down
	case VK_RIGHT :
	case VK_DOWN :

		SetPos( m_nPos+1 );
		break;

	// Home
	case VK_HOME :

		SetPos( m_nMin );
		break;

	// End
	case VK_END :

		SetPos( m_nMax );
		break;

	// Page up
	case VK_PRIOR :

		SetPos( m_nPos - m_nPage );
		break;

	// Page down
	case VK_NEXT :

		SetPos( m_nPos + m_nPage );
		break;

	default :

		CStatic::OnKeyDown(nChar, nRepCnt, nFlags);
		return;
	}
	
	::PostMessage(
		GetParent()->GetSafeHwnd(),	WM_BITMAPSLIDER_MOVED,
		GetDlgCtrlID(), m_nPos );

	CStatic::OnKeyDown(nChar, nRepCnt, nFlags);
}

/**********************************************************************************************//**
 * Control looses its focus
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 *
 * \param [in,out]	pNewWnd	If non-null, the new window.
 **************************************************************************************************/

void CBitmapSlider::OnKillFocus(CWnd* pNewWnd) 
{
	CStatic::OnKillFocus(pNewWnd);

	m_bFocus = FALSE;
	Invalidate();
	UpdateWindow();
}

/**********************************************************************************************//**
 * This control gains its focus
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 *
 * \param [in,out]	pOldWnd	If non-null, the old window.
 **************************************************************************************************/

void CBitmapSlider::OnSetFocus(CWnd* pOldWnd) 
{
	CStatic::OnSetFocus(pOldWnd);

	m_bFocus = TRUE;
	Invalidate();
	UpdateWindow();
}

/**********************************************************************************************//**
 * Release resources
 * 
 * ************************************ Method:    OnDestroy FullName:  CBitmapSlider::OnDestroy
 * Access:    protected Returns:   void Qualifier:
 * 
 * 2015/06/19: checked for correct closing of window
 * ************************************
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 **************************************************************************************************/

void CBitmapSlider::OnDestroy() 
{
	
	m_bmThumb.DeleteObject();
	m_bmThumbMask.DeleteObject();

	m_bmThumbActive.DeleteObject();
	m_bmThumbActiveMask.DeleteObject();

	m_bmThumbBg.DeleteObject();

	m_bmChannel.DeleteObject();
	m_bmChannelMask.DeleteObject();

	m_bmChannelActive.DeleteObject();
	m_bmChannelActiveMask.DeleteObject();

	CStatic::OnDestroy();

}
