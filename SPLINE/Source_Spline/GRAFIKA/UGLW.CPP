#include <stdio.h>
#include <process.h>
#include <malloc.h>
#include <conio.h>
#include <math.h>
#include <windows.h>
#include <vcl.h>
#include "Unit5.h"
extern TForm5 *Form5;
//extern char uch_name[80]; этот для записи а для обмена с praktika cxema.dat
extern char igl_name[80];// вместо ugol.ugl
    double s,s0;
extern  struct rp1
  { long pk;
    float pl;
    double km;
  }rp[50];
extern int krp;/*число точек в RP, т.е. номер последней krp-1*/
 double spk=20.0,spp=30.0;//минимальные прямые вставки при стыковке клотоид
 double spmin=20,spmax=180,skmin=20;//по длинам клотоиды и окружности
 double Rmin=200, Rmax=3000.;

  int km_to_pic(double km,long *pic,float *plus);
  void pic_to_km(  long pic, float plus,double *km);
  void pic_to_km(  long pic, float plus,double *km)
  {
  int  i,con,pp;
  long  a,b;
    if ((pic==rp[krp-1].pk) && (fabs(plus-rp[krp-1].pl)<0.001))
     {
       *km=rp[krp-1].km; return;
     }
    pp=0;/*если останется нуль, то не перевели*/
    i=1;
    while (i<krp)
       {
        while (fabs(rp[i].km-rp[i-1].km)<0.001)i++;
          if (i>=krp){
       // ShowMessage("krp=%d,равные km до i=%d",krp,i,1);
    //      ShowMessage("krp=,равные km до i=d");
          //getch();
           exit(0);}
        a=rp[i-1].pk;
        b=rp[i].pk;
        con=1;
        if ((b<a)||((b==a) && (rp[i].pl<rp[i-1].pl)))
            con=-1;
        if ((pic-a)*(b-pic)>=0)
          /* pic  внутри*/
            {
              if (b!=pic)
                   {
                  pp=1; break;
/*pp==1 когда можно считать от левого,если нельзя то на следующую пару*/
                   }
                  else
                    {
                      if (fabs(plus-rp[i].pl)<0.001)
                          {
                             *km=rp[i].km;
                             return;
                           }
                          else
                            if ((plus-rp[i].pl)*con<0)
                            { pp=1; break;}
                    }
              }/*pic внутри*/
      i=i+1;
        } /*   {while}*/
    if(pp==1)
    {
   *km=rp[i-1].km+con*((pic-a)*100+plus-rp[i-1].pl);
    return;
     }
    if (pp==0)
  {
//  ShowMessage ("не преобразован pk=%7ld+%5.3f",pic,plus);
   ShowMessage ("не преобразован");
  // printf("\nNe preobrazovan pk=%7ld+%5.3f",pic,plus);
  //getch();exit(0);
   }
    }

   int km_to_pic(double km,long *pic,float *plus)
{
  int   i,j,k;
   double d;
   if ((km<rp[0].km) || (km>rp[krp-1].km))
   {
//   ShowMessage("  за пределами перегона ");
//    ShowMessage(" %10.2lf за пределами перегона %10.2lf и %10.2lf,ENTER",
//	       km, rp[0].km,rp[krp-1].km,2);

   //  printf("km= %10.2lf,rp[0].km=%10.2lf,rp[krp-1].km=%10.2lf,ENTER",
    //	       km, rp[0].km,rp[krp-1].km);
     //           getch(); exit(0);
    }
   i=0;
   while ((i<krp) && (km>rp[i].km+0.0001))
   i++;/*номер следующей за km границы*/
  if (fabs(km-rp[i].km)<0.0001)
  {
	  *pic=rp[i].pk;
          *plus=rp[i].pl;
      return(0);
  }
      else
    {
          i=i-1;
          j=i;/*левая*/
          if (rp[i].pk>rp[i+1].pk)  j=i+1;/*убывающий пикетаж*/
/*j-меньшая из границ, т.е. правая при убывании*/
/* неправильный пикет в любую сторону начало*/
          if ((labs(rp[i].pk-rp[i+1].pk)==1)
       &&
             (fabs(rp[i].pl+rp[i+1].pl)<0.0001))/*оба плюса нули*/
            {
                   k=1;
                   if (j!=i) k=-1;
                   *pic=rp[j].pk;
		   *plus=(km-rp[j].km)*k;
             }
/* неправильный пикет в любую сторону конец*/

             else
                {
		   d=fabs(km-rp[j].km)+rp[j].pl;
                   i=d/100.0;
                   *pic=rp[j].pk+i;
                   *plus=d-i*100;
                };
  }/* else fabs*/
 return(0);
}


 void okr(unsigned int nk1,unsigned int& kk1);
 void fopr(unsigned int nu,unsigned int ku,unsigned int pr);
 /*участок без кривой и прямой стыкует с предыдущим*/
  FILE* fu,*fp;
  // при повторном вызове глобальная инициализация не работает!!!
  // перенёс в схему Она теперь не MAIN!!!


 int ne=0;/*ne-число участков для записи*/

 struct vix
 { double x,sig; char pr;} v[200];
/*формируемый выход-x,pr и если 2 то кривизна, иначе ноль*/

   long pk; float pl;
   unsigned int n,m,inach,ksm=0,kuch=0,*num,*grl,*grp,nu,ku,i,j,jj,k;
   double *x,*ugl,*ugls,*kriv,*grxl,*grxp,ek,ekl;
