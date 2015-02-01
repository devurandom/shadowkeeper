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


// stdafx.h : include file for standard system include files
// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__8C7CE887_597C_11D4_9BE4_E8E92BBF2D0A__INCLUDED_)
#define AFX_STDAFX_H__8C7CE887_597C_11D4_9BE4_E8E92BBF2D0A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
#define OEMRESOURCE

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxtempl.h>
#include <atlbase.h>
#include <math.h>

#define	DIR_MULTISAVE		"mpsave\\"
#define	DIR_SINGLESAVE		"save\\"
#define	DIR_PORTRAITS		"Portraits\\"
#define	DIR_OVERRIDE		"Override\\"
#define	DIR_CHARACTERS		"Characters\\"

#define	LOGFILE				"ShadowKeeper.log"

// Defines for the user list filenames.
#define ULF_KITS			"Kits.uld"
#define ULF_GENDER		"Gender.uld"
#define ULF_RACE			"Race.uld"
#define ULF_CLASS			"Class.uld"
#define ULF_ALIGNMENT	"Alignment.uld"
#define ULF_RACIALENEMY "RacialEnemy.uld"
#define ULF_NUMATTACKS	"NumAttacks.uld"
#define ULF_PROFS			"Profs.uld"
#define ULF_AFFECTS		"Affects.uld"

// Standard document hints.
#define	HINT_CHANGE_GRIDLINES		5000
#define	HINT_OVERRIDE_CHANGED		5001

// Resource types used by the infinity data files.
#define RESTYPE_BMP			0x0001
#define RESTYPE_WAV			0x0004
#define RESTYPE_BAM			0x03e8
#define RESTYPE_MOS			0x03ec
#define RESTYPE_ITM			0x03ed
#define RESTYPE_SPL			0x03ee
#define RESTYPE_BCS			0x03ef
#define RESTYPE_IDS			0x03f0
#define RESTYPE_CRE			0x03f1
#define RESTYPE_2DA			0x03f4
#define RESTYPE_BS			0x03f9

#define ERR_NONE				0

// Used tell CMainFrame to index the resource.key file.
#define MSG_DORESOURCES		WM_APP+1

extern const char *_pszWebAddress;
extern const char *_pszEmailAddress;

/////////////////////////////////////////////////
// Number validation macros.
//

// Validates for a 1-20 value.
#define VALIDATE20(nValue) ((nValue < 1 ? 1 : (nValue > 20 ? 20 : nValue)))

// Validates a character attribute.
#define VALIDATE_ATTR(nValue) ((nValue < 2 ? 2 : (nValue > 25 ? 25 : nValue)))

// Validates a character proficiency.
#define VALIDATE_PROF(nValue) ((nValue < 0 ? 0 : (nValue > 7 ? 7 : nValue)))

// Validates a character resistance.
#define VALIDATE_RES(nValue) ((nValue < 0 ? 0 : (nValue > 255 ? 255 : nValue)))

// Validates thief skills.
#define VALIDATE_THIEF(nValue) ((nValue < 0 ? 0 : (nValue > 255 ? 255 : nValue)))

// Validates a given range.
#define VALIDATE_RANGE(nValue,nMin,nMax) ((nValue < nMin ? nMin : (nValue > nMax ? nMax : nValue)))

//
/////////////////////////////////////////////////


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__8C7CE887_597C_11D4_9BE4_E8E92BBF2D0A__INCLUDED_)
