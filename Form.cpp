//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Form.h"
#include <string>
#include <sstream>



//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TMainForm *MainForm;
processingThread *th;


//---------------------------------------------------------------------------
__fastcall TMainForm::TMainForm(TComponent* Owner)
	: TForm(Owner)
{

	CSP = new myCryptoClass( PROV_GOST_2001_DH);
	MainForm->UsernameEdit->Text = (UnicodeString) CSP->GetUserName().c_str();
	bool CreateContainer();
	EncryptFileButton->Enabled =false;
	DecryptFileButton->Enabled =false;
	ExportKeyButton->Enabled =false;
	GenerateKeyButton->Enabled =false;
	LoadKeyButton->Enabled =false;
	SessionExBtn->Enabled = false;
	SessionLoadBtn->Enabled = false;

	this->usingImportKey=false;
	this->isPublicKeyLoaded=false;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::CreateContainerButtonClick(TObject *Sender)
{
	if(CSP->CreateContainer(MainForm->UsernameEdit->Text.c_str()))
	{
		EncryptFileButton->Enabled =true;
		DecryptFileButton->Enabled =true;
		ExportKeyButton->Enabled =true;
		GenerateKeyButton->Enabled =true;
		LoadKeyButton->Enabled  =true;
		SessionLoadBtn->Enabled = true;
		stringstream msg;
		msg << "Контейнер " << CSP->GetUserName().c_str() <<  " создан";

		InfoLabel->Caption = msg.str().c_str();
	}
	else
	{
		InfoLabel->Caption = "Ошибка создания конейнера (либо контейнер был создан ранее - попробуйте загрузить его)";
	}
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::LoadContainerButtonClick(TObject *Sender)
{
	if(CSP->LoadContainer(MainForm->UsernameEdit->Text.c_str()))
	{
		EncryptFileButton->Enabled =true;
		DecryptFileButton->Enabled =true;
		ExportKeyButton->Enabled =true;
		GenerateKeyButton->Enabled =true;
		LoadKeyButton->Enabled  =true;
		SessionLoadBtn->Enabled = true;
		InfoLabel->Caption = "Контейнер был успешно загружен";
	}
	else
	{
		InfoLabel->Caption = "Ошибка загрузки конейнера (либо контейнер был загружен ранее)";
	}
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::DeleteContainerButtonClick(TObject *Sender)
{
	if(CSP->DeleteContainer(MainForm->UsernameEdit->Text.c_str()))
	{
		EncryptFileButton->Enabled =false;
		DecryptFileButton->Enabled =false;
		ExportKeyButton->Enabled =false;
		GenerateKeyButton->Enabled =false;
		LoadKeyButton->Enabled =false;
		SessionExBtn->Enabled = false;
		SessionLoadBtn->Enabled = false;
		InfoLabel->Caption = "Контейнер был успешно удален";
		this->isPublicKeyLoaded=false;
	}
	else
	{
		InfoLabel->Caption = "Контейнер не был удален (либо удален ранее)";
	}
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::FormClose(TObject *Sender, TCloseAction &Action)
{
	CSP->CleanUp();
    ServerSocket->Close();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::GenerateKeyButtonClick(TObject *Sender)
{
	if(CSP->CreateExchangeKey())
	{
//		SessionExBtn->Enabled = true;
		InfoLabel->Caption = "Пара ключей сгенерирована";
	}
	else
	{
		InfoLabel->Caption = "Ошибка генерации ключей (либо ключи уже был сгенерированы ранее)";
	}
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ExportKeyButtonClick(TObject *Sender)
{

	if(SaveExKeyDialog->Execute())
	{
		CSP->ExportExchangeKey(SaveExKeyDialog->FileName.c_str());
		InfoLabel->Caption = "Открытый ключ успешно экспортирован";
	}
	else
	{
		InfoLabel->Caption = "Ошибка экспорта открытого ключа";
	}

}
//---------------------------------------------------------------------------


void __fastcall TMainForm::EncryptFileButtonClick(TObject *Sender)
{
	if(EnPasswordEdit->Text != PassConfirmEdit->Text)
	{
		Application->MessageBoxW(L"Несовпадение паролей, попробуйте еще раз.",L"Ошибка", MB_OK);
		return;
	}

	usingImportKey = UsingImportKeyCheck->Checked;
	if(OpenFileDialog->Execute())
	{
		th = new processingThread(
						 true,
						 CSP,
						 EnPasswordEdit->Text.c_str(),
						 OpenFileDialog->FileName.c_str(),
						 usingImportKey,
						 false);
	}
	SessionExBtn->Enabled = true;

}
//---------------------------------------------------------------------------


void __fastcall TMainForm::DecryptFileButtonClick(TObject *Sender)
{
    usingImportKey = UsingImportKeyCheck->Checked;
	if(OpenFileDialog->Execute())
	{
		th = new processingThread(
						 false,
						 CSP,
						 DePasswordEdit->Text.c_str(),
						 OpenFileDialog->FileName.c_str(),
						 usingImportKey,
						 false);

	}
	SessionExBtn->Enabled = true;
}
//---------------------------------------------------------------------------



void __fastcall TMainForm::SessionExBtnClick(TObject *Sender)
{
	if(SaveExKeyDialog->Execute())
	{
		if(!this->isPublicKeyLoaded)
		{
			Application->MessageBoxW(L"Для экспортирования зашифрованного сессионного ключа сначала необходимо импортировать открытый ключ удаленного пользователя, с которым будет осуществляться обмен.",L"Ошибка", MB_OK | MB_ICONWARNING);
			return;
		}
		CSP->ExportSessionKey(SaveExKeyDialog->FileName.c_str());
		InfoLabel->Caption = "Сессионный ключ успешно экспортирован";
	}
	else
	{
		InfoLabel->Caption = "Ошибка экспорта сессионного ключа";
	}

}
//---------------------------------------------------------------------------


void __fastcall TMainForm::LoadKeyButtonClick(TObject *Sender)
{
	if(OpenFileDialog->Execute())
	{
		CSP->LoadExchangeKey(OpenFileDialog->FileName.c_str());
		InfoLabel->Caption = "Открытый ключ успешно импортирован";
		this->isPublicKeyLoaded=true;
	}
	else
	{
		InfoLabel->Caption = "Ошибка импорта открытого ключа";
	}
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::SessionLoadBtnClick(TObject *Sender)
{
	if(OpenFileDialog->Execute())
	{
        if(!this->isPublicKeyLoaded)
		{
			Application->MessageBoxW(L"Для импорта зашифрованного сессионного ключа сначала необходимо импортировать открытый ключ удаленного пользователя, с которым будет осуществляться обмен.",L"Ошибка", MB_OK | MB_ICONWARNING);
			return;
		}
		CSP->LoadSessionKey(OpenFileDialog->FileName.c_str());
		InfoLabel->Caption = "Сессионный ключ успешно импортирован";
	}
	else
	{
        InfoLabel->Caption = "Ошибка импорта сессионного ключа";
	}
}
//---------------------------------------------------------------------------





void __fastcall TMainForm::SendButtonClick(TObject *Sender)
{
//	TFileStream *File;
	if(OpenFileDialog->Execute())
	{

		File = new TFileStream( OpenFileDialog->FileName, fmOpenRead | fmShareDenyNone );
		ServerSocket->Socket->Connections[0]->SendText( "file#" + OpenFileDialog->FileName + "#" + IntToStr( File->Size ) + "#" );

//		ServerSocket->Socket->Connections[0]->SendText("#end");
	}


}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ConnectButtonClick(TObject *Sender)
{
	ClientSocket->Address = IPEdit->Text;
	ClientSocket->Active=true;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ServerSocketClientConnect(TObject *Sender, TCustomWinSocket *Socket)
{
	wstringstream msg;

	msg << L"Клиент " << (ServerSocket->Socket->Connections[0]->RemoteAddress).c_str() << L" запрашивает подключение";
	int MB = Application->MessageBoxW(msg.str().c_str(), L"Warning", MB_OKCANCEL);
	if(MB != IDOK)
	{
	   ServerSocket->Socket->Connections[0]->Close();
	   return;
	}
	OutputDebugStringA("Client is connected to this server");
}
//---------------------------------------------------------------------------



void __fastcall TMainForm::ClientSocketDisconnect(TObject *Sender, TCustomWinSocket *Socket)
{
	OutputDebugStringA("You are disconnected");
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ClientSocketConnect(TObject *Sender, TCustomWinSocket *Socket)
{
	OutputDebugStringA("You are connected to server");
}
//---------------------------------------------------------------------------



void __fastcall TMainForm::ClientSocketRead(TObject *Sender, TCustomWinSocket *Socket)
{

	BYTE *buffer;

//  Rtext = ClientSocket->Socket->ReceiveText() ;
	int nBytesRead =0;
	nBytesRead = ClientSocket->Socket->ReceiveLength();
	buffer = new BYTE[nBytesRead] ;
	ClientSocket->Socket->ReceiveBuf(buffer, nBytesRead );
	AnsiString Rtext = (char*)buffer;



	if(Rtext.SubString( 0,Rtext.Pos("#")-1) == "file" )
	{
		Rtext.Delete( 1 , Rtext.Pos( "#" ) ) ;
		AnsiString Name = Rtext.SubString( 0 , Rtext.Pos( "#" ) -1 );
		AnsiString FileName = Name.SubString( Name.LastDelimiter( "\\" ) + 1 , Name.Length() );
		Rtext.Delete( 1 , Rtext.Pos( "#" ) );
		FileSize = StrToInt( Rtext.SubString( 0 , Rtext.Pos( "#" ) - 1) ) ;
		Rtext.Delete( 1 , Rtext.Pos( "#" ) );

		wstringstream msg;
		msg <<L"Принять от " << (ClientSocket->Socket->RemoteAddress).c_str() << L" файл " << FileName <<" ? ";
		int MB = Application->MessageBoxW(msg.str().c_str(), L"Warning", MB_OKCANCEL);
		if(MB != IDOK)
		{
			ClientSocket->Socket->SendText( "CANCELfromclient#" );
			return;
		}
		SaveExKeyDialog->FileName = FileName;
		if(SaveExKeyDialog->Execute())
		{

			File = new TFileStream( SaveExKeyDialog->FileName, fmCreate | fmOpenReadWrite );//
			delete(buffer);
		}
		ClientSocket->Socket->SendText( "OKfromclient#" );
		return;

	}
	else
	{
		File->Write(buffer,nBytesRead);
		if(File->Size >=FileSize)
		{
			File->Free();
        }
	}




}
//---------------------------------------------------------------------------


void __fastcall TMainForm::ServerSocketClientRead(TObject *Sender, TCustomWinSocket *Socket)
{
	BYTE *buffer;
	int nBytesRead =0;
	nBytesRead = ServerSocket->Socket->Connections[0]->ReceiveLength();
	buffer = new BYTE[nBytesRead] ;
	ServerSocket->Socket->Connections[0]->ReceiveBuf(buffer, nBytesRead );
	AnsiString Rtext = (char*)buffer;

	if(Rtext.SubString( 0,Rtext.Pos("#")-1) == "OKfromclient" )
	{
		ServerSocket->Socket->Connections[0]->SendStream(File);
	}

}
//---------------------------------------------------------------------------