/*ksm-число смен знака кривизны,
kuch-число учасков постоянного знака кривизны c длиной больше xpr*/
   double xnach=0.0,x1,x2,krlev,kmin,xpr;

    /*xpr-провал кривизны не образующий элемента,
      x1,x2-левая и правая на участке*/
      unsigned int nk,kk,nkk,nk1,kk1;// начало и конец кривой на участке 0- не нашли

  float fkriv( unsigned int nu,unsigned int ku,unsigned m,double* y,int p)
  {
  //y-текущая кривизна на прямой =0 на окр. постоянна
  // прямая аппроксимирующая график кривизны  даёт к=0 на окр и на прямой
  // если k велик, то это клотоида
//   программа даёт начало прямой или окр.
   double s,s1,sx,sy,sxx,sxy,k,absk;
   nk=0; kmin=10000;
   for (unsigned int i=nu;i<=ku-m+1;i++)
  {sx=sy=sxx=sxy=0.0;
    for (unsigned int j=i;j<=i+m-1;j++)
     {sx=sx+x[j];sy=sy+y[j]; sxx=sxx+x[j]*x[j];sxy=sxy+x[j]*y[j];
     }//for j
   /*if(i==nu)k=(sxy-y[i]*sx)/(sxx-x[i]*sx);
     else*/ k=(sx*sy-m*sxy)/(sx*sx-m*sxx);
      fprintf(fp,"\nfkriv m=%d,i=%d,k=%10.7f,ekl=%10.7f,p=%d",m,i,k,ekl,p);
      absk=fabs(k);
      s=x[i+m-1]-x[i];s1=s; if (s>spmax)s1=spmax;
    // if(absk*s<=ekl*s1)
      if(absk<=ekl)
      {nk=i;
      //if (nk>nu+3)nk--;//
       return k;}
      else
      { if(absk<kmin) {nkk=i;kmin=absk;}
      }
      if(p) return kmin;
   }//i
      return kmin;
 //при нулевом р даёт k (угловой коэфф графика кривизны)
 // и nk - номер точки начиная с которой k <  допуска т.е. начало кривой или прямой
 // если такой точки нет, то kmin и nkk-начало уч- ка a при р=1
 //вызывается с увеличением m пока не получим nk=0 -это конец горизонт. уч-ка
     }//fkriv


    void fkrivold(unsigned int nu,unsigned int ku)
    {   int pr=0; double klev,kt;
         nk= 0;
      for (unsigned int i=nu;i<ku;i++)
    {
       kt=fabs(ugl[i])/(x[i]-x[i-1]);
       if(kt<ek)
       {if(nk) {kk=i-1; break;}else {pr=0; continue;}}
        if(!pr) {pr=1;klev=kt; continue;} else
       {
         if(fabs(kt-klev)<ekl*(x[i]-x[i-1]))
           {if(!nk)nk=i-1; continue;} else
           if(nk) {kk=i-1; break;} else {klev=kt;  continue;}
       }

      }//for
      }//fkrivold даёт начало nk и конец kk 1=ой кривой на участке nk=0-нет кpивой

    void fprym (unsigned int nu,unsigned int ku)
     {  double kt;
       nk= 0; kk=ku;
      for (unsigned int i=nu;i<ku;i++)
     {
       kt=fabs(kriv[i]);
       if(kt>ekl*(x[i]-x[i-1]))
       { if(nk) {kk=i; if(kk-nk>4)kk--;// во избежание длинных прямых
                break;}//большая кривизна после обнаружения прямой== конец прямой 4 точки????
        else  continue;
       }
        if(!nk) nk=i;  //малая кривизна== начало

      }//for

     }//fprym  дает начало nk  и конец kk первой прямой на участке иначе nk=0

 void fopr(unsigned int nu,unsigned int ku,unsigned int pr)
 {  //добавляет клотоиду и cтыкует с предыдущим участком
   int j;
     s=(ugls[ku]-ugls[nu])/(x[ku]-x[nu]);//кривизна на участке
fprintf(fp,"\nfopr клотоида ,nu=%d,ku=%d,nk=%d,nkk=%d,kmin=%10.7lf,s=%9.6lf",
                  nu,ku,nk,nkk,kmin,s);
  fprintf(fp,"\n fopr  длина =%7.2f,spmax=%7.2f,кривизна s=%9.6lf",
   x[ku]-x[nu],spmax,s);

     // if(x[ku]-x[nu]<=spmax)
      //  {//короткий без горизонтали надо клотоиду какую? если длинный исправить!
 fprintf(fp,"\nfopr,pr слева=%d,ne=%d,v[ne-1].x=%7.2lf",v[ne-1].pr,ne,v[ne-1].x);
      switch (v[ne-1].pr)
        {
       case 0:{v[ne].x=x[ku]; v[ne].pr=1;v[ne].sig=s; ne++;
          fprintf(fp,"\nдоб клот  после прямой ne=%d,x=%7.2lf",ne,v[ne-1].x);
               break;}
       case 1:
           {x1=(kriv[nu-1]+kriv[nu]+kriv[nu+1])/3.0;
               if(fabs(x1)>ek)j=1;else j=0;
    //1-стыковка с добавлением  кривой 0-  прямой
                if(v[ne-1].sig*s>0.0)//один знак кривизны клотоид
              {
              fprintf(fp,"\n2  клотоиды встык одного знака s=%9.6lf",s);
                  if(x[ku]-v[ne-2].x<spmax)
                  {
// ошибка объединять можно только при одном знаке и кривизны и параметра k
                  fprintf(fp,"\nобъединил x[ku]=%7.2lf",x[ku]);
                  v[ne-1].x=x[ku]; break;}
                  else//большая общая длина поэтому нужна вставка
                  {
                  fprintf (fp,"\nНужна вставка j=%d",j);
                  if(j==1)//добавлю окружность и клотоиду
                    { k=1;
                     while ((x[nu+k]-x[nu]<spk)&&(x[nu+k]+spmin<x[ku]))k++;
                      v[ne].x=x[nu+k];
                      v[ne].pr=2;v[ne].sig=x1; ne++;
                      if(nu+k==ku){ku++;if(pr)nkk++;else nk++;}
                      v[ne].x=x[ku];v[ne].pr=1; v[ne].sig=s;ne++;
                 fprintf(fp,"\nдобавил окр и клотоиду k=%d,xokr=%7.2lf,xkl=%7.2lf",
                        k, v[ne-2].x,v[ne-1].x);
                     } else //добавлю прямую и клотоиду
                     {
                         k=1;
                      while ((x[nu+k]-x[nu]<spp)&&(x[nu+k]+spmin<x[ku]))k++;
                      //spp-мин прямая вставка между клотоидами одного знака spp=50 ??
                      v[ne].x=x[nu+k];
                      v[ne].pr=0;v[ne].sig=0; ne++;
                      if(nu+k==ku){ku++;if(pr)nkk++;else nk++;}
                      v[ne].x=x[ku];v[ne].pr=1; v[ne].sig=s;ne++;
                 fprintf(fp,"\nдобавил прям и клотоиду k=%d,xпря=%7.2lf,xkl=%7.2lf",
                        k, v[ne-2].x,v[ne-1].x);

                      }
                    }//нужна вставка
                    break;
                 }//один знак кривизны на стывовке
                 else //разные знаки
                  {
          fprintf(fp,"\nразные знаки кривизны клотоид при стыковке" );
                    k=1;
                      while ((x[nu+k]-x[nu]<spp)&&(x[nu+k]+spmin<x[ku]))k++;
                      /*
                      v[ne-1].x=x[nu-1];// сдвиг для клотоиды
                      v[ne].x=x[nu];v[ne].pr=1;v[ne].sig=v[ne-1].sig/2.0;
                      ne++;// клотоида уже была к ней добавить прямую и клотоиду
                      */
                      k--;
                      if(x[nu+k]-v[ne-1].x<spp)v[ne-1].x= x[nu-1];
                      v[ne].x=x[nu+k];
                      v[ne].pr=0;v[ne].sig=0; ne++;// прямая
                      if(nu+k==ku){ku++;if(pr)nkk++;else nk++;}
                      v[ne].x=x[ku];v[ne].pr=1; v[ne].sig=s;
                        ne++;
            fprintf(fp,"\nдоб //kл//, прям и клотоиду pr=%d, k=%d,xпря=%7.2lf,xkl=%7.2lf",
                       pr,k, v[ne-2].x,v[ne-1].x);
                      break;
//     ошибка               доб kл, прям и клотоиду pr=0, k=3,xпря=1095.81,xkl=1111.20


                   }  //разные знаки
                }// case 1
             case 2:
                {
                fprintf(fp,"\n fopr слева окружность");
                if(v[ne-1].sig*s>0.0)
                  {
                  v[ne].x=x[ku]; v[ne].pr=1;v[ne].sig=s; ne++;
        fprintf(fp,"\n тот же знак добавил клотоиду ku=%d,x[ku]=%7.2lf",ku,x[ku]);
                  }
                  else
                    { k=1;
                      while ((x[nu+k]-x[nu]<spp)&&(x[nu+k]+spmin<x[ku]))k++;
                      v[ne-1].x=x[nu-1];//подвинул окружность
                      v[ne].x=x[nu];v[ne].pr=1;v[ne].sig=v[ne-1].sig/2.0;
                      ne++;//добавил клотоиду на одном шаге
                      v[ne].x=x[nu+k];
                      v[ne].pr=0;v[ne].sig=0; ne++;//добавил прямую
                      if(nu+k==ku){ku++;
                      if(pr)nkk++;else nk++;//корректировка конца за сдвижку вправо
                                  }
                      v[ne].x=x[ku];v[ne].pr=1; v[ne].sig=s;ne++;
 fprintf(fp,"\nдругой знак  доб kл,прям и клот nu=%d,k=%d,ku=%d,xпр=%7.2lf,xkl=%7.2lf,s=%9.6lf",
 nu,k,ku,x[nu+k],x[ku],s);

                     }
                      break;
                }//case 2
          }//switch

 fprintf(fp,"\n fopr конец ne=%d,nu=%d,ku=%d,x=%7.2lf,pr=%d",
       ne,nu,ku,v[ne-1].x,v[ne-1].pr);
          //}//конец короткий
          //else fprintf(fp,"\n fopr клотоида больше spmax!!!");
      for(int j1=ne-4;j1<ne;j1++)
fprintf(fp,"\n fopr 4 эл-та  j1=%d,v[j1].x=%8.2lf,pr=%d",j1,v[j1].x,v[j1].pr);

 }//fopr

