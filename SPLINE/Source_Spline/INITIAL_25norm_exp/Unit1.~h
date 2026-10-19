
//---------------------------------------------------------------------------
#include <vcl.h>
#include "Praktika.h"

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#endif
extern  TForm1 *Form1;
extern double maxrix;
double d3,d4; int nprz,nugz;//номер точки и угла на последней нормали
                          //для записи это не центр а оптимум
 int nzapf,nzapmax,nzapmax1;
 //первая и последняя нормаль в которую была запись + copy
    int ipp;//печать
    FILE* f1,*pr1; //отладка  +прямые
 struct INF
{//
int norm,ntoch,nugl; double xp,yp,ugp;//сделать с переменными углами
 double sum;//цена прихода от начала
 double R,L1,L2,Lk,def1,def2,teta,xv,yv;
 double xlev,ylev,xpr,ypr;
 //  xlev,ylev это начало левой клотоиды  a,b
 //на правой (своей) стороне конец xpr,ypr==c,d1 правой клотоиды
 //а начало следующей  клотоиды это сама текущая точка
 //не все нормали нужны,но удобно разбить все  кроме последней кривой
 }*ukinf,*ukinfp; // текущая и последняя
 INF **Line;
  struct ANGLE
{  //изначально известны в этой структуре только ugkas всё остальное
 // заполняется   при наличии связи то есть вписывания кривой
 ANGLE *uk,*ukrev;//на предыдущее состояние;   нельзя в inf
 INF*sv;//в своём угле есть номер  нормали, номер точки на ней
          //вершина , радиус, переходная, угол поворота и
}*ukangle; //нельзя ли вынести *ukrev ??? из списка
struct TOCH
{ //точка на нормали : координаты и массив углов
 double x; double y;
 ANGLE* ukp;  //массив структур углов со связями и прочим 2*nug+1
};
struct NORMAL
{ TOCH*point;  //2*npr+1 0 центр,вверх плюс до npr вниз минус до-npr
 //массив точек их число задаётся пока фиксирую три npr=1
 int pr;//признак блокировки начала элемента. 0 или конец прямой-2
}*masnorm;//

extern double lfirst,lmin,lmax,lkmin,lprmin,Rmin,Rmax,*xt,*yt,*beta,*x;
 extern double xv,yv,teta;
 // xt[n]-последняя здесь n- номер последней точки, а число точек n+1
 //x-массив абсцисс точек съёмки их n+1
 extern double Xmax;//длина трассы
 extern int n,ndef;
// extern float xA; // не нужен начало в нуле плюс dx,dy,dugl
 extern double dx,dy,dugl,L1,L2,Lk;
 double Lmin,Lmax,Tmin,Tmax;//это по элементу
 double pi=3.1415926535897,pi2;//=1.570796325;
 int nach,kon,Nnorm,nver,ntoch,nugl,nverp,ntochp,nuglp,nminotkl;
 //nver,ntoch,nugl-номера нормали, точки на ней и угла в ней
 //центр 0, вверх + , вниз -
 int npr,nug,pov,inach,ikon,nkr;
int ntochmin,ntochmax,nuglmin,nuglmax;
   //int kdop;
//сдвиг правого конца правой нормали при попадании на прямую
 //pov=0-вращаю правую, 1-левую

 double ugb;
 //nach-номер точки на Lmin от начала в нумерации расстояний от начала
 // её коорд xt[nach],yt[nach],угол beta[nach];
 //расстояние от начала x[nach], kon-то же от конца
 double def1,def2,tetamin;
 int pr,icorr;
  //def1,def2- по тангенсам первому и второму норма минус наличная величина
 extern double s,ugl;// extern double &Bc;
 extern char elt_name[80];
 double s1,s2,s3,s4;//рабочие
 double t1,t2,t,Bc,Rmin1,Bcmin,Bcmax,dtek,dtek1,dopt;//for sum_mod1
 // и на второй прямой( вращается)Rmin1-min R с учётом teta
 double xl,yl,xp,yp,ugp;// пересчитывать
 double sum,sumall;
 extern   double dpr,dug;
 //extern int kdL,kdR; extern double dL;   //dL=10.,
extern int kvadrat (double a,double b,double c,double &x1,double& x2);
 int bisclot1(double t,double l,double teta,double&R,double&R1,double &T);

 extern double clox(double k,double l);
 extern double cloy(double k,double l);
 extern double cloxs_1 (double x0,double k,double sig, double phi, double l);
extern double cloys_1 (double y0,double k,double sig, double phi, double l);
// extern int crossokrnew(double ksi,double eta,double psi,double xk,double yk, double r,
  //             double u,double v,double alf,double& cross,int& np);
extern int  conla(double xl,double yl,double phil,double sig,double l,
			 double *xr,double *yr,double *phir);
extern int conl(double ksi,double eta,double phi,double sig,double k,double l,
	  double *ksi1,double *eta1,double *phi1,double *sig1);
extern int crossokrnew(double ksi,double eta,double psi,double xk,double yk, double r,
             double u,double v,double alf,double& cross,int& np);
extern int crossclonew(double ksi,double eta,double phi,double sig,double k,
      double l,double u,double v,double alf,double& cross);
