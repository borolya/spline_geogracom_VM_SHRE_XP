//---------------------------------------------------------------------------

#ifndef UnFManFilesH
#define UnFManFilesH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>

#include "UI.h"
#include "USZD.h"

//#include "Unit3.h" по кривым
//#include "Unit4.h"
#include "Unit5.h"
#include <ComCtrls.hpp>
//#include "Unit6.h"

//---------------------------------------------------------------------------
class TFManFiles : public TForm
{
__published:	// IDE-managed Components
        TListBox *ListBox1;
        TButton *UDAL;
        TButton *Button1;
        TLabel *Label2;
        void __fastcall ListBox1DblClick(TObject *Sender);
        void __fastcall UDALClick(TObject *Sender);
        void __fastcall Button1Click(TObject *Sender);
        //void __fastcall Button3Click(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        //void __fastcall Button2Click(TObject *Sender);
        //void __fastcall Button4Click(TObject *Sender);
      //  void __fastcall FormCreate(TObject *Sender);
       // void __fastcall Label2Click(TObject *Sender);
       //void __fastcall Edit1Change(TObject *Sender);


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
