//---------------------------------------------------------------------------
#pragma hdrstop
#include "Unit3.h"
//my

//---------------------------------------------------------------------------
#include<vcl.h>
#pragma package(smart_init)
#include "extern.h"
#include <math.h>
//#include <io.h>
#include <conio.h>
#include <string.h>
#include<process.h>
#include<stdio.h>
#include "Unit4.h"
extern TForm4 *Form4;
extern double * beta;
double CONV_ANGLE(char sig,int grad,int min,double sec);//Unit4
void CONV_GRAD( double ugl,char &sig,int& grad,int& min,double& sec);
double mt(double LM, double SM);
int trans_rad_to_gm(double angle,long *grad, long *min);
int append_information(int i,double*p,int*np,double*t,int*nt,double *tk,int*ntk);
 double clox(double k,double l);
  double cloy(double k,double l);
int  conla(double xl,double yl,double phil,double sig,double l,
			 double *xr,double *yr,double *phir);
 int conl(double ksi,double eta,double phi,double sig,double k,double l,
	  double *ksi1,double *eta1,double *phi1,double *sig1);
double cloxs_1 (double x0,double k,double sig, double phi, double l);
double cloys_1 (double y0,double k,double sig, double phi, double l);


double pravxy1(double x0,double y0,double phi0 );
int print_trace_p( void );
extern vizov(char*);
double PI= 3.1415926;
extern int razpech;
double CONV_ANGLE(char sig, int grad,int min,double sec)
{
  double g;
  g=(grad+(min+sec/60.)/60.)/180.*PI;
  if(sig==1)g=-g;
  return g;
}
void CONV_GRAD( double ugl,char &sig,int& grad,int& min,double& sec)
{
double ug,a;
ug=ugl;
if(ugl<0.0){ug=-ugl; sig=1;} else sig=0;
a=ug*180.0/PI;//угол в долях градуса
grad=a; //отбросил дробную часть
a=(a-grad)*60.0;//число минут дробное
min=a;  //отбросил дробную часть
sec=(a-min)*60.0;

}
double mt(double LM, double SM)
{  double res;

  res=(((1./17280.*(LM*SM*LM*SM)-1./120.)*(LM*SM*LM*SM))+1)*LM/2.;
  return(res);
}
 int trans_rad_to_gm(double angle,long *grad, long *min)
{ //даёт градусы и минуты оба со знаком
 long allmin;
 int sig;
 if  (angle<0)
 {
     sig=-1;
     angle*=-1;
 }
// allmin = KRADMIN*angle+0.5;
 allmin = 10800.0/3.1415926*angle+0.5;
 *grad= allmin/60;
	allmin-=*grad*60;
 *min = allmin;
 if (sig==-1)
 {
    if (*grad>0) {  *grad*=-1; return(0);}// этого не бывает, ноль или минус
    if (*min>0) {  *min*=-1; return(0);}
 }
 return(0);
}
////////
int append_information(int num,double*p,int*np,double*t,int*nt,double*tk,int*ntk)
{
 double rad_c,rad_l,rad_r,sig_c,sig_l,sig_r;
 double alf;
 static  double  tk_first,tk_last,p1,dt;
/*p1-сдвижка справа,dt-поправка к тангенсам*/
/*  FILE * fi;
 fi=fopen("tan.dbg","wt");
 fprintf(fi,"\n tkfirst=%9.4lf,tklast=%9.4lf",tk_first,tk_last);
 calc_append_information(i,trace_elts_curr,&p,&np,&t,&nt,&tk,&ntk);

  */
  //num на 1 больше номера элемента если считать с нул
 if (num!=1)rad_l  = elts[num-2].rad;    //предыдущий
 else rad_l = 0.;
 if (  num!=n_elts)
     rad_r  = elts[num].rad;// следующий cur->next->rad;
 else rad_r= 0.;
 rad_c  = elts[num-1].rad;// текущий cur->rad;

 sig_l = 0.; sig_c = 0.; sig_r=0.;
 if (fabs(rad_l)>0.00001) sig_l = 1./rad_l;
 if (fabs(rad_c)>0.00001) sig_c = 1./rad_c;
 if (fabs(rad_r)>0.00001) sig_r = 1./rad_r;
/*fprintf(fi,"\nsigl=%9.4lf,sigc=%9.4lf,sigr=%9.4lf",sig_l,sig_c,sig_r);*/

// *p = (cur->l_clot)*(cur->l_clot)/24.*(sig_c-sig_l);
 *p = (elts[num-1].l_clot)*(elts[num-1].l_clot)/24.*(sig_c-sig_l);//вопрос о знаке и почему разность

/*fprintf(fi,"\n*p=%9.5lf,l_clot=%8.3f",*p,cur->l_clot);*/
 *np = num;
 if (fabs(sig_c)<1.e-15)  *ntk=-1;//==0
 else
 {
//   alf = sig_c*((cur->l_clot+cur->next->l_clot)/2. + cur->l_circ);
alf = sig_c*((elts[num-1].l_clot+elts[num].l_clot)/2. +elts[num-1].l_circ);
// почему с переходными? в формулах не так чистый угол надо!
   *tk = (rad_c+ (*p))*tan(alf/2);
/*fprintf(fi,"\n radc=%9.5lf,*p=%9.5lf,alf=%9.5lf,tk=%9.5lf",rad_c,*p,alf,*tk);*/
   *ntk = num;
   if (sig_l == 0)  tk_first = *tk;
   if (sig_r == 0)
   {
     /* tk_last = *tk;было только это*/
//     p1=(cur->next->l_clot)*(cur->next->l_clot)/24.*sig_c;/*p справа!*/
  p1=(elts[num].l_clot)*(elts[num].l_clot)/24.*sig_c;/*p справа!*/

     /*    fprintf(fi,"\n дл tk_last p1=%9.4lf",p1 );*/
    tk_last=(rad_c+ p1)*tan(alf/2);

     //if((sig_l==0) && (sig_r==0))
  if((fabs(sig_l)<1.e-15) && (fabs(sig_r)<1.e-15))
      {/*поправки похоже не завист от знака угла*/
       dt=(*p-p1)/sin(alf);
       tk_first=tk_first-dt;
       tk_last=tk_last+dt;
       *ntk=-1;//было ниже в отдельном операторе
      }
/*fprintf(fi,"\n 2radc=%9.5lf,alf=%9.5lf,tk=%9.5lf",rad_c,alf,tk_last);*/
   }//sigr==0
/*  fprintf(fi,"\n tk=%9.4lf, tk_f =%9.4lf,tk_l =%9.4lf,alf=%9.4lf",
   *tk,tk_first,tk_last,alf);*/
 }
// if (sig_l ==  0 && sig_r == 0) *ntk=-1; перенёс выше в то же условие
 *nt = -1;

  if (fabs(sig_l)<1.e-15)
 {
  *nt =  num;
  *t = tk_first + mt(elts[num-1].l_clot,sig_c);
/*fprintf(fi,"\n sigl=0 и mt=%9.4lf,t=%9.4lf,tk_first=%9.4lf",(*t-tk_first),*t,tk_first);*/
 }
 if (fabs(sig_c)<1.e-15)
 {
  *ntk =  -1;
  *nt = num;
//    *t = tk_last + mt(cur->l_clot,sig_l);
  *t = tk_last + mt(elts[num-1].l_clot,sig_l);

  /* fprintf(fi,"\nsigc=0 и mt=%9.4lf,t=%9.4lf,tk_last=%9.4lf",*t-tk_last,*t,tk_last);*/
 }
  /*if(!(cur->next)) fclose(fi);*/
return(1);
}
///
int  conla(double xl,double yl,double phil,double sig,double l,
			 double *xr,double *yr,double *phir)
/*      *********** ДВИЖЕНИЕ ПО КРИВОЙ  *************/
 {
     double cos_phi,sin_phi,dphi,dx,dy,dxn,dyn;
	   cos_phi =cos(phil);
	   sin_phi =sin(phil);
     dphi= l*sig;
     /* вставить обработку плохой кривизны */
     dx  = sin(dphi)/sig;
     dy  = (1- cos(dphi))/sig;

	   dxn  = dx*cos_phi - dy*sin_phi;
	   dyn  = dy*cos_phi + dx*sin_phi;
     *xr  = xl+dxn;
     *yr  = yl+dyn;
     *phir = phil + dphi;
/*похоже что здесь кривизна со знаком и все верно*/
     return(0);


 }

 int conl(double ksi,double eta,double phi,double sig,double k,double l,
	  double *ksi1,double *eta1,double *phi1,double *sig1)
 {
/*        *********** ДВИЖЕНИЕ ПО КЛОТОИДЕ  при малом k по кривой*************/
	double l0,phi0,x0,y0,x1,y1,l1,sin0,cos0;
///
   *sig1=sig+k*l;
   *phi1=phi+l*(sig+l*k/2);


	if(fabs(sig)>0.000001)
    {
      *ksi1=cloxs_1(ksi,k,sig,phi,l);
	*eta1=cloys_1(eta,k,sig,phi,l);
	return 0;
     }
  /*
      x1=cloxf(k,l)
	y1=cloyf(k,l) ! это в повёрнутой и сдвинутой системе (новые)
	ksi1=ksi+x1*cos(phi)-y1*sin(phi)
	eta1=eta+x1*sin(phi)+y1*cos(phi) ! в старой системе
    7 continue
		return
   */
///
	double eps=0.000000001;
	if ( k>-eps && k<eps)
	{
	   conla(ksi,eta,phi,sig,l,&(*ksi1),&(*eta1),&(*phi1));
	}
	else
	{

	l0=sig/k;
	phi0=phi-k*l0*l0/2;
	x0=clox(k,l0);   y0=cloy(k,l0);
	sin0=sin(phi0);  cos0=cos(phi0);
	l1=l0+l;
	x1=clox(k,l1);   y1=cloy(k,l1);
	*ksi1=ksi+((x1-x0)*cos0 - (y1-y0)*sin0);

	*eta1=eta+((x1-x0)*sin0 + (y1-y0)*cos0);
	}


	return(0);
 }

 double clox(double k,double l)
 {
  double eps = 0.0000001;
/*  double eps1 = 0.000000001;*/
  double q,a,b,t_clox;
  int i;
	   if (l<eps && l>-eps) return(0.);
	    q = k*l*l/2;  q = -q*q;
	    b = l;
	    t_clox = l;
           i=0;
	    do
	    {
              i++;
	      if (i>=100) { ShowMessage ("ЗAЦИKЛИBAHИE B CLOX");
               exit(0);   }

	      b = b*q/(2*i)/(2*i-1);
	      a = b/(4*i+1);
	      t_clox = t_clox + a;
	     }
	    while (a>eps || a<-eps);
	     return(t_clox);
 }

 double cloy(double k,double l)
 {
/*FILE *fy;*/
  double eps = 0.0000001;
/*  double eps1 = 0.000000001;*/
  double a,b,q,t_cloy;
  int i;
/*fy=fopen("testcly.pch","wt");
fprintf(fy,"\n k=%10.8lf,l=%7.3lf",k,l);
*/
	   if (l<eps && l>-eps) return(0);
	    q = k*l*l/2;  q = -q*q;
	    b = k*l*l*l/2;
	    t_cloy = b/3;
            i=0;
	    do
          {
                i++;
/*fprintf(fy,"\n i=%d,b=%10.6lf,q=%10.6lf,t_cloy=%10.5lf",i,b,q,t_cloy);*/
	     if (i>=100){
/*fclose(f); fclose(fy);*/
ShowMessage("*** ЗAЦИKЛИBAHИE B CLOY");
 exit(0);                        }
	      b = b*q/(2*i+1)/(2*i);
	      a = b/(4*i+3);
	      t_cloy = t_cloy + a;
/*fprintf(fy,"\n i=%d,a=%10.6lf,b=%10.6lf,t_cloy=%10.5lf",i,a,b,t_cloy);*/
          }
              while (a>eps || a<-eps);
/*fclose(fy);*/
	     return(t_cloy);
  }
