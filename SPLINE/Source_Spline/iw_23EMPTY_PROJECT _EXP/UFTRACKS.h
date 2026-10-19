//---------------------------------------------------------------------------

#ifndef UFTracksH
#define UFTracksH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TfTrack : public TForm
{
__published:	// IDE-managed Components
        TPanel *Panel1;
        TButton *Button2;
        TPanel *Panel2;
        TStringGrid *sgTrack;
        TSpeedButton *sbAssemTrace;
        TPanel *Panel3;
        TGroupBox *GroupBox2;
        TLabel *Label3;
        TLabel *Label2;
        TEdit *Edit1;
        TEdit *Edit2;
        TButton *Button3;
        TButton *Button4;
        TGroupBox *GroupBox1;
        TButton *Button5;
        TButton *Button6;
        TButton *Button1;
        TButton *Button8;
        TButton *Button9;
        TLabel *Label1;
        TEdit *Edit3;
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall sbAssemTraceClick(TObject *Sender);
        void __fastcall Button3Click(TObject *Sender);
        void __fastcall Button4Click(TObject *Sender);
        void __fastcall Button5Click(TObject *Sender);
        void __fastcall Edit1Click(TObject *Sender);
        void __fastcall Edit2Click(TObject *Sender);
        void __fastcall Edit1Exit(TObject *Sender);
        void __fastcall Edit2Exit(TObject *Sender);
        void __fastcall Button6Click(TObject *Sender);
       // void __fastcall Button7Click(TObject *Sender);
        void __fastcall Button2Click(TObject *Sender);
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall Button8Click(TObject *Sender);
        void __fastcall Button9Click(TObject *Sender);
      //  void __fastcall Edit2Change(TObject *Sender);
//        void __fastcall sgTrackClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TfTrack(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfTrack *fTrack;
//---------------------------------------------------------------------------
#endif
