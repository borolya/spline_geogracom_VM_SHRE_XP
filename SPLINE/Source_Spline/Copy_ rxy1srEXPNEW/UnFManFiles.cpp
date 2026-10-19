

//---------------------------------------------------------------------------
#include<system.hpp>
#include<iostream.h>
#include <vcl.h>
#include <stdio.h>
#include <string.h>
#pragma hdrstop

#include "UnFManFiles.h"


//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
extern int read_trace( char *file_name );
extern void trace_show(void);
extern TForm1 *Form1;
char uch_name [80],elt_name[80];
char set_uch=0,set_var=0;
char num_var[4];
TFManFiles *FManFiles;
int newvar;//==1 если новый вариант при выборе
AnsiString trackfilename,eltfilename;
//AnsiString asdfilename; не нужен



//---------------------------------------------------------------------------
__fastcall TFManFiles::TFManFiles(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TFManFiles::ListBox1DblClick(TObject *Sender)
{
    if(FManFiles->Tag==1)
    {
    //выбор участка
 trackfilename = FManFiles->ListBox1->Items->Strings[ ListBox1->ItemIndex ];
 strcpy(uch_name,trackfilename.c_str());
 //fTrack->Caption = "”часток: " + trackfilename;
 //asdfilename =trackfilename +".asd";
Form1-> uch->Caption="            "+trackfilename;// uch_name;
Form1-> uch->Visible=true;
// asdfilename=trackfilename + ".asd";
 trackfilename =trackfilename +".tra";
  set_uch=1;
 int j= read_trace( trackfilename.c_str());
 if(j)
 { if(j==-3)ShowMessage("”часток пуст ");else
    ShowMessage("”часток не прочитан ");
  }
   //---------------------
   }
  else
    {        int j;
 //выбор варианта
       if( ListBox1->ItemIndex == 0 )
 {       AnsiString str;
 newvar=1;  //новый вариант булем строить нач прибл
  str = InputBox("¬ведите номер варианта от 1 до 99:","","");
 strcpy(num_var,str.c_str());
  j =StrLen(num_var);
 if(j<2) str="0"+str;
  Form1-> var->Caption="            "+str;
   eltfilename=AnsiString(uch_name);
   eltfilename = eltfilename+".e"+str;

 }
  else
  {newvar=0;//не новый вариант
 Form1-> var->Caption="            "+
 FManFiles->ListBox1->Items->Strings[ ListBox1->ItemIndex ];
 //даЄт 01 или 99 или...
strcpy(num_var,FManFiles->ListBox1->Items->Strings[ ListBox1->ItemIndex].c_str());
 eltfilename=AnsiString(uch_name);
 eltfilename = eltfilename+".e"+FManFiles->ListBox1->Items->Strings[ ListBox1->ItemIndex ];
   }
 strcpy(elt_name,eltfilename.c_str());// строка с им€.е01

Form1-> var->Visible=true;
   set_var=1;

  }

    FManFiles->Close();


 }

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

void __fastcall TFManFiles::Button1Click(TObject *Sender)
{
   //FManFiles->
      Close();
}


//---------------------------------------------------------------------------

