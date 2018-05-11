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
	public:
		netClass(DWORD prov);
		bool AutogenerateSessionKey();
		bool NetExportPublickKey(TCustomWinSocket * connection);
		bool NetImportPublickKey(TCustomWinSocket * connection, BYTE *buffer, int nBytesRead);







};

#endif