///
 double cloxs_1 (double x0,double k,double sig, double phi, double l)
    {
    /*
    Входные данные этой функции : x0- абсцисса начала куска клотоиды, К- параметр
    клотоиды, sig-кривизна, phi-угол, l-длина куска клотоиды
    Входные данные к программе см. в main
    Возвращает абсциссу конца и печатает i- порядок производной -1
    В clox и в cloxs- i имеет разный смысл.
    Алгоритм: хранится последняя из вычисленных строка коэффициентов по степеням sig
    но без синусов( косинусов) это массив prev в начале есть
    prev[0]=1 так как первая производная равна cosp.
    При дифференцировании множитеоя с sig (шаг влево) степень понижается
    и поэтому новый элемент тот же, но умноженный на k и степень делённый на sig, а при
    дифференцировании sin или cos ( шаг вправо) умножаем на sig*prt. prt=1 если
    в текущем столбце синус, если косинус то -1. Слагаемое в сумму для производной
    вычисляется с учётом синусов и косинусов ( множитель sc) Шаг вправо меняет prt
    а влево не меняет
     */
  int i;//i+1-порядок вычисляемой производной, но не степени L!!!
  int prn,prt; // признак равен 1 работа с синусом
  double prev[60];// массив слагаемых в строке
  int j,j1; // рабочие
  double st,s,s1,sc; // рабочие: st- степень делённая на факториал,
                     //s- сумма в строке,sc- множитель синус или косинус
  double x;// результат
  double cosp,sinp,E;
  E=0.000001;
  cosp=cos(phi); sinp=sin(phi);
  x=x0;
  prn=-1;//1 -есть  синус в начальном элементе исходной строки  иначе -1
  // prt; наличие синуса в текущем элементе исходной строки если нет то косинус

  st=l;  x=x+cosp*st;
  for(j1=0;j1<60;j1++) prev[j1]=0.0;
  prev[0]=1.0;
  i=1;// i- номер последней заполненной строки, то есть уже вычисленной производной
  //начинаем со второй производной но исходная строка первая
  while (1)
  {
  // для вычисления и хранения новой строки слагаемых упорядоченных по степеням sig
   j=!(i%2);// j- номер начального не нулевого элемента ( степени sig) в заполненной (i-ой) строке
   // в нечётных заполненных это нулевой элемент, а строки нумерую с нуля)
    //  с какого столбца идём влево- вправо в исходной строке
  prt=prn;// синуса не было, был косинус
  st=st*l/(i+1);// заготовка под степени по l
  s=0.0;//сумма по строке в конце её умножим на степень делённую на факториал
  for (j1=j;j1<i; j1=j1+2) //j1-это степень sig!!!
    {
if(j1>0)// можно влево
{if(prt==1)sc=sinp; else sc=cosp;
s1=prev[j1]*j1*k/sig;
prev[j1-1]=prev[j1-1]+s1;// это без синусов и косинусов
  s=s+s1*sc;

}// шаг влево j1- очередной столбец
 //теперь шаг вправо
if (prt==1) sc=cosp; else sc=sinp;// меняем sc
s1=prev[j1]*sig*prt; // здесь учтён минус если дифференцировали косинус
prev[j1+1]=prev[j1+1]+s1;
  s=s+s1*sc;
  prt=-prt;// меняем признак наличия синуса при переходе к новому столбцу *( через1)
  prev[j1]=0.0;//сходили налево и направо а сам в новой строке стал нулём
    }//for
  s1= s*st;
    x=x+s1;
    if((fabs(s1)<E)&&(i>5)) break;
      if(!j)prn=-prn; // наличие синуса в начале строки меняется через раз
 if(i>58){ShowMessage("\n CIRCLE IN CLOXS"); return x;}
        i++; // на ноыую строку для новой производной

  }//while
//printf("\n IN CLOXS_1  i=%d",i);
//getch();
   return x;
   }//cloxs_1

        double cloys_1 (double y0,double k,double sig, double phi, double l)
    {
    /*
    Входные данные этой функции : y0- ордината начала куска клотоиды, К- параметр
    клотоиды, sig-кривизна, phi-угол, l-длина куска клотоиды
    Входные данные к программе см. в main
    Возвращает ординату конца и печатает i- порядок производной -1
    В cloy и в cloys- i имеет разный смысл.
    Алгоритм: хранится последняя из вычисленных строка коэффициентов по степеням sig
    но без синусов( косинусов) это массив prev в начале есть
    prev[0]=1 так как первая производная равна sinp.
    При дифференцировании множителя с sig (шаг влево) степень понижается
    и поэтому новый элемент тот же, но умноженный на k и на показатель степени
   и делённый на sig, а при  дифференцировании sin или cos ( шаг вправо)
умножаем на sig*prt. prt=1 если  в текущем столбце синус, если косинус то -1.
   Слагаемое в сумму для производной  вычисляется с учётом синусов и косинусов
   ( множитель sc) Шаг вправо меняет prt а влево не меняет
     */
  int i;//i+1-порядок вычисляемой производной, но не степени L!!!
  int prn,prt; // признак равен 1 работа с синусом
  double prev[60];// массив слагаемых в  строке
  int j,j1; // рабочие
  double st,s,s1,sc; // рабочие: st- степень делённая на факториал,
                     //s- сумма в строке,sc- множитель синус или косинус
  double y;// результат
  double cosp,sinp,E;
  E=0.000001;
  cosp=cos(phi); sinp=sin(phi);
  y=y0;
  prn=1;//1 -есть  синус в начальном элементе исходной строки  иначе -1
  // prt; наличие синуса в текущем элементе исходной строки если нет то косинус

  st=l;  y=y+sinp*st;
    for(j1=0;j1<60;j1++) prev[j1]=0.0;
  prev[0]=1.0;
  i=1;// i- номер последней заполненной строки, то есть уже вычисленной производной
  //начинаем со второй производной но исходная строка первая
  while (1)
  {

  // для вычисления и хранения новой строки слагаемых упорядоченных по степеням sig
  j=!(i%2);// j- номер начального не нулевого элемента ( степени sig) в заполненной (i-ой) строке
   // в нечётных заполненных это нулевой элемент, а строки нумерую с нуля)
    //  с какого столбца идём влево- вправо в исходной строке
  prt=prn;// косинуса не было, был синус
  st=st*l/(i+1);// заготовка под степени по l
  s=0.0;//сумма по строке в конце её умножим на степень делённую на факториал
  for (j1=j;j1<i; j1=j1+2) //j1-это степень sig!!!
    {
if(j1>0)// можно влево
{if(prt==1)sc=sinp; else sc=cosp;
s1=prev[j1]*j1*k/sig;
prev[j1-1]=prev[j1-1]+s1;// это без синусов и косинусов
  s=s+s1*sc;

}// шаг влево j1- очередной столбец
 //теперь шаг вправо
if (prt==1) sc=cosp; else sc=sinp;// меняем sc
s1=prev[j1]*sig*prt; // здесь учтён минус если дифференцировали косинус
prev[j1+1]=prev[j1+1]+s1;
  s=s+s1*sc;
  prt=-prt;// меняем признак наличия синуса при переходе к новому столбцу *( через1)
    prev[j1]=0.0;
    }//for
  s1= s*st;
    y=y+s1;
    if((fabs(s1)<E)&&(i>5)) break;
   // if(fabs(s1)<E) break;

      if(!j)prn=-prn; // наличие синуса в начале строки меняется через раз
 if(i>58){ShowMessage("\n CIRCLE IN CLOYS"); return y;}
        i++; // на новую строку для новой производной

  }//while

//printf("\n IN CLOYS_1  i=%d",i);
//getch();
     return y;

    }//cloys_1

