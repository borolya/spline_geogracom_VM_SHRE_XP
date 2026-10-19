//---------------------------------------------------------------------------

#ifndef pppUnitH
#define pppUnitH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Menus.hpp>
#include <Grids.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include <ToolWin.hpp>
//---------------------------------------------------------------------------
class TMainForm : public TForm
{
__published:	// IDE-managed Components
        TGroupBox *GroupBox1;
        TButton *Button1;
        TButton *Button2;
        TButton *Button4;
        TButton *Button3;
        TButton *Button5;
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall Button4Click(TObject *Sender);
        void __fastcall Button2Click(TObject *Sender);
        void __fastcall Button3Click(TObject *Sender);
        void __fastcall Button5Click(TObject *Sender);
        void __fastcall FormCreate(TObject *Sender);
private:	// User declarations
public:		// User declarations
       __fastcall TMainForm(TComponent* Owner);
   //__fastcall TMainForm(TComponent* Owner): TForm(Owner){};
};
//---------------------------------------------------------------------------
extern PACKAGE TMainForm *MainForm;

//---------------------------------------------------------------------------
#endif
