//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "Praktika.h"
#include "Unit5.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm5 *Form5;
//extern int npr,nug; extern double dpr,dug;
//---------------------------------------------------------------------------
__fastcall TForm5::TForm5(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TForm5::Button2Click(TObject *Sender)
{ //это выход без записи из Form5(спрямление)
 //ShowMessage(" Без записи данных расчёт невозможен!");
  if(MessageDlg("Продолжить работу?",
   mtConfirmation, TMsgDlgButtons() << mbYes<< mbNo,0) == mrYes)
 {
 ModalResult=mrOk;
 }
else
  {
ModalResult=3;
 //Если оставить 0 в ModalResut кнопки то форму не закрыть
  }
 }
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------



//---------------------------------------------------------------------------

