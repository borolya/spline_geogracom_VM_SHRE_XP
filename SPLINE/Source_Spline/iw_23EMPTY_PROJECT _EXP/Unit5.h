//---------------------------------------------------------------------------

#ifndef Unit5H
#define Unit5H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Grids.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TfCorners : public TForm
{
__published:	// IDE-managed Components
        TStringGrid *sgCorners;
        TButton *Button1;
        TButton *Button2;
        TButton *Button3;
        TButton *Button4;
        TButton *Button5;
        void __fastcall FormCreate(TObject *Sender);
        //void __fastcall sgCornersGetEditText(TObject *Sender, int ACol,
         // int ARow, AnsiString &Value);
       // void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall Button2Click(TObject *Sender);
        void __fastcall Button3Click(TObject *Sender);
        void __fastcall Button4Click(TObject *Sender);
        void __fastcall Button5Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TfCorners(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfCorners *fCorners;
//---------------------------------------------------------------------------
#endif
