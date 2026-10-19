//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <stdio.h>
#include <io.h>//для remove
#include<math.h>
#include "Unit1.h"
#include "extern.h"
#include <string.h>
#include<stdlib.h>
#include "Unit2.h"
#include "Unit4.h"
#include "Unit5.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
extern int newvar;
extern Tel *el;
extern TForm4 *Form4;// добавил
extern TForm5 *Form5;//добавил
void sgzap();// запись коридора при редактировании его
void trace_show(void);
void elts_show(void);
 int read_elts(char*file_name);
 void write_trace( char *file_name );
  int read_trace( char *file_name );
 double shmin,shmax,ksi,eta,xc,yc,ugl1,gam,tk,phi;
 int razpech;
 //при выборе вар-та 0, чтобы подготовить и вызвать оптимизатор как
 //для нового и разрешить печать
  double Asd(AnsiString s)
  {//Борьба с запятой в редактируемом тексте даёт из текста double
  // StrToFloat(AnsiString) требует запятую в строке
     double y;
    char * sd=new char [128];
    strcpy(sd,s.c_str());
     for(unsigned int j=0;j<strlen(sd);j++)
   {if(sd[j]==','){sd[j]='.'; break;}}
   y=atof(sd);
   delete sd;
    return y;
  }
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------   TRACE_PTS *trace_pts_first,*trace_pts_last;  //  UI.cpp без extern

   struct TRACE_PTS_TAG *tr;
   struct  TRACE_ELTS_TAG *elts;
     FILE *fp;//для чтения и записи
 int n_pts; double ugl,xl,yl;  double uglpr,xlpr,ylpr;// for extern
 long picnach; //tr[0].ch.chain
  double plusnach;//tr[0].ch.plus
  double per5;
 int n_elts; double min_l_first,max_l_first,l_first,min_l_last;
//для оптимизации
 int numper;
double deo=0.5,plus;
long picet;
int chkor=0;// признак изменения коридора
double *beta=NULL;
     STARTUPINFO si;
    PROCESS_INFORMATION pi;

  void vizov(char* st)
  {
    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    ZeroMemory( &pi, sizeof(pi) );
    // Start the child process.
    if( !CreateProcess( NULL, // No module name (use command line).
        st, // Command line.
        NULL,             // Process handle not inheritable.
        NULL,             // Thread handle not inheritable.
        FALSE,            // Set handle inheritance to FALSE.
        0,                // No creation flags.
        NULL,             // Use parent's environment block.
        NULL,             // Use parent's starting directory.
        &si,              // Pointer to STARTUPINFO structure.
        &pi )             // Pointer to PROCESS_INFORMATION structure.
    )
    {
        //ErrorExit( "CreateProcess failed prof1." ); нет такой функции?
  Application->MessageBox("CreateProcess failed.",st,MB_OK|MB_ICONERROR);
    }

    // Wait until child process exits.
    WaitForSingleObject( pi.hProcess, INFINITE );

    // Close process and thread handles.
    CloseHandle( pi.hProcess );
    CloseHandle( pi.hThread );

  }


