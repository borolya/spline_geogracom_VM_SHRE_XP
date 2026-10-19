//---------------------------------------------------------------------------

#ifndef Unit4H
#define Unit4H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Grids.hpp>
//---------------------------------------------------------------------------
class TfElems : public TForm
{
__published:	// IDE-managed Components
        TStringGrid *sgElems;
        TButton *Button1;
        TButton *Button2;
        TButton *Button3;
        TButton *Button4;
        TButton *Button5;
        TLabel *Label1;
        TLabel *Label2;
        TLabel *Label3;
        TLabel *Label4;
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall sgElemsClick(TObject *Sender);
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall Button2Click(TObject *Sender);
        void __fastcall Button3Click(TObject *Sender);
        void __fastcall Button4Click(TObject *Sender);
        void __fastcall Button5Click(TObject *Sender);
 //       void __fastcall sgElemsGetEditText(TObject *Sender, int ACol,
   //       int ARow, AnsiString &Value);
private:	// User declarations
public:		// User declarations
        __fastcall TfElems(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfElems *fElems;
//---------------------------------------------------------------------------
#endif
