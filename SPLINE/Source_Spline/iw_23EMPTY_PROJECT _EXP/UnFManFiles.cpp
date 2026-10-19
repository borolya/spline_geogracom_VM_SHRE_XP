

//---------------------------------------------------------------------------
//исправлена ошибка (ItemIndex не увеличивался при добавлении)
//в программах ввода кривых с машинного носителя май 2020
#include <windows.h>
#define NO_WIN32_LEAN_AND_MEAN
#include <shlobj.h>
#include <process.h>
#include<system.hpp>
#include<iostream.h>
#include <vcl.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#pragma hdrstop
#include "UFTracks.h"
#include "UnFManFiles.h"
#include "pppUnit.h"
#include "UI.h"

#include <stdio.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
  TFManFiles *FManFiles; //перенёс снизу этого файла

char uch_name [80];
extern TfTrack *fTrack;
//TFManFiles *FManFiles; отнёс выше
AnsiString trackfilename;
AnsiString asdfilename;
int type_survey;
TRACE_PTS *curr_pts,*last_pts,*end_pts,*next_pts ;
//double ugl,uglt,ugp,ugpt,xl,xlt,yl,ylt,xp,xpt,yp,ypt;
double dist,shf_r=2.0,shf_l=-2.0;//ошибка в присваивании поэтому здесь наоборот
 extern void spisok(char*);
int include_curve_p( char * );
extern void trace_show(void);
 extern void freeCorners();

FILE *fp2,*fp1;
 AnsiString str;
 int i,j;
  /*
void spisok(char*s)
{

//даёт список кривых в FManFiles->ListBox1
     // Формирует список файлов *.crv
   char curpath[ 128 ];
  GetCurrentDirectory(128, curpath);
  _WIN32_FIND_DATA fattr;
//   HANDLE hndl = FindFirstFile( strcat( curpath, "\\*.crv"  ), &fattr);
   HANDLE hndl = FindFirstFile( strcat( curpath, s  ), &fattr);
   AnsiString fname = AnsiString(fattr.cFileName);
   FManFiles->ListBox1->Items->Clear();
   FManFiles->ListBox1->Items->Add( fname.SubString(1,fname.Length()-4) );
   while( FindNextFile(hndl, &fattr)  )
  {
   fname = AnsiString(fattr.cFileName);
   FManFiles->ListBox1->Items->Add( fname.SubString(1,fname.Length()-4) );
  }

}//spisok
    */
void trace_show(void)
   {
    TRACE_PTS *trace_pts_curr = trace_pts_first;
     int sgrow = 1;
    char str[60];
    fTrack->sgTrack->RowCount=n_pts+1;
       while( trace_pts_curr != NULL )
    {
  fTrack->sgTrack->Cells[0][sgrow] =
  AnsiString(trace_pts_curr->ch.chain)+"+"+AnsiString(trace_pts_curr->ch.plus);
     sprintf(str,"%15.3f", trace_pts_curr->ev_x);
     fTrack->sgTrack->Cells[1][sgrow] =  str;
          sprintf(str,"%15.3f", trace_pts_curr->ev_y);
     fTrack->sgTrack->Cells[2][sgrow] =  str;
 //      sprintf(str,"%15.3f", trace_pts_curr->shift);
   //  fTrack->sgTrack->Cells[3][sgrow] =  str;
//     fTrack->sgTrack->Cells[3][sgrow] = AnsiString(trace_pts_curr->shift);

     sgrow ++;
     trace_pts_curr = trace_pts_curr->next;
    } //while
   }