/*  int count_chain( CHAINAGE ch1,  double dist, CHAINAGE *ch2);
       int km_to_pic(double km,long *pic,float *plus);
   int pic_to_km(  long pic, float plus,double *km);
   */
   //+++++++++++++
   void opt_past_ugl()
{
//После углограммы готовит и вызывает оптимизатор с нулём
 //sgzap();
 //нужно при оптимизации могли править границы, но не здесь
 //коридор-> tr + write_trace работает с fp!!!
 FILE *fp;
 int i;
fp=fopen("fortxy1.txt","wt");
if(fp==NULL){ ShowMessage("Не открывает для записи fortxy1.txt"); exit(0);}
int pr=0;
fprintf (fp,"%5d",pr);
 fprintf (fp,"%5d%10.2lf%10.2lf",0,0.0,0.0);
// double shmin,shmax,ksi,eta,xc,yc,ugl1,gam,tk,phi;
  tr[0].dist=0.;
 xc=yc=0; ugl1=0;//в оптимизации всe с нуля в пределах круга
  i=-n_pts;// чтобы был нуль итераций -npts?!!!
   fprintf (fp,"\n%5d%5d",i,n_elts);

for( i=0;i<n_pts;i++)
{
shmin=tr[i].min_shift; shmax=tr[i].max_shift;
//при чтении трассы их делал 0.0015 в фортране точность 0.004
    xc+= tr[i].dist*cos(ugl1);
    yc+= tr[i].dist*sin(ugl1);
 if(i==0)
   {/*длину слева сделал нулевую */
    gam=ugl1;
/*в начале не нужен поперечник, а угол с осью х пригодится
     его запишем и используем вместо нормали
*/
   }
  else
 {
      //if(!trace_pts_curr->next)
        if(i==n_pts-1)
       { tk=1; phi=0.;}
     else
    {
 //tk=(trace_pts_curr->next->dist/trace_pts_curr->dist);
    tk=tr[i+1].dist/tr[i].dist;
//     phi=trace_pts_curr->angle;
   phi=tr[i].angle;
     }
   gam=ugl1+phi/(1+tk)+3.1415926/2;
     /*gam-от центра кривизны к вершине или наоборот но всегда
в положительном направлении против стрелки(влево)
углы от центра к вершине и к середине хорды относятся как хорды
 их сумма равна phi
     */
   }  //else
// fprintf(file,"\n%12.3lf %12.3lf %12.8f",xc,yc,gam);

     ugl1+= tr[i].angle;

///
//fprintf (fp,"\n%10.3lf%10.3lf%12.3lf%12.3lf%12.8f",shmin,shmax,xc,yc,gam);
fprintf (fp,"\n%10.3lf%10.3lf%16.8lf%16.8lf%12.8f",shmin,shmax,xc,yc,gam);
}//for

//++++
/* ugl1-конечный угол по хордам началь нуль!!!== phifin ksi,eta не используются
  добавил запись нач данных,вычисление и запись в новый файл x,y и углов
  направлений к центру кривизны с осью х поворот влево- плюс
*/

ksi=tr[n_pts-1].ev_x-min_l_last;
eta=tr[n_pts-1].ev_y-phifin*min_l_last;
fprintf (fp,"\n%20.8lf%20.5lf%20.5lf",ugl1,ksi,eta);
fprintf (fp,"\n%12.6lf%12.6lf%12.6lf",min_l_first,l_first,max_l_first);

for(int i=0;i<=n_elts;i++)
{
if(elts[i].l_clot<0.00001)
fprintf (fp,"\n%12.6lf%12.6lf%12.6lf",0.000,0.000,0.000);
else
fprintf (fp,"\n%12.6lf%12.6lf%12.6lf",
elts[i].min_l_clot,elts[i].l_clot,elts[i].max_l_clot);
if(i==n_elts) break;
fprintf (fp,"\n%12.6lf%12.6lf%12.6lf",
elts[i].min_l_circ,elts[i].l_circ,elts[i].max_l_circ);
if(fabs(elts[i].rad)<0.0001)
fprintf (fp,"\n%20.8lf%20.8lf%20.12lf",0.0,0.0,0.0);
else
{
shmin=1.0/elts[i].max_rad;
shmax=1.0/elts[i].min_rad;
  if(elts[i].rad<0.0)
   {
   shmin=-1.0/elts[i].min_rad;
   shmax=-1.0/elts[i].max_rad;
   }
fprintf (fp,"\n%20.12lf%20.12lf%20.12lf",shmin,1.0/elts[i].rad,shmax);
}//было в фортране если мин и мах радиусы равны то кривизна нуль???
}//for
// последняя клотоида записана в цикле
//этот кусок оформить как функцию и вызывать её после углограммы, но deo?
//функция с параметром здесь с нулём, а твм с 1, то есть там deo нуль !!!
   fclose(fp);
   rewind (fp);
 // exit(0);
 if(FileExists("bufxy1.txt"))
 {
 i= remove("bufxy1.txt");
if(i)ShowMessage("Не удалён bufxy1.txt");
  }
vizov("reorxy1.exe");
 //Здесь не было оптимизации только рихтовки взял и bufxy1 здесь не нужен
 //Но должен читать всё из-за рихтовок которые в хвосте

fp=fopen("bufxy1.txt","rt");
double s,s2;
 double s1;//float
 el->Close();
s=per5;
i= fscanf(fp,"%lf",&l_first);
 if(i!=1){ShowMessage("Сдвижки не вычислены. Нет оптимизации. Ошибки");
           fclose(fp); return;
         }
 s=s+l_first;

 for(int i=0;i<=n_elts;i++)
  {
   km_to_pic(s,&elts[i].cl_ch0.chain,&s1);
   elts[i].cl_ch0.plus=s1;
  if(i<n_elts)
  fscanf(fp,"%lf%lf%lf",&elts[i].l_clot,&elts[i].l_circ,&s2);
  else fscanf(fp,"%lf",&elts[i].l_clot);
  s=s+elts[i].l_clot;
  km_to_pic(s,&elts[i].cl_ch1.chain,&s1);
  elts[i].cl_ch1.plus=s1;
  if(i==n_elts) break;
   if(fabs(s2)<1.e-8) elts[i].rad=0.0;else elts[i].rad=1.0/s2;
   s=s+elts[i].l_circ;
  }//for i прочли из buf новые длины элементов там ещё рихтовки
   elts_show();//занести новые элементы в форму
   //write_elts (elt_name);
//записать новые элементы в файл не нужно их записала UGLOGRAMMA
    for(int i=0;i<n_pts;i=i+5)
{
    for (int j=0;j<5;j++)
    { if(i+j<n_pts)
      fscanf(fp,"%lf",&tr[i+j].shift);else break;
    }//for j
}//for i запись рихтовок в массив tr
if(beta==NULL)beta=new double [n_pts];
 for(int i=0;i<n_pts;i=i+5)
{
 for (int j=0;j<5;j++)
    { if(i+j<n_pts)
      fscanf(fp,"%lf",&beta[i+j]);else break;
    }//for j  fp===bufxy1 
}//for i чтение новых  углов нормалей
tr[0].shift=sqrt((xlpr-xl)*(xlpr-xl)+(ylpr-yl)*(ylpr-yl));//не нуль при новом начале
    trace_show(); // новые рихтовки в форму
    write_trace(trackfilename.c_str());//в файл с новыми рихтовками
   el->sgEl->Visible=true;
     //ShowMessage(" el->ShowModal()");
    el->ShowModal();
    fclose(fp);
}

   //+++++++++++=
 int read_elts(char*file_name)
 { double s;
 double s1;//float
 s=per5;
fp = fopen ( file_name, "rt" );
   if( fscanf (fp,"%d", &n_elts ) != 1 )
   { fclose( fp );return( -2 );}
   fscanf(fp,"\n%lf",&l_first);
    fscanf(fp,"\n%lf%lf%lf",&min_l_first,&max_l_first,&min_l_last);
   if( max_l_first<min_l_first+0.1)max_l_first=700.0;
    elts=new TRACE_ELTS_TAG[n_elts+1];
    s=s+l_first;

     for (int i=0;i<n_elts;i++)
     {
 km_to_pic( s,&elts[i].cl_ch0.chain,&s1);
 elts[i].cl_ch0.plus=s1;
fscanf(fp,"\n%lf%lf%lf",&elts[i].l_clot,&elts[i].min_l_clot,&elts[i].max_l_clot);
fscanf(fp,"\n%lf%lf%lf",&elts[i].rad,&elts[i].min_rad,&elts[i].max_rad);
fscanf(fp,"\n%lf%lf%lf",&elts[i].l_circ,&elts[i].min_l_circ,&elts[i].max_l_circ);
fscanf(fp,"\n%lf", &elts[i].h);
elts[i].h=0.0;// добавил при повторе обнуляю иначе звёзды в печати
    s=s+elts[i].l_clot;
   km_to_pic( s,&elts[i].cl_ch1.chain,&s1);
   elts[i].cl_ch1.plus=s1;
    s=s+elts[i].l_circ;
    elts[i].vpas=elts[i].vgruz= 0.0;
     }//for
fscanf(fp,"\n%lf%lf%lf",&elts[n_elts].l_clot,&elts[n_elts].min_l_clot,&elts[n_elts].max_l_clot);
km_to_pic( s,&elts[n_elts].cl_ch0.chain,&s1);
elts[n_elts].cl_ch0.plus=s1;
  s=s+elts[n_elts].l_clot;
   km_to_pic( s,&elts[n_elts].cl_ch1.chain,&s1);
   elts[n_elts].cl_ch1.plus=s1;
fclose(fp);
formugl();//формирует файл проектной углограммы uch_name.ugl для графики
 return(0);
 }

//
  void write_trace( char *file_name )
{

   int i;
   long chain;
   double plus;

//   char source_angle_r[12],source_arrow_r[8];
//------------------
   fp = fopen ( file_name, "wt" );
    fprintf (fp,"%d", n_pts );
    fprintf (fp,"\n%15.8lf%18.8lf%18.8lf",ugl,xl,yl);


   for( i=0; i<n_pts; i++ )
   {
      fprintf(fp,"\n%10ld%10.5lf",tr[i].ch.chain,tr[i].ch.plus);
      fprintf (fp,"\n%15.9lf%15.9lf",tr[i].dist,tr[i].angle);
      fprintf (fp,"\n%12.6lf%12.6lf",tr[i].min_shift,tr[i].max_shift);
      fprintf (fp,"\n%12.4lf%12.4lf",tr[i].ev_x,tr[i].ev_y );//12.6 было
      fprintf (fp,"\n%12.6lf", tr[i].shift );
  }//for
   fclose( fp );
}

