//---------------------------------------------------------------------------
#include <string.h>
#include <vcl.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#pragma hdrstop
#include "Unit1.h"
#include "Unit5.h" //только своё

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfCorners *fCorners;
char str[80];
void mynew_otpr(double x0,double y0,double x1,double y1,double a,double b,
                double &x,double &y)
   { double  s,cos,sin;
    s=sqrt((x0-x1)*(x0-x1)+(y0-y1)*(y0-y1));
    cos=(x1-x0)/s; sin=(y1-y0)/s;
   x=(a-x0)*cos+(b-y0)*sin;
   y=-(a-x0)*sin+(b-y0)*cos;
   return ;
 /*
   x0,y0,x1,y1-точки в начале и конце прямой,
   a,b-точка вне прямой;x,y-её координаты в системе с центром
   в x0,y0, повёрнутой на alf; y-расст от прямой (сдвижка)
   по нормали( влево +), x- расст. проекции точки от начала
   */
      }//mynew_otpr
   int Lev;
    char st[30],st1[30],st2[30];
  long n_pts,i,j,k,imin,imax; int nzap;
  int jf=0;
   double pk1,x,y,x0,y0,x1,y1,s,s1,s2,s3;
   double RN,SIGN,Lmin,Lk,dH;
   double PI=3.1415926;
  FILE* fin,*fin1,*fp,*fp1;
 double krt(double& x1,double& y1,double& x2,double& y2,double& x3,double& y3);
