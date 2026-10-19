#include <stdio.h>
#include <process.h>
#include <malloc.h>
#include <conio.h>
#include <math.h>
#include <windows.h>
#include <vcl.h>
#include <fstream.h>
#include "UGLW.h"

extern char elt_name[80];

int vu(int n1,int n2,int n3, int n4, double &xv, double &yv,double &teta);
int vunew(int n1,int n3, double &xv, double &yv,double &teta,
double xlv,double ylv,double &xpv,double &ypv);
 extern double Lmin;
extern int obrVUPoint(void);
extern char igl_name[80];
double xv,yv,teta,uglpov;
  double s;
 double L1,L2,Lk,Lpr;
 double dx,dy,dugl;//начальные точка и угол после приведения к фортрану
 long pkn; float pln;
 long pk; float pl;
    int i,j;

 extern int n,colt,ne;
extern int*VUPoint;
// при повторном вызове глобальная инициализация не работает!!!
//  xt,yt читаю в FromFile,kriv вычисляю там же ? в cxema есть
  double rast(double x1,double y1,double x2,double y2)
 {return (sqrt((x1-x2) *(x1-x2)+(y1-y2)*(y1-y2)));}

  int kvadrat (double a,double b,double c,double &x1 ,double& x2)
  {
  double D=b*b-4.0*a*c;
  if(D<-1.E-8)return 0;
  if(D< 1.E-8){x1=x2=(-b/(2.0*a)); return 1;}
  x1=(-b-sqrt(D))/(2.*a); x2=(-b+sqrt(D))/(2.*a);
  return 2;
   }
 double krt (int j)
  //даёт кривизну в точке j по трём вершинам xt,yt (j-1,j,j+1)- глобальные
  {  // Но это в массиве kriv будет kriv[j-1]!!!!
   double x1,y1,x2,y2,x3,y3;//для построения  окружности
   double a,b,c,kr,v;

x1=xt[j-1];y1=yt[j-1];x2=xt[j];y2=yt[j];x3=xt[j+1];y3=yt[j+1];
a= sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
b= sqrt((x3-x2)*(x3-x2)+(y3-y2)*(y3-y2));
c= sqrt((x1-x3)*(x1-x3)+(y1-y3)*(y1-y3));

//x[1]=a*b*c/(4.0*sqrt(p*(p-a)*(p-b)*(p-c)));//радиус
// kr=4.0*sqrt(p*(p-a)*(p-b)*(p-c))/(a*b*c);
 v= (y3-y2)*(x2-x1)-(x3-x2)*(y2-y1);
 //удвоенная площадь по векторному произведению со знаком

kr=2.0*v/(a*b*c);
 //if(v<0) kr=-kr;
 return kr;
  }//krt