//
  int read_trace( char *file_name )
{

   int i;
   long chain;
   double plus;

//   char source_angle_r[12],source_arrow_r[8];
//------------------
   fp = fopen ( file_name, "rt" ); //.tra
   if( fscanf (fp,"%d", &n_pts ) != 1 )
   { fclose( fp );return( -2 );}
   if( fscanf (fp,"\n%lf%lf%lf",&ugl,&xl,&yl) != 3 )  //begin
   { fclose( fp );return( -2 );}
   if( n_pts <= 3 )
   {
      fclose( fp );
      return( -3 );
   }

    tr=new  TRACE_PTS_TAG[n_pts];
   if ( tr== NULL )
   { fclose( fp ); return( -4 );}

      for( i=0; i<n_pts; i++ )
   {
      if( fscanf(fp,"\n%ld%lf", &chain,&plus) != 2 )
      { fclose( fp );return( -5 );}

      tr[i].ch.chain = chain;
      tr[i].ch.plus = plus;
    if( fscanf (fp,"\n%lf%lf",
     &tr[i].dist,&tr[i].angle  ) != 2 )
      { fclose( fp );return( -5 );}
      if( fscanf (fp,"\n%lf%lf",
       &tr[i].min_shift,&tr[i].max_shift) != 2 )
      {	 fclose( fp );	 return( -5 ); }
   /*
   if(i>n_pts-3) {tr[i].min_shift=-0.0015;
                   tr[i].max_shift=0.0015;
                  }
  */
      if( fscanf (fp,"\n%lf%lf",
      &tr[i].ev_x,&tr[i].ev_y ) != 2 )
      {	 fclose( fp );	 return( -5 );  }
      if( fscanf (fp,"\n%lf", &tr[i].shift ) != 1 )
      {	 fclose( fp );	 return( -5 );}
  }//for
pic_to_km(  tr[0].ch.chain, tr[0].ch. plus, &per5);// начальная абсцисса
   fclose( fp );
   //временно для проверки
       /*
 double x,y,ugl1;  FILE *fpr;
 fpr=fopen("Nevjzka.txt","wt");
 x=xl; y=yl; ugl1=ugl;
  fprintf(fpr,"pk=%5ld pl=%7.3lf,x=%12.4lf,y=%12.4lf",
   tr[0].ch.chain,tr[0].ch.plus,x,y);
   for(i=0;i<n_pts;i++)
 {
   x+=tr[i].dist*cos(ugl1); y+=tr[i].dist*sin(ugl1);
   fprintf(fpr,"\npk=%5ld pl=%7.3lf,x=%12.4lf,y=%12.4lf",
   tr[i].ch.chain,tr[i].ch.plus,x,y);
   ugl1+=tr[i].angle;

 }
 fclose(fpr); //конец проверки
         */
   return 0 ;
}

  CHAINAGE StrToChainage(char *sp)
{
 CHAINAGE ch;

  ch.chain =  atol( sp );
  ch.plus  =  atof( strchr(sp, '+') + 1 );

 return ch;
}

void trace_show(void)
   {//из массива tr  заполняет форму но не показывает
    int sgrow = 1;
    char str[127];
    Form1->sgTrack->Cells[0][0] = "   NN  ";
    Form1->sgTrack->Cells[1][0] = "    Пикетаж";
 Form1->sgTrack->Cells[2][0] = "Min.сдвижка (вправо минус)";
 Form1->sgTrack->Cells[3][0] = "Max.сдвижка ( влево плюс)";
 Form1->sgTrack->Cells[4][0] = "    Сдвижка";
        Form1->sgTrack->RowCount=n_pts+1;
       while( sgrow<=n_pts )
    {
      Form1-> sgTrack->Rows[sgrow]->Clear();
  Form1->sgTrack->Cells[0][sgrow] ="   "+AnsiString(sgrow)+"    ";
  Form1->sgTrack->Cells[1][sgrow] =
  AnsiString(tr[sgrow-1].ch.chain)+"+"+AnsiString(tr[sgrow-1].ch.plus);
     sprintf(str,"%10.3f", tr[sgrow-1].min_shift);
  Form1-> sgTrack->Cells[2][sgrow] =  str;
          sprintf(str,"%10.3f", tr[sgrow-1].max_shift);
  Form1->   sgTrack->Cells[3][sgrow] =  str;
        sprintf(str,"%10.3f", tr[sgrow-1].shift);
  Form1->    sgTrack->Cells[4][sgrow] =  str;
//     fTrack->sgTrack->Cells[3][sgrow] = AnsiString(trace_pts_curr->shift);

     sgrow ++;

    } //while
//     if(StrToInt(Form1->sgTrack->Cells[0][5])>9) ShowMessage(Form1->sgTrack->Cells[0][5] );
   }
 //----------------------------------------------------------------
void __fastcall TForm1::sgTrackDrawCell(TObject *Sender, int ACol,
      int ARow, TRect &Rect, TGridDrawState State)
{// из массива в форму с красным цветом
//   if(set_var) {return;}
//   if(State.Contains(gdSelected)) ShowMessage("gdSelected"); //else ShowMessage("else");

 if(ARow==0)
 sgTrack-> Canvas->TextOut( Rect.Left,Rect.Top,sgTrack->Cells[ACol][ARow] );
          else
      {
  if((ACol==4)&&((tr[ARow-1].shift>tr[ARow-1].max_shift)||
     (tr[ARow-1].shift<tr[ARow-1].min_shift)))
      sgTrack->Canvas->Font->Color = clRed; else
      sgTrack->Canvas->Font->Color = clWindowText;

// sgTrack-> Canvas->TextOut( Rect.Left,Rect.Top,sgTrack->Cells[ACol][ARow] );
sgTrack->Canvas->TextRect(Rect, Rect.Left,Rect.Top,sgTrack->Cells[ACol][ARow]);
     }//>0
    }
   //-----------------------------------------------------------
      void elts_show(void)
   {   AnsiString s;
      char str[127];
      el->nkriv->Text=AnsiString(n_elts);
      el->Lmin->Text=AnsiString(min_l_first);
      el->Lmax->Text=AnsiString(max_l_first);
      el->Lfact->Text=AnsiString(l_first);
      el->Lend->Text=AnsiString(min_l_last);
            el->sgEl->RowCount=2;
                   el->sgEl->RowCount=n_elts+1;
  for(int i=0;i<n_elts;i++)
    {
    el->sgEl->Cells[0][i]=AnsiString(i+1);
    el->sgEl->Cells[1][i]=AnsiString(elts[i].min_l_clot);
    sprintf(str,"%8.4lf", elts[i].l_clot);
    el->sgEl->Cells[3][i]=AnsiString(str);
    el->sgEl->Cells[2][i]=AnsiString(elts[i].max_l_clot);
    sprintf(str,"%6.2lf", elts[i].cl_ch0.plus);
    el->sgEl->Cells[4][i]=AnsiString(elts[i].cl_ch0.chain)+"+"+
                          AnsiString(str);
   sprintf(str,"%6.2lf", elts[i].cl_ch1.plus);
    el->sgEl->Cells[5][i]=AnsiString(elts[i].cl_ch1.chain)+"+"+
                           AnsiString(str);


    el->sgEl->Cells[6][i]=AnsiString(elts[i].min_rad);
 sprintf(str,"%8.4lf", elts[i].rad);
    el->sgEl->Cells[8][i]=AnsiString(str);
    el->sgEl->Cells[7][i]=AnsiString(elts[i].max_rad);
    el->sgEl->Cells[9][i]=AnsiString(elts[i].min_l_circ);
    el->sgEl->Cells[10][i]=AnsiString(elts[i].max_l_circ);
        sprintf(str,"%10.5lf", elts[i].l_circ);
    el->sgEl->Cells[11][i]=AnsiString(str);
    }//for
   el->sgEl->Cells[0][n_elts]=AnsiString(n_elts+1);
    el->sgEl->Cells[1][n_elts]=AnsiString(elts[n_elts].min_l_clot);
     sprintf(str,"%8.4lf", elts[n_elts].l_clot);

    el->sgEl->Cells[3][n_elts]=AnsiString(str);
    el->sgEl->Cells[2][n_elts]=AnsiString(elts[n_elts].max_l_clot);
        sprintf(str,"%6.2lf", elts[n_elts].cl_ch0.plus);
    el->sgEl->Cells[4][n_elts]=AnsiString(elts[n_elts].cl_ch0.chain)+"+"+
                          AnsiString(str);
    sprintf(str,"%6.2lf", elts[n_elts].cl_ch1.plus);

    el->sgEl->Cells[5][n_elts]=AnsiString(elts[n_elts].cl_ch1.chain)+"+"+
                           AnsiString(str);
     for(int i=6;i<12;i++) el->sgEl->Cells[i][n_elts]=" ";//гасит хвост
     el->sgEl->RowCount=n_elts+1;
   };