//
 typedef struct
  {
  //long pk;
  double s,x,y,ugl,sig;
  }PLAN;
  PLAN*plan;
  struct DEFECT
  {  long nach,kon;
     double Rdef,d,L;//L-длина
   // Rdef= дефектный (>RN)радиус,defR- RN минус радиус
  };
   DEFECT *def,*defout;
 long ndef;//число участков с такими радиусами

  double otkl(long in,long i,double &s1 )
    {
       double  cos,sin;
     s1= plan[i].s-plan[in].s;  //оформить участок
     s1=s1-(plan[in+1].s-plan[in].s)/2.-(plan[i].s-plan[i-1].s)/2.;
   //длина с уточнением
        //проверка по отклонениям
         s2=10.; s3=-10.;
       //fprintf(fp,"\n i=%ld,nnuch=%ld,nkuch=%ld,dlina s1=%7.2lf",
      //            i,nnuch,nkuch,s1);
    // if(s1>Lmin)//длинный участок большого радиуса
      x0=plan[in].x;y0=plan[in].y;
      x1=plan[i].x;y1=plan[i].y;
    s=sqrt((x0-x1)*(x0-x1)+(y0-y1)*(y0-y1));
    cos=(x1-x0)/s; sin=(y1-y0)/s;
        for(long j=in+1;j<i;j++)
        {
 //   x=(plan[j].x-x0)*cos+(plan[j].y-y0)*sin;
      y=-(plan[j].x -x0)*sin+(plan[j].y-y0)*cos;
      if(y<s2){s2=y; imin=j;}; //min
      if(y>s3){s3=y; imax=j;}  //max
        }//for j
      s=fabs(s3); if(s<fabs(s2))s=fabs(s2);
     return s;//макс отклонение от прямой от in до i
      }//otkl

    void  oform(long in,long i)

  {
     s=otkl(in,i,s1); //s1 длина  с поправкой
   if(s>dH)  //отклонение больше допуска
     if(!Lev)return; else
     {//могла быть стыковка  Lev==0 в начале или после недопустимого эл=та
      if(ndef)
        {if (def[ndef-1].L<Lmin){ndef--;Lev=0; }
        }//ndef>0
       return;
     }//end Lev>0
    //сюда при  участок  годен по отклонениям от хорды
   //общий радиус не нужен но пока вычислю
          //участок годен но не с чем стыковать
          def[ndef].nach=in;def[ndef].kon=i;
         def[ndef].d=s;
          s2= plan[i-1].ugl-plan[in].ugl; //угол исходящий
          if(fabs(s2)<1.e-8)
           {
             def[ndef].Rdef=RN;
           } else
            {def[ndef].Rdef=s1/s2;} //общий  радиус по участку
            def[ndef].L=plan[i].s-plan[in].s; //s1;//длина
 /*Суммарный радиус не нужен! На участке все радиусы большие.
  Длина может увеличиться при объединении. Выбрасывать не за радиус ,
  а за отклонения
 */
  if(!Lev) //участок пригоден его оформили
    {
       if(i<n_pts-2){Lev=1; ndef++;}
          else
                {if(def[ndef].L>=Lmin) ndef++;}
      return;
     }//оформил участок под будущую проверку и стыковку
            // if(  fabs(def[ndef].Rdef)>RN)ndef++;//не меняя ndef
  else //Lev!=0  стыковать если не далеко от предыдущего
    { if(plan[def[ndef].nach].s-plan[def[ndef-1].kon].s<Lk-0.001)
   {  //0-не стыкуется
    j=def[ndef-1].nach;
    s=otkl(j,i,s1); //на общий участок
    if (s<dH)
      {  //def[ndef].nach=def[ndef-1].nach; левый участок стыкуем
         def[ndef-1].kon=i;
           def[ndef-1].L=plan[i].s-plan[j].s;// s1;
           def[ndef-1].d=s;
           s2= plan[i-1].ugl-plan[j].ugl;
          if(fabs(s2)<1.e-8)
           {
   //        ShowMessage ("Мала разность углов   "+AnsiString(i));
           def[ndef-1].Rdef=RN;
            } else {def[ndef-1].Rdef=s1/s2;} //общий  радиус по участку
       return;// состыковал  но число ndef  осталось
        }
    }
       //   else //присоединение не проходит по dH
     // {// разбираться с каждым отдельно
      Lev=1;  if (def[ndef-1].L>=Lmin)
       {
       if(i<n_pts-2) {
       ndef++; return;} else
      if (def[ndef].L>=Lmin)  {ndef++; return;}
       }
       if (def[ndef-1].L<Lmin)
       { def[ndef-1].L=def[ndef].L; def[ndef-1].d=def[ndef].d;
       def[ndef-1].Rdef=def[ndef].Rdef; def[ndef-1].nach =def[ndef].nach;
       def[ndef-1].kon=def[ndef].kon;
       if(i<n_pts-2 )return;
       if(i>=n_pts-2 && def[ndef].L<Lmin)ndef--; //перекачали короткую надо учесть
      // ndef--;   вопрос почему убрал уменьшение ndef????
       }
        return;
      //}
    }//есть слева участок  Lev!=0
           }//oform

 void Control()
 {  long j=n_pts/2;long i,in,ik;//начало и конец участка >Rn
     //Rn- максимальный радиус
    long imin,imax,nkr,*kkr;//число и границы концов кривых
   Lev=0;// не с чем объединяться слева 1- есть
   fprintf(fp,"\n     Координаты исходных точек + радиусы ");
   fprintf(fp,"\nN точки     Х            У            R");
  for (long i=0;i<n_pts;i++)//i-номер строки; нулевая занята
  { if(!i|| i==n_pts-1)fprintf(fp,"\n%d %15.3lf %15.3lf",
                         i,        plan[i].x ,plan[i].y);
    else
    { s1=plan[i].sig;
       if ( fabs(s1)>1.e-9)
    fprintf(fp,"\n%d %15.3lf %15.3lf %10.3lf",i,plan[i].x ,plan[i].y,1./s1);
     else  fprintf(fp,"\n%d %15.3lf %15.3lf прямая ",i,plan[i].x ,plan[i].y);
     }
  }//for i

  fp1=fopen(st2,"wt");//выходной файл, result +имя сделал при записи норм
  SIGN=1./RN; //с ней сравниваем, если меньше, то прямая
  //номер точки начала поиска очередного участка большого радиуса
    ndef=0;
    def=new DEFECT[j]; //j==n_pts/2
    if(!def){ShowMessage(" Нет памяти для def,j="+AnsiString(j));
         Form1->Close (); exit(0);}
    i=1;
    while (i<n_pts -1)
    {    in=0;
      if(fabs(plan[i].sig)> SIGN){i++;continue;}
      //нашли  большой радиус ищу прямые
      in=i-1;//было in=i; ik=i;
       while (i<n_pts -1) //n_pts-1 это номер последней точки
       {  i++;
         if(fabs(plan[i].sig)<=SIGN)
      { //оформить дефект при достижении конца участка
       if(i<n_pts-2)continue; //дефект продолжен
       oform(in,i);
       i++;break;
      }
 //сюда пришёл при  встрече точки i с большой
 //кривизной любого знака
       oform(in,i);
       i++; break;
       }//while i внутри
     }//while i внешний
     if(!in && ndef && def[ndef-1].L<Lmin)ndef--;
 //fprintf(fp,"\n\nЧисло участков с радиусами >%7.2lf равно %ld",
//RN,ndef);
  if(!ndef)
  {fprintf(fp,"\n  Нет участков c радиусами >=%7.2lf, длиной >=%7.2lf ",RN,Lmin);
  ShowMessage("Нет длинных участков с радиусом >   "+AnsiString(RN));
  fclose(fp); return;
  }   /*
  fprintf(fp,"\n Участки с большими радиусами");
   fprintf(fp,"\nN уч-ка  начало  конец   длина     общ. радиус     отклонение");
      for (long i=0;i<ndef;i++)
   { s1=plan[def[i].kon].s-plan[def[i].nach].s;
   fprintf(fp,"\n%5ld  %5ld   %5ld %8.2lf  %14.3lf  %10.3lf",
   i, def[i].nach,def[i].kon,s1,def[i].Rdef,def[i].d);
   }
       */
 //  fclose(fp);        Form1->Close (); exit(0);
  if(Lk<=0.)fprintf(fp,"\n Участки больших радиусов не стыкуются Lk=%6.2lf",Lk);
 else fprintf(fp,"\n Участки больших радиусов стыкуются при Lk=%6.2lf",Lk);

  if(ndef)
 {  FILE *pr;
    pr=fopen("1pr.txt","wt+");
    //доп. информация номера точек по концам прямых для визуализации
    fprintf(pr,"\n%d",ndef);
  defout=def;
 fprintf(fp,"\n%ld участка с L>%5.0lf м и R>%6.0lf м откл. от прямой <=% 5.2lf м\n" ,
           ndef,Lmin,RN,dH);
 fputs( " =====================================================================\n", fp);
 fputs( " |     Начало           |     Конец            |   Длина    | max от  |\n",fp);
 fputs( " |№ точки|  Километраж  |№ точки| Километраж   |            | прямой  |\n",fp);
  fputs( " =====================================================================\n",fp);
  for (long i=0;i<ndef; i++)
  {    s1=plan[defout[i].nach].s;
   s2=plan[defout[i].kon].s;
      sprintf( str," |%5ld  |%13.3lf |%5ld  |%13.3lf |%10.3lf  |%9.3lf|\n",
    defout[i].nach,s1,defout[i].kon,s2,s2-s1, defout[i].d);
     fputs( str, fp );
   fprintf(pr,"\n %10ld %10ld %10.2lf",
                  defout[i].nach,defout[i].kon,defout[i].Rdef);
   }//for i
   fputs( " =====================================================================",fp);
 fclose(pr);
  } else // no ndef
  fprintf(fp,"\n Участков длиннее %7.2lf радиуса больше  %8.2lf не обнаружено",Lmin,RN);

   fclose(fp);      //  Form1->Close (); exit(0);


 if(ndef)
 {
 strcpy(str,fCorners->Edit4->Text.c_str());
  // fprintf(fp1,"\nint=%d,long=%d",sizeof(int),sizeof(long));
  fprintf(fp1,"\n      Объект  %s \n",str);
   if(Lk<=0.)fprintf(fp1,"\n Участки больших радиусов не стыкуются Lk<=%6.2lf м");
 fprintf(fp1,"\n%ld участка с L>%5.0lf м, R>%6.0lf м, откл. от прямой <=% 5.2lf м\n",
           ndef,Lmin,RN,dH);
 fputs( " =====================================================================\n", fp1);
 fputs( " |     Начало           |     Конец            |   Длина    | max от  |\n",fp1);
 fputs( " |№ точки|  Километраж  |№ точки| Километраж   |            | прямой  |\n",fp1);
  fputs( " =====================================================================\n",fp1);
  for (long i=0;i<ndef; i++)
  {    s1=plan[defout[i].nach].s;
   s2=plan[defout[i].kon].s;
      sprintf( str," |%5ld  |%13.3lf |%5ld  |%13.3lf |%10.3lf  |%9.3lf|\n",
    defout[i].nach,s1,defout[i].kon,s2,s2-s1, defout[i].d);
      fputs( str, fp1 );
   }//for ndef
   fputs( " =====================================================================",fp1 );
  } else
  fprintf(fp1,"\n Участков большого радиуса %8.2lf длиной >=%6.2lf не обнаружено",RN,Lmin);
   fclose(fp1);
  char put[80];
  strcpy(put,"AkelPad.exe ");
 strcat(put, st2); //rezult.txt

 WinExec(put,SW_SHOW);
//vizov(put);попробовать через WinExec но без ..// в своей папке

      delete []def;
      //delete[]defout;

   return;
    }; //control



 double krt(double& x1,double& y1,double& x2,double& y2,double& x3,double& y3)
  //даёт кривизну в точке  по трём вершинам
  {   double a,b,c,kr,v;
   a= sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
   b= sqrt((x3-x2)*(x3-x2)+(y3-y2)*(y3-y2));
   c= sqrt((x1-x3)*(x1-x3)+(y1-y3)*(y1-y3));
  //double p,s,v1;
  //p=(a+b+c)/2.0;
//x[1]=a*b*c/(4.0*sqrt(p*(p-a)*(p-b)*(p-c)));//радиус
//  s=sqrt(p*(p-a)*(p-b)*(p-c));
//kr=4.0*sqrt(p*(p-a)*(p-b)*(p-c))/(a*b*c);
 v= (y3-y2)*(x2-x1)-(x3-x2)*(y2-y1);
 //v1=(y3-y1)*(x2-x1)-(x3-x1)*(y2-y1);//совпали
 //удвоенная площадь по векторному произведению со знаком
 kr=2.0*v/(a*b*c);
 /*
 double s1,s2,s3,s4;
    s1=(y2-y1)/a;
    s1=asin(s1);
    s2=(y3-y2)/b;
    s2=asin(s2);
    s3=(a+b)/2./(s2-s1);
 //верно!
     s4=1./kr;
   */
 return kr;
  }//krt