extern  int crossln(double ksi,double eta,double x1,double y1,
                    double u,double v,double alf,double& sh);
 extern  double otpr(double xa,double ya,double xb,double yb,double x,double y)
{
/*возвращает расстояние от точки до прямой >0 если точка слева
 из а в b a,b-точки на прямой,x,y-ищем расстояние от неё
*/
 double d, x1,y1,x2,y2,vpr;
if(fabs(xb-xa)<0.00001)//вертикаль
  {d=xa-x;  if (yb<ya) d=-d; //down
  return d;
  }//
  else
  {
x1=xb-xa; y1=yb-ya;
x2=x-xa; y2=y-ya;//
vpr=x1*y2-x2*y1;
// векторное произведение=absin(alf)=ad; d=vpr/a;
d=vpr/sqrt(x1*x1+y1*y1);
return d;
   }
}//otpr
extern   double rast(double x1,double y1,double x2,double y2)
 {return (sqrt((x1-x2) *(x1-x2)+(y1-y2)*(y1-y2)));}

  void opt_line();
  int  zapline(int npr,int nug);
 void zapis (int npr,int nug);
 double Rixt(int j,double xc,double yc,double R1);
 void pouglam(int n1,int n3);
 int postr(double xl,double yl,double xp,double yp,double xv,double yv,
           double &Bc, int n2,int n3,double du);
 void vse(void);
 void posl(int j2,int j3);
 int perv(void);
 int NNom(double L);
  int proverka( double xv,double yv,double du,int n2,int n3);
 int corrtg(double du,double& a,double& b,int n1,int n3);
 // void zapis (int,int);  повтор
  void udal(void);

  double Rixt(int j,double xc,double yc,double R1)
 {  double a,b,c,d; //sum- global
 sum=0.0;//сумма модулей трёх сдвижек ( вместо критерия)
  a=rast(xt[j],yt[j],xc,yc)-R1;
  b=rast(xt[j-1],yt[j-1],xc,yc)-R1;
  c=rast(xt[j+1],yt[j+1],xc,yc)-R1;
//  fprintf(f1,"\n xtj=%7.2lf,yjt=%7.2lf,rixa=%7.2lf",xt[j],yt[j],a);
//  fprintf(f1,"\n xtj-1=%7.2lf,yjt-1=%7.2lf,rixb=%7.2lf",xt[j-1],yt[j-1],b);
 // fprintf(f1,"\n xtj+1=%7.2lf,yjt+1=%7.2lf,rixcc=%7.2lf",xt[j+1],yt[j+1],c);
   sum=fabs(a)+ fabs(b)+fabs(c);
      d=(a+b+c)/3.0;//средняя сдвижка
 // fprintf(f1,"\nСред.d=%7.2lf,sum=%7.2lf,nver=%d,ntoch=%d,nugl=%d",
   //             d,sum,nver,ntoch,nugl);
 return d;
  }//Rixt
  int zaptek (int pov,double R1,double tetanew,double a,double b,
               double c,double d1,double xv,double yv,double xp,double yp,double ugp);
  int VUxy(double x1,double y1,double x2,double y2,double ug1,double ug2,
          double& xv,double& yv);// teta глобальная
  int NNom(double L)
  { //даёт номер точки в нумерации расстояний от начала первой за L
    for(int i=0;i<n;i++)
   {
   if( x[i]>=L-0.001)return (i+1);
 //x[i]-расст. от начала по ломаной x[0]- длина первой хорды
 // всего их n c 0 по n-1,n-номер последней точки съёмки x[n]-длина
 //трассы. Если накрыла L i-ая хорда, то её конец в i+1-ой точке
    }   return 0;
  }

  int  konlev(int k,int& lev,int& prav)
    { double s2,s3;
       s3=x[k+nach-1];
 //k-номер нормали(точки) в новой нумерации, это конец элемента
  //k+nach в исходной.x[k+nach-1]-расст от начала точки k+nach
 //по s3 ищу возможное его начало: не далее Lmax не ближе Lmin.
 //это же и конец предшествующего элемента!
 if (s3<2.*Lmin) return 1;//плохо близко к концу построенного элемента
 //всего нет даже двух элементов
       s2=s3-Lmax;
       if(s2<Lmin)s2=Lmin;
       lev=NNom(s2)-nach;
       s2=s3-Lmin;
       prav=NNom(s2)-nach;
        return 0;
   }//konlev

 void vse()      
   {
    int i,j2,lev,prav,levnach;
    //пределы для правой lev,prav при каждой левой  nver1
    Form1->PRB->Visible=true;
  Form1->Label2->Visible=true;
  Form1->Repaint();
//  Application->ProcessMessages();
 Form1-> PRB->Min=0;
 Form1-> PRB->Max=Nnorm;
    int nver1;
  //nzapf-первая записанная нормаль
 nzapmax=0;
 // nzapmax1=NNom(Xmax-2*Lmin)-nach; надо по kon
 //if(nzapmax1<nzapf)nzapmax1=nzapf;
//fprintf(f1,"\nnzapf=%d,nzapmax1=%d,kon=%d,n=%d",
  //              nzapf,nzapmax1,kon,n);
              //новое
 nzapmax1=NNom(x[kon+nach-1]-Lmin)-1-nach;//с гарантией на 2Lmin
  //левыйсконец накрывшей хорды, иначе близко к концу трассы

   if(nzapmax1<nzapf)nzapmax1=nzapf;

 fprintf(f1,"\nvse nzapf=%d,nzapmax1=%d,kon=%d,n=%d",
                  nzapf,nzapmax1,kon,n);
 for (nver1=nzapf;nver1<=nzapmax1; nver1++)
  //цикл по левой на всю от первой записанной до  kon-Lmin
 //это не ближе к концу чем  2Lmin
 {  /*
   if(nver1==338)
   {   ukangle= masnorm[348].point[2].ukp[2].uk;
     ukinf =masnorm[348].point[2].ukp[2].sv;
  if(ukangle!=NULL)
 fprintf(f1,"\n348norm=%d,ntoch=%d,nugl=%d,sum=%7.3lf,348sum=%7.3lf",
ukangle->sv->norm,ukangle->sv->ntoch,ukangle->sv->nugl,ukangle->sv->sum,
ukinf->sum);
ukangle= masnorm[258].point[2].ukp[2].uk;
     ukinf =masnorm[258].point[2].ukp[2].sv;
  if(ukangle!=NULL)
 fprintf(f1,"\n258norm=%d,ntoch=%d,nugl=%d,sum=%7.3lf,348sum=%7.3lf",
ukangle->sv->norm,ukangle->sv->ntoch,ukangle->sv->nugl,ukangle->sv->sum,
ukinf->sum);
 //fclose(f1); exit(0);

    } */
   if(masnorm[nver1].point==NULL) continue;
   nzapf=-1; nzapmax=0;
  // первая и последняя записанные правые нормали меняются в  zaptek
  //для каждой левой новый контроль
    s3=x[nver1+nach-1]+Lmin;
  lev=NNom(s3)-nach;
//правая,ибо левая может быть ближе к nver1 чем Lmin
  s3=x[nver1+nach-1]+Lmax;
if(s3>x[kon+nach-1])prav=kon; else
{
 prav= NNom(s3)-nach;  if(prav>kon)  prav=kon;
 }
      // lev и prav не далее kon!!!
   if(!ndef) // нет прямых вообще
  {
  for(nverp=lev;nverp<=prav;nverp++)
  //конец по правой пока %d,так, надо kon
   {  //Здесь continue при отмеченной nverp  с корректировкой prav
 /*if(nver1==297)
   fprintf(f1,"\nvse297nver1=%d, lev=%d,prav=%d",
              nver1,lev,prav);
   */
   posl(nver1,nver1);
        if(nzapf>0 && nverp-nzapmax>50)
      { //запись была но последние 50 правых записи не дали
 fprintf(f1,"\nНет записи 50 для nver1=%d,nverp=%d,lev=%d,prav=%d,nzapmax=%d",
 nver1,nverp,lev,prav,nzapmax);
          break;
       }//if
    }//for nverp  конец по правым при фиксированной левой nver1
       } else  //!ndef
      {//теперь при наличии прямых   ndef !=0 dH>0.001
 //kdop=0; //он не нужен так как при встрече с прямой
       //её проходим или нет всегда  конец поиска по правой
 int j;
    nverp=lev;
   while (nverp<=prav)//+kdop)
   {
   if ( masnorm[nverp].pr)
    {//начало прямой
        //fprintf(f1,"\n vse прям  lev nver1=%d,nverp=%d,pr=%d",
        //nver1, nverp,masnorm[nverp].pr);
       j=masnorm[nverp].pr;//на прямой там конец минус  3?
        nverp=masnorm[nverp].pr; //переход в конец-3
     for (int i1=0;i1<4;i1++)
      { posl(nver1,nver1);
        nverp++;
      }//end for i1
       //за прямой не ищем связи с левой  поэтому break;
      break;//za while
     //прямую не прошли с записью
     }//if начало прямой
    else  { posl(nver1,nver1);nverp++;}
   }// end while
  }//end есть прямые
      Form1->PRB->Position=nver1;
 //if(nver1>nzapmax1-10)
 fprintf(f1,"\n vse nver1=%d, nzapf=%d,nzapmax=%d,lev=%d,prav=%d",
                   nver1,nzapf,nzapmax,lev,prav);

 }//for nver1 левые для каждой левой правые nverp в цикле lev--prav
   //далее просто posl
  fprintf(f1,"\n vse до последнего nzapmax=%d,nzapmax1=%d",
  nzapmax,nzapmax1);
   nverp=n-nach;//последняя нормаль ( точка)
  nzapf=-1;nzapmax=0;//добавил
//  i=konlev(nverp,lev,prav);//концы левой нормали включительно
  //последний элемент начало в инт-ле lev,prav, конец  nverp
  //это без учёта прямой в конце надо через kon
  s3=x[kon+nach-1]-Lmax;
  lev=NNom(s3)-nach;
  prav=kon;
  /*
       if(i)
     { fprintf(f1,"\n vse нет левой для nverp=%d",nverp);
     //Вся длина меньше двух элементов. Так не бывает. Ошибка
    fclose(f1);  exit (0);
     }
   */
    //это для поиска последнего элемента
 //вместо циклов можно только центр при фиксации оставил на ++
  fprintf(f1,"\n для посл. эл-та:lev=%d,prav=%d,nverp=%d",
  lev,prav,nverp);
   //номера для записи  точки на левой  от -npr ->npr
   //номера для записи углы на левой  от -nugl ->nugl
  // maxrix=2.;
          posl(lev,prav);// границы для левой   а nverp правая
      fprintf(f1,"\nafter последняя nverp=%d,nzapmax=%d",nverp,nzapmax);
   if(masnorm[Nnorm-1].point==NULL)
    {fprintf(f1,"\nVSE нет точек на последней нормали");
    ShowMessage ("\nVSE нет точек на последней нормали");
    fclose(f1);  exit (0);
    }
   double s=1.e7;
   nprz=-15;
     for(ntochp=0; ntochp<=2.*npr;ntochp++)
    {
    if (masnorm[Nnorm-1].point[ntochp].ukp==NULL)
              {fprintf(f1,"\nнет ukp ntochp=%d",ntochp);
                continue;
              }//на след точку
      for(nuglp=0; nuglp<=2.*nug;nuglp++)
  {
 if (masnorm[Nnorm-1].point[ntochp].ukp[nuglp].sv==NULL)continue;
 if (masnorm[Nnorm-1].point[ntochp].ukp[nuglp].sv->sum<s)
 { s= masnorm[Nnorm-1].point[ntochp].ukp[nuglp].sv->sum;
   nprz=ntochp; nugz=nuglp; //номер точки и угла на последней нормали с миним суммой
 fprintf(f1,"\n для записи opt s=%8.3lf,nprz=%d,nugz=%d",s,ntochp,nuglp);
  ukinfp=masnorm[Nnorm-1].point[ntochp].ukp[nuglp].sv;
  fprintf(f1,"\nverposl=%d, R=%7.2lf,Lk=%7.2lf,L1=%7.2lf,L2=%7.2lf",
 ukinfp->norm, ukinfp->R,ukinfp->Lk,ukinfp->L1,ukinfp->L2);
  }//if s
      }//nuglp
     }//ntochp
     if(nprz>-1)
     {
  fprintf(f1,"\n vse оптим стоим, точка и угол sopt=%8.3lf,nprz=%d,nugz=%d",
  s,nprz,nugz);
   }
   else { fprintf(f1,"\n vse Ничего не записано");
  fclose(f1);   exit(0);
        }
 }//vse

  int zaptek (int pov,double R1,double tetanew, double a,double b,
      double c,double d1,double xv,double yv,double xp,double yp, double ugp)
   {
   double t; int pp;
    if(!pov)//первая
    {
     if(masnorm[nver].point==NULL)//точек не было
      {masnorm[nver].point= new TOCH[2*npr+1];
    for (int i=0;i<2*npr+1;i++)masnorm[nver].point[i].ukp=NULL;
  // взял память под точки и занулил их указатели на ANGLE
       }
        if( masnorm[nver].point[ntoch+npr].ukp==NULL)
    { masnorm[nver].point[ntoch+npr].ukp=new ANGLE[2*nug+1];
        for (int i=0;i<2*nug+1;i++)
        { masnorm[nver].point[ntoch+npr].ukp[i].sv=NULL;
          masnorm[nver].point[ntoch+npr].ukp[i].uk=NULL;
        } //for
    }//if  .ukp==0
    ukinf= masnorm[nver].point[ntoch+npr].ukp[nug+nugl].sv;
  if( masnorm[nver].point[ntoch+npr].ukp[nug+nugl].sv==NULL)
 ukinf= masnorm[nver].point[ntoch+npr].ukp[nug+nugl].sv=new INF;
 //if( nver==15 && ntoch==-2 && nugl==2 )
   // fprintf(f1,"\nzap_pov0 sum=%7.3lf,R1=%7.2lf",sum,R1);
 ukinf->sum=sum;
 ukinf->xlev=a;ukinf->ylev=b;ukinf->xpr=c;ukinf->ypr=d1;
  ukinf->norm=nver;ukinf->ntoch=ntoch+npr; //нормали записаны в неполной нумер
  ukinf->nugl=nugl+nug;
  // точки и углы записаны в нумерации снизу с нуля. nver,nverp  без nach
  ukinf->xp=xp;ukinf->yp=yp;ukinf->ugp=ugp;
 a=R1; if(tetanew<0.0)a=-a;
    ukinf->R=a;
 ukinf->Lk=Lk;//fabs(tetanew)*R1-(L1+L2)/2.0;//lmin;
 masnorm[nver].point[ntoch+npr].ukp[nug+nugl].uk=NULL;
 // пока приходим только с Lmin
 ukinf->L1=L1;   ukinf->L2=L2;
 ukinf->def1=def1;   ukinf->def2=def2;
 ukinf->teta=tetanew; ukinf-> xv=xv;ukinf->yv=yv;
     /*
 if(nver==11)// && ntoch==0 && nugl==0)

  {fprintf(f1,"\n\nzap pov0 nver=%d,ntoch=%d,nugl=%d,sum=%8.3lf,ugp=%8.6lf,R1=%7.2lf,teta=%8.6lf",
                        nver,ntoch,nugl,sum,ugp,R1,tetanew);
fprintf(f1,"\ndopt=%6.2lf,def1=%6.2lf,def2=%6.2lf,Tmax=%6.2lf,L1=%6.2lf,Lk=%6.2lf\n",
               dopt,def1,def2,Tmax,L1,Lk);
   }     */
if(nzapf<0)nzapf=nver;//первая записанная нормаль
  if(nver>nzapmax) nzapmax=nver;//последняя из записанных
      } //это о первой кривой нет сравнений
     //конец первая !pov
     //последняя
 // пересчёт по номеру с нуля в номер в массиве ntoch+npr
     if(pov==1)//последняя
  {  pp=0;// не надо записывать
    t=masnorm[nver].point[ntoch+npr].ukp[nug+nugl].sv->sum;// на левой
    if(t+sum<sumall){ sumall=t+sum; pp=1;}
    if(!ukinfp)ukinfp=new INF;
     if(pp)
     {
   // ukinfp->normprev=nver;
   // ukinfp->ntoch=npr;//справа т.В ( конец) она центральная
    //ukinfp->ugkas=ugp;
    ukinfp->xlev=a;ukinfp->ylev=b;ukinfp->xpr=c;ukinfp->ypr=d1;
    a=R1;if(tetanew<0.0)a=-a;
    ukinfp->R=a;
    ukinfp->Lk=Lk; // переделать на разные клотоиды
   ukinfp->L1=L1;   ukinfp->L2=L2;
   ukinfp->def1=def1;   ukinfp->def2=def2;
    ukinfp->teta=tetanew; ukinfp-> xv=xv;ukinfp-> yv=yv;
      ukinfp->norm=nverp;ukinfp->xp=xp;ukinfp->yp=yp;ukinfp->ugp=ugp;
 //ukinfp-глобальный указатель на ANGLE это адрес структуры слева эл-та ukp
    ukangle=&masnorm[nver].point[ntoch+npr].ukp[nugl+nug];//.uk;//отдельный
    }// запись

fprintf(f1,"\n\n\nZAPIS C pov=%d, nver=%d,ntoch=%d,nugl=%d,sumall=%7.2lf,t=%7.2lf,sum=%7.2lf\n",
                      pov,nver,ntoch,nugl,sumall,t,sum);
  } //pov==1 последняя
//+++
    if(pov==2)//vse
    {
      pp=0;// не надо записывать ОШИБКА исправлена
     t=masnorm[nver].point[ntoch+npr].ukp[nug+nugl].sv->sum;// на левой

      // далее все адреса не нули, но не определён ukinf->sum см ниже
     if(masnorm[nverp].point==NULL)//точек на правой не было
      {masnorm[nverp].point= new TOCH[2*npr+1];
    for (int i=0;i<2*npr+1;i++)masnorm[nverp].point[i].ukp=NULL;
  // взял память под точки и занулил их указатели на ANGLE
      }
    if( masnorm[nverp].point[ntochp].ukp==NULL)
   {masnorm[nverp].point[ntochp].ukp=new ANGLE[2*nug+1];
      for (int i=0;i<2*nug+1;i++)
      { masnorm[nverp].point[ntochp].ukp[i].sv=NULL;
        masnorm[nverp].point[ntochp].ukp[i].uk=NULL;
      } //for
   }//if  .ukp==0
    ukinf= masnorm[nverp].point[ntochp].ukp[nuglp].sv;
   if( masnorm[nverp].point[ntochp].ukp[nuglp].sv==NULL)
 {
  ukinf= masnorm[nverp].point[ntochp].ukp[nuglp].sv=new INF;pp=1;
  ukinf->sum=t+sum;//суммарно по новому пути
 }
   else
    { //pp останется нуль, если стало хуже и записи не будет

     if(t+sum <ukinf->sum){ukinf->sum=t+sum; pp=1;}
    }

   //запись
 //точку и угол открыть всегда надо написать в первый раз иначе пропустить

    if(pp)
   {
     ukinf->xlev=a;ukinf->ylev=b;ukinf->xpr=c;ukinf->ypr=d1;
    a=R1;if(tetanew<0.0)a=-a;
    ukinf->R=a;
 ukinf->Lk=Lk;//fabs(tetanew)*R1-(L1+L2)/2.0;lmin;
 ukinf->L1=L1;ukinf->L2=L2;ukinf->def1=def1;ukinf->def2=def2;
  ukinf->teta=tetanew; ukinf->xv=xv;ukinf->yv=yv;
  ukinf->norm=nverp;ukinf->ntoch=ntochp;ukinf->nugl=nuglp;
  ukinf->xp=xp;  ukinf->yp=yp;ukinf->ugp=ugp;
 masnorm[nverp].point[ntochp].ukp[nuglp].uk //на правой точки в новой нумерации
     =&masnorm[nver].point[ntoch+npr].ukp[nugl+nug];//.uk;//
  /*
   if(nverp>=26 &&nverp<=91)
     {
  fprintf(f1,"\n nver=%d,nverp=%d,ntoch=%d,nugl=%d,ntochp=%d,nuglp=%d",
      nver,nverp,ntoch,nugl,ntochp,nuglp);
   ukangle= masnorm[nverp].point[ntochp].ukp[nuglp].uk;
  if(ukangle!=NULL)
 fprintf(f1,"\nоткуда norm=%d,ntoch=%d,nugl=%d,sum=%7.3lf,nver=%d,ntoch=%d,nugl=%d,sumall=%7.3lf",
ukangle->sv->norm,ukangle->sv->ntoch,ukangle->sv->nugl,ukangle->sv->sum,
nver,ntoch,nugl,ukinf->sum);
 fclose(f1); exit(0);
      }
  */
   if(nzapf<0)nzapf=nverp;// первая записанная
   if(nverp>nzapmax)nzapmax=nverp;
    // if( nverp==311  )
   //fprintf(f1,"\n zaptek verp=%d,nver=%d,nzapmax=%d ",nverp,nver,nzapmax);
     }//if pp
   } //это о текущей кривой (vse)
   return 0;
 }//zaptek

 int  zapline(int npr,int nug)
 {  //Здесь параметры оптимальной точки не обязательно это центр
 ANGLE *ukt;
int i=1;
if(masnorm[Nnorm-1].point==NULL)
{ShowMessage("zapline NO Line");
 fclose(f1); return 0;
}//NOLINE
 ukt= &masnorm[Nnorm-1].point[npr].ukp[nug]; //n-nach центр
 if(ukt->sv==NULL)
 {fprintf(f1,"\nzapline Нет центра в последней точке");
 ShowMessage("zapline Нет центра в последней точке");
 fclose(f1); return 0;
 }
  fprintf(f1,"\nzapline norm =%d,sum=%7.2lf,teta=%7.5lf,R=%7.2lf,xp=%8.3lf,yp=%8.3lf",
  ukt->sv->norm,ukt->sv->sum,ukt->sv->teta,ukt->sv->R,ukt->sv->xp,ukt->sv->yp);
   fprintf(f1,"\nzapline xv=%8.3lf,yv=%8.3lf,xpr=%8.3lf,ypr=%8.3lf\n",
               ukt->sv->xv,ukt->sv->yv, ukt->sv->xpr,ukt->sv->ypr);
fprintf(f1,"\nzap откуда nver =%d,ntoch=%d,nugl=%d,sum=%9.3lf",
  ukt->uk->sv->norm,ukt->uk->sv->ntoch,ukt->uk->sv->nugl,ukt->uk->sv->sum );
     while (1)
     {
 //ukt адрес структуры в массиве ukp на центральной точке по центральному углу
   /*
    if(ukt->sv->norm==60)
    {
  fprintf(f1,"\nzapline norm =%d,sum=%7.2lf,teta=%7.5lf,tetamin=%7.5lf",
  ukt->sv->norm,ukt->sv->sum,ukt->sv->teta,tetamin);
 fprintf(f1,"\nпрод. xlnew=%7.2lf,ylnew=%7.2lf,xv=%7.2lf,yv=%7.2lf,xpnew=%7.2lf,ypnew=%7.2lf",
  ukt->uk->sv->xp,ukt->uk->sv->yp,ukt->sv->xv,ukt->sv->yv,ukt->sv->xp,ukt->sv->yp);
 fprintf(f1,"\nпрод. ugl=%8.6lf, ugp=%8.6lf,R=%7.2lf",
   ukt->uk->sv->ugp,ukt->sv->ugp,ukt->sv->R);
     fprintf(f1,"\nzapline LEV norm =%d,sum=%7.2lf,ntoch=%d,nugl=%d",
  ukt->sv->norm,ukt->sv->sum,ukt->sv->ntoch,ukt->sv->nugl);
 }
 */
  ukt->uk-> ukrev=ukt; //предыдущему rev присвоил текущий
  ukt=ukt->uk;//перешёл на шаг влево
   i++;
  if (ukt->uk==NULL) break;
     }//while  ukt-правая стор. начального угла ANGLE! не uk
 Line=new INF *[i];  //i-число вершин==прaвых сторон==кривых
 for (int j=0;j<i;j++,ukt=ukt->ukrev)
 Line[j]=ukt->sv;
 //здесь ukt-адрес структуры Angle на первой правой стороне
   /*  ukt->ukrev NULL
  fprintf(f1,"\nzapline TEST  ukt norm =%d,sum=%7.2lf,ntoch=%d,nugl=%d",
  ukt->sv->norm,ukt->sv->sum,ukt->sv->ntoch,ukt->sv->nugl);
     */
   for(int j=0;j<i;j++)
     {
     //if(Line[j]->teta<0.)Line[j]->R=-Line[j]->R;
 fprintf(f1,"\nzapline j=%d,nver=%d,xv=%9.4lf,yv=%9.4lf,R=%9.4lf,L1=%7.2lf,ntoch=%d,nugl=%d",
   j,Line[j]->norm,Line[j]->xv,Line[j]->yv,Line[j]->R,Line[j]->L1,Line[j]->ntoch,Line[j]->nugl);
   fprintf(f1,"\nzap xlev=%8.3lf,ylev=%8.3lf,xpr=%8.3lf,ypr=%8.3lf,teta=%10.6lf\n",
     Line[j]->xlev,Line[j]->ylev,Line[j]->xpr,Line[j]->ypr,Line[j]->teta);
     } // for j

        return i;//число вершин
  }//  zapline
 //+++ R,L1,L2,Lk,def1,def2,teta,xv,yv,xp,yp,ugp;  ukline[0->nkr-1]

  int  indv(double teta,double& s1,double s2,double s3,double s4 )
  {   int ind;
  ind=0;
  s1=s2+s3+s4;// сумма по всем нужна
  if (s2<=0.&&s3<=0.&&s4<=0.)ind=1; //при угол >0 идти от вершины
      if (s2>=0.&&s3>=0.&&s4>=0.)ind=-1;//при угол >0 идти к вершине
      if(teta<0) ind=-ind;//ind для отриц углов поворота; от вершины ind=1
    return ind;
  }//indv

 void zapis (int npr,int nug)
 {  //to fortran  временная с фиксацией длин
 ANGLE *ukt;//,*ukt1 ;
int i=1;
if(masnorm[Nnorm-1].point==NULL)
 {ShowMessage ("ZAPIS Нет ТОЧЕК");};
 ukt= &masnorm[Nnorm-1].point[npr].ukp[nug]; //n-nach
 fprintf(f1,"\nzapis sumall=%7.3lf,npr=%d,nug=%d,i=%d",
  ukt->sv->sum,npr,nug,i);
     while (1)
     {
  //это адрес структуры в массиве углов ukp
  ukt->uk-> ukrev=ukt; //предыдущему rev присвоил текущий
  ukt=ukt->uk;//перешёл на шаг влево
  i++;
  if (ukt->uk==NULL) break;
     }//while  ukt-правая стор. начального угла ANGLE! не uk
      fprintf(f1,"\n zapis i=%d",i);  //i=2,число правых сторон углов,ne==3 //

      /*ukt1=ukt; //nkr=i;

 for(int i=0;i<11;i++,ukt1=ukt1->ukrev)
 {

 fprintf(f1,"\nzapis i=%d,norm=%d,xp=%7.2lf,yp=%7.2lf,ugp=%8.6lf,ntoch=%d,nugl=%d,sum=%7.2lf",
 i,ukt1->sv->norm,ukt1->sv->xp,ukt1->sv->yp,ukt1->sv->ugp,ukt1->sv->ntoch,ukt1->sv->nugl,ukt1->sv->sum);
 fprintf(f1,"\nпрод a=%6.2lf,b=%6.2lf,c=%6.2lf,d=%6.2lf,R=%7.2lf,L1=%6.2lf,L2=%6.2lf",
 ukt1->sv->xlev,ukt1->sv->ylev,ukt1->sv->xpr,ukt1->sv->ypr,ukt1->sv->R,ukt1->sv->L1,ukt1->sv->L2);

  if(ukt1->sv->norm==129)
  {
  fprintf(f1,"\nnver=%d,ntoch=%d,nugl=%d,R=%7.2lf,sum=%7.2lf",
ukt1->sv->norm,ukt1->sv->ntoch,ukt1->sv->nugl,ukt1->sv->R,ukt1 ->sv->sum);
fprintf(f1,"\nnverp=%d,ntochp=%d,nuglp=%d,R105=%7.2lf,sum105=%7.2lf",
ukt1->ukrev->sv->norm,ukt1->ukrev->sv->ntoch,ukt1->ukrev->sv->nugl,ukt1->ukrev->sv->R,
ukt1->ukrev->sv->sum);
  break;
  }//if 129

    }//for i
   */
 ///

 fprintf(f1,"\n1sum=%7.3lf,R=%7.2lf,L1=%7.2lf,L2=%7.2lf",
  ukt->sv->sum,ukt->sv->R,ukt->sv->L1,ukt->sv->L2);
  fprintf(f1,"\n1def1=%7.2lf,def2=%7.2lf",
  ukt->sv->def1,ukt->sv->def2);

 fprintf(f1,"\n2sum=%7.2lf,R=%7.2lf,L1=%7.2lf,L2=%7.2lf",
 ukt->ukrev->sv->sum,ukt->ukrev->sv->R,ukt->ukrev->sv->L1,ukt->ukrev->sv->L2);
 fprintf(f1,"\n2def1=%7.2lf,def2=%7.2lf",
  ukt->ukrev->sv->def1,ukt->ukrev->sv->def2);

  FILE *f; int ne,nvu;
 f=fopen(elt_name,"wt");
 ne= 2*i-1;  nvu=i; //число вершин==числу правых сторон
  fprintf(f,"%5d",ne);
  //число элементов,т.е.NCURV=число строк (клотоида +окр (или прямая))
  //число переменных : длина клотоиды,кривизна, длина окр ( или прямой)+ концевая клотоида
  //на две ВУ переменных 9+1
 s=ukt->sv->xlev;  // это a,b
 s1=ukt->sv->ylev;
 s2=rast(s,s1,dx,dy); //первая прямая lfirst
 fprintf(f1,"\ns=xlev=%7.2lf,s1=ylev=%7.2lf,dx=%7.2lf,dy=%7.2lf,first=%7.2lf",
 s,s1,dx,dy,s2);
  fprintf(f,"\n%12.2lf",s2);
  fprintf(f,"\n%12.2lf%12.2lf%12.2lf",0.0,s2+100.0,0.0);

  for(i=0;i<nvu;i++)
  {  s1=ukt->sv->L1;
 fprintf(f,"\n%12.6lf%12.6lf%12.6lf",s1,lmin,lmax);
 s2= ukt->sv->Lk;
 fprintf(f,"\n%12.6lf %12.6lf   %12.6lf",ukt->sv->R,Rmin,Rmax);
 fprintf(f,"\n%12.6lf%12.6lf%12.6lf", s2,lkmin,s2+100.);
 fprintf(f,"\n %12.6lf",0.0);//возвышение
 s2=ukt->sv->L2;
fprintf(f,"\n%12.6lf%12.6lf%12.6lf",s2,lmin,lmax);
  if(i==nvu-1) break;
  s=ukt->sv->xpr; s1=ukt->sv->ypr; //конец клотоиды c,d1
 fprintf(f1,"\ni=%d,def1=%7.3lf,def2=%7.3lf",
    i,ukt->sv->def1,ukt->sv->def2);
  ukt=ukt->ukrev;
  s2=rast(s,s1, ukt->sv->xlev,ukt->sv->ylev); //прямая
 fprintf(f1,"\nxpr=%7.2lf,ypr=%7.2lf,x+=%7.2lf,y+=%7.2lf,вставка s2=%7.2lf",
                 s,s1,ukt->sv->xlev,ukt->sv->ylev,s2);
   fprintf(f1,"\nR+=%12.6lf%12.6lf  %12.6lf",ukt->sv->R,Rmin,Rmax);
  fprintf(f,"\n%12.6lf%12.6lf  %12.6lf",0.0,Rmin,Rmax);
  fprintf(f,"\n%12.6lf%12.6lf%12.6lf", s2,lprmin,s2+80. );
  fprintf(f,"\n %12.6lf",0.0);//возвышение

   }//for
 fclose(f);
  }//zapis
  ////
   void zapis_GOOD (void)   // no use
 {  //to fortran
 ANGLE *ukt;
int i=1;
if(masnorm[Nnorm-1].point==NULL)
 {ShowMessage ("ZAPIS Нет ЦЕНТРА");
 fclose(f1);}
 ukt= &masnorm[Nnorm-1].point[npr].ukp[nug]; //n-nach центр
    fprintf(f1,"\nzapis sumall=%7.2lf", ukt->sv->sum);
     while (1)
     {
  //это адрес структуры в массиве углов ukp
  ukt->uk-> ukrev=ukt; //предыдущему rev присвоил текущий
  ukt=ukt->uk;//перешёл на шаг влево
  i++;
  if (ukt->uk==NULL) break;
     }//while  ukt-правая стор. начального угла ANGLE! не uk

      fprintf(f1,"\n zapis i=%d",i);   //i=2,число правых сторон углов,ne==3
  fprintf(f1,"\n1sum=%7.2lf,R=%7.2lf,L1=%7.2lf,L2=%7.2lf",
ukt->sv->sum,ukt->sv->R,ukt->sv->L1,ukt->sv->L2);
fprintf(f1,"\n xv=%9.4lf,yv=%9.4lf",ukt->sv->xv,ukt->sv->yv);
  fprintf(f1,"\n1def1=%7.2lf,def2=%7.2lf",
  ukt->sv->def1,ukt->sv->def2);
 fprintf(f1,"\n2sum=%7.2lf,R=%7.2lf,L1=%7.2lf,L2=%7.2lf",
 ukt->ukrev->sv->sum,ukt->ukrev->sv->R,ukt->ukrev->sv->L1,ukt->ukrev->sv->L2);
 fprintf(f1,"\n2def1=%7.2lf,def2=%7.2lf",
  ukt->ukrev->sv->def1,ukt->ukrev->sv->def2);

 FILE *f; int ne,nvu;
f=fopen(elt_name,"wt");
//f=fopen("1zap.txt","wt");
ne= 2*i-1;  nvu=i; //число вершин==числу правых сторон
  fprintf(f,"%5d",ne);
  //число элементов,т.е.NCURV=число строк (клотоида +окр (или прямая))
  //число переменных : длина клотоиды,кривизна, длина окр ( или прямой)+ концевая клотоида
  //на две ВУ переменных 9+1
 s=ukt->sv->xlev;  // это a,b
 s1=ukt->sv->ylev;
 s2=rast(s,s1,dx,dy); //первая прямая lfirst
 fprintf(f1,"\ns=xlev=%7.2lf,s1=ylev=%7.2lf,dx=%7.2lf,dy=%7.2lf,first=%7.2lf",
 s,s1,dx,dy,s2);
  fprintf(f,"\n%12.2lf",s2);
  fprintf(f,"\n%12.2lf%12.2lf%12.2lf",0.0,s2+100.0,0.0);

  for(i=0;i<nvu;i++)
  {
 fprintf(f,"\n%12.6lf%12.6lf%12.6lf",ukt->sv->L1,lmin,lmax);
 fprintf(f,"\n%12.6lf %12.6lf   %12.6lf",ukt->sv->R,Rmin,Rmax);
 fprintf(f,"\n%12.6lf%12.6lf%12.6lf", ukt->sv->Lk,lkmin,3000.0);
 fprintf(f,"\n %12.6lf",0.0);//возвышение
fprintf(f,"\n%12.6lf%12.6lf%12.6lf",ukt->sv->L2,lmin,lmax);
  if(i==nvu-1) break;
  s=ukt->sv->xpr; s1=ukt->sv->ypr; //конец клотоиды c,d1
  ukt=ukt->ukrev;
  s2=rast(s,s1, ukt->sv->xlev,ukt->sv->ylev); //прямая
 /*
 fprintf(f1,"\nxpr=%7.2lf,ypr=%7.2lf,x+=%7.2lf,y+=%7.2lf,вставка s2=%7.2lf",
                 s,s1,ukt->sv->xlev,ukt->sv->ylev,s2);
 fprintf(f1,"\n%12.6lf%12.6lf  %12.6lf",ukt->sv->R,Rmin,Rmax);
   */
  fprintf(f,"\n%12.6lf%12.6lf  %12.6lf",0.0,Rmin,Rmax);
  fprintf(f,"\n%12.6lf%12.6lf%12.6lf", s2,lprmin,100.0);
  fprintf(f,"\n %12.6lf",0.0);//возвышение
   }//for
 fclose(f);
  }//zapis

  void udal(void)
  {

  for(int i=0;i<Nnorm;i++)
   {
   if(masnorm[i].point!=NULL )//есть точки на нормали i
    { for(int j=0;j<2*npr+1;j++)
     {
     if(masnorm[i].point[j].ukp!=NULL)
       {  for(int k=0;k<2*nug+1;k++) //есть углы в точке j
         {
     //  delete masnorm[i].point[j].ukp[k].uk;
    //   delete masnorm[i].point[j].ukp[k].ukrev;
       delete masnorm[i].point[j].ukp[k].sv;
          }//for k
        }//if есть массив углов в  точке j на нормали i
         //{ delete masnorm[i].point;//end нет углов
         delete [] masnorm[i].point[j].ukp;
      }//for j
     } //if
     delete [] masnorm[i].point;
   }//for i
    delete []masnorm;
    //delete []ukline;
  }//udal
