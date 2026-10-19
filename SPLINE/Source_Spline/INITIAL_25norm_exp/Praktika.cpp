//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include<process.h>
#include "Praktika.h"
#include "stdlib.h"
#include "math.h"
#include "stdio.h"
#include "SysUtils.hpp"
#include "MainMenu.h"
#include <fstream.h>
#include <iostream.h>
#include "Unit2.h"
#include "Unit6.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
 TForm1 *Form1;
extern TForm2 *Form2;
extern  TForm6 *Form6;

extern void pech();
extern char elt_name[80];
extern char igl_name[80];
///
//extern double spk,spp;//минимальные прямые вставки при стыковке клотоид
  double lfirst;//добавил
 int n;// количество точек для поиска ВУ
 double lmin,lmax,lkmin,lprmin;//по длинам клотоиды и окружности
 double Rmin, Rmax,*xt,*yt,*beta;
extern int obrVUPoint(); // по отмеченным точкам строит ВУ
int ne,colt;/*ne-число участков для записи,colt-число точек для ВУ*/
extern double dx,dy,dugl;
extern double* kriv, *ugls;  //для выдачи в окно
extern double *x,*ugl;
extern int npr,nug;
 extern double Lmax,Lmin;
 double maxrix;
 double dH;
 double dpr,dug;    double pk0;//пикетаж начала
  double  dL; int kdL,kdR;//from Form6
DTPoint *ExPoint; //на массив из 1-го и 2-го столбца файла
TPoint *PMPoint; //массив исходный точек в пикселях ????
float xA; //xA- глобальная переменная -начальная абсцисса
long pkt; float plt;
double SizeKofX,SizeKofY;
 //SizeKofY=200.0; //коэффициенты масштаба по Х и У
 double Xmin, Xmax, Ymin, Ymax;
//Xmax далее вычисляется при вводе как длина трассы
bool isDbClick = false; //флаг двойного щелчка. Если 0 - не было двойного нажатия, 1 - было
int x0; //координата мыши для перемешения
 struct rp1
  { long pk;
    float pl;
    double km;
  }rp[50];
   int krp;/*число точек в RP, т.е. номер последней krp-1*/
 int *VUPoint; //массив исходный номеров точек  для ВУ
     //
extern   int km_to_pic(double km,long *pic,float *plus);
extern  void pic_to_km(  long pic, float plus,double *km);
 void cxema();
    double Asd(AnsiString s)
  {
    char sd[128];
    strcpy(sd,s.c_str());
     for(unsigned int j=0;j<strlen(sd);j++)
   {if(sd[j]==','){sd[j]='.'; break;}}
    return atof(sd);
  }
//---------------------------------------------------------------------------
//пока не нужен
   STARTUPINFO si;
   PROCESS_INFORMATION pi1;
  void vizov(char* st)
  {
    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    ZeroMemory( &pi1, sizeof(pi1) );
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
        &pi1 )             // Pointer to PROCESS_INFORMATION structure.
    )
    {
        //ErrorExit( "CreateProcess failed prof1." ); нет такой функции?
  Application->MessageBox("CreateProcess failed.",st,MB_OK|MB_ICONERROR);
    }

    // Wait until child process exits.
    WaitForSingleObject( pi1.hProcess, INFINITE );

    // Close process and thread handles.
    CloseHandle( pi1.hProcess );
    CloseHandle( pi1.hThread );
  }


__fastcall TForm1::TForm1(TComponent* Owner): TForm(Owner)
{
}
 FILE*ff;

/*******************************************************
Основная функция отрисовки изображения на форме
********************************************************/
   void TForm1::ReDraw()

