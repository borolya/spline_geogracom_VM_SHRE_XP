//---------------------------------------------------------------------------
#pragma hdrstop
#include <stdio.h>
#include<process.h>
#include "stdlib.h"
#include "math.h"
#include <fstream.h>
#include <iostream.h>
#include "Unit1.h"
//#include "Praktika.h"
#include <vcl.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)
//extern  TForm1 *Form1;
 double floor01(double &r);
extern int kdL,kdR; extern double dL; extern double pk0;
 //int kdR,kdL; -количество неудачных шагов до прерывания
 //в optline  много больше чем newprev, newvse
   //int nzapmax;//in perv and vse
 double def1prev,def2prev,def1isx,def2isx;
  double t1isx,t2isx,xvisx,yvisx,xlisx,ylisx,xpisx,ypisx;
 //дефицит перед очереным поворотом
 int pribl;//1-построен первый элемент,0- нет
 double*sdv;
  struct PLAN
  {
   double s,x,y,ugl,sig;
  }*plan;
  struct DEFECT
  {  int nach,kon;
  double Rdef,d,L;//L-длина участка R<Rmax
    //d-отклонение от хорды
  };
  double Rad(int in,int i);
   void  oform1(int in,int i);
   void  oform(int in,int i);
   DEFECT *def,*defout; //массив  кусков с малым  радиусом
 int n_pts,ndef;//число участков с такими радиусами
 int Lev;//0 левее нет прямой для объединения
 extern double dH,mel,maxrob,minLob;
