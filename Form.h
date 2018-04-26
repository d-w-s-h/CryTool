//---------------------------------------------------------------------------

#ifndef FormH
#define FormH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Dialogs.hpp>

#include <stdio.h>
#ifdef _WIN32
#   include <windows.h>
#   include <wincrypt.h>
#else
#   include <string.h>
#   include <stdlib.h>
#   include <CSP_WinDef.h>
#   include <CSP_WinCrypt.h>
#endif
#include "WinCryptEx.h"

#include "myCryptoClass.h"
//---------------------------------------------------------------------------
class TMainForm : public TForm
{
__published:	// IDE-managed Components
	TButton *LoadKeyButton;
	TButton *GenerateKeyButton;
	TButton *ExportKeyButton;
	TEdit *Edit1;
	TEdit *Edit2;
	TEdit *Edit3;
	TButton *EncryptFileButton;
	TButton *DecryptFileButton;
	TButton *CreateContainerButton;
	TButton *LoadContainerButton;
	TButton *DeleteContainerButton;
	TMemo *Memo1;
	TOpenDialog *OpenDialog1;
	TSaveDialog *SaveDialog1;
	TEdit *UsernameEdit;
	void __fastcall CreateContainerButtonClick(TObject *Sender);
	void __fastcall LoadContainerButtonClick(TObject *Sender);
	void __fastcall DeleteContainerButtonClick(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:	// User declarations
public:		// User declarations
	__fastcall TMainForm(TComponent* Owner);
	myCryptoClass *CSP;


};
//---------------------------------------------------------------------------
extern PACKAGE TMainForm *MainForm;
//---------------------------------------------------------------------------
#endif