{
   int i,j; //счетчик
   ff=fopen("ris.txt","wt");
   //Xmax = MPoint[QPoint-1].x1;
  // if(Xp(Xmax)>4096 )SizeKofX=4076.0/Xmax;
   //задаем ширину и высоту области графика
//  Fild->Width = QPoint*20*SizeKofX+40;
  Fild->Width = QPoint*20*SizeKofX;
       //если изображение выходит за рамки области, расширяем канву
   for (i=0;i<QPoint;i++)
      //перевод координат точек углограммы в пиксели
   {
       PMPoint[i].x = Xp(MPoint[i].x1);
       PMPoint[i].y = Yp(MPoint[i].y1);  //xg0 разный!!!
 // fprintf(ff,"\n i=%d, Mxi=%10.2lf, Pxi=%d, Myi=%10.2lf, Pyi=%d",
   //     i,MPoint[i].x1, PMPoint[i].x,MPoint[i].y1, PMPoint[i].y);
    }
    //всё с нуля но xg0 ,yg0  меняются при работе
    //задаем параметры пера
   Fild->Canvas->Brush->Color = clWhite;
   Fild->Canvas->Pen->Color = clBlack;
   Fild->Canvas->Pen->Width = 1;
   //очистить поле графика
   Fild->Canvas->Rectangle(0,0,Fild->Width,Fild->Height);
   //чертим систему координат

   int j0,j1;// ордината рисовки оси ОY вместо Yp(0)
   int jx=14; double ch; char st[10];
   j0=Fild->Height/2; j1=20;
//   fprintf(ff,"\nxg0=%d,yg0=%d,H=%d",xg0,yg0,Fild->Height);
//fprintf(ff,"\nYp(0)=%d, Xp(Xmin)=%d, Xp(Xmax)=%d,Yp(Ymin)=%d, Yp(Ymax)=%d",
  //            Yp(0),Xp(Xmin), Xp(Xmax),Yp(Ymin), Yp(Ymax));
   Fild->Canvas->MoveTo(j1,Panel1->Top);
   Fild->Canvas->LineTo(j1,Fild->Top);
   //положение осей не меняю, но их разбивка и
  //надписи плавают вместе с чертежом эта разбивка должна быть разная!!!
   //горизонталь по центру от 20 до конца трассы
  Fild->Canvas->MoveTo(Xp(Xmin),j0);
    Fild->Canvas->LineTo(Xp(Xmax),j0);
  // fprintf(ff,"\nXmin=%7.2lf,Xp(Xmin)=%d,Xmax=%7.2lf,Xp(Xmax)=%d",
  // Xmin,Xp(Xmin),Xmax,Xp(Xmax));
//могут быть промежуточные точки число двадцаток надо определить
   // по длине участка а не по числу точек
    //LOGFONT LF;//создаем переменную под объект структуры для шрифта
   //создаем объект шрифта
   Fild->Canvas->Font->Name="Arial";//имя шрифта
   Fild->Canvas->Font->Size = 8;//размер шрифта
   Fild->Canvas->Font->Color = clBlack; //цвет шрифта
   Fild->Canvas->Brush->Color = clWhite;
//   GetObject(Fild->Canvas->Font->Handle, sizeof(LF), &LF);

   double xt,xtris,xx;// локальные переменные!!!double* xt global
   int pktris,ost;
   if(!regime)//uglogramma
  {
   pkt=(long)ExPoint[0].x1;
   plt=(float)ExPoint[0].y1; //далее разбивка по 20 от пикетов
   xtris=0.0;
   pic_to_km(pkt,plt,&xt);//начальная абсцисса
   xA=xt;//xA- глобальная переменная -начальная абсцисса
        //разбивка 20, но нумерация пикетажа в увязке с RP1.lst
         //реальный пикетаж не использует поэтому остаток всегда ноль плохо!!
   while (xtris<=Xmax) //Xmax -длина трассы а не XMAX
     {
       Fild->Canvas->MoveTo(Xp(xtris),j0-5);
    //рисование делений на оси Х
     Fild->Canvas->LineTo(Xp(xtris),j0+5);
   //  if((pkt>100)&&(xtris>0.1)) pktris=pkt%100; else  pktris=pkt;
     if(!(pkt%100))pktris=pkt;else  pktris=pkt%100;
   //При плюс>0 пикет не пишется, только при нуле без плюса
        ost=(int)plt%20;
        if(xtris<0.001)  // начало
        {
            if(plt<0.01)
           { // Yp(0) j0

        Fild->Canvas->TextOutA(Xp(xtris-0.1),j0+10,IntToStr(pktris));
      //  Fild->Canvas->Brush->Color = clWhite;
                 xt=xt+20.0; xtris=xtris+20.0;
            }
                else
              { // j0 Yp(0)
Fild->Canvas->TextOutA(Xp(xtris-0.1)-10,j0-20,IntToStr(pktris)+'+'+FloatToStr(plt));
                if(ost==0)
                    {xt=xt+20.0; xtris=xtris+20.0;}
                  else
                    {//начало на плюсе не кратном 20
                       pic_to_km(pkt,0,&xx);
                        if(xx<xt)
                         {//возрастающий пикетаж
                            xt=xt-ost+20; xtris=xtris-ost+20;
                          }
                              else
                                {//убывающий пикетаж
                                 xt=xt+ost; xtris=xtris+ost;
                                }
                           } //начало на плюсе не кратном 20

                }//начало на любом плюсе
       }//начало
   else // не начало
        {
          if(plt<0.01)
            {      // j0 Yp(0)
                 Fild->Canvas->Font->Size = 12;//размер шрифта
   Fild->Canvas->Font->Color = clRed; //цвет шрифта
                 // Fild->Canvas->Brush->Color = clRed;
             Fild->Canvas->TextOutA(Xp(xtris-0.1),j0+5,IntToStr(pktris));
             Fild->Canvas->Font->Size = 8;//размер шрифта
   Fild->Canvas->Font->Color = clBlack; //цвет шрифта
                 // Fild->Canvas->Brush->Color = clWhite;
            }
          else
            {  // j0 Yp(0)
     Fild->Canvas->TextOutA(Xp(xtris-0.2),j0+15,FloatToStr(plt));
            }
          xt=xt+20.0; xtris=xtris+20.0;
        }//end не начало
 // отрисована точка и подготовлена следущая но её надо смотреть на пикетаж
    km_to_pic(xt,&pkt,&plt);
    ost=(int)plt%20;
     if(ost)
  {
      pic_to_km(pkt,0,&xx);
      if(xx<xt) // нет реального пикетажа,остаток
                //при наличии неправильных пикетов есть!
     {//возрастающий пикетаж встаём на +кратное 20 от пикета
       //нарушая разбивку по 20 Разбивка всегда от пикетапо 20 м!!!
          xt=xt-ost;   xtris=xtris-ost;
     }
        else
       { xt=xt-(20-ost); xtris=xtris-(20-ost);}
   } //ost
    km_to_pic(xt,&pkt,&plt);
   }//while
            //углограмма мин и макс углы лучше по  MPoint[i].y1
 // fprintf(ff,"\nregime=%d,Ymin=%7.3lf,Ymax=%7.3lf,Yp(Ymin)=%d,Yp(Ymax)=%d",
   //       regime,Ymin,Ymax,Yp(Ymin),Yp(Ymax));
       ch = Ymin;
           while(ch<=Ymax)
            { sprintf(st,"%4.1lf",ch);
              sscanf(st,"%4.1lf",&ch);
         Fild->Canvas->TextOutA(jx,Yp(ch),st);
      Fild->Canvas->MoveTo(jx-6,Yp(ch));
       Fild->Canvas->LineTo(jx+6,Yp(ch));
     //  fprintf(ff,"\nch=%7.5lf, st=%s",ch,st);
          ch=ch+0.1;   if(fabs(ch)<0.001)ch=0.0;
            }//while
 }//end uglogramma
   else
   { // SizeKofX=2;  SizeKofY=2;
     //  Fild->Canvas->Brush->Color=clWhite;
        j=Xmin/10.;
         xtris = j*10; //Xmin;
         while (xtris<=Xmax) //Xmax -здесь не длина трассы а MAX Х
         {
                Fild->Canvas->MoveTo(Xp(xtris),j0-5);
    //рисование делений на оси Х
                 Fild->Canvas->LineTo(Xp(xtris),j0+5);
        pkt=fabs(xtris)/100.;
         plt=fabs(xtris) -pkt*100;
         if(plt>99){pkt+=1;plt=0.;} j=plt;
        if(pkt>100)pktris=pkt%100;else pktris=pkt;
     if(plt<0.01)
     { Fild->Canvas->Font->Size = 12;//размер шрифта
   Fild->Canvas->Font->Color = clRed; //цвет шрифта
    if(xtris<0.)pktris=-pktris;
     Fild->Canvas->TextOutA(Xp(xtris-0.1),j0+5,IntToStr(pktris));
    Fild->Canvas->Font->Size = 8;//размер шрифта
   Fild->Canvas->Font->Color = clBlack; //цвет шрифта
      }      else  // plt не нуль
      { if(xtris<0.)j=-j;
        Fild->Canvas->TextOutA(Xp(xtris-0.2),j0+15,IntToStr(j));
       }
            xtris=xtris+10.;
         }//while  END OX
          j=Ymin/10.;
         xtris = j*10; //Ymin;
           while(xtris<=Ymax)
      {
       Fild->Canvas->MoveTo(jx-6,Yp(xtris));
       Fild->Canvas->LineTo(jx+6,Yp(xtris));
          pkt=xtris/100;
         plt=xtris -pkt*100;
         if(fabs(plt)>99.)
         {if(plt>0.)pkt+=1;else pkt-=1;plt=0.;}
          if(plt>0.)j=(plt+0.5);else j=plt-0.5;
        if(fabs(pkt)>100)pktris=pkt%100;else pktris=pkt;
     if(fabs(plt)<0.01)
     { Fild->Canvas->Font->Size = 12;//размер шрифта
   Fild->Canvas->Font->Color = clRed; //цвет шрифта
        Fild->Canvas->TextOutA(jx+15,Yp(xtris)-5,IntToStr(pktris));
        Fild->Canvas->Font->Size = 8;//размер шрифта
   Fild->Canvas->Font->Color = clBlack; //цвет шрифта
      }
            else
        Fild->Canvas->TextOutA(jx+5,Yp(xtris),IntToStr(j));
            xtris=xtris+10.;
            }//while
    }// END XY
 // PMPoint=new TPoint[QPoint];перенёс в чтение файла чтобы один раз брать
   //массив координат исходный точек в пикселях для отрисовки
   //fprintf(ff,"\nxg0=%d,yg0=%d,mx=%7.2lf,my=%7.2lf,H/2=%d",
  //  xg0,yg0,SizeKofX,SizeKofY,Fild->Height/2);

 //начертить ломаную
   Fild->Canvas->MoveTo(PMPoint[0].x,PMPoint[0].y);
   Fild->Canvas->Polyline(PMPoint,QPoint-1);
 //отметить исходные точки
  Fild->Canvas->Brush->Color=clRed; //для эллипса
     for (i=0;i<QPoint;i++)
     {

Fild->Canvas->Ellipse(PMPoint[i].x-2,PMPoint[i].y-2,PMPoint[i].x+2,PMPoint[i].y+2);
 //Fild->Canvas->Rectangle(PMPoint[i].x-2,PMPoint[i].y-2,PMPoint[i].x+2,PMPoint[i].y+2);
      }
//отобразить VUPoint
      for (int j=0;j<colt;j++)
      { i=VUPoint[j];
Fild->Canvas->Ellipse(PMPoint[i].x-4,PMPoint[i].y-4,PMPoint[i].x+4,PMPoint[i].y+4);
      }
   //отрисовка границ убрана
   /*
    Fild->Canvas->Pen->Color = clBlue;
    Fild->Canvas->Pen->Width = 1;

                   */
                  fclose(ff); //ris.txt
  }//ReDraw??

