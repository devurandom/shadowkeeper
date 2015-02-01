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

#ifndef __INFBAM__
#define __INFBAM__

#pragma pack(push,1)

struct INF_BAM_HEADER
{
	char		chSignature[4];
	char		chVersion[4];
	WORD		wFrameCount;
	BYTE		chCycleCount;
	BYTE		chTransparentIndex;
	DWORD		dwFrameOffset;
	DWORD		dwPaletteOffset;
	DWORD		dwFrameLookupTableOffset;
};

struct INF_BAM_FRAME
{
	WORD		wWidth;
	WORD		wHeight;
	WORD		wCenterX;
	WORD		wCenterY;
	DWORD		dwFrameDataOffset;		// bit 31 is a flag for RLE. 1 = RLE
};

struct INF_BAM_CYCLE
{
	WORD		wFrameIndexCount;
	WORD		wFirstFrameIndex;			// Index into the frame lookup table.
};

#pragma pack(pop)

#define MAX_ICON_WIDTH		400
#define MAX_ICON_HEIGHT		400

class CInfBam
{
public:
	CInfBam();
	~CInfBam();

	// Use this function rather than the base class Attach. When Attach is
	// used with a CMemFile the CMemFile will not free the memory used when
	// the object is destroyed. Using SetBuffer still calls attach but also
	// sets the m_pData member, which is freed by this derrived class when
	// it is destoryed.
	void SetBuffer(BYTE *pData, UINT nDataLen);

	BYTE* GetBuffer()								{ return(m_pData); }

	// Makes a windows bitmap from the BAM file. If bForceResize is TRUE then
	// the nWidth and nHeight must also be set. If the bitmap created is smaller
	// than the desired size the bitmap will be centered on a surface the size
	// of the rquired bitmap.
	BOOL MakeBitmap(int nFrameWanted, COLORREF clrTrans, HBITMAP &hBitmap,
		BOOL bForceSize=FALSE, int nWidth=0, int nHeight=0);

	// Changes to the palette to greyscale.
	static BOOL PASCAL ConvertToGrey(COLORREF *pPal);

	// Expands raw compressed bits into a 32bit-depth buffer.
	static BOOL PASCAL ExpandBamBits(
		const BYTE *pRawBits,				// Uncompressed bit buffer.
		int nRawLen,							// Length of uncompressed buffer.
		int nWidth, int nHeight,			// Width and height of expanded image.
		BYTE chTransparentIndex,			// Palette index to use for transparency.
		COLORREF clrTransparent,			// Color to use for transparent sections.
		const COLORREF *pPal,				// Pointer to an array of 256 COLORREFs.
		COLORREF *pDIBits,					// Pointer to hold 32bit-depth expanded image.
		BOOL bIsCompressed);					// Some are not compressed and just need to be expanded to 32 bits.

	// Takes a 32bit-depth buffer and creates a windows bitmap.
	static BOOL PASCAL MakeBitmap(
		const COLORREF *pDIBits,			// Pointer to the 32bit-depth bits.
		int nWidth, int nHeight,			// Width and height of the image.
		HBITMAP &hBitmap);					// Handle of newly created bitmap.

	// Not all of the item icons are actually 32x32. For smaller ones this causes a problem
	// with the image list, which expects bitmaps of a constant size. This function takes
	// a handle to a bitmap and if it isn't 32x32 it creates a another bit that IS 32x32
	// and centers the passed in bitmap on it. Returns TRUE if the bitmap is already the
	// correct size or a successful attempt is made to resize it.
	static BOOL PASCAL FitAndCenterBitmap(
		HBITMAP &hOriginal,					// Handle to the bitmap to resize.
		COLORREF clrBackground,				// Color to use around expanded areas.
		int nWidth, int nHeight);			// Desired bitmap size.

private:
	// Generic buffer for converted BAMs.
	static COLORREF	m_clrDIBits[MAX_ICON_WIDTH*MAX_ICON_HEIGHT];
	
	// Pointer to the data buffer attached to the memory file.
	BYTE				*m_pData;
	UINT				m_nDataLen;

	INF_BAM_HEADER	*m_pHeader;
};

#endif
