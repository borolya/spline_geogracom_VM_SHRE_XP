//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "MainMenu.h"
#include "Praktika.h"
#include <stdio.h>
#include <math.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm4 *Form4;
extern double lmin,lmax,lkmin,lprmin,Rmin, Rmax;//по длинам клотоиды и окружности
extern double rast(double x1,double y1,double x2,double y2);
   //extern  void pech(void);
//---------------------------------------------------------------------------
__fastcall TForm4::TForm4(TComponent* Owner)
        : TForm(Owner)
{

}
//---------------------------------------------------------------------------
void __fastcall TForm4::Button1Click(TObject *Sender)
{ //выбор по углограмме на форме 4  УВыбор метода расчёта
//Form3->ShowModal();  //инструкция

         Form1->regime = 0;
        Form4->Close();
       //  Form3->ShowModal();
}
//--------------------------------------------------------------------

void __fastcall TForm4::Button2Click(TObject *Sender)
{
 ShowMessage ("Расчёт  по координатам");
     Form1->regime = 1;
     Form4->Close();
}
//---------------------------------------------------------------------------

void __fastcall TForm4::Button3Click(TObject *Sender)
{
// ShowMessage ("Автоматический расчёт."); //
  Form1->regime = 2;
  Form4->Close();
//Form4->Hide();
}
//---------------------------------------------------------------------------

void __fastcall TForm4::Button4Click(TObject *Sender)
{       //     выход
      Form1->regime = 3;
      Form4->Close();
      Form4->Hide();

}

//---------------------------------------------------------------------------