/************************************************
перевод мировых координат в пиксели для х
*************************************************/
  int TForm1::Xp (double x)
 {return (xg0+x*SizeKofX); }
//   return (x*SizeKofX); }// зачем +20, это просто отступ от оси
/************************************************
перевод мировых координат в пиксели для y
*************************************************/
int TForm1::Yp (double y)
 {
 return (yg0+Fild->Height/2-y*SizeKofY);
 }

/*******************************************************************
Перевод из пикселей в мировые координаты Х
*********************************************************************/
 double TForm1::pX(double xp)
  {return ((xp-xg0)/SizeKofX);}
   // return (xp/SizeKofX);}//зачем -20?

 double TForm1::pY(double yp)
{return ((yg0+Fild->Height/2-yp)/SizeKofY);}
/***************************************************************
получение исходных данных из файла
****************************************************************/
bool  TForm1::FromFile()
{
  FILE *rpfile;
  rpfile=fopen("..\\rp1.lst","rt");//"..//rp1.lst" читать  сверху
  if(rpfile==NULL)
  {
    ShowMessage("Не задано соответствие пикетажа.ENTER");
    return false;
  }
  int i=0;
  double s;
  while(!feof(rpfile))
  {
    fscanf(rpfile,"%lf",&s);
    if(feof(rpfile))break;
    fscanf(rpfile,"%f%lf",&rp[i].pl,&rp[i].km);
    rp[i].pk=(long)s;
    i++;
  }
  krp=i;
  fclose(rpfile);
  QPoint = 0;   //член -данное класса
  colt=0;//число точек для ВУ
      double z1,z2,z3,z4;
 //ff=fopen("otlris.txt","wt");
 //ff-global open in ReDraw and shut in it
  ifstream Fin;
  Fin.open(igl_name);//исходная углограмма своя на участке
//пикет, плюс, хорда, угол поворота-Это не углограмма а съёмка
  if(!Fin)   //открыть файл
      {
        ShowMessage ("Не удалось открыть файл исходной съёмки .igl");
        return false;
      }

    // получить количество точек
    Fin>>QPoint;
    MPoint=new DTPoint[QPoint+1];
    ExPoint= new DTPoint[QPoint+1];
    PMPoint=new TPoint[QPoint];// перенёс из DRAW()

    VUPoint=new int[QPoint/4];
    int j = 0;
  while (!Fin.eof())
   {  //заполняет лишний j=Qpoint., а надо -1
    Fin>>z1;
    if(Fin.eof()) break;
    Fin>>z2>>z3>>z4;
    if (j==0)
     {
      MPoint[j].x1=z3;
      MPoint[j].y1=z4;
      pk0=z1*100+z2;
     }
    else
     { //углы и хорды суммируются
       MPoint[j].x1=MPoint[j-1].x1+z3;
       MPoint[j].y1=MPoint[j-1].y1+z4;
     }
     ExPoint[j].x1=z1; //пикеты и плюсы
     ExPoint[j].y1=z2;
      j++;
      }//while
    if(j!=QPoint) ShowMessage("Ошибка в исходном файле съёмки");
     n=QPoint;
     /*    for (i=0;i<QPoint;i++)   //перевод координат в пиксели
   {  //ВЕРНУЛ в рисовку из-за масштабов
       PMPoint[i].x = Xp(MPoint[i].x1);
       PMPoint[i].y = Yp(MPoint[i].y1);
  }//перенёс  и зря так как при вызове рисовки могли измениться масштабы
  */
    Xmax = MPoint[QPoint-1].x1;
 // длина углограммы она же длина трассы,а MPoint[QPoint-1].y1- угол в конце?
    Fin.close();
//
     Fin.clear();
//углограмму читаю и создаю массивы во всех режимах
    Fin.open("fort.txt");
    //создаётся при открытии участка, выбор варианта новый
  if(!Fin)   //
      {
        ShowMessage ("Не открылся файл fort.txt ");
  delete []MPoint;
  delete[] ExPoint;delete []PMPoint;delete[] VUPoint;
        return false;
      }
 // Fin>>z1>>z2>>z3>>z4;//строка для фиксации  клотоиды при доп. ограничении   // получить число точек
    Fin>>QPoint;
     xt=new double[QPoint+1];
    yt=new double[QPoint+1];
    beta=new double[QPoint+1];
    //MPoint=new DTPoint[QPoint];
    //ExPoint= new DTPoint[QPoint+1]; //пикеты и плюсы
    //uglpop=new double [QPoint];
    //PMPoint=new TPoint[QPoint];
  // тип TPoint в системе
    for (j=0;j< QPoint;j++)
    {  //заполняет лишний j=Qpoint.,надо -1
     Fin>>z3>>z4>>z1;  //x,y,beta
    //
               xt[j]=z3;
               yt[j]=z4;
               beta[j]=z1;
 //uglpop[j]=z1; не объявлен но записан на будущее теперь это beta
    }//for
     Fin.close();
     double sin,cos;
 s=sqrt((xt[2]-xt[0])*(xt[2]-xt[0])+(yt[2]-yt[0])*(yt[2]-yt[0]));
 sin=(yt[2]-yt[0])/s; cos=(xt[2]-xt[0])/s;
 s=-(xt[1] -xt[0])*sin+(yt[1]-yt[0])*cos;//отклонение в первой точке от 0->2
 if(fabs(s)>0.05)ShowMessage("Начало не на прямой s="+FloatToStr(s));
// при работе в координатах  меняю MP через xt,yt  чтобы нарисовать
    if(regime)
  {  //не углограмма  будем высвечивать график в координатах
    //SizeKofX=1.0;  SizeKofY=1.0;
//z1=xt[0]; z2=yt[0];
  //   fort.txt уже выставлен на начальный нуль
  for (j=0;j< QPoint;j++)
     {
     MPoint[j].x1=xt[j]-xt[0];  // fort.txt
     MPoint[j].y1=yt[j]-yt[0];
     //fprintf(ff,"\nj=%d, mx1=%10.2lf, my1=%10.2lf",
       // j,MPoint[j].x1,MPoint[j].y1);//otlris.txt <-
     }//for
    //Xmax = MPoint[QPoint-1].x1; x может уменьшаться
  }//rezime !=0

 //прочитать добавки в начальной точке
   dugl=0.0;
   FILE *nu;
   nu=fopen("nach_ugl.txt","rt");
    if(nu)
     {
     fscanf(nu,"%lf", &dx);//
     fscanf(nu,"\n%lf", &dy);
    fscanf(nu,"\n%lf",&dugl);//разность проектного и существующего углов
//    ShowMessage(FloatToStr(dugl));
     }
   fclose(nu);
               cxema();
   //снова читает съёмку: хорда+угол,вычисляет абсциссы, сумм. угол и кривизны
          //} rezime==0
    return true;
}//end FromFile

