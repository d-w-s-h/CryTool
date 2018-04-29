//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Form.h"


//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TMainForm *MainForm;


//---------------------------------------------------------------------------
__fastcall TMainForm::TMainForm(TComponent* Owner)
	: TForm(Owner)
{

	CSP = new myCryptoClass( PROV_GOST_2001_DH);
	MainForm->UsernameEdit->Text = (UnicodeString) CSP->GetUserName().c_str();
	bool CreateContainer();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::CreateContainerButtonClick(TObject *Sender)
{

	CSP->CreateContainer(MainForm->UsernameEdit->Text.c_str());
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::LoadContainerButtonClick(TObject *Sender)
{
    CSP->LoadContainer(MainForm->UsernameEdit->Text.c_str());
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::DeleteContainerButtonClick(TObject *Sender)
{
    CSP->DeleteContainer(MainForm->UsernameEdit->Text.c_str());
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::FormClose(TObject *Sender, TCloseAction &Action)
{
	CSP->CleanUp();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::GenerateKeyButtonClick(TObject *Sender)
{
	CSP->CreateExchangeKey();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ExportKeyButtonClick(TObject *Sender)
{

	if(SaveExKeyDialog->Execute())
	{
		CSP->ExportExchangeKey(SaveExKeyDialog->FileName.c_str());
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

	if(OpenFileDialog->Execute())
	{
		CSP->Encrypt_File(EnPasswordEdit->Text.c_str(),OpenFileDialog->FileName.c_str());
	}



}
//---------------------------------------------------------------------------

