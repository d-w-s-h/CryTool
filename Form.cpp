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

//	CSP = new myCryptoClass( PROV_GOST_2001_DH);
	CSP = new netClass(PROV_GOST_2001_DH);
	MainForm->UsernameEdit->Text = (UnicodeString) CSP->GetmyUserName().c_str();
//	bool CreateContainer();
	CSP->CreateContainer(MainForm->UsernameEdit->Text.c_str());

	EncryptFileButton->Enabled =false;
	DecryptFileButton->Enabled =false;
	ExportKeyButton->Enabled =false;
	LoadKeyButton->Enabled =false;
	SessionExBtn->Enabled = false;
	SessionLoadBtn->Enabled = false;
	DisconnectButton->Enabled = false;


	this->usingImportKey=false;
	this->isPublicKeyLoaded=false;

	DownloadProgressBar->Visible=false;

    if(CSP->LoadContainer(MainForm->UsernameEdit->Text.c_str()))
	{
		EncryptFileButton->Enabled =true;
		DecryptFileButton->Enabled =true;
		ExportKeyButton->Enabled =true;

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
void __fastcall TMainForm::UpdateContainerButtonClick(TObject *Sender)
{
	CSP->CleanUp();
	if(CSP->DeleteContainer(MainForm->UsernameEdit->Text.c_str()))
	{
		InfoLabel->Caption = "Контейнер был успешно удален";
		this->isPublicKeyLoaded=false;
	}
	else
	{
		InfoLabel->Caption = "Контейнер не был удален (либо удален ранее)";
	}
	if(CSP->CreateContainer(MainForm->UsernameEdit->Text.c_str()))
	{
		wstringstream msg;
		msg << L"Контейнер " << CSP->GetmyUserName().c_str() <<  L" создан";
		InfoLabel->Caption = msg.str().c_str();
	}
	else
	{
		InfoLabel->Caption = "Ошибка создания конейнера (либо контейнер был создан ранее - попробуйте загрузить его)";
	}
	if(CSP->CreateExchangeKey())
	{
//		SessionExBtn->Enabled = true;
		InfoLabel->Caption = "Пара ключей сгенерирована";
	}
	else
	{
		InfoLabel->Caption = "Ошибка генерации ключей (либо ключи уже был сгенерированы ранее)";
	}
	if(ClientSocket->Socket->Connected)
	{
		ClientSocket->Socket->Close();
	}
	else if(ServerSocket->Socket->ActiveConnections != 0)
	{
		ServerSocket->Socket->Connections[0]->Close();
	}



}
//---------------------------------------------------------------------------
void __fastcall TMainForm::UsingImportKeyCheckClick(TObject *Sender)
{
	if(UsingImportKeyCheck->Checked)
	{
		EnPasswordEdit->Enabled=false;
		DePasswordEdit->Enabled=false;
		PassConfirmEdit->Enabled=false;
	}
	else
	{
		EnPasswordEdit->Enabled=true;
		DePasswordEdit->Enabled=true;
		PassConfirmEdit->Enabled=true;
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



	if(ClientSocket->Socket->Connected)
	{   if(OpenFileDialog->Execute())
		{
			File = new TFileStream( OpenFileDialog->FileName, fmOpenRead | fmShareDenyNone );
			ClientSocket->Socket->SendText( "file#" + OpenFileDialog->FileName + "#" + IntToStr( File->Size ) + "#" );
		}
	}
	else if(ServerSocket->Socket->ActiveConnections != 0)
	{
		if(OpenFileDialog->Execute())
		{
			File = new TFileStream( OpenFileDialog->FileName, fmOpenRead | fmShareDenyNone );
			ServerSocket->Socket->Connections[0]->SendText( "file#" + OpenFileDialog->FileName + "#" + IntToStr( File->Size ) + "#" );
		}
	}
	else
	{
		Application->MessageBoxW(L"Нет активных соединений",L"Ошибка", MB_OK | MB_ICONWARNING);
	}





}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ConnectButtonClick(TObject *Sender)
{
	ClientSocket->Address = IPEdit->Text;
	ClientSocket->Active=true;
	ConnectButton->Enabled = false;
	DisconnectButton->Enabled = true;
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
	CSP->NetExportPublicKey(ServerSocket->Socket->Connections[0]);
	ConnectButton->Enabled = false;
	DisconnectButton->Enabled = true;


}
//---------------------------------------------------------------------------



void __fastcall TMainForm::ClientSocketDisconnect(TObject *Sender, TCustomWinSocket *Socket)
{
	OutputDebugStringA("You are disconnected");
	InfoLabel->Caption = "Соединение разорвано.";
	ConnectButton->Enabled = true;
	DisconnectButton->Enabled = false;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ServerSocketClientDisconnect(TObject *Sender, TCustomWinSocket *Socket)
{
	OutputDebugStringA("You are disconnected");
	InfoLabel->Caption = "Соединение разорвано.";
	ConnectButton->Enabled = true;
	DisconnectButton->Enabled = false;
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

		}
		ClientSocket->Socket->SendText( "OKfromclient#" );
		DownloadProgressBar->Visible=true;
		DownloadProgressBar->Max=FileSize;


	}
	else if(Rtext.SubString( 0,Rtext.Pos("#")-1) == "OKfromSERVER" )
	{
		//ClientSocket->Socket->SendStream(File);
		CSP->NetSendEncryptFile(ClientSocket->Socket, File);
	}
	else if(Rtext.SubString( 0,Rtext.Pos("#")-1) == "PUBLICKEY" )
	{
		if(CSP->NetImportPublicKey(ClientSocket->Socket, buffer, nBytesRead))
		{
			InfoLabel->Caption = "Публичный ключ сервера импортирован...";
			CSP->NetExportPublicKey(ClientSocket->Socket);
		}

	}
	else if(Rtext.SubString( 0,Rtext.Pos("#")-1) == "SESSIONKEY" )
	{
		if(CSP->NetImportSessionKey(ClientSocket->Socket, buffer, nBytesRead))
		{
			InfoLabel->Caption = "Установлено защищенное соединение.";
			ClientSocket->Socket->SendText("CLIENTSESSIONUP#");
        }
	}
//	else if(Rtext.SubString( 0,Rtext.Pos("#")-1) == "DownloadSuccess" )
//	{
//		InfoLabel->Caption = "Передача файла завершена";
//		CSP->ClearupAfterDownload(File);
//	}
	else
	{
		File->Write(buffer,nBytesRead);
		DownloadProgressBar->Position= File->Size ;
		if(File->Size >= FileSize)
		{
			CSP->NetRecieveEncryptFile(ClientSocket->Socket, File);
//			File->Free();
			DownloadProgressBar->Visible=false;
			DownloadProgressBar->Position=0;
//			ServerSocket->Socket->Connections[0]->SendText( "DownloadSuccess#" );
		}
	}

	delete(buffer);


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
	if(Rtext.SubString( 0,Rtext.Pos("#")-1) == "file" )
	{
		Rtext.Delete( 1 , Rtext.Pos( "#" ) ) ;
		AnsiString Name = Rtext.SubString( 0 , Rtext.Pos( "#" ) -1 );
		AnsiString FileName = Name.SubString( Name.LastDelimiter( "\\" ) + 1 , Name.Length() );
		Rtext.Delete( 1 , Rtext.Pos( "#" ) );
		FileSize = StrToInt( Rtext.SubString( 0 , Rtext.Pos( "#" ) - 1) ) ;
		Rtext.Delete( 1 , Rtext.Pos( "#" ) );

		wstringstream msg;
		msg <<L"Принять от " << (ServerSocket->Socket->Connections[0]->RemoteAddress).c_str() << L" файл " << FileName <<" ? ";
		int MB = Application->MessageBoxW(msg.str().c_str(), L"Warning", MB_OKCANCEL);
		if(MB != IDOK)
		{
			ServerSocket->Socket->Connections[0]->SendText( "CANCELfromSERVER#" );
			return;
		}
		SaveExKeyDialog->FileName = FileName;
		if(SaveExKeyDialog->Execute())
		{
			File = new TFileStream( SaveExKeyDialog->FileName, fmCreate | fmOpenReadWrite );//
		}
		ServerSocket->Socket->Connections[0]->SendText( "OKfromSERVER#" );

		DownloadProgressBar->Visible=true;
		DownloadProgressBar->Max=FileSize;

	}

	else if(Rtext.SubString( 0,Rtext.Pos("#")-1) == "OKfromclient" )
	{
		//ServerSocket->Socket->Connections[0]->SendStream(File);
		CSP->NetSendEncryptFile(ServerSocket->Socket->Connections[0], File);
	}

	else if(Rtext.SubString( 0,Rtext.Pos("#")-1) == "PUBLICKEY" )
	{
		if(CSP->NetImportPublicKey(ServerSocket->Socket->Connections[0], buffer, nBytesRead))
		{
			InfoLabel->Caption = "Публичный ключ клиента импортирован...";
			CSP->NetExportSessionKey(ServerSocket->Socket->Connections[0]);
        }
	}

	else if(Rtext.SubString( 0,Rtext.Pos("#")-1) == "CLIENTSESSIONUP" )
	{
		InfoLabel->Caption = "Установлено защищенное соединение";
	}
//	else if(Rtext.SubString( 0,Rtext.Pos("#")-1) == "DownloadSuccess" )
//	{
//		InfoLabel->Caption = "Передача файла завершена";
//		CSP->ClearupAfterDownload(File);
//	}
	else
	{
		File->Write(buffer,nBytesRead);
		DownloadProgressBar->Position= File->Size ;
		if(File->Size >=FileSize)
		{
			DownloadProgressBar->Visible=false;
			CSP->NetRecieveEncryptFile(ServerSocket->Socket->Connections[0], File);
//			File->Free();
//			ServerSocket->Socket->Connections[0]->SendText( "DownloadSuccess#" );

		}
	}
	delete(buffer);

}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------





void __fastcall TMainForm::DisconnectButtonClick(TObject *Sender)
{
	if(ClientSocket->Socket->Connected)
	{
		ClientSocket->Socket->Close();
	}
	else if(ServerSocket->Socket->ActiveConnections != 0)
	{
		ServerSocket->Socket->Connections[0]->Close();
	}
	else
	{
		Application->MessageBoxW(L"Нет активных соединений",L"Ошибка", MB_OK | MB_ICONWARNING);
	}
}
//---------------------------------------------------------------------------


void __fastcall TMainForm::ClientSocketError(TObject *Sender, TCustomWinSocket *Socket, TErrorEvent ErrorEvent,
          int &ErrorCode)
{
	Application->MessageBoxW(L"Заданный клиент недоступен.",L"Ошибка", MB_OK | MB_ICONWARNING);
	ClientSocket->Socket->Close();
	ErrorCode =0;
}
//---------------------------------------------------------------------------