void ABCTmax(double teta,double Bc,double Tmax,double&a,double&b,double&c)
{
/*
a=3.0*(1.0-cos(teta/2.0))+2.0*sin(teta/2.0)*sin(teta/2.0);
b=4.0*Bc*sin(teta/2.0)*sin(teta/2.0)-4.0*sin(teta/2.0)*Tmax-3.0*Bc*cos(teta/2.0);
c=2.0*Bc*Bc*sin(teta/2.0)*sin(teta/2.0)-4.0*Bc*sin(teta/2.0)*Tmax+2.0*Tmax*Tmax;
*/
a=6.0*(1.0-cos(teta/2.0))+sin(teta/2.0)*sin(teta/2.0);
b=-(6.0*Bc*cos(teta/2.0)-Bc*(1.0-cos(teta))+2.0*Tmax*sin(teta/2.0));
c=(Tmax-Bc*sin(teta/2.0))*(Tmax-Bc*sin(teta/2.0));

}
 int rashet(int n1,int n3,double xv,double yv,double teta,
double&L1,double& L2,double& Lk,double& Lpr,
 double xlv,double ylv,double& xpv,double& ypv);

 int rashet1(int znak,int npk,int kpk,int n3, int n4,double xv,double yv,
 double teta,double&L1,double& L2,double& Lk,double& Lpr,
 double xlv,double ylv,double& xpv,double& ypv);

 int vunew(int n1,int n3, double &xv, double &yv,double &teta,
double xlv,double ylv,double &xpv,double &ypv)
{// не использую
//n1- на левом из неё исходящий ugls(n1-1), n3 на правом аналогично, но
  // на первой прямой n1 не работает есть dx,dy,dugl
 double ugllev,uglpr,x1,y1,x3,y3;
 double b1,b2,k1,k2;
 //double Pi=3.1415926;было
// FILE*ff;
  int i;
 //ff=fopen("ERRORVU","wt");
 //fprintf(ff,"\n ne=%d,n1=%d,n3=%d,dugl=%9.6lf,ugls[n3-1]=%9.6lf",
   //             ne,n1,n3,dugl,ugls[n3-1]);
 if(!ne){ugllev=dugl;x1=dx;y1=dy;} else {ugllev=ugls[n1-1];x1=xt[n1];y1=yt[n1];}
 uglpr=ugls[n3-1];x3=xt[n3];y3=yt[n3];
 //углы исходящие
 teta=fabs(uglpr-ugllev);
 //fprintf(ff,"\nugllev=%9.6lf,uglpr=%9.6lf,teta=%9.6lf",ugllev,uglpr,teta);
 if((teta<1.e-6)||(fabs(teta-pi)<1.e-6))
     {
    ShowMessage("Параллельность. Нет ВУ");
   // fclose(ff);
          return 1;
      }//

      if(fabs (cos(ugllev))<1.e-8)
 {// слева вертикаль
 yv=y1;
 xv=x3+(yv-y3)/tan(uglpr);
 }else
 {
 if(fabs (cos(uglpr))<1.e-8)
 {// справа вертикаль
 yv=y3;
 xv=x3+(yv-y1)/tan(ugllev);
 } else
   {
   k1=tan(ugllev); k2=tan(uglpr);
b1=y1-k1*x1; b2=y3-k2*x3;
    xv=(b1-b2)/(k2-k1); yv=k1*xv+b1;
   }//нет справа вертикали
  }//нет слева вертикали

         if(teta<pi)
{
  i=rashet(n1,n3,xv,yv,teta,L1,L2,Lk, Lpr,xlv,ylv,xpv,ypv);
 return i;
 }
 else  {
       ShowMessage("Шпилька 1"+FloatToStr(teta));
       teta=2.0*pi-teta;
       //znak=1; if(uglpov<0.0)znak=-1;
// i=rashet1(znak,npk,kpk,n3,n4,xv,yv,teta,L1,L2,Lk, Lpr,xlv,ylv,xpv,ypv);
       return 1;
       }
   //return 0;
}//vunew