int obrpr(int in,int i);
   double floor01(double &r)
   {
    char st[30];
     if(r>0.)r=r-0.05;else r=r+0.05;
     sprintf(st,"\n%8.1lf",r);
     sscanf(st, "%lf",&r);
     return (r);
   }//floor01
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
double otkl2(int in,int i,double ugl,double &s2,double &s3,int &imax,int &imin,
        double &ugmin,double &ugmax);
   //
    double otkl(int in,int i,int &imax,double &s1 )
    {
       double  co,si;
       double s2,s3,x0,x1,y0,y1,x,y;
       //int imin,imax;
     s1= plan[i].s-plan[in].s;  //оформить участок
     s1=s1-(plan[in+1].s-plan[in].s)/2.-(plan[i].s-plan[i-1].s)/2.;
   //длина с уточнением
        //проверка по отклонениям
         s2=0.;// s3=-100.;
      x0=plan[in].x;y0=plan[in].y;
      x1=plan[i].x;y1=plan[i].y;
    s=sqrt((x0-x1)*(x0-x1)+(y0-y1)*(y0-y1));
    co=(x1-x0)/s; si=(y1-y0)/s;
        for(int j=in+1;j<i;j++)
        {
 //   x=(plan[j].x-x0)*co+(plan[j].y-y0)*si;
      y=-(plan[j].x -x0)*si+(plan[j].y-y0)*co;
      if(fabs(y)>fabs(s2)){s2=y; imax=j;}//max
         }//for j
       return s2;//макс отклонение от прямой от in до i
  }//otkl
 double otkl2(int in,int i,double ugl,double &s2,double &s3,int &imax,int &imin,
        double &ugmin,double &ugmax)
    {
       double  co,si;
       double x0,x1,y0,y1,x,y;
      s2=-100.; s3=100.;
       si=sin(ugl);
       co=cos(ugl);

      x0=plan[in].x;y0=plan[in].y;
    //  x1=plan[i].x;y1=plan[i].y;
     for(int j=in+1;j<i;j++)
        {
      x=(plan[j].x-x0)*co+(plan[j].y-y0)*si;
      y=-(plan[j].x -x0)*si+(plan[j].y-y0)*co;
      if(y>s2){s2=y; imax=j;ugmax=atan(y/x);}//max
      if(y<s3){s3=y; imin=j;ugmin=atan(y/x);}  //min
         }//for j
       return s2;//макс отклонение от прямой от in до i
  }//otkl2
 //

  //++++++
   double Rad(int in,int i)
  {   double b1,s1,s2;
   if(!in)b1=plan[0].ugl+pi/2.;else b1=beta[in];

     s1= plan[i].s-plan[in].s;  //оформить участок
        //s1=s1-(plan[in+1].s-plan[in].s)/2.-(plan[i].s-plan[i-1].s)/2.;
           //  s2= plan[i-1].ugl-plan[in].ugl;
      s2=beta[i]-b1;//угол поворота на кривой
             if(fabs(s2)<1.e-8)
           {
    ShowMessage ("Rad. Мала разность углов"+AnsiString(s2));
 fprintf(f1,"\nБлизкие углы in=%d,i=%d,b1=%9.2lf,b2=%9.2lf",
     in,i,b1,beta[i]);
    //fclose(f1); exit(0);
    return 100000;
            } else
       return s1/s2; // радиус по участку
   }//Rad
    void  oform(int in,int i)
 { double s1,s2;  int j;
   s1=Rad(in,i);//радиус новой кривой
  if(ndef>0) //увязка с последним.
   {//Если порвали на смене знака c предельным R, то перехлёст
   if(in== def[ndef-1].kon-1) in++;//ndef-1-номер последнего
  else
  {   j= def[ndef-1].kon;
 // if((def[ndef-1].Rdef *s1>0)&&(plan[i].s-plan[in].s<lprmin))

  if((def[ndef-1].Rdef *s1>0)&&(plan[i].s-plan[j].s<Lmin)&&fabs(Rad(def[ndef-1].nach,i))<Rmax)
     {
    in=def[ndef-1].nach;  ndef--;
    //объединить близкие участки одного знака
     }
  } //else
   }//ndef>0

          def[ndef].nach=in;def[ndef].kon=i;
          def[ndef].Rdef=Rad(in,i);//in-возможно новый

     ndef++;
       }//oform
 int priamaia(double *x, double *y)
  {
  double x1,y1,s1,s2,s3,SIGN,R,pk1=0.0;
  double PI=3.1415926535897;
  int j,i,in; int *kkr;
 fprintf(f1,"\n dH=%5.2lf",dH);
  plan=new PLAN[n_pts];
 for( i=0;i<n_pts;i++)
  //n_pts- число точек ( узлов) ломаной  n==n_pts-1 -номер последней точки
  {
  plan[i].x=x[i];plan[i].y=y[i];//xt,yt
   if(!i){x1=x[i]; y1=y[i];}else
            {
            pk1=pk1+sqrt((x[i]-x1)*(x[i]-x1)+(y[i]-y1)*(y[i]-y1));
            }
      plan[i].s =pk1;
      plan[i].x=x[i];plan[i].y=y[i];
      x1=x[i]; y1=y[i];
  }//for i

     x1=plan[1].x -plan[0].x; y1=plan[1].y -plan[0].y;
   s1=asin((plan[1].y -plan[0].y )/(plan[1].s-plan[0].s));//plan[0].s===0
   if(x1<0.){if(y1>0.)s1=PI-s1;else s1=-(PI+s1);}
  plan[0].ugl=s1;//нач угол может быть тупым! при движении влево

   for(int i=1;i<n_pts-1;i++)
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
 double *sigb;
     sigb=new double[n_pts];
     for (int i=1;i<n_pts-1; i++)
     {
   sigb[i]=(beta[i+1]-beta[i])/(plan[i+1].s-plan[i].s);
     if(fabs(sigb[i])<1.e-8) sigb[i]=1.e-6;
     }
  for (int i=1;i<n_pts-1;i++)
 {plan[i].sig=krt(plan[i-1].x,plan[i-1].y,plan[i].x,plan[i].y,plan[i+1].x,plan[i+1].y);
  s1= plan[i].sig;
 if(fabs(plan[1].sig)>1./1000.)
 { ShowMessage("Начало на кривой R= "+AnsiString (1./s1));
fclose(f1); exit(0);
 }
  if(fabs(s1)>1.e-8)
  fprintf(f1,"\n%d s=%8.2lf,ugl=%9.6lf, sig=%9.3lf,R=%12.3lf",
  i,plan[i].s,plan[i].ugl,1./sigb[i],1./s1);else
    fprintf(f1,"\n%d s=%8.2lf,ugl=%9.6lf,sig=%9.3lf, прямая ",
    i,plan[i].s, plan[i].ugl,1./sigb[i]);
    plan[i].sig=sigb[i];
  } //end for

//  fclose(f1);exit(0);
  SIGN=1./Rmax;
  ndef=0;
  delete[]sigb;
    def=new DEFECT[n_pts/2];
 //
   i=1;
    int zk=0;
//знак кривизны:1 плюс,-1 минус 0 не было кривой
     while (i<n_pts -1)
    //включая последний узел, n_pts -1-номер последней точки
    {
      if(fabs(plan[i].sig)< SIGN){i++;continue;}//нет кривой
      //нашли малый радиус
     in=i;
     // in=i-1;
      if ( plan[i].sig>0.0)zk=1; else zk=-1;
      // далее до конца кривой или смены знака
       while (i<n_pts -1) //n_pts-1 это номер последней точки
    {  i++;
         if((fabs(plan[i].sig)>=SIGN)&&(plan[i].sig*zk>0.))
       { //оформить кривую при достижении конца участка
         if(i<n_pts-2)continue; //кривая продолжена
         oform(in,i); //дошли до конца
         i++;break;
       } //if
     //сюда пришёл при  большом радиусе в точке i или смене знака
    //без контроля на длину просто занести кусок кривой
     if(i-in>=2)oform(in,i);
   if(plan[i].sig*zk>0.)i++;
  break;
       }//while i внутри
    }//while i внешний
  fprintf(f1,"\nndef=%d",ndef);
  if(!ndef)
  {fprintf(f1,"\n  Нет малых радиусов вообще");
  ShowMessage("Нет малых радиусов вообще"); return 1;
  }
  fprintf(f1,"\n Участки малых радиусов\n");
   for (int i=0;i<ndef;i++)
   { s1=plan[def[i].kon].s;
     s2=plan[def[i].nach].s;
   fprintf(f1,"\ni=%d,nach=%d kon=%d snach=%7.2lf skon=%7.2lf Rdef=%6.2lf",
   i,def[i].nach,def[i].kon,s2,s1,def[i].Rdef);
   }  //  def[i].Rdef,def[i].defR не нужны для поиска стабильных зон
   //Участки найдены, объединять надо
 kkr=new int[ndef];//номера участков по концам кривых, их число nkr
if(!kkr){ShowMessage(" Нет памяти для kkr,ndef="+AnsiString(ndef));
         fclose(f1); exit(0);}
  nkr=0;
   for ( int i=0;i<ndef-1;i++)//ndef-1-последний участок
  { if( def[i].Rdef*def[i+1].Rdef<0.||plan[def[i+1].nach].s-plan[def[i].kon].s>3.*Lmin)
 {kkr[nkr]=i; nkr++;} //в одну кривую попадёт всё между ними  короткое.
    }//for
   kkr[nkr]=ndef-1; nkr++;
   fprintf(f1,"\n Число кривых nkr=%d",nkr);

   for (int i=0;i<nkr;i++)
   fprintf(f1,"\ni=%d, kkr=%d,nach=%d,kon=%d,send=%10.2lf",
   i,kkr[i],def[kkr[i]].nach,def[kkr[i]].kon,plan[def[kkr[i]].kon].s);
    //здесь  начало и конец последнего участка кривой и абсцисса конца
     fprintf(f1,"\n  начала и концы кривых");
     for (int i=0;i<nkr;i++)
     { if(!i)
     fprintf(f1,"\n i=%d, начало=%d,конец=%d",
     i,def[0].nach,def[kkr[i]].kon);
     else
     fprintf(f1,"\n i=%d, начало=%d,конец=%d",
     i,def[kkr[i-1]+1].nach,def[kkr[i]].kon);
      }
  //kkr[i-1]+1- номер точки начала кривой
     //начало слияния участков
    //номера точек по концам кривых,kk-номер последнего участка в сборке
    int k=0;//число сбитых участков,готовых к записи
    defout=new DEFECT [ndef];//для записи длинных участков
 if(!defout){ShowMessage(" Нет памяти для defout,ndef="+AnsiString(ndef));
         fclose(f1); exit(0);}
    int kk,nnuch,nkuch;// начало и конец участка сборки
    //kk номер последнего участка в сборке в случае успеха
     j=0;//номер участка с которого начнём сборку
   for(int i=0;i<nkr;i++ )
   // по кускам одного знака,включающим несколько кривых их nkr
 {   //kkr массив номеров последних в куске
    //из массива кривых малых радиусов
     if(i) j= kkr[i-1]+1;
    kk=-1;//проход выполнен записывать нечего
    while (j<=kkr[i])//последний включаю  //надо до конца кривой!!!
    { //по куску, по очерели каждую объединяем с остальными с проверкой
      //j-номер в куске с которого начинаем последовательно
         nnuch=def[j].nach; // ошибка при одной кривой в куске
    for( int i1=j+1;i1<=kkr[i];i1++)//сборка включительно с последним
     {
         nkuch=def[i1].kon;
       s1=plan[nkuch].s-plan[nnuch].s;//длина
       if(s1<Lmin)continue;  //нельзя записать в итоговый масив
     // вычислить радиус и решить вопрос о сборке с i1
    // s1=s1-(plan[nnuch+1].s-plan[nnuch].s)/2.-(plan[nkuch].s-plan[nkuch-1].s)/2.;
    //s2= plan[nkuch-1].ugl-plan[nnuch].ugl;
    //R=s1/s2; // радиус по кривой
    R=Rad(nnuch,nkuch);
    s3=fabs(R)-Rmax;
    if(s3>0.) continue;//продолжить участок сборки
    //иначе заносить в выходной массив
    else {
 defout[k].nach=nnuch;defout[k].kon=nkuch;
 defout[k].Rdef=R;//defout[k].defR=-s3;
  kk=i1;//конечный в сборке, если дальше не прибавится
          }//заносить каждый удачный на одно и тоже место останется длиннейший

     }//for i1
  // записать, если есть что kk=-1 остался или нет
    if(kk==-1)
    {
    // проверка j-oго если его не удалось собрать с чем-то
      nnuch=def[j].nach;  nkuch=def[j].kon;
     s1=plan[nkuch].s-plan[nnuch].s;//длина
     if(s1>=Lmin && fabs(def[j].Rdef)<Rmax)  //можно записать в итоговый масив
       {
       defout[k].nach=nnuch;defout[k].kon=nkuch;
       //defout[k].defR=def[j].defR;
       defout[k].Rdef=def[j].Rdef;
       k++; //начальный на кривой записал и сборка без него
       }//if
        j++;continue;//конец проверки j-го при отсутствии сборки
    } else {k++;j=kk+1;kk=-1;}
 //засылка при kk была!!!
    }//while j
 }//по кривым
 if(!k){
    fprintf(f1," Нет дефектных кривых длиной >=Lmin");
       }
 else
 {
 fprintf(f1,"\n собрано кривых k=%d",k);
 for(int i=0;i<k;i++)
   { s1=plan[defout[i].nach].s;
   s2=plan[defout[i].kon].s;
   fprintf (f1,"\n %d nach=%d,kon=%d,R=%8.3lf,s1=%12.3lf,s2=%12.3lf",
           i,defout[i].nach,defout[i].kon, defout[i].Rdef,s1,s2);
    }
 }

 char str[80];
/*
 strcpy(str,fCorners->Edit4->Text.c_str());
  // fprintf(f1,"\nint=%d,long=%d",sizeof(int),sizeof(long));
  fprintf(f1,"\n      Объект  %s \n",str);
 */
 double Lmax1=0.;
 if(k)
 {
 fprintf(f1,"\n   Участки длиной L>%5.0lf м и радиусом <%8.2lf м\n",Lmin,Rmax);
 fputs( " =======================================================================\n", f1 );
 fputs( " |     Начало           |     Конец            |   Длина    | Радиус    |\n", f1 );
 fputs( " |№ точки|  Километраж  |№ точки| Километраж   |            |           |\n", f1 );
  fputs( " =======================================================================\n", f1 );
  for (int i=0;i<k; i++)
  {    s1=plan[defout[i].nach].s+pk0;
   s2=plan[defout[i].kon].s+pk0;
   if(s2-s1>Lmax1)Lmax1=s2-s1;//max длина кривой
      sprintf( str,
    " |%5d  |%13.2lf |%5d  |%13.2lf |%10.2lf  |%9.2lf  |\n",
    defout[i].nach,s1,defout[i].kon,s2,s2-s1, defout[i].Rdef);
      fputs( str, f1 );
   }//for i
   fputs( " ===================================================================", f1 );
  } else
  fprintf(f1,"\n Участков нарушения радиуса %8.2lf не обнаружено",Rmax);
  /* fclose(fp1);
  char put[80];
  strcpy(put,"AkelPad.exe ");
 strcat(put, st2);

 WinExec(put,SW_SHOW);
//vizov(put);попробовать через WinExec но без ..// в своей папке

 С прямой из начальной точки разобраться здесь
 в итоге или не считать вообше или найти кусок начальный, его угол
 вместо  угла по двум первым.
 если макс отклонение от прямой меньше допуска dH/2, то занести прямую в массив прямых и
 перейти к обработке следующей "прямой". Иначе нужна подготовка смены нач угла.
  при поиске отклонений по otkl2 находим сектор для нач угла и запоминаем.
 обрабатывать длинные прямые по obrpr. В итоге каждая из них может дать
  одну или несколько  прямых в допуске, а пока это участки, на которых не
 нашли кривых круче заданной. При поиске первого элемента по найденной настоящей
 прямой установить  nach и считать. Если настоящей прямой не найдено, то считать
  без учёта нач прямой. Если первый элемент не найден, диапазон углов разбить и
  считать с перебором нач угла.
  Здесь же и  поиск других прямых
 //nkr-новoе число прямых и def их массив получаю из defout
 */
 nkr=0;//новый набор прямых
        if(!pribl)
        {
        j= defout[0].nach; //это начало первой кривой
        in=0;
          while(j>=3) //если было меньше 3 в цикл не идём и in=0
       {  s=otkl(0,j,kk,s1);
           if(fabs(s)<dH/2.){in=j;break;} else j--;
       } //while dH/2только для начальной и конечной прямых
            if(in>3){def[0].nach=0;def[0].kon=in; nkr=1;}
         }//начальная прямая при непостроенном первом элементе
     if(k>1) //другие прямые
           {
  for(int j=0;j<k-1;j++)
  {
   in=defout[j].kon;i=defout[j+1].nach; //начало и конец прямой
   if(plan[i].s-plan[in].s<3.5*Lmin)continue;
   s=otkl(in,i,kk,s1);
    fprintf(f1,"\ndo obrpr in=%d,i=%d,kk=%d,nkr=%d,s=%7.2lf",
    in,i,kk,nkr,s);
   if(fabs(s)<dH){def[nkr].nach=in;def[nkr].kon=i;nkr++;
             continue;}
    obrpr(in, i );
    fprintf(f1,"\n obr nkr=%d,dH=%7.2lf",nkr,dH);
 //поиск настоящей прямой
  } //for
  fprintf(f1,"\n fin nkr=%d,dH=%7.2lf",nkr,dH);
   }  //if k>1  dH/2только для начальной и конечной прямых

  //последняя прямая
  /* Если она настоящая занести в массив. Иначе:{
     если её длина меньше 2Lmax с конца убрать макс. кусок
     настоящей , иначе {obrpr на всей и если последняя не настоящая
     //меньше Lmin,отодвинуть правый конец, до Lmin }}
  */
     //  defout[k-1].kon=defout[k-1].kon-50;
   s2=plan[n_pts-1].s-plan[defout[k-1].kon].s;// длина последней прямой
   if(s2>Lmin)
    {in= defout[k-1].kon; i=n_pts-1;//последняя прямая
     //последняя прямая //s2 длина куска с поправкой
    int in1;
     s=otkl(in,i,kk,s1);
      if (fabs(s)<=dH/2.)
      {def[nkr].nach=in;def[nkr].kon=i;def[nkr].L=s1;nkr++;}
       else
       { j=0;
         if(s1<2.*Lmax)//если больше, то поищем несколько прямых
        {in1=in+1;
          while (i-in1>3)
          {s=otkl(in1,i,kk,s1);
            if( fabs(s)<=dH)
          {def[nkr].nach=in1;def[nkr].kon=i;def[nkr].L=s1;nkr++;
           j=in1; break;
          } else in1++;
        }//while
     }// s1<2.*Lmax поиска куска настоящей прямой с конца
        if(!j)obrpr(in,i);
    }//end of else обработка вся не прошла
       } //хвост по кривым >Lmin
  if(!nkr)
  ShowMessage(" Длинных прямых не найдено");
    else {
       if(def[nkr-1].kon< n_pts-1)//последняя прямая не на конце
      { s2= plan[n_pts-1].s-plan[def[nkr-1].kon].s;
        if(s2<Lmin)//отодвинуть
         { j=def[nkr-1].kon-1; //конец прямой
            while(plan[n_pts-1].s-plan[j].s<Lmin)j--;
           def[nkr-1].kon=j;
           def[nkr-1].L=plan[j].s-plan[def[nkr-1].nach].s;
         }//сдвиг
       }//последняя прямая
     // конец от конца прямой до финиша надо  больше Lmin
   fprintf(f1,"\n          ПРЯМЫЕ     ");
  for (int j=0;j<nkr;j++)
  { s2= otkl(def[j].nach,def[j].kon,k,s);
  fprintf(f1,"\n  j=%d,nach=%d,kon=%d,npk=%7.2lf,kpk=%7.2lf,s2=%9.5lf",
 j,def[j].nach,def[j].kon,plan[def[j].nach].s+pk0,plan[def[j].kon].s+pk0,s2);
   }
   }//else есть прямые
   ndef=nkr;//так в pech
 delete[]defout;
   //delete []def;
 //  fclose(f1); exit(0);
   return 0;
  }// priamaia
  //++++

  //+++++
     /*
   double opt_kriv(double& R1,double tetanew,double xvnew,double yvnew,
        double uglnew,double ugpnew,double xlnew,double ylnew,
        double xpnew,double ypnew,int zap);
       */
 double opt_kriv1(double& R1,double tetanew,double xvnew,double yvnew,
        double uglnew,double ugpnew,double xlnew,double ylnew,double xpnew,
        double ypnew,int zap);
 int obrclot(double L,double teta,double Tmax,double &t,double &Rmax1);
  double tangen1(double L,double R,double teta,int rt)
  {
  double p,t,phi,k;
  phi=L/(2*R); k=1./(R*L);
  switch(rt)
  {
  case 0:
   {  p=L*L/(24.*R); t=L/2.;
   break;
   }
  case 1:
   {p= L*L/(24.*R)-L*L*L/R/R*(1.0/1536.-L/R/336.);
    t=L/2.*(1.-L*L/(R*R*120.));
    break;
    }
    case 2:
    {p=cloy(k,L)-R*(1.-cos(phi));
     t=clox(k,L)-R*sin(phi);
    break;
    }
    default : ShowMessage("Ошибка в вызове tang");
  }//switch
  return ((R+p)*tan(fabs(teta/2.))+t);
 } //tangen1

 void tangen2(double L1,double L2,double R,double teta,
               int rt,double &T1,double &T2)
  {
  double p1,t1,phi1,p2,t2,phi2,k1,k2,T,Tp1,Tp2;
  double s1,s2,s3,s4;
   phi1=L1/(2*R);phi2=L2/(2*R);
   k1=1./(R*L1);  k2=1./(R*L2);
   T=R*tan(fabs(teta/2.));
   s1=L1/R; s2=L2/R;
  switch(rt)
  {

  case 0:
   {
       //p1, p2 от прямой до окружности ==a,b in formulaes
      p1=L1*s1/24.; t1=L1/2.;
      p2=L2*s2/24.; t2=L2/2.;
      break;
   }

  case 1:
   {
   s3=s1*s1; s4=s2*s2;
p1=L1*s1*( 1.-s1*s1/56.)/6.-R*(1.-cos(phi1));
t1=L1*(1.-s1*s1/40.+s3*s3/3456.)-R*sin(phi1);
p2=L2*s2*( 1.-s2*s2/56.)/6.-R*(1.-cos(phi2));
t2=L2*(1.-s2*s2/40.+s4*s4/3456.)-R*sin(phi2);

     break;
    }
    case 2:
    {
     p1=cloy(k1,L1)-R*(1.-cos(phi1));
     t1=clox(k1,L1)-R*sin(phi1);
     p2=cloy(k2,L2)-R*(1.-cos(phi2));
     t2=clox(k2,L2)-R*sin(phi2);
           break;
    }
    default : ShowMessage("Ошибка в вызове tang");

  }//switch
   Tp1=(p2-p1*cos(teta))/sin(fabs(teta));
   Tp2= (p1-p2*cos(teta))/sin(fabs(teta));
   T1=T+Tp1+t1;  //teta>Pi/2 проверить
   T2=T+Tp2+t2;
  //  fprintf(f1,"\n\np1=%9.6lf,p2=%9.6lf,teta=%9.6lf",p1,p2,teta);
   //fprintf(f1,"\nT=%9.6lf,Tp1=%9.6lf,Tp2=%9.6lf,t1=%9.6lf,t2=%9.6lf",
     //         T,Tp1,Tp2,t1,t2);

   return;
 } //tangen2

 void tangen3(double L1,double L2,double R,double teta,int rt,
  double &T1,double &T2,double &T,double &Tp1,double &Tp2)
  {
  double p1,t1,phi1,p2,t2,phi2,k1,k2;//,T,Tp1,Tp2;
  double s1,s2,s3,s4;
   phi1=L1/(2*R);phi2=L2/(2*R);
   k1=1./(R*L1);  k2=1./(R*L2);
   T=R*tan(fabs(teta/2.));
   s1=L1/R; s2=L2/R;
  switch(rt)
  {

  case 0:
   {
       //p1, p2 от прямой до окружности ==a,b in formulaes
      p1=L1*s1/24.; t1=L1/2.;
      p2=L2*s2/24.; t2=L2/2.;
      break;
   }

  case 1:
   {
   s3=s1*s1; s4=s2*s2;
p1=L1*s1*( 1.-s1*s1/56.)/6.-R*(1.-cos(phi1));
t1=L1*(1.-s1*s1/40.+s3*s3/3456.)-R*sin(phi1);
p2=L2*s2*( 1.-s2*s2/56.)/6.-R*(1.-cos(phi2));
t2=L2*(1.-s2*s2/40.+s4*s4/3456.)-R*sin(phi2);

     break;
    }
    case 2:
    {
     p1=cloy(k1,L1)-R*(1.-cos(phi1));
     t1=clox(k1,L1)-R*sin(phi1);
     p2=cloy(k2,L2)-R*(1.-cos(phi2));
     t2=clox(k2,L2)-R*sin(phi2);
           break;
    }
    default : ShowMessage("Ошибка в вызове tang");

  }//switch
   Tp1=(p2-p1*cos(teta))/sin(fabs(teta));
   Tp2= (p1-p2*cos(teta))/sin(fabs(teta));
   T1=T+Tp1+t1;  //teta>Pi/2 проверить
   T2=T+Tp2+t2;
  //  fprintf(f1,"\n\np1=%9.6lf,p2=%9.6lf,teta=%9.6lf",p1,p2,teta);
   //fprintf(f1,"\nT=%9.6lf,Tp1=%9.6lf,Tp2=%9.6lf,t1=%9.6lf,t2=%9.6lf",
     //         T,Tp1,Tp2,t1,t2);
   return;
 } //tangen3

 int sum_mod(double T1,double xvnew,double yvnew,double xlnew,double ylnew,
    double xpnew,double ypnew,double uglnew,double R1,double tetanew,
    double &sum1,int jnach,int jkon,double &d,int prp=0);

  int sum_mod1(double T1,double xvnew,double yvnew,double xlnew,double ylnew,
 double xpnew,double ypnew,double uglnew,double R1,double tetanew,double &sum1,
 int jnach,int jkon,double &d1,double &d2,int &id1,int &id2,int prp=0);

  int obrclot(double L,double teta,double Tmax,double &t,double &Rmax1)
 {  double R,R1,s1,s2; int j;  //t-Tmax ,L Rmax1-без нарушения Tmax
 Rmin1=(L+lkmin)/fabs(teta); //>Rmax не вписаться в угол
  if(Rmin1>Rmax)   return 5;
  //5- при малом угле, не вписаться с мин длинами не нарушая Rmax
  if(Rmin1<Rmin) Rmin1=Rmin;//не было
  s1=fabs(teta/2.); s2= L*L/24.;
    //тангенс при Rmin1 меньше радиус не лезет по углу
t=(Rmin1+s2/Rmin1)*tan(s1)+L/2.; //t-тангенс при Rmin1
 if(t>Tmax)  return 5;//при min длинах (Rmin1) тангенс больше допустимого
   //L нельзя увеличить по углу т.к. вырастет Rmin1
  Bcmin= Rmin1*(1.0/cos(s1)-1.0)+s2/Rmin1/cos(s1);
    j=tgclot( Tmax,L,Bcmax,teta,R,R1);//Rmin1, Bcmin по L и teta миним
   //Bcmax при Tmax- биссектриса R1 радиус далее корр по Rmax
 //R,R1-два радиуса при L,Tmax,R1 при плюс корень, то есть больший
  if(j!=2){fprintf(f1,"\n1obrclot нет корней в tgclot L=%7.2lf,Tmax=%7.2lf",
            L,Tmax);
           fclose(f1); exit(0);
          }
 //проверено Tmax не достигается при Rmin1
  // при макс тангенсе эту клотоиду надо уменьшать
 //R1 при Tmax! а заходим с новой L!  и выдаём Rmin,t->Tmin
  if(R1>Rmax){j=1;R1=Rmax;}else j=0;
  Bcmax= R1*(1.0/cos(s1)-1.0)+s2/R1/cos(s1);
  //она же из tgclot при Tmax и R1<Rmax
 Rmax1=R1;// max радиус  при Tmax ,L Rmax  R1 уменьшил,а тангенс???
 return j;
 //R1 это  при L и  Tmax : j=0-R1<Rmax, 1- R1>Rmax и стал равен ему
 //5-  при этой клотоиде не вписаться в угол с ьин длинами без
//превышения Rmax или  Tmax; t-тангенс при миним длинах(Rmin1)
//Bcmin,Bcmax- для угла и мин длин соответствуют выходным Rmin1 и Rmax1
 }//end obrclot

 void pech(void)
  {   //Здесь-число точек пока n на 1 меньше
    int j,j1,j2,k;double s3; //s3-global
    pi2=pi/2.0;
    //etap=0;//только биссектрисы
    ipp=0;//пeчать
    f1=fopen("Test_VUxy.txt","wt");
    //Xmax надо суммарную длину а там максим Х координата
    Xmax=x[n-1];
    fprintf(f1,"\n n=%d,Xmax=%8.2lf,dx=%8.2lf,dy=%8.2lf,dugl=%9.6lf,maxrix=%6.3lf",
                n,Xmax,dx,dy,dugl,maxrix);
    fprintf(f1,"\n x[0]=%8.2lf,beta[0]=%9.6lf",x[0],beta[0]);
    fprintf(f1,"\n x[n-1]=%8.2lf,beta[n-1]=%8.6lf,beta[n]=%8.6lf",
                  x[n-1],beta[n-1],beta[n]);
    fprintf(f1,"\n xt[0]=%8.2lf,xt[1]=%8.2lf,xt[n-1]=%8.2lf,xt[n]=%8.2lf",
                xt[0],xt[1],xt[n-1],xt[n]);
    // абсциссы- это текущая сумма длин хорд, а не координаты
    //всего абсцисс n а не n+1, а точек съёмки,т.е. нормалей n+1(c 0->n включительно)
    //начальная прямая  dx=dy=dugl=0.0 в BOHINI но вообще не обязательно
    //координаты точек сЪёмки xt[0->n],yt[0->n], их n+1,
    // а абсциссы x[0->n-1] и бета сдвинуты n уменьшен в cxema при вводе
    //x[0]- длина первой хорды от нулевой до первой точки;
    //всего абсцисс n а не n+1  c 0 n-1
    dL=10.; kdL=16,kdR=24;//no in Form6
    fprintf(f1,"\nn=%d,Lmin=%7.2lf,Lmax=%7.2lf,dL=%5.2lf,Rmax=%8.2lf",
              n,Lmin,Lmax,dL,Rmax);
    fprintf(f1,"\nRmin=%8.2lf",Rmin);
    fprintf(f1,"\nlmin=%6.2lf,lmax=%6.2lf,lkmin=%6.2lf,lprmin=%6.2lf",
              lmin,lmax,lkmin,lprmin);
    fprintf(f1,"\nnpr=%d,nug=%d,dpr=%5.2lf,dug=%7.5lf,kdR=%d,kdL=%d",
              npr,nug,dpr,dug,kdR,kdL);
    n_pts=n+1;  //n- номер последней точки съёмки в исходной нумерации
    // n_pts- число точек всего
    int in,kk;   in=0;
    for(int i1=1;i1<n;i1++)
    {
      if(fabs(-(xt[i1]-dx)*sin(dugl)+(yt[i1]-dy)*cos(dugl))>dH/2.)
      {
        in=i1-1; break;
      }
    }//for i1 попытка заданным углом построить первый элемент
    
    if(!in)
    {
      ShowMessage("Весь участок-это одна прямая");
      fclose(f1); exit(0);
    }
    nach=in+NNom(Lmin);//по правому концу накрывшей хорды
    kon=NNom(Xmax-Lmin)-1;
    xl=dx; yl=dy; ugl=dugl;
    //ugl=dugl исходные возможно изменение при ndef до perv

    fprintf(f1,"\nnach=%d,xt[nach]=%6.2lf,xt[nach+1]=%6.2lf",
                    nach,xt[nach],xt[nach+1]);
    fprintf(f1,"\nпо старой kon=%d,x[kon-1]=%7.2lf",kon, x[kon-1]);
    kon=kon-nach;//в новую нумерацию c новым nach
    //до этой нормали строить без последнего элемента
    tetamin=(lmin+lkmin)/Rmax; // использую !!
    inach=0; //для perv  в новой нумерации  ikon за Lmax от nach
    //====   /есть глобальная s3
    j1=0;//vse нужны a для коротких участков будет j1=1 только perv
    // s3=Lmax;if(s3>Xmax-Lmin)s3= Xmax-Lmin;
    if(Xmax<2.*Lmin)
    {
      j1=1;ikon=n-nach;
    }//достаточно perv
    else ikon=NNom (x[nach-1]+Lmax)-nach;//правая точка
    fprintf(f1,"\n для perv inach=%d,ikon=%d,j1=%d,kon=%d",
                     inach,ikon,j1,kon);
    // kon-для последнего эл-та, а inach,ikon для первого
    Nnorm=n-nach+1;//n+1-число точек  с 0->n  54
    //число нормалей для поиска до конечной точки беру все!!1
    fprintf(f1,"\nn=%d,nach=%d,Nnorm=%d",n,nach,Nnorm);
    masnorm=new NORMAL[Nnorm];
    for(int i=0;i<Nnorm;i++)
    {
      masnorm[i].point=NULL;
      masnorm[i].pr=0;//признак блокировки на длинных прямых
    }
    nzapf=-1; nzapmax=0;
    sum=sumall=0.0;
    perv(); //в пределах inach=0 до  ikon-последняя за Lmin  до конца
    if(!nzapmax)
    {
      ShowMessage("Не построен первый элемент.Меняем азимут");
      fprintf(f1,"\n Не построен первый элемент.Меняем азимут");
      delete []masnorm; pribl=0;
    }
    else
    {
      pribl=1;
      fprintf(f1,"\nafter perv nzapf=%d,nzapmax=%d,dugl=%9.6lf",
               nzapf,nzapmax,dugl);
    }
  //fclose(f1); exit(0);
//++++
 if(dH>0.001)priamaia(xt,yt);else ndef=0;
  if(!pribl)
  {
  nach=NNom(Lmin);
 //  plan не нужен  работать по "абсциссам"plan[q].s=x[q-1]
   if(ndef&&def[0].kon>nach)//def[0].kon-конец прямой
 {//иначе прямая короче Lmin и не влияет на nach
if(def[0].nach<=1)
   {//прямая с самого начала
 fprintf(f1,"\nnach=%d,dugl=%9.6lf,xkon=%7.2lf,Lmin=%5.2lf",
            nach,dugl, x[def[0].kon-1],Lmin);
 nach=NNom(x[def[0].kon-1]+Lmin); //по правому концу
  dugl=ugl=ugol(xt[0],yt[0],xt[def[0].kon],yt[def[0].kon]);
  ShowMessage("Угол взят по прямой с нуля dugl="+AnsiString(dugl));
  fprintf(f1,"\nnach=%d,dugl=%9.6lf,def=%d",nach,dugl,def[0].kon);
   // выправил начальный угол ЕГО НАДО ДАЛЬШЕ УЧЕСТЬ!!!
// вариант с def[0].nach>0 но ближе к началу чем  Lmin
   } else
     {  if(def[0].nach<nach)
 ShowMessage("Начало на кривой.Возможны проблемы с первым элементом");
     }
 }//if ndef  с углом надо разбираться!
 }//!pribl если не построен первый элемент

 //абсцисса( расст от начала по ломаной) x[nach-1], было x[nach]    ошибка
  fprintf(f1,"\nnach=%d,xt[nach]=%6.2lf,xt[nach+1]=%6.2lf",
                nach,xt[nach],xt[nach+1]);
  fprintf(f1,"\n x[0]=%6.2lf,x[1]=%6.2lf",x[0],x[1]);
  ////
 //kon=NNom(Xmax-Lmin);
 kon=NNom(Xmax-Lmin)-1;//нет ndef или нет прямой в конце
  if(ndef )
 { if (def[ndef-1].kon==n)
    kon=NNom(x[def[ndef-1].nach-1]-Lmin)-1;//левая точка
  //kon-конечная нормаль до неё дойти по vse и не далее
   else
  if(Xmax-x[def[ndef-1].kon-1]<Lmin)
   ShowMessage("Последняя точка на кривой.Возможны проблемы с последним элементом");
  } //есть ndef

  //беру начало накрывшей хорды, иначе близко к концу
 fprintf(f1,"\nпо старой kon=%d,x[kon-1]=%7.2lf",kon, x[kon-1]);
  kon=kon-nach;//в новую нумерацию c новым nach
//до этой нормали строить без последнего элемента
 // tetamin=(lmin+lkmin)/Rmax; // использую !!
  inach=0; //для perv  в новой нумерации  ikon за Lmax от nach
  //====   /есть глобальная s3
   j1=0;//vse нужны a для коротких участков будет j1=1 только perv
 // s3=Lmax;if(s3>Xmax-Lmin)s3= Xmax-Lmin;
  if(Xmax<2.*Lmin){j1=1;ikon=n-nach;}//достаточно perv
  else ikon=NNom (x[nach-1]+Lmax)-nach;//правая точка
  fprintf(f1,"\n для perv inach=%d,ikon=%d,j1=%d,kon=%d",
                     inach,ikon,j1,kon);
  // kon-для последнего эл-та, а inach,ikon для первого
 if(!pribl){
    Nnorm=n-nach+1;//n+1-число точек  с 0->n  54
    //число нормалей для поиска до конечной точки беру все!!1
  fprintf(f1,"\nn=%d,nach=%d,Nnorm=%d",n,nach,Nnorm);
  masnorm=new NORMAL[Nnorm];
    for(int i=0;i<Nnorm;i++)
 {  masnorm[i].point=NULL;
 masnorm[i].pr=0;//признак блокировки на длинных прямых
 }
 }//if!pribl
 double ugolpr;
 if(ndef)
 {  int jj1;//нельзя портить j1
   for (int i=0;i<ndef;i++)
   {  if(def[i].nach<=nach)continue;
     jj1= def[i].nach;j2=def[i].kon;//в старой нумерации
 ugolpr=ugol(xt[jj1],yt[jj1],xt[j2],yt[j2]); // для нормалей
 fprintf(f1,"\n i=%d,jj1=%d,j2=%d,ugolpr=%9.6lf",i,jj1,j2,ugolpr);
    for(int k=jj1;k<=j2;k++)
    {
     masnorm[k-nach].pr= j2-nach-3;
      if(k>=j2-3)
  { fprintf(f1,"\npech k=%d,do beta[k]=%9.6lf",k,beta[k]);
      beta[k]=ugolpr+pi2 ;
   fprintf(f1,"\n k=%d,pos beta[k]=%9.6lf",k,beta[k]);
   //это номер нормали куда перейти при попадании на прямую
  }
    }//end for k
    }//end for i
      }//end if ndef
     //надо пометить все точки зоны конечная -3-nach
  // в четырёх последних исправил нормали,вычисляя угол
  //если последняя точка на прямой, то нормаль поправлена и там
  delete []plan;
   if(!pribl)
  {
    nzapf=-1; nzapmax=0;
    sum=sumall=0.0;
   //fclose(f1); exit(0);
       perv();
   if(ndef&& def[0].nach==0)
 {  FILE *nu;
 nu=fopen("nach_ugl.txt","wt");
 fprintf(nu,"%15.6lf",dx);
 fprintf(nu,"\n%15.6lf",dy);
  //временно вместо dugl
 fprintf(nu,"\n%12.8lf",dugl);
 fclose(nu);
 }

 fprintf(f1,"\nafter perv nzapf=%d,nzapmax=%d,dugl=%9.6lf",
               nzapf,nzapmax,dugl);
   //fclose(f1);Form1->Close();exit(0);

   if(!nzapmax){ShowMessage("2Не построен первый элемент");
    fclose(f1);Form1->Close();exit(0);
             }
 }//if(!pribl)
   Form1->Fild->Visible=false;
  Form1->Panel1->Visible=false;
  if(!j1)// подготовка пределов для  vse с использованием posl
 { //posl не вызываю с j1==1, vse до конца если не хватает perv
    //правая от  (2Lmin  до n-nach а не kon )
   // j2=Nnom(2.*Lmin)-nach;
   // в нумерации нормалей это для правой j2->n-nach (kon)
    // левая от max из(правая -Lmax,Lmin) до правая -Lmin)
 //k-текущая правая s3=x(k+nach-1) её абсцисса; s2=max(s3-Lmax,Lmin)-абсцисса
 //начала левой при заданной правой NNom(s2)-nach её номер для начала цикла
    // NNom(s3-Lmin)-nach- конец цикла по левой
  /* есть nzapf-nzapmax-интервал изменения левой это уже построеные
  возможно с дырками   для каждой левой nver определяю границы
  изменения правой  как nver+Lmin
  с переводом в номер точки (левая граница) nver+Lmax но не ближе Lmin от конца
posl внутри два цикла по точкам и углам а цикла по левым нет. Он вынесен на
верх в интервал   nzapf-nzapmax
  */
  pov=2;
 vse();
  if( masnorm[Nnorm-1].point[npr].ukp!=NULL)
  {
   if(masnorm[Nnorm-1].point[npr].ukp[nug].sv!=NULL)
  fprintf(f1,"\nafter vse nzapmax=%d,sum по центру =%7.2lf",
 nzapmax,masnorm[Nnorm-1].point[npr].ukp[nug].sv->sum);
  //else { fclose(f1);return;}
 }
 }
 nkr=zapline(nprz,nugz);// nprz,nugz-номер точки и угла
//записана линия (можно не центр а оптимальную взять!)
//далее её анализ и оптимизация
fprintf(f1,"\nend zapline nkr=%d",nkr);
 opt_line();
 fprintf(f1,"\n afterline");
 zapis(nprz,nugz);//временно
 fprintf(f1,"\nend zapis");
 fprintf(f1," \nНачальное приближение построено");
  fclose(f1);
 return; //в Praktika кнопка Рассчитать
 //etap=1;
 pov=0;
 //pervnew();// только для Line[0]->norm (24)
 pov=2;
fprintf(f1,"\nend pervnew");
 //newvse();  не нашёл её
fprintf(f1,"\nend newvse");
   nkr=zapline(nprz,nugz);   //есть ли линия???
 opt_line(); //  портит sum и не пишет в masnorm

 zapis(nprz,nugz);  //центрального без выбора пока
 udal();
 fclose(f1);
 return;//posl in vse
  }//pech

 int pervnew(void)
 { //global xl,yl,xp,yp,ugl,ugp,pr;  //это etap==1 pov==0
// double t1isx,t2isx,def1isx,def2isx,xvisx,yvisx,xpisx,ypisx;
 //для центральной точки
   int prvu,j;//в pech j1=0 надо строить все,если 1-сразу последний
  // fprintf (f1,"\n nach=%d,kon=%d,Nnorm=%d,x[nach]=%7.2lf",
    //       nach,kon,Nnorm,x[nach]);
 //В массиве нормалей нумерация от 0 до Nnorm-1; номеру k в этом
 //массиве соответствует нормаль с номером nach+k+1???в исходной нумерации
 //и nach+k в нумерации расст от начала ( абсцисс) по ломаной
  //xl=dx; yl=dy; ugl=dugl;//В первом эл=те всегда первая прямая фиксирована
  //  for( nver=1;x[nach+nver]<=s3;nver++) //нумерация нормалей с нуля nach
 for( nver=inach;nver<=ikon;nver++)//в новой нумерации нормалей
   //for( nver=10;nver<=12;nver++)
 //до Lmax,но не далее Lmin от конца
   { //основной цикл
      // брать память под точки и углы, если есть связь.
    //Сначала центральная точка, если нет связи  половина отсеется
  ntoch=nugl=0;
  xp= xt[nach+nver]; yp= yt[nach+nver]; ugp=beta[nach+nver]-pi2;
//это центральная точка и угол в ней как при съёмке будем её проверять
/*if(nver==41)
{ fprintf (f1,"\nnver41 xl=%7.2lf,yl=%7.2lf,xp=%7.2lf,,yp=%7.2lf",xl,yl,xp,yp);
fprintf(f1,"\nugl=%8.6lf,ugp=%8.6lf,beta=%8.6lf",ugl,ugp,beta[nach+nver]);
 }*/
  //  def1=def2=0.0;  //отрицательный значит запас по тангенсу
    teta=ugp-ugl; // s=fabs(teta);
    pov=0;
    //0=вращаем вторую сторону с ug2 (teta поворот от 1 к 2 стороне 2-дальше от начала
  pr=VUxy(xl,yl,xp,yp,ugl,ugp,xv,yv);
  //1-при ошибке 2-не влез тангeнс,0-хорошо

 if(pr==1){//fprintf(f1,"\nза пределами nver=%d,x[nach+nver]=%8.3lf,x[nach+nver+1]=%8.3lf",
// nver,x[nach+nver],x[nach+nver+1]);
   return 1;//continue;
   }//за= пределами
//ВУ за пределами (отрицательный тангенс)уйти на другую нормаль
   prvu=proverka(xv,yv,0.0,1,nach+nver);
   //fprintf (f1,"\n proverka=%d,Bcmin=%7.2lf",prvu,Bcmin);
   if(prvu){//fprintf(f1,"\nprvu=1=%d,nver=%d",prvu,nver);
   continue;}
   //хорошо при 0,1-внутри кривой 2-велика  биссектриса Bcmin
   //если ВУ внутри то надо бы не на след нормаль, а на сдвиг по своей!
// prvu не 0(внутри кривой или точки ближе к ВУ чем мин биссек.)ушли
  if(pr==0)
  {
  // fprintf(f1,"\nНашли допуск в центральной точке");
//fprintf(f1,"\n\nнули Работать nver=%d,x[nach+nver]=%8.3lf\n\n",nver,x[nach+nver]);
     postr(xl,yl,xp,yp,xv,yv,Bc,1,nach+nver,0.0);
  }
   // мал тангенс или всё хорошо pr==1 уже исключили

     t1isx=t1;t2isx=t2; def1isx=def1;def2isx=def2; //до поворотов
     xvisx=xv; yvisx=yv; //xv,yv перевычислю но не испорчу центр точку
     xpisx=xp;ypisx=yp; // правая точка ходит по нормали
  // fprintf(f1,"\nцентр pr=%d,def1=%7.2lf,def2=%7.2lf",pr,def1,def2);
     pouglam(1,nach+nver); //портит  Tmax in corrtg
      double dt1,dt2;
    for(int k=1;k<=npr;k++)
   {
    //teta старый не меняю для одной нормали ВУ надо менять,
// но она пересчитывается. pouglam c дефицитом 1 раз и break
      ntoch=k; nugl=0;//центр в новой точке
 //dt1,dt2- приращ. тангенсов,а не сдвиг по сторонам угла!смещаю t2-правый
        dt1= -k*dpr/sin(teta);
         if(-dt1>t1isx)break;
        dt2=k*dpr*cos(teta)/sin(teta);//нельзя tan(teta),нужны оба
         if(-dt2>t2isx-lprmin) break;;// вершина ушла за левую точку
  def1=def1isx-dt1;// добавка с минусом dpr/sin(teta)
  def2=def2isx-dt2;//добавка dpr/tan(teta). новый def=def-добавка
     //if((def1>0.)||(def2>0.))break;
     xv=xvisx+dt1*cos(ugl);
     yv=yvisx+dt1*sin(ugl);
      prvu=proverka(xv,yv,0.0,1,nach+nver);
      //if(prvu) break;
      if(prvu)continue;
    t1=t1isx+dt1;
    t2= t2isx+dt2;
    xp=xpisx-k*dpr*sin (ugp);//+cos(ugp+pi2)
    yp=ypisx+k*dpr*cos (ugp);// sin(ugp+pi2)
    Tmax=t1;
    if(Tmax>t2-lprmin)Tmax=t2-lprmin;
   if((def1>0.)||(def2>0.)){pouglam(1,nach+nver); break;}
  //if((def1<=0.)&&(def2<=0.))postr(xl,yl,xp,yp,xv,yv,Bc,1,nach+nver,0.0);
      pouglam(1,nach+nver); //проверил после шага вверх
 //fprintf(f1,"\nцентр вверх проверен pr=%d,k=%d,dt1=%7.3lf,dt2=%7.3lf",
   //  pr,k,dt1,dt2);
    }//for  k
 for(int k=1;k<=npr;k++)
 {
     ntoch=-k;//вниз с минусом
       nugl=0;
      dt1= k*dpr/sin(teta);
      if(-dt1>t1isx)break;
      dt2=-k*dpr*cos(teta)/sin(teta);
      if(-dt2>t2isx) break; //ВУ ушла за точку, не идти pouglam
       def1=def1isx-dt1;// добавка с минусом dpr/sin(teta)i
       def2=def2isx-dt2;//добавка dpr/tan(teta). новый def=def-добавка
  //   if((def1>0.)||(def2>0.))break;
     xv=xvisx+dt1*cos(ugl);
     yv=yvisx+dt1*sin(ugl);
     //здесь верно, движение по первому тангенсу
     prvu=proverka(xv,yv,0.0,1,nach+nver);
    // if(prvu)break;
    if(prvu) continue;
    //1-внутри кривой 2-велика  биссектриса Bcmin
       t1=t1isx+dt1;
       t2= t2isx+dt2;
       xp=xpisx-k*dpr*cos(ugp+pi2);yp=ypisx-k*dpr*sin(ugp+pi2);
        Tmax=t1;
    if(Tmax>t2-lprmin)Tmax=t2-lprmin;

    if((def1>0.)||(def2>0.)){ pouglam(1,nach+nver);break;}
      postr(xl,yl,xp,yp,xv,yv,Bc,1,nach+nver,0.0);
     pouglam(1,nach+nver); //проверил после шага вниз
    }//for k минус
   } //for nver
    return 0;
   } //pervnew

    void poslnew(int j2,int j3)
  { // j2,  j3-концы интервала для левой а  правая для vse pov==2 nverp
    //для просто posl  правая j3==n-nach //xv,yv-глобальные
    int i,j,prvu;
  // double t1isx,t2isx,def1isx,def2isx,xvisx,yvisx,xlisx,ylisx;//,xpisx,ypisx;
     //старая точка нужна : правая для perv,левая для posl
   ukinf=NULL;//работает при pov==1(последняя)
     ukinfp=NULL;//работает при pov==2(vse)
 //для каждой новой точки или угла создаётся заново при записи если
 // в это состояние не приходили ранее
   // fprintf(f1,"\nposl j2=%d,j3=%d,nach=%d,betan=%8.6lf",
   // j2,j3,nach,beta[n]);
 xp= xt[nverp+nach];yp= yt[nverp+nach];ugp=beta[nverp+nach]-pi2;//угол по beta
 if(pov==2)//vse
 { if(ntochp!=npr)//не центр на правой
   {  xp=xp+dpr*(ntochp-npr)*cos(beta[nverp+nach]);
      yp=yp+dpr*(ntochp-npr)*sin(beta[nverp+nach]);
   }
   if(nuglp!=nug)ugp=ugp+dug*(nuglp-nug);
 }//if pov==2 случай 1 в запасе
   // Надо менять правую точку и угол при движении по правой нормали!!!!

   for (nver=j2;nver<=j3;nver++)// nver- левая нормаль
  {
   i=nver+nach;//номер левой текущей точки( нормали) в исх нумерации
     if(masnorm[nver].point==NULL)continue;//на левой нет точек
       //Если нет в центральной точке, то может быть в других
       //и тоже с углом  сразу бросать эту нормаль
      ntoch=0; nugl=0;  //центр на левой там такая нумерация
    ugl=beta[i]-pi2; xl=xt[i]; yl=yt[i]; //угол и точка слева центральная ??
     pr=VUxy(xl,yl,xp,yp,ugl,ugp,xv,yv);//1-при ошибке 2-не влез тангeнс,0-хорошо
 //даёт t1,t2( расст от вершины) и Tmax
   if(pr==1)
   continue;// на след. нормаль  1-|| или за пределами 2-не влез тангенс
   prvu=proverka(xv,yv,0.0,i,nverp+nach);
  // fprintf (f1,"\n prvu=%d,Bcmin=%7.2lf",prvu,Bcmin);
   //if(prvu){//fprintf(f1,"\nprvu=%d,nver=%d",prvu,nver); continue;}
   //хорошо при 0,1-внутри кривой 2-велика  биссектриса Bcmin
   //если ВУ внутри то надо бы не на след нормаль, а на сдвиг по своей!
// prvu не 0)(внутри кривой или точки ближе к ВУ чем мин биссек.)ушли
  if(pr==0&&prvu==0 && masnorm[nver].point[ntoch+npr].ukp!=NULL &&
      masnorm[nver].point[ntoch+npr].ukp[nug+nugl].sv!=NULL)
       postr(xl,yl,xp,yp,xv,yv,Bc,i,nverp+nach,0.0);//в центральной левой
 //При pr==2 ( не влез тангенс) надо менять точку и углы
   //на точках и углах идёт пересчёт от центра поэтому контроль на связь
  //только при попытке записать ( postr)
 // мал тангенс или всё хорошо pr==1 уже исключили, но по углам надо!!

      t1isx=t1;t2isx=t2; def1isx=def1;def2isx=def2; //до поворотов
     xvisx=xv; yvisx=yv; //xv,yv перевычислю но не испорчу центр точку
    // xpisx=xp;ypisx=yp;
     xlisx=xl;ylisx=yl; // левая точка ходит по нормали
  // fprintf(f1,"\nposl центр pr=%d,def1=%7.2lf,def2=%7.2lf",pr,def1,def2);
   if(masnorm[nver].point[ntoch+npr].ukp!=NULL)
    pouglam(i,nverp+nach);
       //i->nverp+nach-интервал проверки,портит  Tmax in corrtg
    //центр проверяю всегда, нужно для пересчёта
       double dt1,dt2;// double a,b;
    for(int k=1;k<=npr;k++)
   {  //teta старый не меняю для одной нормали ВУ надо менять,
      // но по углам она пересчитывается
      ntoch=k; nugl=0;//центр в новой точке
  dt1=k*dpr*cos(teta)/sin(teta); //нельзя /tan при pi/2!
  dt2= -k*dpr/sin(teta);//было наоборот , ошибка
 // это приращение тангенсов. левый t1, смещается левый
 //сравнивать новые t1,t2 c Tmax!
 if((-dt1>t1isx) || (-dt2>t2isx-lprmin)) break;// вершина ушла за правую точку

   //  if( masnorm[nver].point[ntoch+npr].ukp==NULL) continue;//нет углов на левой
       if( (masnorm[nver].point[ntoch+npr].ukp==NULL) ||
       ( masnorm[nver].point[ntoch+npr].ukp[nug+nugl].sv==NULL))
         continue;// на левой нет информации
    def1=def1isx-dt1;// добавка с минусом dpr/sin(teta)i
    def2=def2isx-dt2;//добавка dpr/tan(teta). новый def=def-добавка
  // if((def1>0.)||(def2>0.))break;
    t1=t1isx+dt1;
    t2= t2isx+dt2;
    //if(!pov)//pov==0 двигаем или вращаем вторую (prev),1- левую posl
    //{xp=xpisx-k*dpr*sin(ugp); yp=ypisx+k*dpr*cos(ugp);} else
     xl=xlisx-k*dpr*sin(ugl); yl=ylisx+k*dpr*cos(ugl);
    //избавился от pi2 ugl+pi2=beta.        }
    Tmax=t1;
    if(Tmax>t2-lprmin)Tmax=t2-lprmin;

    xv=xvisx-dt2*cos(ugp); //смещение по второму тангенсу равно с минусом его изменению
    yv=yvisx-dt2*sin(ugp);
      //смещение по второму тангенсу равно с минусом его изменению
    // xv=xvisx+dt1*cos(ugl);
    // yv=yvisx+dt1*sin(ugl);
     //для нового центра новая ВУ по углам она используется
      //сделал новую центральную точку и ВУ
//fprintf(f1,"\nnew point vverx pr=%d,k=%d,t1=%7.2lf,t2=%7.2lf",pr,k,t1,t2);
//fprintf(f1,"\npov=%d, dt1=%7.3lf,dt2=%7.3lf,def1=%7.3lf,def2=%7.3lf",
  //             pov, dt1,dt2,def1,def2);
//fprintf(f1,"\nnew point vver xv=%7.2lf,yv=%7.2lf",xv,yv);
       prvu=proverka(xv,yv,0.0,i,nverp+nach);
//fprintf(f1,"\n vverx prvu=%d,i=%d,def1=%7.2lf,def2=%7.2lf",
  //prvu,nver,def1,def2);
   if(prvu) continue;//break; //куда? на сдвиг вниз,но лучше его не делать
   //хорошо при 0,1-внутри кривой 2-велика  биссектриса Bcmin
   //Если 2(велика биссектриса), нельзя то надо бы по углам!!!!
      if((def1>0.)||(def2>0.)){pouglam(i,nverp+nach);break;}
     postr(xl,yl,xp,yp,xv,yv,Bc,i,nverp+nach,0.0);
       pouglam(i,nverp+nach); //проверил после шага вверх
 //fprintf(f1,"\nцентр вверх проверен pr=%d,k=%d,dt1=%7.3lf,dt2=%7.3lf",
   //  pr,k,dt1,dt2);
   }//FOR k
    for(int k=1;k<=npr;k++)
   { ntoch=-k;//вниз с минусом
         nugl=0;
     if( (masnorm[nver].point[ntoch+npr].ukp==NULL) ||
       ( masnorm[nver].point[ntoch+npr].ukp[nug+nugl].sv==NULL))
       continue;// на левой нет информации
    dt1=-k*dpr*cos(teta)/sin(teta); //нельзя /tan при pi/2!
      if(-dt1>t1isx) break;// вершина ушла за левую точку
  dt2= k*dpr/sin(teta);//было наоборот , ошибка
   if(-dt2>t2isx-lprmin) break;// вершина ушла за правую точку
      def1=def1isx-dt1;// добавка с минусом dpr/sin(teta)i
  //был +dt1 противоречие! надо -dt1 9добавка к тангенсу dt1 деф=надо -есть
      def2=def2isx-dt2;//добавка dpr/tan(teta). новый def=def-добавка
  //   if((def1>0.)||(def2>0.))break;
       t1=t1isx+dt1;
       t2= t2isx+dt2;
       xl=xlisx+k*dpr*sin(ugl);yl=ylisx-k*dpr*cos(ugl);
        Tmax=t1;
    if(Tmax>t2-lprmin)Tmax=t2-lprmin;//в posl не обязательна lprmin
      //сделал новую центральную точку
      xv=xvisx-dt2*cos(ugp); //смещение по второму тангенсу равно с минусом его изменению
     yv=yvisx-dt2*sin(ugp);
     //xv=xvisx+dt1*cos(ugl);
     //yv=yvisx+dt1*sin(ugl);
//   fprintf(f1,"\nnew point vniz pr=%d, k=%d,t1=%7.2lf,t2=%7.2lf",
  //                    pr,k,t1,t2);
   //fprintf(f1,"\ndt1=%7.3lf,dt2=%7.3lf,def1=%7.3lf,def2=%7.3lf",
     //             dt1,dt2,def1,def2);
    //fprintf(f1,"\nnew point vniz xv=%7.2lf,yv=%7.2lf",xv,yv);
    prvu=proverka(xv,yv,0.0,i,nverp+nach);
    //1-внутри кривой 2-велика  биссектриса Bcmin
    //fprintf(f1,"\nperv vniz prvu=%d,nver=%d,def1=%7.2lf,def2=%7.2lf",
  //prvu,nver,def1,def2);
   if(prvu) continue;//break;
   //на след точку той же нормали
      if((def1>0.)||(def2>0.)){pouglam(i,nverp+nach);break;}
      postr(xl,yl,xp,yp,xv,yv,Bc,i,nverp+nach,0.0);
       pouglam(i,nverp+nach); //проверил после шага вниз
    }//for k минус
      }//for nver
   }//poslnew

   int proverka(double xv,double yv,double du,int n2,int n3)
  { // n2,n3-начало и конец поиска в иcходной нумерации 0 ->nach+i+1?
  double t,a,p,d2,s1,s2,tetanew; //int nminotkl;
  if(!pov)tetanew=teta+du;else tetanew=teta-du;
  if(tetanew*teta<0.) return 1;
  if(fabs(tetanew)<tetamin)return 1;
   t=(lmin+lkmin)/fabs(tetanew);
  if( t<Rmin)t=Rmin;
  //при малом угле получал большой радиус при lkmin>100
   Rmin1=t;
   a=fabs(tetanew/2.);
    p=lmin*lmin/(24.0*Rmin1);
 Bcmin=Rmin1*(1.0/cos(a) -1.0)+p/cos(a);
   //пересчёт Bcmin есть в сorrtg
   //но он не вызывается при больших тангенсах там его можно убрать
/*
        Bc=1.0e7;
 for( int i=n2;i<n3;i++)
{
//Проверка попадания всех точек отlev prav
d2= rast(xt[i],yt[i],xv,yv);
if(d2<Bc){Bc=d2;nminotkl=i;}
//это не биссектриса так как не по центру
// принять за биссектрису а направление вычислить по углам
 }//for
 s1=otpr(xl,yl,xv,yv,xt[nminotkl],yt[nminotkl]);
 s2=otpr(xv,yv,xp,yp,xt[nminotkl],yt[nminotkl]);
// s1=otpr(xt[n2],yt[n2],xv,yv,xt[nminotkl],yt[nminotkl]);
 //s2=otpr(xv,yv,xt[n3],yt[n3],xt[nminotkl],yt[nminotkl]);
 //fprintf(f1,"\nproverka n2=%d,n3=%d,nmin=%d,Bc=%7.2lf,teta=%8.6lf",
   //           n2,n3,nminotkl,Bc,teta);
 //fprintf(f1,"\nproverka s1=%6.2lf,s2=%6.2lf",s1,s2);

  if((s1*tetanew<0.0||s2*tetanew<0.0))//не внутри угла  а где-то
   {
 fprintf(f1,"\nвнутри кривой,s1=%5.2lf,s2=%5.2lf,n2=%d,n3=%d,xv=%7.2lf,yv=%7.2lf,nmin=%d,Bc=%7.2lf",
                    s1,s2,n2,n3,xv,yv,nminotkl,Bc);
 fprintf(f1,"\n xt[n2]=%9.2lf,yt[n2]=%9.2lf,xt[n3]=%9.2lf,yt[n3]=%9.2lf,xt=%9.2lf,yt=%9.2lf",
                  xt[n2],yt[n2],xt[n3],yt[n3],xt[nminotkl],yt[nminotkl]);
  //             return 1;
    }
    */
//использую это не рихтовки, а отклонения!   1m и Bc+maxrix <Bcmin не возможна
 // Bcmin нельзя использовать старую при изменении угла поворота
//пересчитал при углах в corrtg, а его не вызывали при сдвиге и больших t1
  //return 0;
   for( int i=n2;i<n3;i++)
   {
  s1=otpr(xl,yl,xv,yv,xt[i],yt[i]);
  s2=otpr(xv,yv,xp,yp,xt[i],yt[i]);
   if(s1*tetanew<0.0 && fabs(s1)>maxrix)return 1; //вне левой и далеко
   if(s2*tetanew<0.0 && fabs(s2)>maxrix)return 1;
   /*
  d2=s1*s2;
  if(d2>0.)continue;
  if(fabs(d2)<0.0001) continue;
  if(d2<0.0)
  {  if(s1*tetanew<0.0)
       {if(fabs(s1)>maxrix ) return 1;else continue;}//1 sektor
       else
       { if(fabs(s2)>maxrix) return 1;else continue;}//>0 3sektor
   } else
  { a=rast(xt[i],yt[i],xv,yv);if(s1*tetanew<0.0 && a>0.3*maxrix)
   return 1;
   }//d2>0
     */
   }//for
  /*
   if(Bc+maxrix<Bcmin)  // ближайшая к вершине Bc а при мин рад Bcmin
   {//fprintf(f1,"\n\nВелика биссектриса Bc=%7.2lf,Bcmin=%7.2lf,Rmin1=%7.2lf,maxrix=%7.2lf",
      //      Bc,Bcmin,Rmin1,maxrix);
   return 2;
   }
   */
    return 0;
  }//proverka

  int postr(double xl,double yl,double xp,double yp,double xvnew,
    double yvnew,double &Bc, int n2,int n3,double du)
  { double t, T,a,b,c,sint,cost,tant,R,R1,L; //double Tmin1;
    double r1,r2,r3,Ropt,Lopt,sumopt;// пределы для  при фиксированной клотоиде L
  //L1-длина левой клотоиды,  L2-правой  пока обе по lmin
  //xvnew,yvnew это a,b из corrtg с учётом поворота как параметр xv,yv- глобальная
 //осталась после смещения но до поворота.
 // Bc из proverka раст от ВУ ближней точки не использую но меняю
   int j, k;  double tetanew,uglnew,ugpnew,Rmax1,d;
  double  sum1,s2,s3,s4,b1,b2,db=0.02;// пределы бисс
  int  bL; //int pvv;  //double dR1,dR2;//,jdl=3,jbt=10;
    //Tmax( global) вычислил в corrtg  сначала в VUxy
  //мин расст от ВУ ДО ТОЧКИ НА CТОРОНЕ УГЛА
   //t1,t2-глобальные расст от ВУ Меняются при поворотах и сдвигах!!!
      //Rmin1 ,Bcmin-global
    if(!pov){tetanew=teta+du; uglnew=ugl;ugpnew=ugp+du;}
      else {uglnew=ugl+du;ugpnew=ugp;tetanew=teta-du;}
  if(fabs(tetanew)<tetamin)return 1;
  if(teta*tetanew<=0.0) return 1;
  ugb=uglnew+tetanew/2.0;
 if (tetanew>0)ugb=ugb+pi2;else ugb=ugb-pi2;
  //Rmin1 вычислен и пришёл. это по мин клот и углу
   //Tmin1=(Rmin1+lmin*lmin/(24.*Rmin1))*tan(fabs(tetanew/2.))+lmin/2.0;
   sint=sin(fabs(tetanew/2.));cost=cos(fabs(tetanew/2.));
   tant=sint/cost;
    // s2=(lmin+lkmin)/fabs(tetanew);//test
    int jnach,jkon,id1,id2; double d1,d2;
     if(!pov ){jnach=1;jkon=nver+nach;}
  else {jnach=nver+1+nach;jkon=nverp+nach;}//границы по нормалям
L1=L2=L=lmin;    sumopt=1.e8; Ropt=1.e8;
 // s1=rast(xvnew,yvnew,xl,yl); //s2=rast(xvnew,yvnew,xp,yp);

   //для КАЖДОЙ НОВОЙ L (клот) нужны Rmin1,Rmax1,Bcmin,Bcmax
  // по известным L,teta,lk(окр)и Tmax+lmax,Rmax
 // int obrclot(double L,double teta,double Tmax)
   //даёт глобальные Bcmin, Bcmax,Rmin1 для новой длины клот `L
//при макс тангенсе: j=0-хорошо,1- радиус больше максимального
 //5-  при этой клотоиде мин длинах и Rmax не вписаться в угол и тангенс
 //global Bcmin, Bcmax,текущие L,Bc,t,R
     double s3old;//,sumold;
     dopt=1.e8;//max отклонение global наилучшее
     bL=0;//число L с превышением полосы( недопустимые подряд )
     // Tmax-пришёл по мин расст от ВУ до концов
      s4=lmax; if(s4>2.*Tmax)s4=2.*Tmax;
    while (L<=s4)
    {dtek1=1.e8;// в Unit1.h останется если нет допустимого при данной L
   k=   obrclot( L, tetanew, Tmax,t,Rmax1);
   //Rmin1, Bcmin по L и teta миним Bc=Bcmin;  см ниже
  //Bcmax - биссектриса R1 радиус при Tmax далее корр по Rmax
  // даёт Rmin1,R1 при L,Tmax t-min тангенс при Rmin1;k==1 при R1>Rmax corr
   //для sum-mod1 надо тангенс и радиус   для мин считает повторно
     if(k==5)break;
  // при L радиус меньше минимального (угол мал) за цикл по L
   if(Bcmin>Bcmax){fprintf(f1,"\nBcmin>%7.3lf,Bcmax=%7.3lf,Rmin1=%7.2lf,Tmax=%7.2lf",
                   Bcmin,Bcmax,Rmin1,Tmax); fclose(f1);exit(0);
                   }
      L1=L2=L;  int i; //для входа в цикл
   if((Bcmax-Bcmin)<0.2)    //на перебор
  { b2=Bcmin; b1=Bcmax; goto M;}
    s3old=0.;
    if(dopt>1.e7){ // не было радиуса Ropt
     R=(Rmin1+Rmax1)/2; // беру полусумму
     t= (R+L*L/24./R)*tant+L/2.;//тангенс
      Bc= R*(1.0/cos(tetanew/2.) -1.0)+L*L/24./R/cos(tetanew/2.);
                  }
    else
    {
     //Rmax1-max радиус по Tmax или Rmax при большом Tmax
    //Bcmax-биссектриса при Rmax1
  //Проверка Ropt уже бывшего, а полусумму не надо проверять
     if(Ropt<Rmin1){R=Rmin1;Bc=Bcmin;} //t- остался
     else
     {
       if(Ropt>Rmax1)
       {R=Rmax1;Bc=Bcmax;
       t= (R+L*L/24./R)*tant+L/2.;//тангенс
       } else
      { R=Ropt;  t= (R+L*L/24./R)*tant+L/2.;
   Bc= R*(1.0/cos(tetanew/2.) -1.0)+L*L/24./R/cos(tetanew/2.);
      }
     }
     }//Ropt был

   for ( i=0;i<7; i++) //при новой L опять с минималки зачем?
   {   //поиск оптимальной биссект и радиуса при фикс L
       if(t>Tmax+0.01)
    {fprintf(f1,"\n1 postr t>%8.4lf,t=%8.4lf,R=%9.3lf,L=%7.2lf,Bc=%8.3lf,Bcmax=%8.3lf",
    t,Tmax,R,L,Bc, Bcmax);
    fprintf(f1,"\n pov=%d,nver=%d,nverp=%d,ntoch=%d,ntochp=%d,nug=%d,nugp=%d",
                  pov,nver,nverp,ntoch,ntochp,nugl,nuglp);
    }
 j=sum_mod1(t, xvnew,yvnew,xl,yl,xp,yp,uglnew,R,tetanew,sum,jnach,jkon,d1,d2,id1,id2,1);
//j==1 браковать L сразу нельзя так как есть другие Bc
//d1,d2 спроектированные на биссектрису макс и мин рихтовки d3,d4 -без проектироапния
//fprintf(f1,"\nisum i=%d,j=%d, d1=%7.2lf,d2=%7.2lf,d3=%7.2lf,d24=%7.2lf,id1=%d,id2=%d,sum=%7.2lf",
    //           i,j,d1,d2,d3,d4,id1,id2,sum);
 if(!j)
 {
 if(sum<sumopt ){sumopt=sum;Ropt=R,Lopt=L;dopt=dtek;}
 dtek1=dtek;//чтобы при отсутствии допустимого вар-та не увеличивать L
  } // при j==1 надо смотреть другие биссектирисы
//  d1=d3; d2=d4;// proba  d1,d2 с углами сдвижек d3,d4 просто сдвижки
 // найти пределы по Bc и перебор
//j вернётся 5 при больших рихтовках на первой прямой и уйти
 if(d1*d2<0.0)
 {  if(fabs(d1-d2)>2.5*maxrix )  break;//без k=5
 //за цикл по i  k=5 не дал увеличить L
 //b1-max Bc, b2-min Bc d1-max rixt, d2-min
  b1=d1;if(d1>d2+maxrix)b1=d2+maxrix;
 //вверх на maxrix это макс риxтовка в старом смысле
 ///т.е. увеличение минимальной сдвижкой  в сторону увеличения рихтовок
  b2=d2; if(d2<d1-maxrix)b2=d1-maxrix; // уменьшение максимальной
  // для выпуклой это же приращения биссектрисы
 if(b2>b1)
 {s1=b1; b1=b2;b2=s1;}
 if(tetanew<0.){b1=Bc+b1;b2=Bc+b2;}else {s1=b1; b1=Bc-b2;b2=Bc-s1;}
  //b1,b2 макс и мин значения биссектрисы после  предельных сдвижек
  if(i)  { s1=Bc-s3old; if(s3old>0. && b2<s1)b2=s1;
  //пришли с увеличением и нет смысла быть ниже того откуда пришли
         if(s3old<0. && b1>s1)b1=s1;
         }  // коррекция с учётом прошлой сдвижки
 // а где она запоминалась при разных? нигде цикл при разных не повторяется!
   if(b1<Bcmin||b2>Bcmax)break;//after for i на увеличение L
    b1=b1+0.1; b2=b2-0.1;//запас на неточность d1,d2
   if(b1>Bcmax)b1= Bcmax;
     if(b2<Bcmin)b2= Bcmin;
 if(b2>b1)
  {
 fprintf(f1,"\ni=%d,b2=%8.5lf,b1=%8.5lf,Bcmin=%9.5lf,Bcmax=%9.5lf,Bc=%9.5lf",
         i,b2,b1,Bcmin,Bcmax,Bc);
 fprintf(f1,"\nTmax=%8.4lf,R=%9.4lf,Rmax1=%9.4lf,Rmin1=%9.4lf",
                       Tmax,R,Rmax1,Rmin1);
 fprintf(f1,"\nd1=%7.3lf,d2=%7.3lf,tetanew=%9.6lf",d1,d2,tetanew);
 ShowMessage("Ошибка в расчёте биссектрисс");
 fclose(f1);exit(0);
 }
 M: Bc=b2; //db так как  с Bcmin  уже сработали!!?? Не всегда!!!!
  int i1; i1=0;
    while(Bc<=b1+0.0001) //ДАЛЕЕ ПЕРЕБОР ОТ МИН К МАКС
     {
  bisclot1(Bc,L, tetanew,R,R1,t);  //t,R1-тангенс и радиус по L,Bc
  R=R1;
  if(t>Tmax+0.01)
    {fprintf(f1,"\n2 postr t>%8.4lf,T=%8.4lf,R1=%9.3lf,L=%7.2lf,Bc=%8.3lf",
    t,Tmax,R1,L,Bc);
//    fclose (f1);
    }
  j=sum_mod1(t, xvnew,yvnew,xl,yl,xp,yp,uglnew,R,tetanew,sum,jnach,jkon,d1,d2,id1,id2,1);
    if(!j)
 {
   if(sum<sumopt )
    {
      sumopt=sum;Ropt=R,Lopt=L;dopt=dtek;
    }
 dtek1=dtek;//чтобы при отсутствии допустимого вар-та не увеличивать L
  } //
// >maxrix не допускаю и оптимум запоминаю
     if(i1)break;  //конец перебора
//корректировка с использованием d1,d2

 if (tetanew<0.)
 { if(d1<0.)break;
   if( Bc+d1<b1)b1=Bc+d1;
 } else
  { if( d2>0.)break;
    if(Bc-d2<b1)b1=Bc-d2;
  }
       Bc=Bc+db;
     if(Bc>b1){Bc=b1; i1=1;}
    }//while  Bc<=b1
   break;   //after for i=7  на другую L
  }//разные
  //if(i>2)ShowMessage("Есть i="+IntToStr(i));
 //сюда при совпадении знаков мин и макс отклонений посчитана Bc
  s3=(d1+d2)/2;//по новому
 if(tetanew>0.)s3=-s3;// изменение биссектрисы
   if(Bc+s3>Bcmax){if(Bcmax-Bc<0.01)break;//за for i 7 on new L
                else { s3=Bcmax-Bc; Bc=Bcmax;
          s3old=s3;   t=Tmax;  R=Rmax1; //sumold=sum;
          if(k==1)t=(Rmax1+L*L/24./Rmax1)*tant+L/2.;
            continue; //на увеличение i
     //для неё тангенс t, а радиус Rmax1  запомнили!!
    //новая биссектриса по ней новый радиус и тангенc и sum_mod
                       }
                     } //Bc+s3>Bcmax
   if(Bc+s3<Bcmin)
   {if(Bc-Bcmin<0.01)break;//т.е.Bcmin уже обсчитали  с этой L
       else  s3=Bcmin-Bc; //s3=(Bcmin-Bc)/2;
     //новая биссектриса по ней новый радиус и тангенс и sum_mod
    }  //<Bcmin
        // а если меньше двинуть
     if(fabs(s3)<0.01)break;
 //за for i с этой переходной закончили
    Bc=Bc+s3;
   s3old=s3;//sumold=sum;
   bisclot1(Bc,L, tetanew,R,R1,t);  //t,R1-тангенс и радиус по L,Bc
    R=R1;
     }//end for i 7
 //dtek-из summod max! сдвижка, dtek1-в него dtek при удачном summod
 // dopt-минимальная из максимальных сдвижка для данного угла R,L,Bc
  //далее увеличение L  dtek1>maxrix только при негодной L
  // if(k==5) break; не приходим с 5
   if(dtek1>maxrix) {if(bL>7)break; else bL++;}
      else {
// if(bL>7)fprintf(f1,"\nзап bL=%d, dtek1=%9.7lf,L=%5.0lf,s4=%5.1lf",
 //bL,dtek1,L,s4);
 bL=0;
            }
   // bL-число  превышений  полосы подряд по L
     L=L+dL;
     }//while L

   if(dopt>maxrix) return 1; //только при отсутствии допустимой клотоиды
     sum=sumopt;//for zaptek
  T=tangen1(Lopt, Ropt,tetanew,1);
  R1=Ropt;L1=L2=Lopt; Lk=Ropt*fabs(tetanew)-L1;
   a=xvnew-T*cos(uglnew);//начало левой переходной  ugl?
   b=yvnew-T*sin(uglnew);
   c=xvnew+T*cos(ugpnew);
   d1=yvnew+T*sin(ugpnew); //конец правой переходной для записи
 def1=-(rast(xl,yl,xvnew,yvnew)-T);
 def2=-(rast(xp,yp,xvnew,yvnew)-T-lprmin);
  zaptek (pov,R1,tetanew,a,b,c,d1,xvnew,yvnew,xp,yp,ugpnew);
      return 0;
   }//postr
