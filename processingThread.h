//---------------------------------------------------------------------------

#ifndef processingThreadH
#define processingThreadH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include "myCryptoClass.h"
//---------------------------------------------------------------------------
class processingThread : public TThread
{
protected:
	void __fastcall Execute();
	bool IsEncrypt;
	myCryptoClass *CSP;
	wstring Password;
	wstring Filepath;
	ULONGLONG *progress;
public:
	__fastcall processingThread(bool isEncrypt , myCryptoClass *CSP,wstring password,wstring filepath, bool CreateSuspended);

};
//---------------------------------------------------------------------------
#endif