int vu(int n1,int n2,int n3, int n4, double &xv, double &yv,double &teta)
{
//параметры номера точек  съёмки на тангенсах при движении 1-2,3->4
//xlv,ylv- на ней слева всё кончилось,её даём, xpv,ypv-конец справа получаем
//Контроль на прямую вставку убран. Просто не залезать
//за xlv,ylv - на первом шаге даём начальную точку,
// в итоге вычисляем xlv,ylv,xpv,ypv
double x1,y1, x2,y2,x3, y3,x4,y4;// координаты
double k1,k2,b1,b2,d=0.000001;
/*даёт ВУ по двум точкам  на каждой прямой  и угол поворота teta от прямой 1-2
 к прямой 3-4, возвращает 1 при параллельности, иначе 0
 варианты первая вертикальна:вторая вертикальна ( криминал)
вторая общий случай включая первую горизонталь
не вертикаль но вторая вертикаль     2-первая обычная и вторая тоже
*/
x1=xt[n1]; y1=yt[n1]; x2=xt[n2]; y2=yt[n2];
x3=xt[n3]; y3=yt[n3];x4=xt[n4]; y4=yt[n4];
if(fabs(x1-x2)<d)//1-vert
   {
    if(fabs(x3-x4)<d)//2-vert
      {
        ShowMessage("две вертикали при поиски ВУ");
          return 1;
      }//две вертикали
    else
      {
            //1-vert, 2-common
        xv=x1; k2=(y4-y3)/(x4-x3);
         yv=y3+k2*(x1-x3);
       }
//конец 1-vert, 2-comon, 2-включая горизонталь
     }//1-vert
else
{
if(fabs(x3-x4)<d)//2-vert, а первая нет
     {
xv=x3; k1=(y2-y1)/(x2-x1); yv=y1+k1*(xv-x1);
     }//2-vert
  else
 { k1=(y2-y1)/(x2-x1); k2=(y4-y3)/(x4-x3);
  if(fabs(k2-k1)<d )
      {
    ShowMessage("Параллельность. Нет ВУ");
          return 1;
      }//
    else
   { b1=y1-k1*x1; b2=y3-k2*x3;
    xv=(b1-b2)/(k2-k1); yv=k1*xv+b1;
   }// нет вертикалей   и параллельности
 }//- не вертикаль то есть  нет вертикалей
}//1-не вертикаль
if((rast(x1,y1,xv,yv)<rast(x2,y2,xv,yv))||
   (rast(x4,y4,xv,yv)<rast(x3,y3,xv,yv))||
   (rast(x1,y1,xv,yv)+rast(x2,y2,xv,yv)<rast(x1,y1,x2,y2)+0.01)||
   (rast(x3,y3,xv,yv)+rast(x4,y4,xv,yv)<rast(x3,y3,x4,y4)+0.01) )
   {
   ShowMessage("vu Ошибка в задании прямых. Нет ВУ.");
   fprintf(fp,"\nvu x1=%9.2lf,y1=%9.2lf,x2=%9.2lf,y2=%9.2lf,xv=%9.2lf,yv=%9.2lf",
                   x1,y1,x2,y2,xv,yv);
   fprintf(fp,"\nvu x3=%9.2lf,y3=%9.2lf,x4=%9.2lf,y4=%9.2lf",
                  x3,y3,x4,y4);
   return 1;
   }
int i;

double s1,s2,vpr,cpr,sintet,costet;//uglpov- фактический
//длины, произведения и про угол, teta- параметр функции по ссылке
// векторы после приведения к началу координат
//нумерация углов и кривизн на 1 меньше чем точек.n -тоже уменьшен
//Угол в точке с учётом поворота в ней
i=n1-1; if(i<0)i=0;
uglpov=ugls[n4-1]-ugls[i];//n1 а не n1-1 чтобы с нуля можно начинать
//teta- параметр функции по ссылке

vpr= (x2-x1)*(y4-y3)-(y2-y1)*(x4-x3);   //векторное
cpr=(x2-x1)*( x4-x3)+ (y2-y1)*( y4-y3);    //  скалярное
s1=sqrt((x2-x1)*( x2-x1)+ (y2-y1)*( y2-y1));
s2=sqrt((x4-x3)*( x4-x3)+ (y4-y3)*( y4-y3));
sintet=vpr/(s1*s2);
costet=cpr/(s1*s2);
 if(sintet>0)
{
if(costet>0)
 {
 if((uglpov>0)||(asin(sintet)-uglpov)<0.001) teta=asin(sintet);
 //1-четверть
  else
{ ShowMessage ("Разные знаки углов поворота в ВУ и суммарно по ломаной ");
 fprintf(fp,"\nvu Плохой угол,ugl=%9.6lf,sin=%9.6lf,cos=%9.6lf",
  uglpov,sintet,costet); return 1;
}
 }//costet>0
 else//2 четверть
 {
 if(uglpov>0) teta=pi-asin(sintet);//2-четверть
  else
{ ShowMessage ("Разные знаки углов поворота в ВУ и суммарно по ломаной ");
 fprintf(fp,"\nvu Плохой угол,ugl=%9.6lf,sin=%9.6lf,cos=%9.6lf",
  uglpov,sintet,costet); return 1;
}
 }  //2
}//sintet>0

else //3+4четверть
{

if(costet<0)//3
 {
 if(uglpov<0.0001) teta=-pi-asin(sintet);
 //3-четверть asin<0 teta со знаком угла поворота
  else
{ ShowMessage ("Разные знаки углов поворота в ВУ и суммарно по ломаной ");
 fprintf(fp,"\nvu Плохой угол,uglpov=%9.6lf,sin=%9.6lf,cos=%9.6lf",
  uglpov,sintet,costet); return 1;
}
 }//costet<0

 else//4 четверть costet>0

 {
   if(uglpov<0.0001) teta=asin(sintet);//4-четверть asin<0
  else
{ ShowMessage ("Разные знаки углов поворота в ВУ и суммарно по ломаной ");
 fprintf(fp,"\nvu Плохой угол,ugl=%9.6lf,sin=%9.6lf,cos=%9.6lf",
  uglpov,sintet,costet); return 1;
}
 }//4

 }  //3+4

//teta исчислен как угол поворота от 1->2 к 3->4
//  знак принят по углу поворота на углограмме
// проверить +шпилька!
return 0;
  /*
teta=fabs(uglpov);

if(teta<pi)
{

 i=rashet(n1,n3,xv,yv,teta,L1,L2,Lk, Lpr,xlv,ylv,xpv,ypv);
 return i;
 }
 else  {
       ShowMessage("Шпилька 1"+FloatToStr(teta));
       teta=2.0*pi-teta;

       znak=1; if(uglpov<0.0)znak=-1;
 i=rashet1(znak,npk,kpk,n3,n4,xv,yv,teta,L1,L2,Lk, Lpr,xlv,ylv,xpv,ypv);
       return i;
       }
    */
//Достаточно этого угла!!!!
/*
vpr= (x2-x1)*(y4-y3)-(y2-y1)*(x4-x3);   //векторное
cpr=(x2-x1)*( x4-x3)+ (y2-y1)*( y4-y3);    //  скалярное
s1=sqrt((x2-x1)*( x2-x1)+ (y2-y1)*( y2-y1));
s2=sqrt((x4-x3)*( x4-x3)+ (y4-y3)*( y4-y3));
sintet=vpr/(s1*s2);
costet=cpr/(s1*s2);

 if(sintet>0)
{
if(costet>0) { //1-четверть
          teta=asin(sintet);
  if(uglpov>0&&uglpov<pi/2.0)// на всякий случай контроль
  {
    //вызов
   rashet(npk,kpk,n1,n3,xv,yv,teta,L1,L2,Lk, Lpr,x1,y1,xpr,ypr);
  }
  else ShowMessage("Шпилька 1");
              }  //1-четверть
 else
  {
teta=pi- asin(sintet);//2
if(uglpov>pi/2.0&&uglpov<pi)// на всякий случай контроль
  {
  rashet(npk,kpk,n1,n3,xv,yv,teta,L1,L2,Lk, Lpr,x1,y1,xpr,ypr);
  }// обработка
  else ShowMessage("Шпилька 2");
              }
 }//sintet>0
else //3+4четверть
{
if(costet<0)
{ teta=pi- asin(sintet);//3 asin<0
 if(uglpov>-pi/2.0&&uglpov<0)// на всякий случай контроль
  {

  }// обработка
  else ShowMessage("Шпилька 3");
 }//3
 else
  {
teta=pi+ asin(sintet);//4
if(uglpov>-pi&&uglpov<-pi/2.0)// на всякий случай контроль
  {
  }// обработка
  else ShowMessage("Шпилька 4");
 }//4

}//3+4 четверь
//teta исчислен как угол поворота от
// 1->2 к 3->4 против часовой стрелки
// проверить +шпилька!

  */

  }// конец nu, знаю teta, xv,yv,знал n1,n2,n3,n4 и их координаты

