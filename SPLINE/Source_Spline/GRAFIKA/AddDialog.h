//---------------------------------------------------------------------------

#ifndef AddDialogH
#define AddDialogH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TForm2 : public TForm
{
__published:	// IDE-managed Components
        TLabel *Label1;
        TComboBox *List;
        TButton *Ok;
        TButton *Cancel;
        //void __fastcall ListChange(TObject *Sender);
        void __fastcall OkClick(TObject *Sender);
        void __fastcall CancelClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
        friend struct region;
        __fastcall TForm2(TComponent* Owner);
        bool krivizna (region* r); //рассчет кривизны
};
//---------------------------------------------------------------------------
extern PACKAGE TForm2 *Form2;
//---------------------------------------------------------------------------
#endif
