//---------------------------------------------------------------------------

#ifndef USZDH
#define USZDH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class Tftype : public TForm
{
__published:	// IDE-managed Components
        TButton *Button1;
        TLabel *Label1;
        TEdit *Edit1;
        TLabel *Label2;
        TEdit *Edit2;

        void __fastcall Button1Click(TObject *Sender);
        //void __fastcall FormShow(TObject *Sender);
       // void __fastcall FormCreate(TObject *Sender);
private:	// User declarations
public:
        __fastcall Tftype(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE Tftype *ftype;


//---------------------------------------------------------------------------
#endif