///
  void pouglam(int n1,int n3)
  { // du -поворот правой при pov==0 и левой при pov=2
      double a,b;int k;
   if(def1prev<=0 && def2prev<=0.)k=0; else
  { if( teta>0. && teta<pi/2.)//какой-то дефицит есть
    { if( def1prev>0.)k=1; else k=2;}else
   { if(teta>-pi && teta<-pi/2.)
    { if( def2prev>0.)k=1; else k=2;} else
  { if(teta>pi/2.&& teta<pi)
       {if(!pov)k=1; else k=2;} else
   { if ( teta<-pi &&teta<-pi/2.)
       {if(!pov)k=2; else k=1;}
   }}}}
 //k: 0-оба; 1-вверх; 2 - вниз
  if(k<2)
  {
   for (int j=1;j<=nug;j++)
      //по углам в к-ой точке начало с центральной
    {
    nugl=j;//номер угла
    s=j*dug; //global /pov ==0 только в perv
 if(pov && masnorm[nver].point[ntoch+npr].ukp[nug+nugl].sv==NULL)
 continue;
    icorr=corrtg(s,a,b,n1,n3);
      if(icorr==3) break;
      //нет смысла увеличивать дефицит или поворот дал малый угол <tetamin
   if(!icorr)postr(xl,yl,xp,yp,a,b,Bc,n1,n3,s);
    //меняется xv,yv
   }//конец поворота с увеличение угла
 }
  if(k==1)return;
 //if((def1prev<=0 &&def2prev<=0.)||

      def1prev=def1isx; def2prev=def2isx;
      for (int j=1;j<=nug;j++)
  {    s=-j*dug;//угол поворота
        nugl=-j;
    if(pov && masnorm[nver].point[ntoch+npr].ukp[nug+nugl].sv==NULL)
    continue;
       icorr=corrtg(s,a,b,n1,n3);
    if (icorr==3) break;
   if(!icorr)
      {//fprintf(f1,"\n\npouglam vniz Раcчёт!! icorr=%d,j=%d,s=%8.4lf",
         //            icorr,j,s);
        postr(xl,yl,xp,yp,a,b,Bc,n1,n3,s);
      }
    } //for j
  }//pouglam

    int corrtg(double du,double& a,double& b,int n1,int n3)
  { // использую t1,t2,teta,dpr,dug,def1,def2,Tmin-норма TG
  //t1, t2-расст. от ВУ до концов;на второй( вращение pov=0 c t2)
  //они пересчитаны за сдвиг, исходные сохранены
    double x1,x2,t,tetanew;   double p;
  // смещение по первой из-за поворота, новая минус старая t2
  //здесь вычисляю в a,b  новую ВУ после поворота а старая xv,yv  иду в postr
  if(!pov)tetanew=teta+du;else tetanew=teta-du;
  if(teta*tetanew<=0.0) return 3;
  if(fabs(tetanew)<tetamin)return 1;//они одного знака и можнл
  //1- продолжить изменение угла без postr
  //3-сменить направление поворота
  t=(lmin+lkmin)/fabs(tetanew);
  if( t<Rmin)t=Rmin;
  Rmin1=t;
  Tmin=(Rmin1+lmin*lmin/(24*t))*fabs(tan(tetanew/2.))+lmin/2.0;
    //новый тангенс  x1=t2*sin(du)/sin(teta+du);
   //сдвижка за поворот радиусом t2 из правой точки
    a=fabs(tetanew/2.);
    p=lmin*lmin/(24.0*Rmin1);
 Bcmin=Rmin1*(1.0/cos(a) -1.0)+p/cos(a);
 // Bcmin Пересчитана на новый угол нужен пересчёт с сохранением t1,t2
  //fprintf(f1,"\ncorr old t1=%6.2lf,t2=%6.2lf,tetanew=%7.4lf,du=%6.2lf,Bcmin=%6.2lf",
    //            t1,t2,tetanew,du,Bcmin);
 //fprintf(f1,"\nx1=%6.2lf,yl=%6.2lf,xp=%6.2lf,yp=%6.2lf",xl,yl,xp,yp);
 t=sin(du)/sin(tetanew);
  if(!pov)
  {    //вращаю правую х1-смещение по левой
  x1=t2*t;   //нет ошибки ! при тета<0
  if(-x1>t1) return 1;// вершина ушла за левую
        //уже   (-x1>t1-Tmin) плохо!!
// x2=t2*sin(teta)/sin(tetanew);//это new t2 а не х2
  //fprintf(f1,"\ncorrtg pov=%d, du=%5.2lf,t2=%5.2lf,x1pov=%8.3lf,Bcmin=%7.2lf",
    //          pov,du,t2,x1,Bcmin);
  //сдвижка по левому за поворот на du с центром в правой
  //радиус t2 по центр точке его пересчитывать при сдвигах
  a=xv+x1*cos(ugl);  //new vu xv,yv-после смещения теперь за поворот
  b=yv+x1*sin(ugl);
  }else // pov==2; т.е.вращаю левую
  {
   x2=t1*t; //новый тангенс нет это приращение правого при повороте левого
    if(x2>t2-lprmin) return 1;
// вершина ушла за правую при полож. смещении. за левую не может
    //fprintf(f1,"\ncorrtg pov=%d, du=%5.2lf,t2=%5.2lf,x2pov=%8.3lf,Bcmin=%7.2lf",
      //         pov, du,t2,x2,Bcmin);
  //сдвижка по правoму за поворот на du с центром в левой
  //радиус t1 по центр точке его пересчитывать при сдвигах
   a=xv+x2*cos(ugp);  //new vu  смещение по второй (дальней)
   b=yv+x2*sin(ugp); //проверить
   }
// fprintf(f1,"\ncorrtg pov=%d,new vu a=xv=%7.2lf,b=yv= %7.2lf",pov,a,b);
 double d1,d2;//дефицит
 Tmax=rast(xl,yl,a,b); //global Tmax это новый t1
// t1,t2 пересчитываются за сдвиг, а за угол нет  и не надо!
 d1=Tmin-Tmax;// def по левому: надо минус есть
 t=rast(xp,yp,a,b)-lprmin; // это t2new-lprmin. всегда нужен справа
  d2=Tmin-t;  //d2 зачем?? см. ниже по ним выход это дефицит
 if(t<Tmax)Tmax=t;//нужен
   //вариант выхода 0-можно строить,1-на следующий угол
  //3-сменить направление вращения

  if( (d1>0.&& d1>def1prev)||(d2>0.&& d2>def2prev))
  { def1prev=d1;def2prev=d2; //деф. вырос надо вращать иначе
    return 3;
    }
    def1prev=d1;def2prev=d2;
  if((d1>0.) ||(d2>0.))return 1;//деф есть, но не вырос
 //пересчёт ВУ по левому тангенсу нужно пересчитать Tmin  и Tmax!!
//fprintf(f1,"\ncorrtg  xl=%7.2lf,yl=%7.2lf,xp=%7.2lf,yp=%7.2lf",
  //            xl,yl,xp,yp);
  //fprintf(f1,"\ncorrtg du=%6.2lf,d1=def1=%7.2lf,d2=def2=%7.2lf,Tmin=%6.2lf,Tmax=%6.2lf",
              //      du,d1,d2,Tmin,Tmax);
  //a,b-VU
//  if(proverka(a,b,du,n1,n3))return 2;else return 0;
  if(proverka(a,b,du,n1,n3))return 1;//на следующий угол
  return 0;
    }//corrtg
    //

