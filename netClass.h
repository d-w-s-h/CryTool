//---------------------------------------------------------------------------

#ifndef netClassH
#define netClassH
#include "myCryptoClass.h"
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
	public:
		netClass();
        AutogenerateSessionKey();




};

#endif
