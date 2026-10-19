//---------------------------------------------------------------------------


#pragma hdrstop

//---------------------------------------------------------------------------
#include<vcl.h>
#pragma package(smart_init)

#include <math.h>
//#include <io.h>
#include <conio.h>
#include <string.h>
#include<process.h>
#include<stdio.h>
 extern FILE* f1;
 void new_otpr(double x0,double y0,double alf,double a,double b,
          double &x,double &y);

  int new_crossln(double ksi,double eta,double x1,double y1,
                    double u,double v,double alf,double& sh,double &sx);

      int crossln(double ksi,double eta,double x1,double y1,
                    double u,double v,double alf,double& sh);
  extern double clox(double k,double l);
  extern double cloy(double k,double l);
int  conla(double xl,double yl,double phil,double sig,double l,
			 double *xr,double *yr,double *phir);
 int conl(double ksi,double eta,double phi,double sig,double k,double l,
	  double *ksi1,double *eta1,double *phi1,double *sig1);
double cloxs_1 (double x0,double k,double sig, double phi, double l);
double cloys_1 (double y0,double k,double sig, double phi, double l);
//int crossokrnew(double ksi,double eta,double psi,double xk,double yk, double r,
  //             double u,double v,double alf,double& cross,int& np);
int crossclonew(double ksi,double eta,double phi,double sig,double k,
      double l,double u,double v,double alf,double& cross);


//double PI= 3.1415926;
int  conla(double xl,double yl,double phil,double sig,
          double l,double *xr,double *yr,double *phir)
/*
   *********** ДВИЖЕНИЕ ПО КРИВОЙ  *************/
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
    /*
	if(fabs(sig)>0.000001)
    {
      *ksi1=cloxs_1(ksi,k,sig,phi,l);
	*eta1=cloys_1(eta,k,sig,phi,l);
	return 0;
     }
      */
