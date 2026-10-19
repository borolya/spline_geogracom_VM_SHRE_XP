//---------------------------------------------------------------------------

#include <vcl.h>
#include <stdio.h>
#pragma hdrstop

#include "UFTracks.h"
#include "pppUnit.h"
#include "UnFManFiles.h"
#include "UFManTracks.h"
    //#include "UFDelTrack.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
extern AnsiString trackfilename;
extern TfTrack *fTrack;
TMainForm *MainForm;
extern char pereg[30];
//---------------------------------------------------------------------------


 __fastcall TMainForm::TMainForm(TComponent* Owner)
        : TForm(Owner){};
//Этот конструктор можно включить в класс
     void spisok(char*s)
     {//даёт список кривых в FManFiles->ListBox1
     // Формирует список файлов *.crv

   char curpath[ 128 ];
  GetCurrentDirectory(128, curpath);
  _WIN32_FIND_DATA fattr;
//   HANDLE hndl = FindFirstFile( strcat( curpath, "\\*.crv"  ), &fattr);
   HANDLE hndl = FindFirstFile( strcat( curpath, s  ), &fattr);
   AnsiString fname = AnsiString(fattr.cFileName);
   FManFiles->ListBox1->Items->Clear();
   FManFiles->ListBox1->Items->Add( fname.SubString(1,fname.Length()-4) );
   while( FindNextFile(hndl, &fattr)  )
  {
   fname = AnsiString(fattr.cFileName);
   FManFiles->ListBox1->Items->Add( fname.SubString(1,fname.Length()-4) );
  }

  FManFiles->ListBox1->ItemIndex = 0;

      }//такой же список есть в UnFiles.cpp


//---------------------------------------------------------------------------
void __fastcall TMainForm::Button1Click(TObject *Sender)
{//ввод кривых показывает список кривых

       spisok("\\*.crv");
   FManFiles->ListBox1->Items->Insert(0,"------НОВАЯ------");
    FManFiles->UDAL->Visible=0;
   // FManFiles->Button4->Visible=0; //добавить
    FManFiles->Tag=0;
     FManFiles->ListBox1->ItemIndex = 0;
     FManFiles->Caption="Выбор кривой";
     FManFiles->Label2->Caption=" Имена кривых";
      //FManFiles->ListBox1->ItemIndex = 0;//dobavil есть выше
    FManFiles->Show();

}
//---------------------------------------------------------------------------


void __fastcall TMainForm::Button4Click(TObject *Sender)
{
//  удаление кривой на форме FManFiles кнопкой UDAL
     spisok("\\*.crv");
    FManFiles->Tag=1;//признак ввода 1-удаление 2- включить кривую
    FManFiles->UDAL->Visible=1;
    FManFiles->Caption="Удаление кривых";
    FManFiles->Label2->Caption=" Имена кривых";
    FManFiles->Show();

}
//---------------------------------------------------------------------------
  void __fastcall TMainForm::Button2Click(TObject *Sender)
{
   // ввод участка
      spisok("\\*.tra");
    FManFiles->ListBox1->Items->Insert(0,"-----НОВЫЙ-----");
    FManFiles->UDAL->Visible=0;
    FManFiles->Tag=3;
    FManFiles->ListBox1->ItemIndex = 0;
    FManFiles->Caption="Выбор участка";
    FManFiles->Label2->Caption=" Имена участков";
    fTrack->Panel3->Height = 0;//добавил при повторе надо убрать включить кривую
    FManFiles->Show();

}
//---------------------------------------------------------------------------

void __fastcall TMainForm::Button3Click(TObject *Sender)
{// удаление участка

    spisok("\\*.tra");
    FManFiles->Tag=4;
 //признак 0-ввод кривых 1-удаление 2- включить кривую 3-ввод участка 4-его удаление
    FManFiles->UDAL->Visible=1;
    FManFiles->Caption="Удаление участков";
    FManFiles->Label2->Caption=" Имена участков";
    FManFiles->Show();

}
//---------------------------------------------------------------------------

void __fastcall TMainForm::Button5Click(TObject *Sender)
{
  MainForm->Close();
  MainForm->Release();

}
//---------------------------------------------------------------------------

void __fastcall TMainForm::FormCreate(TObject *Sender)
{ //ввод RP в i.cpp
    MainForm->Caption= MainForm->Caption +pereg;
}
//---------------------------------------------------------------------------

