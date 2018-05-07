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
#include <Vcl.ComCtrls.hpp>
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
#include "processingThread.h"

#include "myCryptoClass.h"
//---------------------------------------------------------------------------
class TMainForm : public TForm
{
__published:	// IDE-managed Components
	TButton *LoadKeyButton;
	TButton *GenerateKeyButton;
	TButton *ExportKeyButton;
	TEdit *EnPasswordEdit;
	TEdit *PassConfirmEdit;
	TEdit *DePasswordEdit;
	TButton *EncryptFileButton;
	TButton *DecryptFileButton;
	TButton *CreateContainerButton;
	TButton *LoadContainerButton;
	TButton *DeleteContainerButton;
	TOpenDialog *OpenFileDialog;
	TSaveDialog *SaveExKeyDialog;
	TEdit *UsernameEdit;
	TLabel *Label1;
	TLabel *Label2;
	TButton *SessionExBtn;
	TButton *SessionLoadBtn;
	TCheckBox *UsingImportKeyCheck;
	TLabel *InfoLabel;
	void __fastcall CreateContainerButtonClick(TObject *Sender);
	void __fastcall LoadContainerButtonClick(TObject *Sender);
	void __fastcall DeleteContainerButtonClick(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall GenerateKeyButtonClick(TObject *Sender);
	void __fastcall ExportKeyButtonClick(TObject *Sender);
	void __fastcall EncryptFileButtonClick(TObject *Sender);
	void __fastcall DecryptFileButtonClick(TObject *Sender);
	void __fastcall SessionExBtnClick(TObject *Sender);
	void __fastcall LoadKeyButtonClick(TObject *Sender);
	void __fastcall SessionLoadBtnClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TMainForm(TComponent* Owner);
	myCryptoClass *CSP;
    bool usingImportKey;
    bool isPublicKeyLoaded;

};
//---------------------------------------------------------------------------
extern PACKAGE TMainForm *MainForm;
//---------------------------------------------------------------------------
#endif