int VUxy(double x1,double y1,double x2,double y2,double ug1,double ug2,
          double& xv,double& yv)
  { // 2020 берёт две точки и углы в них с осью х, даёт VU
     // и угол поворота  от первой ко второй прямой
        double k1,k2,x3,y3,x4,y4;//,pi=3.1415926;
            //работал при поворотах <pi  теперь нет
   //++++++++  //угол исходящий ugl0 ->из 0 в 1
  teta=ug2-ug1;  s=fabs(teta);
  if(s<tetamin)return 1; //tetamin рассчитан на Rmax(4000)
 /*
 { s1=tetamin-fabs(teta);
 fprintf(f1,"\n мал угол teta=%9.6lf,tetamin=%9.6lf,def=s1=%9.6lf",teta,tetamin,s1);
 fprintf(f1,"\npov=%d,nver=%d,nverp=%d,ntoch=%d,nugl=%d,ugl=%8.6lf,ug2=%8.6lf",
              pov, nver,nverp,ntoch,nugl,ug1,ug2);
fprintf(f1,"\n ntochp=%d,nuglp=%d,ugl=%8.6lf,ug2=%8.6lf,ugl=%8.6lf,ugp=%8.6lf",
               ntoch,nuglp,ug1,ug2,ugl,ugp);
 // ShowMessage("мал угол teta");
 // fclose(f1);
    return (1);//на следующую нормаль  НЕТ на поворот
  }
   */
          if(s>=pi)
  {fprintf(f1,"\nteta>pi ug1=%8.6lf,teta=%8.6lf,nl=%d,np=%d,nt=%d,np=%d,nu1=%d,nu2=%d",
     ug1,teta,nver,nverp,ntoch,ntochp,nugl,nuglp);
      ShowMessage("  поворот больше pi");
      fclose (f1);   exit (0);
      //  return 1;
    }//s>=pi
    if (s<0.00001 || fabs (s-pi)<0.00001 )
  {fprintf(f1,"\n VUxy параллельность ugl=%8.6lf,ugp=%8.6lf",ug1,ug2);
     ShowMessage("Параллельность");
  return 1;//считаю что при этих неувязках   perv return posl return
   }
   //++++++++

     x3=x1+cos(ug1);y3=y1+sin(ug1);// точка на первой на расст. 1
     x4=x2+cos(ug2);y4=y2+sin(ug2);// точка на 2-ой на расст. 1
     if(fabs(x3 -x1)<0.001)// первая вертикальна
     {
       xv=x1; yv=y2+tan(ug2)*(x1-x2);
      }// если первая вертикальна, то вторая нет, т.к. нет параллели
      if(fabs(x4 -x2)<0.001)//вторая вертикальна
        {xv=x2; yv=y1+tan(ug1)*(x2-x1);}//вторая верт а первая нет
        else //нет вертикалей
        { k1=tan(ug1);k2=tan(ug2);
          xv=(k1*x1-y1-k2*x2+y2)/(k1-k2);
           yv=k1*(xv-x1)+y1;
        }
        //при s<pi ВУ от 1 в направлении , а от 2 против
         //при s>pi  наоборот

      if((((x1-xv)*(x1-x3)<=0)||((x2-xv)*(x2-x4)>=0))&&
         (((y1-yv)*(y1-y3)<=0)||((y2-yv)*(y2-y4)>=0)))
   {      /*
fprintf(f1,"\nVUxy1045 vu за пределами xv=%10.2lf,yv=%10.2lf,ug1=%9.6lf,ug2=%9.6lf",
     xv,yv,ug1,ug2);
  fprintf(f1,"\n x1=%10.2lf,y1=%10.2lf,x2=%10.2lf,y2=%10.2lf",x1,y1,x2,y2);
  fprintf(f1,"\n x3=%10.2lf,y3=%10.2lf,x4=%10.2lf,y4=%10.2lf",x3,y3,x4,y4);
  //   if(s<pi)       ShowMessage("1 teta< pi и vu за пределами ");
fprintf(f1,"\nnver=%d,ntoch=%d,nugl=%d,nverp=%d,ntochp=%d,nuglp=%d",
             nver,ntoch,nugl,nverp,ntochp,nuglp);
                // fclose(f1); exit(0);
     */
 return 1;// на след нoрмаль левую
    }
     Rmin1= (lmin+lkmin)/fabs(teta);
  if(Rmin1<Rmin) Rmin1=Rmin;// иначе не хватает длины ( накладка)
   double a,p;
    a=fabs(teta/2.);
    p=lmin*lmin/(24.0*Rmin1);
 Bcmin=Rmin1*(1.0/cos(a) -1.0)+p/cos(a);
 // fprintf(f1,"\nVU Bcmin=%7.2lf,Rmin1=%7.2lf",Bcmin,Rmin1);
 //Tmin =(Rmin1+p)*tan(a);//без lmin/2
   s3= t1=rast(x1,y1,xv,yv); // а слева большой запас ??!!!
   t2=rast(x2,y2,xv,yv); s2=t2-lprmin;
 if(s2<t1)s3=s2;//min расст от вершины это и есть максимально допустимый тангенс,
 //но без учёта ситуации слева, где может быть большой запас!!!
 Tmax=s3;//max допустимый тангенс
  s4= (Rmin1+p)*fabs(tan(teta/2.0))+lmin/2.0; //требуется
 //fprintf(f1,"\nVUРасст. от ВУ t1=%9.2lf,t2=%9.2lf",t1,t2);
  def1=s4-t1;def2=s4-s2;

   //fprintf(f1,"\n VU def1=%5.2lf,def2=%5.2lf",def1,def2summod);
   if(s3<s4)//есть невязка
  { //норма минус наличие если>0 то дефицит иначе запас по тангенсу
     //fprintf(f1,"\nне влезает мин.тангенс на 1 =%9.2lf,на 2=%9.2lf,надо %9.2lf",
       //         t1,s2,s4);
    //if(t1<s2)fprintf(f1,"\nне влезает мин. тангенс на первой прямой");else
      //   fprintf(f1,"\nне влезает мин. тангенс на второй прямой");
  return 2;
  }

   return 0;
  }//-VUxy
  //n++;// в extern на 1 меньше
