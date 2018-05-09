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

#   include <iostream>


#include "WinCryptEx.h"
#include <sstream>
#include <fstream>
#include <iterator>
#include <vector>
using namespace std;
//---------------------------------------------------------------------------

class myCryptoClass
{
	protected:
		HCRYPTPROV hProv;        // Дескриптор контекста  критографического провайдера.
		DWORD Prov;
		HCRYPTKEY ExchKey ; //      открытый ключ, предназначенный для обмена сессионными ключами
		DWORD keyLen;  //длина ключа обмена
		DWORD BLOCK_LENGTH;
		HCRYPTKEY EnSessionKey;
		HCRYPTKEY DeSessionKey;
		HCRYPTKEY hAgreeKey;    //ключ согласования (им шифруется сессионный ключ)
		BYTE *pbKeyBlobInternal;  // Указатель на наш ключевой BLOB
		BYTE *pbKeyBlobExternal;  // Указатель на присланный ключевой BLOB

	public:
		myCryptoClass(DWORD prov);
		void CleanUp();
		string __fastcall GetUserName();
		string __fastcall ErrorIdToStr( __int64 err_type );
		string __fastcall AlgIDToStr( DWORD alg_type );
		bool CreateContainer(wstring userName);
		bool LoadContainer(wstring userName);
		bool DeleteContainer(wstring userName);

		bool CreateExchangeKey();
		bool LoadExchangeKey(wstring filename);
		bool ExportExchangeKey(wstring filename);

		bool Encrypt_File(wstring password, wstring filepath, bool usingImportKey);
		bool Decrypt_File(wstring password, wstring filepath, bool usingImportKey);

		bool LoadSessionKey(wstring filename);
		bool ExportSessionKey(wstring filename);

};



#endif
