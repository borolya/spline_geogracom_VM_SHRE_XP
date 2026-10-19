extern  struct rp1
  { long pk;
    float pl;
    double km;
  }rp[50];
extern int krp;/*число точек в RP, т.е. номер последней krp-1*/
 extern double Rmin, Rmax,lmin,lmax,lkmin,lprmin;
 extern double *xt,*yt,*beta;
  double *x,*ugl,*ugls,*kriv;
extern int colt,ne;
extern double dx,dy,dugl;
     FILE* fu,*fp,*f;
     const double pi=3.141592652;


  int kvadrat (double a,double b,double c,double &x1 ,double& x2);
     void ABCTmax(double teta,double Bc,double Tmax,double&a,double&b,double&c);
 double rast(double x1,double y1,double x2,double y2);
int rashtg(int n2,int n3 ,double xv,double yv,
  double tetaish,double&Rabs,double&Bc, double& L1,double& T);
int corrtg (double teta,double &Rabs,double &L1,double &Tmax,double &Bc);
  double clox(double k,double l)
 {
  double eps = 0.00001;
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
	      if (i>=100) { ShowMessage ("ЦИКЛ B CLOX");
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
  double eps = 0.00001;
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
ShowMessage("*** ЦИКЛ B CLOY");
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

 //---------------------------------------------------------------------------
/*
 int korny (int col, double r1,double r2,double&Rabs, double Rmin1,double Rmax1);
  // выявляет попадание r1,r2 в интервал Rmin1,Rmax1
 // и даёт Rabs, возвращает количество попавших, col-число исходных или 2

 void krug( int n1,int n2,double &R,double &xc,double &yc,double &krit)
{//n1,n2 концы дуги окружности // n-общее число точек съёмки
//xt[i],yt[i],kriv[i]-глобальные//даёт радиус по средней кривизне,
//центр по точке в которой кривизна// ближе всех к средней,
// сумму модулей сдвижек на дуге //где брать дугу?

double x1,y1,x2,y2,x3,y3;
//для построения  окружности
int i,j;  double x4,y4,x5,y5,k4,k5;
   double r1,kr1,d,dmin;
   FILE*ff;
ff=fopen("krug.txt","wt");
//начальное приближение
 // для n1->kriv[n-1] то есть это номера вершин и кривизну в крайних считаю

    if(n1==n2)
    {
    //kr1=kriv[n1-1];
    j=n1;//точка для определения центра кривизны

    } //n1==n2 одна точка
   else
   {
     r1=0.0;
    for(int i=n1;i<=n2;i++ )
    {
        r1=r1+kriv[i-1];
    }//for

    kr1=r1/(n2-n1+1);//средняя кривизна
fprintf(ff,"\n радиус по ср. кр. %12.7lf",1.0/kr1);
    dmin=fabs(kriv[n1-1]-kr1);
    for(int i=n1+1;i<=n2;i++ )
    {
    d=fabs(kriv[i-1]-kr1);
  if(fabs(d<dmin)){dmin=d; j=i;}
    }
fprintf(ff,"\nмин_откл_от_ ср_кр dmin=%12.7lf,в точке j=%d, крив=%12.7lf, rad=%12.7lf",
            dmin,j,kriv[j],1.0/kriv[j]);
  //j- номер  вершины , a в массиве кривизн  это j-1
//  на 1 меньше чем в вершине
       }//else n1==n2

       fprintf(ff,"\n n1=%d,n2=%d,j=%d",n1,n2,j);
x1=xt[j-1];y1=yt[j-1];x2=xt[j];y2=yt[j];
x3=xt[j+1];y3=yt[j+1];
//лучший треугольник для R
x4=(x1+x2)/2.0; y4=(y1+y2)/2.;
x5=(x2+x3)/2.0;  y5=(y2+y3)/2.;
// средние точки

if((fabs(y2-y1)>1.e-8)&&( fabs(y3-y2)>1.e-8))
{//нет горизонталей,нормалях нет вертикалей
k4=-(x2-x1)/(y2-y1);
k5=-(x3-x2)/(y3-y2);
//для нормалей  в средних точках 4 и 5
if(fabs(k5-k4)<1.e-8)
{
ShowMessage ("Krug, Средние точки на прямой");
fclose(ff);  //comment
return;
}
xc=(y5-y4+k4*x4-k5*x5)/(k4-k5);
yc=y4+k4*(xc-x4);
// координаты центра x,y соответственно
} else
{
if(  fabs(y2-y1)<1.e-8)
 {// слева вертикаль
    xc=x4;
    yc=y5+k5*(x4-x5);
 } else
       {
   xc=x5;
   yc=y4+k4*(x5-x4);

       }
   }
    R=1.0/kriv[j-1];   //-радиус
fprintf(ff,"\n радиус=%9.5lf,xcentr=%9.4lf,ycentr=%9.4lf",R,xc,yc);

krit=0.0; double rabs=fabs(R);
for(int i=n1;i<=n2;i++ )
     {
r1=sqrt((xt[i]-xc)*(xt[i]-xc)+(yt[i]-yc)*(yt[i]-yc));
//r1-расстояние  точки съёмки от центра

   // krit=krit+ fabs(r1-rabs);
   if (fabs(r1-rabs)>krit)krit=fabs(r1-rabs);
//расст от точки до окружности

   }//for

fprintf(ff,"\nпо ср кр R=%12.7lf,krit=%7.2lf" ,R,krit);
fclose(ff);  //comment
// Если радиус меньше минимального надо сделать его минимальным вместо среднего и
// снова найти точку ближайшую по кривизне и новый центр
   } //krug
 int interv3(double &Rmin1,double& Rmax1,double s4,double s);
//интервал плюс вне корней s4 и меньше плюс s и больше
 int interv3(double &Rmin1,double& Rmax1,double s4,double s)
 {     //не нужна
//Rmin1 -> Rmax1, s4 и меньше плюс s и больше это три интервал для радиуса
// даёт число зон в итоге, интервал изменённого Rmin1->Rmax1 или
// при двух зонах это две зоны по R Rmin->s4,s->Rmax
 if(Rmin1<s4)
 {
 if(Rmax1<s4) return 1; else
 if(Rmax1<s){Rmax1=s4; return 1;}
 ShowMessage ("две зоны по R Rmin->s4,s->Rmax"); return 2;
 }else
 {
 if(Rmin1<s)
   {
  if(Rmax1<s) return 0; else{Rmin1=s; return 1;}
   }
 }
 return 1;
 }

 int korny (int col, double r1,double r2,double&Rabs, double Rmin1,double Rmax1)
  {           int nk; double s1=-1.0;
  nk=0;//программа не нужна два корня конкретно но не интервал сажает на интервал
    if((r1>=Rmin1)&&(r1<=Rmax1)){s1=r1;nk++;}
    if((col==2) &&(r2>=Rmin1)&&(r2<=Rmax1))nk++;
    if(!nk) return 0;
    if(nk==1){ if( s1>0.0)Rabs=r1; else Rabs=r2; return 1;}
    // 2 корня
      if(fabs(Rabs-r1)<fabs(Rabs-r2))Rabs=r1; else Rabs=r2;
      return 2;

  } //korny
 //
 */
int km_to_pic(double km,long *pic,float *plus);
  void pic_to_km(  long pic, float plus,double *km);
//для krug
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
 double otpr(double xa,double ya,double xb,double yb,double x,double y)
{
/*возвращает расстояние от точки до прямой >0 если точка слева
a,b-точки на прямой,x,y-ищем расстояние от неё
*/
 double d, x1,y1,x2,y2,vpr;
if(fabs(xb-xa)<0.00001)//вертикаль
  {d=xa-x;  if (yb<ya) d=-d;
  return d;
  }//
  else
  {
x1=xb-xa; y1=yb-ya;
x2=x-xa; y2=y-ya;// 2  ??????? ?? ????? ?
vpr=x1*y2-x2*y1;
// векторное произведение=absin(alf)=ad; d=vpr/a;
d=vpr/sqrt(x1*x1+y1*y1);
return d;
   }
}//otpr
double tang (double teta,double R, double Bc, double &p)
{
  //даёт  p и тангенс
p=Bc*cos(teta/2.0)-R*(1.0-cos(teta/2.0));
 if(p<0.0)return 1;
return ((R+p)*tan(teta/2.0)+sqrt(24.0*R*p)/2.0);
}//tang
 int interv3(double &Rmin1,double& Rmax1,double s4,double s);
//интервал плюс вне корней s4 и меньше плюс s и больше
 int interv3(double &Rmin1,double& Rmax1,double s4,double s)
 {
//Rmin1 -> Rmax1, s4 и меньше плюс s и больше это три интервал для радиуса
// даёт число зон в итоге, интервал изменённого Rmin1->Rmax1 или
// при двух зонах это две зоны по R Rmin->s4,s->Rmax
 if(Rmin1<s4)
 {
 if(Rmax1<s4) return 1; else
 if(Rmax1<s){Rmax1=s4; return 1;}
 ShowMessage ("две зоны по R Rmin->s4,s->Rmax"); return 2;
 }else
 {
 if(Rmin1<s)
   {
  if(Rmax1<s) return 0; else{Rmin1=s; return 1;}
   }
 }
 return 1;
 }

   int rashet(int n1,int n3,double xv,double yv,
  double teta,double& L1,double& L2,double& Lk,double& Lpr,
  double xlv,double ylv,double& xpv,double& ypv)
   //xlev,ylev,xpr,ypr- точки слева и справа за них не залезать!
   //xpv,ypv- полученный реально конец справа
   //выход длины L1=L2,Lk,Lpr- клотоиды, окр, прямая слева, xpr,ypr- конец
// n1-начало,n3-номер точки на втором тангенсе для угла.возвращает нуль при неудаче
// Нужна точка слева xlv,ylv, на которой всё закончилось до работы с этой ВУ На первой ВУ-
// это начало xt[0],yt[0], а далее это xpv,ypv ->xlv,ylv
  {
   double Tlev,Tpr,Bc,T,Tmin,Tmax;//L1,L2-клотоиды левая и правая
   double Bcmin,Bcmax;
double R,Rabs,p,s,s1,s2,s3,s4,skr,Rmin1,Rmax1,d1,d2;
double s5,s6,s7;
 double a,b,c,r1,r2;
 int nminotkl,i,pr;
 //npk,kpk-кривая в ней ищем  радиус и биссектрису пока простым способом
// надо ещё учесть конец кривой слева, чтобы
// не залезть и вычислить длину прямой вставки
// тангенсы 1->2  и 3->4предварительно по сходу с тангенса
//Bc-биссектриса суммарно,T- пробный тангенс при равных переходных
Rmin1=Rmin; Rmax1=Rmax;// Rmin,Rmax не менять
s5=(lmin+lkmin)/teta;
if(Rmin1<s5){ShowMessage("Малы радиус и угол поворота");
ShowMessage("Радиус увеличен до "+FloatToStr(s5)+" м");
fprintf(fp, "\nРадиус увеличен до %9.3lf м",s5);
Rmin1=s5;
}
Tmax=rast(xlv,ylv,xv,yv)-lprmin;
p=lmin*lmin/(24.0*Rmin1);
T=(Rmin1+p)*tan(teta/2.0)+lmin/2.0;
if(T>Tmax){ShowMessage("Ошибка! Нет тангенса при минимальных нормах");
fprintf(fp,"Ошибка! ВУ близки. Нет тангенса при минимальных нормах");
fprintf(fp,"\n T=%9.2lf,Tmax=%9.2lf,p=%6.2lf,Rmin1=%9.2lf",T,Tmax,p,Rmin1);
fclose(fp); return 1;//Минимум при R=lmin/sqrt(5), но он меньше Rmin
}
//Rmin,Rmax глобальные нельзя портить!! blk- биссектриса для любого R по lkmin

Bcmin=(1.0/cos(teta/2.0)-1.0)*Rmin1;//Иначе p<0
Bcmax=(Tmax-lmin/2.0)/sin(teta/2.0)-Rmin1;
fprintf(fp,"\n teta=%9.6lf,Rmin1=%7.2lf,Rmax1=%7.2lf,Bcmin=%7.2lf,Bcmax=%7.2lf,Tmax=%7.2lf",
              teta,Rmin1,Rmax1,Bcmin,Bcmax,Tmax);
fprintf(fp,"\n xv=%10.4lf,yv=%10.4lf,xlv=%10.4lf,ylv=%10.4lf",
xv,yv,xlv,ylv);
if(Bcmax<Bcmin)
{ShowMessage("Ошибка в нормах или Tmax");fclose(fp);return 1;};
// p- сдвижка, s,s1,s2,s3,s4,Rmin1,Rmax1-рабоч
Bc=Bcmax;
for(int i=n1;i<n3;i++)
{
d2= rast(xt[i],yt[i],xv,yv);
if(d2<Bc){Bc=d2;nminotkl=i;}
//это не биссектриса так как не по центру
// принять за биссектрису а направление вычислить по углам
 }//for
 s5=fabs(kriv[nminotkl-1]);
 s1=fabs(kriv[nminotkl-2]);
 s2=fabs(kriv[nminotkl]);
 i=-1;
  if(s2>s1){s1=s2; i=1;} //наибольшая кривизна
 if(s5<1.E-6)
 { fprintf(fp,"\nПропущена прямая");
 ShowMessage("Пропущена прямая");
  if((kriv[nminotkl-2]*kriv[nminotkl]>1.e-9)&&(s1>1.e-5))
   { R=1.0/kriv[nminotkl-1+i];
   ShowMessage("Расчёт продолжен с радиусом"+FloatToStr(R)+" м");
  }
  //одинаковый знак и есть что взять

  else { ShowMessage("Расчёт прерван. Задать новые точки");fclose(fp); return 1; }
 } else
 R=1.0/kriv[nminotkl-1];//радиус в ближайшей к вершине точке
 //Rmin,lmin,lkmin-глобальные
 Rabs=fabs(R); if(Rabs<Rmin1)Rabs=Rmin1;     //исследуемый радиус
 fprintf(fp,"\nmin otkl в  i=%d,x[i]=%7.2lf,y[i]=%7.2lf",
nminotkl,xt[nminotkl],yt[nminotkl]);
fprintf(fp,"\nНачало lmin=%7.2lf,Tmax=%7.2lf,Bc=%7.2lf,R=%7.2lf,Rmin=%7.2lf",
                      lmin,Tmax,Bc,R,Rmin1);
  s5=(lkmin+lmin)/teta;
//if(Rabs<s5)Rabs=s5;
if(Rabs>Rmax)
{ShowMessage("Ошибка в teta или Rmax");fclose(fp);return 1;};
if((Rmin1+lmin*lmin/(24.0*Rmin1))*tan(teta/2.0)+lmin/2.0>Tmax)
{ShowMessage("Ошибка в нормах или Tmax");fclose(fp);return 1;}

skr=lmin*sin(teta/4.0)/(sqrt(3.0)*cos(teta/2.0));
//верхняя точка >lkmin*lkmin иначе нет решений
//r=Bc*cos(teta/2)/(1.0-cos(teta/2.0))>2.0*Rmin1

 if(skr>Bcmin)Bcmin=skr;
 //s1=2.0*Rmin1/cos(teta/2)*(1.0-cos(teta/2.0));
 //???минус в правой части неравенства по lmin на него не ставить это м.б. избыточно
 s1=Rmin1/cos(teta/2)*(1.0-cos(teta/2.0));
 //Bc при ней под корнем нуль при Rmin1.при меньшей Bc нечего искать

 // if(s5>Bcmin)Bcmin=s5;
 s5=(1.0/cos(teta/2.0)-1.0)*Rmin1+lmin*lmin/(24.0*cos(teta/2.0)*Rmin1);
// fprintf(fp,"\n skr(верх)=%7.2lf,s1(правая)=%7.2lf,s5(корень>Rmin)=%7.2lf,Bcmin=%7.2lf",
 //skr,s1,s5,Bcmin);
 if(s5<s1)s5=s1;
if(Bcmin<s5)Bcmin=s5;//правый корень по lmin больше Rmin1
//3 условия совместно гарантируют наличие допустимого радиуса в данном угле при
    //при минимальной биссектрисе при увеличении её тем более
//проверка по lmin иначе нет радиуса дающего min переходную, а так между корней
// по lmin закончили  Похоже что s1 можно не вычислять s5>s1 всегда?!!!

//fprintf(fp,"\nконтроль lmin Bc=%7.2lf,Rmin1=%7.2lf,Rmax1=%7.2lf,Bcmin=%7.2lf,Bcmax=%7.2lf",
  //               Bc,Rmin1,Rmax1,Bcmin,Bcmax);

  //интервал для Bc из условия l>lmin построен теперь по lkmin
//Увеличение Bcmin по lkmin не требуется так как Rmin выставлен по lkmin,lmin,а
//Bcmin гарантирует радиус не меньше Rmin1

// теперь про Tmax
// Если нет корней или один при исследуемой Bc, то годится любой радиус
// Rkr<min(Bc*cos(teta/2)/(1-cos(teta/2)-правая,Tmax/sin(teta/2)-Bc)- нуль правой части
//при 2 корнях r1 и r2 интервалы: 0->r1 и r2->Rkr при r2>Rkr второй
// интервал отсутствует; первый скорректировать :
//если r1<Rmin первый интервал пустой иначе от Rmin1->r1
// Имеем Bcmin->Bcmax Ставим Bc текущую в интервал и ищем радиус
// вопрос можно ли было попутно корректироваь интервал радиусов????
// skr=Tmax*(1-cos(teta/2.0))/sin(teta/2.0);//это Bc-при s7=R0(правый
 //край) s7-радиус при котором правая часть неравенства по Tmax=0
 // нуль под корнем и нулевая  правая часть при такой Bc
 //При Bc<skr искать корни и если их нет или 1 то про Tmax всё закончено
 //Если их два то анализ то есть это поиски интервала для радиуса а с Bc закончено
 // если же Bc>skr то интересен только меньший корень если он r1>Rmin1
 //Bc годится и интервал от Rmin1->r1 Иначе надо попытаться уменьшить Bc
 // Находим меньший корень при Bcmin если он <Rmin1 нет решений Иначе
 // ищем Bc перебором с уменьшением от имеющейся (она больше минимальной) до нахождения корня
 // который> Rmin1 Это будет фактически новая Bcmax
//Bcmax=Tmax/sin(teta/2.0)-Rmin1; уже было при проверке
// При такой Bc s7=Rmin1 и при ней есть решение только при R0=Rmin1 (край)
//double R0;// правый край
//Нужен ли Bcmax???
if(Bcmin>Bcmax)//нет решений
{
ShowMessage("Ошибка в нормах или Tmax");fclose(fp);return 1;
}
//Проверка Bcmin,если нет, то искать нечего так как только уменьшая Bc можно Tmax

 ABCTmax(teta, Bcmin, Tmax,a,b,c);//даёт коэфф для поиска корней
i=kvadrat(a,b,c,r1,r2);
if(i>1)//Иначе Bcmin годится Так как нет пересечений
{
  s1=Bc*cos(teta/2)/(1-cos(teta/2));
s7=Tmax/sin(teta/2.0)-Bcmin;
//fprintf(fp,"\n проверка Bcmin=%7.2lf,s1(правая)=%7.2lf,s7(нуль правой части)=%7.2lf",
  //             Bcmin,s1,s7);
//s1-радиус при котором под корнем нуль, s7-нуль в правой части неравенства
//при s7<s1-правый корень чужой
if((r1<Rmin1)&&(s7<s1+0.00001))
//нет решений так как уменьшать Bc не куда
 {
ShowMessage("Ошибка в нормах или Tmax");fclose(fp);return 1;
 }
}
//Это проверка Bcmin меньше для Tmax сделать Bc нельзя
// построение Bcmax Не нужно так как при увеличении Bc Tmax не поймать
 if(Bc<Bcmin)Bc=Bcmin;
//при малом Bc проверять уже не надо но интервал нужно вычислить
  pr=0;//признак наличия интервала радиусов
  Bc=Bc+0.1;
  int jj=0;
  double Rmin2;
  Rmin2=Rmin1;
   //Rmin1 надо сохранять, чтобы начать сначала при новой BC
   // Но это не Rmin, который Rmin1 уже не Rmin!
   while (Bc> Bcmin+0.09)
   {  jj++;
Rmin1=Rmin2; Rmax1=Rmax;
      Bc=Bc-0.1;
   //
      a=6.0*(1.0-cos(teta/2.0));
 b=-6.0*Bc*cos(teta/2.0);
 c=lmin*lmin/4.0;
 i=kvadrat(a,b,c,r1,r2);//корни после возведения в квадрат
//if(jj<4) fprintf(fp,"\n по lmin i=%d,r1=%7.2lf,r2=%7.2lf",i,r1,r2);
 if((!i)||(r2<Rmin1-0.0001)||(r1>Rmax1))
 {ShowMessage("Ошибка при вычислении биссектрисы для lmin");fclose(fp);return 1;}
  if(r1>Rmin1) Rmin1=r1;
  if(r2<Rmax1)Rmax1=r2;
  //по lmin нашли интервал радиусов теперь lkmin
  s1=Bc*cos(teta/2.0)/(1.0-cos(teta/2.0));
  s7=lkmin/teta;
  if(s7 >s1)
  {ShowMessage("Ошибка при вычислении радиуса  для lkmin");
  fprintf(fp,"\nОшибка при вычислении радиуса  для lkmin R*=%8.2lf,R0=%9.2lf,lkmin=%9.2lf,teta=%9.6lf",
                   s1,s7,lkmin,teta);
  fclose(fp);return 1;}

 a=teta*teta+24.0*(1.0-cos(teta/2.0));
 b=-2.0*(teta*lkmin+12.0*Bc*cos(teta/2.0));
 c=lkmin*lkmin;
 i=kvadrat(a,b,c,r1,r2);
 if((!i)||(r2>Rmax1+0.0001))
 {ShowMessage("Ошибка при вычислении радиуса  для lkmin");
  fprintf(fp,"\nОшибка  в R  для lkmin i=%d,r1=%9.2lf,r2=%9.2lf,Rmax1=%9.2lf",
               i,r1,r2,Rmax1);
fprintf(fp,"\n Bc=%9.2lf,cos=%9.6lf,R*=%8.2lf,R0=%9.2lf,lkmin=%9.2lf,teta=%9.6lf",
                  Bc,cos(teta/2.0), s1,s7,lkmin,teta);
  fclose(fp);return 1;}
  if(r2>Rmin1)Rmin1=r2;
if(jj<4)fprintf(fp,"\n интервал по lmin+lkmin от %7.2lf до%7.2lf", Rmin1,Rmax1);
    //Строить общий интервал с возможным уменьшением Bc
   //Вероятно ошибка при работе с lkmin!!!!

  ABCTmax(teta, Bc, Tmax,a,b,c);//даёт коэфф для поиска корней по Tmax
i=kvadrat(a,b,c,r1,r2);
s7=Tmax/sin(teta/2.0)-Bc; // нуль прямой
//if(jj<4)fprintf(fp,"\nпо Tmax i=%d,r1=%7.2lf,r2=%7.2lf,s7=%7.2lf",i,r1,r2,s7);
pr=0;
 //pr- это число интервалов при рассмотрении всех трёх интервалов по lmim+lkmin
 // и 2 возможных по Tmax
//i<2-нет пересечения или касание  значит Tmax не мешает и интервал 1 старый
if(i<2){pr=1;
break;} else//для  Bc интервал  Rmin1->Rmax1
    {
if((r1<Rmin1)&&((r2>s7) ||(r2>Rmax1)))continue;
    }
s5=Rmin1;  s7=Rmax1;//портятся
    pr=1;// 1 или 2 интервала есть!
    if(r2>s7)//есть!!! только левый интервал его обработать
{
if(r1>Rmin1) // лишняя проверка
 {
if(r1<Rmax1) Rmax1=r1;
  break;
  }
//интервал  Rmin1->Rmax1 ушли с pr=1
}
//здесь r2 <s7 ( два пересечения в итоге один или два интервала

                pr=0;
//независимо обрабатываю два корня в пересечении со старым интервалом Rmin1->Rmax1
   if(r1>Rmin1)
 {pr=1; if(r1<Rmax1) Rmax1=r1;}
 //левый корень обработан итоговый интервал Rmin1->Rmax1 к нему от
 // r2 вправо  с исходным интервалом s5->s7
 // получим  один или два интервала

   if(r2<s7) //есть ещё правый интервал от r2 при r2>s7 его нет
  {
  pr++;
   if(pr==1)
   {  //он единственный
      Rmin1=s5; Rmax1=s7;
   if(r2>Rmin1)Rmin1=r2;//интервал r2->Rmax1
   }
    else//  это новый интервал его надо дать как s4->s
   {s4=s5; s=s7;//останется при r2<s5
    if(r2>s4) s4=r2;
    }
   //2 интервала Rmin1->Rmax1 левый и s4->s- правый
  }
  break;
 }//while

  if(!pr)
   {
ShowMessage("Ошибка в нормах или Tmax,при уменьшении Bc");fclose(fp);return 1;
   }
   if(pr==1)
   {
fprintf(fp,"\npr=%d,Rabs=%7.2lf,Rmin1=%7.2lf,Rmax1=%7.2lf,Bc=%7.2lf",
                  pr,Rabs,Rmin1,Rmax1,Bc);
   if(Rabs<Rmin1)Rabs=Rmin1;
    if(Rabs>Rmax1)Rabs=Rmax1;
   }    else
   {

      //2 интервала для Rabs если попадает его оставить иначе ближайший

//fprintf(fp,"\npr=%d,Rabs=%7.2lf,Rmin1=%7.2lf,Rmax1=%7.2lf,Bc=%7.2lf",
  //                pr,Rabs,Rmin1,Rmax1,Bc);
//fprintf(fp,"\ns4=%7.2lf,s=%7.2lf",s4,s);
a=Rabs;
if(!(((Rabs>Rmin1)&&(Rabs<Rmax1))||((Rabs>s4)&&(Rabs<s))))//не попал
    {
    if(Rabs<Rmin1)Rabs=Rmin1; else
     if(Rabs>s)Rabs=s;else //провал
     if(fabs(Rabs-Rmax1)<fabs(Rabs-s4))Rabs=Rmax1; else Rabs=s4;
     }
   }//2 интервала

fprintf(fp,"\n итог jj=%d, pr=%d,Rabs=%8.3lf,Bc=%8.3lf",
jj,pr,Rabs,Bc);


 T=tang(teta,Rabs,Bc,p);
 fprintf(fp,"\nTold=%12.6lf,Rabs=%9.4lf,Bc=%7.2lf,pold=%9.4lf",
               T,Rabs,Bc,p);
  L1=L2=sqrt(24.0*Rabs*p);
fprintf(fp,"\nL1old=%13.8lf",L1);
 //p=Bc*cos(teta/2.0)-Rabs*(1.0-cos(teta/2.0));


 if(p<0){ShowMessage("Не найдена биссектриса.p<0");
      fclose(fp);   return 1;}

  //для этих R,Bc найдём L1+тангенс поточнее

 s1=56.0+sqrt(56.0*56.0-42.*64.*p/Rabs);
  s2=56.0-sqrt(56.0*56.0-42.*64.*p/Rabs);
 s1=Rabs*sqrt(s1);
 s2=Rabs*sqrt(s2);
 if(fabs(L1-s1)<fabs(L1-s2))L1=s1;else L1=s2;
  L2=L1;
 fprintf(fp,"\nL1new=%13.6lf,s1=%13.6lf,s2=%13.6lf",L1,s1,s2);


//T= (Rabs*cos(L1/2./Rabs)+p)*tan(teta/2.0)+L1-L1*L1*L1/Rabs/Rabs/40.0-Rabs*sin(L1/2./Rabs);
T= (Rabs+p)*tan(teta/2.0)+L1-L1*L1*L1/Rabs/Rabs/40.0-Rabs*sin(L1/2./Rabs);
 fprintf(fp,"\nTnew=%12.6lf,p=%9.6lf",T,p);
 double kl,xl,yl;
 kl=1.0/(Rabs*L1);
 xl=clox(kl,L1);
 yl=cloy(kl,L1);
T= (Rabs*cos(L1/2./Rabs)+yl)*tan(teta/2.0)+xl-Rabs*sin(L1/2./Rabs);
 fprintf(fp,"\nTnew=%12.6lf,xl=%10.6lf,yl=p=%9.6lf",T,xl,yl);


   //разобраться
  Lk=(teta-L1/Rabs)*Rabs;
  s1=xlv; s2=ylv;// старая
  if(!ne){s1=xt[0]; s2=yt[0];}
   if(!ne)s5=dugl;else s5=ugls [n1-1];
  xlv=xv-T*cos(s5);   ylv=yv-T*sin(s5);
  xpv=xv+T*cos(ugls[n3-1]);ypv=yv+T*sin(ugls[n3-1]);
    Lpr=sqrt((s1-xlv)*(s1-xlv)+(s2-ylv)*(s2-ylv));
 fprintf(fp,"\nL1=%9.4lf,Lk=%9.4lf,Lpr=%9.4lf", L1,Lk,Lpr) ;
 fprintf(fp,"\ns1=%9.4lf,s2=%9.4lf,xlv=%9.4lf,ylv=%9.4lf,xpv=%7.2lf,ypv=%7.2lf,ugllev=%9.6lf,uglpr=%9.6lf",
  s1,s2,xlv,ylv,xpv,ypv,s5,ugls[n3-1]);
  if(R<0)Rabs=-Rabs;
  if(!ne)
  {
  fprintf(f,"%5d",2*colt-3);
  fprintf(f,"\n%12.6lf",Lpr);
  fprintf(f,"\n%12.6lf%12.6lf%12.6lf",lprmin,Lpr+100.0,lprmin);
    }else
    { fprintf(f,"\n%12.6lf%12.6lf  %12.6lf",0.0,Rmin,Rmax);
  fprintf(f,"\n%12.6lf%12.6lf%12.6lf", Lpr,lkmin,3000.0);
  fprintf(f,"\n %12.6lf",0.0);//возвышение
   ne++;
  }
 fprintf(f,"\n%12.6lf%12.6lf%12.6lf",L1,lmin,200.0);
 fprintf(f,"\n%12.6lf %12.6lf   %12.6lf",Rabs,Rmin,Rmax);
 fprintf(f,"\n%12.6lf%12.6lf%12.6lf", Lk,lkmin,3000.0);
 fprintf(f,"\n %12.6lf",0.0);//возвышение
 ne++;
  fprintf(f,"\n%12.6lf%12.6lf%12.6lf",L2,lmin,200.0);
  fprintf(fp,"\n ne=%d,colt=%d",ne,colt);
//
// fclose(fp);

return 0;


}//rashet
   //
 int rashet1(int znak,int n3, int n4,int npk,int kpk,double xv,double yv,
  double teta,double& L1,double& L2,  double& Lk,double& Lpr,
   double xlv,double ylv,double& xpv,double& ypv)
   //xlev,ylev,xpr,ypr- за эти точки  слева и справа  не залезать!
   //xpv,ypv- полученный конец справа
//выход длины L1,L2,Lk,Lpr- клотоиды, окр, прямая слева, xpr,ypr- конец
// n4-номер точки на втором тангенсе для угла.возвращает нуль при неудаче
//это расчёт при угле больше pi, заменили углом 2pi-ugpov
//(вместо отрицательного модуль), но переходные в другую сторону
//znak=1- поворот большой кривой против часовой иначе минус 1
  {
double R,Rabs,p,s,s1,s2,s3,s4,Rmin1,Rmax1,d1,d2;
//Rmin,Rmax глобальные нельзя портить!!
Rmin1=Rmin; Rmax1=Rmax;// Rmin,Rmax не менять
   double Tlev,Tpr,Bc,T,Tmin,Tmax;//L1,L2-клотоиды левая и правая
// надо ещё учесть конец кривой слева, чтобы
// не залезть и вычислить длину прямой вставки
// тангенсы 1->2  и 3->4предварительно по сходу с тангенса
//Bc-биссектриса суммарно,T- пробный тангенс при равных переходных
/*
Tlev=rast(xt[npk],y[npk],xv,yv); //npk-точка схода с 1->2
Tpr=rast(xt[kpk],y[kpk],xv,yv);//kpk-точка схода с 4->3
Tmax=Tlev; if(Tmax>Tpr)Tmax=Tpr;
//надо обеспечить оба, не более Tmax иначе поздний сход
*/
//double x1,y1,x2,y2;//не все нужны
double x3,y3,x4,y4;
double a,b,c,r1,r2;
 int i1,pr;//pr==0  одна зона по радиусу после проверки по тангенсу иначе две
//x1=xt[n1]; y1=yt[n1]; x2=xt[n2]; y2=yt[n2];
x3=xt[n3]; y3=yt[n3];x4=xt[n4]; y4=yt[n4];
//пока сход не учитывается Только 1)6Rp>lmin*lmin даёт интервал радиусов
//2)тангенс больше 0 или от левой прямой
//важно ВУ ближе к окружности чем  конец  уже пройденной кривой
// случай а)pr1==1 или дальше случай б) pr1=-1
//а)T>rast(ВУ, левая)+lprmin
//б)Tmin=-rast(ВУ, левая)+lprmin.Если это меньше нуля то ноль
//s=rast(xlev,ylev,x[npk],y[npk]);
//Tmin=Tlev-s+lprmin;
// при любом положении конца левой кривой относительно ВУ!!!!
 //if(Tmin>Tmax)Tmin=Tmax;// для прямой вставки  даже с поздним сходом
s=otpr(x3,y3,x4,y4,xlv,ylv);
s1= rast(xv,yv,xlv,ylv);
//нужно сохранить где конец пройденной кривой
int pr1;
if(s*znak>0) pr1=1;else pr1=-1;//1 конец кривой ближе к окружности чем ВУ
 if(pr1==1)Tmin=s1+lprmin; //конец кривой ближе к окружности чем ВУ
 else {Tmin= -s1+lprmin; if(Tmin<0.0) Tmin=0.0;}//вместо 0 лучше lprmin???
 //Tmax нет так как не учтён сход
//учли ориентацию конца пройденной (левой) прямой
int nmaxotkl;
//на кривой найти точку наиболее удалённую от ВУ от неё плясать

int j;
Bc=0.0;//начало  поиска
for(int i=npk;i<npk;i++)
{
d2= rast(xt[i],yt[i],xv,yv);
if(d2>Bc){Bc=d2;nmaxotkl=i;}
//это не биссектриса так как не по центру
// принять за биссектрису а направление вычислить по углам
 }//for

 R=1.0/kriv[nmaxotkl-1];//радиус в дальней  от вершины точке
 //Rmin,lmin,lkmin-глобальные
 Rabs=fabs(R); if(Rabs<Rmin1)Rabs=Rmin1;
 Bc=Bc-2.0*Rabs;//переход к ближней точке
 // условие существования ралиуса при котором переходная больше минимальной:
 //6Rp>lmin*lmim, p=Bccos(Teta/2)-R(1-cos(Teta/2))из Bc=(R+p)sec(teta/2)-R
 //Итог даёт дискриминант>0 и потом интервал для R

 s=2.0*lmin*sin(teta/4.0)/(sqrt(3.0)*cos(teta/2.0));//D>0
 if(Bc<s)Bc=s;//проверка на существование R для  lmin
//проверка по lkmin не нужна
//s=lkmin*(sqrt(teta*teta+6.0*(1.0-cos(teta/2.0)))-teta)/(3.0*cos(teta/2.0));
 //if(Bc<s)Bc=s;//проверка по lkmin на шпильке не нужна!!!
//имеем Bc-проходит по teta,lmim ,lkmin, но R в интервале от нуля до s
//далее d1-шаг по Bc, d2- шаг по R
 d1=d2=0.05;
s1=cos(teta/2.0);
int i=0;     //признак успеха при поиске допустимой Bc  и радиуса
  for(int j=0;j<10;Bc=Bc+d1,j++)
  {//восстановление
    Rmin1=Rmin; Rmax1=Rmax;
 s2=3.0*Bc*s1;
 s3=sqrt(s2*s2-6.0*(1.0-s1)*lmin*lmin);
 s=(s2+s3)/(6.0*(1.0-s1));
 s4=(s2-s3)/(6.0*(1.0-s1));
 // радиус от s4 до s из-за lmin
  if((s<Rmin1)||(s4>Rmax1))continue;// увеличит Bc
  else {if(s<Rmax1)Rmax1=s; if(s4>Rmin1)Rmin1=s4;}// max R
  //проверка по lmin закончена
  //Здесь проверка T>Tmin
    a=4.0-cos(teta)-3.0*cos(teta/2.0);
    b=Bc*(2.0*(1.0-cos(teta))-3.0*cos(teta/2.0))-4.0*Tmin*sin(teta/2.0);
    c=Bc*(Bc*(1.0-cos(teta))-4.0*Tmin*sin(teta/2.0))-2.0*Tmin*Tmin;
 i1=kvadrat(a,b,c,s4,s);
      pr=0;//одна зона по радиусу
 if(i1!=2) goto M1; //нет корней или один значит любой радиус годится
 // надо быть вне интервала<s4 or >s т.е. есть две зоны
 // Сколько их в интервале Rmin1->Rmax1!!!
 if(Rmin1<s4)
 {
 if(Rmax1<s4)
   {   goto M1;// весь интервал в левой зоне
   } else
 if(Rmax1<s)
 {
 Rmax1=s4; goto M1;// весь интервал в левой зоне , но скорректирован
 }
 ShowMessage ("две зоны по R Rmin->s4,s->Rmax"); pr=1;goto M1;
 }else //то есть Rmin>s4 и зона одна правая возможно с корректировкой
 {
 if(Rmin1<s)
   {
  if(Rmax1<s) continue; else{Rmin1=s; goto M1;}
   }
  }
 M1: ;//поиск Rabs
  if(!pr)//1 зона или 2 пока не важно
  {
 if(Rabs<Rmin1){Rabs=Rmin1;i=1; break;}
 else {if(Rabs>Rmax1) Rabs=Rmax1;i=1; break;}
   }
// это как при одной так и двух зонах
if(pr){//2 zony если  между ними то коррекция иначе оставить
 if((Rabs>s4) && (Rabs<s))// в провале
 {
  if(fabs(Rabs-s4)<fabs(Rabs-s))Rabs=s4; else Rabs=s;
  i=1; break;
  }
       } //2 zony
   }//for j Bc

   // или исчерпались или прошли проверку i=1
if(!i){ShowMessage ("Нет допустимой биссектрисы. Проверить нормы");
return 0;}
 //Нашли Bc,Rabs

 p=Bc*cos(teta/2.0)-Rabs*(1.0-cos(teta/2.0));
 if(p<0){ShowMessage("Ошибка! Не найдена биссектриса.p<0");
         return 0;}
 L1=L2=sqrt(6.0*Rabs*p);
 T=(Rabs+p)*tan(teta/2.0)-L1/2.0;//шпилька
 Lk=(2.0*pi-teta)*Rabs-L1;//длина оставшейся круговой
 xpv=xv-T*cos(ugls[n4]);ypv=yv-T*sin(ugls[n4]);
//вычислить длину прямой вставки как расст от конца левой до начала нашей

 s1= rast(xv,yv,xlv,ylv);
 if(pr1==1) //конец кривой ближе к окружности
 {Lpr=T-s1;
 }else Lpr=T+s1;
return 1;

}//rashet1

///+++++++++++++++++++++++++++++++++++++++


 int rashtg(int n2,int n3 ,double xv,double yv,
  double tetaish,double&Rabs,double&Bc, double& L1,double& T)
   //xlev,ylev,xpr,ypr- точки слева и справа за них не залезать!
   //xpv,ypv- полученный реально конец справа
 //выход длины L1=L2,Lk,Lpr- клотоиды, окр, прямая слева, xpr,ypr- конец
// n1-начало,n3-номер точки на втором тангенсе для угла.возвращает нуль при неудаче
// Нужна точка слева xlv,ylv, на которой всё закончилось до работы с этой ВУ На первой ВУ-
// это начало xt[0],yt[0], а далее это xpv,ypv ->xlv,ylv
//
  {
 int nminotkl,i,jj;
 double teta,d2,s1,s2,R,Rmin1,Rmax1,Rlev,Rpr,Bcmin,p;
 double a,b,c,r1,r2;
double s5,s6,s7;
 Rmin1=Rmin; Rmax1=Rmax;
 teta=fabs(tetaish);
 Bc=1.0e7;
 for( int i=n2;i<n3;i++)
{
d2= rast(xt[i],yt[i],xv,yv);
if(d2<Bc){Bc=d2;nminotkl=i;}
//это не биссектриса так как не по центру
// принять за биссектрису а направление вычислить по углам
 }//for
 s1=otpr(xt[n2],yt[n2],xv,yv,xt[nminotkl],yt[nminotkl]);
 s2=otpr(xv,yv,xt[n3],yt[n3],xt[nminotkl],yt[nminotkl]);
 s5=fabs(kriv[nminotkl-1]);
 fprintf(fp,"\nrashtg n2=%d,n3=%d,nmin=%d,1/R=%9.6lf,Bc=%7.2lf",
              n2,n3,nminotkl,kriv[nminotkl-1],Bc);
if((s1*tetaish<0)||(tetaish*s2<0))
{
ShowMessage("ВУ внутри кривой");

fprintf(fp,"\n rashtg ВУ внутри кривой,s1=%12.9lf,s2=%12.9lf, n2=%d,n3=%d,xv=%7.2lf,yv=%7.2lf,\nnmin=%d,tetaish=%9.6lf,Bc=%7.2lf",
                      s1,s2,n2,n3,xv,yv,nminotkl,tetaish,Bc);
fprintf(fp,"\n rashtg xt[n2]=%9.2lf,yt[n2]=%9.2lf,xt[n3]=%9.2lf,yt[n3]=%9.2lf",
               xt[n2],yt[n2],xt[n3],yt[n3]);

if(MessageDlg("Продолжить с Rmin?",
   mtConfirmation, TMsgDlgButtons() << mbYes<< mbNo,0) == mrYes)
   {

   p=lmin*lmin/(24.0*Rmin);
   Bc=(Rmin+p)/cos(teta/2.0)-Rmin;//teta по модулю
     R=Rmin; if(tetaish<0) R=-R;//далее с Rmin

   } else  return 1;

               //return 1;
}

 if(s5<1.E-6)
 { fprintf(fp,"\n  rashtg Пропущена прямая");
 ShowMessage("Пропущена прямая");
 s1=fabs(kriv[nminotkl-2]);
 s2=fabs(kriv[nminotkl]);
 i=-1;
 fprintf(fp,"\nnmin=%d,krlev=%9.6lf,krpr=%9.6lf,kriv=%9.6lf",
       nminotkl, kriv[nminotkl-2],kriv[nminotkl],kriv[nminotkl-1]);
  if(s2>s1){s1=s2; i=1;} //наибольшая кривизна
fprintf(fp,"\ni=%d,1/R=%9.2lf",i,kriv[nminotkl-1]);
  if((kriv[nminotkl-2]*kriv[nminotkl]>1.e-9)&&(s1>1.e-5))
   { R=1.0/kriv[nminotkl-1+i];
   fprintf(fp,"\n rastg Расчёт продолжен с радиусом R=%7.2lf",R);
   ShowMessage("Расчёт продолжен с радиусом"+FloatToStr(R)+" м");
   }
  //одинаковый знак и есть что взять

  else { ShowMessage("Расчёт прерван. Задать новые точки"); return 1; }
 } else
 {
fprintf(fp,"\nrastg nmin=%d,krlev=%9.6lf,krpr=%9.6lf,kriv=%9.6lf",
       nminotkl, kriv[nminotkl-2],kriv[nminotkl],kriv[nminotkl-1]);
fprintf(fp,"\nrastg R=%9.2lf",1.0/kriv[nminotkl-1]);
 //R=1.0/kriv[nminotkl-1];//радиус в ближайшей к вершине точке

   R=3.0/(kriv[nminotkl-2]+kriv[nminotkl-1]+kriv[nminotkl]);
 fprintf(fp,"\nrastg Rсред=%9.2lf",R);
 }
  if(R*tetaish<0)
  {ShowMessage("Радиус и угол имеют разные знаки");
  fprintf(fp,"\nrastg Радиус и угол имеют разные знаки,R=%7.2lf,tetaisx=%9.6lf",
                     R,tetaish);
 if(MessageDlg("Продолжить с Rmin?",
   mtConfirmation, TMsgDlgButtons() << mbYes<< mbNo,0) == mrYes)
   {
   p=lmin*lmin/(24.0*Rmin);
   Bc=(Rmin+p)/cos(teta/2.0)-Rmin;//teta по модулю
     R=Rmin; if(tetaish<0) R=-R;//далее с Rmin

   } else  return 1;
  }
 //Rmin,lmin,lkmin-глобальные

 Rabs=fabs(R); if(Rabs<Rmin1)Rabs=Rmin1;     //исследуемый радиус
 fprintf(fp,"\n rastg min otkl в  i=%d,x[i]=%7.2lf,y[i]=%7.2lf",
nminotkl,xt[nminotkl],yt[nminotkl]);
fprintf(fp,"\nНачало lmin=%7.2lf,Bc=%7.2lf,R=%7.2lf,Rmin1=%7.2lf",
                      lmin,Bc,R,Rmin1);
s5=(lmin+lkmin)/teta;
if(Rmin1<s5){ShowMessage("Малы радиус и угол поворота");
ShowMessage("Min. радиус увеличен до "+FloatToStr(s5)+" м");
fprintf(fp, "\nrastg Min. радиус увеличен до Rmin1=%9.3lf м",s5);
Rmin1=s5;
}
//sqrt(-6RR(1-cos(teta/2)+6RBccos(teta/2))>lmin/2
// из этого плюс R>Rmin надо найти R при минимальной корректировке Bc
// имеем RR(1-cos(teta/2))-RBccos(teta/2)+lminlmin/24<0
//Для своей Bс нахожу интервал для R (R>Rmin1!!!)
 //и при минимальной корректировке R
//Если интервала нет, то найду Bc при которой интервал цепляет Rmin

s1=lmin/cos(teta/2.0)*sqrt((1.0-cos(teta/2.0))/6.0);
if(Bc<s1)Bc=s1;// это из условия дискриминант>0
 s2=sqrt(Bc*Bc*cos(teta/2.0)*cos(teta/2.0)-(1.0-cos(teta/2.0))*lmin*lmin/6.0);
 Rpr=(Bc*cos(teta/2.0)+s2)/(2.0*(1.0-cos(teta/2.0)));
// Интервал будет  Rlev--Rpr
 if(Rpr>Rmin1)// есть интервал иначе нет его
 {
 Rlev=(Bc*cos(teta/2.0)-s2)/(2.0*(1.0-cos(teta/2.0)));
 if(Rlev<Rmin1)Rlev=Rmin1;
 if(Rabs<Rlev)Rabs=Rlev;
 if(Rabs>Rpr)Rabs=Rpr;//осталась Bc и найден Rabs
 fprintf(fp,"\nrastg есть интервал Rabs=%8.2lf,R1=%8.2lf,R2=%8.2lf,Bc=%8.2lf,Rmin1=%8.2lf",
                        Rabs,Rlev,Rpr,Bc,Rmin1);
 } else
 {
 s2=(Rmin1*(1.0-cos(teta/2.0))+lmin*lmin/(24.0*Rmin1))/cos(teta/2.0);
// s5=(1.0/cos(teta/2.0)-1.0)*Rmin1+lmin*lmin/(24.0*cos(teta/2.0)*Rmin1);
 fprintf(fp,"\nrastg нет интервала s2=%8.2lf,R1=%8.2lf,R2=%8.2lf,Bc=%8.2lf,Rmin1=%8.2lf",
                          s2, Rlev,Rpr,Bc,Rmin1);
 Rabs=Rmin1;
 Bc=s2;
 }
 //но при этой Bc может не хватать lkmin
 //по lmin нашли интервал радиусов теперь lkmin
  s1=Bc*cos(teta/2.0)/(1.0-cos(teta/2.0)); //правая точка оси R
  s7=lkmin/teta;// нуль правой части неравенства sqrt(24Rp)<teta*R-lkmin
  if(s7 >s1)
  {ShowMessage("Ошибка при вычислении радиуса  для lkmin");
  fprintf(fp,"\nrastg Ошибка при вычислении радиуса  для lkmin R*=%8.2lf,R0=%9.2lf,lkmin=%9.2lf,teta=%9.6lf",
                   s1,s7,lkmin,teta);
return 1;
  }

 a=teta*teta+24.0*(1.0-cos(teta/2.0));
 b=-2.0*(teta*lkmin+12.0*Bc*cos(teta/2.0));
 c=lkmin*lkmin;
 i=kvadrat(a,b,c,r1,r2);
 if((!i)||(r2>Rmax1+0.0001))
 {ShowMessage("Превышен Rmax при вычислении радиуса  для lkmin");
  fprintf(fp,"\nrashtg Превышен Rmax  для lkmin i=%d,r1=%9.2lf,r2=%9.2lf,Rmax1=%9.2lf",
               i,r1,r2,Rmax1);
fprintf(fp,"\n do Bc=%9.2lf,cos=%9.6lf,R*=%8.2lf,R0=%9.2lf,lkmin=%9.2lf,teta=%9.6lf",
                  Bc,cos(teta/2.0), s1,s7,lkmin,teta);
Bc=(Rmax1*(1.0-cos(teta/2.0))+(teta*Rmax1-lkmin)*(teta*Rmax1-lkmin)/24.0/Rmax1)/cos(teta/2.0);
Rabs=Rmax1;
fprintf(fp,"\nrashtg posle Bc=%9.5lf,Rmax1=%9.2lf",Bc,Rmax1);
  s1=Bc*cos(teta/2.0)/(1.0-cos(teta/2.0)); //правая точка оси R
  s7=lkmin/teta;// нуль правой части неравенства sqrt(24Rp)<teta*R-lkmin
  if(s7 >s1)
  {ShowMessage("Ошибка при вычислении радиуса  для lkmin");
  fprintf(fp,"\nrashtg Ошибка при вычислении радиуса  для lkmin R*=%8.2lf,R0=%9.2lf,lkmin=%9.2lf,teta=%9.6lf",
                   s1,s7,lkmin,teta);
   }
fprintf(fp,"\n rashtg для lkmin R*=%8.2lf,R0=%9.2lf,lkmin=%9.2lf,teta=%9.6lf",
                   s1,s7,lkmin,teta);
//
a=teta*teta+24.0*(1.0-cos(teta/2.0));
 b=-2.0*(teta*lkmin+12.0*Bc*cos(teta/2.0));
 c=lkmin*lkmin;
 i=kvadrat(a,b,c,r1,r2);
  fprintf(fp,"\nrashtg Corr  Bc+ Rmax  для lkmin i=%d,r1=%9.2lf,r2=%9.2lf,Rmax1=%9.2lf",
               i,r1,r2,Rmax1);
 //
 }else
  if(r2>Rmin1)Rmin1=r2;
 //++++
 p=Bc*cos(teta/2.0)-Rabs*(1.0-cos(teta/2.0));
L1=sqrt(24.0*Rabs*p);
T=(Rabs+p)*tan(teta/2.0)+L1/2;
  fprintf(fp,"\nrashtg Итог Bc=%7.2lf, Rabs=%9.2lf,p=%6.2lf,L1=%6.2lf,T=%6.2lf",
                     Bc, Rabs, p, L1, T);
                      return 0;
  }//rashtg
//fprintf(fp,"\nконтроль lmin Bc=%7.2lf,Rmin1=%7.2lf,Rmax1=%7.2lf,Bcmin=%7.2lf,Bcmax=%7.2lf",
  //               Bc,Rmin1,Rmax1,Bcmin,Bcmax);

  //интервал для Bc из условия l>lmin построен теперь по lkmin
//Увеличение Bcmin по lkmin не требуется так как Rmin выставлен по lkmin,lmin,а
//Bcmin гарантирует радиус не меньше Rmin1
//при минимальных Bc,lmin,Rmin-гарантирована Lkmin,
//но только при минимальных!!!
// при текущей Bc найдём интервал радиусов по мин клотоиде lmin

int corrtg (double tetaish,double &Rabs,double &L1,double &Tmax,double &Bc)
{
double teta,Bcmin,Bcmax,p,s,s1,s2,s3,s4,skr,Rmin1,Rmax1,T,d1,d2;
 double a,b,c,r1,r2;
double s5,s6,s7;
int i,pr;
fprintf(fp,"\n пришли в corrtg teta=%9.6lf,Tmax=%9.2lf,Rabs=%9.2lf,Bc=%7.2lf",
                         tetaish,Tmax,Rabs,Bc);
teta=fabs(tetaish);

p=(Bc+Rabs)*cos(teta/2.0)-Rabs;
s6=sqrt(24.0*Rabs*p);
s7=Rabs*teta-s6;
fprintf(fp,"\ncorrtg p=%7.2lf,L1=%7.2lf,Lk=%7.2lf",p,s6,s7);

 Rmin1=Rmin; Rmax1=Rmax;// Rmin,Rmax не менять
s5=(lmin+lkmin)/teta;
if(Rmin1<s5){ShowMessage("Малы радиус и угол поворота");
ShowMessage("corrtg. Радиус увеличен до "+FloatToStr(s5)+" м");
fprintf(fp, "\ncorrtg. Min. Радиус увеличен до %9.3lf м",s5);
fprintf(fp, "\ncorrtgбыл Rmin= %9.3lf,teta=%9.6lf,lmin=%7.2lf,lkmin=%7.2lf",
                Rmin,teta,lmin,lkmin);
Rmin1=s5;
            } //скобка if(Rmin1<s5
p=lmin*lmin/(24.0*Rmin1);
T=(Rmin1+p)*tan(teta/2.0)+lmin/2.0;
if(T>Tmax){ShowMessage("\nCorrtg: Ошибка! Нет тангенса при минимальных нормах");
fprintf(fp,"\nОшибка! Corrtg: Нет тангенса при минимальных нормах");
fprintf(fp,"\n T=%9.2lf,Tmax=%9.2lf,p=%6.2lf,Rmin1=%9.2lf",T,Tmax,p,Rmin1);
 return 1;//Минимум при R=lmin/sqrt(5), но он меньше Rmin
          }
s1=Bcmin=(1.0/cos(teta/2.0)-1.0)*Rmin1;//Иначе p<0
Bcmax=(Tmax-lmin/2.0)/sin(teta/2.0)-Rmin1;
fprintf(fp,"\ncorrtg teta=%9.6lf,Rmin1=%7.2lf,Rmax1=%7.2lf,Bcmin=%7.2lf,Bcmax=%7.2lf,Tmax=%7.2lf",
              teta,Rmin1,Rmax1,Bcmin,Bcmax,Tmax);
if(Bc<Bcmin)Bc=Bcmin;
if(Bcmax<Bcmin)
{ShowMessage("Ошибка в нормах или Tmax");
 fprintf(fp,"corrtgОшибка в нормах или Tmax,Bcmin=%7.2lf,Bcmax=%7.2lf",
                                      Bcmin,Bcmax);
 return 1;
 };
// p- сдвижка, s,s1,s2,s3,s4,Rmin1,Rmax1-рабоч

//  s5=(lkmin+lmin)/teta;
if((Rmin1+lmin*lmin/(24.0*Rmin1))*tan(teta/2.0)+lmin/2.0>Tmax)
{ShowMessage("Corrtg: Ошибка в нормах или Tmax");
 fprintf(fp,"corrtg Ошибка в нормах или Tmax=%7.2lf,Нет тангенса при min нормах",
                        Tmax);
 return 1;
}

skr=lmin*sin(teta/4.0)/(sqrt(3.0)*cos(teta/2.0));
//верхняя точка >lmin*lmin /4 иначе нет решений


 if(skr>Bcmin)Bcmin=skr;

 s5=(1.0/cos(teta/2.0)-1.0)*Rmin1+lmin*lmin/(24.0*cos(teta/2.0)*Rmin1);
 fprintf(fp,"\ncorrtg skr(верх)=%7.2lf,s1(правая)=%7.2lf,s5(корень>Rmin)=%9.4lf,Bcmin=%9.4lf",
 skr,s1,s5,Bcmin);
 //s5 - такая Bc которая гарантирует что при Rmin выполнено Lmin
 //при увеличении Bc тем более но уменьшит нельзя

if(Bcmin<s5)Bcmin=s5;//правый корень по lmin больше Rmin1

//2 условия совместно гарантируют наличие допустимого радиуса в данном угле
    //при минимальной биссектрисе при увеличении её тем более
//проверка по lmin иначе нет радиуса дающего min переходную, а так между корней
// по lmin закончили

fprintf(fp,"\ncorrtg контроль lmin Bc=%7.2lf,Rmin1=%7.2lf,Rmax1=%7.2lf,Bcmin=%9.4lf,Bcmax=%7.2lf",
               Bc,Rmin1,Rmax1,Bcmin,Bcmax);

  //интервал для Bc из условия l>lmin построен теперь по lkmin
//Увеличение Bcmin по lkmin не требуется так как Rmin выставлен по lkmin,lmin,а
//Bcmin гарантирует радиус не меньше Rmin1

// теперь про Tmax
// Если нет корней или один при исследуемой Bc, то годится любой радиус
// Rkr<min(Bc*cos(teta/2)/(1-cos(teta/2)-правая,Tmax/sin(teta/2)-Bc)- нуль правой части
//при 2 корнях r1 и r2 интервалы: 0->r1 и r2->Rkr при r2>Rkr второй
// интервал отсутствует; первый скорректировать :
//если r1<Rmin первый интервал пустой иначе от Rmin1->r1
// Имеем Bcmin->Bcmax Ставим Bc текущую в интервал и ищем радиус
// вопрос можно ли было попутно корректироваь интервал радиусов????
// skr=Tmax*(1-cos(teta/2.0))/sin(teta/2.0);//это Bc-при s7=R0(правый
 //край) s7-радиус при котором правая часть неравенства по Tmax=0
 // нуль под корнем и нулевая  правая часть при такой Bc
 //При Bc<skr искать корни и если их нет или 1 то про Tmax всё закончено
 //Если их два то анализ то есть это поиски интервала для радиуса а с Bc закончено
 // если же Bc>skr то интересен только меньший корень если он r1>Rmin1
 //Bc годится и интервал от Rmin1->r1 Иначе надо попытаться уменьшить Bc
 // Находим меньший корень при Bcmin если он <Rmin1 нет решений Иначе
 // ищем Bc перебором с уменьшением от имеющейся (она больше минимальной) до нахождения корня
 // который> Rmin1 Это будет фактически новая Bcmax
//Bcmax=Tmax/sin(teta/2.0)-Rmin1; уже было при проверке
// При такой Bc s7=Rmin1 и при ней есть решение только при R0=Rmin1 (край)
//double R0;// правый край
//Нужен ли Bcmax???
if(Bcmin>Bcmax)//нет решений
{
ShowMessage("Corrtg: Bcmin>Bcmax,Ошибка в нормах или Tmax");
fprintf(fp,"\n Corrtg: Bcmin>Bcmax, Bcmin=%7.2lf,Bcmax=%7.2lf",Bcmin,Bcmax);
return 1;
}
//Проверка Bcmin,если нет, то искать нечего так как только уменьшая Bc можно Tmax

 ABCTmax(teta, Bcmin, Tmax,a,b,c);//даёт коэфф для поиска корней
i=kvadrat(a,b,c,r1,r2);

fprintf(fp,"\ncorrtg i=%d,проверка Bcmin=%9.4lf,Bc=%9.4lf",
             i, Bcmin,Bc);
if(i>1)//Иначе Bcmin годится Так как нет пересечений
{
  s1=Bc*cos(teta/2)/(1-cos(teta/2));
s7=Tmax/sin(teta/2.0)-Bcmin;
fprintf(fp,"\ncorrtg Rmin1=%7.2lf,r1=%7.2lf,r2=%7.2lf,s1(правая)=%7.2lf,s7(нуль правой части)=%7.2lf",
            Rmin1,r1,r2,s1,s7);
//s1-радиус при котором под корнем нуль, s7-нуль в правой части неравенства
//при s7<s1-правый корень чужой
if((r1<Rmin1)&&(s7<s1+0.00001))
//нет решений так как уменьшать Bc не куда
 {  fprintf(fp,"\nCorrtg: Ошибка в нормах или Tmax");
ShowMessage("Corrtg: Ошибка в нормах или Tmax");return 1;
 }
}//i>1
//Это проверка Bcmin меньше для Tmax сделать Bc нельзя
// построение Bcmax Не нужно так как при увеличении Bc Tmax не поймать
 if(Bc<Bcmin)Bc=Bcmin;
//при малом Bc проверять уже не надо но интервал нужно вычислить
  pr=0;//признак наличия интервала радиусов
  Bc=Bc+0.01;
  int jj=0;
  double Rmin2;
  Rmin2=Rmin1;
   //Rmin1 надо сохранять, чтобы начать сначала при новой BC
   // Но это не Rmin, который Rmin1 уже не Rmin!

   while (Bc> Bcmin)
   {  jj++;
Rmin1=Rmin2; Rmax1=Rmax;
      Bc=Bc-0.01;
if(Bc<Bcmin) Bc=Bcmin;
   //
 a=6.0*(1.0-cos(teta/2.0));
 b=-6.0*Bc*cos(teta/2.0);
 c=lmin*lmin/4.0;
 i=kvadrat(a,b,c,r1,r2);//корни после возведения в квадрат
if(jj<4) fprintf(fp,"\n по lmin jj=%d,i=%d,Bc=%9.4lf,r1=%7.2lf,r2=%7.2lf",
              jj,i,Bc,r1,r2);
 if((!i)||(r2<Rmin1-0.001)||(r1>Rmax1))
 {ShowMessage("Ошибка при вычислении биссектрисы для lmin");
  fprintf(fp,"\n corrtg Ошибка при вычислении биссектрисы для lmin");
  fprintf(fp,"\n i=%d,Bc=%9.4lf,Bcmin=%9.4lf,r1=%7.2lf,r2=%9.3lf,Rmin1=%7.2lf,Rmax1=%7.2lf",
               i,Bc,Bcmin,r1,r2,Rmin1,Rmax1);
    return 1;
  }
  if(r1>Rmin1) Rmin1=r1;
  if(r2<Rmax1)Rmax1=r2;
  //по lmin нашли интервал радиусов теперь lkmin
  s1=Bc*cos(teta/2.0)/(1.0-cos(teta/2.0));
  s7=lkmin/teta;
  if(s7 >s1)
  {ShowMessage("Ошибка при вычислении радиуса  для lkmin");
  fprintf(fp,"\ncorrtg Ошибка при вычислении радиуса  для lkmin R*=%8.2lf,R0=%9.2lf,lkmin=%9.2lf,teta=%9.6lf",
                   s1,s7,lkmin,teta);
return 1;
  }

 a=teta*teta+24.0*(1.0-cos(teta/2.0));
 b=-2.0*(teta*lkmin+12.0*Bc*cos(teta/2.0));
 c=lkmin*lkmin;
 i=kvadrat(a,b,c,r1,r2);
 if((!i)||(r2>Rmax1+0.0001))
 {ShowMessage("Ошибка при вычислении радиуса  для lkmin");
  fprintf(fp,"\ncorrtg Ошибка  в R  для lkmin i=%d,r1=%9.2lf,r2=%9.2lf,Rmax1=%9.2lf",
               i,r1,r2,Rmax1);
fprintf(fp,"\ncorrtg Bc=%9.2lf,cos=%9.6lf,R*=%8.2lf,R0=%9.2lf,lkmin=%9.2lf,teta=%9.6lf",
                  Bc,cos(teta/2.0), s1,s7,lkmin,teta);
 return 1;}
  if(r2>Rmin1)Rmin1=r2;
if(jj<5)fprintf(fp,"\ncorrtg интервал по lmin+lkmin от %7.2lf до%7.2lf",
                Rmin1,Rmax1);
    //Строить общий интервал с возможным уменьшением Bc
   //Вероятно ошибка при работе с lkmin!!!!
 // При корректировке радиуса и переходной ( через Bc) из-за увеличения тангенса
 // всё также  Rmin1,Bcmin интервал по lmin+lk но далее не интервал а корень?!!!
  ABCTmax(teta, Bc, Tmax,a,b,c);//даёт коэфф для поиска корней по Tmax
i=kvadrat(a,b,c,r1,r2);
s7=Tmax/sin(teta/2.0)-Bc; // нуль прямой
if(jj<4)fprintf(fp,"\nпо Tmax jj=%d,i=%d,r1=%7.2lf,r2=%7.2lf,s7=%7.2lf",
                            jj,i,r1,r2,s7);
pr=0;
 //pr- это число интервалов при рассмотрении всех трёх интервалов по lmin+lkmin
 // и 2 возможных по Tmax
//i<2-нет пересечения или касание  значит Tmax не мешает и интервал 1 старый
if(i<2){pr=1;
break;} else//для  Bc интервал  Rmin1->Rmax1
    {
if((r1<Rmin1)&&((r2>s7) ||(r2>Rmax1+0.001)))
{if(jj<4)
fprintf(fp,"\ncorrtg i=%d,Bc=%9.4lf,r1=%10.5lf,r2=%10.5lf,s7=%9.3lf,Rmin1=%9.3lf,Rmax1=%10.5lf",
                   i,Bc,r1,r2,s7,Rmin1,Rmax1);
continue;
}
    }
s5=Rmin1;  s7=Rmax1;//портятся
    pr=1;// 1 или 2 интервала есть!
    if(r2>s7)//есть!!! только левый интервал его обработать
{
if(r1>Rmin1) // лишняя проверка
 {
if(r1<Rmax1) Rmax1=r1;
  break;
  }
//интервал  Rmin1->Rmax1 ушли с pr=1
}
//здесь r2 <s7 ( два пересечения в итоге один или два интервала

                pr=0;
//независимо обрабатываю два корня в пересечении со старым интервалом Rmin1->Rmax1
   if(r1>Rmin1)
 {pr=1; if(r1<Rmax1) Rmax1=r1;}
 //левый корень обработан итоговый интервал Rmin1->Rmax1 к нему от
 // r2 вправо  с исходным интервалом s5->s7
 // получим  один или два интервала

   if(r2<s7+0.001) //есть ещё правый интервал от r2 при r2>s7 его нет
  {
  pr++;
   if(pr==1)
   {  //он единственный
      Rmin1=s5; Rmax1=s7;
   if(r2>Rmin1)Rmin1=r2;//интервал r2->Rmax1
   }
    else//  это новый интервал его надо дать как s4->s
   {s4=s5; s=s7;//останется при r2<s5
    if(r2>s4) s4=r2;
    }
   //2 интервала Rmin1->Rmax1 левый и s4->s- правый
  }
  break;
 }//while

  if(!pr)
   {
ShowMessage("Ошибка в нормах или Tmax,при уменьшении Bc");
fprintf(fp,"\ncorrtg Ошибка в нормах или Tmax,при уменьшении Bc");
fprintf(fp,"\ncorrtg pr=%d,Rabs=%7.2lf,Rmin1=%7.2lf,Rmax1=%7.2lf,Bc=%7.2lf",
                  pr,Rabs,Rmin1,Rmax1,Bc);
return 1;
   }
   if(pr==1)
   {
fprintf(fp,"\ncorrtg pr=%d,Rabs=%7.2lf,Rmin1=%7.2lf,Rmax1=%7.2lf,Bc=%7.2lf",
                  pr,Rabs,Rmin1,Rmax1,Bc);
   if(Rabs<Rmin1)Rabs=Rmin1;
    if(Rabs>Rmax1)Rabs=Rmax1;
   }    else
   {

      //2 интервала для Rabs если попадает его оставить иначе ближайший

//fprintf(fp,"\npr=%d,Rabs=%7.2lf,Rmin1=%7.2lf,Rmax1=%7.2lf,Bc=%7.2lf",
  //                pr,Rabs,Rmin1,Rmax1,Bc);
//fprintf(fp,"\ns4=%7.2lf,s=%7.2lf",s4,s);
a=Rabs;
if(!(((Rabs>Rmin1)&&(Rabs<Rmax1))||((Rabs>s4)&&(Rabs<s))))//не попал
    {
    if(Rabs<Rmin1)Rabs=Rmin1; else
     if(Rabs>s)Rabs=s;else //провал
     if(fabs(Rabs-Rmax1)<fabs(Rabs-s4))Rabs=Rmax1; else Rabs=s4;
     }
   }//2 интервала

fprintf(fp,"\ncorrtg итог jj=%d, pr=%d,Rabs=%8.3lf,Bc=%8.3lf",
jj,pr,Rabs,Bc);


  //для этих R,Bc найдём L1+тангенс поточнее
  p=Bc*cos(teta/2.0)-Rabs*(1.0-cos(teta/2.0));
  L1=sqrt(24.0*p*Rabs);
//теперь точный тангенс
 double kl,xl,yl;
 kl=1.0/(Rabs*L1);
 xl=clox(kl,L1);
 yl=cloy(kl,L1);
T= (Rabs*cos(L1/2./Rabs)+yl)*tan(teta/2.0)+xl-Rabs*sin(L1/2./Rabs);
 fprintf(fp,"\ncorrtg Tnew=%12.6lf,xl=%10.6lf,yl=p=%9.6lf",T,xl,yl);
Tmax=T;//возврат из corrtg

  /*
   //разобраться
  Lk=(teta-L1/Rabs)*Rabs;
  s1=xlv; s2=ylv;// старая
  if(!ne){s1=xt[0]; s2=yt[0];}
   if(!ne)s5=dugl;else s5=ugls [n1-1];
  xlv=xv-T*cos(s5);   ylv=yv-T*sin(s5);
  xpv=xv+T*cos(ugls[n3-1]);ypv=yv+T*sin(ugls[n3-1]);
    Lpr=sqrt((s1-xlv)*(s1-xlv)+(s2-ylv)*(s2-ylv));
 fprintf(fp,"\nL1=%9.4lf,Lk=%9.4lf,Lpr=%9.4lf", L1,Lk,Lpr) ;
 fprintf(fp,"\ns1=%9.4lf,s2=%9.4lf,xlv=%9.4lf,ylv=%9.4lf,xpv=%7.2lf,ypv=%7.2lf,ugllev=%9.6lf,uglpr=%9.6lf",
  s1,s2,xlv,ylv,xpv,ypv,s5,ugls[n3-1]);
  if(R<0)Rabs=-Rabs;
  if(!ne)
  {
  fprintf(f,"%5d",2*colt-3);
  fprintf(f,"\n%12.6lf",Lpr);
  fprintf(f,"\n%12.6lf%12.6lf%12.6lf",lprmin,Lpr+100.0,lprmin);
    }else
    { fprintf(f,"\n%12.6lf%12.6lf  %12.6lf",0.0,Rmin,Rmax);
  fprintf(f,"\n%12.6lf%12.6lf%12.6lf", Lpr,lkmin,3000.0);
  fprintf(f,"\n %12.6lf",0.0);//возвышение
   ne++;
  }
 fprintf(f,"\n%12.6lf%12.6lf%12.6lf",L1,lmin,200.0);
 fprintf(f,"\n%12.6lf %12.6lf   %12.6lf",Rabs,Rmin,Rmax);
 fprintf(f,"\n%12.6lf%12.6lf%12.6lf", Lk,lkmin,3000.0);
 fprintf(f,"\n %12.6lf",0.0);//возвышение
 ne++;
  fprintf(f,"\n%12.6lf%12.6lf%12.6lf",L2,lmin,200.0);
  fprintf(fp,"\n ne=%d,colt=%d",ne,colt);
//
*/
// fclose(fp);

return 0;


}//corrtg