/****************************************************************
вывод значений по умолчанию на форму
*****************************************************************/
 void TForm1:: ToForm()
 {

   ButSizeX->Position = SizeKofX;
   ButSizeY->Position = SizeKofY;
   SizeX->Text = SizeKofX;
   SizeY->Text = SizeKofY;

  }
/*******************************************************************
обработка события onPaint для области графика (объект PainBox)
********************************************************************/
void __fastcall TForm1::FildPaint(TObject *Sender)
{
  if (PMPoint!=NULL)
    { ReDraw();}
}


/***********************************************************
Обработка события перемещения мыши
**************************************************************/
void __fastcall TForm1::FildMouseMove(TObject *Sender, TShiftState Shift,
      int X, int Y)
{ int j,k;
     if( ButCenter->Down && MousePressed ) //кнопка СДВИГ
 {
  j=Xp(Xmin);   k=Xp(Xmax);
   if(((X>xprev)&&(j<20))||((X<xprev)&&(k>20)))
  {  // fprintf(ff,"\nj=%d,k=%d,xg0=%d,X=%d,xprev=%d",j,k,xg0,X,xprev);
   //fprintf(ff,"\nXmin=%7.2lf,Xp(Xmin)=%d",Xmin,Xp(Xmin));
//if( xg0 + X-xprev <= j)  //xg0>20 в нуле будет точка с X<0
   xg0 = xg0 + X - xprev;  //почему нет скобок? Х по условию а Y нет
   }
   yg0 = yg0  + Y - yprev;
    ReDraw();
   xprev = X; yprev = Y;
  } //if But
   Form1->x1 = X;  //считываем координаты мыши на поле графика
    Form1->y1 = Y; //добавил
}
/***********************************************************
Обработка события двойного нажатия клавиши мыши
**************************************************************/

