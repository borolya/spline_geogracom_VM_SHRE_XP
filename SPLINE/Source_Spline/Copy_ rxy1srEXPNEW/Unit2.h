//---------------------------------------------------------------------------

#ifndef Unit2H
#define Unit2H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Grids.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class Tel : public TForm
{
__published:	// IDE-managed Components
        TLabel *Label1;
        TEdit *nkriv;
        TLabel *nach;
        TEdit *Lmin;
        TLabel *Label2;
        TEdit *Lmax;
        TLabel *Lf;
        TEdit *Lfact;
        TEdit *Lend;
        TButton *dob;
        TButton *vst;
        TButton *udal;
        TButton *Button4;
        TPanel *Panel1;
        TPanel *Panel2;
        TPanel *Panel4;
        TPanel *Panel5;
        TPanel *Panel6;
        TPanel *Panel7;
        TPanel *Panel8;
        TPanel *Panel9;
        TStringGrid *sgEl;
        TPanel *Panel10;
        TPanel *Panel11;
        TPanel *Panel12;
        TPanel *Panel13;
        TPanel *Panel14;
        TPanel *Panel15;
        TLabel *Label3;
        TPanel *Panel3;
        TButton *Button1;
        TButton *MClose;
        TStaticText *dop;
        TEdit *numel;
        TEdit *pic;
        TEdit *pl;
        TPanel *kon;
        TButton *Button2;
        TButton *Button3;
        void __fastcall Button4Click(TObject *Sender);
        void __fastcall dobClick(TObject *Sender);
        void __fastcall udalClick(TObject *Sender);
        void __fastcall vstClick(TObject *Sender);
        void __fastcall sgElClick(TObject *Sender);
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall MCloseClick(TObject *Sender);
        void __fastcall sgElSetEditText(TObject *Sender, int ACol,
          int ARow, const AnsiString Value);

        void __fastcall Button2Click(TObject *Sender);
        void __fastcall Button3Click(TObject *Sender);
        void __fastcall LminChange(TObject *Sender);
        void __fastcall LmaxChange(TObject *Sender);
        void __fastcall LfactChange(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall Tel(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE Tel *el;
//---------------------------------------------------------------------------
#endif