//++

  int sum_mod(double T1,double xvnew,double yvnew,double xlnew,double ylnew,
     double xpnew,double ypnew,double uglnew,double R1,double tetanew,
     double &sum1,int jnach,int jkon,double &d,int prp)
         //s2,s3-слева и справа не нужны, d- окружность нужен
{     double k,xk,yk,sig,xk1,yk1,cross,phi1,sig1,s1,t;
       double a,b; //xv,yv,xl,yl-global
   double r,xc,yc,ugpnew;// радиус со знаком и центр окружности;ugpnew надо передаваьб
   int k1,pp,np;//для вычисл d-средней рихт на окружноти при prp==1
   sum1=d=0.0;  k1=0;

     a=xvnew-T1*cos(uglnew);
     b=yvnew-T1*sin(uglnew);
   s1=(xlnew- a)*cos(uglnew)+(ylnew-b)*sin(uglnew);
  if(s1<-0.01)  //был 0 это ошибка при начале клотоиды без прямой
   {//есть прямая   0.37  начальная точка до начала клотоиды (a,b)
   xk=(xt[jnach]- xlnew)*cos(uglnew)+(yt[jnach]- ylnew)*sin(uglnew);
   if(xk<-0.001) //точка съёмки до начальной нормали
   {
 fprintf(f1,"\n do ln1 nverp=%d,ntochp=%d,nuglp=%d,jnach=%d,xk=%8.4lf,T1=%7.2lf",
                     nverp,ntochp,nuglp,jnach,xk,T1);
fprintf(f1,"\n nver=%d,ntoch=%d,nugl=%d,xlnew=%6.2lf,ylnew=%6.2lf,uglnew=%9.6lf",
                    nver,ntoch,nugl,xlnew,ylnew,uglnew);
fprintf(f1,"\nxt=%7.2lf,yt=%7.2lf,xvnew=%7.2lf,yvnew=%7.2lf,a=%7.2lf,b=%7.2lf",
   xt[jnach],yt[jnach],xvnew,yvnew,a,b);
   fclose (f1);       exit(0);
    }
        //далeе цикл по старому с пересечениями
 for (int j=jnach;j<=jkon;j++)
  {
   // возврат: 1-внутри,2,3 ровно по концам
 // -2 левее левого  -3 правее правого
 pp=  crossln(xlnew,ylnew,a,b,xt[j],yt[j],beta[j],cross);
 if(!pp ||pp==-2)
 {  s1= rast(xlnew,ylnew,xvnew,yvnew); s2= rast(a,b,xvnew,yvnew);
 fprintf(f1,"\nсбой в  crossln1 pov=%d,pp=%d,j=%d,jnach=%d,L1=%7.2lf,L2=%7.2lf",
             pov,pp,j,jnach,L1,L2);
  fprintf(f1,"\nln1 jnach=%d,T1=%7.3lf,a=%7.3lf,b=%7.3lf,uglnew=%8.6lf",
                 jnach,T1,a,b,uglnew);
 fprintf(f1,"\nxl=%7.3lf,yl=%7.3lf,xvnew=%7.3lf,yvnew=%7.3lf,t1=%7.3lf,s1=%7.3lf,s2=%7.3lf",
           xlnew,ylnew,xvnew,yvnew,t1,s1,s2);
 fprintf(f1,"\n xt[j]=%7.2lf,yt[j]=%7.2lf,beta[j]=%8.5lf,uglnew=%7.5lf",
                     xt[j],yt[j],beta[j],uglnew);
 fprintf(f1,"\n nver=%d,nverp=%d,ntoch=%d,ntochp=%d,nugl=%d,nuglp=%d",
                    nver,nverp,ntoch,ntochp,nugl,nuglp);
    fclose (f1);    exit(0);
   return pp;
 }
   if(pp==-3){jnach=j; break;}//на след поперечник
      s1=fabs(cross);
   //  if(s1>maxrix) { sum1=cross;return 1;}
      sum1=sum1+s1;
     if(pp==1 ||pp==2) continue;
     if(pp==3){jnach=j+1; break;}
    }//for конец прямой


  } //есть прямая   s1<
   sig=1.0/R1; if(tetanew<0)sig=-sig;
       k=sig/L1;//  знак верно
   conl(a,b,uglnew, 0.0, k,L1,&xk,&yk,&phi1,&sig1);

//начало замены на пересечения по бета
 for (int j=jnach;j<=jkon;j++) //jnach-первый поперечник на клотоиде
    { // начальная кривизна здесь всегда ноль
 pp=crossclonew( a,b,uglnew,0.0, k,L1,xt[j],yt[j],beta[j],cross);
        if(!pp ||pp==-2)
 { fprintf(f1,"\n сбой в  crossclo pp=%d, jnach=%d",pp,jnach);
   //return pp;
    fclose (f1); exit(0);
 }
   if(pp==-3){jnach=j; break;}// ?на следующую окруж
       s1=fabs(cross);
  //   if(s1>maxrix) { sum1=cross;return 1;}
      sum1=sum1+s1;
       if(pp==1 ||pp==2) continue;
     if(pp==3){jnach=j+1; break;}
     }//for crossclo
//конец  замены на пересечения по бета
//  если вышли изнутри клотоиды по jkon то всё плохо
  // s1=uglnew+k*L1*L1/2.;
//phi1 угол в конце левой клотоиды t-угол в конце окружности
    Lk=fabs(tetanew)*R1-(L1+L2)/2.0; //длина дуги окружности
    if(Lk<0.)
     {fprintf(f1,"\n Lk=%7.2lf,pov=%d,nverp=%d,ntochp=%d,nuglp=%d,R1=%8.4lf",
                    Lk,pov, nverp,ntochp,nuglp,R1);
 fprintf(f1,"\n  nver=%d,ntoch=%d,nugl=%d,tetanew=%8.5lf",
                    nver,ntoch,nugl,tetanew);
 fprintf(f1,"\nL1=%7.2lf,L2=%7.2lf",L1,L2);
    }
     conla( xk,yk,phi1 ,sig1, Lk, &xk1,&yk1,&t);
    //sig со знаком
  //начало пересечения с окружностью это замена на бета
  np=0;//нужен в  crossokrnew при повторных вызовах он используется
    //sig это R1 со знаком
     for (int j=jnach;j<=jkon;j++)
     {
 pp=crossokrnew(xk,yk,phi1,xk1,yk1,1./sig,xt[j],yt[j],beta[j],cross,np);

         if(!pp ||pp==-2)
 {fprintf(f1,"\nсбой  в  crossokrnew pp=%d, jnach=%d,jkon=%d,j=%d",
    pp,jnach,jkon,j);
      fprintf(f1,"\nnp=%d,xk=%7.2lf,yk=%7.2lf,s1=%8.6lf,xk1=%7.2lf,yk1=%7.2lf,r=%7.2lf",
                    np,xk,yk,s1,xk1,yk1,1./sig);
 fprintf(f1,"\n xt[j]=%7.2lf,yt[j]=%7.2lf,beta[j]=%8.5lf,uglnew=%7.5lf,t=%8.6lf",
                     xt[j],yt[j],beta[j],uglnew,t);
       //return pp;
       fclose (f1); exit(0);
     }
    if(pp==-3){jnach=j; break;}
   if(prp==1){d=d+cross; k1++;}  //для выч средней на окр
     s1=fabs(cross);
  //   if(s1>maxrix){ sum1=cross; return 3;}
      sum1=sum1+s1;
      if(pp==1 ||pp==2) continue; //это ближнее или дальнее пересечение
   // if(pp==3){jnach=j+1; break;}здесь такого нет но при след проходе будет -3
   }//for crossokr
 if(prp==1&& k1)d=d/k1;  //могли проскочить окружность

    //конец замены пересечения с окружностью по бета

   xk=xk1; yk=yk1;//нач угол ==t
  k=-sig/L2;//sig=1/R1 со знаком
   conl(xk,yk,t, sig, k,L2,&xk1,&yk1,&phi1,&sig1);
 // xk1=  cloxs_1 (xk, k, sig,t,L2);
  //yk1=  cloys_1 (yk, k, sig,t,L2);
 //начало замены за клотоиду 2 на пересечения по бета

  for (int j=jnach;j<=jkon;j++) //jnach-первый поперечник на клотоиде
    {
  pp=  crossclonew( xk,yk,t,sig, k,L2,xt[j],yt[j],beta[j],cross);
      if(!pp ||pp==-2)
 { fprintf(f1,"\n сбой в  crossclo2 pp=%d, jnach=%d",jnach,pp);
   //return pp;
   fclose (f1); exit(0);
 }
   if(pp==-3){jnach=j; break;}//на  следующую прямую
       s2=fabs(cross);
     //if(s2>maxrix) { sum1=cross; return 4;}
       sum1=sum1+ s2;
       if(pp==1 ||pp==2) continue;
     if(pp==3){if(j==jkon)return 0; else {jnach=j+1; break;}}
      }//for crossclo 2

   ugpnew=phi1;// по концу клотоиды но лучше передавать по тангенсу
     s1=rast(xk1,yk1,xpnew,ypnew);
  if (s1>0.001)// && pov==2)есть прямая большой цикл
  {  //xk1,yk1= начало прямой == конец клотоиды
    //при jnаch==jkon и повороте нормали (nugl1=0)КОНЕЧНАЯ ТОЧКА ПРЯМОЙ УХОДИТ ЗА
 // xpnew,ypnew в проекции )
       xk=(xt[jnach]- xk1)*cos(ugpnew)+(yt[jnach]- yk1)*sin(ugpnew);
 if( xk<-0.01) //до конца клотоиды а не на прямой за ней
   {
  fprintf(f1,"\n do ln2  pov=%d,nverp=%d,ntochp=%d,nuglp=%d,jnach=%d,jkon=%d",
                    pov, nverp,ntochp,nuglp,jnach,jkon);
 fprintf(f1,"\n nug=%d, nver=%d,ntoch=%d,nugl=%d,xpnew=%10.4lf,ypnew=%10.4lf",
                nug, nver,ntoch,nugl,xpnew,ypnew);
 fprintf(f1,"\nxt=%10.4lf,yt=%10.4lf,xk1=%10.4lf,yk1=%10.4lf,s1=%8.5lf,xk=%7.5lf",
             xt[jnach],yt[jnach],xk1,yk1,s1,xk);
 fprintf(f1,"\nR1=%12.6lf,L1=%7.2lf,L2=%7.2lf",R1,L1,L2);
  fclose (f1);
   exit(0);
   }

 //старое
  for (int j=jnach;j<=jkon;j++)
    {

pp=  crossln(xk1,yk1,xpnew,ypnew,xt[j],yt[j],beta[j],cross);
 if(!pp ||pp==-2)
 {
 fprintf(f1,"\nсбой ln2 pp=%d, pov=%d,nverp=%d,ntochp=%d,nuglp=%d,j=%d,jnach=%d",
                   pp, pov, nverp,ntochp,nuglp,j,jnach);
 fprintf(f1,"\nсбой ln2 nver=%d,ntoch=%d,nugl=%d,xpnew=%6.2lf,ypnew=%6.2lf",
                    nver,ntoch,nugl,xpnew,ypnew);
  fclose (f1);
   exit(0);
   return pp;
  }
   if(pp==-3&& j!=jkon)
  {fprintf(f1,"\nsum_mod пересечение за правым концом прямой 2");
  fprintf(f1,"\njnach=%d,j=%d,jkon=%d,xpnew=%7.3lf,ypnew=%7.3lf",
  jnach,j,jkon,xpnew,ypnew);
   fprintf(f1,"\npov=%d,nverp=%d,ntochp=%d,nuglp=%d,nver=%d,ntoch=%d,nugl=%d",
  pov,nverp,ntochp,nuglp,nver,ntoch,nugl);
  fclose(f1); exit(0);
  }

   //последнюю при -3 могли не взять это новое
     sum1=sum1+fabs(cross);
      if(pp==1 ||pp==2) continue;
     if(pp==3) break; // может ли зайти за конец трассы?
     //3 и -3 сдвижки в конце учтены
    }//for конец прямой
   } //есть прямая  если её нет, то в конце клотоиды взяли сдвижку
     return 0;
 }//sum_mod

 //+++++++++++++++++
  int sum_mod1(double T1,double xvnew,double yvnew,double xlnew,double ylnew,
  double xpnew,double ypnew,double uglnew,double R1,double tetanew,double&sum1,
     int jnach,int jkon,double &d1,double &d2,int &id1,int &id2,int prp)
         //d1,d2-max и min сдвижки,id1,id2-номера их элементов,prp-печать
{     double k,xk,yk,sig,xk1,yk1,cross,phi1,sig1,s1,t;
       double a,b,d; //xv,yv,xl,yl-global
   double r,xc,yc,ugpnew;// радиус со знаком и центр окружности;ugpnew надо передаваьб
  double spr1;//длина начальной прямой от xlnew,ylnew   до a,b
   dtek=sum1=0.0;d1=-1.e7;d2=-d1;;d3=d1;d4=d2; id1=id2=0;
   //in UNIT1h s1-s4,t
 int i1,i2,j3;double evpr5,evpr;

 if(!pov)s3=0.0; else s3=x[jnach-2];
 //s3-расст по хордам от начала трассы nver+ nach
 //до левой точки (nver+nach==jnach-1)x[0]-длина первой хорды
     a=xvnew-T1*cos(uglnew);
     b=yvnew-T1*sin(uglnew);
     if(T1>Tmax+0.002)
     {fprintf(f1,"\nsum T1>%8.4lf,Tmax=%8.4lf",T1,Tmax);
      fprintf(f1,"\nsum pov=%d,nver=%d,nverp=%d,ntoch=%d",
        pov,nver,nverp,ntoch);
     }
 i2=jnach;//исходный далее он первый за прямой
    spr1=-((xlnew- a)*cos(uglnew)+(ylnew-b)*sin(uglnew));

  if(spr1>0.01)
 {//есть прямая   0.37  начальная точка до начала клотоиды (a,b)
   xk=(xt[jnach]- xlnew)*cos(uglnew)+(yt[jnach]- ylnew)*sin(uglnew);
   if(xk<-0.001) //точка съёмки до начальной нормали
   {
 fprintf(f1,"\n do ln1 nverp=%d,ntochp=%d,nuglp=%d,jnach=%d,xk=%8.4lf,T1=%7.2lf",
                     nverp,ntochp,nuglp,jnach,xk,T1);
fprintf(f1,"\n nver=%d,ntoch=%d,nugl=%d,xlnew=%6.2lf,ylnew=%6.2lf,uglnew=%9.6lf",
                    nver,ntoch,nugl,xlnew,ylnew,uglnew);
fprintf(f1,"\nxt=%7.2lf,yt=%7.2lf,xvnew=%7.2lf,yvnew=%7.2lf,a=%7.2lf,b=%7.2lf",
   xt[jnach],yt[jnach],xvnew,yvnew,a,b);
  ShowMessage("Точка съёмки до начальной нормали");
  fclose (f1);
   exit(0);
    }

     for (int j=jnach;j<=jkon;j++)
  {
    xk=(xt[j]-a)*cos(uglnew)+(yt[j]-b)*sin(uglnew);
    if(xk>0.01){jnach=j;break;}
    //jnach первый за прямой и возможно за клотоидой
    //a,b-начало клотоиды
    cross=(xt[j]- xlnew)*sin(uglnew)-(yt[j]- ylnew)*cos(uglnew);
   s1= fabs(cross);
     sum1=sum1+s1;
     if(s1>dtek)dtek=s1;  // global dtek in *.h dtek=0 выше
       if(j==jkon)
 {fprintf(f1,"\nsummod1 DO CL1 прямая от jnach до jkon ущла на клот ,jnach=%d,jkon=%d",
        jnach,jkon);
   //
fprintf(f1,"\ndo cl1 nverp=%d,ntochp=%d,nuglp=%d,jnach=%d,jkon=%d,xk=%8.4lf",
                    nverp,ntochp,nuglp,jnach,jkon,xk);
 fprintf(f1,"\n   nver=%d,ntoch=%d,nugl=%d,a=%6.2lf,b=%6.2lf,uglnew=%8.5lf,tetanew=%8.5lf",
                    nver,ntoch,nugl,a,b,uglnew,tetanew);
 fprintf(f1,"\nL1=%7.2lf,L2=%7.2lf,R1=%9.5lf, T1=%7.4lf",L1,L2,R1,T1);
 fprintf(f1,"\nxlnew=%7.2lf,ylnew=%7.2lf,xvnew=%7.2lf,yvnew=%7.2lf,xpnew=%7.2lf,ypnew=%7.2lf",
   xlnew,ylnew,xvnew,yvnew,xpnew,ypnew);
   fprintf(f1,"\nxtjnach=%7.2lf,yt=%7.2lf,xtkon=%7.2lf,ytkon==%7.2lf,i2=%d",
   xt[jnach],yt[jnach],xt[jkon],yt[jkon],i2);
   fclose (f1);
   exit(0);
 //        return 1;
      }
   // против нормали идти к прямой это минус
   if(prp==2)
        {fprintf(f1,"\n ln1 j=%d,cross=%7.4lf,sum1=%7.4lf,dtek=%7.4lf",
        j,cross,sum1,dtek);
        }
    }//for конец прямой 1 Если >maxrix выйти с 5
     //и на другую нормаль или точку , угол  Нечего строить!
  } //есть прямая длиной spr1,если прямой нет, то spr1<0.01

    //далее с клотоидой
    sig=1.0/R1; if(tetanew<0)sig=-sig;
    k=sig/L1;//  знак верно
   conl(a,b,uglnew, 0.0, k,L1,&xk,&yk,&phi1,&sig1);
 // должно быть sig1 ==sig нашли конец клотоиды
double xkt,ykt,phikt,sigkt,xkt1,ykt1,phikt1,sigkt1,u,v;
  double uglt,x2;
 // в текущем перес нормали с клотоидой до и после сдвига
     xc=(xt[jnach]- a)*cos(uglnew)+(yt[jnach]-b)*sin(uglnew);
   if(xc<-0.001)
   {//Здесь плохо  точка левее начала клотоиды а если иду влево??
fprintf(f1,"\ndo cl1  pov=%d,nverp=%d,ntochp=%d,nuglp=%d,jnach=%d,jkon=%d,xc=%8.4lf",
                    pov, nverp,ntochp,nuglp,jnach,jkon,xc);
 fprintf(f1,"\n clo1 nver=%d,ntoch=%d,nugl=%d,a=%6.2lf,b=%6.2lf,uglnew=%8.5lf,tetanew=%8.5lf",
                    nver,ntoch,nugl,a,b,uglnew,tetanew);
 fprintf(f1,"\nL1=%7.2lf,L2=%7.2lf,R1=%9.5lf, T1=%7.4lf",L1,L2,R1,T1);
 fprintf(f1,"\nxlnew=%7.2lf,ylnew=%7.2lf,xvnew=%7.2lf,yvnew=%7.2lf,xpnew=%7.2lf,ypnew=%7.2lf",
   xlnew,ylnew,xvnew,yvnew,xpnew,ypnew);
   fprintf(f1,"\nxtjnach=%7.2lf,yt=%7.2lf", xt[jnach],yt[jnach]);
    fclose (f1);
    exit(0);
    }
 xkt=a; ykt=b;phikt=uglnew;sigkt=0.; //xk,yk -конец клотоиды
 if(spr1<0.01)spr1=0.;
    j3=0;
  for (int j=jnach;j<=jkon;j++)
      {
      u= xt[j];v=yt[j];
      s2=(u-xk)*cos(phi1)+(v-yk)*sin(phi1); //в конце клотоиды
      if(s2>0.0001){j3=j; break;}
       } //for j3 первый за клотоидой
   if(j3>jnach)
  {
    evpr5=0.;
   //i2== стaрый jnach  а новый это первый за прямой
  /*
 if((i2==46)&&(jnach>46))//&&(nverp==67)&&(ntoch==1)&&(ntochp==0)&&(nugl==-2)&&(nuglp==3))
  { fprintf(f1,"\nсбой jnach=%d,nver=%d,nverp=%d,ntoch=%d,ntochp=%d",
                     jnach, nver,nverp,ntoch,ntochp);
    fprintf(f1,"\nR1=%9.4lf,xvnew=%10.5lf,yvnew=%10.5lf",R1,xvnew,yvnew);
   }  //угол исходящий ugl0 ->из 0 в 1
    */
      // if(spr1>0.01)
    for( int j=i2;j<=jnach;j++) //почему включительно? учтено далее
   {
     uglt=ugol(xt[j-1],yt[j-1],xt[j],yt[j]);
    if(j==1&&j==i2)t=x[0];else t=x[j-1]-x[j-2];
     evpr5=evpr5+uglt*t;//t-длина элемента с концом j
   }//for j

 //s3-расст по хордам от начала трассы до nver+ nach;
 //jnach -первый за прямой
    for(int i=jnach;i<j3;i++)
     { s4=x[i-1]-s3;
     s2=s4-spr1;//принято как длина клотоиды от начала до текущей
      phikt1=uglnew+k*s2*s2/2.;//для рихтовок
      s1=spr1*uglnew+s2*(uglnew+s2*k*s2/6. );
  if(pov)s1=s1+ntoch*dpr;// сдвиг левой стороны по нормали
       //длина эвольвенты клотоиды  от  nver+ nach до i
      if(i>jnach) //иначе уже есть  evpr5  брали включительно!
      {
         uglt=ugol(xt[i-1],yt[i-1],xt[i],yt[i]);
        t=x[i-1]-x[i-2];
     evpr5=evpr5+uglt*t;//t-длина элемента с концом в точке i
      }
      //рихтовки
       cross=s1-evpr5;//рихтовка плохо!
       //если  в nver+ nach уже есть рихтовка  ntoch !0
  s1= fabs(cross);
     sum1=sum1+s1;
     if(s1>dtek)dtek=s1;
      if(cross>d3)d3=cross;
     if(cross<d4)d4=cross;
      d=cross/fabs(sin(ugb-phikt1));
      if(d>d1){d1=d; id1=2;}
     if(d<d2){d2=d; id2=2;}
    r=1./sig;//радиус R1  без знака r со знаком

    }//for i

    jnach=j3;//первый за клотоидой
  } //j3>jnach//клотоиду не пропустили
 //  если вышли изнутри клотоиды по jkon то всё плохо
    s1=uglnew+k*L1*L1/2.;// угол в конце левой клотоиды t-угол в конце окружности
    Lk=fabs(tetanew)*R1-(L1+L2)/2.0; //длина дуги окружности
    if(Lk<0.)
     {fprintf(f1,"\n Lk=%7.2lf,pov=%d,nverp=%d,ntochp=%d,nuglp=%d,R1=%8.4lf",
                    Lk,pov, nverp,ntochp,nuglp,R1);
 fprintf(f1,"\n  nver=%d,ntoch=%d,nugl=%d,tetanew=%8.5lf",
                    nver,ntoch,nugl,tetanew);
 fprintf(f1,"\nL1=%7.2lf,L2=%7.2lf",L1,L2);
      fclose(f1); exit(0);
    }
     conla( xk,yk,s1 ,sig1, Lk, &xk1,&yk1,&t);
    //sig1 со знаком

   s2=(xt[jnach]-xk)*cos(s1)+(yt[jnach]-yk)*sin(s1);
 if(s2<-0.01)
  { fprintf(f1,"\n do окружности jnach=%d,jkon=%d",jnach,jkon);
     fprintf(f1,"\nxk=%7.2lf,yk=%7.2lf,s1=%8.6lf,1/sig=%7.2lf",
                    xk,yk,s1,1./sig);
     fprintf(f1,"\n xt[jnach]=%7.2lf,yt[jnach]=%7.2lf,s2=%8.6lf",
                     xt[jnach],yt[jnach],s2);
     fclose (f1);
      exit(0);
   }// проверено точка правее начала, а конца ? там точка xk1,yk1 и угол t
   r=1./sig;//r===R1!!! минус учтён а в R1  нет!!!
  xc=xk-r*sin(s1);yc=yk+r*cos(s1);  //центр верно
      for (int j=jnach;j<=jkon;j++)
     { s2=(xt[j]-xk1)*cos(t)+(yt[j]-yk1)*sin(t);
       if(s2>0.01){jnach=j;break;}
  //при j==jnach проскочили короткую окружность!!!
       s2=rast(xt[j],yt[j],xc,yc);//от точки до центра
       cross=s2-R1;
       // при отр кривизне точка вне -рихтовка минус, внутри плюс
     // if(prp==1){d=d+cross; k1++;}  //для выч средней на окр
        if(sig<0.)cross=-cross; //рихтовка
        s1=fabs(cross);
        sum1=sum1+s1;
        if(s1>dtek)dtek=s1;
  s2= ugol( xc, yc, xt[j],yt[j]);//из центра в точку
    if(cross>d3)d3=cross;
     if(cross<d4)d4=cross;
    d=cross/fabs(cos(s2-ugb));
      if(d>d1){d1=d; id1=3;}
      if(d<d2){d2=d; id2=3;}
      //нужен угол из центра в точку съёмки  с ОХ==нормаль
  if(prp==2){fprintf(f1,"\n okr j=%d,cross=%7.4lf,sum1=%7.4lf",
            j,cross,sum1);}
    if(j==jkon){fprintf(f1,"\n summod внутри окр jkon=j=%d,jnach=%d",j,jnach);
     fprintf(f1,"\n pov=%d,nverp=%d,ntochp=%d,nuglp=%d,R1=%8.4lf",
                    pov, nverp,ntochp,nuglp,R1);
 fprintf(f1,"\n  nver=%d,ntoch=%d,nugl=%d,tetanew=%8.5lf",
                    nver,ntoch,nugl,tetanew);
 fprintf(f1,"\nL1=%7.2lf,L2=%7.2lf",L1,L2);
 fprintf(f1,"\nxlnew=%7.2lf,ylnew=%7.2lf,xvnew=%7.2lf,yvnew=%7.2lf,xpnew=%7.2lf,ypnew=%7.2lf",
   xlnew,ylnew,xvnew,yvnew,xpnew,ypnew);
   fprintf(f1,"\nxtjnach=%7.2lf,yt=%7.2lf",xt[jnach],yt[jnach]);

       fclose(f1);
       exit(0);//       return 1;
               }
     }//for j do jkon
 //if(prp==1&& k1)d=d/k1;  //могли проскочить окружность
 //работа с клотоидой jnach за её началом++++++++++
  xk=xk1; yk=yk1;//нач угол ==t конец окружности -> начало клотоиды  xk1 ,yk1
  k=-sig/L2;//sig=1/R1 со знаком,R1-без знака!
   conl(xk,yk,t, sig, k,L2,&xk1,&yk1,&phi1,&sig1); //xk1 ,yk1 конец клотоиды
     xc=(xt[jnach]-xk )*cos(t)+(yt[jnach]-yk)*sin(t);
   if(xc<-0.0001)//проекция на касат. внешней точки левее начала
   {
 fprintf(f1,"\n do clo2  pov=%d,nverp=%d,ntochp=%d,nuglp=%d,jnach=%d,xc=%8.4lf",
                   pov, nverp,ntochp,nuglp,jnach,xc);
 fprintf(f1,"\n clo2 nver=%d,ntoch=%d,nugl=%d,xk=%6.2lf,yk=%6.2lf",
                   pov, nver,ntoch,nugl,xk,yk);
  fclose (f1);
   exit(0);
   }
 //++++ клотоида jnach-первый в ней, но мог быть и за её концом.

 //xkt=xk; ykt=yk;phikt=t;sigkt=sig; //в конце окружности
 double ugab,ugac,dev;
 //dev-проектная эвольвента в начале клотоиды
     j3=0;//j3-первый за концом клотоиды jnach-первый за окружностью

   for (int j=jnach;j<=jkon;j++) //jnach-первый поперечник на клотоиде
    {u= xt[j];v=yt[j];
    s2=(u-xk1)*cos(phi1)+(v-yk1)*sin(phi1); //в конце клотоиды
    if(s2>0.001){j3=j;break;}//за клотоидой
     }//for j
    if(j3>jnach)//иначе проскочили клотоиду
    { //сначала найти рихтовку в т.А==начало клотоиды
    //все направления задаю углами с осью ОХ
    uglt=ugol(xt[jnach-1],yt[jnach-1],xt[jnach],yt[jnach]);
    //t-угол в конце окружн==т.А;uglt-угол звена ломаной
 //нужен угол между нормалями:1)из т.А к звену ломаной ugab и
 //  из т.А по нормали( какой?) к касательной в сторону звена ugac

   yc=-((xk-xt[jnach-1])*sin(uglt)-(yk-yt[jnach-1])*cos(uglt));
   //ордината  т.А  в системе по звену от jnach-1
   xc=(xk-xt[jnach-1])*cos(uglt)+(yk-yt[jnach-1])*sin(uglt);
   //xc,yc-коорд. т.А в сист. с осью Х по звену ломанойif(fabs(yc)>0.001)
  x2=xc;//по звену от его начала до точки проекции т.А (т.В)
  if(fabs(yc)>0.001)
    {
   if(yc>0.)ugab=uglt-pi/2.;else ugab=uglt+pi/2.;
   //нормаль к звену из А
   ugac=t+pi/2.;
   if(fabs(ugac-ugab)>pi/2.)ugac=t-pi/2.;
 //ugab-ведёт к звену по нормали из А,ugc должен составлять
 // с ним острый угол иначе не пересечёт эвено
   dev=yc/cos(ugac-ugab);
    //рихтовка==разность длин эвольвент проектная - прямая в т.А
  //а если cos()==??? не бывает так как jnach за началом клотоиды!!!
     s2=dev*sin(ugac-ugab);
 //fprintf(f1,"\nxc=%7.4lf,dev=%7.2lf, s2=%7.4lf,ugab=%9.6lf,ugac=%9.6lf",
   //           xc,d,s2,ugab,ugac);
    evpr=x2+s2;//только для сравнения
     s2=fabs(s2);
  // s2 сдвиг от проекции т.А на звено(т.В) до
  //  перес. с нормалью к касательной в т.А (т.С)
  //нельзя заменить разностью углов касательных

  if(yc>0.)
 {if(ugac>ugab)x2=x2+s2;else x2=x2-s2;}
 else{if(ugac>ugab)x2=x2-s2;else x2=x2+s2;}
 //Похоже что формула x2=x2+s2; где s2=d*sin(ugac-ugab);
 // без взятия модуля верна во всех случаях !!!
 //и fabs и 3 строки не нужны
   //fprintf(f1,"\nproverka evpr=%7.4lf,s2=%7.4lf,x2=%7.4lf",
      //          evpr,s2,x2);
  if(fabs(evpr-x2)>0.001)ShowMessage("Не совпали");
//+++  dev-рихтовка== разность длин эвольвент /
//проектная минус сущ в т. С
  }else dev=0.; //yc  мал т.А ==т.В лежит на звене x2=xc сделал

  for(int i=jnach;i<j3;i++)
     {
      if(i==jnach)
      {
      s2=x[jnach-1]-x[jnach-2]-x2;
      //остаток звена от т С до jnach
     //далее длины по звеньям и их же беру для клотоиды
      //t- угол касательной в начале клотоиды
//s1-длина эвольвенты клотоиды с учётом разности в т.С
     evpr5=s2*uglt;//по куску звена
       }
      else //при не первом
      {
      s4=x[i-1]-x[i-2];
       uglt=ugol(xt[i-1],yt[i-1],xt[i],yt[i]);
     evpr5=evpr5+uglt*s4;//s4-длина элемента с концом в точке i
       s2=s2+s4;//приписываю длине клотоиды от начала
       }
      //рихтовки
      phikt1=t+sig*s2+k*s2*s2/2.;//для рихтовокt-нач угол
      s1=dev+s2*(t+s2*(sig/2.+k*s2/6.) );//длина эвольвенты
       cross=s1-evpr5;//рихтовка
      s1= fabs(cross);
     sum1=sum1+s1;

     if(s1>dtek)dtek=s1;
     if(cross>d3)d3=cross;
     if(cross<d4)d4=cross;
      d=cross/fabs(sin(ugb-phikt1));
      if(d>d1){d1=d; id1=2;}
     if(d<d2){d2=d; id2=2;}

 }//for i
    jnach=j3;//первый за клотоидой
  } //j3>jnach//клотоиду не пропустили
     ugpnew=phi1;// по концу клотоиды но лучше передавать по тангенсу
     s1=rast(xk1,yk1,xpnew,ypnew);   // от конца клотоиды до правой точки
  if (s1>0.1)// && pov==2)
  {  //xk1,yk1= начало прямой == конец клотоиды
  //ошибка при нет прямой и конечная точка не учтена
   xk=(xt[jnach]- xk1)*cos(ugpnew)+(yt[jnach]- yk1)*sin(ugpnew);
   if(xk<-0.001) //до конца клотоиды а не на прямой за ней
   {
  fprintf(f1,"\n do ln2  pov=%d,nverp=%d,ntochp=%d,nuglp=%d,jnach=%d,jkon=%d",
                    pov, nverp,ntochp,nuglp,jnach,jkon);
 fprintf(f1,"\n do ln2 nver=%d,ntoch=%d,nugl=%d,xpnew=%6.2lf,ypnew=%6.2lf",
                    nver,ntoch,nugl,xpnew,ypnew);
 fprintf(f1,"\nxt=%7.2lf,yt=%7.2lf,xk1=%7.2lf,yk1=%7.2lf,s1=%7.2lf,xk=%7.5lf",
             xt[jnach],yt[jnach],xk1,yk1,s1,xk);
 fprintf(f1,"\nR1=%12.6lf,L1=%7.2lf,L2=%7.2lf",R1,L1,L2);
  fclose (f1);
   exit(0);
   }
   //new
 for (int j=jnach;j<jkon;j++)
    {
  s2=(xt[j]-xpnew)*sin(ugpnew)-(yt[j]-ypnew)*cos(ugpnew);
   //ордината с минусом ==рихтовка
  cross=s2;//рихтовка
     s1=fabs(cross);
     sum1=sum1+s1;
        if(s1>dtek)dtek=s1;

     }//for j это вся прямая кроме jkon там нельзя по проекции при повороте
     //возьму от точки  т е от центра на нормали до нового его положения
    // как с номерами при pov=0 вычитать npr, 2??


} //есть прямая или нет последнюю точку взять правильно не по проекции
  if(!pov) cross=ntoch*dpr;else
              cross=(ntochp-npr)*dpr;
       s1=fabs(cross);
       if(s1>dtek)dtek=s1;
        sum1=sum1+s1;
 //      d1=d3;d2=d4;// пределы сдвижек без проекции на биссектрису
   if( dtek<maxrix)return 0;
 else return 1;//dtek-макс сдвижка , возможны разные знаки
 }//sum_mod1 всё до правой точки

 //+++++++++++++++++
 void opt_line()
 {   //global double dL=5.0;  // временно
   double sum,R1,T1,T2,t1new,t2new;
   int k;
     //int jnach,jkon; вычисляются в opt_kriv по global pov,nver,nverp,nach
    xl=dx; yl=dy; ugl=dugl;//global
     pov=0;
     for(int i=0;i<nkr;i++)
   {
     nver= Line[i]->norm;
     k= Line[i]->norm+nach;
//в новой нумерации  это правая но при  pov=0 это nver
  fprintf(f1,"\nopt_line i=%d,nver=%d,nverold=k=%d,R=%7.2lf",
               i,nver,k,Line[i]->R);
   xp=Line[i]->xp; yp=Line[i]->yp;ugp=Line[i]->ugp;
   ntochp=Line[i]->ntoch;nuglp=Line[i]->nugl;
 fprintf(f1,"\nntoch=%d,nugl=%d,xp=%9.3lf,yp=%9.3lf,ugp=%9.6lf",
                ntochp,nuglp,xp,yp,ugp);
   s1=ugp+pi2;//beta
  fprintf(f1,"\nxt[=%8.3lf,yt=%8.3lf,beta=%9.6lf,s1=%9.6lf\n",
  xt[k],yt[k],beta[k],s1);
   }

 for(int i=0;i<nkr;i++)
 fprintf(f1,"\ni=%d,def1=%7.3lf,def2=%7.3lf",i,Line[i]->def1,Line[i]->def2);
 ///+++
 for(int i=0;i<nkr-1;i++)
     {  int j;
   s1=(Line[i]->def2-Line[i+1]->def1)/2.;//смещение точки
   if(fabs(s1)<0.1)continue;
   xp=Line[i]->xp; yp=Line[i]->yp;ugp=Line[i]->ugp;
   k= Line[i]->norm+nach;
   j=k;//исходный
   fprintf(f1,"\ni=%d,k=%d,s1=%7.3lf,xp=%7.3lf,yp=%7.3lf",
               i,k,s1,xp,yp);
   s2=0.0;
      while(fabs(s2)<fabs(s1))
     {
       if(s1>0.0)k++;else k--;
       s2=(xt[k]-xp)*cos(ugp)+(yt[k]-yp)*sin(ugp);
      }//while
 fprintf(f1,"\nafter while k=%d,s2=%7.3lf",k,s2);
   if(s1>0.0)k--;else k++;
   if(k==j)continue;//вернулись в исходный
   Line[i]->norm=k-nach;
   s2=(xt[k]-xp)*cos(ugp)+(yt[k]-yp)*sin(ugp);
    Line[i]-> xp= xp+s2*cos(ugp);
    Line[i]-> yp= yp+s2*sin(ugp);
   fprintf(f1,"\nnew i=%d,xp=%7.3lf,yp=%7.3lf",
         i,Line[i]-> xp,Line[i]-> yp);
         beta[k]=ugp+pi/2.;
    }//for

   //    ++++
for(int i=0;i<nkr;i++)
     {
 R1=fabs(Line[i]->R);L1=Line[i]->L1;L2=Line[i]->L2;
  xp=Line[i]->xp; yp=Line[i]->yp;
 tangen2(L1,L2,R1,Line[i]->teta ,1,T1,T2);
//Line[i]-> xlev=Line[i]-> xv-T1*cos(ugl);//начало левой переходной  ugl?
 //Line[i]-> ylev=Line[i]-> yv-T1*sin(ugl);
 //Line[i]-> xpr=Line[i]-> xv+T2*cos(ugp);
// Line[i]-> ypr=Line[i]-> yv+T2*sin(ugp);
 t1new=rast(xl,yl,Line[i]-> xv,Line[i]-> yv);
 t2new=rast(xp,yp,Line[i]-> xv,Line[i]-> yv);
 Line[i]->def1=T1-t1new;Line[i]->def2=T2-t2new+lprmin;
if(Line[i]->def1>0.001 || Line[i]->def2>0.001)
 {
 fprintf(f1,"\n1Line i=%d def1=%7.3lf,def2=%7.3lf",
 i,Line[i]-> def1,Line[i]->def2);
 ShowMessage("opt_line. короткий тангенс");
  fclose(f1);exit(0);
 }
xl=xp; yl=yp;
     }  //for i (0 --nkr-1 вкл)
 for(int i=0;i<nkr;i++)
 fprintf(f1,"\npo i=%d,def1=%7.3lf,def2=%7.3lf",i,Line[i]->def1,Line[i]->def2);
 fprintf(f1,"\n");

 xl=dx; yl=dy;
//+++++
      for(int i=0;i<nkr;i++)
     {  //печать до
  //   if(Line[i]->teta<0.)Line[i]->R=-Line[i]->R; //всe R стали >0
fprintf(f1,"\noptLine do i=%d,nver=%d,xv=%7.2lf,R=%7.2lf,teta=%7.5lf,L1=%7.2lf,ntoch=%d,sum=%7.3lf",
   i, Line[i]->norm+nach,Line[i]->xv,Line[i]->R,Line[i]->teta,Line[i]->L1,Line[i]->ntoch,Line[i]->sum);
     }//for i
      for(int i=0;i<nkr;i++)
     {
   ugp=Line[i]->ugp; xp=Line[i]->xp; yp=Line[i]->yp;
  R1=fabs(Line[i]->R);L1=Line[i]->L1;L2=Line[i]->L2;
  ntochp=Line[i]->ntoch;nuglp=Line[i]->nugl;
   //нужны для sum_mod только для печати при сбоях
   //Дать новые переходные для новой кривой
  //Line[i]->R-  идёт по ссылке и возвращается Ropt,L1,L2 -global
 // ntoch,nugl  здесь не работают, нет записи R1,L1,L2,они нужны дальше
 //fprintf(f1,"\n optpo i=%d,nver=%d,R1=%7.3lf,L1=%6.2lf,L2=%6.2lf,sum=%6.2lf\n",
   //           i,nver,R1,L1,L2,sum);
   // для opt_kriv jkon jnach=1;jkon=nver+nach;
  fprintf(f1,"\n do Kriv i=%d, R1=%12.6lf,teta=%9.6lf,xv=%9.6lf,yv=%9.6lf",
      i, R1,Line[i]->teta,Line[i]-> xv,Line[i]->yv);
fprintf(f1,"\nugl=%9.6lf,ugp=%9.6lf,xl=%9.6lf,yl=%9.6lf,xp=%9.6lf,yp=%9.6lf",
        ugl,ugp,xl,yl,xp,yp);
    sum=opt_kriv1( R1,Line[i]->teta,Line[i]-> xv,Line[i]->yv,
        ugl,ugp,xl,yl,xp,yp,k); //k==zap лишний
   fprintf(f1,"\ni=%d,sum=%9.6lf,R1=%12.6lf,L1=%9.5lf,L2=%9.5lf,Lk=%9.5lf",
      i, sum, R1,L1,L2,Lk); // R1 здесь по модулю

  if(sum>0){//есть улучшение
  if(Line[i]->teta<0.)R1=-R1; Line[i]->R=R1;
     Line[i]->L1=L1;Line[i]->L2=L2;Line[i]->Lk=Lk;//global
   //это не всё нужна новая запись вместо этого
 s3=   Line[i]->sum;
   if(!i) Line[i]->sum=sum;else
   Line[i]->sum=Line[i-1]->sum +sum;
 // вместо суммарных в Line[i]->sum  попадают локальные
  fprintf(f1,"\n лучше i=%d,до s3=%8.3lf, po sum1=%8.3lf", i, s3,Line[i]->sum);
   }else  //т.е. не лучше sum<0

 { if(!i) Line[i]->sum=-sum;else Line[i]->sum=Line[i-1]->sum -sum;
   fprintf (f1,"\n opt_line нет улучшений i=%d,R1=%9.5lf,sum=%9.4lf",i,R1,sum);
  // R1= floor(Line[i]->R); if(R1<0.)R1=R1+1.;
  R1= floor01(Line[i]->R);  //do 0.1
  Line[i]->R=R1;
  L1=Line[i]->L1;L2=Line[i]->L2;
  if(R1<0.)R1=-R1;//для tangen2
    Line[i]->Lk=R1*fabs(Line[i]->teta)-(L1+L2)/2.;

  int jnach,jkon;
  if(!pov ){jnach=1;jkon=nver+nach;}//pov для нулевой кривой сделал ноль
  else {jnach=nver+nach+1;jkon=nverp+nach;}
  tangen2(L1,L2,R1,Line[i]->teta,1,T1,T2);
 k=sum_mod(T1,Line[i]-> xv,Line[i]->yv,xl,yl,xp,yp,ugl,R1,Line[i]->teta,sum,jnach,jkon,s1);
// opt_kriv1( R1,Line[i]->teta,Line[i]-> xv,Line[i]->yv,
  //      ugl,ugp,xl,yl,xp,yp,k);

 if(!i) Line[i]->sum=sum;else Line[i]->sum=Line[i-1]->sum+sum;
 fprintf (f1,"\nfloor i=%d,k=%d,R1=%9.5lf,sum=%9.5lf,sumful=%9.5lf",
              i,k,R1,sum,Line[i]->sum);
 } //не лучше
 if(R1<0.)R1=-R1;
 tangen2(L1,L2,R1,Line[i]->teta ,1,T1,T2);
 Line[i]-> xlev=Line[i]-> xv-T1*cos(ugl);//начало левой переходной  ugl?
 Line[i]-> ylev=Line[i]-> yv-T1*sin(ugl);
 Line[i]-> xpr=Line[i]-> xv+T2*cos(ugp);
 Line[i]-> ypr=Line[i]-> yv+T2*sin(ugp);
 fprintf(f1,"\ni=%d,xlev=%7.2lf,ylev=%7.2lf,xpr=%7.2lf,ypr=%7.2lf\n",
              i, Line[i]->xlev,Line[i]->ylev,Line[i]-> xpr,Line[i]->ypr);
 t1new=rast(xl,yl,Line[i]-> xv,Line[i]-> yv);
 t2new=rast(xp,yp,Line[i]-> xv,Line[i]-> yv);
 Line[i]->def1=T1-t1new;Line[i]->def2=T2-t2new+lprmin;
 if(Line[i]->def1>0. || Line[i]->def2>0)
 {
 fprintf(f1,"\n2Line i=%d def1=%7.3lf,def2=%7.3lf",
 i,Line[i]-> def1,Line[i]->def2);
  fclose(f1);exit(0);
 }

    ugl=ugp; xl=xp;yl=yp; ntoch=ntochp-npr;nugl=nuglp-nug;
   pov=2; //ntochp=Line[i]->ntoch;nuglp=Line[i]->nugl;
   //  pov=2;для opt_kriv jkon; a  ntoch  nugl Зачем они здесь??
  if(i+1<nkr){  nver= Line[i]->norm; nverp=Line[i+1]->norm;}
 // pov=2 nver и nverp нужны оба для jnach,jkon
  }//for i

   fprintf(f1,"\n\n OPTpo nach nverp=%d,ntochp=%d,nuglp=%d,R=%7.2lf",
  Line[1]->norm+nach,Line[1]->ntoch,Line[1]->nugl,Line[1]->R);
fprintf(f1,"\nnver=%d,ntoch=%d,nugl=%d\n",
Line[0]->norm+nach,Line[0]->ntoch,Line[0]->nugl);

      for(int i=0;i<nkr;i++)
     {
     if(Line[i]->teta<0.)Line[i]->R=-fabs(Line[i]->R);
  fprintf(f1,"\noptLpo i=%d, nver=%d, Lk=%9.4lf, R=%9.4lf,L1=%7.2lf,L2=%7.2lf,sum=%8.4lf",
   i,Line[i]->norm+nach,Line[i]->Lk, Line[i]->R,Line[i]->L1,Line[i]->L2,Line[i]->sum);
  fprintf(f1,"\noptLpo xlev=%8.3lf,ylev=%8.3lf,xpr=%8.3lf,ypr=%8.3lf",
       Line[i]->xlev,Line[i]->ylev,Line[i]->xpr,Line[i]->ypr);
     }

 }//opt_line

 double opt_kriv1(double& R1,double tetanew,double xvnew,double yvnew,
        double uglnew,double ugpnew,double xlnew,double ylnew,double xpnew,
        double ypnew,int zap) //zap-лишний
    {//возвращает sum1=sumopt L1,L2 получены как  глобальные
     double T,T1,T2,t1new,t2new,R1opt,L1opt,L2opt,cost,tant;
 int j,jnach,jkon; double a,b,c,d,d1,sum1,s1,s2,s3,s4,sumopt;
       double t,Bcmin,r1,r2,r3;//пределы по R1 при поиске оптимума
      double sL1m,sL2m;
   int przap;//bl1,bl2-было  текущее число неуд. измененийL1,L2
   //max можно kdL//по радиусу  макс kdR  неи их
    przap=0;//если не найдём лучше 0 останется и замены не будет
 // L1,L2 global;  приходим с L1=L2 по записанной линии
  //R1 менять с учётом малости угла, без накладок!!!
  //при малом угле получал большой радиус при lkmin>100
  cost=cos(tetanew/2.);//углы меньше 180
 // sint=sin(fabs(tetanew/2.));
 tant=tan(fabs(tetanew/2.));
  t1new=rast(xlnew,ylnew,xvnew,yvnew);
  t2new=rast(xpnew,ypnew,xvnew,yvnew);
   tangen2(L1,L2,R1,tetanew,1,T1,T2);
  s1=t1new-T1;  s2=t2new-lprmin-T2;// запасы по тангенсам для нач приближ
 s3=s1;if(s3>s2)s3=s2;//min запас при <0 ошибка
 if(s3<0.)
 {fprintf(f1,"\noptkr нет тангенса T1=%5.2lf,t1=%5.2lf,T2=%5.2lf,t2=%5.2lf",
     T1,t1new,T2,t2new);
     fclose(f1);   exit(0);
 }
 if(!pov ){jnach=1;jkon=nver+nach;}//pov для нулевой кривой сделал ноль
  else {jnach=nver+nach+1;jkon=nverp+nach;}
    //границы по нормалям в полной нумерации(до n)
   // можно бы передать старую по опт линии
  if(sum_mod(T1,xvnew,yvnew,xlnew,ylnew,xpnew,ypnew,uglnew,R1,tetanew,sum1,jnach,jkon,d,1))
 {fprintf(f1,"\noptkriv1 sum_mod sum1=%10.2lf,R1=%7.2lf,L1=%7.2lf,d=%7.2lf",sum1,R1,L1,d);
     fclose(f1);
     exit(0);
     //возврат нуля при хорошо иначе ошибка
 } //получил среднюю сдвижку на круговой кривой по нач прибл d со знаком
 //пределы по радиусу через пределы по биссектрисе как в postr
 // fprintf(f1,"\nkriv1 d=%7.2lf,R1=%7.2lf,L1=%7.2lf",d,R1,L1);
  t=(R1+L1*L1/(24.*R1))/cost -R1;// биссектриса по нач прибл L1=L2 из postr
  Rmin1=(lmin+lkmin)/fabs(tetanew);//иначе накладка
  if( Rmin1<Rmin)Rmin1=Rmin;// минимальный радиус для угла и переходной
  Bcmin=(Rmin1+lmin*lmin/(24.*Rmin1))/cost-Rmin1;
 //d пришёл из summod это средня рихтовка на круговой
 //для пределов по биссектрисе

 //s1=t+maxrix;//-d; //максимальная биссектриса
  //s2=t-maxrix;//-d); нужен диапаон по биссектрисе начальная равна t при R1
 //использую среднюю d.
 double dob;//для диапазона от биссектрисы ( то есть 0.6 м)
 dob=0.4;
  // не мало ли? t-биссектриса по нач прибл R1 перебор по радиусам
 if( tetanew>0.)
  {  if(d>=0.)
     { s1=t+dob; //max
        s2=t-d-dob;   //min
     } else
      {
      s1= t-d+dob;//max
      s2=t-dob;//min
      }
   }else
  { if(d>=0.)
    {
      s1= t+d+dob;//max
      s2=t-dob;
    }else
      {
    s1=t+dob;
    s2=t+d-dob;
      }
  }//tetanew<0
  if(s2<Bcmin){r1=Rmin1;
  fprintf(f1,"\n bismin teta=%9.6lf,s2=%9.6lf,Bcmin=%9.6lf,r1=%9.6lf",
         tetanew,s2,Bcmin,r1 );
  //минимальная биссектриса
  s2=Bcmin;}
   else
   {//мин биссектриса s2>Bcmin нужен мин радиус в r1
  a=24.*(1.-cost);
  b=-24.*s2*cost;
  double lt;
  lt=lmax;
   for(int j1=0;j1<=3;j1++)
 {
  c=lt*lt;
  // был lmin,но  возможно уменьшение радиуса при l>lmin
  //нельзя при увел L  со старой s2!!!
   j=kvadrat(a,b,c,r2,r1);
  //r1-при плюс корень радиус по биссектр и lmax перех
 //  fprintf(f1,"\nj1=%d, lt=%7.2lf,r2=%9.6lf,r1=%9.6lf",
   // j1,lt,r2,r1);
         if(!j)
  {
    fprintf(f1,"\nBc1 нет корней");
   fprintf(f1,"\nlt=%7.2lf,s2=t=%7.3lf,a=%9.6lf,b=%12.6lf,c=%9.2lf",
              lt,s2,a,b,c);
   fprintf(f1,"\nteta=%9.6lf,cost=%7.4lf,tetamin=%9.6lf",
            tetanew,cost,tetamin);
         //   fclose(f1);exit(0); перейти на меньшую клотоиду
    lt=lt-lkmin; if(lt<lmin){lt=lmin;j1=2;}
    continue;
  } //!j
  else {
        s3=r1*fabs(tetanew)-lt;//окружность
  // fprintf(f1,"\nold lk=s3=%7.3lf,r2=%7.3lf,r1=%7.3lf,lt=%7.3lf",
    //       s3,r2,r1,lt);
    s4=(r1+lt*lt/(24.*r1))/cost-r1;
  //fprintf(f1,"\nbis teta=%9.6lf,s4=%7.3lf, s2=%7.3lf,Bcmin=%7.3lf\n",
    //         tetanew,s4,s2,Bcmin);
        if(s3>=lkmin-0.01) break; else
        {
        lt=lt-(lkmin-s3)/2.;if(lt<lmin){lt=lmin;j1=2;}
        //fprintf(f1,"\ns3=%7.3lf,lt=%9.6lf,lkmin=%9.6lf,lmin=%9.6lf",
          //            s3,lt,lkmin,lmin);
          continue;//for j1
        }
     }//else то есть корни есть
  }////for j1
 }//else не миним бисектриса s2 теперь s1
    a=24.*(1.-cost);
    b=-24.*s1*cost;//при макс биссектрисе max R  при lmin
      c=lmin*lmin;
      j=kvadrat(a,b,c,d,r2);//d-при минус корень не нужен
  if(!j){fprintf(f1,"\nBc22 нет корней t=%7.2lf,d=%7.2lf,s1=%7.2lf,cost=%8.5lf",
               t,d,s1,cost);
               fprintf(f1,"\na=%7.2lf,b=%7.2lf,c=%7.2lf",a,b,c);
     d=cost/sin(tetanew/4.); a=sqrt(3)*d*s2;
     fprintf(f1,"\nd=%9.6lf,a=L=%9.5ld,s2=%7.3lf",d,a,s2);
        fclose(f1); exit (0);
         }
   //  r1-мин. радиус  r2-max на все переходные ==диапазон поиска
   //if(r1<Rmin1)r1=Rmin1; см ниже
   if(r2>Rmax) r2=Rmax;
 //fprintf(f1,"\nкоридор r1=%9.5lf,R1=%9.5lf,r2=%9.5lf,sum1=%7.3lf",
 // r1,R1,r2,sum1);
  double R1isx,r1isx,r2isx,dR1,dR2;
  R1isx=R1opt=R1;r1isx=r1;r2isx=r2;L1opt=L2opt=L1;//по нач прибл
  sL2m=sL1m=sumopt=sum1;//покa а лучшее

      for (double L22=lmin;L22<lmax+0.01;L22=L22+dL)
  {  r1=r1isx;  r2=r2isx; L2=L22;
      for (double L11=lmin;L11<lmax+0.01;L11=L11+dL)
     {R1=R1isx;  L1=L11;
     Rmin1=((L1+L2)/2.+lkmin)/fabs(tetanew);//иначе накладка
      if( Rmin1<Rmin)Rmin1=Rmin;
      if(r1<Rmin1)r1=Rmin1;if(R1<Rmin1)R1=Rmin1;
      if(Rmin1>r2){if (L1<lmin+0.001) goto M1; else break;}
   //r1-r2 это коридор по радиусу. при всех клотоидах!
    //но не во всех парах r,l
   //ошибка нет восстановления коридора они корректируются по тангенсам
  //при входе тангенс не нарушен с входными L1==L2, R1
   // tangen2(L1,L2,R1,tetanew,1,T1,T2);
 //  s1=t1new-T1;  s2=t2new-lprmin-T2;// запасы по тангенсам для нач приближ
 //s3=s1;if(s3<s2)s3=s2;//min не нужен
  a=tant; b=(L1/2.-t1new);
 //поиск при этих клот. макс радиуса по левому тангенсу
  c=(L2*L2-L1*L1*cos(tetanew))/24./sin(fabs(tetanew));
  s3= b*b-4.*a*c;
if(s3<0.){if(L1<lmin+0.001)goto M1;else break;}
//M1 за двойной цикл, break на увеличение L2 и заново L1=lmin
//нет радиуса под макс тангенс при этих переходных s2=(-b +sqrt(b*b-4.*a*c))/2./a;
 s2=(-b+sqrt(s3))/(2*a);
  if(s2<Rmin1){if(L1<lmin+0.001)goto M1;else break;}
 if(r2>s2)r2 =s2; //уменьшил макс чтобы влезть в левый тангенс
   b=(L2/2.-(t2new-lprmin));//как учесть от ВУ до xp,ypслева
   c=(L1*L1-L2*L2*cos(tetanew))/24./sin(fabs(tetanew));
  s3= b*b-4.*a*c;
if(s3<0){if(L1<lmin+0.001)goto M1;else break;}
//нет радиуса под макс тангенс при этих переходных
//нужно использовать резерв  увеличенич тангенсов //за счёт кривой слева!!1
  s2=(-b+sqrt(s3))/(2*a);
  if(s2<Rmin1) {if(L1<lmin+0.001)goto M1;else break;}
  if(r2>s2)r2 =s2;if(R1>r2)R1=r2;
  // скорректированы пределы s1--s2 по радиусу без нарушения тангенся
     if(R1-r1>0.1)//не работать при равных
      {
    //  if(R1-r1<100.)dR1=0.1; else dR1=1.;
   if(R1-r1<100.)dR1=0.4; else if(R1-r1<1000.)dR1=1.;else dR1=10.;
        //dR1=0.1;
        r3=R1-dR1;
   while(r3>=r1)
   {  tangen2(L1,L2,r3,tetanew,1,T1,T2);// нужен для sum_mod
    j=sum_mod(T1,xvnew,yvnew,xl,yl,xp,yp,uglnew,r3,tetanew,sum,jnach,jkon,d);
 if(!j && sum<sum1-0.0001){sum1=sum;R1opt=r3;}
       r3=r3-dR1;
   }//while
     } //R1-r1>dR1 min radii
        if(r2-R1>=0.1)//увеличение R1 ->r2   Перебор от начального R!
     { if(r2-R1<100.)dR2=0.4; else if(r2-R1<1000.)dR2=1.;else dR2=10.;
     //dR2=0.1;
   r3=R1+dR2;//меняя радиус надо пересчитывать T для sum_mod
      while(r3<=r2)
   {
   //Для каждой L нашли макс r2 без превышения тангенса и более
    //его не вычисляем.//Tmax - от вершины до точки нормали
 tangen2(L1,L2,r3,tetanew,1,T1,T2);
 j=sum_mod(T1,xvnew,yvnew,xl,yl,xp,yp,uglnew,r3,tetanew,sum,jnach,jkon,d);
   if(!j && sum<sum1-0.0001){sum1=sum;R1opt=r3;}
       r3=r3+dR2;
   } //while
     }//if r2-R1 >0.1

//прошлись по радиусам при этой паре переходных итог sum1 исходная sLm1=sumopt
  if (sum1<sumopt-0.001)
  {przap=1;sumopt=sum1;L1opt=L1;L2opt=L2;}
 //на след L1 сравнение с итогом лучшей из пройденных
      if (sum1<sL1m)sL1m=sum1;
       } //for L1
     if(sL1m<sL2m-0.001){sL2m=sL1m;}

   }//for L2   Не были вычислены  L1opt и др из-за больших рихтовок
M1: if(!przap) return (-sum1);// нечего записывать вернул старую
 L1=L1opt;L2=L2opt;R1=R1opt;Lk=R1*fabs(tetanew)-(L1+L2)/2.;
     sum=sumopt;//для записи sum -global
 //R1=floor(R1opt);
 R1=floor01(R1opt);
 Lk=R1*fabs(tetanew)-(L1+L2)/2.;
 tangen2(L1,L2,R1,tetanew,1,T1,T2);
 j=sum_mod(T1,xvnew,yvnew,xl,yl,xp,yp,uglnew,R1,tetanew,sum,jnach,jkon,d);
fprintf(f1,"\nj=%d,R1opt=%9.4lf,R1floor=%9.4lf,sumopt=%9.4lf,sum=%9.4lf",
              j, R1opt,R1,sumopt,sum);
  return sum;
     }//opt_kriv1

