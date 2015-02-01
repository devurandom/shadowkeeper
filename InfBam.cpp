/************************************************************************************
	Copyright (c) 2000 Aaron O'Neil
	All rights reserved.

	Redistribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions
	are met:

		1) Redistributions of source code must retain the above copyright notice, 
				this list of conditions and the following disclaimer.
		2) Redistributions in binary form must reproduce the above copyright notice, 
				this list of conditions and the following disclaimer in the documentation
				and/or other materials provided with the distribution.
		3) Redistributions in binary form must reproduce the above copyright notice on
				program startup. Additional credits for program modification are acceptable
				but original copyright and credits must be visible at startup.
		4) You may charge a reasonable copying fee for any distribution of Shadow Keeper. 
				You may charge any fee you choose for support of Shadow Keeper. You may not 
				charge a fee for Shadow Keeper itself.

  THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
	IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
	OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
	IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
	INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
	NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
	DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
	THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
	(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
	THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

**************************************************************************************/

#include "StdAfx.h"
#include "InfBam.h"

COLORREF CInfBam::m_clrDIBits[MAX_ICON_WIDTH*MAX_ICON_HEIGHT];

CInfBam::CInfBam()
{
	m_pData = NULL;
	m_pHeader = NULL;
	m_nDataLen = 0;
}

CInfBam::~CInfBam()
{
	delete [] m_pData;
}

void CInfBam::SetBuffer(BYTE *pData, UINT nDataLen)
{
	m_pData = pData;
	m_nDataLen = nDataLen;
	m_pHeader = (INF_BAM_HEADER*)m_pData;
}

BOOL CInfBam::MakeBitmap(int nFrameWanted, COLORREF clrTrans, HBITMAP &hBitmap,
	BOOL bForceSize, int nWidth, int nHeight)
{
	ASSERT(m_pHeader);

	if (m_pHeader->wFrameCount < 1 )
		return(FALSE);

	if (nFrameWanted >= m_pHeader->wFrameCount)
		nFrameWanted = 0;

	INF_BAM_FRAME *pFrame = (INF_BAM_FRAME*)(m_pData+m_pHeader->dwFrameOffset+(nFrameWanted*sizeof(INF_BAM_FRAME)));

	if (pFrame->wWidth > MAX_ICON_WIDTH || pFrame->wHeight > MAX_ICON_HEIGHT)
		return(FALSE);

	COLORREF *pPal = (COLORREF*)(m_pData+m_pHeader->dwPaletteOffset);
	BYTE *pRawBits = m_pData+(pFrame->dwFrameDataOffset&0x7FFFFFFF);
	BOOL bIsCompressed;
	if (pFrame->dwFrameDataOffset&0x80000000)
		bIsCompressed = FALSE;
	else
		bIsCompressed = TRUE;

	if (!ExpandBamBits(
			pRawBits,
			pFrame->wWidth*pFrame->wHeight,
			pFrame->wWidth,
			pFrame->wHeight,
			m_pHeader->chTransparentIndex,
			clrTrans,
			pPal,
			m_clrDIBits,
			bIsCompressed))
		return(FALSE);

	if (!MakeBitmap(
			m_clrDIBits,
			pFrame->wWidth,
			pFrame->wHeight,
			hBitmap))
		return(FALSE);

	if (bForceSize)
		if (!FitAndCenterBitmap(hBitmap,clrTrans,nWidth,nHeight))
			return(FALSE);

	return(TRUE);
}

BOOL PASCAL CInfBam::ConvertToGrey(COLORREF *pPal)
{
	int nRed, nGreen, nBlue;
	BYTE chGrey;
	BYTE *pClr;
	for (int i=0;i<256;i++)
	{
		pClr = (BYTE*)(pPal+i);

		nBlue = *pClr+1;
		nGreen = *pClr+2;
		nRed = *pClr+3;

		chGrey = (BYTE)((nRed+nGreen+nBlue)/3);

		*(pClr+1) = chGrey;
		*(pClr+2) = chGrey;
		*(pClr+3) = chGrey;
	}

	return(TRUE);
}