////
double pravxy1(double x0,double y0,double phi0 )
{//здесь не нужна
//идёт по длинам снятых хорд, откладывает эти длины по  пpоектной трассе
// и даёт координаты по трассе и длину трассы
   double x, y,lsum, phi, sig, k,lx,xp,yp,lxsum,llom;
   double ksi1,eta1,phi1,sig1,sig0,sig2;
   int j;
   int i,i1;
      FILE* ff;
      ff=fopen("mamitko.txt","wt");
     fprintf (ff,"\nНач.точка  Х=%12.5lf, Y=%12.5lf",x0,y0);
    phi=0.000518/15.98994; //phi=phi*180/3.1415926;
    char a;
    CONV_GRAD( phi,a,i,j,sig);
    fprintf (ff,"\nрад=%9.7lf",phi);
    fprintf(ff,"\ngr=%d, min=%d, sec=%12.7lf", i,j,sig);
//   fclose(ff); exit(0);
	 xp=x0; yp=y0; lxsum=0.0;
         llom=tr[n_pts-1].ev_x;// последн точка это обща длина
      for(i=0;i<n_pts&& lxsum<=l_first;i++,lxsum+=tr[i].dist)
   {    xp=x0+tr[i].ev_x*cos(phi0);
	yp=y0+tr[i].ev_x*sin(phi0);
       tr[i].ev_x =xp;
       tr[i].ev_y =yp;
/*fprintf(fp,"\nпрма evx=%7.2f,lfir=%6.2f", trace_pts_curr->ev_x,l_first);*/

   } //во всех циклах надо сохранть номера точек и элементов!!!
//     какой i после цикла for???
      lsum=l_first;
      x =x0+ l_first*cos(phi0);
      y =y0+ l_first*sin(phi0);
     fprintf (ff,"\nКонец нач. прямой L=%10.5lf, Х=%12.5lf, Y=%12.5lf",
     l_first,x,y);
      phi = phi0;
      sig = 0;
      sig0=sig;
      xp=x; yp=y;
      lx= lxsum- lsum;
        for(i1=0;i1<n_elts;i1++)
        //по всем кроме последней клотоиды в списке она была
        //и в массиве её номер n_pts
   {
       if( elts[i1].l_clot > 0.0012 )
     {

	if( elts[i1].rad == 0 )
	 k = -sig/elts[i1].l_clot;
	else
	 k = (1./elts[i1].rad-sig)/elts[i1].l_clot;
     if(lx<=elts[i1].l_clot)
     {
              // сохранился ли i  после цикла for ДА!
            for(; (i<n_pts)&&(lxsum<=lsum+elts[i1].l_clot); i++,lxsum+=lx)
	{
	  conl(xp,yp,phi,sig, k,lx,&ksi1,&eta1,&phi1,&sig1);

          tr[i].ev_x=ksi1;  tr[i].ev_y=eta1;
	  xp=ksi1; yp=eta1; phi=phi1; sig=sig1;

          if(i<n_pts-1)  // не последняя
	    {
             lx=tr[i+1].dist;
	    }
	 } /*for*/

      }
  /*fprintf(fp,"\n do conl x=%7.2lf,y=%7.2lf,phi=%9.5lf,k=%7.4lf,sig=%7.2lf",
	     x,y,phi,k,sig);*/

//  conl(x,y,phi0,sig0, k,(double) trace_elts_curr->l_clot,&x,&y,&phi0,&sig0);
  conl(x,y,phi0,sig0, k, elts[i1].l_clot,&x,&y,&phi0,&sig0);
 fprintf (ff,"\nКонец клотоиды L=%9.5lf, Х=%12.5lf, Y=%12.5lf, Угол=%9.5lf",
elts[i1].l_clot, x,y,phi0);
	    lsum=lsum+elts[i1].l_clot;
  /*fprintf(fp,"\nпосле conl x=%7.2lf,y=%7.2lf,phi=%9.5lf,sig0=%7.4lf,lsum=%7.2lf",
	     x,y,phi,sig0,lsum);
   */
      } /*при нулевой клотоиде сразу сюда*/
      if( fabs(elts[i1].rad)<1.e-15 )//==0
	 { sig = 0; j=0;}
      else
	{  sig = 1./elts[i1].rad; j=1;}
  xp= x;
  yp= y;
  phi=phi0;
  sig0=sig;
  lx= lxsum- lsum;
  if(lx<=elts[i1].l_circ)
    {

    for(;i<n_pts && lxsum<=lsum+elts[i1].l_circ;i++,lxsum+=lx)
      {
         if(j==0)
          {
              ksi1=xp+lx*cos(phi);
              eta1=yp+lx*sin(phi);
          }
	  else
	   {

	 conla(xp,yp,phi,sig,lx,&ksi1,&eta1,&phi1);
         phi=phi1;
	   }
         tr[i].ev_x=ksi1;  tr[i].ev_y=eta1;
         xp=ksi1; yp=eta1;

         if( i<n_pts-1)
	    {
//            lx=trace_pts_curr->next->dist;
            lx=tr[i+1].dist;
            }
       } /*конец кривой или прмой*/
    }
        if(j==0)
          {
              ksi1=x+(elts[i1].l_circ)*cos(phi);
              eta1=y+(elts[i1].l_circ)*sin(phi);
fprintf (ff,"\nКонец прямой L=%9.5lf, Х=%12.5lf, Y=%12.5lf, Угол=%9.5lf",
elts[i1].l_circ,ksi1,eta1,phi);
          }
        else
//       conla(x,y,phi0,sig,(double) trace_elts_curr->l_circ,&ksi1,&eta1,&phi1);
       conla(x,y,phi0,sig,elts[i1].l_circ,&ksi1,&eta1,&phi1);
         x=ksi1;  y=eta1;lsum=lsum+ elts[i1].l_circ;
fprintf (ff,"\nКонец окр L=%9.5lf, Х=%12.5lf, Y=%12.5lf, Угол=%9.5lf",
elts[i1].l_circ,x,y,phi1);
	       if(j)  phi = phi1; /* sig0 осталс*/
         phi0=phi; lx=lxsum-lsum; xp=x; yp=y;
 /*fprintf(fp,"\nпосле conla x=%7.2lf,y=%7.2lf,phi=%9.5lf,sig=%7.4lf,lsum=%7.2lf",
	     x,y,phi,sig,lsum);
   */
   }
 /*fprintf(fp,"\nlxsum=%8.3lf,lsum=%8.3lf",lxsum,lsum);*/
   /* первый цикл все элементы кроме последнего*/
   if( elts[i1].l_clot>0.0001 )
  {
  /*fprintf(fp,"\n2l_clot=%11.7f", trace_elts_curr->l_clot);*/
      k = -sig/elts[i1].l_clot;
      xp= x;
      yp= y;
     phi0=phi;
     sig0=sig;
      lx= lxsum- lsum;
 /*fprintf(fp,"\n до клот в цикле xp=%7.2lf,yp=%7.2lf,phi=%9.5lf,sig=%9.6lf,k=%9.6lf",
	     xp,yp,phi,sig,k);
 */
    if(lx<=elts[i1].l_clot)
   {
/*   for( ;
	trace_pts_curr != NULL &&
	   lxsum <= lsum+trace_elts_curr->l_clot;
	trace_pts_curr = trace_pts_curr->next,
           lxsum+= lx )
           */
     for(;(i<n_pts )&& (lxsum<=lsum+elts[i1].l_clot); i++,lxsum+=lx)
    {
	  conl(xp,yp,phi,sig, k,lx,&ksi1,&eta1,&phi1,&sig1);
          tr[i].ev_x=ksi1;  tr[i].ev_y=eta1;
	  xp=ksi1;yp=eta1; phi=phi1; sig=sig1;
/*          if( trace_pts_curr->next)
	    {
            lx=trace_pts_curr->next->dist;
            }
  */
           if(i<n_pts-1) lx=tr[i+1].dist;
     }/* for*/
  }
 /*fprintf(fp,"\nпосле клот xp=%7.2lf,yp=%7.2lf,phi=%9.5lf,sig=%9.6lf,lxsum=%7.2lf",
	     xp,yp,phi,sig,lxsum);
  fprintf(fp,"\ndo conl x=%7.2lf,y=%7.2lf,phi0=%9.5lf,k=%9.6lf,sig0=%9.6lf,lsum=%7.2lf",
	     x,y,phi0,k,sig0,lsum);
  fprintf(fp,"\n3l_clot=%11.7f", trace_elts_curr->l_clot);
  */
// conl(x,y,phi0,sig0, k,(double)trace_elts_curr->l_clot,&ksi1,&eta1,&phi1,&sig1);
 conl(x,y,phi0,sig0, k,elts[i1].l_clot,&ksi1,&eta1,&phi1,&sig1);
          x=ksi1;  y=eta1; phi=phi1;lsum+=elts[i1].l_clot;
 fprintf (ff,"\nКонец клотоиды L=%9.5lf, Х=%12.5lf, Y=%12.5lf, Угол=%9.5lf",
elts[i1].l_clot, x,y,phi1);
  /*fprintf(fp,"\nпосле conl x=%7.2lf,y=%7.2lf,phi0=%9.5lf,phi=%9.5lf,sig=%7.2lf,lsum=%7.2lf",
	     x,y,phi0,phi,sig,lsum);
    */
 }  /*при нулевой клотоиде сразу сюда*/
   sig = 0;
/*fprintf(fp,"\n 2за клот x=%7.2f,y=%7.2f,phi=%8.5f,lsum=%8.3lf,lxsum=%8.3lf",
x,y,phi,lsum,lxsum);
  */
  xp= x;
  yp= y;
  lx= lxsum- lsum;
//   for( ;trace_pts_curr != NULL;trace_pts_curr = trace_pts_curr->next )
 for(;i<n_pts;i++)
   {
    /* fprintf(fp,"\nx=%7.2lf,y=%7.2lf,phi=%7.4lf,lx=%7.2lf",x,y,phi,lx);
     fprintf(fp,"\nev_x=%7.2f",trace_pts_curr->ev_x);
      */
       xp=xp+ lx*cos(phi);
       yp=yp+ lx*sin(phi);
      /*fprintf(fp,"\nxp=%7.2lf,yp=%7.2lf",xp,yp);*/
       lsum+=lx;
       tr[i].ev_x=xp;
       tr[i].ev_y=yp;
//         if( trace_pts_curr->next) lx=trace_pts_curr->next->dist;
         if( i<n_pts-1) lx=tr[i+1].dist;

    }  /* for*/
/*fprintf(fp,"\nвыхx=%7.2lf,y=%7.2lf,phi=%7.4lf,xp=%8.3lf,yp=%8.3lf,lsum=%7.2lf"
,x,y,phi,xp,yp,lsum);
    */
fprintf (ff,"\nКонец прямой  Х=%12.5lf, Y=%12.5lf", xp,yp,phi);

    xp=xp-(lsum-llom)*cos(phi);    yp=yp-(lsum-llom)*sin(phi);
   /*fprintf(fp,"\nна встрече xp=%8.3lf,yp=%8.3lf, llom=%8.3lf,lsum=%8.3lf",
             xp,yp,llom,lsum);
         fclose(fp);
     */
     fprintf(ff,"\nна встрече xp=%8.3lf,yp=%8.3lf, llom=%8.3lf,lsum=%8.3lf",
             xp,yp,llom,lsum);
             fclose(ff);
    return(lsum);
}
////////  !!!!!! new по элементам
 int print_trace_p( void )
{
  double PI=3.1415926;
   FILE *file;
   FILE *vix;
  /* FILE *f;*/
   CHAINAGE temp_ch,temp_ch1;
//ugl,xl,yl-по extern из extern.h uglpr, xlpr,ylpr- проектные там же
   int    prn, i;
   long    gr_, min_;
   double p,t,tk,p1;
   int np,nt,ntk;
   double angle;
   double dist;// вычислется при обращении в программы но не используется
   char print_file_name[80],str[150];
   char vix_file_name[80];
   //конец переменных для вычисления тангенсов
   double sn=0.0,sp=0.0,xcrl,ycrl,xcr,ycr;
/*длины по хордам существ. и проектна, проект предыдущие*/
/*-----------------------------------------*/
   double  xc,yc,gam,phi,phi0,phi1,sig,sig1,k,x,y,xp,yp;
   char sig0;
   int gr0,mi0; double se0; // из unit 4 по extern  беру угол ugl */
   //для вычисления вершин углов
      double ugl1;
     //char c1,c2;// глобальна
double lsum;
double rix,rixl,rixp;
/*-----------------------------------------*/
   strcpy( print_file_name, elt_name);
   i = strlen(print_file_name) - 3;
   print_file_name[i] = 'P';
    strcpy( vix_file_name,print_file_name);
     vix_file_name[i] = 'V';
  file = fopen( print_file_name, "wt" );
   vix=fopen( vix_file_name,"wt" );//выход  X,Y,X,Y,s

  //  в файле *.tra начальный угол или 100
   /*  ввёл при выборе расчёт участка начало сущ и проектное
 if(ugl>10.){sig0=0;gr0=mi0=0;se0=0.0;}
 else
 CONV_GRAD(ugl, sig0,gr0,mi0,se0);
 if(sig0==1) Form4->sign->Text="-";else Form4->sign->Text="+";
   Form4->gr->Text= AnsiString(gr0);
  Form4->min->Text= AnsiString(mi0);
  Form4->sec->Text= AnsiString(se0);
   Form4->X0->Text= AnsiString(xl);
   Form4->Y0->Text= AnsiString(yl);
               Form4->Tag=0;
Form4->ShowModal();
if(Form4->ModalResult!=mrOk)
{   Form4->Tag=0; //Form4->Close();
   fclose( file ); return(0);
}

Form4->Button1->Visible=0;
*/
//Form4->Button1->Visible=0;
//Form4->Button2->Visible=0;
Form4->Button1->Caption="Посмотреть";
Form4->Caption="Сводные данные. Подробные в файле  уч. p № вар";
      //xl,yl,ugl были в extern при чтении файла*.tra или из формы unit 4
   //здесь начальные координаты должны быть по проектной трассе  !!!!
   //xlpr,ylpr,uplpr-проектное начало всё ввожу при "расчёт участка"
  sprintf( str, "               Участок %s вариант %c%c\n",
         uch_name,print_file_name[i+1],print_file_name[i+2] );
   fputs( str, file );
   fputs( "-------------------------------------------------------------------------\n",
	  file );

           /*
 fprintf(file,"\n                  Начало участка:     ");
 fprintf(file,"\n x0=%14.5lf,  y0=%14.5lf,  угол= %12.8lf\n",xlpr,ylpr,uglpr);
 fprintf(file,"\n                  Элементы:     \n\n");

   for( i = 1;i<=n_elts+1;i++)
   {
      append_information(i,&p,&np,&t,&nt,&tk,&ntk);
      if( elts[i-1].l_clot > 0.002 )
   {
	 if  (nt>0)
        {
	 sprintf( str,
" L%02d = %10.6lf    НПК%02d =%5ld+%8.4lf   КПК%02d =%5ld+%8.4lf  P%02d=%10.3lf  T%02d=%10.3lf",
  i, elts[i-1].l_clot,
  i, elts[i-1].cl_ch0.chain, elts[i-1].cl_ch0.plus,
  i, elts[i-1].cl_ch1.chain,elts[i-1].cl_ch1.plus,np,p,nt,t);
	} else
	 sprintf( str,
" L%02d = %10.6lf    НПК%02d =%5ld+%8.4lf   КПК%02d =%5ld+%8.4lf  P%02d=%10.3lf",
  i, elts[i-1].l_clot,
  i, elts[i-1].cl_ch0.chain, elts[i-1].cl_ch0.plus,
  i, elts[i-1].cl_ch1.chain,elts[i-1].cl_ch1.plus,np,p);
fputs( str, file );
      if( i > n_elts )	 break;
   if( fabs(elts[i-1].rad)<1.e-15)
      {
	 sprintf( str,
		  " П%02d = %11.6lf\n",
		  i,elts[i-1].l_circ);//trace_elts_curr->l_circ );
	 fputs( str, file );
   fputs( "\n-----------------------------------------------------------------------------------------\n",
	  file );
      }
    }  else// net perehodnix
      {
	 if  (nt>0) sprintf( str,"    T%02d=%10.3lf\n",nt,t);
         if( i > n_elts ){	 fputs( str, file );	 break;}
   if( fabs(elts[i-1].rad)<1.e-15)
      {
	 sprintf( str,
		  "  П%02d = %11.6lf\n",
		  i,elts[i-1].l_circ );
	 fputs( str, file );
   fputs( "\n--------------------------------------------------------------------------------\n",
	  file );
      }
        }//end net perehodnix
   if( fabs(elts[i-1].rad)>1.e-15)
{
   if (elts[i-1].l_clot<0.0012) elts[i-1].l_clot=0; //добавлено
//  на полдлины клотоид
  //      count_chain( (temp_ch = elts[i-1].cl_ch0),
    // (dist = elts[i-1].l_clot/2), &temp_ch);
//sprintf(str," К%02d = %6.2lf    НК%02d =%5ld+%5.2lf",i,elts[i-1].l_circ+
 //0.5*(elts[i-1].l_clot+elts[i].l_clot),i,temp_ch.chain,temp_ch.plus);


      count_chain(  elts[i-1].cl_ch0,elts[i-1].l_clot, &temp_ch);
sprintf(str," К%02d = %11.6lf    НК%02d =%5ld+%8.4lf",i,elts[i-1].l_circ,
 i,temp_ch.chain,temp_ch.plus);

   fputs( str, file );
  //   на полклотоиды
   //count_chain( (temp_ch = elts[i-1].cl_ch1),
     //		      (dist =elts[i-1].l_circ+ elts[i].l_clot/2), &temp_ch);
       //	 sprintf( str,
	 //	  "    КК%02d =%5ld+%5.2lf   R%02d =%9.2lf\n",
	   //	  i, temp_ch.chain,temp_ch.plus,
	     //	  i, elts[i-1].rad);//trace_elts_curr->rad );

       count_chain( elts[i-1].cl_ch1, elts[i-1].l_circ, &temp_ch);
       sprintf( str,
		  "    КК%02d =%5ld+%8.4lf   R%02d =%14.6lf\n",
		  i, temp_ch.chain,temp_ch.plus,
		  i, elts[i-1].rad);
	 fputs( str, file );
         if(elts[i].l_clot<0.0012)elts[i].l_clot=0;//добавлено
angle =(elts[i-1].l_circ+0.5*(elts[i-1].l_clot+elts[i].l_clot))/elts[i-1].rad;
  	 trans_rad_to_gm(angle,&gr_,&min_);
	 sprintf( str,
		  "                 Уг%02d = %4ldгр.%2ld мин.",
		  i, gr_, min_);
	 fputs( str, file );
	 if  (ntk>0)
	 sprintf( str,"                       TK%02d=%10.3lf\n",ntk,tk);
	 else  sprintf( str,"\n");
	 fputs( str, file );
     }// R<>0
   }//for  i в элементах поправил НК и КК не посередине а в начале и в конце
     //чистой круговой

   fputs( "\n-------------------------------------------------------------------------\n\n",
	  file );
               */
    int jk=0;//0-пишу начало клотоиды, 1-конец
    struct toch {double x; double y;}*Nach,*Kon;
    Nach=new toch[n_elts]; Kon=new toch[n_elts];
              //НОВОЕ   надо uglpr вместо ugl!!!
    // fprintf(file,"\n xl=%12.3lf, yl=%12.3lf,",xl,yl);
  fprintf(file,"\n ugl=%12.6lf, uglpr=%12.6lf",ugl,uglpr);
         fprintf(file,"\n              КООРДИНАТЫ ПО ЭЛЕМЕНТАМ     \n  ");
  fprintf(file,"\n Начало трассы:" );
  fprintf(file,"\n Координаты:  x=%12.3lf, y=%12.3lf,",xlpr,ylpr);
     CONV_GRAD(ugl, sig0,gr0,mi0,se0);//ugl- в оптимизато нач угл ==0, этот записан
   if(!sig0)
  fprintf(file,"\n Угол по хорде влево в рад. =%10.7lf,или %3dгр.%2d'%6.2lf\"",
  ugl,gr0,mi0,se0); else
   fprintf(file,"\n Угол по хорде вправо в рад. =%10.7lf,или %3dгр.%2d'%6.2lf\"",
  ugl,gr0,mi0,se0);
  //uglpr=ugl;
    CONV_GRAD(uglpr, sig0,gr0,mi0,se0);
    if(!sig0)
  fprintf(file,"\n Угол по касательной влево в рад. =%10.7lf,или %3dгр.%2d'%6.2lf\"",
  uglpr,gr0,mi0,se0); else
  fprintf(file,"\n Угол по касательной вправо в рад. =%10.7lf,или %3dгр.%2d'%6.2lf\"",
  uglpr,gr0,mi0,se0);
      xp =xlpr+ l_first*cos(uglpr);    //pr
      yp =ylpr+ l_first*sin(uglpr);//pr
      phi = uglpr;//pr
//    ;
  fprintf(file,"\n Начальная прямая: L=%10.3lf,xk=%12.3lf,yk=%12.3lf",l_first,xp,yp);
  fprintf(file,"\n==================================================================================");
      sig=0.0; int jnk=0;//номер кривой а не элемента
 for(i=0;i<=n_elts;i++)
 {
   if( elts[i].l_clot > 0.001 )
   { if( fabs(elts[i].rad)>0.001 )k = (1.0/elts[i].rad-sig)/elts[i].l_clot;
    else k=-sig/elts[i].l_clot;//sig-в начале клотоиды на прямой нуль на круговой записывать
    conl(xp,yp,phi,sig, k,elts[i].l_clot,&x,&y,&phi1,&sig1);
     if(!jk){Nach[jnk].x=xp;Nach[jnk].y=yp;jk=1;} else
          {Kon[jnk].x=x;Kon[jnk].y=y;jk=0;jnk++;}
         xp=x; yp=y;
 CONV_GRAD(phi1-phi, sig0,gr0,mi0,se0);

 fprintf(file,"\n Клотоида: L=%9.4lf, xk=%12.4lf,yk=%12.4lf,угол в конце=%9.6lf",
 elts[i].l_clot,x,y,phi1);
if(!sig0)
 fprintf(file,"\n поворот влево на %3dгр.%2d'%6.2lf\"",gr0,mi0,se0); else
 fprintf(file,"\n поворот вправо на %3dгр.%2d'%6.2lf\"",gr0,mi0,se0);

 if(fabs(sig)<1.E-9)p=0.0; else p=1.0/sig;
  if(fabs(sig1)<1.E-9)p1=0.0; else p1=1.0/sig1;
  fprintf(file,"\n радиус в начале=%10.3lf,радиус в конце=%10.3lf",p,p1);
         phi=phi1; sig=sig1;
         if(i==n_elts)
          {
           fprintf(file,"\n===================================================================================");
           break;
          }
         }/*uchli klotoidy */
         if( fabs(elts[i].rad)>0.001 )
         {
         sig=1/ elts[i].rad;
      conla(xp,yp,phi,sig, elts[i].l_circ,&x,&y,&phi1);
      CONV_GRAD(phi1-phi, sig0,gr0,mi0,se0);
        xp=x;  yp=y; phi=phi1;
 fprintf(file,"\n Круговая: L=%9.3lf,R=%10.3lf, xk=%12.3lf,yk=%12.3lf,угол в конце=%9.6lf",
                     elts[i].l_circ,elts[i].rad,x,y,phi1);

 if(!sig0)
 fprintf(file,"\n поворот влево на %3dгр.%2d'%6.2lf\"",gr0,mi0,se0); else
 fprintf(file,"\n поворот вправо на %3dгр.%2d'%6.2lf\"",gr0,mi0,se0);
  fprintf(file,"\n===================================================================================");
          }// круговая
          else
          {//Прямая
          sig=0.0;
    xp=xp+elts[i].l_circ*cos(phi);
   yp=yp+elts[i].l_circ*sin(phi);
  fprintf(file,"\n Прямая: L=%9.3lf, xk=%12.3lf,yk=%12.3lf",elts[i].l_circ,xp,yp);
  fprintf(file,"\n===================================================================================");

            }

      }//счёт по элементам
//verchiny yglov
 fprintf (file,"\n\n                     ВЕРШИНЫ  УГЛОВ ПОВОРОТА");
//sleva xc,yc,phi,sprava xp,yp,phi1,verhina x,y nomer her nt
FILE* fp1;
 fp1=fopen("BY.txt","wt");

      xc =xlpr+ l_first*cos(uglpr);    //pr
      yc =ylpr+ l_first*sin(uglpr);//pr
      phi = uglpr;//pr
      nt=0;
fprintf(fp1,"\n xc=%9.4lf, yc=%9.4lf,l_first=%9.4lf, phi=%9.6lf",
xc,yc,l_first,phi);
    //xc,yc,phi-точка и угол слева по которым строим ВУ
    //xp,yp,gam- в начале очередной окружности или прямой при повторении цикла и на входе

    sig = 0;/*krivizna sleva*/

    if( elts[0].l_clot > 0.0012 )
     {
	 k = 1.0/elts[0].rad/elts[0].l_clot;
          conl(xc,yc,phi,sig, k,elts[0].l_clot,&xp,&yp,&phi1,&sig1);
          gam=phi1; sig=sig1;

      }/*uchli klotoidy sleva*/
   else {xp=xc; yp=yc;gam=phi;}//was mistake early

fprintf(fp1,"\n xp=%9.4lf,yp=%9.4lf,gam=%9.6lf",xp,yp,gam);

// первую клотоиду взяли имеем xp,yp,gam,sig их же надо иметь при возврате в цикле

    for( i=1;i<=n_elts;i++)

   {
     if( (elts[i-1].rad >0.001)||(elts[i-1].rad <-0.001) )
    {
      nt++;
      sig=1/ elts[i-1].rad;
      conla(xp,yp,gam,sig,(double) elts[i-1].l_circ,&x,&y,&phi1);
         xp=x;  yp=y; gam=phi1;
/*vctali v konec okrugnosti*/
fprintf(fp1,"\n в конце окр xp=%9.4lf,yp=%9.4lf, phi1=%9.6lf",xp,yp,phi1);
        if(i==n_elts )
       {/*poslednii radius dalche odna klotoida*/
      prn=1;
       } else
        {

 if((elts[i].rad >0.001)||(elts[i].rad <-0.001))// следующая
           {
             prn=2;
            } else {
              prn=1;
                   }
         } // не последняя
   if( elts[i].l_clot > 0.0012 )
      {// есть клотоида справа

      p=elts[i].l_clot;  //длина правой клотоиды
      if(prn==2)
       {// составная: есть клотоида и окружность справа от нашей окружности
            p=p/2.0;
 k = (1.0/elts[i].rad-sig)/elts[i].l_clot; // sig- кривизна нашей окружности??
       } else
   // не составная значит справа прямая
 k = -sig/elts[i].l_clot;//почему минус??? справа прямая поэтому это k- правой клотоиды
  conl(xp,yp,gam,sig, k,p,&x,&y,&phi1,&sig1);
     xp=x;yp=y;gam=phi1; sig=sig1;
fprintf(fp1,"\n правая кл. xp=%9.4lf,yp=%9.4lf,phi1=%9.6lf",xp,yp,phi1);
    //это конец или середина правой клотоиды для составной
   //если  нет клотоиды  останется конец кривой

      }/*est klotoida sprava*/

      // phi-угол в начале левой клотоиды ( конце прямой)
       //phi1-в конце или середине правой клотоиды или в конце окружности

     if((fabs(fabs(phi)-PI/2.0)<0.0001) ||
     (fabs(fabs(phi)-PI*3/2)<0.0001))
       {x=xc; y=yp+tan(phi1)*(x-xp);
        } else
  if((fabs(fabs(phi1)-PI/2.0)<0.0001) ||
     (fabs(fabs(phi1)-PI*3/2)<0.0001))
       { x=xp; y=yc+tan(phi)*(x-xc);
        }
     else
     {
        if(fabs(phi1-phi)<1.e-10)
          { ShowMessage("Ошибка! Нулевой угол.");
              exit(0);
           }
        if(fabs(fabs(phi1-phi)-PI)<1.e-10)
           {
             ShowMessage("Угол =180 градусов. Разбить кривую!");
             exit(0);
           }

     // точка пересечения касательных, то есть ВУ
       x=(yp-yc+tan(phi)*xc-tan(phi1)*xp)/(tan(phi)-tan(phi1));
       y=yc+tan(phi)*(x-xc);
 fprintf(fp1,"\nВУ x=%9.4lf,y=%9.4lf,xc=%9.4lf,yc=%9.4lf, xp=%9.4lf,yp=%9.4lf,phi=%9.6lf,phi1=%9.6lf",
 x,y,xc,yc,xp,yp,phi,phi1);
     }
/*==============*/
 t=phi1-phi;
 if(t>PI) t=t-2.*PI;
    sig0='+'; if(t<0.) {sig0='-'; t=-t;}
    phi0=t*180./PI;
    gr0=phi0;
    phi0=(phi0-gr0)*60.;
    mi0=phi0;
    phi0=(phi0-mi0)*60.;
    se0=phi0;
    if(sig0=='-') gr0=-gr0;
/*+++++++*/

fprintf(file,"\n\n ВУ%d x=%15.6lf, y=%15.6lf, ugol=%9.6lf или %3dгр%2d'%5.2lf\"",
nt,x,y,phi1-phi,gr0,mi0,se0);

      xc=xp;yc=yp;phi=phi1;

         if(prn==2)
          {
           //sostavnaz bez klotoidi vnutri нет это добавка от середины клотоиды
            if( elts[i].l_clot > 0.0012 )
            {
              conl(xp,yp,gam,sig, k,p,&x,&y,&phi1,&sig1);
               xp=x;yp=y;gam=phi1;sig=sig1;
            }
          }
      continue;
    }
           // else // к if окружность НО ВОПРОС к чему else !!!
     // {     //прямая

        if(i==n_elts) break;
   xp=xp+elts[i-1].l_circ*cos(gam);
   yp=yp+elts[i-1].l_circ*sin(gam);

/*gam v konce klotoidi , no  phi vozmogno v seredine*/
   if(elts[i].l_clot>0.0012)
  {    sig=0.0;
     k = 1.0/elts[i].rad/elts[i].l_clot;
     conl(xp,yp,gam,sig, k,elts[i].l_clot,&x,&y,&phi1,&sig1);
     xp=x;yp=y;gam=phi1; sig=sig1;
   }

     //}//прямая//
}/*for*/
fclose(fp1);

fprintf(file,"\n\n");
//end verchiny
    fprintf (file,"                                           Э Л Е М Е Н Т Ы\n\n");

 fputs( "  ====================================================================|\n", file );
 fputs( "  |          Пикетаж                |  Радиус     |   Переходные   |  Начало кривой     | Конец кривой       |\n",file);
 fputs( "  |-------------------------------- |       м         |--------------------------------------------------------------------- ---  |\n", file );
 fputs( "  |    Начало    |    Конец     |                   | левая   | правая  |   X         |     Y       |       X     |      Y       |\n", file );
 fputs( "  ====================================================================|\n", file );
          jnk=0;
   for( i = 1;i<=n_elts;i++)
   {   // тoлько по элементам с окружностми
         if(fabs(elts[i-1].rad)>1.e-15)//не нуль
     {
  if((i==1)||(fabs(elts[i-1].rad)<1.e-15))temp_ch=elts[i-1].cl_ch0;
          else
     /* на полклотоиды
 count_chain( (temp_ch = elts[i-1].cl_ch0),
      (dist = elts[i-1].l_clot/2), &temp_ch);
 if(fabs(elts[i].rad)<1.e-15)//==0
            temp_ch1=elts[i].cl_ch1;
           else
	 count_chain( (temp_ch1 = elts[i-1].cl_ch1),
		      (dist = elts[i-1].l_circ+elts[i].l_clot/2),
		      &temp_ch1);

   */
   //
   temp_ch=elts[i-1].cl_ch0;
  if(fabs(elts[i].rad)<1.e-15)temp_ch1=elts[i].cl_ch1;
           else
  count_chain( (elts[i-1].cl_ch1),( elts[i-1].l_circ+elts[i].l_clot),&temp_ch1);
  sprintf(str,"  |%6ld+%7.3lf|%6ld+%7.3lf| %11.3f|%9.3f|%9.3f|%9.3lf|%9.3lf|%9.3lf|%9.3lf|\n",
         temp_ch.chain,temp_ch.plus,temp_ch1.chain,temp_ch1.plus,elts[i-1].rad,
  elts[i-1].l_clot,elts[i].l_clot,Nach[jnk].x,Nach[jnk].y,Kon[jnk].x,Kon[jnk].y);
  jnk++;
       	 fputs( str, file );
     }  //if
   }           //for i
   fputs( "  =====================================================================\n",file);
   fputs( "  -----------------------------------------------------------------------------------------------------------------------------\n",
	  file );
  delete []Nach; delete []Kon;
//net v dos  p1= pravxy1(x0,y0,phi0); //p1-суммарная длина, в ev_x,ev_y коорд по кривой
//  вычисляет заново углы и по рихтовкам проектные кординаты
 // лучще записать их в фортране
 //здесь начальные координаты по существующей трассе  !!!!
fputs( "                           ПО ТОЧКАМ СЪЁМКИ\n",file);
 fputs( "   =====================================================\n", file );
 fputs( "   |                    | К о о р д и н а т ы       | К о о р д и н а т ы        |                 |\n", file );
 fputs( "   |    Пикетаж       н а т у р н ы е           |   п р о е к т н ы е          | Сдвижки |\n", file );
 fputs( "   |                    |         X                  Y      |         X              Y          |                |\n", file );
 fputs( "   ===================================================== \n", file );
     rixl=0.0; rixp=0.0;//new
    tr[0].dist=0.0;   //float  была

    //xc = xl; yc = yl; ugl1 = ugl ;
    //xcrl=xl; ycrl=yl;
    ugl1=ugl; //правильно так как по земле идём а проектные по рихтовкам через углы
    xc=yc=0.0; //начало учтено при печати добавлением xl,yl
    xcrl=0.0; ycrl=0.0;
// иду по существующей как и ранее а по рихтовкам вычисляю проектные координаты
    double nevx,nevy;
    double kv=0.0,maxr; int numr;
    maxr=fabs(tr[0].shift); numr=0;
   // fp1=fopen("NATURA.txt","wt");
        for (i=0;i<n_pts;i++)
   {
  rix=tr[i].shift;
  kv=kv+rix*rix;
  if(fabs(rix)>maxr){maxr=fabs(rix); numr=i;}
  if(rix>0.0) rixl=rixl+rix; else rixp=rixp-rix;   //левые и правые суммирую
     sn=sn+tr[i].dist;
     xc+= tr[i].dist*cos(ugl1);
     yc+= tr[i].dist*sin(ugl1);
  //xc,yc координаты по существующей xcr,ycr по проектной
   if(i==0)
   { //фактически xcr=xlpr; ycr=ylpr;  при печати добавка xl, yl
    xcr=xlpr-xl;
    ycr=ylpr-yl;
   }
  else
   {
  if(i==n_pts-1)
   { tk=1; phi=0.;}
   else
    {
     tk=(tr[i].dist/tr[i+1].dist);
     phi=tr[i].angle;
     }
     if (phi>=0.) gam=PI/2.-phi/(1+tk);
     else       gam=-(PI/2.+phi/(1+tk));
    /*пока gam угол между направлением на центр кривизны и правой хордой*/
     t=gam; /* дл отл. печати*/
     p=tk;
     gam=ugl1+phi+gam;
     /*теперь gam-угол между направлением к центру кривизны и осью x
           далее учитывается знак угла поворота
           при вычислении координат по рихтовке
      */
     tk=tr[i].shift;
     if(phi<0.)tk=-tk;
   // xcr=xc+tk*cos(gam);  //это проектные будет xcr=xc+shift[i]*cos(beta+ugl)
    // ycr=yc+tk*sin(gam);
     xcr=xc+tr[i].shift*cos(beta[i]+ugl);
     ycr=yc+tr[i].shift*sin(beta[i]+ugl);
     sp=sp+sqrt((xcr-xcrl)*(xcr-xcrl)+(ycr-ycrl)*(ycr-ycrl));
    /*fprintf(f,"\nxcrl=%8.2lf,ycrl=%8.2lf,xcr=%8.2lf,ycr=%8.2lf,sp=%7.2lf",
		 xcrl,ycrl,xcr,ycr,sp);
     fprintf(f,"\nsn=%8.2lf,sp=%8.2lf",sn,sp);
    */
     xcrl=xcr; ycrl=ycr;
   }
/*fprintf(f,"\np=%6.3lf,xc=%6.2lf,xcr=%6.2lf,yc=%6.2lf,ycr=%6.2lf,shift=%6.2f",
p,xc,xcr,yc,ycr,trace_pts_curr->shift);
fprintf(f,"\ntk=%6.3lf,ugl1=%7.4lf,t=%6.3lf,gam=%7.4lf,phi=%7.4lf,cos=%7.4lf,sin=%7.4lf\n",
 tk,ugl1,t,gam,phi,cos(gam),sin(gam));
 */
 sprintf( str,"  |%ld+%6.2lf   |%12.3lf |%12.3lf|%12.3lf |%12.3lf|   %9.3lf|\n",
  tr[i].ch.chain,tr[i].ch.plus,
       xc+xl,yc+yl,xcr+xl,ycr+yl,tr[i].shift);
      fputs( str, file );

 fprintf(vix,"\n%12.3lf;%12.3lf;%12.3lf;%12.3lf;%12.3lf"
  , xc+xl,yc+yl,xcr+xl,ycr+yl,tr[i].shift);
      /*
      if(i<65) {   //NATURA.txt
      fprintf(fp1," x=%9.3lf,y=%9.3lf",xcr+xl,ycr+yl);
      if(!((i+1)%5))  fprintf(fp1,"\n");
                }  else {fclose(fp1); break;}
        */
     ugl1+= tr[i].angle;
 if(i==n_pts-1){nevx=xcr-xc;  nevy=ycr-yc;}
   }  //for
//tk=tr[n_pts-1].shift;// не нужен для невязок нет угла по нормали
 fputs( "   ======================================================", file );
 fclose(vix);
///вычисление длин трассы по хордам и по элементам
      lsum=l_first;
     // xp =xl+ l_first*cos(ugl);
      //yp =yl+ l_first*sin(ugl);
      xp = l_first*cos(uglpr);
      yp = l_first*sin(uglpr);
      phi = uglpr;
   //pr 3 раза считаю что конец проектный и существующий совпадают
      sig = 0;
    for( i=0;i<n_elts;i++)
   {
       if( elts[i].l_clot > 0.0012 )
     {

	if( fabs(elts[i].rad) <1.e-15 )
	 k = -sig/elts[i].l_clot;
	else
	 k = (1.0/elts[i].rad-sig)/elts[i].l_clot;
 conl(xp,yp,phi,sig, k,elts[i].l_clot,&x,&y,&phi1,&sig1);
      lsum+=elts[i].l_clot;
      xp=x;yp=y;phi=phi1;sig=sig1;
      }
       if( fabs(elts[i].rad) <1.e-15  )
        {
          xp=xp+elts[i].l_circ*cos(phi);
          yp=yp+elts[i].l_circ*sin(phi);
           sig=0.0;
        } else
     { sig=1.0/ elts[i].rad;
     conla(xp,yp,phi,sig,(double) elts[i].l_circ,&x,&y,&phi1);
         xp=x;  yp=y; phi=phi1;
      }
        lsum=lsum+ elts[i].l_circ;
    }/*end cicl vse krone last klot*/
    double xpbkl,ypbkl;// без последней клотоиды
    xpbkl=xp;ypbkl=yp;
  // если есть клотоида то по расстоянию от её начала узнаю,
  // есть кусок прямой в конце или нет
   if( elts[n_elts].l_clot>0.0001 )
  {
      k = -sig/elts[n_elts].l_clot;
    conl(xp,yp,phi,sig, k,elts[n_elts].l_clot,&x,&y,&phi1,&sig1);
      xp=x;  yp=y; phi=phi1;
        lsum=lsum+ elts[n_elts].l_clot;
  }/*xp,yp- конец без последней прямой*/

    //       p1=lsum+sqrt((xp-xcr)*(xp-xcr)+(yp-ycr)*(yp-ycr));
         p1=sqrt((xp-xcr)*(xp-xcr)+(yp-ycr)*(yp-ycr));
if((xpbkl-xcr)*(xpbkl-xcr)+(ypbkl-ycr)*(ypbkl-ycr)<
    (xpbkl-xp)*(xpbkl-xp)+(ypbkl-yp)*(ypbkl-yp))p1=0;
    p1=lsum+p1;

////

   Form4->Panel1->Visible=false;
   Form4->Panel2->Visible=false;
   Form4->Panel3->Visible=true;
  //Form4->Tag=1;
   p=fabs(p1-sn); /*p1-была по элементам*/
  // fprintf(file,"\np=%10.5lf,p1=%10.5lf,sn=%10.5lf",p,p1,sn);
  fprintf(file,"\n  Длина существующей трассы=%10.3lfм.\
   \n  Длина проектной трассы по хордам =%10.3lfм.\
   \n  Длина проектной трассы по кривым =%10.3lfм.\
   \n  Длина проектной трассы без последней прямой =%10.3lfм.",
    sn,sp,p1,lsum);

   if(p<1.e-5) t=0.0; else t=sn/p; gr_=t;
   p=p/sn*100;  //относительное изменение длины
//   fprintf(file,"\n2p=%10.5lf",p);
 if(gr_==0)
     fprintf(file,"\n  Относительное изменение длины= 0");
else fprintf(file,"\n  Относительное изменение длины= 1/%ld",gr_);
     fprintf(file,"\n  Невязка в координатах по x=%7.4lfм.,по y=%7.4lfм.",
   //tk*cos(phi),tk*sin(phi)); phi-  по касательной а tk по нормали почти
   nevx,nevy);
   t= rixl+rixp;
  fprintf(file,"\n  Угол в начале =%10.5lf, в конце=%10.5lf,надо %10.5lf",
     uglpr,phi,ugl+phifin);
//phi-чисто по кривым угол в конце трассы,
 //phifin -угол поворота трассы по хордам ( в оптимизаторе всё с нуля!)
 //ugl -начальный угол в исходной системе
  //     fprintf(file,"\n  С учётом добавленных точек:");
     fprintf(file,"\n  Сумма рихтовок влево= %10.4lfм.\
                   \n  Сумма рихтовок вправо=%10.4lfм.\
   \n                  Всего %10.4lfм.\
                   \n  Средняя абс. %12.6lfм\
                   \n  Число точек=%ld\
                   \n  Сумма квадратов=%13.7lf \
                   \n Макс. рихтовка=%9.4lf в точке %d ",
                   rixl,rixp,rixl+rixp,t/n_pts,n_pts,kv,maxr,numr+1);
                  // rixl,rixp,rixl+rixp,t/66,n_pts,kv,maxr,numr+1);
 Form4->Ls->Text=FloatToStr(sn);
 Form4->Lp->Text=FloatToStr(sp);
 Form4->Lm->Text=FloatToStr(p1);
 char strr[10];
 sprintf(strr,"%7.3lf",p);
 Form4->dL->Text=strr;
 sprintf(strr,"%8.6lf",nevx);
 Form4->dx->Text=strr;
 sprintf(strr,"%8.6lf",nevy);
  Form4->dy->Text=strr;
    Form4->Tag=0;
  Form4->ShowModal();
  Form4->Panel1->Visible=true;
  Form4->Panel2->Visible=true;
  Form4->Panel3->Visible=false;
   Form4->Button1->Visible=1;
  Form4->Button1->Caption="Записать";
//  Form4->Tag=0;
//  Form4->Close() ;
   fclose( file );
   if(Form4->ModalResult==mrOk)

   {
     char put[80];
 strcpy(put,"..//AkelPad.exe ");
 strcat(put, print_file_name);
  // Form4->Button1->Caption="Записать";
// WinExec(put,SW_SHOW);
vizov(put);
    }
   return( 0 );
}


