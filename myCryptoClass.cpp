//---------------------------------------------------------------------------

#pragma hdrstop

#include "myCryptoClass.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
myCryptoClass::myCryptoClass(DWORD prov)
{
	this->Prov = prov;
	this->hProv = 0;        // Дескриптор контекста  критографического провайдера.
	this->hKey = 0;
	this->ExchKey = 0;
	this->keyLen =0;
    this->BLOCK_LENGTH =  1024;

}

bool myCryptoClass::CreateContainer(wstring userName)
{
	// Объявление и инициализация переменных.
	LPSTR pszUserName;		  // Буфер для хранения имени  ключевого контейнера.
	DWORD dwUserNameLen;	  // Длина буфера.
	LPCWSTR UserName;	          // Добавленное по выбору имя пользователя
					  // здесь будет использовано как имя
				  // ключевого контейнера (ограничение на 100 символов).

	UserName = userName.c_str();
	//  Для создания нового ключевого контейнера строка второго параметра
	//  заменяется на NULL здесь :
	// Создание нового контейнера.
	if(!CryptAcquireContextW(
					&this->hProv,
					UserName,
					NULL,
					this->Prov,
					CRYPT_NEWKEYSET))
	{
		OutputDebugStringW(L"Could not create a new key container.\n");
		OutputDebugStringA(ErrorIdToStr(GetLastError()).c_str());
	}
	else
	{
		OutputDebugStringW(L"A new key container has been created.\n");
		OutputDebugStringA(ErrorIdToStr(GetLastError()).c_str());
	}

}
bool myCryptoClass::LoadContainer(wstring userName)
{
	LPSTR pszUserName;		  // Буфер для хранения имени  ключевого контейнера.
	DWORD dwUserNameLen;	  // Длина буфера.
	LPCWSTR UserName;	          // Добавленное по выбору имя пользователя
	UserName = userName.c_str();
	if(CryptAcquireContext(
					&this->hProv,               // Дескриптор CSP
					UserName,                  // Имя контейнера
					NULL,                      // Использование провайдера по умолчанию
					this->Prov,	   // Тип провайдера
					0))                        // Значения флагов
	{
		wstringstream deb;
		deb << "A cryptcontext with the " << UserName << " key container has been acquired.\n" ;
		OutputDebugStringW(deb.str().c_str());
	}
	else OutputDebugStringA(ErrorIdToStr(GetLastError()).c_str());
}
bool myCryptoClass::DeleteContainer(wstring userName)
{
    LPSTR pszUserName;		  // Буфер для хранения имени  ключевого контейнера.
	DWORD dwUserNameLen;	  // Длина буфера.
	LPCWSTR UserName;	          // Добавленное по выбору имя пользователя
	UserName = userName.c_str();
	if(CryptAcquireContext(
					&this->hProv,               // Дескриптор CSP
					UserName,                  // Имя контейнера
					NULL,                      // Использование провайдера по умолчанию
					this->Prov,	   // Тип провайдера
					CRYPT_DELETEKEYSET))         // Значения флагов
	{
		wstringstream deb;
		deb << "Container" << UserName << " is deleted !\n" ;
		OutputDebugStringW(deb.str().c_str());
	}
	else OutputDebugStringA(this->ErrorIdToStr(GetLastError()).c_str());
}

bool myCryptoClass::CreateExchangeKey()
{

		// попытка получения дескриптора ключа обмена
	if(CryptGetUserKey(
					hProv,                     // Дескриптор CSP
					AT_KEYEXCHANGE,                   // Спецификация ключа
					&ExchKey))                         // Дескриптор ключа
	{
		OutputDebugStringA("A exchange key is already available.\n");
		return false;
	}

	// генерирование ключа

	if(!CryptGenKey(
					hProv,
					AT_KEYEXCHANGE,
					CRYPT_EXPORTABLE,
					&ExchKey))
	{
		OutputDebugStringA("Error occurred creating a exchange key.\n");
		OutputDebugStringA(ErrorIdToStr(GetLastError()).c_str());
		return false;
	}
	else OutputDebugStringA("Created a exchange key.\n");

	//следует запросить длину созданного ключа
	BYTE *pSizeData= new BYTE(4);
	DWORD sizeofkeysize = 4;

	if(!CryptGetKeyParam(ExchKey, KP_KEYLEN, pSizeData, &sizeofkeysize,0))
	{
	   OutputDebugStringA("********CryptGetKeyParam FAILED");
	}
	wstringstream deb;
	deb << "Key lenght - " << DWORD(*pSizeData);
	OutputDebugStringW(deb.str().c_str());

	CryptGetKeyParam(ExchKey, KP_ALGID, pSizeData, 0, 0) ;
	stringstream deb2;
	deb2 << "Key algorithm - " << this->AlgIDToStr(DWORD(*pSizeData));
	OutputDebugStringA(deb2.str().c_str());

}