int obrVUPoint(void)
{
     int i,j,j1,j2,ndob,nvu;
//double xlv,ylv,xpv,ypv;
  double *xvu,*yvu,*tet;
  double s,s1;
  //colt -чётное число точек снятых с углограммы,
       // общее число точек это n
 //f=fopen(elt_name,"wt");  //для записи
 fp=fopen("OBRVU.txt","wt");
 fprintf(fp,"\ncolt=%d,n=%d,dx=%8.3lf,dy=%8.3lf,dugl=%9.6lf",
              colt,n,dx,dy,dugl);
 for (i=0;i<colt;i++)
 { j=VUPoint[i];
 fprintf(fp,"\n i=%d,VUPoint[i]=%d,xt=%10.4lf,yt=%10.4lf",
 i,j,xt[j],yt[j]);
 }
 fprintf(fp,"\nпроверить прямые: не есть ли они кривые больших радиусов?");
   ndob=0;
 //i-номер точки начиная  с  нуля
   i=2;
   while(i<colt-1)
   {
  j=VUPoint[i];//номер точки на ломаной
  j1=VUPoint[i+1];
  s=0.0;
  fprintf(fp,"\n i=%d,j=%d,j1=%d",i,j,j1);
  if(x[j1-1]-x[j-1]<Lmin) {i=i+2; continue;}
     for (j2=j;j2<j1;j2++)
     {if(fabs(kriv[j2-1])>1.0/Rmin)
       {
     fprintf(fp,"\n выброс j2=%d,r=%9.2lf",j2,1.0/kriv[j2-1]);
      continue;//пропуск  выбросов
       }
      s=s+kriv[j2-1];//нумерация в кривизне сдвинута
     }  //for  j2
     s=s/(j1-j);
     if(fabs(s)>1.e-15)
     fprintf(fp,"\ni=%d,j=%d,j1=%d,r=%8.2lf",
                     i,j,j1,1.0/s);
     if(fabs(s)<1.0/Rmax){i=i+2; continue;}
     ShowMessage("Кривая вместо прямой Rсред= " +AnsiString(1.0/s));
     fprintf(fp,"\nКривая вместо прямой j=%d,j1=%d,Rсред=%9.2lf,Rmax=%9.2lf",
                     j,j1,1./s,Rmax);
/*
   if(MessageDlg("Добавить точки?",
   mtConfirmation, TMsgDlgButtons() << mbYes<< mbNo,0) == mrYes)
 {
  for(j2=colt-1;j2>i;j2--)

VUPoint[j2+2]=VUPoint[j2];//сдвинул все правее i-ой
VUPoint[i+2]=VUPoint[i+1]-1;
VUPoint[i+1]=VUPoint[i]+1;
    i=i+2;
   colt=colt+2;
   ndob=ndob+2;
 ShowMessage("Добавлены точки");
 //fprintf(fp,"\nДобавлены точки ");
  }else i=i+2;//if добавлять
  */
   }//while i
if(ndob)
{ // по краям прямой с R>Rmax??? а малые тангенсы??
fprintf(fp,"\nДобавлены точки, ndob=%d,colt=%d",ndob,colt);
}

for (i=0;i<colt;i++)
 { j=VUPoint[i];
 fprintf(fp,"\n i=%d,VUPoint[i]=%d,xt=%10.4lf,yt=%10.4lf",
 i,j,xt[j],yt[j]);
 }
  xvu=new double [colt/2];
  yvu=new double [colt/2];
  tet=new double [colt/2];// всё с большим запасом
  nvu=0;//число созданных vu будет colt/2 -1
 //задача создать массив vu

 for (int i=0;i<colt-3;i=i+2)
 { //if(!i){xlv=xt[0]; ylv=yt[0];}else {xlv=xpv; ylv=ypv;}
j=vu(VUPoint[i],VUPoint[i+1],VUPoint[i+2],VUPoint[i+3],xv,yv,teta);
//j=  vunew(VUPoint[i],VUPoint[i+1],xv,yv,teta,xlv,ylv,xpv,ypv);
if(j){ShowMessage("Нет ВУ c точки "+IntToStr(VUPoint[i]));
fprintf(fp,"\nНет ВУ i=%d,VUPi=%d,VUP1=%d,VUP2=%d,VUP3=%d",
     i,VUPoint[i],VUPoint[i+1],VUPoint[i+2],VUPoint[i+3]);
 fclose(fp);// remove(elt_name);

delete [] xvu;delete []yvu;delete []tet;
delete [] kriv; delete [] x;delete [] ugls;
 // exit(0);
  return 1;
     }//не было этой скобки  ??
xvu[nvu]=xv;yvu[nvu]=yv; tet[nvu]=teta;
nvu++;

    j=VUPoint[i]-1; if(j<0)j=0;
uglpov=ugls[VUPoint[i+3]-1]-ugls[j];//n1 а не n1-1 чтобы с нуля можно начинать
fprintf(fp,"\nugly i=%d, xv=%12.4lf,yv=%12.4lf,teta=%9.6lf,uglpov=%9.6lf",
              i,xv,yv,teta,uglpov);
      } //for  все ВУ найдены

//   fclose(f);
//проверить попадание ВУ между точками.
// Номера ВУ: i от 0 до nvu-1 слева точка 2i+1
           /*
for (i=0;i<nvu;i++)// цикл по ВУ
{ //проверка плохая только по левой стороне угла
j=VUPoint[i+i+1];//точка слева от ВУ
j1=VUPoint[i+i];
if(rast(xt[j1],yt[j1],xvu[i],yvu[i])+rast(xt[j],yt[j],xvu[i],yvu[i])<
   rast(xt[j1],yt[j1],xt[j],yt[j])+0.001)
    {
    ShowMessage("ВУ между точками ");
fprintf(fp,"\nВУ между точками i=%d,j1=%d,j=%d,xt[j1]=%9.2lf,xt[j]=%9.2lf,xvu[i]=%9.2lf",
                              i,j1,j,xt[j1],xt[j],xvu[i]);
s=yt[j1]+(yt[j]-yt[j1])/(xt[j]-xt[j1])*(xvu[i]-xt[j1]);
fprintf(fp,"\n s=%9.2lf,yvu=%9.2lf",s,yvu[i]);
fclose(fp);
delete xvu;delete yvu;delete tet;
   return 1;

     }
j1=VUPoint[i+i+2]; j=VUPoint[i+i+3];
  if(rast(xt[j1],yt[j1],xvu[i],yvu[i])+rast(xt[j],yt[j],xvu[i],yvu[i])<
   rast(xt[j1],yt[j1],xt[j],yt[j])+0.001)
    {
    ShowMessage("ВУ между точками ");
fprintf(fp,"\nВУ между точками i=%d,j1=%d,j=%d,xt[j1]=%9.2lf,xt[j]=%9.2lf,xvu[i]=%9.2lf",
                              i,j1,j,xt[j1],xt[j],xvu[i]);
delete xvu;delete yvu;delete tet;
fclose(fp);
   return 1;
    }
}     //for
*/
//проверка положения ВУ закончена нужны тангенсы
double* Tv;
//массив минимальных тангенсов в вершинах, вычислен независимо
 Tv=new double[nvu];
 s=lmin*lmin/(24.0*Rmin);
 for (int i=0;i<nvu;i++)
 {teta=fabs(tet[i]);
 //Tv[i]=(Rmin+s)*tan(teta/2.0)+lmin/2.;см. далее после корректир s1
 s1=(lmin+lkmin)/teta;//min радиус по углу
 if(s1<Rmin)s1=Rmin;
 Tv[i]=(s1+s)*tan(teta/2.0)+lmin/2.;
 fprintf(fp,"\n Тангенсы i=%d,s=%7.2lf,teta=%9.6lf,s1=%7.2lf,Rmin=%8.2lf,lmin=%7.2lf,lprmin=%7.2lf,Tv=%7.2lf",
                 i,s,teta,s1,Rmin,lmin,lprmin,Tv[i]);
 } //for i<nvu
 for (int i=0;i<nvu-1;i++)//начальную  и конечную прямую не контролирую
 {
s= rast(xvu[i],yvu[i],xvu[i+1],yvu[i+1]);
 if(s<Tv[i]+Tv[i+1]+lprmin)
   {
   ShowMessage("Близкие ВУ "+IntToStr(i)+IntToStr(i+1));
   fprintf(fp,"\n  Близкие ВУ i=%d,rast= %7.2lf,Tlev=%7.2lf,Tpr=%7.2lf",
                   i,s,Tv[i],Tv[i+1]);
   fclose(fp);
   delete []xvu; delete []yvu;delete []tet;delete []Tv;
   return 1;
   }
 }//for
 double *Rabs,*Bc,*T,*L1;
 Rabs=new double[nvu];  Bc=new double[nvu];
 T=new double[nvu];L1=new double[nvu];
 //int rashtg(int n2,int n3 ,double xv,double yv,
 // double tetaish,double&Rabs,double&Bc, double& L1,double& T)
 //вызов rashet с teta, xvu,yvu,Tv и др
 for (int i=0;i<nvu;i++)
 {int j1;
 j=VUPoint[2*i+1];
 j1=VUPoint[2*i+2];

 if (rashtg(j,j1,xvu[i],yvu[i],tet[i],Rabs[i],Bc[i],L1[i],T[i]))
   {delete[]xvu;delete []yvu;delete[] tet;delete []Rabs;
    delete[] Bc;delete []L1;delete[] T;
   fclose(fp); return 1;
   }

 }//for
 double *Lpr,*Lk;
 Lpr=new double[nvu];Lk=new double[nvu];
//даёт  Rabs[i],Bc[i],T[i],L1[i]
for (int i=0;i<nvu;i++)
{
fprintf(fp,"\ni=%d,R=%9.2lf,teta=%9.6lf,T=%9.2lf,Tv=%7.2lf,L1=%7.2lf,Bc=%7.2lf",
               i,Rabs[i],tet[i],T[i],Tv[i],L1[i],Bc[i]);
}
  //n+1- число точек fprintf(fp,"\nn=%d,xt[n]=%7.2lf",n,xt[n]);
//s,s1-левая и правая стороны угла поворота
 double res1,res2,def1,def2,dob1=0.0,dob2=0.0,dob3;
 //res1,res2- ресурс слева и справа,def1,def2,dob1,dob2-
 //-дефицит и его покрытие слева и справа dob3- рабочая
  s=rast(xt[0],yt[0],xvu[0],yvu[0]);
 def1=T[0]+lprmin-s;
 if(def1<0.0){def1=0.0;dob3=0.0;} else
 {if(def1>T[0]-Tv[0])
   {
   ShowMessage("obrVUP. Мала начальная длина");
   fprintf(fp,"\n Мала начальная длина s=%7.2lf,T[0]=%7.2lf,Tv[0]=%7.2lf,lprmin=%7.2lf,def=%7.2lf",
                 s,T[0],Tv[0],lprmin,def1);
     return 1;
   }
 dob3=def1;//dob1-итоговая для левого может увеличиться
 }
 //fprintf(fp,"\ndo s=%7.2lf,T[0]=%7.2lf,def1=%7.2lf,dob3=%7.2lf",
//                  s,T[0],def1,dob3);
 //слева приходит то что нужно добрать и не меньше это  и есть dob3
 for (int i=0;i<nvu;i++)
 {//перед корректировкой вычиcлить требуемый
 //тангенс считая дефицит слева и справа
    dob1=dob3;
  if(i!=nvu-1)// не последняя
 {
 s1=rast(xvu[i],yvu[i],xvu[i+1],yvu[i+1]);
 def2=T[i]+lprmin+T[i+1]-s1-dob3;
 // дефицит по правой стороне после корректировки его можно убрать как за счёт
 //ещё большего увеличения левого или только за счёт правого
 if(def2<0.0){def2=0.0;dob2=0.0;}else
   {
   res1=T[i]-dob3-Tv[i];//остаток ресурса слева
     if(res1<0.0)res1=0.0;
    res2=T[i+1]-Tv[i+1];
    if(res2<0.0)res2=0.0;
    if(res1+res2<def2)
        {
    ShowMessage("Нет ресурса для увеличения тангенса");
 fprintf(fp,"obr нет ресурса i=%d,def2=%7.2lf,dob3=%7.2lf,Ti=%9.2lf,Tvi=%9.2lf,Ti1=%9.2lf,Tvi1=%9.2lf",
                       i,def2,dob3,T[i],Tv[i],T[i+1],Tv[i+1]);
        }
     dob1=res1*def2/(res1+res2);dob2=res2*def2/(res1+res2);
     //остаток дефицита на правом поделили на левый и правый
     dob1=dob1+dob3;// реальная добавка для левого
     dob3=dob2;// необходимо для правого при следующем заходе
   }    //есть def2
 }else
 {//последняя
  s=rast(xt[n],yt[n],xvu[nvu-1],yvu[nvu-1]);
 def2=T[nvu-1]+lprmin-s;
 if(def2<0.0){def2=0.0;dob3=0.0;} else
 {if(def2>T[nvu-1]-Tv[nvu-1])
   {
   ShowMessage("obrVUP. Мала последняя длина");
   fprintf(fp,"\n Мала последняя длина s=%7.2lf,T[0]=%7.2lf,Tv[0]=%7.2lf,lprmin=%7.2lf,def=%7.2lf",
                 s,T[nvu-1],Tv[nvu-1],lprmin,def2);
     return 1;
   }

  dob2=def2;
  }//else
 }//на последней ВУ справа не учитываем тангенс
 if(dob1+dob2<0.001)
  {def1=def2=0.0; dob3=0.0;
  fprintf(fp,"\nсвободен i=%d,T[i]=%7.2lf",i,T[i]);
   }//if dob
 //есть один или оба
 else
   {
    if((i==nvu-1)&&(dob2>dob1))dob1=dob2;
 dob3=dob2;//для следующего угла  или в никуда
 T[i]=T[i]-dob1;
  if(T[i]<Tv[i])
   {
 ShowMessage("Близкие ВУ");
 fprintf(fp,"\nobrVUPoint Близкие ВУ i=%d,T[i]=%9.2lf,Tv[i]=%9.2lf",
                                   i,T[i],Tv[i]);
delete[] xvu;delete[] yvu;delete[] tet;delete[] Rabs;
    delete []Bc;delete[] L1;delete []T; delete[] Lpr;
    delete []Lk;
    fclose(fp); return 1;
   }
 }
 if (corrtg (tet[i],Rabs[i],L1[i],T[i],Bc[i]))
   {delete[] xvu;delete[] yvu;delete[] tet;delete [] Rabs;
    delete []Bc;delete[] L1;delete[] T;
     delete[] Lpr; delete []Lk;
   fclose(fp); return 1;
   }

 }//for

  for (int i=0;i<nvu;i++)
 {
  if(tet[i]<0.0)Rabs[i]=-Rabs[i];
  if(!i)
{s=rast(xt[0],yt[0],xvu[0],yvu[0]);
Lpr[0]=s-T[0];
}
 else
 {
s=rast(xvu[i-1],yvu[i-1],xvu[i],yvu[i]);
Lpr[i]=s-T[i-1]-T[i];
  }
Lk[i]=Rabs[i]*tet[i]-L1[i];
fprintf(fp,"\n i=%d,L1=%7.2lf,s=%7.2lf,T=%7.2lf,R=%8.2lf,Lpr=%7.2lf,Lk=%7.2lf,tet=%9.6lf",
               i,L1[i],s,T[i],Rabs[i],Lpr[i],Lk[i],tet[i]);
 }//for
     fclose(fp);
FILE *f;
f=fopen(elt_name,"wt");
ne= 2*nvu-1;
  fprintf(f,"%5d",ne);  //число элементов
  fprintf(f,"\n%12.6lf",Lpr[0]);
  fprintf(f,"\n%12.6lf%12.6lf%12.6lf",lprmin,Lpr[0]+100.0,lprmin);
  for(i=0;i<nvu;i++)
  {
 fprintf(f,"\n%12.6lf%12.6lf%12.6lf",L1[i],lmin,200.0);
 fprintf(f,"\n%12.6lf %12.6lf   %12.6lf",Rabs[i],Rmin,Rmax);
 fprintf(f,"\n%12.6lf%12.6lf%12.6lf", Lk[i],lkmin,3000.0);
 fprintf(f,"\n %12.6lf",0.0);//возвышение
fprintf(f,"\n%12.6lf%12.6lf%12.6lf",L1[i],lmin,200.0);
  if(i==nvu-1) break;
 fprintf(f,"\n%12.6lf%12.6lf  %12.6lf",0.0,Rmin,Rmax);
  fprintf(f,"\n%12.6lf%12.6lf%12.6lf", Lpr[i+1],lkmin,3000.0);
  fprintf(f,"\n %12.6lf",0.0);//возвышение
   }//for
 fclose(f);
delete []xvu;delete[] yvu;delete[] tet;delete[] Rabs;
    delete[] Bc;delete []L1;delete[] T;
    delete[] Lpr; delete []Lk;


 return 0;
}
  void cxema()
 {
//координаты и прочее     читал в FromFile
// в kriv кривизна в точках начиная с первой и кончая предпоследней
// и нумерация снова с нуля
kriv=new double[n];
 if(kriv==NULL){ShowMessage("Нет памяти для KRIV");return;}
  for(j=1;j<n-1;j++)
  kriv[j-1]=krt(j);  //это функция даёт кривзну по трём точкам
  // n-2 вершина пойдёт в n-3 кривизну
  // в массиве kriv заполнены  с нулевой по n-3 было по n-2,
  //в конце была хорда и после неё угол
  kriv[n-2]=0.0;
  fp=fopen("testugl.txt","wt");
 double r;
for( i=0;i<n-1;i++)
{ if(fabs(kriv[i])>1.E-9)r=1./kriv[i]; else r=0.0;
fprintf(fp,"\ni=%d,kriv[i]= %9.6lf, r=%10.2lf", i,kriv[i],r);
 }
 fclose(fp);

// long pkn; float pln;
 // при повторном вызове глобальная инициализация не работает!!!
 //ne=0;

 fp=fopen("testugl_2.txt","wt");

    // igl_name уже читал в FromFile но в другие массивы
 fu=fopen(igl_name,"rt");
   if(fu==NULL){ShowMessage("Нет файла igl_name");  return;}
   fscanf(fu,"%d",&n);
   x= new double[n+1];  // хорды
 if(x==NULL){ShowMessage("Нет памяти для x");return;}

   ugl=new double[n];
 if(ugl==NULL){ShowMessage("Нет памяти для UGL");return;}
   ugls=new double[n];
 if(ugls==NULL){ShowMessage("Нет памяти для UGLS");return;}
//x-хорда а не абсцисса ,ugl-угол поворота из файла iglname */
     fscanf(fu,"%ld%f%lf%lf",&pkn,&pln,&x[0],&ugl[0]);
     n--;
     //пропускаю начало и нумерую с нуля после первой хорды
     // возможно есть ошибка так как суммарный угол
     //в точке с добавкой поворота в ней не лишний ли поворот
   for(i=0;i<n;i++)
    {
     fscanf(fu,"%ld%f%lf%lf",&pk,&pl,&x[i],&ugl[i]);
 fprintf(fp,"\ni=%d,pk=%3ld,pl=%5.2f,x[i]=%8.2lf,ugl[i]=%9.6lf",i,pk,pl,x[i],ugl[i]);
     if(i) {x[i]=x[i-1]+x[i]; ugls[i]=ugls[i-1]+ugl[i];}

     else ugls[0]=ugl[0];

     /*теперь это абсциссы а не хорды ugl-приращения ugls  суммарные
 с учётом следующего поворота и это плохо??!ugl-остались повороты слева???
      */
    }  //for
    kriv[0]=ugl[0]/(x[1]/2.0);
     for(i=1;i<n;i++)
          {if(i<n-1) kriv[i]=ugl[i]/((x[i+1]-x[i-1])/2.0);
           else kriv[i]=ugl[i]/(x[n-1]-x[n-2]);
           }
   // fprintf(fp,"\nначало n=%d,x[n-1]=%8.2lf",n,x[n-1]);
           ugls[n]=ugls[n-1];// надо для вычисления конца тангенса
 for(i=0;i<n;i++)fprintf(fp,"\ni=%d,kriv[i]= %9.6lf,ugl=%9.6lf,ugls=%9.6lf",
i,kriv[i],ugl[i],ugls[i]);
            delete ugl;//больше не нужен
   //n- стал меньше чем число точек  нулевую в кривизне выбросили
 //точки съёмки при нумерации с нуля,
   //kriv[0]-кривизна в первой а не в нулевой точке, абсциссы x[0]- первой точки, а не нулевой
  //если номер точки в массиве kriv равен  j, то номер вершины j+1 в xt,yt
  //абсцисса вершины x[j+1]
//n уменьшил на 1!!!!
 fclose(fp);
  fclose(fu);
           return;

  }//cxema






