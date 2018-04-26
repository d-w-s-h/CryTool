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

	CSP = new myCryptoClass(PROV_GOST_2012_256);
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