//
void f_copy(char * copy_name,char*uch_name_old,char c)
{
int i,i1;char a[2],b[4];

strcpy(b,".e01");
b[1]=c;
    i1=strlen(copy_name);
     i=strlen(uch_name_old); //старое
    strcat(copy_name,b); strcat(uch_name_old,b);
    for(int k=0;k<=9;k++)
    { itoa(k,a,10);
      copy_name[i1+2]=a[0]; uch_name_old[i+2]=a[0];
    for (int j=0;j<=9;j++)
    {
    itoa(j,a,10);
    copy_name[i1+3]=a[0]; uch_name_old[i+3]=a[0];
    if(FileExists(uch_name_old))
   CopyFile( uch_name_old,copy_name,1);
     }   //j
    }//k
 }//f_copy
void f_del(char* del_name,char c)
{
int i,i1;  char a[2],b[4];
strcpy(b,".e01");
b[1]=c;
    i1=strlen(del_name);
    strcat(del_name,b);
    for(int k=0;k<=9;k++)
    { itoa(k,a,10);
      del_name[i1+2]=a[0];
    for (int j=0;j<=9;j++)
    {
    itoa(j,a,10);
    del_name[i1+3]=a[0];
    DeleteFile(del_name);
    }   //j
    }//k
}         //f_del
//---------------------------------------------------------------------------

void __fastcall TForm1::FormCreate(TObject *Sender)
{ // ShowMessage(pereg);  //pereg extern 
/*int j=0;
for (int i=strlen(pereg)-1;i>0;i--)
{if(pereg[i]=='\\'){j=i+1; break;}
}
*/
peregon->Caption="   "+(AnsiString)pereg;
}
//---------------------------------------------------------------------------
 int spisok(char*s,char pr)
      {//koord
     // Формирует список файлов *s  в FManFiles->ListBox1
   char curpath[ 128 ];
  GetCurrentDirectory(128, curpath);
  _WIN32_FIND_DATA fattr;
//   HANDLE hndl = FindFirstFile( strcat( curpath, "\\*.crv"  ), &fattr);
   HANDLE hndl = FindFirstFile( strcat( curpath, s  ), &fattr);
   AnsiString fname = AnsiString(fattr.cFileName);
   FManFiles->ListBox1->Items->Clear();
  if(pr==1)
   {
    if(hndl==INVALID_HANDLE_VALUE)
     {
     ShowMessage("Участков нет! Сформировать!" );

     return 1;

     }

   FManFiles->ListBox1->Items->Add( fname.SubString(1,fname.Length()-4) );
   }
   else
{
        FManFiles->ListBox1->Items->Insert(0,"------НОВЫЙ------");
         if(hndl!=INVALID_HANDLE_VALUE)
      {
  FManFiles->ListBox1->Items->
  Add( fname.SubString(fname.Length()-1,fname.Length()) );
       }
 }
 // для участков берём начало для вариантов конец
   while( FindNextFile(hndl, &fattr)  )
  {
   fname = AnsiString(fattr.cFileName);
if(pr==1)
   FManFiles->ListBox1->Items->Add( fname.SubString(1,fname.Length()-4) );else
 FManFiles->ListBox1->Items->
  Add( fname.SubString(fname.Length()-1,fname.Length()));

  }

  FManFiles->ListBox1->ItemIndex = 0;
  return 0;
      }
//---------------------------------------------------------------------------
void __fastcall TForm1::viborClick(TObject *Sender)
{
// выбор участка
chkor=0;// изменение коридора
FManFiles->Tag=1;
  if( spisok("\\*.tra",1)) Form1->Close();
    //FManFiles->UDAL->Visible=0;
    //FManFiles->Tag=3;
    FManFiles->ListBox1->ItemIndex = 0;
    FManFiles->Caption="Выбор участка";
    FManFiles->Label2->Caption=" Имена участков";

    ras->Visible=true;
    cop->Visible=true;
    udal->Visible=true;

  //  fin->Visible=true;

    FManFiles->ShowModal();
//форма со списком имён при выборе читает и
//создаёт массив tr из файла tra ugl,xl,yl
     evolvents();// не нужна в файле трассы  есть эвольвенты
     trace_show();     // запись в массив  tr из формы??
 // заносит в форму участка данные из массива tr но где взять его ?
 //массив создан при выборе участка из списка и чтения файла tra
     sgTrack->Visible=true;
 }
//---------------------------------------------------------------------------