void __fastcall TForm1::FildDblClick(TObject *Sender)
{
     isDbClick = true;  //флаг того, что был двойной щелчок мыши
if(regime!=2) btnDelete->Visible=true;  //кнопка Удалить все точки
}

/***********************************************************
Обработка события нажатия клавиши мыши
Возможно
-перемещение, если стоит флаг isMove = true
//как теперь без границ??? Не границ а точек Для ВУ!!!
-добавление границы, если был двойной щелчек (флаг isDbClick = true;)
-активания границы для редактирования
**************************************************************/

void __fastcall TForm1::FildMouseDown(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{ //помечает точку после двойного щелчка

   MousePressed = true;
   xprev = X;
   yprev = Y;
   //надо отмечать две на нач. прямой и длиннющие прямые
  if(regime==2)return;
  // в автомате точки не нужны а сдвиг нужен!
   if (isDbClick) //был двойной щедчок левой
         {
   //Fild->Canvas->Pen->Color = clBlue; отмечаем все ниже при выходе
  Fild->Canvas->Pen->Width = 1;
  double s,s1;  int j=0;
   double s2,s3;
  for(int i=0;i<QPoint;i++)
  {   //находим ближайшую к текущей и отмечаем этот узел
  //s=sqrt((PMPoint[i].x-X)*(PMPoint[i].x-X)+(PMPoint[i].y-Y)*(PMPoint[i].y-Y));
   s2=PMPoint[i].x-X;s2=s2*s2;
    s3=PMPoint[i].y-Y;s3=s3*s3;
//    fprintf(ff,"\n i=%d,s2=%15.1f,s3=%15.lf", i,s2,s3);
    s=sqrt(s2+s3);
    if(i==0)s1=s; else if(s<s1){s1=s;j=i; continue;}
   }//for
  VUPoint[colt]=j;
  Fild->Canvas->Ellipse(PMPoint[j].x-6,PMPoint[j].y-6,PMPoint[j].x+6,PMPoint[j].y+6);
 //пересчитывать при движении экрана
   colt++;
    isDbClick = false;
   //return;
   }
}

/**************************************************************************
Перевод х в пикет +
***************************************************************************/
/*
void TForm1::ToSysKoord(region *t)
{
 double x1=xA+t->regionX;
 km_to_pic(x1,&pkt,&plt);
t->picet =pkt; t->plus=plt;
}

*/
/**************************************************************************
Обработка события закрытия формы - диалог сохранения
***************************************************************************/

void __fastcall TForm1::btnSaveClick(TObject *Sender)
{
if(ne)  ShowMessage(" Начальное приближение построено");
 else {ShowMessage("Расчёт не выполнен");
if(MessageDlg("Продолжить работу?",
mtConfirmation, TMsgDlgButtons() << mbYes<< mbNo,0) == mrYes)	   return;
        }
    delete []ExPoint; delete[] PMPoint; delete[] MPoint;
    delete[] VUPoint;
    delete[] xt; delete []yt;delete []x;
    delete []kriv;delete []ugls;
Form1->Close();
Form1->Release();
}

/****************************************************************************
Изменение маштаба по х
*****************************************************************************/
void __fastcall TForm1::ButSizeXClick(TObject *Sender, TUDBtnType Button)
{
if (Button==btNext)
 {
        SizeKofX = SizeKofX+0.2;
 }
 if ((Button== btPrev)&&(SizeKofX-0.2>0.00001) )
 {
        SizeKofX = SizeKofX-0.2;
 }
 SizeX->Text = SizeKofX;
 ReDraw();
}
/****************************************************************************
Изменение маштаба по у
*****************************************************************************/
void __fastcall TForm1::ButSizeYClick(TObject *Sender, TUDBtnType Button)
{
   double d;
   if(!regime)d=10.0; else d=0.2;//0-углограмма
if (Button==btNext)
 {
        SizeKofY = SizeKofY+d;
 }
 if ((Button== btPrev)&&(SizeKofY-d>0.00001) )
 {
        SizeKofY = SizeKofY-d;
 }
   SizeY->Text = SizeKofY;
   ReDraw();
}
/*********************************************************************
Вызов инструкции
**********************************************************************/

void __fastcall TForm1::btnRefClick(TObject *Sender)
{
 /*
 char put[180];
 strcpy(put,"..\\AkelPad.exe ");
 strcat(put, " ..\\INSTR.txt");
 ShowMessage(put);
 */
 FILE *imd;
imd=fopen("..//imd.txt","rt");
char st[180], put[180];
fscanf(imd,"%s",st);
fclose(imd);
 ShowMessage(AnsiString(st));
 strcpy(put,"..//AkelPad.exe ");
 strcat(put,st);
 strcat(put, "\\INSTR.txt");
 ShowMessage(put);
 //vizov(put);
  WinExec(put,SW_SHOW);
 return;
}

/*********************************************************************
Изменение размера формы
**********************************************************************/

void __fastcall TForm1::FormResize(TObject *Sender)
{
  //похоже не вызывается

 Panel1->Width = Form1->ClientWidth;
Panel1->Top = Form1->ClientHeight-Panel1->Height-200;//-80 длбавил
   if(!regime) SizeKofY = MainBox->ClientHeight/2;
     //SizeKofY = MainBox->ClientHeight;
      SizeY->Text = SizeKofY;
}

void __fastcall TForm1::FormShow(TObject *Sender)
{
  MousePressed = false;
  xg0 = 20 ; yg0 = 0;
  Form4->ShowModal();//даёт режим и только
   if(regime==3) //выход
   {Form1->Close();
   Form1->Release();
   return;
   }
     if(regime==2)btnDelete->Visible=0;
      else  btnDelete->Visible=1;
   //сделал сначала рисовка по выбранному режиму с переключение
   //и только при выборе РАССЧИТАТЬ запрос норм ( форма 8)
   //но как не в автомате расчёт??
// else {Form1->Close(); Form1->Release();return;}
     if (FromFile())
  { //получение данных из файла  съёмки .igl
    if(!regime){SizeKofX=2.0;SizeKofY=300.0;
   //            Xmin=0.; Xmax = MPoint[QPoint-1].x1;
    //           Ymin=-9.; Ymax =9; //лучше после поиска Ymin,Ymax
              Form1->Caption=" Углограмма";
              }//углограмма
     else
     { SizeKofX=2.0;SizeKofY=2.0;
       Form1->Caption=" Расчёт начального приближения по координатам  Х,У";
     }//XY
     //fprintf(ff,"\nregime=%d,Xmin=%7.2lf,Xmax=%7.2lf,Ymin=%7.2lf,Ymax=%7.2lf",
       //              regime,Xmin,Xmax,Ymin,Ymax);
      Xmin=MPoint[0].x1;  Xmax=MPoint[0].x1;
       Ymin=MPoint[0].y1;  Ymax=MPoint[0].y1;
       for (int i=1;i<QPoint;i++)
        { //fprintf(ff,"\ni=%d,X=%7.2lf,y=%7.2lf",i,
        //MPoint[i].x1,MPoint[i].y1);
          if( Xmin>MPoint[i].x1) Xmin=MPoint[i].x1;
          if( Ymin>MPoint[i].y1) Ymin=MPoint[i].y1;
          if( Xmax<MPoint[i].x1) Xmax=MPoint[i].x1;
          if( Ymax<MPoint[i].y1) Ymax=MPoint[i].y1;
        }//for
     ToForm();  //вывод значений по умолчанию на форму
     ReDraw(); //отрисовка углограммы или графика в координатах
   }

  else
  {  ShowMessage("Не прочитан файл");
        Form1->Close();
        Form1->Release();
     return;
  }
  return;
    }

//---------------------------------------------------------------------------

void __fastcall TForm1::Button2Click(TObject *Sender)
{  //Смена режима

  delete []ExPoint;delete []PMPoint; delete[] MPoint;
    delete []VUPoint;
    delete []xt;delete[] yt; delete []x;
    delete []kriv;delete []ugls;

 TForm1::FormShow(Sender);
 return;
/*   if(MessageDlg("Продолжить работу?",
 mtConfirmation, TMsgDlgButtons() << mbYes<< mbNo,0) == mrYes)
  {//Form4->ShowModal(); // не было

 //   Form1->Show();  не работает без Hide
   return;
   }
     fclose(ff);
 remove(elt_name);
Form1->Close();
Form1->Release();
*/
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ButCenterClick(TObject *Sender)
{
  // ButCancel->Visible  = ButCenter->Down;
 }
//---------------------------------------------------------------------------

void __fastcall TForm1::FildMouseUp(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
  MousePressed = false;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ButCancelClick(TObject *Sender)
{ // В начало
    xg0 = 20 ; yg0 = 0;
    ReDraw();
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

void __fastcall TForm1::Button3Click(TObject *Sender)
{
//Кнопка Рассчитать на Форм1; нормы читать в любом режиме
//colt-число точек обнуляется при чтении углограммы
 ne=0; // число построенных элементов
FILE *f4;
  f4=fopen("Normy.txt","wt");

if (regime!=2)
{ Form2->Edit11->Visible=0;
  Form2->Label13-> Visible=0;
} else
    { Form2->Edit11->Visible=1;
  Form2->Label13-> Visible=1;
}
Form2->ShowModal(); //нормы из формы 2 всегда
 if(Form2->ModalResult==mrOk)
 {
  lmin= Asd(Form2->Edit3->Text);
  lmax= Asd(Form2->Edit4->Text);
  lkmin=Asd(Form2->Edit2->Text);
  Rmin=  Asd(Form2->Edit1->Text);
  Rmax=  Asd(Form2->Edit5->Text);
  lprmin=Asd(Form2->Edit6->Text);
  dH=Asd(Form2->Edit7->Text);//dH max откл от прямой
 if (regime==2) Lmax=Asd(Form2->Edit11->Text);
  Lmin=lmin+lkmin+lmin+lprmin;
  fprintf(f4," %7.3lf %7.3lf %8.3lf %8.3lf %7.3lf %7.3lf",
  lmin,lmax,Rmin,Rmax,lkmin,lprmin);
  fclose(f4); }else
  {ShowMessage("Без записи норм расчёт невозможен");
   return;
   }
 if (regime==2)
  {
//  ShowMessage("Расчёт автоматический");
 Form6->ShowModal();
 if(Form6->ModalResult==mrOk)
 {
  npr=StrToInt(Form6->Edit7->Text);
  nug=StrToInt(Form6->Edit8->Text);
  dpr= Asd(Form6->Edit1->Text);
  dug= Asd(Form6->Edit10->Text);
  maxrix=Asd(Form6->Edit12->Text);
 //  параметры перебора с панели
 //не использую но могут понадобиться
 //  dL=Asd(Form6->Edit15->Text);
  // kdL=Asd(Form6->Edit16->Text);
  // kdR=Asd(Form6->Edit17->Text);
   } else return;
   btnSave->Visible=false;
          pech();
     Form1->Close();
    exit(0);
 //return;
  }
  else //не автомат
 {  //colt-число точек обнуляется при чтении углограммы
/*
 colt=18;//colt=14;
 VUPoint[0]=0;
 VUPoint[1]=1;//xt=   40.0729,yt=   -0.0087
 VUPoint[2]=19;//,xt=  300.4121,yt=   25.5352
 VUPoint[3]=22;//,xt=  456.3918,yt=   49.6637
 VUPoint[4]=32;//,xt=  575.5325,yt=   66.3128
 VUPoint[5]=40;//,xt=  674.5451,yt=   79.7123
 VUPoint[6]=58;//,xt=  842.0834,yt=  139.0122
 VUPoint[7]=60;//,xt=  918.8589,yt=  203.7843
 VUPoint[8]=74;//,xt= 1038.0420,yt=  310.3700
 VUPoint[9]=76;//,xt= 1142.6788,yt=  403.2884
 VUPoint[10]=103;//,xt= 1554.3781,yt=  809.2446
 VUPoint[11]=105;//,xt= 1650.8179,yt=  944.8442
 VUPoint[12]=128;//,xt= 1830.4815,yt= 1118.4144
 VUPoint[13]=131;//,xt= 1898.1816,yt= 1166.3721
 VUPoint[14]=138;VUPoint[15]=140;
 VUPoint[16]=153;VUPoint[17]=155;
  */
  if(!colt){ShowMessage("Точки не заданы");return;}
 if(colt%2)
  {ShowMessage("Число точек должно быть чётным");return;}
int i;
i=obrVUPoint();
 if(i){ShowMessage(" ВУ не рассчитаны. Ошибки.");}
  else
    {
    TForm1::btnSaveClick(Sender);
     }//запись
   }// не автомат
 return;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::btnDeleteClick(TObject *Sender)
{
colt=0;
btnDelete->Visible=false;
ReDraw();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FildClick(TObject *Sender)
{
  double s,s1;  int j=0;
  double s2,s3;
  for(int i=0;i<QPoint;i++)
  {
  // s=sqrt((PMPoint[i].x-x1)*(PMPoint[i].x-x1)+
   //(PMPoint[i].y-y1)*(PMPoint[i].y-y1));

    s2=PMPoint[i].x-x1;s2=s2*s2;
    s3=PMPoint[i].y-y1;s3=s3*s3;
  //  fprintf(ff,"\n i=%d,s2=%15.1f,s3=%15.lf",
    //               i,s2,s3);
    s=sqrt(s2+s3);

  if(i==0)s1=s; else if(s<s1){s1=s;j=i; continue;}
  }//for

//if(fabs(kriv[j])>1.e-6)
//{Edit1->Text=FloatToStr(1.0/kriv[j]);}else
//{Edit1->Text="Прямая";}
  if(CheckBox1->Checked) //радиус показать
  { if(regime)j=j-1;
  //давал радиус в правой а не средней точке
  // а в углограмме и так было верно верно
      if(fabs(kriv[j])>1.e-6)
Edit1->Text=FloatToStr(1.0/kriv[j]);else
Edit1->Text="Прямая";
   } else  Edit1->Text=FloatToStr(ugls[j-1]);

 // }//for
return;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::CheckBox1Click(TObject *Sender)
{
 if (CheckBox1->Checked)CheckBox1->Caption="Показать радиус ";
    else  CheckBox1->Caption="Показать угол";
}
//---------------------------------------------------------------------------
  void __fastcall TForm1::Button1Click(TObject *Sender)
{     //В конец
 xg0=0; yg0=0;//надо! xg0 участвует в Xp
 xg0= -Xp(MPoint[QPoint-1].x1)+150;
  yg0=-Yp(MPoint[QPoint-1].y1)+Fild->Height/2;
  //всё сдвинуто влево по горизонтали осталось 150 пикселей
   ReDraw();
}