bool myCryptoClass::ExportExchangeKey(wstring filename)
{
	DWORD bufflen = 0;
	CryptExportKey(ExchKey, 0, PUBLICKEYBLOB, 0, 0, &bufflen);
	vector<BYTE> buffer(bufflen);

	CryptExportKey(ExchKey, 0, PUBLICKEYBLOB, 0, &buffer[0], &bufflen);

	ofstream outfile(filename.c_str(), ios::out | ios::binary);
	outfile.write(&buffer[0], buffer.size());
	outfile.close();
}

bool myCryptoClass::Encrypt_File(wstring password, wstring filepath)
{
	bool result = true;
	HCRYPTHASH hash;
	HCRYPTKEY SessionKey = 0;
	//создадим новый дескриптор хэш-объекта по алгоритму ГОСТ 34.11-94
	result &= CryptCreateHash(hProv, CALG_GR3411, 0, 0, &hash);
	//хэширование парольной фразы
	result &= CryptHashData(hash, (const BYTE*)password.c_str(), password.size()*2, 0);
	//сессионный ключ с возможностью его экспорта
	result &= CryptDeriveKey(hProv, CALG_G28147, hash, CRYPT_EXPORTABLE, &SessionKey);
	CryptDestroyHash(hash);

	DWORD IVsize = 0;
	CryptGetKeyParam(SessionKey, KP_IV, 0, &IVsize, 0);
	//инициализационный вектор (начальный вектор) шифрования по ГОСТ 28147-89 в сессионном ключе SessionKey
	BYTE* IV = new BYTE(IVsize);
	//получение адреса и длинны инит вектора
	CryptGetKeyParam(SessionKey, KP_IV, IV , &IVsize, 0);
	//заполнение буфера рендомом
	CryptGenRandom(hProv, IVsize, IV );

	//Укажем, что новый буфер со случайными данными будет использоваться в качестве инициализационного вектора шифрования:
	result &= CryptSetKeyParam(SessionKey, KP_IV, IV , 0);

	//откроем файлы
	HANDLE hSource = CreateFileW(
					filepath.c_str(),
					GENERIC_READ,
					FILE_SHARE_READ | FILE_SHARE_WRITE,
					NULL,
					OPEN_EXISTING,
					FILE_ATTRIBUTE_NORMAL,
					NULL);
	if (hSource == INVALID_HANDLE_VALUE) return false;

	wstringstream encryptpath;
	encryptpath << filepath.c_str() <<".encrypted";

	HANDLE hDestination = CreateFileW(
					encryptpath.str().c_str(),
					GENERIC_WRITE,
					0,
					NULL,
					CREATE_ALWAYS,
					FILE_ATTRIBUTE_NORMAL,
					NULL);
	if (hDestination == INVALID_HANDLE_VALUE) return false;

	LARGE_INTEGER offset;
	offset.QuadPart = 0;
	bool eof = false;
	DWORD nWrote = 0;
	vector<BYTE> content;
	content.reserve(BLOCK_LENGTH);

	result &= WriteFile(         // записываем IV
					hDestination,
					IV,
					IVsize,
					&nWrote,
					NULL);
	DWORD sourceSize= GetFileSize(hSource, NULL);
	stringstream debug;

	do
	{
		DWORD newPosition = SetFilePointer(
					hSource,
					offset.LowPart,
					&offset.HighPart,
					FILE_BEGIN);
		result &= (newPosition == offset.QuadPart);

		DWORD nRead = 0;
		result &= ReadFile(
					hSource,
					&content[0],
					BLOCK_LENGTH,
					&nRead,
					NULL);

		if (!result)
		{
			return false;
		}

		eof = (nRead < BLOCK_LENGTH);

		result &= CryptEncrypt(
					SessionKey,
					0,
					eof,
					0,
					&content[0],
					&nRead,
					BLOCK_LENGTH);

		result &= WriteFile(
					hDestination,
					&content[0],
					nRead,
					&nWrote,
					NULL);
		offset.QuadPart += nRead;

		debug << "encrypted: " <<  offset.QuadPart << " of " << sourceSize;
		OutputDebugStringA(debug.str().c_str());
		debug.str("");

	}
	while(!eof && result);
	CloseHandle(hSource);
	CloseHandle(hDestination);
	delete[] IV;
	return result;
}

