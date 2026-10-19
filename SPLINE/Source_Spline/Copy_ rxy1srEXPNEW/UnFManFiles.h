//---------------------------------------------------------------------------

#ifndef UnFManFilesH
#define UnFManFilesH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include "Unit1.h"

//---------------------------------------------------------------------------
class TFManFiles : public TForm
{
__published:	// IDE-managed Components
        TListBox *ListBox1;
        TLabel *Label2;
        TButton *Button1;
        void __fastcall ListBox1DblClick(TObject *Sender);
        //void __fastcall UDALClick(TObject *Sender);
        void __fastcall Button1Click(TObject *Sender);
//        void __fastcall ListBox1Click(TObject *Sender);
        //void __fastcall Label2Click(TObject *Sender);

private:
         // User declarations
public:		// User declarations
        __fastcall TFManFiles(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFManFiles *FManFiles;



//extern LINK line[100];



//---------------------------------------------------------------------------
#endif