void new_otpr(double x0,double y0,double alf,double a,double b,
          double &x,double &y)
   {
   x=(a-x0)*cos(alf)+(b-y0)*sin(alf);
   y=-(a-x0)*sin(alf)+(b-y0)*cos(alf);
   return;
 /*
   x0,y0-точка в начале прямой,alf её угол с осью Х,
   a,b-точка вне прямой;x,y-её координаты в системе с центром
   в x0,y0, повёрнутой на al)f; y-расст от прямой (сдвижка)
   по нормали( влево +), x- расст. проекции точки от начала
   */
   }//new_otpr

    int perv(void)
 { //global xl,yl,xp,yp,ugl,ugp,pr;
// double t1isx,t2isx,def1isx,def2isx,xvisx,yvisx,xpisx,ypisx; надо глобал
 //для центральной точки
   int prvu,jj;//в pech j1=0 надо строить все,если 1-сразу последний
  // fprintf (f1,"\n nach=%d,kon=%d,Nnorm=%d,x[nach]=%7.2lf",
    //       nach,kon,Nnorm,x[nach]);
 //В массиве нормалей нумерация от 0 до Nnorm-1; номеру k в этом
 //массиве соответствует нормаль с номером nach+k в исходной нумерации
 //и nach+k-1 в нумерации расст от начала ( абсцисс) по ломаной
  //xl=dx; yl=dy; ugl=dugl;//В первом эл=те всегда первая прямая фиксирована
  //нумерация нормалей с нуля nach
  pov=0; jj=0;
 for( nver=inach;nver<=ikon;nver++)//в новой нумерации нормалей
  //по правым нормалям от точки nach до Lmax,но не далее Lmin от конца
 //,а левая -это исходная нулевая нормаль
   {     //угол исходящий ugl0 ->из 0 в 1
    //основной цикл .Сначала центральная точка,
     if ( masnorm[nver].pr && !jj)
     {nver=masnorm[nver].pr;jj++;ikon=nver+3;}
  ntoch=nugl=0;
  xp= xt[nach+nver]; yp= yt[nach+nver]; ugp=beta[nach+nver]-pi2;
//это центральная точка и угол в ней как при съёмке будем её проверять
 /*if(nver==41)
{ fprintf (f1,"\nnver41 xl=%7.2lf,yl=%7.2lf,xp=%7.2lf,,yp=%7.2lf",xl,yl,xp,yp);
fprintf(f1,"\nugl=%8.6lf,ugp=%8.6lf,beta=%8.6lf",ugl,ugp,beta[nach+nver]);
 } */
  //  def1=def2=0.0;  //отрицательный значит запас по тангенсу
  //0=вращаем вторую сторону с ug2 (teta поворот от 1 к 2 стороне 2-дальше от начала
  //Не вызывать при fabs(ugp-ugl)<tetamin

   pr=VUxy(xl,yl,xp,yp,ugl,ugp,xv,yv);//1-при ошибке 2-не влез тангeнс,0-хорошо
  //fprintf (f1,"\n\npr=%d,xv=%7.2lf,yv=%7.2lf,teta=%8.6lf",pr,xv,yv,teta);
 //if((nverp==47) && (ntochp==npr) && (nuglp==nug)&&(nver==18))
 //fprintf (f1,"\n\nVU xv=%7.2lf,yv=%7.2lf,teta=%8.6lf,ugl=%8.6lf,ugp=%8.6lf",
   //                   xv,yv,teta,ugl,ugp);
 if(pr==1)  continue;
  //ВУ за= пределами  и поворотом не пытаться исправить
 if(def1>0 && def2>0 && fabs(teta)<pi/2.)continue;
 //малые смещения и повороты левой
 //не могут ликвидировать дефицит
   if(pr!=2)
   {
   prvu=proverka(xv,yv,0.0,1,nach+nver);
   //fprintf (f1,"\n proverka=%d,Bcmin=%7.2lf",prvu,Bcmin);

 //хорошо при 0,1-большие отклонения от сторон угла за его пределами
 if(!prvu)
  {
  // fprintf(f1,"\nНашли допуск в центральной точке");
//fprintf(f1,"\n\nнули Работать nver=%d,x[nach+nver]=%8.3lf\n\n",nver,x[nach+nver]);
     postr(xl,yl,xp,yp,xv,yv,Bc,1,nach+nver,0.0);
  }
  }//pr!=2 ===pr==0
   // мал тангенс или всё хорошо pr==5 уже исключили
    t1isx=t1;t2isx=t2; def1isx=def1;def2isx=def2; //до поворотов
     xvisx=xv; yvisx=yv; //xv,yv перевычислю но не испорчу центр точку
     xpisx=xp;ypisx=yp; // правая точка ходит по нормали
  // fprintf(f1,"\nцентр pr=%d,def1=%7.2lf,def2=%7.2lf",pr,def1,def2);
    def1prev=def1;def2prev=def2;//исходные до поворота
     pouglam(1,nach+nver); //портит  Tmax in corrtg
 double dt1,dt2;
    for(int k=1;k<=npr;k++)
   {
    //teta старый не меняю для одной нормали ВУ надо менять,
// но по углам она пересчитывается. t1,t2-от ВУ до точки
     ntoch=k; nugl=0;//центр в новой точке
 //dt1,dt2- приращ. тангенсов,а не сдвиг по сторонам угла!смещаю t2-правый
    dt1= -k*dpr/sin(teta);
    dt2=k*dpr*cos(teta)/sin(teta);//нельзя tan(teta),нужны оба  pi/2
  //dt1,dt2 проверен для углов поворота по модулю меньше pi

   if((-dt1>t1isx)||(-dt2>t2isx-lprmin)) goto M1;// вершина ушла за левую точку
    t1=t1isx+dt1;
    t2= t2isx+dt2;
    xp=xpisx-k*dpr*sin (ugp);//+cos(ugp+pi2)
    yp=ypisx+k*dpr*cos (ugp);// sin(ugp+pi2)
    Tmax=t1;
    if(Tmax>t2-lprmin)Tmax=t2-lprmin;
    def1=def1isx-dt1;// добавка с минусом dpr/sin(teta)i
    def2=def2isx-dt2;//добавка dpr/tan(teta). новый def=def-добавк
 /*    if(def1>0. || def2>0)
 {
 fprintf(f1,"\nperv1 def1=%7.3lf,def2=%7.3lf", def1,def2);
 fprintf(f1,"\nd2isx=%7.3lf,dt2=%7.3lf,k=%d,dpr=%5.2lf,teta=%9.6lf",
               def2isx,dt2,k,dpr,teta);
 // fclose(f1);exit(0);
 }  */
     xv=xvisx+dt1*cos(ugl);
     yv=yvisx+dt1*sin(ugl);
     //для нового центра новая ВУ по углам она используется
// сделал новую центр. точку и ВУ движение по левому тангенсу
//fprintf(f1,"\nnew point vverx pr=%d,k=%d,t1=%7.2lf,t2=%7.2lf",pr,k,t1,t2);
//fprintf(f1,"\nperv dt1=%7.3lf,dt2=%7.3lf,def1=%7.3lf,def2=%7.3lf",  dt1,dt2,def1,def2);
//нужно строже чем выход ВУ. Условия t1isx + dt1 >Tmin;t2isx+dt2>Tmin+lprmin
// иначе goto M1
//мин для тета и Rmin от точки до вершины
   //++++++===
     prvu=proverka(xv,yv,0.0,1,nach+nver);
 // fprintf(f1,"\n vverx prvu=%d,i=%d,def1=%7.2lf,def2=%7.2lf",
  //prvu,nver,def1,def2);
   if(prvu){ goto M1;} //куда?
   //хорошо при 0,1-внутри кривой 2-велика  биссектриса Bcmin
   if((def1<=0)&&(def2<=0)&&!prvu)
 {// fprintf(f1,"\nРасчёт  в новой центр. точке k=%d,Bcmin=%7.2lf",k,Bcmin);
           postr(xl,yl,xp,yp,xv,yv,Bc,1,nach+nver,0.0);
  }
   def1prev=def1;def2prev=def2;//
     pouglam(1,nach+nver); //проверил после шага вверх
 //fprintf(f1,"\nцентр вверх проверен pr=%d,k=%d,dt1=%7.3lf,dt2=%7.3lf",
   //  pr,k,dt1,dt2);

  M1:  ntoch=-k;//вниз с минусом
       nugl=0;
       dt1= -dt1;//k*dpr/sin(teta);
       if(-dt1>t1isx)continue;
       dt2=-dt2;//k*dpr/tan(teta);
       if(-dt2>t2isx) continue;
       t1=t1isx+dt1;
       t2= t2isx+dt2;
       xp=xpisx-k*dpr*cos(ugp+pi2);yp=ypisx-k*dpr*sin(ugp+pi2);
        Tmax=t1;
    if(Tmax>t2-lprmin)Tmax=t2-lprmin;
       def1=def1isx-dt1;// добавка с минусом dpr/sin(teta)i
       //был +dt1
      def2=def2isx-dt2;//добавка dpr/tan(teta). новый def=def-добавка
      //сделал новую центральную точку
 /*  if(def1>0. || def2>0)
 {
 fprintf(f1,"\nperv2 def1=%7.3lf,def2=%7.3lf", def1,def2);
 // fclose(f1);exit(0);
 } */
     xv=xvisx+dt1*cos(ugl);
     yv=yvisx+dt1*sin(ugl);
     //здесь верно, движение по первому тангенсу
   //fprintf(f1,"\nnew point vniz pr=%d, k=%d,t1=%7.2lf,t2=%7.2lf",
       //                 pr,k,t1,t2);
   //fprintf(f1,"\ndt1=%7.3lf,dt2=%7.3lf,def1=%7.3lf,def2=%7.3lf",
     //             dt1,dt2,def1,def2);
    prvu=proverka(xv,yv,0.0,1,nach+nver);
    //1-внутри кривой 2-велика  биссектриса Bcmin
    //fprintf(f1,"\nperv vniz prvu=%d,nver=%d,def1=%7.2lf,def2=%7.2lf",
  //prvu,nver,def1,def2);
   if(prvu) continue;
   //на след точку той же нормали
       if((def1<=0)&&(def2<=0))
   {//fprintf(f1,"\nРасчёт  в новой центральной точке k=%d",k);

       postr(xl,yl,xp,yp,xv,yv,Bc,1,nach+nver,0.0);
        }
     def1prev=def1;def2prev=def2;
     pouglam(1,nach+nver); //проверил после шага вниз
    }//for k
    // мал тангенс  или всё хорошо
    } //for nver
   // fclose(f1); exit(0);
    return 0;
   } //perv

    void posl(int j2,int j3)
  { // j2,  j3-концы интервала для левой а  правая для vse pov==2
 // это nverp  в цикле от lev до prav при вызове posl
 //для просто posl  правая j3==n-nach ; для vse j2==j3 =nver1
 //xv,yv-глобальные
    int i,j,prvu,nallev;
  // double t1isx,t2isx,xvisx,yvisx,xlisx,ylisx;//,xpisx,ypisx;
     //старая точка нужна : правая для perv,левая для posl
      for(ntochp=0; ntochp<=2.*npr;ntochp++)
       {
     //номера для записи  точки на левой  от -npr ->npr
         for(nuglp=0; nuglp<=2.*nug;nuglp++)
          {
          //номера для записи углы на левой  от -nugl ->nugl
     ukinf=NULL;//работает при pov==1(последняя)
     ukinfp=NULL;//работает при pov==2(vse)
 //для каждой новой точки или угла создаётся заново при записи если

 xp= xt[nverp+nach];yp= yt[nverp+nach];ugp=beta[nverp+nach]-pi2;
 //угол по beta
 if(pov==2)//vse
 { if(ntochp!=npr)//не центр на правой
   {  xp=xp+dpr*(ntochp-npr)*cos(beta[nverp+nach]);//начал снизу -npr
      yp=yp+dpr*(ntochp-npr)*sin(beta[nverp+nach]);
   }
   if(nuglp!=nug)ugp=ugp+dug*(nuglp-nug); //начал снизу -nug
 }//if pov==2 случай 1 в запасе
   // Надо менять правую точку и угол при движении по правой нормали!!!!
   nallev=0;// наличие хотя бы одной точке на левых нормалях

   for (nver=j2;nver<=j3;nver++)
   // nver- левая нормаль одна j2==j3 for vse !!!
  {if(masnorm[nver].point==NULL)continue;
//   if(nver==32 && nverp==51)
  //    i=nver+nach;//номер левой текущей  нормали в исх нумерации
     i=nver+nach;
 ugl=beta[i]-pi2; xl=xt[i]; yl=yt[i]; //угол и точка слева центральная ?? yes
 if(fabs(ugp-ugl)<tetamin) continue;
 //на этой левой нет точек,если на всех левых нет точек, то обрыв
 //Если нет в центральной точке, то может быть в других
  //и тоже с углом  сразу бросать эту нормаль
      nallev=1;//нашли точку на левой для связи с правой
      ntoch=0; nugl=0;  //центр на левой там такая нумерация
   //ntoch -номер точки на левой нормали по ним здесь перебор
//меняем от -2 до +2 но в zaptek это учтено и пишет в + npr +nug

 //if(nverp==27 &&ntochp==0 &&nuglp==0 && nver==17 && ntochp==0 && nugl==0)
      //if(ipp)fprintf(f1,"\nugl=%8.5lf,xl=%7.2lf,yl=%7.2lf,ugp=%8.5lf,xp=%7.2lf,yp=%7.2lf",
      //ugl,xl,yl,ugp,xp,yp);
 pr=VUxy(xl,yl,xp,yp,ugl,ugp,xv,yv);//1-при ошибке 2-не влез тангeнс,0-хорошо
    //даёт t1,t2( расст от вершины) и Tmax
// if(ipp)fprintf(f1,"\nVU pr=%d,xv=%9.2lf,yv=%9.2lf,t1=%7.2lf,t2=%7.2lf,def2=%7.2lf",
 //pr,xv,yv,t1,t2,def2);

   if(pr==1)
   continue;// на след. нормаль  1-|| или за пределами 2-не влез тангенс
  //если 2-- не влез тангенс, то зачем проверять?
  //нужно выяснить поможет ли для центра по углам и какое направление
 if(def1>0 && def2>0 && fabs(teta)<pi/2.) continue;
  //малые смещения и повороты левой
 //не могут ликвидировать двойной дефицит
   if(pr!=2) //pr==0
   {
   prvu=proverka(xv,yv,0.0,i,nverp+nach);
  // fprintf (f1,"\n prvu=%d,Bcmin=%7.2lf",prvu,Bcmin);
   //if(prvu){//fprintf(f1,"\nprvu=%d,nver=%d",prvu,nver); continue;}
   //хорошо при 0,1-внутри кривой 2-велика  биссектриса Bcmin
   //если ВУ внутри то надо бы не на след нормаль, а на сдвиг по своей!
// prvu не 0)(внутри кривой или точки ближе к ВУ чем мин биссек.)ушли
  if(prvu==0 && masnorm[nver].point[ntoch+npr].ukp!=NULL &&
      masnorm[nver].point[ntoch+npr].ukp[nug+nugl].sv!=NULL)
  postr(xl,yl,xp,yp,xv,yv,Bc,i,nverp+nach,0.0);//в центральной левой

 //При pr==2 ( не влез тангенс) надо менять точку и углы
   //на точках и углах идёт пересчёт от центра поэтому контроль на связь
  //только при попытке записать ( postr)
 // мал тангенс или всё хорошо pr==1 уже исключили, но по углам надо!!
    }//if pr!=2   то есть 0 а 1 уже ушла

      t1isx=t1;t2isx=t2; def1isx=def1;def2isx=def2; //до поворотов
      xvisx=xv; yvisx=yv; //xv,yv перевычислю но не испорчу центр точку
    // xpisx=xp;ypisx=yp;
     xlisx=xl;ylisx=yl; // левая точка ходит по нормали
  // fprintf(f1,"\nposl центр pr=%d,def1=%7.2lf,def2=%7.2lf",pr,def1,def2);
   if(masnorm[nver].point[ntoch+npr].ukp!=NULL)
    { def1prev=def1; def2prev=def2;
    pouglam(i,nverp+nach);
     }
       //i->nverp+nach-интервал проверки,портит  Tmax in corrtg
    //центр проверяю всегда, нужно для пересчёта
       double dt1,dt2;
    for(int k=1;k<=npr;k++)
   {  //teta старый не меняю для одной нормали ВУ надо менять,
      // но по углам она пересчитывается
   ntoch=k; nugl=0;//центр в новой точке
   dt1=k*dpr*cos(teta)/sin(teta); //нельзя /tan при pi/2!
  dt2= -k*dpr/sin(teta);//было наоборот , ошибка
 // это приращение тангенсов. левый t1, смещается левый
 //сравнивать новые t1,t2 c Tmax!
 if((-dt1>t1isx) || (-dt2>t2isx-lprmin)) goto M1;// вершина ушла за правую точку
       //  dt1=b; dt2=a;//нужны оба в M1
   //  if( masnorm[nver].point[ntoch+npr].ukp==NULL) continue;//нет углов на левой
       if(masnorm[nver].point[ntoch+npr].ukp==NULL)
         goto M1;// на левой нет точки  ntoch+npr
// сделать точку потом с этим углом ( если есть) или с другими
    t1=t1isx+dt1;
    t2= t2isx+dt2;
      xl=xlisx-k*dpr*sin(ugl); yl=ylisx+k*dpr*cos(ugl);
    //избавился от pi2 ugl+pi2=beta.        }
    Tmax=t1;
    if(Tmax>t2-lprmin)Tmax=t2-lprmin;
    def1=def1isx-dt1;// добавка с минусом dpr/sin(teta)i
    def2=def2isx-dt2;//добавка dpr/tan(teta). новый def=def-добавка

    xv=xvisx-dt2*cos(ugp); //смещение по второму тангенсу равно с минусом его изменению
    yv=yvisx-dt2*sin(ugp);
 // if(ipp)fprintf(f1,"\ndef1=%7.2lf,def2=%7.2lf,xvi=%8.3lf,xv=%8.3lf,yvi=%8.3lf,yv=%8.3lf",
   // def1,def2,xvisx,xv,yvisx,yv);
      //смещение по второму тангенсу равно с минусом его изменению
    // xv=xvisx+dt1*cos(ugl);
    // yv=yvisx+dt1*sin(ugl);
     //для нового центра новая ВУ по углам она используется
      //сделал новую центральную точку и ВУ
   if ( masnorm[nver].point[ntoch+npr].ukp[nug+nugl].sv!=NULL)
 {//есть и точка и угол
       prvu=proverka(xv,yv,0.0,i,nverp+nach);

   //хорошо при 0,1-внутри кривой 2-велика  биссектриса Bcmin
   //Если 2(велика биссектриса), нельзято надо бы по углам!!!!
   if(!prvu&& (def1<=0)&&(def2<=0))  // есть prvu уже исключён!!!
        postr(xl,yl,xp,yp,xv,yv,Bc,i,nverp+nach,0.0);
 } //конец есть точка и угол
 //else     def1prev=def1; отладка
      def1prev=def1;def2prev=def2;
     pouglam(i,nverp+nach); //проверил после шага вверх
 //fprintf(f1,"\nцентр вверх проверен pr=%d,k=%d,dt1=%7.3lf,dt2=%7.3lf",
   //  pr,k,dt1,dt2);
 M1:  ntoch=-k;//вниз с минусом
         nugl=0;
    if(masnorm[nver].point[ntoch+npr].ukp==NULL)continue;
    //на левой нет такой точки
  //точку надо сделать и потом проверить по углам
       dt1= -dt1;//k*dpr/sin(teta);
    if(-dt1>t1isx) continue;// вершина ушла за левую точку
       dt2=-dt2;//k*dpr/tan(teta);
    if(-dt2>t2isx-lprmin) continue;// вершина ушла за правую точку
       t1=t1isx+dt1;
       t2= t2isx+dt2;
       xl=xlisx+k*dpr*sin(ugl);yl=ylisx-k*dpr*cos(ugl);
        Tmax=t1;
    if(Tmax>t2-lprmin)Tmax=t2-lprmin;//в posl не обязательна lprmin
      def1=def1isx-dt1;// добавка с минусом dpr/sin(teta)i
  //был +dt1 противоречие! надо -dt1 9добавка к тангенсу dt1 деф=надо -есть
      def2=def2isx-dt2;//добавка dpr/tan(teta). новый def=def-добавка
      //сделал новую центральную точку

    xv=xvisx-dt2*cos(ugp); //смещение по второму тангенсу равно с минусом его изменению
     yv=yvisx-dt2*sin(ugp);
  if( masnorm[nver].point[ntoch+npr].ukp[nug+nugl].sv!=NULL)
   {//есть точка и угол
   //если угла нет то по другим углам из этой точки
    prvu=proverka(xv,yv,0.0,i,nverp+nach);
    //1-внутри кривой 2-велика  биссектриса Bcmin
    //fprintf(f1,"\nperv vniz prvu=%d,nver=%d,def1=%7.2lf,def2=%7.2lf",
     //prvu,nver,def1,def2);
    //  if(prvu) continue;
   //на след точку той же нормали
    if(!prvu&&(def1<=0)&&(def2<=0))
  //fprintf(f1,"\nРасчёт  в новой центральной точке k=%d",k);
       postr(xl,yl,xp,yp,xv,yv,Bc,i,nverp+nach,0.0);
   } //конец есть точка и угол
 //else    def1prev=def1; отладка
      def1prev=def1;  def2prev=def2;
      pouglam(i,nverp+nach); //проверил после шага вниз
    }//for k
    }//for nver

 if(!nallev && j2!=j3){ShowMessage(" Увеличить максимально допустимое отклонение");
 fprintf(f1,"\nj2=%d,j3=%d,nverp=%d,ntochp=%d",j2,j3,nverp,ntochp);
 fprintf(f1,"\nPosl.nallev=0. Нет точек на j2-j3 для связи с концом");
  fclose(f1); exit(0);
                       }
      }//for nuglp  на правой нормали
      }//for ntochp точки правой нормали

  //если не было точек ни на одной левой из интервала ->  обрыв
 //не перебирая точки и углы правой
   }//posl
   /*
 int obrclotnew(double L,double teta,double Tmax,double &t,double &Rmax1)
 {  double R,R1; int j;  //t-Tmax ,L Rmax1-без нарушения Tmax
 Rmin1=(L+lkmin)/fabs(teta); //>Rmax

 // if(Rmin1>Rmax) return 5;
  if(Rmin1<Rmin) Rmin1=Rmin;//Rmin1-min при L,teta,Rmin заданный
  if(Rmin1>Ropt) return 5;
 s1 =fabs(teta/2.); s2= L*L/24.;
    //тангенс при Rmin1 меньше радиус нельзя по углу мала сумма длин
t=(Rmin1+s2/Rmin1)*tan(s1)+L/2.; //t-тангенс при Rmin1
 if(t>Tmax)return 5;
   //L нельзя увеличить по углу т.к. вырастет Rmin1
  Bcmin= Rmin1*(1.0/cos(s1)-1.0)+s2/Rmin1/cos(s1);
    j=tgclot( Tmax,L,Bcmax,teta,R,R1);//Rmin1, Bcmin по L и teta миним
   //Bcmax при Tmax- биссектриса R1 радиус далее корр по Rmax
   if(j!=2){fprintf(f1,"\n1obrclot нет корней в tgclot L=%7.2lf,Tmax=%7.2lf",
            L,Tmax);
           fclose(f1); exit(0);
           // return j;
           }
 //if(R1<Rmin1) return (5); //проверено Tmax не достигается при Rmin1
  // при макс тангенсе эту клотоиду надо уменьшать
 //R1 при Tmax! а заходим с новой L!  и выдаём Rmin,t->Tmin
 // R1>Rmax при большом Tmax , уменьшаем и пересчитываем Bcmax
 if(R1>Rmax){j=1;R1=Rmax;}else j=0;
  Bcmax= R1*(1.0/cos(s1)-1.0)+s2/R1/cos(s1);
  //она же из tgclot при Tmax и R1<Rmax
 Rmax1=R1;// max радиус при Tmax,или Rmax при большом Tmax
 //L Rmax  R1 уменьшил,а тангенс не мешает
 return j;
 //при макс тангенсе: j=0-хорошо,1- радиус больше максимального
 //5- радиус меньше минимального при этой клотоиде нет биссектрисы end
 }//end obrclotnew
 */