bool myCryptoClass::Decrypt_File(wstring password, wstring filepath)
{
	HANDLE hSource = CreateFileW(
					filepath.c_str(),
					GENERIC_READ,
					FILE_SHARE_READ | FILE_SHARE_WRITE,
					NULL,
					OPEN_EXISTING,
					FILE_ATTRIBUTE_NORMAL,
					NULL);
	if (hSource == INVALID_HANDLE_VALUE) return false;

	wstringstream decryptpath;
	decryptpath << filepath.c_str() <<".decrypted";

	HANDLE hDestination = CreateFileW(
					decryptpath.str().c_str(),
					GENERIC_WRITE,
					0,
					NULL,
					CREATE_ALWAYS,
					FILE_ATTRIBUTE_NORMAL,
					NULL);
	if (hDestination == INVALID_HANDLE_VALUE) return false;


	bool result = true;
	HCRYPTHASH hash;
	HCRYPTKEY SessionKey = 0;

	result &= CryptCreateHash(hProv, CALG_GR3411, 0, 0, &hash);
	result &= CryptHashData(hash, (const BYTE*)password.c_str(), password.size()*2, 0); //хэширование парольной фразы
	result &= CryptDeriveKey(hProv, CALG_G28147, hash, CRYPT_EXPORTABLE, &SessionKey);
	CryptDestroyHash(hash);

	DWORD IVsize = 0;
	CryptGetKeyParam(SessionKey, KP_IV, 0, &IVsize, 0);
	//инициализационный вектор (начальный вектор) шифрования по ГОСТ 28147-89 в сессионном ключе SessionKey
	BYTE* IV = new BYTE(IVsize);
	//получение IV из файла
	DWORD nRead = 0;
	result &= ReadFile(
					hSource,
					IV,
					IVsize,
					&nRead,
					NULL);
	//Укажем, что новый IV будет использоваться в качестве инициализационного вектора шифрования:
	result &= CryptSetKeyParam(SessionKey, KP_IV, IV , 0);



	LARGE_INTEGER offset;
	offset.QuadPart = 0;
	DWORD nWrote = 0;
	offset.QuadPart = IVsize;
	bool eof = false;
	vector<BYTE> content;
	content.reserve(BLOCK_LENGTH);
	while (!eof && result)
	{
		DWORD newPosition = SetFilePointer(
					hSource,
					offset.LowPart,
					&offset.HighPart,
					FILE_BEGIN);
		result &= (newPosition == offset.QuadPart);

		result &= ReadFile(
					hSource,
					&content[0],
					BLOCK_LENGTH,
					&nRead,
					NULL);

		if (!result)
		{
			return false;
		}

		eof = (nRead < BLOCK_LENGTH);


		result &= CryptDecrypt(
					SessionKey,
					0,
					eof,
					0,
					&content[0],
					&nRead);

		result &= WriteFile(
					hDestination,
					&content[0],
					nRead,
					&nWrote,
					NULL);
        offset.QuadPart += nRead;
	}
	CloseHandle(hSource);
	CloseHandle(hDestination);
	delete[] IV;
	return result;
}




























//Для работы с контейнером должна быть создана функция получения имени текущего пользователя
string __fastcall myCryptoClass::GetUserName()
{
  stringstream result;
  char Buffer[ MAX_PATH + 1 ];
  DWORD sz = 0;
  sz = MAX_PATH - 1;
  if ( GetUserNameA( Buffer, &sz ) )
  {
	result << Buffer;
  }
  else
	result << "Error " <<  GetLastError() ;
  return result.str();
}

