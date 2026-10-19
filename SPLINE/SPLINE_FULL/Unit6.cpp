//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "Unit1.h"
#include "Unit6.h"
#include "myfun.h";
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm6 *Form6;
//---------------------------------------------------------------------------
__fastcall TForm6::TForm6(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------


void __fastcall TForm6::BitBtn1Click(TObject *Sender)
{
//записать таблицу

//ModalResult=mrOk;  как свойство кнопки уже есть!
// Form6->Close();
}
//---------------------------------------------------------------------------

void __fastcall TForm6::BitBtn2Click(TObject *Sender)
{
 // не записывать таблицу
 Form6->Close();
}
//---------------------------------------------------------------------------

void __fastcall TForm6::Button1Click(TObject *Sender) // функция добавления строки
{
 RP->RowCount++;
 //AddString();
}
//---------------------------------------------------------------------------

void __fastcall TForm6::Button2Click(TObject *Sender) //функция вставки строки
{
 AnsiString tmp;
 RP->RowCount++;
 int i,j;
 for ( i =  RP->RowCount;i > RP->Row  ;i--)
  for (j=0; j<RP->ColCount;j++)
   RP->Cells[j][i] = RP->Cells[j][i-1];
 for (j=0;j<RP->ColCount;j++)
   RP->Cells[j][RP->Row]="";
}
//---------------------------------------------------------------------------
void __fastcall TForm6::Button3Click(TObject *Sender)
 //функция удаления строки
{
  int i,j;
  if (RP->Row==1 && RP->RowCount==2)
   {
    i=1;
    for (j=0;j<RP->ColCount;j++)
    RP->Cells[j][i]= "";
   }
  else
 {

  for (i=RP->Row;i<RP->RowCount-1;i++)
   for (j=0;j<RP->ColCount;j++)
    RP->Cells[j][i]= RP->Cells[j][i+1];
  RP->RowCount--;
 }
}
//---------------------------------------------------------------------------
void __fastcall TForm6::FormShow(TObject *Sender)
// при загрузке формы необходимо заполнить поля StringGrid
{	  	 // в зависимости от значения tabflag

                 Form6->Caption = "Соответствие пикетажа";
                 Label1->Caption = "Соответствие пикетажа";
                 RP->ColCount=3;
                 RP->Cells[0][0]="Пикетаж";
                 RP->Cells[1][0]="Плюс";
                 RP->Cells[2][0]="Километраж";

}