//---------------------------------------------------------------------------
__fastcall TfCorners::TfCorners(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfCorners::FormCreate(TObject *Sender)
{   for (long i=0;i<sgCorners->RowCount;i++)
    for (long j=0;j<sgCorners->ColCount; j++)
sgCorners->Cells[j][i] =' ';//очистка
  sgCorners->Cells[0][0] = " Километраж  ";
 sgCorners->Cells[1][0] = "        X";
 sgCorners->Cells[2][0] = "        Y";
 //fCorners->Show();

 }

       double Asd(AnsiString s)
  {
    char sd[128];
    strcpy(sd,s.c_str());
     for(unsigned int j=0;j<strlen(sd);j++)
   {if(sd[j]==','){sd[j]='.'; break;}}
    return atof(sd);
  }
void __fastcall TfCorners::FormShow(TObject *Sender)
{ //можно исключить функцию если перенести nzap=0 в Unit1 OnClick
 nzap=0;
 fCorners->Caption=" Редактирование и запись имени файла и норм";
  return;
  } //show

void __fastcall TfCorners::Button4Click(TObject *Sender)
// записать нормативов и потом в файл input1.txt после редактирования
{
RN=StrToFloat(Edit1->Text);//max радиус всё свыше - прямые
Lmin= StrToFloat(Edit2->Text);//min длина искомой прямой
Lk= StrToFloat(Edit5->Text);//min длина эл=та трассы
dH=Asd(Edit3->Text); //max откл от прямой
    if(nzap==0)
{
    fCorners->sgCorners->Visible=true;
Button1->Visible=true;
Button2->Visible=true;
Button3->Visible=true;
//Button5->Visible=true;
//Button1->Visible=true;
strcpy(st,fCorners->Edit4->Text.c_str());//исх файл
       if((fin1=fopen(st,"rt"))==NULL)
      {
       if(jf<2){ShowMessage(" Нет такого файла данных. Исправить имя "+AnsiString(st));
        jf++; return;}
        Form1->Close();
        exit(0);//надо дать возможность исправить и продолжить
       } else jf=0;
 strcpy(st1,"middle_");
strcat(st1,st); //st-имя исходного
 strcpy(st2,"rezult_");
strcat(st2,st);
  Button4->Caption="Записать координаты";
        //fin1(st)-исходный файл в нём только
      // старые х, у надо сделать расстояния в fin
  fCorners->Caption=" Редактирование и запись координат Х,У";
  pk1=0.0;  i=0;
  fin=fopen("input.txt","wt"); //в него по 3
   while(1)
   { if(fscanf(fin1,"%lf%lf",&x,&y)!=2)break;
     if(!i){x0=x; y0=y;
  fprintf(fin,"\n %12.5lf %12.5lf %12.5lf",pk1, x,y);
      x=0.0;y=0.0;     }else
            { s1=x;s2=y; x=x-x0; y=y-y0;
            pk1=pk1+sqrt((x-x1)*(x-x1)+(y-y1)*(y-y1));
             fprintf(fin,"\n %12.5lf %12.5lf %12.5lf",pk1, s1,s2);
            }
      x1=x; y1=y;
      i++;
   } //while
   fclose(fin);
  //из исходно fin1(x,y)сделал с пикетами fin==input.txt
   fin=fopen("input.txt","rt");

////
fCorners->sgCorners->RowCount=2;//  число строк
fCorners-> sgCorners->Rows[1]->Clear();
 n_pts=0;//int i;
 i=0;//номер заполняемой строки
   while (1)
   {
   if(fscanf(fin,"%lf%lf%lf",&s1,&s2,&s3)!=3)break;else
   n_pts++;
  i++;
  fCorners->sgCorners->Cells[0][i]=AnsiString(s1);
  fCorners->sgCorners->Cells[1][i]=AnsiString(s2);
  fCorners->sgCorners->Cells[2][i] = AnsiString(s3);
  fCorners->sgCorners->RowCount=i+1;// увеличил число строк
   }//число заполненных строк=n_pts; RowCount на 1 больше
  fclose(fin);
  fCorners->sgCorners->Visible=true;
   nzap=1;
   return;
 }//if nzap==0 записал имя файла,нормативы,сделал файл по 3 для формы
 //записал файл в формy
   if( nzap==1)
   {  //запись координат скорректированных в fin1 из формы
  fin1=fopen("input1.txt","wt");
  fp=fopen(st1,"wt");//middle файл, имя сделал при записи норм
    long j;
    for (long k=1;k<sgCorners->RowCount;k++)
   {
   for (long j=0;j<sgCorners->ColCount; j++)
   {
    if(sgCorners->Cells[j][k].Trim().IsEmpty() )
      {
      ShowMessage("Пустые поля");
      fCorners->Close();return;//Надо дать исправить
      }
    }//j
   }//k
  //  fprintf(fp,"\nN точки  длина        Х              У");
  for (int i=1;i<sgCorners->RowCount;i++)//i-номер строки; нулевая занята
  {
    s1=Asd(sgCorners->Cells[0][i]);
    s2=Asd(sgCorners->Cells[1][i]);
    s3= Asd(sgCorners->Cells[2][i]);
    fprintf(fin1,"\n%12.4lf %15.6lf %15.6lf",s1,s2,s3);
 //   fprintf(fp,"\n%d %12.4lf %15.6lf %15.6lf",i-1,s1,s2,s3);
  }
  n_pts= sgCorners->RowCount-1;
  nzap=0;
  fCorners->Caption=" Расчёт или окончание работы";

  fclose (fin1);
     //очистка
    sgCorners->RowCount=2;
    for(long i = 1; i < sgCorners->RowCount ; i ++)
           sgCorners->Rows[i]->Clear();
            sgCorners->Visible=false;
          fCorners->Button6->Visible=true;//расчёт
           fCorners->Button1->Visible=false;
           fCorners->Button2->Visible=false;
           fCorners->Button3->Visible=false;
           fCorners->Button4->Visible=false;
    //      ModalResult=mrNo;
     //     fCorners->Close();
  }//nzap==1
               // return;
    }

   void __fastcall TfCorners::Button6Click(TObject *Sender)
{         //расчёт
  plan=new PLAN[n_pts];
  if(!plan){ShowMessage(" Нет памяти для plan,n_pts="+AnsiString(n_pts));
         Form1->Close (); exit(0);}
       pk1=0.0;
  fin1=fopen("input1.txt","rt");//из формы после возможной корректировки
  //в имя файла вс начало вставить rezult
    /*  for(int i=0;i<n_pts;i++)
   {
    if(fscanf(fin1,"%lf%lf%lf",&s1,&x,&y)!=3)
     {ShowMessage("Плохой fin1");exit(0);}
      //===
     if(!i){x0=x; y0=y;x=0.0; y=0.0;}else
            {x=x-x0; y=y-y0;
            pk1=pk1+sqrt((x-x1)*(x-x1)+(y-y1)*(y-y1));
            }
      plan[i].s =pk1;
      plan[i].x=x;plan[i].y=y;
      x1=x; y1=y;
   } //for i
   */
        for(int i=0;i<n_pts;i++)
   {
    if(fscanf(fin1,"%lf%lf%lf",&s1,&x,&y)!=3)
     {ShowMessage("Плохой fin1");exit(0);}
      //===
     if(!i){x1=x; y1=y;}else
            {
            pk1=pk1+sqrt((x-x1)*(x-x1)+(y-y1)*(y-y1));
            }
      plan[i].s =pk1;
      plan[i].x=x;plan[i].y=y;
      x1=x; y1=y;
   } //for i
/*  fp==middle isx
 fprintf(fp,"\n\n      ИСХОДНЫЕ ДАННЫЕ В НОВОЙ СИСТЕМЕ\n");
   for(long i=0;i<n_pts;i++)
   fprintf(fp,"\n %ld s=%10.6lf,x=%10.6lf,y=%10.6lf",
             i, plan[i].s,plan[i].x,plan[i].y);
  *///Вычисление углов с ОХ
   // s1 оставит   для первой и 4 ой четверти
  /*
  double PI2=PI/2.;
    s1=asin((plan[1].y -plan[0].y )/plan[1].s);
   x1=plan[1].x -plan[0].x; y1=plan[1].y -plan[0].y;
if(x1<0.){if(y1>0.)s1=PI-s1;else s1=-(PI+s1);}
plan[0].ugl=s1;//нач угол может быть тупым! при движении влево

   for(long i=1;i<n_pts-1;i++)
{
  x1=plan[i+1].x -plan[i].x;y1=plan[i+1].y -plan[i].y;
 s3=plan[i+1].s -plan[i].s;
 s2=asin(y1 /s3);//1 и 4 четверть останутся
 if(x1<0.){if(y1>0)s2=PI-s2; //2-ая четверть как положительный
 else  s2=-(PI+s2);}//3 четверть как отрицательный
 //угол в пределах -PI<s2<0 или 0<s2<PI но он же может быть плюс минус 2PI
 if(s2>s1+PI2){if(s2>s1+PI2+2.*PI || s2-2.*PI<s1-PI2)
 {ShowMessage("Угол поворота > 90"); fclose(fp); exit (0);} else s2=s2-2.*PI;
              }else
  {
  if(s2<s1-PI2){if(s2<s1-PI2-2.*PI ||s2+2.*PI>s1+PI2)
  {ShowMessage("Угол поворота <- 90"); fclose(fp); exit (0);} else s2=s2+2.*PI;}
               }//нет выхода влево за пределы угла поворота
 plan[i].ugl= s2; //
 //угол поворота на элементе по модулю меньше PI/2!!!
  s1=plan[i].ugl;
}
*/
     x1=plan[1].x -plan[0].x; y1=plan[1].y -plan[0].y;
   s1=asin((plan[1].y -plan[0].y )/plan[1].s);
   if(x1<0.){if(y1>0.)s1=PI-s1;else s1=-(PI+s1);}
plan[0].ugl=s1;//нач угол может быть тупым! при движении влево

   for(long i=1;i<n_pts-1;i++)
{
  x1=plan[i+1].x -plan[i].x;y1=plan[i+1].y -plan[i].y;
 s3=plan[i+1].s -plan[i].s;
 s2=asin(y1/s3);
 if(x1<0.){if(y1>0.)s2=PI-s2;else s2=-(PI+s2);}
 //if(x1<0.)s2=PI-s2;else if(y1<0.) s2=s2+2.0*PI;
 plan[i].ugl= s2; //положительный в пределах круга
 //угол поворота на элементе по модулю меньше PI!!!
 //более того, не PI а PI/2!!!
 if(s2-s1>PI)plan[i].ugl= s2-2.0*PI;else
 if(s2-s1<-PI) plan[i].ugl= s2+2.0*PI;
//возможно получение углов больше PI по модулю
   if(fabs(plan[i].ugl-s1)>PI/2.0)ShowMessage("Угол поворота >PI/2");
  s1=plan[i].ugl;
}

/* //зачем вычитать начальный угол
  s2=plan[0].ugl;
  for(long i=0;i<n_pts-1;i++)
  { // s3=plan[i+1].s -plan[i].s;//для теста
   s1=plan[i].ugl-s2;
  //fprintf(fp,"\n %d ugl=%9.6lf,с нуля %9.6lf,s3=%10.6lf",
 //i,plan[i].ugl,s1,s3);
 plan[i].ugl=s1;
  }
 */
 /*
 fprintf(fp,"\n\n dugl");
  for(int i=1;i<n_pts-1;i++)
 fprintf(fp,"\n %d dugl=%9.6lf",
 i,plan[i].ugl-plan[i-1].ugl);
   */
   i=0;
  // fprintf(fp,"\n\n     УГЛЫ и РАДИУСЫ по ЛОМАНОЙ \n");
  //  fprintf(fp,"\n%ld s=%8.2lf,ugl=%9.6lf",i,plan[0].s,plan[0].ugl);
     for (long i=1;i<n_pts-1;i++)
 {plan[i].sig=krt(plan[i-1].x,plan[i-1].y,plan[i].x,plan[i].y,plan[i+1].x,plan[i+1].y);

 s1= plan[i].sig;
 if(fabs(s1)>1.e-7)
  fprintf(fp,"\n%ld s=%8.2lf,ugl=%9.6lf, sig=%9.6lf,R=%12.3lf",
  i,plan[i].s,plan[i].ugl,plan[i].sig,1./s1);else
    fprintf(fp,"\n%ld s=%8.2lf,ugl=%9.6lf,sig=%9.6lf, прямая ",
    i,plan[i].s, plan[i].ugl,plan[i].sig);

  } //end for
   double r,xrd,d1,d2;
 /*  r=25000.;
   for(int i=0; i<20;i++,r+=500)
   {  xrd=200;
      fprintf(fp,"\n");
    for(int j=0; j<20;j++,xrd+=100)
    {if(xrd>r/2.)break;
    d1=r-sqrt(r*r-xrd*xrd/4.);
     d2= xrd*xrd/8./r;
    fprintf(fp,"\ni=%d,j=%d,r=%9.3lf,xrd=%8.3lf,d1=%8.3lf,d2=%8.3lf",
                i,j,r,xrd,d1,d2);
     }//for j
   } //for i
     for (xrd=100.;xrd<1500.;xrd+=100.)
    {

       fprintf(fp,"\n");
       for (d1=0.5;d1>0.01;d1-=0.05)
       {  r=(d1*d1+xrd*xrd/4.)/(2*d1);
   fprintf(fp,"\n  xrd=%7.2lf,d1=%7.2lf,r=%9.3lf",xrd,d1,r);
       }//for d1
    }//for ixrd
*/

    Control();//поиск
      delete [] plan; plan=NULL;
   fclose(fp);fclose(fin1);//
   ModalResult=mrNo;
 /*  очистил при записи координат
  for (int i=0;i<sgCorners->RowCount;i++)
    for (int j=0;j<sgCorners->ColCount; j++)
sgCorners->Cells[j][i] =' ';//очистка

 sgCorners->Cells[0][0] = " Километраж  ";
 sgCorners->Cells[1][0] = "        X";
 sgCorners->Cells[2][0] = "        Y";
   fCorners->Close();
   */
}

//из списка загнали в форму


  void __fastcall TfCorners::Button1Click(TObject *Sender)
{
  //добавить
sgCorners->RowCount++;
 for (long i=0;i<sgCorners->ColCount;i++)
 sgCorners->Cells[i][sgCorners->RowCount-1]="";
 sgCorners->Row=sgCorners->RowCount-1;
   sgCorners->Col=0;
 }
 void __fastcall TfCorners::Button2Click(TObject *Sender)
{
  // Вставить
 sgCorners->RowCount++;
  for(long i=sgCorners->RowCount-1; i>=sgCorners->Row+1; i--)
  {
//    RP->Rows[i]-> Assign(RP->Rows[i-1]);
    sgCorners->Rows[i]=sgCorners->Rows[i-1];
  }
 for (long i=0;i<sgCorners->ColCount;i++)
 sgCorners->Cells[i][sgCorners->Row]="";
 sgCorners->Col=0;
}
  void __fastcall TfCorners::Button3Click(TObject *Sender)
{
// удалить
  long R = sgCorners->Row;
  if(sgCorners->RowCount>1)
  {
  if(sgCorners->Row == sgCorners->RowCount-1) sgCorners->Row= sgCorners->Row -1;

  for(long i=R+1;i<sgCorners->RowCount; i++)
  {
//    RP->Rows[i-1]->Assign(RP->Rows[i]);
    sgCorners->Rows[i-1]=sgCorners->Rows[i];

  }
  sgCorners->RowCount--;
  }//>1

}




//---------------------------------------------------------------------------



//---------------------------------------------------------------------------

void __fastcall TfCorners::Button7Click(TObject *Sender)
{
ModalResult=mrOk;
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------