string __fastcall myCryptoClass::ErrorIdToStr( __int64 err_type )
{
	string result;
	switch ( err_type )
	{
		case ERROR_BUSY:
		  result = "The CSP context specified by hProv is currently being used by another process";
		break;
		case ERROR_INVALID_HANDLE:
		  result = "One of the parame-ters specifies an invalid handle";
		break;
		case ERROR_INVALID_PARAMETER:
		  result = "One of the parame-ters contains an invalid value. This is most often an illegal pointer";
		break;
		case ERROR_NO_MORE_ITEMS:
		  result = "The end of the enu-meration list has been reached. No valid data has been placed in the pbData buffer. This error is only returned when dwParam equals PP_ENUMALGS or PP_ENUMCONTAINERS";
		break;
		case ERROR_NOT_ENOUGH_MEMORY:
		  result = "The operating system ran out of memory during the operation";
		break;
		case NTE_BAD_ALGID:
		  result = "The hKey session key specifies an algorithm that this CSP does not support";
		break;
		case NTE_BAD_DATA:
		  result = "The data to be en-crypted is invalid. For example, when a block cipher is used and the Final flag is FALSE, the value specified by pdwDataLen must be a multiple of the block size";
		break;
		case NTE_BAD_FLAGS:
		  result = "The dwFlags parame-ter has an illegal value";
		break;
		case NTE_BAD_HASH:
		  result = "The hHash parameter contains an invalid handle";
		break;
		case NTE_BAD_HASH_STATE:
		  result = "An attempt was made to add data to a hash object that is already marked \"finished";
		break;
		case NTE_BAD_KEY:
		  result = "One or both of the keys specified by hKey and hExpKey are invalid";
		break;
		case NTE_BAD_KEY_STATE:
		  result = "You do not have per-mission to export the key. That is, when the hKey key was created, the CRYPT_EXPORTABLE flag was not specified";
		break;
		case NTE_BAD_KEYSET:
		  result = "The Registry entry for the key container could not be opened and may not exist";
		break;
		case NTE_BAD_KEYSET_PARAM:
		  result = "The pszContainer or pszProvider parameter is set to an illegal value";
		break;
		case NTE_BAD_LEN:
		  result = "The size of the out-put buffer is too small to hold the generated ciphertext";
		break;
		case NTE_BAD_PROV_TYPE:
		  result = "The value of the dwProvType parameter is out of range. All provider types must be from 1 to 999, inclusive";
		break;
		case NTE_BAD_PUBLIC_KEY:
		  result = "The key blob type specified by dwBlobType is PUBLICKEYBLOB, but hExpKey does not contain a public key handle";
		break;
		case NTE_BAD_SIGNATURE:
		  result = "The provider DLL signature did not verify correctly. Either the DLL or the digital signature has been tampered with";
		break;
		case NTE_BAD_TYPE:
		  result = "The dwParam parame-ter specifies an unknown parameter number";
		break;
		case NTE_BAD_UID:
		  result = "The hProv parameter does not contain a valid context handle";
		break;
		case NTE_BAD_VER:
		  result = "The key blob's ver-sion number does not match the CSP version. This usually indicates that the CSP needs to be upgraded";
		break;
		case NTE_DOUBLE_ENCRYPT:
		  result = "The application at-tempted to encrypt the same data twice";
		break;
		case NTE_EXISTS:
		  result = "The dwFlags parame-ter is CRYPT_NEWKEYSET, but the key container already exists";
		break;
		case NTE_FAIL:
		  result = "The function failed in some unexpected way";
		break;
		case NTE_KEYSET_ENTRY_BAD:
		  result = "The Registry entry for the pszContainer key container was found (in the HKEY_CURRENT_USER window), but is corrupt. See the section System Administration for details about CryptoAPI's Registry usage";
		break;
		case NTE_KEYSET_NOT_DEF:
		  result = "No Registry entry exists in the HKEY_CURRENT_USER window for the key container spec-ified by pszContainer";
		break;
		case NTE_NO_KEY:
		  result = "A session key is be-ing exported and the hExpKey parameter does not specify a public key";
		break;
		case NTE_NO_MEMORY:
		  result = "The CSP ran out of memory during the operation";
		break;
		case NTE_PROV_DLL_NOT_FOUND:
		  result = "The provider DLL file does not exist or is not on the current path";
		break;
		case NTE_PROV_TYPE_ENTRY_BAD:
		  result = "The Registry entry for the provider type specified by dwProvType is corrupt. This er-ror may relate to either the user default CSP list or the machine default CSP list. See the section System Administration for de-tails about CryptoAPI's Registry usage";
		break;
		case NTE_PROV_TYPE_NO_MATCH:
		  result = "The provider type specified by dwProvType does not match the provider type found in the Registry. Note that this error can only occur when pszProvider specifies an actual CSP name";
		break;
		case NTE_PROV_TYPE_NOT_DEF:
		  result = "No Registry entry exists for the provider type specified by dwProvType";
		break;
		case NTE_PROVIDER_DLL_FAIL:
		  result = "The provider DLL file could not be loaded, and may not exist. If it exists, then the file is not a valid DLL";
		break;
		case NTE_SIGNATURE_FILE_BAD:
		  result = "An error occurred while loading the DLL file image, prior to verifying its";
		break;
		default:
		result = "Unknown error";
	}
	return result;
}

