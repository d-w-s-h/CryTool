//---------------------------------------------------------------------------

#ifndef functionsH
#define functionsH
#include <stdio.h>

#ifdef _WIN32
#   include <windows.h>
#   include <wincrypt.h>
#else
#   include <string.h>
#   include <stdlib.h>
#   include <CSP_WinDef.h>
#   include <CSP_WinCrypt.h>
#endif
#include "WinCryptEx.h"
#   include <iostream>
#include <sstream>
using namespace std;


//---------------------------------------------------------------------------
string __fastcall GetUserName();
string __fastcall ErrorIdToStr( __int64 err_type );
string __fastcall AlgIDToStr( DWORD alg_type );

#endif
