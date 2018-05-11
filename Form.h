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
#include <System.Win.ScktComp.hpp>
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
#include "netClass.h"
//---------------------------------------------------------------------------
class TMainForm : public TForm
{
__published:	// IDE-managed Components
	TOpenDialog *OpenFileDialog;
	TSaveDialog *SaveExKeyDialog;
	TEdit *UsernameEdit;
	TLabel *InfoLabel;
	TClientSocket *ClientSocket;
	TServerSocket *ServerSocket;
	TButton *UpdateContainerButton;
	TPageControl *PageControl1;
	TTabSheet *TabSheet1;
	TTabSheet *TabSheet2;
	TButton *EncryptFileButton;
	TEdit *EnPasswordEdit;
	TEdit *PassConfirmEdit;
	TButton *SessionExBtn;
	TButton *SessionLoadBtn;
	TCheckBox *UsingImportKeyCheck;
	TEdit *DePasswordEdit;
	TButton *DecryptFileButton;
	TButton *ConnectButton;
	TEdit *IPEdit;
	TButton *SendButton;
	TProgressBar *DownloadProgressBar;
	TButton *ExportKeyButton;
	TButton *LoadKeyButton;
	TButton *DisconnectButton;
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall GenerateKeyButtonClick(TObject *Sender);
	void __fastcall ExportKeyButtonClick(TObject *Sender);
	void __fastcall EncryptFileButtonClick(TObject *Sender);
	void __fastcall DecryptFileButtonClick(TObject *Sender);
	void __fastcall SessionExBtnClick(TObject *Sender);
	void __fastcall LoadKeyButtonClick(TObject *Sender);
	void __fastcall SessionLoadBtnClick(TObject *Sender);
	void __fastcall SendButtonClick(TObject *Sender);
	void __fastcall ConnectButtonClick(TObject *Sender);
	void __fastcall ServerSocketClientConnect(TObject *Sender, TCustomWinSocket *Socket);
	void __fastcall ClientSocketDisconnect(TObject *Sender, TCustomWinSocket *Socket);
	void __fastcall ClientSocketConnect(TObject *Sender, TCustomWinSocket *Socket);
	void __fastcall ClientSocketRead(TObject *Sender, TCustomWinSocket *Socket);
	void __fastcall ServerSocketClientRead(TObject *Sender, TCustomWinSocket *Socket);
	void __fastcall UpdateContainerButtonClick(TObject *Sender);
	void __fastcall UsingImportKeyCheckClick(TObject *Sender);
	void __fastcall DisconnectButtonClick(TObject *Sender);
	void __fastcall ServerSocketClientDisconnect(TObject *Sender, TCustomWinSocket *Socket);
private:	// User declarations
public:		// User declarations
	__fastcall TMainForm(TComponent* Owner);
//	myCryptoClass *CSP;
	netClass *CSP;
    bool usingImportKey;
	bool isPublicKeyLoaded;
	TFileStream * File;
	__int64 FileSize;



};
//---------------------------------------------------------------------------
extern PACKAGE TMainForm *MainForm;
//---------------------------------------------------------------------------
#endif
