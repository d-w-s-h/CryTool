//---------------------------------------------------------------------------

#pragma hdrstop

#include "netClass.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

netClass::netClass(DWORD prov) : myCryptoClass(prov)
{

}
bool netClass::NetExportPublickKey(TCustomWinSocket * connection)
{
	wstringstream keyname;
	if (CreateDirectory(L"TEMP", NULL) ||
	ERROR_ALREADY_EXISTS == GetLastError())
	{

		keyname << L"TEMP\\" << GetmyUserName().c_str() <<".publickey";
		ExportExchangeKey(keyname.str().c_str());
	}
	else
	{
		 // Failed to create directory.
	}
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

bool netClass::NetImportPublickKey(TCustomWinSocket * connection, BYTE *buffer, int nBytesRead)
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
		LoadExchangeKey(wName);

		DeleteFile( wName.c_str() );
	}
	else
	{

	}






}

std::wstring netClass::ansi2unicode(const std::string &str)
{
	int size_needed = MultiByteToWideChar(CP_ACP, 0, &str[0], (int)str.size(), NULL, 0);
	std::wstring wstrTo(size_needed, 0);
	MultiByteToWideChar(CP_ACP, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
	return wstrTo;
}
