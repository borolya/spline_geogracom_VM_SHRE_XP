//---------------------------------------------------------------------------

#ifndef Unit4H
#define Unit4H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TForm4 : public TForm
{
__published:	// IDE-managed Components
        TPanel *Panel1;
        TStaticText *StaticText1;
        TStaticText *X0t;
        TStaticText *Y0t;
        TEdit *X0;
        TEdit *Y0;
        TPanel *Panel2;
        TStaticText *StaticText4;
        TStaticText *signt;
        TEdit *sign;
        TStaticText *grt;
        TStaticText *mint;
        TStaticText *sect;
        TEdit *min;
        TEdit *gr;
        TEdit *sec;
        TButton *Button1;
        TButton *Button2;
        TPanel *Panel3;
        TStaticText *StaticText2;
        TEdit *Ls;
        TStaticText *StaticText3;
        TStaticText *StaticText5;
        TStaticText *StaticText6;
        TStaticText *StaticText7;
        TEdit *dx;
        TStaticText *StaticText8;
        TEdit *dy;
        TEdit *Lp;
        TEdit *Lm;
        TEdit *dL;
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall Button2Click(TObject *Sender);
        void __fastcall X0Change(TObject *Sender);
        void __fastcall Y0Change(TObject *Sender);
        void __fastcall grChange(TObject *Sender);
        void __fastcall minChange(TObject *Sender);
        void __fastcall secChange(TObject *Sender);
        void __fastcall signChange(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TForm4(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm4 *Form4;
//---------------------------------------------------------------------------
#endif
