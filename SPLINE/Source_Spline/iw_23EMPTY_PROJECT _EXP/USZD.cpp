//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UI.h"
#include "USZD.h"
#include "Unit5.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

Tftype *ftype;


//extern AnsiString fname; есть в UI.h
//---------------------------------------------------------------------------
__fastcall Tftype::Tftype(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall Tftype::Button1Click(TObject *Sender)
{
    char buf[80];
    int i=0;
    while (fname[i]!='.')
    {buf[i]=fname[i];buf[i+1]='\0'; i++;}
            // по координатам

      for(int i = 1; i < fCorners->sgCorners->RowCount ; i ++)
     fCorners->sgCorners->Rows[i]->Clear();
     fCorners->Caption ="Кривая " +AnsiString(buf);
//     fCorners->ShowModal(); //а она могла от старого и полна
  ftype->ModalResult=mrOk;
    // ftype->Close();
    return;

}

//---------------------------------------------------------------------------