void __fastcall TForm1::udalClick(TObject *Sender)
{
    char del_name[20];
     if(!set_uch){ ShowMessage(" Не выбран участок"); return;}
    if( Application->MessageBox(uch_name,"Подтверждение удаления участка",  MB_YESNO + MB_ICONQUESTION ) == IDYES )
 {

   if ( DeleteFile( trackfilename  ) ) //удаление файла
  {
   FManFiles->ListBox1->Items->Delete( FManFiles->ListBox1->ItemIndex );
   Application->MessageBox("Информация об удалении участка"," Участок удалён", MB_OK + MB_ICONASTERISK );
//    DeleteFile(asdfilename);не нужен asd

    strcpy(del_name,uch_name);
   f_del(del_name,'e');//портит del_name
  strcpy(del_name,uch_name);
  strcpy(del_name,uch_name);
  strcat(del_name,"igl");
  DeleteFile( del_name);
    Form1-> uch->Caption="               ";
    trackfilename=" /0";
    set_uch=0;
     ras->Visible=false;
    cop->Visible=false;
    udal->Visible=false;
//    fin->Visible=true;
   }
  }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::finClick(TObject *Sender)
{
 delete []beta; beta=NULL;
//sgzap();// коридор
Form1->Close();

}
//---------------------------------------------------------------------------

void __fastcall TForm1::copClick(TObject *Sender)
{     // Переделал копирую uch_name.igl
// .ugl- проектная углограмма как результат графики
// при чтении варианта она формируется и при записи исправленного неважно как
int i;
if( !set_uch) {ShowMessage("Не выбран участок");return;}
sgzap();// коридор
char uch_name_old[20]; char copy_name[20];
strcpy(uch_name_old,uch_name);
AnsiString trackfilename1 = InputBox("Введите имя нового участка:","","");
strcpy(uch_name,  trackfilename1.c_str());//новое имя
//AnsiString asdfilename1=trackfilename1+".asd"; не нужен ASD
AnsiString trackfilename2=trackfilename1+".tra";
if(CopyFile( trackfilename.c_str(), trackfilename2.c_str(),0))
     {Form1-> uch->Caption="            "+trackfilename1;
     trackfilename=trackfilename2;
      strcpy(copy_name,uch_name);//новое cохраняется!!!
 f_copy( copy_name,uch_name_old,'e');//портит имена старое и текущее
     }else {ShowMessage("Не скопировали");return;}
 //при копировании с нулём в существующий файл он заменяется
i=strlen(uch_name_old);
uch_name_old[i-3]='\0';
strcat(uch_name_old,"igl");
i=strlen(copy_name);
copy_name[i-3]='\0';
strcat(copy_name,"igl");
CopyFile(uch_name_old ,copy_name,0);
Form1-> uch->Visible=true;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button8Click(TObject *Sender)
{ //возврат от работы с участком

chkor=1;
sgzap();// коридор
Gr2->Visible=false;
vibor->Visible=true;     //другие закрываютя под Gr2 а этот не хочет
var->Visible=false;
uch->Caption="    ";
sgTrack->Visible=false;
set_uch=0;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::rasClick(TObject *Sender)
{ //Расчёт участка
char sig0;
   int gr0,mi0; double se0;

if( !set_uch) {ShowMessage("Не выбран участок");return;}
sgzap();// коридор
//Есть массив tr : tr->dist-хорда, tr->angle- угол поворота этой хоры в конце её
//  Массив .tra читаем  в readtrace

//RG->Enabled=true;
RG->Visible=true;
 sgTrack->Visible=false;
Button1->Enabled=false;
 Button8->Enabled=false;

double a,b,c,x2,y2,p,S,R,d;


// Form4->Button1->Visible=0;
    Gr2->Left=0;
    Gr2->Top=50;
    Gr2->Visible=true;
    vibor->Visible=false;//другие закрываютcя под Gr2 а этот не хочет

}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button1Click(TObject *Sender)
{
  //выбор варианта

    char vib[50];
FManFiles->Tag=2;
   if(set_var) delete elts;//на случай наличия старого варианта в памяти

  strcpy(vib,"\\");
  strcat(vib,uch_name);
  strcat(vib,".e??");
   spisok(vib,2);
    FManFiles->ListBox1->ItemIndex = 0;
    FManFiles->Caption="Выбор варианта";
    FManFiles->Label2->Caption=" Номера вариантов";
    FManFiles->Tag=2;
   FManFiles->ShowModal();//даёт имя файла в elt_name  как char*
   Button2->Visible=true;
   Button3->Visible=true;
   Button4->Visible=true;
   Button5->Visible=true;
   Button6->Visible=true;
   Button7->Visible=true;
/*
 if(FileExists( elt_name))
   {
     read_elts(elt_name);// читает элементы из файла в новый массив
     elts_show();//в форму
    // evolvents();
  //  sgzap();// коридор
//     new_shifts();     //  по эвольвентам это плохо!!!!
     //write_trace( trackfilename.c_str());
     trace_show(); //заносит в форму данные из массива tr
     el->sgEl->Visible=true;
   //  el->sgEl->Enabled=true;
   el->Close();  //Можно закрывать закрытую
     el->ShowModal();
   } // else//создать файл fort.txt
  */  //чтение nu  временно для отладки здесь
  //но потом перенесть после вызова INITIAL

  if(!FileExists( elt_name))
  {
FILE *fp;
  fp=fopen("fort.txt","wt");
  tr[0].dist=0.;   //важно!!!
   xc=yc=0; ugl1=0;//в оптимизации всe с нуля в пределах круга
 //
  //запись как  и ранее в системе съёмки но дополнитель файл nach_ugl.txt

 fprintf (fp,"\n%5d",n_pts);

for(int i=0;i<n_pts;i++)
{
    xc+= tr[i].dist*cos(ugl1);
    yc+= tr[i].dist*sin(ugl1);
 if(i==0)
   {//длину слева сделал нулевую
    gam=ugl1;// начальный угол по хорде принят за нуль он и записан
//в начале не нужен поперечник, а угол с осью х принят нулём
//отдельно в файле nach_ugl.txt записаны x,y, ugl относительно
//системы съёмки, то есть разности проектные и натуры
// при начале с кривой пересчитал на касательную, но система координат
// по хорде , в этом случае надо иметь нулевую длину прямой и клотоиды
   }
  else
 {  //здесь i>0
        if(i==n_pts-1)
       { tk=1; phi=0.;}
     else
    {

    tk=tr[i+1].dist/tr[i].dist;
   phi=tr[i].angle;
     }
   gam=ugl1+phi/(1+tk)+3.1415926/2;
     //gam-от центра кривизны к вершине или наоборот но всегда
//в положительном направлении против стрелки(влево)
//углы от центра к вершине и к середине хорды относятся как хорды
 //их сумма равна phi Это угол поворота хорды
 //== касательной по центру дуг

   }  //конец else к i==0
// fprintf(file,"\n%12.3lf %12.3lf %12.8f",xc,yc,gam);
   //добавка угла после вычисления нормали
   //поэтому там угол слева от хорды!!
     ugl1+= tr[i].angle;
//fprintf (fp,"\n%10.3lf%10.3lf%12.3lf%12.3lf%12.8f",shmin,shmax,xc,yc,gam);
fprintf (fp,"\n%16.8lf%16.8lf%12.8f",xc,yc,gam);
}//for
           fclose(fp);
 char put[80];
//GetCurrentDirectory(128, put); // полный путь к KD3
// fort.txt x,y, beta-записан в системе
//с началом в нуле и углом нуль то есть ось ОХ по первой хорде
//beta вычислены через углы поворота хорд т.е. это нормали
//повёрныты как и вся картинка Началный угол ugl не работает и
//выплывет только при восстановлении в старой системе при печати
//при проектировании учтены добавки dx,dy, dugl  в nach_ugl.txt
//верно ли учтены доьавки??? как в INITIAL так и в оптимизаторе
 //поправки переведены в систему оптимизации там верно! В UNITIAL?
GetCurrentDirectory(128, put);

 strcpy(put,"..//INITIAL.exe  ");//работает и без ???..\\
    //нет номера и файла варианта номер сначала
   //надо выбрать как в старом тексте
 strcat(put, elt_name); //какой здесь  elt_name  KD3.e77
  ShowMessage(put); // до выбора варианта вызвать нельзя нет в меню
// WinExec(put,SW_SHOW);
vizov(put);  //INITIAL c   elt_name
 // в файле nach_ugl.txt изменённый угол как новая поправка к. tra
 //GetCurrentDirectory(128, put);
 Application->ProcessMessages();
 FILE *nu; double dx,dy,dugl;//
 nu=fopen("nach_ugl.txt","rt");
 fscanf(nu,"%lf",&dx);
 fscanf(nu,"\n%lf",&dy);
 fscanf(nu,"\n%lf",&dugl);
 uglpr=ugl+dugl;
 fclose(nu);
 
   }//else  нет
    if(FileExists( elt_name))
   { razpech=0;
     read_elts(elt_name);// читает элементы из файла в новый массив
     elts_show();//в форму
    // evolvents();
  //  sgzap();// коридор
//     new_shifts();     //  по эвольвентам это плохо!!!!
     //write_trace( trackfilename.c_str());
     trace_show(); //заносит в форму данные из массива tr
     el->sgEl->Visible=true;
   //  el->sgEl->Enabled=true;
   el->Close();  //Можно закрывать закрытую
     el->ShowModal();
   } // else//создать файл fort.txt
    else ShowMessage ("Файл элементов не создан");

}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button3Click(TObject *Sender)
{
//удаление варианта файла  и массива и из списка надо удалять!!!
if(!set_var){ShowMessage("Не выбран вариант"); return;}

  if( Application->MessageBox(num_var,"Подтверждение удаления варианта",  MB_YESNO + MB_ICONQUESTION ) == IDYES )
  {     delete elts;//удаление массива
      if(  DeleteFile( eltfilename  ))ShowMessage(" Удалён"); //удаление файла
        set_var=0;
        var->Visible=false;
        Button2->Visible=false;
   Button3->Visible=false;
   Button4->Visible=false;
   Button5->Visible=false;
   Button6->Visible=false;
   Button7->Visible=false;

   }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button2Click(TObject *Sender)
{  //Копирование вариантa добавить файл *.g??
char a;    char newname[80];char elt_name1[80],newname1[80];
if(!set_var){ShowMessage("Не выбран вариант");return;}
AnsiString name1 = InputBox("Введите номер нового варианта:","","");
// ввели номер 1 или 10 или 88
strcpy(num_var,name1.c_str());// получил строку char
if(strlen(num_var)<2)
{a=num_var[0];num_var[0]='0'; num_var[1]=a;}
//сделал двузначный номер

strcpy(newname,elt_name);
int i=strlen(newname);
   strcpy( elt_name1,elt_name);
     elt_name1[i-3]='g';
newname[i-2]=num_var[0]; newname[i-1]=num_var[1];

if(CopyFile( elt_name,newname ,1))
     {
strcpy(elt_name,newname);
eltfilename=AnsiString(elt_name);
     Form1-> var->Caption="            "+AnsiString(num_var);


     if(FileExists(elt_name1))
     {
     strcpy( newname1,newname);
     newname1[i-3]='g';
    CopyFile( elt_name1,newname1 ,1);
      }
        el->Close();  //Можно закрывать закрытую
        el->ShowModal();
     }
// массив elts остался от старого варианта при повторном выборе
//  он создаётся при чтении файла копировать не куда

}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------

void __fastcall TForm1::sgTrackClick(TObject *Sender)
{
  chkor=1;
  if(sgTrack->Col<2)sgTrack->Col=2;
 if(sgTrack->Col==4)sgTrack->Col--;
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------

void __fastcall TForm1::varClick(TObject *Sender)
{
if (set_var) el->ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button4Click(TObject *Sender)
{
//Углограмма ShowMessage(elt_name);  Где строится углограмма по сущ. пути???

  char put[80];
 strcpy(put,"Grafika.exe ");
 strcat(put, elt_name);
  //ShowMessage(put);  до выбора варианта вызвать нельзя нет в меню
// WinExec(put,SW_SHOW);
vizov(put);
//пишет новые *.e??  и * .g??  не ловит отличие начального проектного угла!!!!
if(FileExists( elt_name))
{
 read_elts(elt_name);// читает элементы из файла в новый массив
// opt_past_ugl- здесь формирование fortxy1.txt исправил xc,yc на  16.8
//и вызов оптимизатора на один проход  и формирование нового массива tr

  opt_past_ugl();

//   elts_show();//в форму
 // write_trace( trackfilename.c_str());
  //   sgzap();// коридор ->tr+write_trace
   //   trace_show();// в форму из tr
 }else ShowMessage("Нет файла элементов после Углограммы");
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button5Click(TObject *Sender)
{  //На FORM1  кнопка Оптимизация
//Оптимизация готовит для фортрана и вызывает его
 chkor=1;//не срабатывает присваивание по sgTracClick
 sgzap();// коридор-> tr + write_trace работает с fp!!!
 FILE *fp;
 FILE *fp1;//для bufxy1 нельзя дважды использовать fp
 int i, pr;// признак наличия доп ограничений
  pr=0;
if(!newvar && razpech!=0) //не новый вариант от INITIAL и только что выбран
 {
el->Close();
el->dop->Visible=true;
el->dop->Caption=" Ширина полосы";
el->numel->Visible=true;
el->numel->Text=FloatToStr(deo);
el->Button2->Visible=1;
el->Button3->Visible=1;
el->ShowModal();
//el->Tag 0 при не записывать и 1 при записать в форме el при старт ==1
if(el->Tag==1)
deo=Asd(el->numel->Text);//преобразует Text c , в double
else return;

 if(MessageDlg("Нужны доп. ограничения?",mtInformation,TMsgDlgButtons() << mbYes<<mbNo,0)==mrYes)
{
el->kon->Visible=true;
el->dop->Caption=" № эл-та  Пикет  Плюс ";
el->numel->Text=IntToStr(numper);
el->pic->Visible=true;
el->pl->Visible=true;
el->Button2->Caption="Продолжить";
el->ShowModal();
//el->Tag 0 при не записывать и 1 при записать в форме el   при старт ==1
el->Button2->Caption="Старт";
el->pic->Visible=false;
el->pl->Visible=false;
el->kon->Visible=false;

 el->dop->Visible=false;
 el->numel->Visible=false;

if(el->Tag==0)return;//не записывать доп. ограничения
else  //записать   доп ограничения
  {
pr=1;
numper=StrToInt(el->numel->Text);
el->numel->Text=" ";
picet=StrToInt(el->pic->Text);
plus=Asd(el->pl->Text);
   }//Tag==1 записать
 }//нужны ограничения  конец возни с ними

 el->Button2->Visible=0;  //старт
  el->Button3->Visible=0;//возврат
  }else// новый вариант или только что выбран
 deo=-0.5;
 //оптимизатор запишет результат в bufxy1.txt потом вызову углограмму
 //с  uсhname.ugl и при записи получу элементы в файле потом
 // new_shifts  и всё показать
 // не получилось нет пикетов в оптимизаторе
   //delete ima;
    if(FileExists("bufxy1.txt"))
 {
 i= remove("bufxy1.txt");
if(i)ShowMessage("Не удалён bufxy1.txt");
  }
 fp=fopen("fortxy1.txt","wt");
if(fp==NULL){ ShowMessage("Не открывает для записи fortxy1.txt"); exit(0);}
fprintf (fp,"%5d",pr);
if(pr)
{
double km;
double pl;//float
pl=plus;
 pic_to_km(  picet,  pl, &km);// начальная абсцисса
 km=km-per5;
fprintf (fp,"%5d%10.2lf%10.2lf",numper,km,km+0.001);
 } //конец записи доп ограничений
 else
 fprintf (fp,"%5d%10.2lf%10.2lf",0,0.0,0.0);
 //записано вместо доп ограничения
 //далее в тот же fp=fortxy1 запись
 double shmin,shmax,ksi,eta,xc,yc,ugl1,gam,tk,phi;
  tr[0].dist=0.;   //важно!!!
   xc=yc=0; ugl1=0;//в оптимизации всe с нуля в пределах круга
 //
 //запись как  и ранее в системе съёмки но дополнитель файл nach_ugl.txt
  i=n_pts;
  if(deo<0.0001)i=-n_pts;
 fprintf (fp,"\n%5d%5d",i,n_elts);

for( i=0;i<n_pts;i++)
{
shmin=tr[i].min_shift; shmax=tr[i].max_shift;
if(-deo>shmin)shmin=-deo;
if(deo<shmax) shmax=deo;
//if(i==n_pts-1){shmin=-0.0001;shmax=0.0001;}
//при чтении трассы их делал 0.0015 в фортране точность 0.004
    xc+= tr[i].dist*cos(ugl1);
    yc+= tr[i].dist*sin(ugl1);
 if(i==0)
   {//длину слева сделал нулевую
    gam=ugl1;// начальный угол по хорде принят за нуль он и записан
//в начале не нужен поперечник, а угол с осью х принят нулём
//отдельно в файле nach_ugl.txt записаны x,y, ugl относительно
//системы съёмки, то есть разности проектные и натуры
// при начале с кривой пересчитал на касательную, но система координат
// по хорде , в этом случае надо иметь нулевую длину прямой и клотоиды
   }
  else
 {
        if(i==n_pts-1)
       { tk=1; phi=0.;}
     else
    {

    tk=tr[i+1].dist/tr[i].dist;
    phi=tr[i].angle;
     }
   gam=ugl1+phi/(1+tk)+3.1415926/2;
     //gam-от центра кривизны к вершине или наоборот но всегда
//в положительном направлении против стрелки(влево)
//углы от центра к вершине и к середине хорды относятся как хорды
 //их сумма равна phi

   }  //else
// fprintf(file,"\n%12.3lf %12.3lf %12.8f",xc,yc,gam);

     ugl1+= tr[i].angle;

///
//fprintf (fp,"\n%10.3lf%10.3lf%12.3lf%12.3lf%12.8f",shmin,shmax,xc,yc,gam);
fprintf (fp,"\n%10.3lf%10.3lf%16.8lf%16.8lf%12.8f",
         shmin,shmax,xc,yc,gam);
}//for

//++++

//  добавил запись нач данных,вычисление и запись
// в новый файл x,y и углов
  //направлений к центру кривизны с осью х поворот влево- плюс

ksi=tr[n_pts-1].ev_x-min_l_last;
eta=tr[n_pts-1].ev_y-phifin*min_l_last;
fprintf (fp,"\n%20.8lf%20.5lf%20.5lf",ugl1,ksi,eta);
fprintf (fp,"\n%12.6lf%12.6lf%12.6lf",min_l_first,l_first,max_l_first);

for(int i=0;i<=n_elts;i++)
{
if(elts[i].l_clot<0.00001)
fprintf (fp,"\n%12.6lf%12.6lf%12.6lf",0.000,0.000,0.000);
else
fprintf (fp,"\n%12.6lf%12.6lf%12.6lf",
elts[i].min_l_clot,elts[i].l_clot,elts[i].max_l_clot);
if(i==n_elts) break;
fprintf (fp,"\n%12.6lf%12.6lf%12.6lf",
elts[i].min_l_circ,elts[i].l_circ,elts[i].max_l_circ);
if(fabs(elts[i].rad)<0.0001)
fprintf (fp,"\n%20.8lf%20.8lf%20.12lf",0.0,0.0,0.0);
else
{
shmin=1.0/elts[i].max_rad;
shmax=1.0/elts[i].min_rad;
  if(elts[i].rad<0.0)
   {
   shmin=-1.0/elts[i].min_rad;
   shmax=-1.0/elts[i].max_rad;
   }
fprintf (fp,"\n%20.12lf%20.12lf%20.12lf",shmin,1.0/elts[i].rad,shmax);
}//было в фортране если мин и мах радиусы равны то кривизна нуль???
}//for
// последняя клотоида записана в цикле
//этот кусок оформить как функцию и вызывать её после углограммы, но deo?
//функция с параметром здесь с нулём, а там с 1, то есть там deo нуль !!!
   fclose(fp); //fortxy1
 //  rewind (fp);
 // exit(0);
// el->Close();
    if(FileExists( "ERROR.txt"))remove("ERROR.txt");
vizov("reorxy1.exe");
//При чтении пустого файла ничего не читается.
// Но s2 остаётся нулём и радиусы нули
if(newvar){newvar=0;deo=0.5;}//если и был новый, то он уже старый
 if(!razpech){razpech=1;deo=0.5;}// теперь можно его напечатать
fp1=fopen("bufxy1.txt","rt");
double s,s2;
 double s1; //float s1;
s=per5;
 i= fscanf(fp1,"%lf",&l_first);
if(i!=1){ShowMessage("Оптимизация не выполнена. Ошибки");
         fclose(fp); fclose(fp1);
         if(FileExists( "ERROR.txt"))
    {ShowMessage("Предупреждения и ошибки при оптимизации");
    vizov("..//AkelPad ERROR.txt");
    }
         return;
         }
 razpech=1;// теперь можно его напечатать
 s=s+l_first;

 for(int i=0;i<=n_elts;i++)
  {
   km_to_pic(s,&elts[i].cl_ch0.chain,&s1);
   elts[i].cl_ch0.plus=s1;
    if(i<n_elts)
   fscanf(fp1,"%lf%lf%lf",&elts[i].l_clot,&elts[i].l_circ,&s2);
  else fscanf(fp1,"%lf",&elts[i].l_clot);
  s=s+elts[i].l_clot;
  km_to_pic(s,&elts[i].cl_ch1.chain,&s1);
  elts[i].cl_ch1.plus=s1;
  if(i==n_elts) break;
   if(fabs(s2)<1.e-8) elts[i].rad=0.0;else elts[i].rad=1.0/s2;
   s=s+elts[i].l_circ;
  }//for i прочли из buf новые длины элементов там ещё рихтовки
   elts_show();//занести новые элементы в форму
   write_elts (elt_name);//записать новые элементы в файл + formugl()!!
//пишем независимо от нажатия кнопки записать
   //вычисляю рихтовку в начальной точке

   //
    for(int i=0;i<n_pts;i=i+5)
{
    for (int j=0;j<5;j++)
    { if(i+j<n_pts)
      fscanf(fp1,"%lf",&tr[i+j].shift);else break;
    }//for j
}//for i запись рихтовок в массив tr
   if(beta==NULL)beta=new double [n_pts];
 for(int i=0;i<n_pts;i=i+5)
{
 for (int j=0;j<5;j++)
    { if(i+j<n_pts)
      fscanf(fp1,"%lf",&beta[i+j]);else break;
    }//for j  была ошибка fp а не fp1
}//for i чтение новых  углов нормалей
   tr[0].shift=sqrt((xl-xlpr)*(xl-xlpr)+(yl-ylpr)*(yl-ylpr));
    trace_show(); // новые рихтовки в форму
    write_trace(trackfilename.c_str());
    formugl();//формирует файл .g??
    //el->MClose->Caption="Повтор";
    //el->Button1->Caption="Возврат";
//    el->ShowModal();
    el->Show();//чтобы смотреть рихтовки
//    el->MClose->Caption="Записать";
//el->Button1->Caption="Не записвыть";
    // здесь по кнопке Повтор снова запись только при редактирования
//    возможно для перехода к анализу рихтовок вызвать без Modal
  //fclose(fp); закрыл выше
    fclose (fp1);
    if(FileExists( "ERROR"))
    {ShowMessage("Предупреждения и ошибки при оптимизации");
    vizov("..//AkelPad ERROR");
    }// else ShowMessage (" При оптимизации не было предупреждений");
}
//---------------------------------------------------------------------------
//void __fastcall TForm1::sgTrackExit(TObject *Sender)
void sgzap()
{
 if(chkor)
   {
//if(MessageDlg("Записать коридор?",mtWarning,TMsgDlgButtons() << mbYes<<mbNo,0)==mrYes)
//{
for (int i=1;i<Form1->sgTrack->RowCount;i++)
{
tr[i-1].min_shift=Asd(Form1->sgTrack->Cells[2][i]);
tr[i-1].max_shift=Asd(Form1->sgTrack->Cells[3][i]);
}// for
   write_trace( trackfilename.c_str());
 //} // if (Mess...
 chkor=0;
 } //if chkor
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button6Click(TObject *Sender)
{ if(razpech) print_trace_p(); //были в оптимизаторе
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button7Click(TObject *Sender)
{
Form5->ShowModal();
// ShowMessage();  //до выбора варианта вызвать нельзя нет в меню

}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------

void __fastcall TForm1::RGClick(TObject *Sender)
{
char sig0;
   int gr0,mi0; double se0;
sgTrack->Visible=false;
Button8->Enabled=false;
double a,b,c,x2,y2,p,S,R,d;
  c=0.0;//поправка к азимуту за касательную
  xlpr=xl; ylpr=yl; uglpr=ugl; // прочитаны из .tra read_trace
   //запомнил так как из формы идут xl,yl,ugl, надо восстанавливать
  CONV_GRAD(ugl, sig0,gr0,mi0,se0); // высвечиваю по хордам пусть меняют
  if(sig0==1) Form4->sign->Text="-";else Form4->sign->Text="+";
   Form4->gr->Text= AnsiString(gr0);
  Form4->min->Text= AnsiString(mi0);
  Form4->sec->Text= AnsiString(se0);
   Form4->X0->Text= AnsiString(xl);
   Form4->Y0->Text= AnsiString(yl);
               Form4->Tag=0;
 switch(RG->ItemIndex)
 {
case 0://вручную
{
//Form4->Button1->Visible=0;
// всегда даёт xl,yl,ugl;
//

 Form4->Caption="Скорректируйте начальную точку и азимут";
                Form4->Tag=0;
Form4->ShowModal();
if(Form4->ModalResult!=mrOk)
// При mrOk=записать идёт преобразование в радианы
{   Form4->Tag=0;
ShowMessage ("Начальные проектные данные не заданы");
//Form4->Close();
//Form4->Caption="Задайте (проверьте) начальные данные по существующей трассе";
  //  return;
}  else //OK
{
   // получил новые проектные, если они не менялись учесть добавку С
// старые xlpr,ylpr,uglpr, новые xl,yl,ugl

Form4->Caption=" Начальные точка и азимут";
}

break; //вручную
}
case 1://хорда
{
Form4->Caption=" Начальные точка и азимут по двум точкам";
Form4->ShowModal();
/*
if(Form4->ModalResult!=mrOk)
// При mrOk=записать идёт преобразование в радианы
{   Form4->Tag=0; //Form4->Close();
ShowMessage ("Начальные данные по существующей трассе не записаны");
    return;
    */
    Form4->Caption=" Начальные точка и азимут";
}
break;

case 2://касательная
{
   ShowMessage("касательная");//добавил за касательную
 a=tr[1].dist;
b=tr[2].dist;
x2=xl+a*cos(ugl);
y2=yl+a*sin(ugl);
c=ugl+tr[1].angle;
x2=x2+b*cos(c);
y2=y2+b*sin(c);
c= sqrt((xl-x2)*(xl-x2)+(yl-y2)*(yl-y2));
 p=(a+b+c)/2;
 d=p*(p-a)*(p-b)*(p-c);
 if(d>1.E-12) S=sqrt(d); else S=0.0;

 if(S>0.001)
 {
 R=a*b*c/S/4;
 if(tr[1].angle<0)R=-R;
 c=asin(a/2./R);// угол между касательной и хордой   добавка к хорде!!
 } else c=0.0;
 //была ошибка при нулевом угле с оставался длиной при S=0
 Form4->Caption=" Начальные точка и азимут по трём точкам";
ugl=ugl-c;
 CONV_GRAD(ugl, sig0,gr0,mi0,se0); // высвечиваю по касательной пусть меняют
  if(sig0==1) Form4->sign->Text="-";else Form4->sign->Text="+";
   Form4->gr->Text= AnsiString(gr0);
  Form4->min->Text= AnsiString(mi0);
  Form4->sec->Text= AnsiString(se0);
   Form4->X0->Text= AnsiString(xl);
   Form4->Y0->Text= AnsiString(yl);

 Form4->ShowModal();
    Form4->Caption=" Начальные точка и азимут";
    // В форму давал по касательной, получил новый проектный
// Если не корректировали, то получится старый
break;
}
}//switch
//if(fabs(uglpr-ugl)>0.000001) c=0.0;
   // задали новый проектный и поправка не нужна
// В  uglpr- старый из .tra, ugl- новый ( после формы)

   a=xl; xl=xlpr; xlpr=a;
   a=yl; yl=ylpr; ylpr=a;
   a=ugl; ugl=uglpr; uglpr=a;
// старые xlpr,ylpr,uglpr, новые xl,yl,ugl
 // восстановил старые  из формы приходят xl,yl,ugl
 //при изменении угла новый uglpr
FILE *nu;
 nu=fopen("nach_ugl.txt","wt"); //поправки: новый-старый
 /*
 fprintf(nu,"%15.6lf",xlpr-xl);
 fprintf(nu,"\n%15.6lf",ylpr-yl);
 fprintf(nu,"\n%12.8lf",uglpr-ugl);
 */ // нужны координаты точки ( проектное начало) в повёрнутой системе
 a=(xlpr-xl)*cos(ugl)+(ylpr-yl)*sin(ugl);
 b=(ylpr-yl)*cos(ugl)-  (xlpr-xl)*sin(ugl);
 //hao pu временно
// a=0.0; b=0.03907687;uglpr=ugl+0.00232425;
 //  xlpr=xl+b*sin(ugl);ylpr=yl+b*cos(ugl);

 //конец хао пу
 fprintf(nu,"%15.6lf",a);
 fprintf(nu,"\n%15.6lf",b);
 fprintf(nu,"\n%12.8lf",uglpr-ugl);

 //в файле всё в новой системе
 //если меняли угол то uglpr- новый ,
 // в файл идёт поправка проектанта или за касательную.
 //Если не меняли то с поправка на касательную идёт в файл

 // и в оптимизацию и там работает но углы поперечников и съёмка по хордам
 // при печати
//   uglpr=uglpr-c; вернулся проектный поправка уже учтена
 // для печати по элементам съёмка по старому и печать по рихтовкам


 fclose(nu);

 RG->ItemIndex=-1;
 //RG->Enabled=false;
 RG->Visible=false;
 sgTrack->Visible=true;
 Button1->Enabled=true;
 Button8->Enabled=true;
 }
//---------------------------------------------------------------------------

