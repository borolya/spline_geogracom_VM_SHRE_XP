//---------------------------------------------------------------------------

#include <vcl.h>
#include <stdio.h>
#pragma hdrstop
#include "Unit1.h"
#include "Unit3.h"
//#include "Unit4.h"
#include <FileCtrl.hpp>
#include <windows.h>
#include <process.h>
#include "myfun.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm3 *Form3;
FILE *imd;
char* curdir = new char[255];
//---------------------------------------------------------------------------
__fastcall TForm3::TForm3(TComponent* Owner)
        : TForm(Owner)
{
 }
//---------------------------------------------------------------------------
 //---------------------------------------------------------------------------
 void __fastcall TForm3::FormClick(TObject *Sender)
{
 Form1->Show();     //Главное меню
 Form3->Visible=false;            //Погасить заставку
}
//---------------------------------------------------------------------------
void __fastcall TForm3::FormDblClick(TObject *Sender)
{   // Form3-заставка
 Form1->Show();
 Form3->Visible=false;
}
//---------------------------------------------------------------------------
void __fastcall TForm3::FormCreate(TObject *Sender)
{ //  curdir-пока пустая
 //char cur[128];
 //GetCurrentDirectory(128,cur); вместо  current_directory
 current_directory(curdir); //посылает текущую в curdir
 ShowMessage(curdir);
 imd=fopen("imd.txt" ,"wt");
 fprintf(imd,"\n%s",curdir);
 fclose(imd);
}
//---------------------------------------------------------------------------

void __fastcall TForm3::FormShow(TObject *Sender)
{
//current_directory(curdir);  //текущая папка где ехе модуль
;
}
//---------------------------------------------------------------------------

void __fastcall TForm3::Button1Click(TObject *Sender)
{
char put[180];
 strcpy(put,"AkelPad.exe ");
 strcat(put, "INSTR_full.txt");
  ShowMessage(put);
 WinExec(put,SW_SHOW);
 return;
 }
//---------------------------------------------------------------------------

