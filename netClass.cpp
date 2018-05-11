//---------------------------------------------------------------------------

#pragma hdrstop

#include "netClass.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

netClass::netClass(DWORD prov) : myCryptoClass(prov)
{

}
bool netClass::NetExportPublicKey(TCustomWinSocket * connection)
{
	wstringstream keyname;
	if (CreateDirectory(L"TEMP", NULL) ||
	ERROR_ALREADY_EXISTS == GetLastError())
	{

		keyname << L"TEMP\\" << GetmyUserName().c_str() <<".publickey";
		ExportExchangeKey(keyname.str().c_str());
		TFileStream *KEY = new TFileStream((UnicodeString) keyname.str().c_str(),  fmOpenReadWrite );//
		AnsiString header;
		header =( "PUBLICKEY#" + (UnicodeString)keyname.str().c_str() +  "##" );
		TMemoryStream *ms = new TMemoryStream();
		ms->Write(header.c_str(), header.Length());

		BYTE *buffer = new BYTE[KEY->Size];
		KEY->ReadBuffer(buffer,KEY->Size);
		ms->WriteBuffer(buffer, KEY->Size);
		ms->Position=0;
		connection->SendStream(ms);

		delete(buffer);
		ms->Clear();
		KEY->Free();

		DeleteFile( keyname.str().c_str() );
	}
	else
	{
		 // Failed to create directory.
	}


}

bool netClass::NetImportPublicKey(TCustomWinSocket * connection, BYTE *buffer, int nBytesRead)
{

	if (CreateDirectory(L"TEMP", NULL) ||
	ERROR_ALREADY_EXISTS == GetLastError())
	{
		AnsiString Rtext = (char*)buffer;
		int dataoffset = Rtext.Pos( "##" );
		Rtext.Delete( 1 , Rtext.Pos( "#" ) ) ;
		AnsiString Name = Rtext.SubString( 0 , Rtext.Pos( "#" ) -1 );
		AnsiString FileName = Name.SubString( Name.LastDelimiter( "\\" ) + 1 , Name.Length() );
		Rtext.Delete( 1 , Rtext.Pos( "#" ) );
		TFileStream *KEY = new TFileStream(Name, fmCreate | fmOpenRead );
		KEY->WriteBuffer(buffer+dataoffset+1,nBytesRead - dataoffset-1);
		KEY->Free();

		string sName = Name.c_str();
		wstring wName;
		wName = ansi2unicode(sName);

		if(LoadExchangeKey(wName))
		{
			DeleteFile( wName.c_str() );
			return true;
		}
		else
		{
			DeleteFile( wName.c_str() );
			return false;
		}
	}
	else
	{
		return false;
	}

}
bool netClass::AutoGenerateSessionKey()
{
	if(!CryptGenKey(this->hProv,
					CALG_G28147,
					CRYPT_EXPORTABLE,
					&this->EnSessionKey))
	{
		return false;
	}
	this->DeSessionKey = this->EnSessionKey;
}

bool netClass::NetExportSessionKey(TCustomWinSocket * connection)
{
	wstringstream keyname;
	if (CreateDirectory(L"TEMP", NULL) ||
	ERROR_ALREADY_EXISTS == GetLastError())
	{
		keyname << L"TEMP\\" << GetmyUserName().c_str() <<".sessionkey";

		if(!this->AutoGenerateSessionKey())
		{
			return false;
		}

		if(!this->ExportSessionKey(keyname.str().c_str()))
        {
			return false;
		}

		TFileStream *KEY = new TFileStream((UnicodeString) keyname.str().c_str(),  fmOpenReadWrite );//
		AnsiString header;
		header =( "SESSIONKEY#" + (UnicodeString)keyname.str().c_str() +  "##" );
		TMemoryStream *ms = new TMemoryStream();
		ms->Write(header.c_str(), header.Length());
		BYTE *buffer = new BYTE[KEY->Size];
		KEY->ReadBuffer(buffer,KEY->Size);
		ms->WriteBuffer(buffer, KEY->Size);
		ms->Position=0;
		connection->SendStream(ms);
		delete(buffer);
		ms->Clear();
		KEY->Free();
		DeleteFile( keyname.str().c_str() );
	}
	else
	{
         return false;
		 // Failed to create directory.
	}
}


bool netClass::NetImportSessionKey(TCustomWinSocket * connection, BYTE *buffer, int nBytesRead)
{
    if (CreateDirectory(L"TEMP", NULL) ||
	ERROR_ALREADY_EXISTS == GetLastError())
	{
		AnsiString Rtext = (char*)buffer;
		int dataoffset = Rtext.Pos( "##" );
		Rtext.Delete( 1 , Rtext.Pos( "#" ) ) ;
		AnsiString Name = Rtext.SubString( 0 , Rtext.Pos( "#" ) -1 );
		AnsiString FileName = Name.SubString( Name.LastDelimiter( "\\" ) + 1 , Name.Length() );
		Rtext.Delete( 1 , Rtext.Pos( "#" ) );
		TFileStream *KEY = new TFileStream(Name, fmCreate | fmOpenRead );
		KEY->WriteBuffer(buffer+dataoffset+1,nBytesRead - dataoffset-1);
		KEY->Free();

		string sName = Name.c_str();
		wstring wName;
		wName = ansi2unicode(sName);

		if(this->LoadSessionKey(wName))
		{
			DeleteFile( wName.c_str() );
			return true;
		}
		else
		{
			DeleteFile( wName.c_str() );
			return false;
		}
	}
	else
	{
		return false;
	}
}

bool netClass::NetSendEncryptFile(TCustomWinSocket * connection, TFileStream * file)
{

	if(!this->Encrypt_File(L"", file->FileName.c_str(), true))
	{
		return false;
	}
	wstringstream EncryptFileName;
	EncryptFileName << file->FileName.c_str() <<".encrypted";
	file->Free();
	file = new TFileStream(EncryptFileName.str().c_str(),  fmOpenRead);
	connection->SendStream(file);
//	file->Free();
//	DeleteFile( EncryptFileName.str().c_str());

}
bool netClass::NetRecieveEncryptFile(TCustomWinSocket * connection, TFileStream * file)
{

	wstring recievedfilename = file->FileName.c_str();
	file->Free();
	if(!this->Decrypt_File(L"", recievedfilename.c_str(), true))
	{
		return false;
	}

	DeleteFile(recievedfilename.c_str());

	wstringstream oldname;
	oldname << recievedfilename.c_str() << ".decrypted";

	_wrename(oldname.str().c_str(),recievedfilename.c_str());
}


//bool netClass::ClearupAfterDownload(TFileStream * file)
//{
//	wstring filename = file->FileName.c_str();
//	file->Free();
//	DeleteFile(filename.c_str());
//}

std::wstring netClass::ansi2unicode(const std::string &str)
{
	int size_needed = MultiByteToWideChar(CP_ACP, 0, &str[0], (int)str.size(), NULL, 0);
	std::wstring wstrTo(size_needed, 0);
	MultiByteToWideChar(CP_ACP, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
	return wstrTo;
}