int obrpr(int in,int i)
  {
    int ipr, col,j,k,jkon,jopt,inj,in1,inopt;
    double s,s1,s2,skon,sopt;
   //ipr=1 продолжаем прямую уже найденную иначе 0
   //то есть новая прямая. col-число найденных прямых,
   //то есть прямая была но кончилась.j конец прямой,in1-начало
   jopt=0;
    ipr=col=0; in1=in;
     j=in; s1=plan[in].s+3.*Lmin;
     while( plan[j].s< s1)j++;//j-конец пробной длины 3.*Lmin
          while (j<=i)
      {
       s2= otkl(in1,j,k,s);
        if(fabs(s2)<dH)ipr=1; else //обрыв
      { if(ipr) //обрыв при попытке удлинить
         {inj=in1;jkon=j-1;skon=plan[j-1].s-plan[in1].s;
           if(!col)//не с чем сравнивать
             { inopt=inj; jopt=jkon; sopt=skon;col++;}
             else { if(skon>sopt)
                      {
 fprintf(f1,"\nobr do inopt=%d,jopt=%d,sopt=%7.2lf",
                    inopt,jopt,sopt);
             sopt=skon;jopt=jkon;inopt=inj;
  fprintf(f1,"\nobr pos inopt=%d,jopt=%d,sopt=%7.2lf",
                    inopt,jopt,sopt);
                      }//skon>sopt
                   } //else k  !col
            ipr=0;
         } //if ipr обрыв при попытке удлинить
       if(col!=0 && in1>=jopt)
    {fprintf(f1,"\n obr== in1=%d,jopt=%d,j=%d,nkr=%d",
                in1,jopt,j,nkr);
          col=0; ipr=0;
       def[nkr].nach=inopt;def[nkr].kon=jopt;
       def[nkr].L=sopt;nkr++;
       s1=plan[in1].s+3.*Lmin;
       if(plan[i].s-s1<3.*Lmin) break; else
         {
          while( plan[in1].s< s1)in1++;
         j=in1; s1=plan[in1].s+3.*Lmin;
         if(plan[i].s-s1<3.*Lmin) break;
         while( plan[j].s< s1)j++;

 fprintf(f1,"\nin1new=%d,jnew=%d",in1,j);
          continue;  //while  j<=i
         } // k else
        }// if(col&& in1==jopt
       else in1++;
       ipr=0;
    } //обрыв  s2>=dH
       j++;
      }//while   j<=i
   if(ipr)//вышли при попытке удлинения
   {   skon=plan[j-1].s-plan[in1].s;
    if(!col)
      {  def[nkr].nach=in1;def[nkr].kon=j-1;
        def[nkr].L=skon;nkr++;
      }
  else  //есть col
    {
      if(skon>sopt)
      {sopt=skon;jopt=j-1;inopt=in1; }
       def[nkr].nach=inopt;def[nkr].kon=jopt;
        def[nkr].L=sopt;nkr++;
    }//col
  }else  //то есть ipr==0
   { if(col)
      {
     def[nkr].nach=inopt;def[nkr].kon=jopt;
        def[nkr].L=sopt;nkr++;
      }//есть col
    }//ipr==0 вышли при поиске новой длины
   fprintf(f1,"\n obr in=%d,i=%d,col=%d,nkr=%d",in,i,col,nkr);
 if(nkr) {  for (int j=0;j<nkr;j++)
    { s2= otkl(def[j].nach,def[j].kon,k,s);
    fprintf(f1,"\n obr j=%d,nach=%d,kon=%d,s2=%9.5lf,in=%d,i=%d",
                      j,def[j].nach,def[j].kon,s2,in,i);
    } //nkr
        }
      return nkr;
   }//obrpr











