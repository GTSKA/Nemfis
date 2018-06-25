
#pragma once
//#ifndef UTILTY_NEMFIS_H
//#define UTILTY_NEMFIS_H

#include <windows.h>
#include <assert.h>
#include <strsafe.h>


//-----------------------------------------------------------------------------
//MACROS
//-----------------------------------------------------------------------------
#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }
#define MULTI_TO_WIDE( ach,wch )  MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, ach, -1, wch, _MAX_PATH );
#define WIDE_TO_MULTI(wch,ach,size) WideCharToMultiByte(CP_ACP,WC_COMPOSITECHECK,wch,-1,ach,size*sizeof(char),NULL,NULL);

//#endif