//---------------------------------------------------------------------------
__fastcall TFManFiles::TFManFiles(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void pravka(char* str)
 {
for(unsigned int j=0; j<strlen(str);j++)
{
if(str[j]==',')str[j]='.';
if(str[j]==';')str[j]=' ';
}
 }

void __fastcall TFManFiles::ListBox1DblClick(TObject *Sender)
{ double d,d1,pk1,pl,x,y,x1,y1;
    char s[128],s1[128]; long pk;
switch (FManFiles->Tag)
{
case 0:
//Ввод кривой
{
if( ListBox1->ItemIndex == 0 )
 {//Новая
 //++++
    j=-1;
 char fname1[39];
  str = InputBox("Введите полное имя файла координат X,Y  :","","");
 if(str.Length()==NULL)return;
  strcpy(fname,str.c_str()); //fname -исходный как задали
  strcpy(fname1,fname);
   fp2=fopen(fname1,"rt");
if(!fp2){ShowMessage("\n Нет исходного файла ");  return;}
  for( i=0;i<strlen(fname1);i++)
  { if(fname1[i]=='.'){j=i; break;} }//for i
  if(j>0) fname1[j]='\0';
 strcat(fname1,".crv");
 //ShowMessage(fname);
   //показывает форму со способами
//FManFiles->Close();
  fp1=fopen(fname1,"wt");  //файл crv
  i=0;j=0; d=0.0;
//получить начальный пикетаж из ftype pk+pl
 pk1=0.0;pk=0;
   ftype->ShowModal();

   if(ftype->ModalResult==mrOk)
   { pk=StrToInt(ftype->Edit1->Text.c_str());
     pl=StrToFloat(ftype->Edit2->Text.c_str());
   } //начало
   pk1=pk*100+pl;//начало
   fprintf(fp1,"%3d%12.6lf%12.6lf",2,100.0,100.0);
  //to crv
while(!feof(fp2)) // исходный
{
if((fgets(s,80,fp2)==NULL) ||(s[0]=='\n')) break;
 pravka(s); //запятую и ; меняет на точку и пробел
 sscanf(s,"%lf%lf",&x,&y);
if(i){ d1=sqrt((x-x1)*(x-x1)+(y-y1)*(y-y1));d=d+d1; pk1=pk1+d1;
//printf("\nx=%lf,y=%lf",x,y);
pk=pk1/100; pl=pk1-pk*100.0;
     }//if i
fprintf(fp1,"\n%10ld%10.4lf%17.4lf%17.4lf",pk,pl,x,y);
i++;
x1=x;y1=y;
}//while  создали файл  .crv
//fclose(fp1); exit(0);
strcpy(fname,fname1);
fclose(fp2);
fclose(fp1);
 spisok("\\*.crv");
   //FManFiles->ClientWidth=448;
  }//новая
 else
  {//для неновой формирует имя файла имя.crv
     //FManFiles->Close();
 char str[128];
   strcpy(str,ListBox1->Items->Strings[ListBox1->ItemIndex].c_str());
  strcpy(fname, strcat(str,".crv"));
  fCorners->Caption = "Кривая: "+ ListBox1->Items->Strings[ListBox1->ItemIndex];
  }

    char str1[60];
     fCorners->Tag=0;//признак работы с кривой,1-с коридором 2-fix
     fCorners->sgCorners->Cells[2][0] = "X";
     fCorners->sgCorners->Cells[3][0] = "Y";
   //  fCorners->Caption = "Кривая:"+ ListBox1->Items->Strings[ListBox1->ItemIndex];

    read_inp_corners(fname); //читает файл crv и создаёт список и LINE

      cur_corner = first_corner;
      i = 1;
     fCorners->sgCorners->RowCount=2;//  число строк
  for(int i = 1; i < fCorners->sgCorners->RowCount ; i ++)
           fCorners->sgCorners->Rows[i]->Clear();
    while( cur_corner !=  NULL )
   {
fCorners->sgCorners->Cells[0][i]=AnsiString(cur_corner->ch.chain);
fCorners->sgCorners->Cells[1][i]=AnsiString(cur_corner->ch.plus);
     sprintf(str1,"%16.3lf", cur_corner->x);
     fCorners->sgCorners->Cells[2][i] = str1;
     sprintf(str1,"%16.3lf", cur_corner->y);
     fCorners->sgCorners->Cells[3][i] = str1;
  // fCorners->sgCorners->Cells[3][i] = AnsiString(cur_corner->y);
     cur_corner = cur_corner->next;
     i ++ ;
     fCorners->sgCorners->RowCount++;// увеличил число строк
   }//while
  fCorners->sgCorners->RowCount--;
//из списка загнали в форму список больше не нужен его надо удалить
// при выходе из формы будет новый список
freeCorners();//удалил список
     fCorners->ShowModal();
  //редактирование координат и запись
break;  //switch FManFiles->Tag(=0)
}
case 1:
//ShowMessage(" Удалить кривую");
break;
case 2:
{//включить кривую

  strcpy(fname,ListBox1->Items->Strings[ListBox1->ItemIndex].c_str());
  ShowMessage(" Включить кривую " +AnsiString(fname));
  strcat(fname,".crv");
 //открыть макс сдвижку
 if(include_curve_p( fname )) // даёт список трассы!!!
 {ShowMessage("Кривая не включена");return; }
     FManFiles->Close();
  //fTrack->Label1->Visible=true;fTrack->Edit3->Visible=true;

  pk=trace_pts_first->ch.chain;
  pl=trace_pts_first->ch.plus;
  fTrack->Edit1->Text=IntToStr(pk)+ "   +   "+ FloatToStr(pl);
  pk=trace_pts_last->ch.chain;
  pl=trace_pts_last->ch.plus;
  fTrack->Edit2->Text=IntToStr(pk)+ "   +   "+ FloatToStr(pl);
   trace_show();// формирует таблицу из списка трассы
      fTrack->Show();
      ShowMessage("Уточнить пикетаж крайних точек и max сдвижку! ");
break;
}
case 3:
//ShowMessage("Ввод участка ");

  if( FManFiles->ListBox1->ItemIndex == 0 )
 {
//новый
  trackfilename = InputBox("Введите имя участка:","","");
  if(trackfilename.Length()==0) return;
  fTrack->Caption = "Участок: " + trackfilename;
  strcpy(uch_name,trackfilename.c_str());
  asdfilename =trackfilename +".asd";
  trackfilename =trackfilename +".tra";
  trace_pts_first=NULL;//чтобы можно было удалять при вызове free_pts()
  n_pts=0;
  ugl=100.0; ugp=100.0; xl=0.0; yl=0.0;
  // при включении кривых corner исправятся
  fTrack->sgTrack->RowCount=2;
  fTrack->sgTrack->Rows[1]->Clear();
   }
  else
 {   //не новый
 trackfilename = FManFiles->ListBox1->Items->Strings[ ListBox1->ItemIndex ];
 strcpy(uch_name,trackfilename.c_str());
 fTrack->Caption = "Участок: " + trackfilename;
 asdfilename =trackfilename +".asd";
 trackfilename =trackfilename +".tra";
 int j= read_trace( trackfilename.c_str());
 if(j)
    { if(j==-3)ShowMessage("Участок пуст ");else
    ShowMessage("Участок не прочитан ");
    }
   //---------------------
   pk=trace_pts_first->ch.chain;
  pl=trace_pts_first->ch.plus;
  fTrack->Edit1->Text=IntToStr(pk)+ "   +   "+ FloatToStr(pl);
  pk=trace_pts_last->ch.chain;
  pl=trace_pts_last->ch.plus;
  fTrack->Edit2->Text=IntToStr(pk)+ "   +   "+ FloatToStr(pl);
   //====
    trace_show(); // даёт форму участка
 }//else то есть не новый
   //-------------------------

    FManFiles->Close();
    fTrack->Show();

break;
case 4:
//ShowMessage(" Удалить участок");
break;
 }//switch Tag
   FManFiles->Close();
}
//---------------------------------------------------------------------------

void __fastcall TFManFiles::UDALClick(TObject *Sender)
{
// Удаление кривых и участков

  // Button4->Visible=0;
  AnsiString fdelname =  FManFiles->ListBox1->Items->Strings[ FManFiles->ListBox1->ItemIndex ];
  if(FManFiles->Tag==1)
  {
 if( Application->MessageBox("Подтверждение удаления кривой", "Удалить эту кривую", MB_YESNO + MB_ICONQUESTION ) == IDYES )
 {
//  AnsiString fdelname =  FManFiles->ListBox1->Items->Strings[ FManFiles->ListBox1->ItemIndex ];
//  ShowMessage(fdelname);return;


   if ( DeleteFile( fdelname + ".crv"  ) ) //удаление файла
  {
   FManFiles->ListBox1->Items->Delete( FManFiles->ListBox1->ItemIndex );
   Application->MessageBox(" Кривая удалена","Информация об удалении кривой", MB_OK + MB_ICONASTERISK );
  }
}//if Application
  }//if tag==1
   else if(FManFiles->Tag==4)
   {
       if( Application->MessageBox("Подтверждение удаления участка", "Удалить этот участок", MB_YESNO + MB_ICONQUESTION ) == IDYES )
 {
//  AnsiString fdelname =  FManFiles->ListBox1->Items->Strings[ FManFiles->ListBox1->ItemIndex ];
   if ( DeleteFile( fdelname + ".tra"  ) ) //удаление файла
  {
   FManFiles->ListBox1->Items->Delete( FManFiles->ListBox1->ItemIndex );
   Application->MessageBox(" Участок  удалён","Информация об удалении участка", MB_OK + MB_ICONASTERISK );
  }
    }//if Application
   } //if Tag==4
   FManFiles->ListBox1->ItemIndex = 0;
}
//---------------------------------------------------------------------------

int include_curve_p( char * curv_f_name )
{//возвращает 1 когда мало точек или вообще плохо
int i_read_crv; /* order of reading curve -1 back, 1 -forward*/
int i_write_crv;/* place of writing curve 0: empty,1: before,2: after*/
int i_curv_beg;
int beg_point_crv,end_point_crv;
int i,j,go;
  FILE* fp;
CHAINAGE  ch_crv,ch_trc;

       if (n_pts != 0)
       {
	  for( trace_pts_last = trace_pts_first;
	       trace_pts_last->next != NULL;
	       trace_pts_last = trace_pts_last->next );

        }
 /*------ Ввод кривой --- */
              uglt=100.0; ugpt=100.0;
      //до сих сделать имя файла выбранной кривой в curv_f_name
   fp=fopen(curv_f_name,"rt");
   fscanf(fp,"%3d",&type_survey); //читает 2

  fclose(fp);// надо будет снова читать type_survey

    read_inp_corners(curv_f_name);
    // создаёт список и LINE и uglt, xlt,ylt и правые

	       if (n_points<4 )
               { ShowMessage ("В кривой менее 4-х точек"); return (1);
               }

       go = 0;  //при перекрытии признак исключения точек
 //  Define i_read
       i_read_crv=1; //  снята слева направо
       count_dist(line[0].ch,line[1].ch,&dist);
       if (dist<0.)  i_read_crv=-1; //кривая снята справа налево
 // Define i_write
       i_write_crv=0; // не было включено кривых

       if (n_pts != 0)//не новый уже были точки участка
       {
	    switch(i_read_crv)
	    {
	    case -1:
		     beg_point_crv=n_points-1;
		     end_point_crv=0;
		     break;
	    case 1:
		     beg_point_crv=0;
		     end_point_crv=n_points-1;
		     break;
	    }

    count_dist(line[beg_point_crv].ch, trace_pts_first->ch,&dist);

	 if (dist>0.)//начало кривой до участка
	 {
        count_dist(line[end_point_crv].ch,trace_pts_last->ch,&dist);
	    if (dist<0.)
	       {
	     ShowMessage ("Участок внутри кривой");
		return(1);
		}
	    i_write_crv=1; // кривая начинается до участка и
                            //кончается до или внутри участка
	  }
      else  //начало кривой за началом участка
	 {
    count_dist(line[end_point_crv].ch,trace_pts_last->ch,&dist);
	    if (dist>0.)
	       {
	       ShowMessage ("Кривая внутри участка");
	       return(1);
	       }
	    i_write_crv=2; //кривая полностью или частично за участком
	  }

       } //n_pts!=0


       switch(i_write_crv)
       {

	case 0:
	//  E M P T Y     T R A C E

		      last_pts=NULL;
      if (( trace_pts_first = new(TRACE_PTS)) == NULL)
     	  { ShowMessage ("Нет памяти !" ); restore_to_empty();
     	       return(1);}
		       curr_pts = trace_pts_first;
		       curr_pts -> prev = NULL;
    		       // ---- инициализация кривая прочитана выше
		       for (i=1; i<n_points; i++)
                  {
			   last_pts = curr_pts;
                       if (( curr_pts = new(TRACE_PTS)) == NULL)
      	   {ShowMessage ("Нет памяти !" ); restore_to_empty();
    			       return(1);
                         }//restore-это удаление списков
    			curr_pts -> prev = last_pts;
		   }//for
		   curr_pts -> next = NULL;
        	   trace_pts_last=curr_pts;
		ch_trc=trace_pts_first->ch;// там ничего нет!!!

		       for (i=1; i<n_points; i++)
		       {
			  last_pts = curr_pts;
			  curr_pts = curr_pts->prev;
			  curr_pts->next = last_pts;
		       }
		      for (i=0; i<n_points; i++)
		  {
			     curr_pts->min_shift=shf_l;
			     curr_pts->max_shift=shf_r;
                             curr_pts->shift=0;

			  switch (i_read_crv)
			  {
			   case 1: //вправо
			     curr_pts->ch = line[i].ch;
			     curr_pts->dist = line[i].l;
			     curr_pts->angle = line[i].alpha;
      curr_pts->ev_x=line[i].x; curr_pts->ev_y=line[i].y;
				   break;
			   case -1: //  влево
			     j = n_points-i-1;
			     curr_pts->ch = line[j].ch;
			     if (i==0)  {curr_pts->dist=0.0;}
			     else
		     curr_pts->dist =  line[j+1].l; //почему +1??
		     curr_pts->angle =- line[j].alpha;
    curr_pts->ev_x=line[j].x; curr_pts->ev_y=line[j].y;
		   	   break;
			   }
                     curr_pts = curr_pts -> next;
		      } //for i

		      n_pts=n_points;
		      i_curv_beg = 1;
//Здесь получаем нач угол и координаты.uglt,xlt из  read_inp_corners

              if(i_read_crv==1)
    { ugl=uglt; ugp=ugpt; xl=xlt; yl=ylt;//xp=xpt; yp=ypt;
    }
         else
          { ugl=ugpt+PI;if(ugl>=2.*PI) ugl=ugl-2.*PI;
	    //ugp=uglt+PI; if(ugp>=2*PI) ugp=ugp-2.*PI;
            xl=xpt; yl=ypt; //xp=xlt; yp=ylt;
//правые для участка не нужны, а для кривой были нужны,
//так как при обратном способе съёмки они станут левыми
          }

        set_ugl=1;// для первой кривой при любом способе съёмки
		    break;

	case 1:
	    // C U R V E   B E F O R E   T R A C E
           //  ShowMessage ("Возможно перекрытие начала трассы");
	          end_pts = trace_pts_first;
           // trace_pts_last не изменяется
		      last_pts=NULL;
		      n_pts++;
		       if (n_pts > MAXPOINTS)
		      { ShowMessage ("Переполнение участка"); restore_to_begin();
								return(1);}
    if (( trace_pts_first = new( TRACE_PTS ) ) == NULL)
		     { ShowMessage ("Нет памяти !" ); restore_to_begin();
					       return(1);}

		      curr_pts = trace_pts_first;
		      curr_pts -> prev = NULL;

		      for (i=1; i<n_points; i++)
			  {
			   last_pts = curr_pts;
			   n_pts++;
				 if (n_pts >MAXPOINTS)
				 { ShowMessage ("Переполнение участка");
				    restore_to_begin(); return(1);}
   if (( curr_pts = new( TRACE_PTS ) ) == NULL)
     { ShowMessage ("Нет памяти !" ); restore_to_begin();
					       return(1);}
			   last_pts -> next = curr_pts;
			   curr_pts -> prev = last_pts;
			  }

		      curr_pts->next = NULL;
		      curr_pts = trace_pts_first;

		      for (i=0; i<n_points; i++)
		      {
			     curr_pts->min_shift=shf_l;
			     curr_pts->max_shift=shf_r;
                             curr_pts->shift=0;
			     switch (i_read_crv)
			  {

			   case 1:
			     curr_pts->ch = line[i].ch;
			     curr_pts->dist =line[i].l;
			     curr_pts->angle = line[i].alpha;
    curr_pts->ev_x=line[i].x; curr_pts->ev_y=line[i].y;
				   break;
			   case -1:
			     j = n_points-i-1;
			     curr_pts->ch = line[j].ch;
			     if (i==0)  {curr_pts->dist=0.0;}
			     else {curr_pts->dist =  line[j+1].l;}
			     curr_pts->angle =- line[j].alpha;
    curr_pts->ev_x=line[j].x; curr_pts->ev_y=line[j].y;
				 break;
                	   }
		ch_crv=curr_pts->ch;//текущий пикет  кривой
		ch_trc=end_pts->ch; //пикет начала остатка трассы
       count_dist (ch_trc,ch_crv,&dist);
			    while (dist >=  0. )

			      {
		 if (go==FALSE)
         {
 if(MessageDlg("Допускаем перекрытие?",mtWarning,TMsgDlgButtons() << mbYes<<mbNo,0)==mrYes)
        go=TRUE; else return(1);

    	 }
                                n_pts=n_pts-1;
				end_pts=end_pts->next;
				ch_trc=end_pts->ch;
  	//if (count_dist (ch_trc,ch_crv,&dist))
        //  message ("перекрытие начала трассы");
   //Нужен новый dist,а выдача не нужна!
                                 count_dist(ch_trc,ch_crv,&dist);
      	      delete(end_pts->prev);
	              }//while
  if  (curr_pts -> next != NULL) curr_pts=curr_pts->next;
		      } //for
			  curr_pts->next= end_pts;
			  end_pts->prev = curr_pts;
                 	  ch_crv= curr_pts->ch;
			  ch_trc=end_pts->ch;
	      if (count_dist(ch_crv,ch_trc,&dist))
  { ShowMessage ("Неверно задан пикетаж"); exit(0);}
		       end_pts->dist=dist;
		      i_curv_beg = 1;

        if(i_read_crv==1)
          { ugl=uglt;  xl=xlt; yl=ylt; }
         else
          { ugl=ugpt+PI;if(ugl>=2.*PI) ugl=ugl-2.*PI;
            xl=xpt; yl=ypt;
          }
             break;

	case 2:     // C U R V E    A F T E R     T R A C E
 		      i=0;
		      curr_pts = trace_pts_first;
		      while (curr_pts != NULL)
			  {  i++;
			     curr_pts = curr_pts->next;
			  }
    // зачем посчитали число точек в имеющейся трассе i?

	       ch_crv = line[beg_point_crv].ch;
	       curr_pts = trace_pts_first;
	       end_pts = NULL;
	       i_curv_beg = 0;
        	while (curr_pts != NULL)
	    {
		  i_curv_beg++;
		  ch_trc=curr_pts->ch;
		  count_dist(ch_crv,ch_trc,&dist);

		  if ( dist >=0. )
		     {
			if (go==FALSE)
			   {
 if( MessageDlg("Допускаем перекрытие?",mtWarning,TMsgDlgButtons()<<mbYes << mbNo,0)==mrYes)
           go=TRUE; else return(1);

			    }
			       i_curv_beg--;
			       n_pts--;
			       next_pts=curr_pts->next;
			     delete (curr_pts);
			       curr_pts=next_pts;
		      }
		  else
		     {
		       end_pts=curr_pts;
		       curr_pts = curr_pts->next;
		     }
        } //while curve_pts !=0

		      n_pts++;
			 if (n_pts >MAXPOINTS)
			   {   if(!go) restore_to_end();
   ShowMessage("Переполнение участка");return(1);
                           }
     if ( ( curr_pts = new( TRACE_PTS) ) == NULL)
			   {  if(!go) restore_to_end();
 ShowMessage ("Нет памяти !" );  return(1);
			   }
		       curr_pts -> prev = end_pts;
		       if (end_pts != NULL)
			   end_pts -> next = curr_pts;
			   else {trace_pts_first=curr_pts;
				 end_pts=trace_pts_first;}
		       curr_pts -> prev = end_pts;
		       for (i=1; i<n_points; i++)
			{
			   last_pts = curr_pts;
			   n_pts++;
			    if (n_pts >MAXPOINTS)
			    {       if(!go) restore_to_end();
        ShowMessage("Переполнение участка");return(1);
      			    }
   if ( ( curr_pts = new(TRACE_PTS) ) == NULL)
			   {if(!go) restore_to_end();
   ShowMessage ("Нет памяти !" ); return(1);
                           }
				curr_pts -> prev = last_pts;
			} //for  n_points
			curr_pts -> next = NULL;
        trace_pts_last=  curr_pts;//для установки конечного пикета

		       for (i=1; i<n_points; i++)
		       {
			  last_pts = curr_pts;
			  curr_pts = curr_pts->prev;
			  curr_pts->next = last_pts;
		       }
                       for (i=0; i<n_points; i++)
		      {
			     curr_pts->min_shift=shf_l;
			     curr_pts->max_shift=shf_r;
                             curr_pts->shift=0;
			     switch (i_read_crv)
			  {

			   case 1:
			     curr_pts->ch = line[i].ch;
			     curr_pts->dist =  line[i].l;
			     curr_pts->angle = line[i].alpha;
   curr_pts->ev_x=line[i].x; curr_pts->ev_y=line[i].y;
			     if (i==0)
			       {
				 last_pts=curr_pts->prev;
	     if (count_dist ( last_pts->ch, curr_pts->ch,&dist))
	 {  ShowMessage ("Неверно задан пикетаж");exit(0);}
				   curr_pts->dist= dist;
				}
				   break;
			   case -1:
			     j = n_points-i-1;
			     curr_pts->ch = line[j].ch;
			     curr_pts->angle =- line[j].alpha;
  curr_pts->ev_x=line[j].x; curr_pts->ev_y=line[j].y;
				   break;
			  } //switch  i_read_crv

//        if(i==n_points-1)trace_pts_last=  curr_pts;
// CHAINAGE PP=       trace_pts_last->ch;
			    curr_pts = curr_pts -> next;
		  }  //for   i
		      curr_pts=end_pts->next;
		      ch_crv=curr_pts->ch;
		      ch_trc=end_pts->ch;
  //
  if (count_dist(ch_trc,ch_crv,&dist)) //при ошибке всегда 1
      { ShowMessage ("Неверно задан пикетаж");exit(0);
      }
				   curr_pts->dist= dist;
//      if(type_survey==CORNER)
  //     {
   //     if(i_read_crv==1)
     //     {  ugp=ugpt; xp=xpt; yp=ypt; }
       //  else
         // {
	   // ugp=uglt+PI; if(ugp>=2*PI) ugp=ugp-2.*PI;
             //xp=xlt; yp=ylt;
         // }
        // было закомм corner  не нцужны правые для участка}
		    break;


  }//switch write_crv нет -нуль  и куда писать в список 1 до 2 после

//fprintf(file_dbg,"\n n_pts=%ld, n_points=%d",n_pts, n_points);
  // w2_pts_first=trace_pts_first;
    //  refresh_pts( i_curv_beg );//ввод и интерполяция обочин и высвечивание
//      таблицы с пределами лево право как на участке
//чтобы потом дать начало и конец лучше это на выходе???
   if (n_pts > MAXPOINTS)
      {ShowMessage ("Переполнение участка"); exit(0);}
    //trace_changed=TRUE;
return( 0 );
}

void __fastcall TFManFiles::Button1Click(TObject *Sender)
{
       //возврат
     Close();
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------


void __fastcall TFManFiles::FormClose(TObject *Sender,
      TCloseAction &Action)
{
//FManFiles->ClientWidth=318;

}
//---------------------------------------------------------------------------