void okr(unsigned int nk1,unsigned int& kk1)
{   //меняет  kk1 при сдвижке вправо
   double s1;
   s1=x[kk1]-x[nk1];
        s=(ugls[kk1]-ugls[nk1])/s1;
fprintf(fp,"\nokr ставлю горизонталь pr=%d,v[ne-1].x=%7.2lf,s=%9.6lf,nk1=%d,kk1=%d,x[nk1]=%7.2lf,x[kk1]=%7.2lf",
     v[ne-1].pr,v[ne-1].x, s,nk1,kk1,x[nk1],x[kk1]);
      //if(s1>Lmax)s1=spmax;
        // if(fabs(s)<ekl*s1/2.0)    //????
       if(fabs(s)<ek)
        {//прямая
              switch (v[ne-1].pr)
              {
           case 0:  v[ne-1].x=x[kk1]; break;
              //      v[ne].x=x[kk1]; break;
           case 1: if((ne>=2)&&(v[ne-2].pr==0))// убрать клотоиду 2 прямых
                   {v[ne-2].x=x[kk1]; ne--;}else {
                     v[ne].x=x[kk1];  v[ne].pr=0; v[ne].sig=0.0; ne++;
                                                 } break;
           case 2: if(fabs(v[ne-1].sig)<0.0003)
                    {v[ne].pr=0; v[ne].sig=0.0; ne++;
                    } else
                    {
                    fprintf(fp,"\nпрямая нужна вставка  nk1=%d",nk1);
                      v[ne-1].x=x[nk1-1];v[ne].x=x[nk1];
                      v[ne].pr=1;v[ne].sig=v[ne-1].sig;
                      ne++;// вставил кл за счёт левой
                      v[ne].x=x[kk1];v[ne].pr=0;v[ne].sig=0.0; ne++;
                    }// вставка сделана за счёт левой

              }//switch
         }//прямая
         else
              {//кривая

  switch (v[ne-1].pr)
    {
       case 0:
          { fprintf(fp,"\nокружность за прямой ne=%d,s=%9.6lf",ne,s);
              if(fabs(s)<0.0003) {v[ne].x=x[kk1]; v[ne].pr=2; v[ne].sig=s; ne++;}
                 else
            {
              if(ne==1)s1=0.0; else s1=v[ne-2].x;
    fprintf(fp,"\ns1=%7.2lf,xpr=%7.2lf,x[kk1]=%7.2lf,v[ne-1].x=%7.2lf",
                 s1,xpr,x[kk1],v[ne-1].x);
              if((v[ne-1].x-s1)> xpr)// слева можно вставить клотоиду
              {v[ne-1].x=x[nk1-1];v[ne].x=x[nk1]; v[ne].pr=1; v[ne].sig=s; ne++;
              v[ne].x=x[kk1];  v[ne].pr=2; v[ne].sig=s; ne++;
              }
              else {if(x[kk1]-v[ne-1].x>xpr)// справа клотоиду
                    { v[ne].x=x[nk1+1]; v[ne].pr=1; v[ne].sig=s; ne++;
                      v[ne].x=x[kk1];  v[ne].pr=2; v[ne].sig=s; ne++;
                    } else {  v[ne].x=x[kk1];  v[ne].pr=2; v[ne].sig=s; ne++;
                            }//оставить окружность
                   }//нельзя слева
            }
                break;
          }
       case 1:
           {v[ne].x=x[kk1]; v[ne].pr=2; v[ne].sig=s; ne++; break;}
       case 2:
  {
      if(s*v[ne-1].sig>0)   // один знак
               {
if(fabs(s-v[ne-1].sig)<0.0001) v[ne-1].x=x[kk1];//общая
                 else// составная со вставкой или без
                 { fprintf(fp,"\n okr составная со вставкой или без");
         if(fabs(s-v[ne-1].sig)<0.0003)
            {v[ne].x=x[kk1]; v[ne].pr=2; v[ne].sig=s; ne++;}//2 без вставки
                   else//нужна вставка  слева мало не бывает
                   {  fprintf(fp,"\nokr нужна вставка  nk1=%d",nk1);
                      v[ne-1].x=x[nk1-1];v[ne].x=x[nk1];
                      v[ne].pr=1;v[ne].sig=s;
                      ne++;// вставка кл за счёт левой
                      v[ne].x=x[kk1];v[ne].pr=2;v[ne].sig=s; ne++;
                    }// вставка сделана за счёт левой
                 }// составная оформлена
               } else  //разные знаки
               { fprintf(fp,"\nokr разные знаки, nk1=%d,ne=%d",nk1,ne);
                 v[ne-1].x=x[nk1-1];v[ne].x=x[nk1];
                 v[ne].pr=1;v[ne].sig=v[ne-1].sig;ne++;// вставка кл за счёт левой
                 if(kk1<nk1+3) kk1=nk1+3;//исправил kk1!!!
                v[ne].x=x[nk1+1];v[ne].pr=0;v[ne].sig=0; ne++;// прямая
                v[ne].x=x[nk1+2];v[ne].pr=1;v[ne].sig=s;ne++;//клотоида
                v[ne].x=kk1; v[ne].pr=2;
                v[ne].sig=(ugls[kk1]-ugls[nk1+2])/(x[kk1]-x[nk1+2]);
                ne++;
                }//разные знаки

        }// //2 окружности
   }//switch


         }//кривая
   }//okr



  void cxema()
 {
     //ShowMessage("Получить схему");


 long pkn; float pln;
 // при повторном вызове глобальная инициализация не работает!!!
 ne=0;
//   Rmax=3000.; spk=20.; spp=30.; spmin=20.; spmax=180.; skmin=20.;

 xnach=0.0;
  ksm=0; kuch=0;// при повторном вызове глобальная инициализация не работает!!!

// SetConsoleOutputCP(1251);
//  system("chcp 1251");
s0=0.0; // используется как рабочая в конке для перевода в пикетаж=-киломеираж
 xpr=spmin+skmin+spmin;//минимальная длина элемента
 ek=1/Rmax;  ekl=1.0/Rmax/spmax*1.5; // min k клотоиды
// ekl=ekl/10.0; //влияет на длину прямой делает меньше
 /* ekl приращение кривизны на 1 метр длины для пологой клотоиды для кривой
  и прямой нужно иметь меньше,
   ek-минимальная кривизна для отличия от прямой*/

//CharToOem(in,out);
 // puts(out); getch();

 fp=fopen("testugl.txt","wt");


 /*for(int i=0;i<krp;i++)
  printf("\nkrp=%d, pk=%6ld,pl=%6.2f,km=%10.2lf",krp,rp[i].pk,rp[i].pl,rp[i].km);
  getch();
   */


   fu=fopen(igl_name,"rt");
   if(fu==NULL){ShowMessage("Нет файла igl_name");  return;}
   fscanf(fu,"%d",&n);
   x= new double[n+1];
 if(x==NULL){ShowMessage("Нет памяти для X");return;}

   ugl=new double[n];
 if(ugl==NULL){ShowMessage("Нет памяти для UGL");return;}
   ugls=new double[n];
 if(ugls==NULL){ShowMessage("Нет памяти для UGLS");return;}
   kriv=new double[n];
 if(kriv==NULL){ShowMessage("Нет памяти для KRIV");return;}

  num=new unsigned int[ n];
  if(num==NULL){ShowMessage("Нет памяти для NUM");return;}
  grl=new unsigned int[n];
   if(grl==NULL){ShowMessage("Нет памяти для grl");return;}
  grp=new unsigned int[n];
     if(grp==NULL){ShowMessage("Нет памяти для grp");return;}
  grxl=new double[n];
     if(grxl==NULL){ShowMessage("Нет памяти для grxl");return;}
  grxp=new double [n];
     if(grxp==NULL){ShowMessage("Нет памяти для grxp");return;}

/*левая и правая границы длинных участков одной кривизны то есть без смены*/
/*num-номера точек с другим знаком угла чем в точке слева
x-хорда,ugl-абсцисса,угол поворота из файла test.ugl */
     fscanf(fu,"%ld%f%lf%lf",&pkn,&pln,&x[0],&ugl[0]);
     n--;
     //пропускаю начало и нумерую с нуля после первой хорды
   for(i=0;i<n;i++)
    {
     fscanf(fu,"%ld%f%lf%lf",&pk,&pl,&x[i],&ugl[i]);
//     fprintf(fp,"\ni=%d,pk=%3ld,pl=%5.2f,x[i]=%8.2lf,ugl[i]=%9.6lf",i,pk,pl,x[i],ugl[i]);
     if(i) {x[i]=x[i-1]+x[i]; ugls[i]=ugls[i-1]+ugl[i];
                      }
     else ugls[0]=ugl[0];
     /*теперь это абсциссы а не хорды ugl-приращения ugls  суммарные*/
    }
    kriv[0]=ugl[0]/(x[1]/2.0);
     for(i=1;i<n;i++)
          {if(i<n-1) kriv[i]=ugl[i]/((x[i+1]-x[i-1])/2.0);
           else kriv[i]=ugl[i]/(x[n-1]-x[n-2]);
           }
    fprintf(fp,"\nначало n=%d,x[n-1]=%8.2lf",n,x[n-1]);
  if(x[n-1]<xpr)
  {ShowMessage(" Длина кривой  мала для расчета");
  fprintf(fp,"\n Длина кривой = %6.2lf мала для расчета",x[n-1]); fclose(fp);return;
  }

  for(i=0;i<n;i++)fprintf(fp,"\ni=%d,kriv[i]= %9.6lf,ugl=%9.6lf,ugls=%9.6lf",
  i,kriv[i],ugl[i],ugls[i]);
            delete ugl;//больше не нужен
   inach=n-1;
   for (i=1;i<n;i++)
    {
                 //   if(fabs(kriv[i])>2.0*ekl*(x[i]-x[i-1]))
    if(fabs(kriv[i])>ekl*(x[i]-x[i-1]))
      {//if(i>5)i--;// во избежание длинной начальной прямой ??? чем плоха она
       xnach=x[i];inach=i; break;
      }
   }//for i
   krlev=kriv[inach];
   fprintf(fp,"\n inach=%d,xnach=%7.2lf,krlev=%10.6lf",inach,xnach,krlev);
/*это конец левой прямой*/
   if(!inach){ShowMessage("Нет начальной прямой");
               fprintf(fp,"Нет начальной прямой"); fclose(fp); exit(0);
               }
   else {v[0].x=xnach;
 v[0].pr=0; v[0].sig=0.0; ne++;
        }
   for (i=inach+1;i<n;i++)
   {
   if( krlev*kriv[i]<0.0)
    { num[ksm]=i;ksm++;krlev=kriv[i];}
   }//for
  fprintf(fp,"\nksm=%d",ksm);
    for(i=0;i<ksm;i++)
    {fprintf(fp,"\n num[%d]=%d,x[num[i]]=%7.2lf",i,num[i],x[num[i]]);}
/*num-номера точек с другим знаком кривизны чем слева*/
/*Обработка смен и формирование очередного участка для поиcка структуры*/
  if(ksm!=0)
  {  num[ksm]=n-1; ksm++;
       for(i=0;i<ksm;i++)
     { if(i==0) x1=xnach; else x1= x[num[i-1]];
        x2=x[num[i]];/*можно уточнить как середину последней хорды*/
        if(x2-x1>xpr)  // можно уложить эл-т на участке до смены знака xpr=60
       { if(i==0) {grl[kuch]=inach;
		    grxl[kuch]=xnach;
		   }
                  else {grl[kuch]=num[i-1];grxl[kuch]=x1;}
	 grp[kuch]=num[i]; grxp[kuch]=x2;kuch++;
         continue;
	}
      }/*for  grl,grp- левая и правая границы в номерах,grxl,grxр в абсциссах */
   }
   else {kuch=1; grl[0]=inach; grp[0]=n-1;grxl[0]=xnach; grxp[0]=x[n-1];}
   fprintf(fp,"\nkuch=%d",kuch);
 //участок позволяет разместить элемент, если смены идут чаще, эти куски пропадают
   for(i=0;i<kuch;i++)
   {fprintf(fp,"\n grl[%d]=%d, grp[%d]=%d,xlev=%7.2lf,xpr=%7.2lf",
     i,grl[i],i,grp[i],grxl[i],grxp[i]);
   }

if(!kuch)
{//много смен но нет участка постоянной кривизны >xpr
 ne=4;
 v[0].x=xnach;
 v[0].pr=0; v[0].sig=0.0;
 v[1].x=v[0].x+40.0; v[1].pr=1; v[1].sig=0.0;
 v[2].x=x[n-1]-40; v[2].pr=2; v[2].sig=ugls[n-1]/x[n-1];
 v[3].x=x[n-1]; v[3].pr=1; v[3].sig=0.0;
     /*Если не определился участок kuch==0 то он длинный */
 }// end участка не было

 else
  {
  if(grl[0]!=inach)
  {
// s=(ugls[grl[0]]-ugls[0])/(grxl[0]-xnach);кривизна от начала до выделенного участка
 //s=(ugls[grl[0]]-ugls[inach])/(grxl[0]-xnach);
  //if (fabs(s)>ek)//не прямая
  s=ugls[grl[0]]-ugls[inach];
  s0= grxl[0]-xnach;
     if( s>ekl*s0*s0/2.0)  //не прямая
     {
 v[1].x=grxl[0]; v[1].pr=1; v[1].sig=s;
      ne=2;
     } else {v[0].x=grxl[0]; v[0].pr=0; v[0].sig=0.0; ne=1;}
   }  //обработали до первого участка  продлил ипрямую или поставил клотоиду
   //else {fprintf(fp,"\n начальная прямая grl[0]=%d",grl[0]);

   fprintf(fp,"\nne=%d,grl[0]=%d",ne,grl[0]);
//++++++++++++++++++++++++++++++++++++++++++++++++++++= начало

   for (i=0;i<kuch;i++)
   // цикл по участкам с обработкой между ними
   // участок одного знака кривизны длиной более 2 мин клот + мин окружн.
  {
      nu=grl[i]; ku=grp[i];

     while (1)
   { //цикл поиcка кривых на участке постоянного знака кривизны
fprintf(fp,"\nначало while i=%d,ne=%d,v[ne-1].x=%7.2lf,pr=%d",i, ne,v[ne-1].x,v[ne-1].pr);
    fkriv(nu,ku,2,kriv,0);  // было 3 вместо 2
  // if(!nk) fkriv(nu,ku,4,kriv,0);
    fprintf(fp,"\n i=%d,grl[i]=%d,grp[i]=%d,nk=%d,nkk=%d,nu=%d,ku=%d",
                   i,grl[i],grp[i],nk,nkk,nu,ku);

  //на участке выделил первую  прямую на графике кривизны (окр или прямая)
      if(!nk)//нет  на участке это криминал только в начале
             // если короткий клотоида иначе две с прямой на nkk
     {
     fprintf(fp,"\nНет горизонтали ,nu=%d,ku=%d,nk=%d,nkk=%d,kmin=%10.7lf",
                  nu,ku,nk,nkk,kmin);
      // if((x[ku]-x[nu]<=xpr)&&(ku-nu<4)){fopr(nu,ku,0);  break;} //добавить клотоиду
      if(x[ku]-x[nu]<=xpr){fopr(nu,ku,0);  break;}  //было spmax а не xpr
       else //длинный участок без горизонтали
       {
//nkk-начало мин k
         fprintf(fp,"\n длинный ne=%d,nu=%d,ku=%d,nkk=%d,kmin=%10.7lf",
                     ne,nu,ku,nkk,kmin);
 k=1;
      while ((x[nu+k]-x[nu]<spmax+spk)&&(x[nu+k]+skmin<x[ku]))k++;
   if(nu+k>ku)k=ku-nu;
fkriv(nu,nu+k,3,kriv,0);
//нашёл nkk-мин k-точку для горизонтали ( окр или прямая)
//от nu до nkk- клотоида далее окружн ( прямая) на 2 шага и повтор
fprintf(fp,"\nНовый nkk nu=%d,ku=%d,k=%d,nkk=%d,kmin=%10.7lf,nk=%d",
              nu,ku,k,nkk,kmin,nk);

 //nu+k- конец этой части уч-ка
 if(nkk==nu)nkk++;
 if(x[nkk]-x[nu]<spmin) nkk++;//во избежание короткой клотоиды
  if(nkk>nu+k-2) nkk=nu+k-2;// запас для стыковки
  fopr(nu,nkk,1); //добавить клотоиду от nu до nkk
  j=nkk+2; okr(nkk,j);
         fprintf(fp,"\n доб. окр.или прямую nu=%d,ku=%d,nkk=%d,j=%d,ne=%d",
                nu,ku,nkk,j,ne);
          //длинный без гориз добавил окружность или прямую с nkk
 if(j==ku) break;// участок кончился
 if(x[ku]-x[j]<xpr)
 {
           v[ne].x=x[ku]; v[ne].pr=1;
           v[ne].sig=(ugls[ku]-ugls[j])/(x[ku]-x[j]);
    fprintf(fp,"\n клотоида до конца ne=%d,v[ne].x=%7.2lf",
                ne,v[ne].x);
                 ne++;
   //длинный без горизонтали но после клот +окр осталось мало и дoб клотоиду
              break;
  }
 else
 {
 //осталось много и повторим
 nu=j;ku=grp[i]; continue;
 }
   }//else длинный без горизонтали
      }     //!nk

         // есть кривая или прямая

        nk1=nk; //запомнили начало кривой
 //+++++++++++++++nk- начало горизонтали
     if(nk!=nu)
        {
 if(x[nk]-x[nu]<spmin){nk++; fopr(nu,nk,0);} else
 if (x[nk]-x[nu]<=spmax) fopr(nu,nk,0);
 else
      {//длинный до горизонтали nk1 -вместо ku
 k=1;
      while ((x[nu+k]-x[nu]<spmax+spk)&&(x[nu+k]+skmin<x[nk1]))k++;
   if(nu+k>nk1)k=nk1-nu;
fkriv(nu,nu+k,3,kriv,0);
//нашёл nkk-мин k-точку для горизонтали ( окр или прямая)
//от nu до nkk- клотоида далее окружн ( прямая) на 2 шага и повтор
fprintf(fp,"\nНовый nkk nu=%d,ku=%d,k=%d,nkk=%d,kmin=%10.7lf,nk=%d",
              nu,ku,k,nkk,kmin,nk);

 //nu+k- конец этой части уч-ка
 if(nkk==nu)nkk++;
 if(x[nkk]-x[nu]<spmin) nkk++;//во избежание короткой клотоиды
  if(nkk>nu+k-2) nkk=nu+k-2;// запас для стыковки
  fopr(nu,nkk,1); //добавить клотоиду от nu до nkk
  j=nkk+2; okr(nkk,j);
         fprintf(fp,"\n доб. окр.или прямую nu=%d,ku=%d,nkk=%d,j=%d,ne=%d",
                nu,ku,nkk,j,ne);
 if(j<nk1)
 // участок до горизотали не кончился
 {
 if(x[nk1]-x[j]<xpr)
 {
           v[ne].x=x[nk1]; v[ne].pr=1;
           v[ne].sig=(ugls[nk1]-ugls[j])/(x[nk1]-x[j]);
    fprintf(fp,"\n клотоида до конца ne=%d,nk1=%d,v[ne].x=%7.2lf",
                ne,nk1,v[ne].x);
                 ne++;
   //длинный без горизонтали но после клот +окр осталось мало и дoб клотоиду
  }
 else
 {
 //осталось много и повторим
 nu=j;ku=grp[i]; continue;
 }
     }// конец участок до горизонтали не кончился
       }   //конец длинный без гориз добавил окружность или прямую с nkk
 // далее окружность или прямая ==горизонталь
 //+++++++++++++===
 //найдём конец горизонтали
  }//  было до прямой что-то (nu!=nk)
        m=3;
         do    //  поиск kk- конца гориз уч-ка на графике кривизны
        {fkriv(nk1,ku,m,kriv,1);
         if(!nk){kk=nk1+m-1; break;} else m++;
        } while(nk1+m-1<=ku);
        if(nk1+m-1>ku)kk=ku;//kk- конец горизонтали
fprintf(fp,"\nпоиск длины m=%d,nu=%d,ku=%d,nk=%d,kk=%d,nk1=%d,ne=%d,x[nu]=%7.2lf,x[nk]=%7.2lf",
            m,nu,ku,nk,kk,nk1,ne,x[nu],x[nk]);
        //  if(kk==nk){fprintf(fp,"\nошибка nk==kk=,%d",nk);fclose(fp);return;}

        okr(nk1,kk);

fprintf(fp,"\nставлю горизонталь okr nk=%d,kk=%d,ne=%d",nk,kk,ne);
                // здесь возможно ставлю прямую после прямой
        if(kk==grp[i]) break;
        if(x[grp[i]]-x[kk]<xpr)//осталось мало на участке клотоида или прямая
         { s=(ugls[grp[i]]-ugls[kk])/(x[grp[i]]-x[kk]);
           if(fabs(s)<ekl*(x[grp[i]]-x[kk])/2.0) //????
              {v[ne].pr=0; v[ne].sig=0.0;}
          else {v[ne].pr=1;v[ne].sig=s;}
        v[ne].x=grxp[i];
   fprintf(fp,"\nмало до конца ne=%d,v[ne].x=%8.2lf,pr=%d",ne,v[ne].x,v[ne].pr);
         ne++; break;
          }
  // осталось много на участке после кривой или прямой
      nu=kk;   ku=grp[i];
      for(int j=0;j<ne;j++)
fprintf(fp,"\n while j=%d,v[j].x=%8.2lf,pr=%d",j,v[j].x,v[j].pr);

       }//while

       //надо обработать до следующего участка если они не встык
       nu=grp[i]; if(i==kuch-1) ku=n-1; else ku=grl[i+1];
       if(nu==ku) continue;
               if(x[ku]-x[nu]<=xpr)
         { //осталось мало на участке клотоида или прямая

           s=(ugls[ku]-ugls[nu])/(x[ku]-x[nu]);
        if(fabs(s)<ekl*(x[ku]-x[nu])/2.0)
        {
          if(v[ne-1].pr==0)v[ne-1].x=x[ku]; else
          {
          v[ne].pr=0; v[ne].sig=0.0;v[ne].x=x[ku];
          ne++;//не было  v[ne].x=x[ku];
          }
        }
        else fopr(nu,ku,0);
         continue;
          } else //осталось много до следующего участка
           {
  fprym(nu,ku);//поиск прямой до начала следующего участка
       if (!nk) {
       fprintf(fp,"\n между уч-ками нет горизонтали nu=%d,ku=%d",nu,ku);
       s=(ugls[ku]-ugls[nu])/(x[ku]-x[nu]);
        if(fabs(s)<ekl*(x[ku]-x[nu])/2.0)  //2
        {
          if(v[ne-1].pr==0) v[ne-1].x=x[ku]; else
         {
         v[ne].pr=0; v[ne].sig=0.0;
        v[ne].x=x[ku];// не было этого почему???
        ne++;
          }
         }
        else fopr(nu,ku,0);

            //если нет прямой до кривой то клотоида
                 } else
          {//есть прямая до следующего участка
  fprintf(fp,"\n между уч-ками есть прямая nk=%d,nk1=%d,kk=%d,kk1=%d,nu=%d,ku=%d",
  nk,nk1,kk,kk1,nu,ku);
  if(nk>nu){fopr(nu,nk,0);

            //сделал клотоиду до начала прямой
            }
            if(v[ne-1].pr==0)v[ne-1].x=x[kk]; else
     {
     if((ne>2) && (v[ne-1].pr==1) && (v[ne-2].pr==0))
      // нельзя ставить прямую после клотоиды если перед той прямая
          {v[ne-2].x=x[kk];ne--;} else
        {
         v[ne].x=x[kk];v[ne].pr=0;v[ne].sig=0.0; ne++;
         }//добавил прямую

      }                                     //if(kk<ku)
     if(x[ku]-x[kk]>spmin)// есть место для клотоиды
     {//ставлю клотоиду после прямой не думая что там на самом деле

      s0=(ugls[ku]-ugls[kk])/(x[ku]-x[kk]);
     if(fabs(s0)/(x[ku]-x[kk])>ekl*(x[ku]-x[kk])/2.0)
     { v[ne].x=x[ku];
       v[ne].pr=1;
       v[ne].sig=s0;
       ne++;
      }  else
        v[ne-1].x=x[ku];//место есть но это не клотоида
      // что такое sig для клотоиды ????
      //ставлю клотоиду если угол поворота больше чем  на этой длине
      // по самой пологой клотоиде  иначе продолжаю прямую
       }
        else
        v[ne-1].x=x[ku];// нет места продлил предыдущую
                 }//конец прямая до кривой
           }//оставалось много до следующего участка

  }//for по участкам
 v[ne-1].pr=0;// даже если и был 0
if(v[ne-2].pr==0){v[ne-2].x=v[ne-1].x; ne--;}

      for(int j=0;j<ne;j++)
fprintf(fp,"\n end j=%d,v[j].x=%8.2lf,pr=%d",j,v[j].x,v[j].pr);

   }// есть участки
  fclose(fu);
 fu=fopen("cxema.dat","wt");
 fprintf(fu,"%d",ne);

// fprintf(fp,"\n pkn=%ld,pln=%7.2f",pkn,pln);
  pic_to_km(pkn,pln,&s0);
// if(v[ne-1].pr==1)v[ne-1].pr=0;//нельзя кончать клотоидой пока так поправил
 for( int i=0;i<ne;i++)
 {s=s0+v[i].x;km_to_pic(s,&pk,&pl);
 if(fabs(pl-100.0)<0.01){pl=0.0;pk=pk+1;}
 fprintf(fu,"\n%10.2lf%3d%11.7lf%6ld%8.2f",
              v[i].x,v[i].pr,v[i].sig,pk,pl);
fprintf(fp,"\n%10.2lf%3d%11.7lf%6ld%8.2f",
              v[i].x,v[i].pr,v[i].sig,pk,pl);
  }//for
//      ShowMessage(AnsiString(uch_name)+" в схемe записан");
   fclose(fp);
   fclose(fu);


  delete x;  delete ugls;delete kriv;
  delete grl; delete grp;delete grxl; delete grxp;delete num;
   //запись файла
  }//main

