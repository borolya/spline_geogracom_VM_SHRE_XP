//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <fstream.h>
#include "Unit1.h"

#include "Unit3.h"

#include "Unit5.h"
#include "Unit6.h"

#include <dir.h>
#include <FileCtrl.hpp>
//#include <shlobj.h>

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
AnsiString Dir;

extern char* curdir;

//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------

void __fastcall TForm1::Button5Click(TObject *Sender)
 // закрытие формы (выход из программы)
{
 Form1->Close();

}
//---------------------------------------------------------------------------
/*
void __fastcall TForm1::Button1Click(TObject *Sender)
{
char buff[MAX_PATH];
char initial_dir[256] = "C:\\";

BROWSEINFO bi;
bi.hwndOwner = Form1->Handle;
bi.pidlRoot = NULL;
bi.pszDisplayName = "";
bi.lpszTitle = "Выберите папку";
bi.ulFlags = NULL;
bi.lpfn = SelectDirCallback;
bi.lParam = (LPARAM)initial_dir;

SHGetPathFromIDList(SHBrowseForFolder(&bi), buff);
ShowMessage(buff);
} */
//------------------------------------------
//---------------------------------------------------------------------------


void __fastcall TForm1::Button1Click(TObject *Sender)
{ //Выбор объекта
 ifstream fp;
 ofstream fp1;
 char a[20]="";
  int i=1,j=0;
//Проверка наличия RP1.lst ввод, если нужно  и запись файла
 // каждый .exe  его читает без проверки и высвечивания

    if(!FileExists("RP1.lst"))
  {
   fp1.open("RP1.lst");
     fp1<<0.0 <<" " << 0.0 <<" " << 0.0<<endl;
      fp1<< 30000.0 <<" "<< 0.0 <<" "<< 3000000.0;
    fp1.close();
  }//nofile

 //ShowMessage(Dir);  пусто
 //ShowMessage(Dir); //curdir недоступна в Dir имя проекта


 if (SelectDirectory(Dir, TSelectDirOpts() << sdAllowCreate << sdPerformCreate << sdPrompt,0))
 // вызов выбора рабочей папки.
{
//*************
//ShowMessage(Dir);//Dir-полный путь к папке данных выбранной только что

 /*
  Form6->Caption=" CООТВЕТСТВИЕ ПИКЕТОВ И КИЛОМЕТРОВ. Объект "
   + Dir;
   Form6->RP->ColCount=3;
   Form6->RP->Cells[0][0]=" Пикет ";
   Form6->RP->Cells[1][0]=" Плюс ";
   Form6->RP->Cells[2][0]=" Километраж ";

  if(FileExists("RP1.lst"))
  {
   fp.open("RP1.lst");
   if (!fp) {ShowMessage (" Не читается файл RP1.lst"); exit(0);}

 while (fp.eof()==0)
   {
     for(int k=0;k<Form6->RP->ColCount; k++)
      {
   fp>>a;
   if(fp.eof()){--i;break;}
   Form6->RP->Cells[k][i]=(AnsiString)a;
      }
   i++;
   }//while
   Form6->RP->RowCount=i;
     fp.close();
  } else// нет файла
  {

 Form6->RP->RowCount=3;
    for ( i=1; i<Form6->RP->RowCount;i++)
    for (j=0; j<Form6->RP->ColCount; j++)
          Form6->RP->Cells[j][i]="";
  }
  Form6->ShowModal();
  if (Form6->ModalResult == mrOk)
  {
     fp1.open("RP1.lst");
     if (!fp1){ShowMessage (" Не не открывается на запись файл RP1.lst"); exit(0);};
    j=Form6->RP->RowCount;
   for (i= Form6->RP->RowCount;i>0;i--)
   {
   for(int k=0;k<Form6->RP->ColCount; k++)
       if(Form6->RP->Cells[k][i-1]==""){j--; break;}
    }// i

// j-число непустых строк идя снизу
// внутри пустые поля и строки не ловит доделать
   for(i=1;i<j;i++)
    {
    for(int k=0;k<Form6->RP->ColCount-1; k++)
       fp1<<Form6->RP->Cells[k][i].c_str()<<" ";
      fp1<< Form6->RP->Cells[Form6->RP->ColCount-1][i].c_str()<<endl;
    }

  fp1.close();
}//mrOk
     */

//++++
//Form8->ShowModal();
Form5->ShowModal();
}//Рабочее меню заменил на план
 else MessageBox(NULL," Объект не выбран !","ОШИБКА",MB_OK);

 }

//---------------------------------------------------------------------------


void __fastcall TForm1::FormClose(TObject *Sender, TCloseAction &Action)
{
 Form3->Close(); //закрыте формы
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------