//++++++++++++++++++
int bistg(double t,double T,double teta,double& R1,double& L1,double& R,double& L)
  { double a,b,c,sint,cost; int j;
  sint=sin(fabs(teta/2.)); cost=cos(teta/2.);
     a=7.0-cost*(cost+6.0); //t-биссектриса
     b=2.*(sint*(t*sint-T)-3.0*t*cost);
     c=(T-t*sint)*(T-t*sint);
   j=kvadrat(a,b,c,R,R1);
 //fprintf(f1,"\nbistg j=%d,a=%9.2lf,b=%9.2lf,c=%9.2lf,R=%9.2lf,R1=%9.2lf",
   //           j,a,b,c,R,R1);
   if(!j){ShowMessage(" bistg Нет корня");}else
  L=2.0*(T-(t+R)*sint); L1=2.0*(T-(t+R1)*sint);
// fprintf(f1,"\n\nR=%7.2lf,L=%7.2lf,R1=%9.2lf,L1=%7.2lf,Bc=t=%7.2lf,T=%7.2lf",
  //              R,L,R1,L1,t,T); ралиусы и клотоиды по тангенсу и биссектрисе
     return j;
   }
  int tgclot(double Tmax,double l,double& t,double teta,double&R,double&R1)
  { int j; double a,b,c;
    b=(Tmax-l/2.0)*cos(teta/2.)/sin(fabs(teta/2.));
     //R1 по тангенсу и клотоиде  не меняя угол
   a=1.0; b=-b; c=l*l/24.0;
   j=kvadrat(a,b,c,R,R1); //R1-с плюсом (максимальный)
   if(!j)    {ShowMessage("\n tgclot нет корней "); }
 //  t= (R1+c/R1)*tan(teta/2.)+l/2.0;//new T==Tmax
    t= (R1+c/R1)/cos(teta/2.)-R1;//new Bc при Tmax R1 при Tmax
    return j;
  }//tgclot   на выходе радиусы по max тангенсу и клотоиде  и биссектриса t

   int bisclot(double t,double lmin,double teta,double&R,double&R1)
  { int j;double a,b,c;
     //t-биссектриса;  на выходе радиусы по клотоиде
   a=24.0*(1-cos(teta/2.));
   b=-24.0*t*cos(teta/2.);c=lmin*lmin;
   j=kvadrat(a,b,c,R,R1);
            if(!j)ShowMessage("\nbisclot нет корней");
     return j;
   }//bisclot

  int bisclot1(double t,double l,double teta,double&R,double&R1,double &T)
  { int j;double a,b,c,p;
     //t-биссектриса;  на выходе радиусы по клотоиде и тангенс T
   a=24.0*(1-cos(teta/2.));
   b=-24.0*t*cos(teta/2.);c=l*l;
   j=kvadrat(a,b,c,R,R1);
    p=c/24./R1;
    T= (R1+p)*tan(fabs(teta/2.))+l/2.;
   if(!j)ShowMessage("\nbisclot1 нет корней");
     return j;
   }//bisclot1

    double ugbis(double uglnew,double tetanew)
  { double ugb;
     ugb=uglnew+tetanew/2.0;//новый угол teta/2
     if (tetanew>0)ugb=ugb+pi2;else ugb=ugb-pi2;
     return  ugb;
   }//ugbis

 double bis(int n2,int n3,double xv,double yv,int &nminotkl)
 {   double Bc,s;
   Bc=1.e7;
  for( int i=n2;i<n3;i++) //n2,n3 пределы поиска мин отклонения от вершины
    {
    s= rast(xt[i],yt[i],xv,yv);
   if(s<Bc){Bc=s;nminotkl=i;}
//это не биссектриса так как не по центру
    }//for
    return Bc;
  }//bis

  double ugol(double x1,double y1,double x2,double y2)
  { double s,s1,pi;//из 1->2
     pi=3.141592652;
     s=(y2-y1)/sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));//s>0 при y2>y1
     s1=asin(s); //s>0 при y2>y1
    if(x2>=x1)return s1; else if (s>0.0) return (pi-s1);
    else return (-(s1+pi));//верно!
    }//ugol ==atan(y/x)2??

