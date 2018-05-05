//---------------------------------------------------------------------------

#include <System.hpp>
#pragma hdrstop

#include "processingThread.h"
#include "Form.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------

//   Important: Methods and properties of objects in VCL can only be
//   used in a method called using Synchronize, for example:
//
//      Synchronize(&UpdateCaption);
//
//   where UpdateCaption could look like:
//
//      void __fastcall processingThread::UpdateCaption()
//      {
//        Form1->Caption = "Updated in a thread";
//      }
//---------------------------------------------------------------------------

__fastcall processingThread::processingThread(bool isEncrypt ,
											myCryptoClass *CSP,
											wstring password,
											wstring filepath,
											bool usingImportKey,
											bool CreateSuspended
											)
	: TThread(CreateSuspended)
{
	FreeOnTerminate = true;
	this->IsEncrypt = isEncrypt;
	this->CSP = CSP;
	this->Password = password;
	this->Filepath = filepath;
	this->UsingImportKey= usingImportKey;
//	Form->ProgressBar->Position = this->progress;
//	Form->ProgressBar->Max = this->FileSystem->getTotalClusters();

}
//---------------------------------------------------------------------------
void __fastcall processingThread::Execute()
{
	NameThreadForDebugging(System::String(L"processingThread"));
	//---- Place thread code here ----
	if (IsEncrypt)
	{
		if(CSP->Encrypt_File(this->Password.c_str(),this->Filepath.c_str(),UsingImportKey))
		{
			Synchronize(&EncryptInfoGood);
		}
		else
		{
			Synchronize(&EncryptInfoError);
		}
	}
	else
	{
		if (CSP->Decrypt_File(this->Password.c_str(),this->Filepath.c_str(),UsingImportKey))
		{
			Synchronize(&DecryptInfoGood);
		}
		else
		{
			Synchronize(&DecryptInfoError);
		}

	}
}
//---------------------------------------------------------------------------

void __fastcall processingThread::EncryptInfoGood()
{
	MainForm->InfoLabel->Caption = "Файл успешно зашифрован";
}

void __fastcall processingThread::EncryptInfoError()
{
	MainForm->InfoLabel->Caption = "Ошибка шифрования файла";
}

void __fastcall processingThread::DecryptInfoGood()
{
	MainForm->InfoLabel->Caption = "Файл успешно расшифрован";
}

void __fastcall processingThread::DecryptInfoError()
{
	MainForm->InfoLabel->Caption = "Ошибка расшифрования файла";
}
