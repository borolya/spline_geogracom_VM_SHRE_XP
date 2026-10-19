//---------------------------------------------------------------------------

#ifndef Unit6H
#define Unit6H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Grids.hpp>
//---------------------------------------------------------------------------
class TfPSK : public TForm
{
__published:	// IDE-managed Components
        TStringGrid *sgPSK;
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall sgPSKSetEditText(TObject *Sender, int ACol,
          int ARow, const AnsiString Value);
        void __fastcall sgPSKSelectCell(TObject *Sender, int ACol,
          int ARow, bool &CanSelect);
private:	// User declarations
public:		// User declarations
        __fastcall TfPSK(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfPSK *fPSK;
//---------------------------------------------------------------------------
#endif
