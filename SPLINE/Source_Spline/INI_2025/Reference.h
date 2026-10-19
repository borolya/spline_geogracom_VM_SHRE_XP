//---------------------------------------------------------------------------

#ifndef ReferenceH
#define ReferenceH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TForm3 : public TForm
{
__published:	// IDE-managed Components
        TButton *btnClose;
        TLabel *Label1;
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall btnCloseClick(TObject *Sender);
        
      //  void __fastcall Label1Click(TObject *Sender);
        //void __fastcall FormShow(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TForm3(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm3 *Form3;
//---------------------------------------------------------------------------
#endif
