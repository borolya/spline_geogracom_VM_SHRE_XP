//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <string.h>
#include<process.h>
#include "Unit4.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm4 *Form4;

 int gr0,min0;
 double sec0;
 char sig;
 extern double xl,yl,ugl; // extern double xlpr,ylpr,uglpr; не нужны здесь
extern double CONV_ANGLE(char sig,int grad,int min,double sec);
extern double Asd(AnsiString s);
extern void CONV_GRAD( double ugl,char &sig,int& grad,int& min,double& sec);
//---------------------------------------------------------------------------
__fastcall TForm4::TForm4(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TForm4::Button1Click(TObject *Sender)
{
//«јѕ»—ј“№
if (Tag==1)//были исправлени€
  {
xl=Asd(X0->Text);
yl=Asd(Y0->Text);
char s[3];
strcpy(s,sign->Text.c_str());
if(s[0]=='-')sig=1; else sig=0;//проверить
gr0=StrToInt(gr->Text);
min0=StrToInt(min->Text);
sec0=StrToFloat(sec->Text);
  ugl =CONV_ANGLE(sig,gr0,min0,sec0);
   }
//  ugl =CONV_ANGLE(sig,gr0,min0,sec0);
ModalResult=mrOk;
//Form4->Close();
//
}
//---------------------------------------------------------------------------
void _fastcall TForm4::Button2Click(TObject *Sender)
{   ModalResult=mrNone;
Form4->Close();
}
//---------------------------------------------------------------------------

void __fastcall TForm4::X0Change(TObject *Sender)
{
 Form4->Tag=1;
 return;
}
//---------------------------------------------------------------------------

void __fastcall TForm4::Y0Change(TObject *Sender)
{
Form4->Tag=1;
 return;
}
//---------------------------------------------------------------------------

void __fastcall TForm4::grChange(TObject *Sender)
{
Form4->Tag=1;
 return;
}
//---------------------------------------------------------------------------

void __fastcall TForm4::minChange(TObject *Sender)
{
Form4->Tag=1;
 return;        
}
//---------------------------------------------------------------------------

void __fastcall TForm4::secChange(TObject *Sender)
{
Form4->Tag=1;
 return;        
}
//---------------------------------------------------------------------------

void __fastcall TForm4::signChange(TObject *Sender)
{
Form4->Tag=1;
 return;
}
//---------------------------------------------------------------------------

