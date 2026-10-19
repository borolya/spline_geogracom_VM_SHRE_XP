//---------------------------------------------------------------------------

#ifndef Unit5H
#define Unit5H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TForm5 : public TForm
{
__published:	// IDE-managed Components
        TEdit *Edit1;
        TEdit *Edit2;
        TEdit *Edit3;
        TEdit *Edit4;
        TEdit *Edit5;
        TEdit *Edit6;
        TEdit *Edit7;
        TLabel *Label1;
        TLabel *Label2;
        TLabel *Label3;
        TLabel *Label4;
        TLabel *Label5;
        TLabel *Label6;
        TLabel *Label7;
        TButton *Button1;
        TLabel *Label8;
        TLabel *Label9;
        TButton *Button2;

        void __fastcall Button1Click(TObject *Sender);

        void __fastcall Button2Click(TObject *Sender);
        void __fastcall FormCreate(TObject *Sender);

private:	// User declarations
public:		// User declarations
        __fastcall TForm5(TComponent* Owner);
  //      struct param data;
    //    struct param data_wrk;
};

//---------------------------------------------------------------------------
extern PACKAGE TForm5 *Form5;
//---------------------------------------------------------------------------
#endif
 