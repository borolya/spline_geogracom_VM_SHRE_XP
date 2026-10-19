//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "MainMenu.h"
#include "Praktika.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm4 *Form4;

//---------------------------------------------------------------------------
__fastcall TForm4::TForm4(TComponent* Owner)
        : TForm(Owner)
{

}
//---------------------------------------------------------------------------
/*
void __fastcall TForm4::Button1Click(TObject *Sender)
{
      //автоматически

        Form1->regime = 0;
        Form4->Close();

} */
//--------------------------------------------------------------------

void __fastcall TForm4::Button2Click(TObject *Sender)
{           //старый вариант
       Form1->regime = 1;
        Form4->Close();

}
//---------------------------------------------------------------------------

void __fastcall TForm4::Button3Click(TObject *Sender)
{         // посмотреть
  Form1->regime = 2;
  Form4->Close();

}
//---------------------------------------------------------------------------

void __fastcall TForm4::Button4Click(TObject *Sender)
{       //     выход
      Form1->regime = 3;
      Form4->Close();


}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------

