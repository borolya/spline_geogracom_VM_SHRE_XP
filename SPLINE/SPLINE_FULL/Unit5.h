//---------------------------------------------------------------------------

#ifndef Unit5H
#define Unit5H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Dialogs.hpp>
//---------------------------------------------------------------------------
class TForm5 : public TForm
{
__published:	// IDE-managed Components
        TButton *Button2;
        TButton *Button4;
        TButton *Button9;
        TOpenDialog *OpenDialog1;
        TGroupBox *GroupBox1;
//        void __fastcall Button1Click(TObject *Sender);
       //void __fastcall Button8Click(TObject *Sender);
        void __fastcall Button9Click(TObject *Sender);
        void __fastcall Button4Click(TObject *Sender);
        //void __fastcall Button5Click(TObject *Sender);
        //void __fastcall Button3Click(TObject *Sender);
        void __fastcall Button2Click(TObject *Sender);
      // void __fastcall GroupBox1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TForm5(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm5 *Form5;
//---------------------------------------------------------------------------
#endif
 