string __fastcall myCryptoClass::AlgIDToStr( DWORD alg_type )
{
  string result;
  switch ( alg_type )
  {
	case CALG_MD2:
	  result = "MD2 hashing algorithm";
	break;
	case CALG_MD4:
	  result = "MD4 hashing algorithm";
	break;
	case CALG_MD5:
	  result = "MD5 hashing algorithm";
	break;
	case CALG_SHA:
	  result = "SHA hashing algorithm";
	break;
	case CALG_MAC:
	  result = "MAC keyed hash algorithm";
	break;
	case CALG_RSA_SIGN:
	  result = "RSA public-key signature algorithm";
	break;
	case CALG_DSS_SIGN:
	  result = "DSA public-key signature algorithm";
	break;
	case CALG_RSA_KEYX:
	  result = "RSA public-key key exchange algorithm";
    break;
	case CALG_DES:
      result = "DES encryption algorithm";
    break;
    case CALG_3DES_112:
      result = "TripleDES 112 encryption algorithm";
    break;
    case CALG_3DES:
	  result = "TripleDES encryption algorithm";
    break;
    case CALG_RC2:
      result = "RC2 block encryption algorithm";
    break;
    case CALG_RC4:
      result = "RC4 stream encryption algorithm";
    break;
    case CALG_SEAL:
      result = "SEAL algorithm";
    break;
    case CALG_DH_SF:
      result = "Diffie-Hellman store and forward key exchange algorithm";
	break;
    case CALG_DH_EPHEM:
      result = "Diffie-Hellman EPHEM algorithm";
    break;
    case CALG_AGREEDKEY_ANY:
      result = "AGREEDKEY ANY algorithm";
    break;
    case CALG_KEA_KEYX:
      result = "KEA key exchange algorithm";
    break;
    case CALG_HUGHES_MD5:
      result = "Hughes MD5 hashing algorithm";
    break;
	case CALG_SKIPJACK:
      result = "SKIPJACK algorithm";
    break;
    case CALG_TEK:
      result = "TEK hashing algorithm";
    break;
    case CALG_CYLINK_MEK:
      result = "CYLINK MEK hashing algorithm";
    break;
    case CALG_SSL3_SHAMD5:
      result = "SSL3 client authentication";
    break;
    case CALG_SSL3_MASTER:
	  result = "SSL3 client authentication";
    break;
    case CALG_SCHANNEL_MASTER_HASH:
      result = "Secure channel MASTER hashing algorithm";
    break;
    case CALG_SCHANNEL_MAC_KEY:
      result = "Secure channel MAC keyed hash algorithm";
    break;
    case CALG_SCHANNEL_ENC_KEY:
      result = "Secure channel ENC keyed hash algorithm";
    break;
    case CALG_PCT1_MASTER:
      result = "PCT1 client authentication";
	break;
    case CALG_SSL2_MASTER:
      result = "SSL2 client authentication";
    break;
    case CALG_TLS1_MASTER:
      result = "TLS1 client authentication";
    break;
    case CALG_RC5:
      result = "RC5 stream encryption algorithm";
    break;
    case CALG_HMAC:
      result = "HMAC keyed hash algorithm";
    break;
	case CALG_GR3411:
      result = "GOST R34.11-93 hashing algorithm";
    break;
    case CALG_G28147_MAC:
      result = "GOST 28147-89 imitodefense algo-rithm";
    break;
//	case CALG_G28147_IMIT:
//	  result = "GOST 28147-89 imitodefense algo-rithm";
//	break;
//	case CALG_GR3410:
//	  result = "GOST R34.10 algorithm";
//	break;
    case CALG_GR3410EL:
	  result = "GOST R34.10 EL algorithm";
    break;
    case CALG_G28147:
      result = "GOST 28147-89 algorithm";
	break;
//	case CALG_DH_EX_SF:
//	  result = "Diffie-Hellman store and forward key exchange algorithm";
//    break;
//	case CALG_DH_EX_EPHEM:
//	  result = "Diffie-Hellman EPHEM key exchange algorithm";
//	break;
	case CALG_PRO_AGREEDKEY_DH:
	  result = "PRO AGREEDKEY Diffie-Hellman algorithm";
	break;
	case CALG_PRO_EXPORT:
	  result = "PRO EXPORT algorithm";
	break;
	case CALG_SIMPLE_EXPORT:
	  result = "SIMPLE EXPORT algorithm";
	break;
	case CALG_TLS1_MAC_KEY:
	  result = "TLS1 HMAC keyed hash algorithm";
	break;
	case CALG_TLS1_ENC_KEY:
	  result = "TLS1 ENC keyed hash algorithm";
	break;
  default:
	result = "unknown algorithm";
  }
  return result;
}

void myCryptoClass::CleanUp()
{
	if(this->hKey)
	CryptDestroyKey(this->hKey);
	if(this->hProv)
	CryptReleaseContext(this->hProv, 0);
}