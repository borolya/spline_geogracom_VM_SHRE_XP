#include "extern.h"
/*============================================================

  Реконструкция плана трассы.
  Вывод дополнительной информации - тангенсы и прочее,
  требуется только при выводе.
============================================================*/
double mt(double LM, double SM);
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
  //num на 1 больше номера элемента если считать с нуля
 if (num!=1)rad_l  = elts[num-2].rad;    //предыдущий
 else rad_l = 0.;
 if (  num!=n_elts)
     rad_r  = elts[num].rad;// следующий cur->next->rad;
 else rad_r= 0.;
 rad_c  = elts[num-1].rad// текущий cur->rad;

 sig_l. = 0; sig_c = 0.; sig_r=0.;
 if (fabs(rad_l)>0.00001) sig_l = 1./rad_l;
 if (fabs(rad_c)>0.00001) sig_c = 1./rad_c;
 if (fabs(rad_r)>0.00001) sig_r = 1./rad_r;
/*fprintf(fi,"\nsigl=%9.4lf,sigc=%9.4lf,sigr=%9.4lf",sig_l,sig_c,sig_r);*/

// *p = (cur->l_clot)*(cur->l_clot)/24.*(sig_c-sig_l);
 *p = (elts[num-1].l_clot)*(elts[num-1].l_clot)/24.*(sig_c-sig_l);

/*fprintf(fi,"\n*p=%9.5lf,l_clot=%8.3f",*p,cur->l_clot);*/
 *np = num;
 if (fabs(sig_c)<1.e-15)  *ntk=-1;//==0
 else
 {
//   alf = sig_c*((cur->l_clot+cur->next->l_clot)/2. + cur->l_circ);
alf = sig_c*((elts[num-1].l_clot+elts[num].l_clot)/2. +elts[num-1].l_circ);

   *tk = (rad_c+ (*p))*tan(alf/2);
/*fprintf(fi,"\n radc=%9.5lf,*p=%9.5lf,alf=%9.5lf,tk=%9.5lf",rad_c,*p,alf,*tk);*/
   *ntk = num;
   if (sig_l == 0)  tk_first = *tk;
   if (sig_r == 0)
   {
     /* tk_last = *tk;было только это*/
//     p1=(cur->next->l_clot)*(cur->next->l_clot)/24.*sig_c;/*p справа!*/
  p1=(elts[num].l_clot)*(elts[num].l_clot)/24.*sig_c;/*p справа!*/

     /*    fprintf(fi,"\n для tk_last p1=%9.4lf",p1 );*/
    tk_last=(rad_c+ p1)*tan(alf/2);

     //if((sig_l==0) && (sig_r==0))
if((fabs(sig_l)<1.e-15) && (fabs(sig_r)<1.e-15))
      {/*поправки похоже не зависят от знака угла*/
       dt=(*p-p1)/sin(alf);
       tk_first=tk_first-dt;
       tk_last=tk_last+dt;
       *ntk=-1;//было ниже в отдельном операторе
      }
/*fprintf(fi,"\n 2radc=%9.5lf,alf=%9.5lf,tk=%9.5lf",rad_c,alf,tk_last);*/
   }
/*  fprintf(fi,"\n tk=%9.4lf, tk_f =%9.4lf,tk_l =%9.4lf,alf=%9.4lf",
   *tk,tk_first,tk_last,alf);*/
 }
// if (sig_l ==  0 && sig_r == 0) *ntk=-1; перенёс выше в то же условие
 *nt = -1;
 if (fabs(sig_l)<1.e-15)
 {
  *nt =  num;
  *t = tk_first + mt(cur->l_clot,sig_c);
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
return(TRUE);
}
double mt(double LM, double SM)
{  double res;

  res=(((1./17280.*(LM*SM*LM*SM)-1./120.)*(LM*SM*LM*SM))+1)*LM/2.;
  return(res);
}
//#define KRADMIN  10800./PI
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
double dabs(double a)
{
 if  (a<0) return(-a);
 else return(a);
 }