///    это было при значимой кривизне в начале ряд Тэйлора?
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
     /*
 double clox(double k,double l)
 {
  double eps = 0.0000001;
//  double eps1 = 0.000000001;
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
  double eps = 0.0000001;

  double a,b,q,t_cloy;
  int i;
	   if (l<eps && l>-eps) return(0);
	    q = k*l*l/2;  q = -q*q;
	    b = k*l*l*l/2;
	    t_cloy = b/3;
            i=0;
	    do
          {
                i++;

	     if (i>=100){

ShowMessage("*** ЗAЦИKЛИBAHИE B CLOY");
 exit(0);                        }
	      b = b*q/(2*i+1)/(2*i);
	      a = b/(4*i+3);
	      t_cloy = t_cloy + a;

          }
              while (a>eps || a<-eps);

	     return(t_cloy);
  }
  */
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
  E=0.00001;
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
        i++; // на новую строку для новой производной

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
  E=0.00001;
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
    return y;
    }//cloys_1

 //++++++++

 int crossclonew(double ksi,double eta,double phi,double sig,double k,
      double l,double u,double v,double alf,double& cross)
  {//на входе ksi,eta, phi,sig, k, l- о клотоиде,
    // u,v,alf -точка вне её  и напр (beta(j)), cross-рихтовка
  //на выходе lp- длина клотоиды от реального начала до пересечения НЕ НУЖНА
  //возвращает 0- ошибка нет пересечения с этим куском клотоиды параллельность
 // cross-рихтовка,np исключил
       int i; double sh;
  //sh-расст. до хорды  надо для crossln здесь не работает,нужно до клотоиды
  //а при самост.  исп. crossln в perv и др. программах нужна до прямой
      double l0,phi0,x0,y0,ksi0,eta0,x1,y1,l1,lh,lk,u1,v1,s,x,y;
      double x2,y2;
  //    fprintf(f1,"\nclo isx u=%7.2lf,v=%7.2lf",u,v);
         ksi0=ksi; eta0=eta;//ksi,eta меняются а расчёт идёт от начала!!!
       	 l1=l;
	 l0=0.0; //! временно
       x0=ksi;y0=eta;

	if(fabs(sig)> 0.000001)
        {
//!не с нулевой кривизны нужно считать точно при малом к но долго!
        x1=cloxs_1(ksi0,k,sig,phi,l);
      	y1=cloys_1(eta0,k,sig,phi,l);
        }
	                      else
        {

        x2=clox(k,l);
	y2=cloy(k,l);// ! это в повёрнутой и сдвинутой системе (новые)
	x1=ksi0+x2*cos(phi)-y2*sin(phi);
	y1=eta0+x2*sin(phi)+y2*cos(phi); //! в старой системе
        }//	endif c- это конец клотоиды
 //ksi,eta,x1,y1- концы хорды поиска,lh,lk-ее нач и кон длины от нуля sig
  i= crossln(ksi,eta,x1,y1,u,v,alf,sh); //это с общей хордой np не нужен
       //последний параметр это рихтовка до прямой здесь не нужна
   // i= 0- параллельность хорды и поперечника,1-внутри, 2-строго начало,
   // 3-конец, -2 до начала, -3 - за концом
	 if(i==0)
         {
    fprintf(f1,"\ncrossclo поперечник и хорда клотоиды параллельны");
    fprintf(f1,"\n x0=%7.2lf,y0=%7.2lf,x1=%7.2lf,y1=%7.2lf,u=%7.2lf,v=%7.2lf,alf=%7.2lf",
                   x0,y0,x1,y1,u,v,alf);
    fprintf(f1,"Счёт надо прекратить");
    return 0;
    	}//endif
       switch (i)
       {
        case 1: break; //пересечение внутри и работать дальше if(i.eq.1) go to 9
        case 2://if(i.eq.2)  пересечение  в начале
      { lh=l0;lk=l0;x=x0;y=y0;ksi=x0;eta=y0;
	goto M77;// ! на оформление выхода проверить знак!!!
     //   break;
       }// end 2if пересечение  в начале
      case 3: //if(i.eq.3)  пересечение в конце
	lh=l1;lk=l1;ksi=x1;eta=y1;x=x1;	y=y1;
	goto M77; //endif 3
       // break;
      case -2://if(i.eq.-2) then ! до начала
      { fprintf(f1,"\ncrossclo до начала клотоиды");
   fprintf(f1,"\nl,x0=%7.2lf,y0=%7.2lf,x1=%7.2lf,y1=%7.2lf,u=%7.2lf,v=%7.2lf,alf=%7.2lf",
            l,x0,y0,x1,y1,u,v,alf);
     //	cross=-3000.; //! ЕСЛИ ЗА КОНЦОМ  ТО -2000 ВЕРНЁМ ИНАЧЕ СДВИЖКА
	return -2;
        //break;
       }//	!endif i==-2
     case -3://         if(i.eq.-3)
	 { //cross=-2000.0; //пересечение за  концом КЛОТОИДЫ
        return -3;
        //break;
	}//endif-3
    };//end switch     9 continue ! здесь i==1первое пересечение внутри
       //Здесь ошибка надо отбросить правую половину!!!
   // -2,-3 значит за пределами 0- параллельность хорды и поперечника
   //здесь только с i==1
       lk=l1;
       lh=l0;
   //fprintf(f1,"\ncrossln lh=%6.2lf,lk-%6.2lf",lh,lk);
      do  // while  (fabs(lk-lh)>0.001)
   {
        s=(lh+lk)/2;
	if(fabs(sig)> 0.000001)
        {//ошибка s- расст от начальной точки
        x=cloxs_1(ksi0,k,sig,phi,s);//здесь s-расст от начала phi другой!!
	y=cloys_1(eta0,k,sig,phi,s); //нельзя менять   ksi  eta
        }
	                      else
        {
        x2=clox(k,s);
	y2=cloy(k,s);// ! это в повёрнутой и сдвинутой системе (новые)
	x=ksi0+x2*cos(phi)-y2*sin(phi);
	y=eta0+x2*sin(phi)+y2*cos(phi);// ! в старой системе
	}//endif
    	i=crossln(ksi,eta,x,y,u,v,alf,sh);  // с начальной половиной
    //	 рихтовка до прямой   sh здесь не нужна она для perv
       switch (i)
       {
       case 1:
       { lk=s;
        break;
        //goto M11;
    //	 if (i.eq.1) go to 10 ! меняю lk
        }
//если не 1 то нет пересечения с половиной,
// которая начинается всегда с lh надо её менять
// делим пополам и снова с lh до х
      case 2: //if(i.eq.2)then
     { lk=lh;x=ksi;y=eta;
      i=1;
      break;// за switch а там конец цикла и при проверку за цикл 77
	  //goto M77;//оформить в конце
      }//2
     case 3:// if(i.eq.3)then ! в конце
     {lh=lk;ksi=x; eta=y;
       i=1;//выйти но не выходить из crossclo
        break;
	//  goto M77;
      }	//3 endif
      case -2:
      {
  // if(i.eq.-2) слева от начала дальше не будет другой точки
	fprintf (f1,"\n crossclo до начала хорды i=%d",i);
	lk=lh;x=ksi;y=eta;
          break;
	  //goto M77;
       }//-2	endif
    case -3:
      {lh=s; ksi=x; eta=y;
        i=1;
        break;//цикл продолжится
        //goto M11;
      }
     }//end switch -3  это правее левой половины работать дальше
  } while  (fabs(lk-lh)>0.001);//end while
  // здесь пересечение с левой то есть от середины к X1 ( при X1<x0 она правая)

    M77: u1=(ksi+x)/2.0;
        v1=(eta+y)/2.0;
        s=sqrt((u1-u)*(u1-u)+(v1-v)*(v1-v));

    //здесь всё верно. направление поперечника всегда в сторону положительных рихтовок
    // так установлено при вычислении углов с осью ОХ ( влево по ходу сущ. трассы)
      if(((v1-v)*sin(alf)<0.)||((u1-u)*cos(alf)<0.))s=-s;
  //fprintf(f1,"\nisx u=%7.2lf,v=%7.2lf,clot u1=%7.2lf,v1=%7.2lf,s=%7.2lf,bet=%8.6lf",
    //                    u,v,u1,v1,s,alf);
       cross=s; //рихтовка до клотоиды
        //lp=(lh+lk)/2.0-l0;//от начала с нулём кривизны не нужна
      //	np=np+1;//зачем ?
        return i;//хорошо  при 1,2, 3 минусы выход  в switch
   }//    end  crossclonew
 //+++++++++


//new
//crossclonew1 (a,b,uglnew,0.0, k,L1,xt[j],yt[j],beta[j],cross);

//




