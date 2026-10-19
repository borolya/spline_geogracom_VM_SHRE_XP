//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include "UnFManFiles.h"
#include <Grids.hpp>
  extern int spisok(char*s);
//extern  TFManFiles *FManFiles;
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
        TPanel *Panel1;
        TLabel *Label1;
        TLabel *Label2;
        TLabel *Label3;
        TPanel *cop;
        TPanel *udal;
        TPanel *fin;
        TPanel *ras;
        TGroupBox *Gr2;
        TButton *Button1;
        TButton *Button2;
        TButton *Button3;
        TButton *Button4;
        TButton *Button5;
        TButton *Button6;
        TButton *Button7;
        TButton *Button8;
        TPanel *vibor;
        TStringGrid *sgTrack;
        TPanel *Panel2;
        TLabel *peregon;
        TLabel *var;
        TLabel *uch;
        TRadioGroup *RG;
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall rasClick(TObject *Sender);
        void __fastcall udalClick(TObject *Sender);
        void __fastcall finClick(TObject *Sender);
        void __fastcall copClick(TObject *Sender);
        void __fastcall Button8Click(TObject *Sender);
        void __fastcall viborClick(TObject *Sender);
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall Button3Click(TObject *Sender);
        void __fastcall Button2Click(TObject *Sender);
        void __fastcall sgTrackClick(TObject *Sender);
        void __fastcall sgTrackDrawCell(TObject *Sender, int ACol,
          int ARow, TRect &Rect, TGridDrawState State);
        void __fastcall varClick(TObject *Sender);
        void __fastcall Button4Click(TObject *Sender);
        void __fastcall Button5Click(TObject *Sender);
        void __fastcall Button6Click(TObject *Sender);
        void __fastcall Button7Click(TObject *Sender);
        void __fastcall RGClick(TObject *Sender);

   //     void __fastcall sgTrackExit(TObject *Sender);


private:	// User declarations
public:		// User declarations
        __fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
