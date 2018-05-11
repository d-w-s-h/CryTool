//---------------------------------------------------------------------------

#ifndef netClassH
#define netClassH
#include "myCryptoClass.h"
#include <System.Win.ScktComp.hpp>

//---------------------------------------------------------------------------
class netClass : public myCryptoClass
{
	protected:
		using myCryptoClass::hProv;
		using myCryptoClass::Prov;
		using myCryptoClass::ExchKey;
		using myCryptoClass::EnSessionKey;
		using myCryptoClass::DeSessionKey;
		using myCryptoClass::pbKeyBlobInternal;
		using myCryptoClass::pbKeyBlobExternal;

		wstring ansi2unicode(const std::string &str);
		bool AutoGenerateSessionKey();
	public:
		netClass(DWORD prov);

		bool NetExportPublicKey(TCustomWinSocket * connection);
		bool NetImportPublicKey(TCustomWinSocket * connection, BYTE *buffer, int nBytesRead);

		bool NetExportSessionKey(TCustomWinSocket * connection);
		bool NetImportSessionKey(TCustomWinSocket * connection, BYTE *buffer, int nBytesRead);

		bool NetSendEncryptFile(TCustomWinSocket * connection, TFileStream * file);
		bool NetRecieveEncryptFile(TCustomWinSocket * connection, TFileStream * file);

//		bool ClearupAfterDownload(TFileStream * file);









};

#endif