////////////


/// старая
          /*
int print_trace_p( void )
{
double PI=3.1415926;
   FILE *file;
 //   FILE *f;
   CHAINAGE temp_ch,temp_ch1;
//ugl,xl,yl-по extern из extern.h uglpr, xlpr,ylpr- проектные там же
   int    prn, i;
   long    gr_, min_;
   double p,t,tk,p1;
   int np,nt,ntk;
   double angle;
   double dist;// вычислется при обращении в программы но не используется
   char print_file_name[80],str[150];
  char vix_file_name[80];
   //конец переменных для вычисления тангенсов
   double sn=0.0,sp=0.0,xcrl,ycrl,xcr,ycr;
//длины по хордам существ. и проектна, проект предыдущие/
//-----------------------------------------/
   double  xc,yc,gam,phi,phi0,phi1,sig,sig1,k,x,y,xp,yp;
   char sig0;
   int gr0,mi0; double se0; // из unit 4 по extern  беру угол ugl
   //для вычисления вершин углов
      double ugl1;
   //char c1,c2;// глобальна
double lsum;
double rix,rixl,rixp;
//-----------------------------------------
   strcpy( print_file_name, elt_name);
   i = strlen(print_file_name) - 3;
   print_file_name[i] = 'P';

   file = fopen( print_file_name, "wt" );
//  в файле *.tra начальный угол или 100
  //начало закомментированого
  //  ввёл при выборе расчёт участка начало сущ и проектное
// if(ugl>10.){sig0=0;gr0=mi0=0;se0=0.0;}
 //else
 //CONV_GRAD(ugl, sig0,gr0,mi0,se0);
 //if(sig0==1) Form4->sign->Text="-";else Form4->sign->Text="+";
 //  Form4->gr->Text= AnsiString(gr0);
 // Form4->min->Text= AnsiString(mi0);
 // Form4->sec->Text= AnsiString(se0);
 //  Form4->X0->Text= AnsiString(xl);
  // Form4->Y0->Text= AnsiString(yl);
   //            Form4->Tag=0;
//Form4->ShowModal();
//if(Form4->ModalResult!=mrOk)
//{   Form4->Tag=0; //Form4->Close();
  // fclose( file ); return(0);
//}

//Form4->Button1->Visible=0;

   ///конец куска давно закомментированного


Form4->Button1->Caption="Посмотреть";
Form4->Caption="Сводные данные. Подробные в файле  уч. p № вар";
      //xl,yl,ugl были в extern при чтении файла*.tra или из формы unit 4
   //здесь начальные координаты должны быть по проектной трассе  !!!!
   //xlpr,ylpr,uplpr-проектное начало всё ввожу при "расчёт участка"
  sprintf( str, "               Участок %s вариант %c%c\n",
         uch_name,print_file_name[i+1],print_file_name[i+2] );
   fputs( str, file );
   fputs( "-------------------------------------------------------------------------\n",
	  file );
           pravxy1( xlpr,ylpr,uglpr);
 fprintf(file,"\n                  Начало участка:     ");
 fprintf(file,"\n x0=%14.5lf,  y0=%14.5lf,  угол= %12.8lf\n",xlpr,ylpr,uglpr);
 fprintf(file,"\n                  Элементы:     \n\n");

   for( i = 1;i<=n_elts+1;i++)
   {
      append_information(i,&p,&np,&t,&nt,&tk,&ntk);
      if( elts[i-1].l_clot > 0.002 )
   {
	 if  (nt>0)
        {
	 sprintf( str,
" L%02d = %10.6lf    НПК%02d =%5ld+%8.4lf   КПК%02d =%5ld+%8.4lf  P%02d=%10.3lf  T%02d=%10.3lf",
  i, elts[i-1].l_clot,
  i, elts[i-1].cl_ch0.chain, elts[i-1].cl_ch0.plus,
  i, elts[i-1].cl_ch1.chain,elts[i-1].cl_ch1.plus,np,p,nt,t);
	} else
	 sprintf( str,
" L%02d = %10.6lf    НПК%02d =%5ld+%8.4lf   КПК%02d =%5ld+%8.4lf  P%02d=%10.3lf",
  i, elts[i-1].l_clot,
  i, elts[i-1].cl_ch0.chain, elts[i-1].cl_ch0.plus,
  i, elts[i-1].cl_ch1.chain,elts[i-1].cl_ch1.plus,np,p);
fputs( str, file );
      if( i > n_elts )	 break;
   if( fabs(elts[i-1].rad)<1.e-15)
      {
	 sprintf( str,
		  " П%02d = %11.6lf\n",
		  i,elts[i-1].l_circ);//trace_elts_curr->l_circ );
	 fputs( str, file );
   fputs( "\n-----------------------------------------------------------------------------------------\n",
	  file );
      }
    }  else// net perehodnix
      {
	 if  (nt>0) sprintf( str,"    T%02d=%10.3lf\n",nt,t);
         if( i > n_elts ){	 fputs( str, file );	 break;}
   if( fabs(elts[i-1].rad)<1.e-15)
      {
	 sprintf( str,
		  "  П%02d = %11.6lf\n",
		  i,elts[i-1].l_circ );
	 fputs( str, file );
   fputs( "\n--------------------------------------------------------------------------------\n",
	  file );
      }
        }//end net perehodnix
   if( fabs(elts[i-1].rad)>1.e-15)
{
   if (elts[i-1].l_clot<0.0012) elts[i-1].l_clot=0; //добавлено
// начало закомментированного на полдлины клотоид

  //    count_chain( (temp_ch = elts[i-1].cl_ch0),
    // (dist = elts[i-1].l_clot/2), &temp_ch);
//sprintf(str," К%02d = %6.2lf    НК%02d =%5ld+%5.2lf",i,elts[i-1].l_circ+
 //0.5*(elts[i-1].l_clot+elts[i].l_clot),i,temp_ch.chain,temp_ch.plus);
  // конец закомментированного ранее

      count_chain(  elts[i-1].cl_ch0,elts[i-1].l_clot, &temp_ch);
sprintf(str," К%02d = %11.6lf    НК%02d =%5ld+%8.4lf",i,elts[i-1].l_circ,
 i,temp_ch.chain,temp_ch.plus);

   fputs( str, file );
  //   на полклотоиды
начало закомм участка
   count_chain( (temp_ch = elts[i-1].cl_ch1),
		      (dist =elts[i-1].l_circ+ elts[i].l_clot/2), &temp_ch);
	 sprintf( str,
		  "    КК%02d =%5ld+%5.2lf   R%02d =%9.2lf\n",
		  i, temp_ch.chain,temp_ch.plus,
		  i, elts[i-1].rad);//trace_elts_curr->rad );
// конец  коммента
       count_chain( elts[i-1].cl_ch1, elts[i-1].l_circ, &temp_ch);
       sprintf( str,
		  "    КК%02d =%5ld+%8.4lf   R%02d =%14.6lf\n",
		  i, temp_ch.chain,temp_ch.plus,
		  i, elts[i-1].rad);
	 fputs( str, file );
         if(elts[i].l_clot<0.0012)elts[i].l_clot=0;//добавлено
angle =(elts[i-1].l_circ+0.5*(elts[i-1].l_clot+elts[i].l_clot))/elts[i-1].rad;
  	 trans_rad_to_gm(angle,&gr_,&min_);
	 sprintf( str,
		  "                 Уг%02d = %4ldгр.%2ld мин.",
		  i, gr_, min_);
	 fputs( str, file );
	 if  (ntk>0)
	 sprintf( str,"                       TK%02d=%10.3lf\n",ntk,tk);
	 else  sprintf( str,"\n");
	 fputs( str, file );
     }// R<>0
   }//for  i в элементах поправил НК и КК не посередине а в начале и в конце
     //чистой круговой

   fputs( "\n-------------------------------------------------------------------------\n\n",
	  file );
//verchiny yglov
 fprintf (file,"\n Вершины углов");
//sleva xc,yc,phi,sprava xp,yp,phi1,verhina x,y nomer her nt
  FILE* fp1;
   fp1=fopen("BY.txt","wt");
      xc =xlpr+ l_first*cos(uglpr);    //pr
      yc =ylpr+ l_first*sin(uglpr);//pr
      phi = uglpr;//pr
      nt=0;
  fprintf(fp1,"\n xc=%9.4lf, yc=%9.4lf,l_first=%9.4lf, phi=%9.6lf",xc,yc,l_first,phi);
    //xc,yc,phi-точка и угол слева по которым строим ВУ
    //xp,yp,gam- в начале очередной окружности или прямой при повторении цикла и на входе
      sig = 0;//krivizna sleva

    if( elts[0].l_clot > 0.0012 )
     {
	 k = 1.0/elts[0].rad/elts[0].l_clot;
          conl(xc,yc,phi,sig, k,elts[0].l_clot,&xp,&yp,&phi1,&sig1);
          gam=phi1; sig=sig1;
      }//uchli klotoidy sleva
   else {xp=xc; yp=yc;gam=phi;}//was mistake early

 fprintf(fp1,"\n xp=%9.4lf,yp=%9.4lf,gam=%9.6lf",xp,yp,gam);

// первую клотоиду взяли имеем xp,yp,gam,sig их же надо иметь при возврате в цикле

    for( i=1;i<=n_elts;i++)

   {
     if( (elts[i-1].rad >0.001)||(elts[i-1].rad <-0.001) )
    {
      nt++;
      sig=1/ elts[i-1].rad;
      conla(xp,yp,gam,sig,(double) elts[i-1].l_circ,&x,&y,&phi1);
         xp=x;  yp=y; gam=phi1;
//vctali v konec okrugnosti/
  fprintf(fp1,"\n в конце окр xp=%9.4lf,yp=%9.4lf, phi1=%9.6lf",xp,yp,phi1);

      if(i==n_elts )
       {//poslednii radius dalche odna klotoida//
      prn=1;
       } else
        {

 if((elts[i].rad >0.001)||(elts[i].rad <-0.001))// следующая
           {
             prn=2;
            } else {
              prn=1;
                   }
         } // не последняя
   if( elts[i].l_clot > 0.0012 )
      {// есть клотоида справа

      p=elts[i].l_clot;  //длина правой клотоиды
      if(prn==2)
       {// составная: есть клотоида и окружность справа от нашей окружности
            p=p/2.0;
 k = (1.0/elts[i].rad-sig)/elts[i].l_clot; // sig- кривизна нашей окружности??
       } else
   // не составная значит справа прямая
 k = -sig/elts[i].l_clot;//почему минус??? справа прямая поэтому это k- правой клотоиды
  conl(xp,yp,gam,sig, k,p,&x,&y,&phi1,&sig1);
     xp=x;yp=y;gam=phi1; sig=sig1;
    //это конец или середина правой клотоиды для составной
   //если  нет клотоиды  останется конец кривой

      }//est klotoida sprava

      // phi-угол в начале левой клотоиды ( конце прямой)
       //phi1-в конце или середине правой клотоиды или в конце окружности

     if((fabs(fabs(phi)-PI/2.0)<0.0001) ||
     (fabs(fabs(phi)-PI*3/2)<0.0001))
       {x=xc; y=yp+tan(phi1)*(x-xp);
        } else
  if((fabs(fabs(phi1)-PI/2.0)<0.0001) ||
     (fabs(fabs(phi1)-PI*3/2)<0.0001))
       { x=xp; y=yc+tan(phi)*(x-xc);
        }
     else
     {
          // точка пересечения касательных, то есть ВУ
       x=(yp-yc+tan(phi)*xc-tan(phi1)*xp)/(tan(phi)-tan(phi1));
       y=yc+tan(phi)*(x-xc);
       }
//==============
 t=phi1-phi;
 if(t>PI) t=t-2.*PI;
    sig0='+'; if(t<0.) {sig0='-'; t=-t;}
    phi0=t*180./PI;
    gr0=phi0;
    phi0=(phi0-gr0)*60.;
    mi0=phi0;
    phi0=(phi0-mi0)*60.;
    se0=phi0;
    if(sig0=='-') gr0=-gr0;
//+++++++

fprintf(file,"\n ВУ%d x=%15.6lf, y=%15.6lf, ugol=%9.6lf или %3dгр%2d'%5.2lf\"",
nt,x,y,phi1-phi,gr0,mi0,se0);

      xc=xp;yc=yp;phi=phi1;

           if(prn==2)
             {
               //sostavnaz bez klotoidi vnutri нет это добавка от середины клотоиды
                  if( elts[i].l_clot > 0.0012 )
                   {
                     conl(xp,yp,gam,sig, k,p,&x,&y,&phi1,&sig1);
                      xp=x;yp=y;gam=phi1;sig=sig1;
                   }
             }
      continue;
    }
           // else // к if окружность НО ВОПРОС к чему else !!!
     // {     //прямая

        if(i==n_elts) break;
   xp=xp+elts[i-1].l_circ*cos(gam);
   yp=yp+elts[i-1].l_circ*sin(gam);
//gam v konce klotoidi , no  phi vozmogno v seredine
   if(elts[i].l_clot>0.0012)
  {
     k = (1.0/elts[i].rad-sig)/elts[i].l_clot;
     conl(xp,yp,gam,sig, k,elts[i].l_clot,&x,&y,&phi1,&sig1);
     xp=x;yp=y;gam=phi1; sig=sig1;
   }

     //}//прямая//
}//for
fprintf(file,"\n");
//end verchiny

   fputs( "=================================================================================\n", file );
   fputs( "|          Пикетаж            |  Радиус  |  Переходные   |Возвышение| Скорости  |\n", file );
   fputs( "|-----------------------------|    м     |-------------  |    мм    |  км/час   |\n", file );
   fputs( "|    Начало    |    Конец     |          | левая | прав. |          |пасс.|груз.|\n", file );
   fputs( "=================================================================================\n", file );

   for( i = 1;i<=n_elts;i++)

   {   // тoлько по элементам с окружностми
         if(fabs(elts[i-1].rad)>1.e-15)//не нуль
     {
//if((i==1)||(trace_elts_curr->prev->rad==0.0))temp_ch=trace_elts_curr->cl_ch0;
//if((i==1)||(trace_elts_curr->prev->rad==0.0))temp_ch=trace_elts_curr->cl_ch0;
if((i==1)||(fabs(elts[i-1].rad)<1.e-15))temp_ch=elts[i-1].cl_ch0;
          else
     // на полклотоиды
     начало комм
 count_chain( (temp_ch = elts[i-1].cl_ch0),
      (dist = elts[i-1].l_clot/2), &temp_ch);
 if(fabs(elts[i].rad)<1.e-15)//==0
            temp_ch1=elts[i].cl_ch1;
           else
	 count_chain( (temp_ch1 = elts[i-1].cl_ch1),
		      (dist = elts[i-1].l_circ+elts[i].l_clot/2),
		      &temp_ch1);

//   конец комм

   temp_ch=elts[i-1].cl_ch0;
  if(fabs(elts[i].rad)<1.e-15)//==0
            temp_ch1=elts[i].cl_ch1;
           else
	 count_chain( (elts[i-1].cl_ch1),
		      ( elts[i-1].l_circ+elts[i].l_clot),
		      &temp_ch1);

   //
 sprintf(str,"|%6ld+%7.3lf|%6ld+%7.3lf| %9.3f|%7.3f|%7.3f|  %6.1f  |%5.1f|%5.1f|\n",
         temp_ch.chain,temp_ch.plus,temp_ch1.chain,temp_ch1.plus,elts[i-1].rad,
  elts[i-1].l_clot,elts[i].l_clot ,elts[i-1].h,
          elts[i-1].vpas,elts[i-1].vgruz);
       	 fputs( str, file );
     }  //if
   }           //for i
   fputs( "=================================================================================\n",file);
   fputs( "-----------------------------------------------------------------------------------\n",
	  file );

//net v dos  p1= pravxy1(x0,y0,phi0); //p1-суммарная длина, в ev_x,ev_y коорд по кривой
//  вычисляет заново углы и по рихтовкам проектные кординаты
 // лучще записать их в фортране
 //здесь начальные координаты по существующей трассе  !!!!
fputs( "                        По элементам\n",file);
 fputs( " ===============================================================================\n", file );
 fputs( " |               | К о о р д и н а т ы      | К о о р д и н а т ы      |        |\n", file );
 fputs( " |    Пикетаж    |    н а т у р н ы е       |   п р о е к т н ы е      | Сдвижки|\n", file );
 fputs( " |               |     X             Y      |     X            Y       |        |\n", file );
 fputs( " ===============================================================================
 =\n", file );
     rixl=0.0; rixp=0.0;//new
    tr[0].dist=0.0;   //float  была

    //xc = xl; yc = yl; ugl1 = ugl ;
    //xcrl=xl; ycrl=yl;
    ugl1=ugl;
    xc=yc=0.0; //начало учтено при печати добавлением xl,yl
    xcrl=0.0; ycrl=0.0;
// иду по существующей как и ранее а по рихтовкам вычисляю проектные координаты
    double nevx,nevy;
    double kv=0.0,maxr; int numr;
    maxr=fabs(tr[0].shift); numr=0;
        for (i=0;i<n_pts;i++)
   {
  rix=tr[i].shift;
  kv=kv+rix*rix;
  if(fabs(rix)>maxr){maxr=fabs(rix); numr=i;}
  if(rix>0.0) rixl=rixl+rix; else rixp=rixp-rix;
     sn=sn+tr[i].dist;
     xc+= tr[i].dist*cos(ugl1);
     yc+= tr[i].dist*sin(ugl1);
   if(i==0)
   { //xcr=xc; ycr=yc;
    xcr=xlpr-xl;
    ycr=ylpr-yl;
   }
  else
   {
  if(i==n_pts-1)
   { tk=1; phi=0.;}
   else
    {
     tk=(tr[i].dist/tr[i+1].dist);
     phi=tr[i].angle;
     }
     if (phi>=0.) gam=PI/2.-phi/(1+tk);
     else       gam=-(PI/2.+phi/(1+tk));
    //пока gam угол между направлением на центр кривизны и правой хордой
     t=gam; // дл отл. печати
     p=tk;
     gam=ugl1+phi+gam;
     //теперь gam-угол между направлением к центру кривизны и осью x
       //    далее учитывается знак угла поворота
         //  при вычислении координат по рихтовке

     tk=tr[i].shift;
     if(phi<0.)tk=-tk;
     xcr=xc+tk*cos(gam);
     ycr=yc+tk*sin(gam);
     sp=sp+sqrt((xcr-xcrl)*(xcr-xcrl)+(ycr-ycrl)*(ycr-ycrl));
    //fprintf(f,"\nxcrl=%8.2lf,ycrl=%8.2lf,xcr=%8.2lf,ycr=%8.2lf,sp=%7.2lf",
		 xcrl,ycrl,xcr,ycr,sp);
     fprintf(f,"\nsn=%8.2lf,sp=%8.2lf",sn,sp);
    //
     xcrl=xcr; ycrl=ycr;
   }
// комм fprintf(f,"\np=%6.3lf,xc=%6.2lf,xcr=%6.2lf,yc=%6.2lf,ycr=%6.2lf,shift=%6.2f",
p,xc,xcr,yc,ycr,trace_pts_curr->shift);
fprintf(f,"\ntk=%6.3lf,ugl1=%7.4lf,t=%6.3lf,gam=%7.4lf,phi=%7.4lf,cos=%7.4lf,sin=%7.4lf\n",
 tk,ugl1,t,gam,phi,cos(gam),sin(gam));
 комм//
      sprintf( str,
	       " |%7ld+%6.2lf |%12.3lf |%12.3lf|%12.3lf |%12.3lf|%8.3f|\n",
	       tr[i].ch.chain,tr[i].ch.plus,
	       xc+xl,yc+yl,xcr+xl,ycr+yl,tr[i].shift);
      fputs( str, file );
     ugl1+= tr[i].angle;
 if(i==n_pts-1){nevx=xcr-xc;  nevy=ycr-yc;}
   }
//tk=tr[n_pts-1].shift;// не нужен для невязок нет угла по нормали
 fputs( " ================================================================================", file );

///вычисленеие длин трассы по хордам и по элементам
      lsum=l_first;
     // xp =xl+ l_first*cos(ugl);
      //yp =yl+ l_first*sin(ugl);
      xp = l_first*cos(uglpr);
      yp = l_first*sin(uglpr);
      phi = uglpr;
   //pr 3 раза считаю что конец проектный и существующий совпадают
      sig = 0;
    for( i=0;i<n_elts;i++)
   {
       if( elts[i].l_clot > 0.0012 )
     {

	if( fabs(elts[i].rad) <1.e-15 )
	 k = -sig/elts[i].l_clot;
	else
	 k = (1.0/elts[i].rad-sig)/elts[i].l_clot;
 conl(xp,yp,phi,sig, k,elts[i].l_clot,&x,&y,&phi1,&sig1);
      lsum+=elts[i].l_clot;
      xp=x;yp=y;phi=phi1;sig=sig1;
      }
       if( fabs(elts[i].rad) <1.e-15  )
        {
          xp=xp+elts[i].l_circ*cos(phi);
          yp=yp+elts[i].l_circ*sin(phi);
           sig=0.0;
        } else
     { sig=1.0/ elts[i].rad;
     conla(xp,yp,phi,sig,(double) elts[i].l_circ,&x,&y,&phi1);
         xp=x;  yp=y; phi=phi1;
      }
        lsum=lsum+ elts[i].l_circ;
    }/end cicl vse krone last klot
    double xpbkl,ypbkl;// без последней клотоиды
    xpbkl=xp;ypbkl=yp;
  // если есть клотоида то по расстоянию от её начала узнаю,
  // есть кусок прямой в конце или нет
   if( elts[n_elts].l_clot>0.0001 )
  {
      k = -sig/elts[n_elts].l_clot;
    conl(xp,yp,phi,sig, k,elts[n_elts].l_clot,&x,&y,&phi1,&sig1);
      xp=x;  yp=y; phi=phi1;
        lsum=lsum+ elts[n_elts].l_clot;
  }//xp,yp- конец без последней прямой/

    //       p1=lsum+sqrt((xp-xcr)*(xp-xcr)+(yp-ycr)*(yp-ycr));
         p1=sqrt((xp-xcr)*(xp-xcr)+(yp-ycr)*(yp-ycr));
if((xpbkl-xcr)*(xpbkl-xcr)+(ypbkl-ycr)*(ypbkl-ycr)<
    (xpbkl-xp)*(xpbkl-xp)+(ypbkl-yp)*(ypbkl-yp))p1=0;
    p1=lsum+p1;

////
   Form4->Panel1->Visible=false;
   Form4->Panel2->Visible=false;
   Form4->Panel3->Visible=true;
   //Form4->Tag=1;
   p=fabs(p1-sn); //p1-была по элементам
  // fprintf(file,"\np=%10.5lf,p1=%10.5lf,sn=%10.5lf",p,p1,sn);
  fprintf(file,"\n  Длина существующей трассы=%10.3lfм.\
   \n  Длина проектной трассы по хордам =%10.3lfм.\
   \n  Длина проектной трассы по кривым =%10.3lfм.\
   \n  Длина проектной трассы без последней прямой =%10.3lfм.",
    sn,sp,p1,lsum);

   if(p<1.e-5) t=0.0; else t=sn/p; gr_=t;
   p=p/sn*100;  //относительное изменение длины
//   fprintf(file,"\n2p=%10.5lf",p);
 if(gr_==0)
     fprintf(file,"\n  Относительное изменение длины= 0");
else fprintf(file,"\n  Относительное изменение длины= 1/%ld",gr_);
     fprintf(file,"\n  Невязка в координатах по x=%7.4lfм.,по y=%7.4lfм.",
   //tk*cos(phi),tk*sin(phi)); phi-  по касательной а tk по нормали почти
   nevx,nevy);
   t= rixl+rixp;
     fprintf(file,"\n  Угол в начале =%10.5lf, в конце=%10.5lf,надо %10.5lf",uglpr,phi,ugl+phifin);
//     fprintf(file,"\n  С учётом добавленных точек:");
     fprintf(file,"\n  Сумма рихтовок влево= %10.4lfм.\
                   \n  Сумма рихтовок вправо=%10.4lfм.\
   \n                  Всего %10.4lfм.\
                   \n  Средняя абс. %12.6lfм\
                   \n  Число точек=%ld\
                   \n  Сумма квадратов=%13.7lf \
                   \n Макс. рихтовка=%9.4lf в точке %d ",
                   rixl,rixp,rixl+rixp,t/n_pts,n_pts,kv,maxr,numr+1);
 Form4->Ls->Text=FloatToStr(sn);
 Form4->Lp->Text=FloatToStr(sp);
 Form4->Lm->Text=FloatToStr(p1);
 char strr[10];
 sprintf(strr,"%7.3lf",p);
 Form4->dL->Text=strr;
 sprintf(strr,"%8.6lf",nevx);

 Form4->dx->Text=strr;
 sprintf(strr,"%8.6lf",nevy);
Form4->dy->Text=strr;
//Form4->dx->Text=FloatToStr(nevx);
//Form4->dy->Text=FloatToStr(nevy);
  Form4->ShowModal();
  Form4->Panel1->Visible=true;
  Form4->Panel2->Visible=true;
  Form4->Panel3->Visible=false;
  Form4->Button1->Caption="Записать";
  Form4->Button1->Visible=1;

  Form4->Tag=0;
//  Form4->Close() ;
   fclose( file );
   if(Form4->ModalResult==mrOk)

   {
     char put[80];
 strcpy(put,"..//AkelPad.exe ");
 strcat(put, print_file_name);

// WinExec(put,SW_SHOW);
vizov(put);
    }
   return( 0 );
}
*/