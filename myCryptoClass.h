//---------------------------------------------------------------------------

#ifndef myCryptoClassH
#define myCryptoClassH

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

class myCryptoClass
{
	protected:
		HCRYPTPROV hProv;        // Дескриптор контекста  критографического провайдера.
		HCRYPTKEY hKey;          // Дескриптор открытого/закрытого ключа.
		DWORD Prov;

	public:
		myCryptoClass(DWORD prov);
		void CleanUp();
		string __fastcall GetUserName();
		string __fastcall ErrorIdToStr( __int64 err_type );
		string __fastcall AlgIDToStr( DWORD alg_type );
		bool CreateContainer(wstring userName);
		bool LoadContainer(wstring userName);
		bool DeleteContainer(wstring userName);
};

#endif
