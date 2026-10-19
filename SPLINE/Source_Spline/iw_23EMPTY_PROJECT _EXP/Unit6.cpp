//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit6.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfPSK *fPSK;
//---------------------------------------------------------------------------
__fastcall TfPSK::TfPSK(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TfPSK::FormCreate(TObject *Sender)
{
 sgPSK->Cells[1][0] = "ѕикетаж";
 sgPSK->Cells[2][0] = "”гол";        
}
//---------------------------------------------------------------------------

void __fastcall TfPSK::Button1Click(TObject *Sender)
{
 tagPOINT lpPoint;
 GetCaretPos( &lpPoint );
 if(!SetCaretPos(2+3*4,2)) ShowMessage("false");
 Caption = AnsiString(lpPoint.x) + "   " + AnsiString(lpPoint.y);
}
//---------------------------------------------------------------------------
void __fastcall TfPSK::sgPSKSetEditText(TObject *Sender, int ACol,
      int ARow, const AnsiString Value)
{
  static int i = 0;
//
   tagPOINT lpPoint;
 GetCaretPos( &lpPoint );

 Caption = AnsiString(lpPoint.x) + "   " + AnsiString(lpPoint.y);
 i++;
}
//---------------------------------------------------------------------------
void __fastcall TfPSK::sgPSKSelectCell(TObject *Sender, int ACol, int ARow,
      bool &CanSelect)
{
 tagPOINT lpPoint;
 GetCaretPos( &lpPoint );
 //SetCaretPos(2+3*7,2);
  if(!SetCaretPos(2+3*4,2)) ShowMessage("false");
}
//---------------------------------------------------------------------------
