//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <fstream.h>
#include "UnFManFiles.h"
#include "UI.h"
#include "Unit5.h"
#include "UFTracks.h"
#include "math.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfTrack *fTrack;
extern spisok(char*);
extern TFManFiles* FManFiles;
extern void trace_show(void);
extern double Asd(AnsiString );
int n_asd;
//---------------------------------------------------------------------------
__fastcall TfTrack::TfTrack(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfTrack::FormCreate(TObject *Sender)
{
 fTrack->Panel3->Height = 0;

 sgTrack->Cells[0][0] = "    Пикетаж";
 sgTrack->Cells[1][0] = "            X            ";
 sgTrack->Cells[2][0] = "            Y            ";
//sgTrack->Cells[3][0] = "    Сдвижка";
}
//---------------------------------------------------------------------------
void __fastcall TfTrack::sbAssemTraceClick(TObject *Sender)
{      //компоновка участка
/* if( sbAssemTrace->Down )
    fTrack->Panel3->Height = 130;
 else
    fTrack->Panel3->Height = 0;
    */
    fTrack->Panel3->Height = 130;
    //Нет работы с ->Down
    spisok("//*.crv");   //создаёт список кривых
    FManFiles->Tag=2;  //включить кривую при работе с формой выбора
    FManFiles->UDAL->Visible=0;
        FManFiles->Caption="Выбор кривой";
        FManFiles->Label2->Caption=" Имена кривых";//добавил
}
//---------------------------------------------------------------------------

void __fastcall TfTrack::Button3Click(TObject *Sender)
{       //очистка участка
 for(int i = 1; i < sgTrack->RowCount ; i ++)
  sgTrack->Rows[i]->Clear();
sgTrack->RowCount=2;
  if(trace_pts_first!=NULL)free_pts(trace_pts_first);
  n_pts=0;  //очистка
}
//---------------------------------------------------------------------------


void __fastcall TfTrack::Button4Click(TObject *Sender)
{
//Углограмма
FILE*fp;
   TRACE_PTS *trace_pts_curr;
  char* name1;
  name1=new char [80];
      strcpy(name1,uch_name);//uch_name=имя участка KD3
      strcat(name1,".igl");// углограмма а uch_name.ugl проект текущий

  fp = fopen ( name1, "wt" );
    fprintf (fp,"%5d", n_pts ) ;
for (trace_pts_curr=trace_pts_first;trace_pts_curr;
				    trace_pts_curr=trace_pts_curr->next)
 {  //запись пикетаж,хорда угол а файл.igl
 fprintf(fp,"\n%8ld%7.2lf%7.3lf%10.6lf",trace_pts_curr->ch.chain,trace_pts_curr->ch.plus,
             trace_pts_curr->dist,trace_pts_curr->angle);

 }
   fclose( fp ); //запись файла .igl (для углограммы)
   delete name1;
   name1=new char [80];
      strcpy(name1,uch_name);
      strcat(name1,".ini");
      // X,Y для показа в новой системе

   //+++
      double *X,*Y,*X1,*Y1;  int i=0;;
  FILE *ff; long pk;double pl;
  X= new double[n_pts];X1= new double[n_pts];
  Y= new double[n_pts];Y1= new double[n_pts];
  ff=fopen(name1,"wt"); //файл .ini
  fprintf (ff,"%5d", n_pts ) ;
 //  fprintf (ff,"\nugl=%15.8lf,xl=%18.8lf,yl=%18.8lf",ugl,xl,yl);
         i=0;
  for (trace_pts_curr=trace_pts_first;trace_pts_curr;
	     	    trace_pts_curr=trace_pts_curr->next)
 {
          X[i] = trace_pts_curr->ev_x;
          Y[i] = trace_pts_curr->ev_y;
  //         fprintf (ff,"\n%12.6lf %12.6lf",X[i],Y[i]);

 // fprintf(ff,"\n X[%d]=%9.4lf,Y[%d]=%9.4lf",i,X[i],i,Y[i]);
          i++;
 }

 // fprintf(ff,"\n-------------------------");
//   ugl=0.;//временно для  блокировки поворота, но с центровкой

   for(i=0;i<n_pts;i++)
    {X1[i]=(X[i]-xl)*cos(ugl)+(Y[i]-yl)*sin(ugl);
     Y1[i]=(Y[i]-yl)*cos(ugl)-(X[i]-xl)*sin(ugl);
    //fprintf(ff,"\n X1[%d]=%9.4lf,Y1[%d]=%9.4lf",i,X1[i],i,Y1[i]);
    fprintf (ff,"\n%12.6lf %12.6lf",X1[i],Y1[i]);
    }
   fclose(ff);
   delete []X; delete []X1;delete[]Y;delete []Y1;
   delete []name1;
 //+++

 char put[80];
 strcpy(put,"..//INI.exe ");  // без ..// тоже работает
 strcat(put, uch_name);
 // ShowMessage (put);
  //ini.exe OVCH1  а не полное как находит INI.exe???
 WinExec(put,SW_SHOW);

}

//---------------------------------------------------------------------------
 
void __fastcall TfTrack::Button5Click(TObject *Sender)
{ //Включить кривую
if(!FManFiles->ListBox1->Items->Count)
{                 //если нет кривых

ShowMessage(" Нет кривых, ввести кривые");
fTrack->Close();
return;
}
  FManFiles->Show();

return;
}
//---------------------------------------------------------------------------

void __fastcall TfTrack::Edit1Click(TObject *Sender)
{
//начальный пикет
sgTrack->TopRow=1;
return;
}
//---------------------------------------------------------------------------
void __fastcall TfTrack::Edit1Exit(TObject *Sender)
{        //    выход из  нач. пикетажа
//ShowMessage(Edit1->Text);
 /*
if(set_beg()!=0)
{ ShowMessage("Не установлено начало"); exit(0);
} else
 {
//   ShowMessage("Есть начало");
trace_show(); //из списка на форму sg

    fTrack->Show();
   }//else
//sgTrack->Refresh();}
*/
  return;

}


//---------------------------------------------------------------------------

void __fastcall TfTrack::Edit2Click(TObject *Sender)
{            //конечный пикет
  sgTrack->TopRow=sgTrack->RowCount-4;
  return;
}
//---------------------------------------------------------------------------


void __fastcall TfTrack::Edit2Exit(TObject *Sender)
{ /*
  if(set_end()!=0)
{ ShowMessage("Не установлен конечный пикетаж"); exit(0);
} else
 {

  // ShowMessage("Есть конец");
    fTrack->Show();
          }//else
          */
return;
}
//---------------------------------------------------------------------------

void __fastcall TfTrack::Button6Click(TObject *Sender)
{ //записать  участка
  double s;
  if(MessageDlg("Записать участок ?",mtWarning,TMsgDlgButtons() << mbYes<<mbNo,0)==mrYes)
 {
    s=Asd(fTrack->Edit3->Text); //max sdvig
     curr_pts=trace_pts_first;
 for (curr_pts=trace_pts_first;curr_pts!=NULL;curr_pts=curr_pts->next)
 { curr_pts->min_shift=-s;curr_pts->max_shift=s; }//for
  //коридор записан
   //evolvents(trace_pts_first);
 if(write_trace(trackfilename.c_str()))
 ShowMessage("Участок не записан");
 //free_pts(trace_pts_first);// список трассы
 fTrack->Panel3->Height = 0;
 //Панель: всё кроме верхней строки и StringGrid
  TfTrack::Button4Click(Sender); //TObject *Sender  запись углограммы
 } //записать
// fTrack->Close();
}
//---------------------------------------------------------------------------
   /*
void __fastcall TfTrack::Button7Click(TObject *Sender)
{
//Коридор
fCorners->Tag=1;
fCorners->Caption=" Коридор сдвижек";
fCorners->sgCorners->Cells[2][0]=" Max. вправо";
fCorners->sgCorners->Cells[3][0]=" Max. влево";
ifstream fp;

fp.open(asdfilename.c_str());
if(!fp)
  {
fCorners->sgCorners->RowCount=2;
fCorners->sgCorners->Rows[1]->Clear();
fp.close();
  }else
  {
if(  read_inp_asd(asdfilename.c_str(),0))
 ShowMessage("Коридор не прочитан");;
  }
fCorners->ShowModal();
}//button7 коридоря
*/
//---------------------------------------------------------------------------

void __fastcall TfTrack::Button2Click(TObject *Sender)
{//Copy
int i;
AnsiString trackfilename1 = InputBox("Введите имя нового участка:","","");
AnsiString asdfilename1=trackfilename1+".asd";
trackfilename1=trackfilename1+".tra";
CopyFile( trackfilename.c_str(), trackfilename1.c_str(),i);
CopyFile( asdfilename.c_str(), asdfilename1.c_str(),i);
}
//---------------------------------------------------------------------------

void __fastcall TfTrack::Button1Click(TObject *Sender)
{  //возврат
 free_pts(trace_pts_first);
   fTrack->Edit1->Text="   +   ";
   fTrack->Edit2->Text="   +   ";
   fTrack->Close();
}
//---------------------------------------------------------------------------



//---------------------------------------------------------------------------

void __fastcall TfTrack::Button8Click(TObject *Sender)
{ //Записать пикетаж;
if(set_beg()!=0)
{ ShowMessage("Не установлен начальный пикетаж");return;
}
//sgTrack->Refresh();}

//////////////////////////////////
if(set_end()!=0)
{ ShowMessage("Не установлен конечный пикетаж"); return;
} else
 {

   ShowMessage("Пикетаж крайних точек записан.");
 // GroupBox2->Hide();
   ShowMessage("Проверить макс. сдвижку и записать участок");
  // GroupBox2->Visible=false;
   trace_show(); //из списка на форму sg UnFmanRFiles.cpp(67)
   fTrack->Show();  }//else
return;
}
//---------------------------------------------------------------------------

void __fastcall TfTrack::Button9Click(TObject *Sender)
{
Close();
// возврат из пикетажа кнопки возврат

}
//---------------------------------------------------------------------------