BOOL PASCAL CInfBam::ExpandBamBits(const BYTE *pRawBits, int nRawLen, 
	int nWidth, int nHeight, BYTE chTransparentIndex, COLORREF clrTransparent, 
	const COLORREF *pPal, COLORREF *pDIBits, BOOL bIsCompressed)
{
	int nNumPixels = nWidth*nHeight;
	int nDestMax = nNumPixels * sizeof(COLORREF);
	int nSourceOff = 0;
	int nPixelCount = 0;
	int nCount;

	// The red and blue values seem to be backwards from the RGB macro. When
	// creating the bitmap it apparently wants it in the reverse order that 
	// the RGB macro makes it.
	BYTE *pClr = (BYTE*)&clrTransparent;
	COLORREF clrFixed = RGB(*(pClr+2),*(pClr+1),*(pClr));

	while(nPixelCount < nNumPixels)
	{
		if (bIsCompressed && pRawBits[nSourceOff] == chTransparentIndex)
		{
			nSourceOff++;
			nCount = pRawBits[nSourceOff]+1;
			while(nCount)
			{
				memcpy(pDIBits+nPixelCount,&clrFixed,sizeof(COLORREF));
				nCount--;
				nPixelCount++;
			}
			nSourceOff++;
		}
		else
		{
			// If it is not compressed, still need to catch the transparent pixels and
			// fill with the transaprent color.
			if (pRawBits[nSourceOff] == chTransparentIndex)
				memcpy(pDIBits+nPixelCount,&clrFixed,sizeof(COLORREF));
			else
				memcpy(pDIBits+nPixelCount,pPal+pRawBits[nSourceOff],sizeof(COLORREF));
			nSourceOff++;
			nPixelCount++;
		}
	} 

	return(TRUE);
}

BOOL PASCAL CInfBam::FitAndCenterBitmap(HBITMAP &hOriginal, COLORREF clrBackground,
	int nWidth, int nHeight)
{
	CBitmap *pBmp = CBitmap::FromHandle(hOriginal);
	if (!pBmp)
		return(FALSE);

	BITMAP bm;
	pBmp->GetBitmap(&bm);
	
	if (bm.bmWidth >= nWidth && bm.bmHeight >= nHeight)
		return(TRUE);

	CDC *pDC = AfxGetMainWnd()->GetDC();
	if (!pDC)
		return(FALSE);

	// DC to use to create the new image.
	CDC dcMem;
	dcMem.CreateCompatibleDC(pDC);

	// Need to have the original bitmap in a dc to manipulate it.
	CDC dcBmp;
	dcBmp.CreateCompatibleDC(pDC);
	CBitmap *pOldBmp = dcBmp.SelectObject(pBmp);

	// Need a new bitmap in the memory dc to copy onto.
	CBitmap bmpNew;
	bmpNew.CreateCompatibleBitmap(pDC,nWidth,nHeight);
	CBitmap *pOldMemBmp = dcMem.SelectObject(&bmpNew);

	// Erase the surface the background color.
	dcMem.FillSolidRect(0,0,nWidth,nHeight,clrBackground);

	// Find the centered coordinates.
	int nLeft = nWidth/2 - bm.bmWidth/2;
	int nTop = nHeight/2 - bm.bmHeight/2;

	// Copy the smaller original bitmap onto the 32x32 in the cenetered position.
	dcMem.BitBlt(nLeft,nTop,bm.bmWidth,bm.bmHeight,&dcBmp,0,0,SRCCOPY);

	// Select out the bitmaps.
	dcMem.SelectObject(pOldMemBmp);
	dcBmp.SelectObject(pOldBmp);

	AfxGetMainWnd()->ReleaseDC(pDC);

	// Delete the original and point it at the new one.
	pBmp->Detach();
	::DeleteObject(hOriginal);

	hOriginal = (HBITMAP)bmpNew;
	bmpNew.Detach();

	return(TRUE);
}

BOOL PASCAL CInfBam::MakeBitmap(const COLORREF *pDIBits, int nWidth, int nHeight, 
	HBITMAP &hBitmap)
{
	CDC *pDC = AfxGetMainWnd()->GetDC();
	if (!pDC)
		return(FALSE);

	BITMAPINFOHEADER bih;
	memset(&bih,0,sizeof(BITMAPINFOHEADER));
	
	bih.biSize = sizeof(BITMAPINFOHEADER);
	bih.biWidth = nWidth;
	bih.biHeight = nHeight * -1;	// The graphics are stored as bottom up DIBs.
	bih.biPlanes = 1;
	bih.biBitCount = 32;
	bih.biCompression = BI_RGB;

	hBitmap = ::CreateDIBitmap(
		pDC->GetSafeHdc(),
		&bih,
		CBM_INIT,
		pDIBits,
		(BITMAPINFO*)&bih,
		DIB_RGB_COLORS);

	AfxGetMainWnd()->ReleaseDC(pDC);

	return(hBitmap != NULL);
}
