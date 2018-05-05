//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Form.h"



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
		InfoLabel->Caption = "Контейнер создан";
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
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::GenerateKeyButtonClick(TObject *Sender)
{
	if(CSP->CreateExchangeKey())
	{
		SessionExBtn->Enabled = true;
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
	SessionExBtn->Enabled = false;
}
//---------------------------------------------------------------------------



void __fastcall TMainForm::SessionExBtnClick(TObject *Sender)
{
	if(SaveExKeyDialog->Execute())
	{
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
		CSP->LoadSessionKey(OpenFileDialog->FileName.c_str());
		InfoLabel->Caption = "Сессионный ключ успешно импортирован";
	}
	else
	{
        InfoLabel->Caption = "Ошибка импорта сессионного ключа";
	}
}
//---------------------------------------------------------------------------





