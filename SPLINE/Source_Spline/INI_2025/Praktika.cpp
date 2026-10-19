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


//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
 TForm1 *Form1;
 double krt (int j);
extern char igl_name[40];
extern char ini_name[40];//insted of fortxy
///
//extern double spk,spp;//минимальные прямые вставки при стыковке клотоид
  double lfirst;//добавил
 int n;//

 double* kriv, *ugls;  //для выдачи в окно

DTPoint *ExPoint; //на массив из 1-го и 2-го столбца файла
TPoint *PMPoint; //массив исходный точек в пикселях
float xA; //xA- глобальная переменная -начальная абсцисса
long pkt; float plt;
double SizeKofX,SizeKofY;
 //SizeKofY=200.0; //коэффициенты масштаба по Х и У
//double Xmin=0, Xmax=1100, Ymin=-1.5, Ymax=1.5;
double Xmin, Xmax, Ymin, Ymax;
//Xmax далее вычисляется при вводе как длина трассы
//bool isDbClick = false; //флаг двойного щелчка. Если 0 - не было двойного нажатия, 1 - было
int x0; //координата мыши для перемешения
int napr;double  shag,Lsh;// расст от начала после шага
double *x;//В схема это текущая сумма хорд==длина до  переломов
//использую при шагах  по графику координат
//napr 1 -шаги по длине трассы  вправо в км,-1-влево
  struct rp1
  { long pk;
    float pl;
    double km;
  }rp[50];
   int krp;/*число точек в RP, т.е. номер последней krp-1*/
 extern int km_to_pic(double km,long *pic,float *plus);
 extern void pic_to_km(  long pic, float plus,double *km);

//int *VUPoint; //массив исходный номеров точек  для ВУ
 double *xt,*yt;
   //
double rast(double x1,double y1,double x2,double y2);
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
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{
}
extern FILE*ff;
/************************************************
Основная функция отрисовки изображения на форме
********************************************************/
   void TForm1::ReDraw()
{
   int i,j; //счетчик
  // double SizeKofX=2.0,SizeKofY=300.0; in Form1->Show()
  // if(Xp(Xmax)>4096 )SizeKofX=4076.0/Xmax;
   //задаем ширину и высоту области графика
//  Fild->Width = QPoint*20*SizeKofX+40;
  Fild->Width = QPoint*20*SizeKofX;

  for (i=0;i<QPoint;i++)   //перевод координат точек углограммы в пиксели
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
   int jx=14; double ch; char st[10],st1[6];
   j0=Fild->Height/2; j1=20;
//   fprintf(ff,"\nxg0=%d,yg0=%d,H=%d",xg0,yg0,Fild->Height);
//fprintf(ff,"\nYp(0)=%d, Xp(Xmin)=%d, Xp(Xmax)=%d,Yp(Ymin)=%d, Yp(Ymax)=%d",
  //            Yp(0),Xp(Xmin), Xp(Xmax),Yp(Ymin), Yp(Ymax));

   Fild->Canvas->MoveTo(j1,Panel1->Top);
   Fild->Canvas->LineTo(j1,Fild->Top); //вертикаль на х=20от 939 до 39

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
 double xt,xtris,xx; int pktris,ost;
  if(!regime)//uglogramma
  {
   pkt=(long)ExPoint[0].x1;
   plt=ExPoint[0].y1;
       sprintf(st1,"%3.0f",plt);
    // sscanf(st1,"%5.2f",&plt);
  xtris=0.0;;

   pic_to_km(pkt,plt,&xt);//начальная абсцисса
   xA=xt;//xA- глобальная переменная -начальная абсцисса
 //разбивка 20, но нумерация пикетажа в увязке с RP1.lst
//реальный пикетаж не использует поэтому остаток всегда ноль плохо!!
   while (xtris<=Xmax) //Xmax -длина трассы а не XMAX
     {
   // Fild->Canvas->MoveTo(Xp(xtris),Yp(0)-5);
    Fild->Canvas->MoveTo(Xp(xtris),j0-5);
    //рисование делений на оси Х
     //Fild->Canvas->LineTo(Xp(xtris),Yp(0)+5);
     Fild->Canvas->LineTo(Xp(xtris),j0+5);
    // if((pkt>100)&&(xtris>0.1)) pktris=pkt%100;
     //  else  pktris=pkt;
       if(!(pkt%100)&&plt<0.001) pktris=pkt; else  pktris= pkt%100;
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
              {
               // j0 Yp(0)
Fild->Canvas->TextOutA(Xp(xtris-0.1)-10,j0-20,IntToStr(pktris)+'+'+st1);//FloatToStr(plt));

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
       Fild->Canvas->TextOutA(Xp(xtris-0.2),j0+15,st1);//FloatToStr(plt));

            }
          xt=xt+20.0; xtris=xtris+20.0;
        }//end не начало
 // отрисована точка и подготовлена следущая но её надо смотреть на пикетаж
    km_to_pic(xt,&pkt,&plt);
    sprintf(st1,"%3.0f",plt);
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
    sprintf(st1,"%3.0f",plt);
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
   else //  по координатам XY
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
  //         xg0,yg0,SizeKofX,SizeKofY,Fild->Height/2);
   //начертить ломаную xg0,yg0 меняются

   Fild->Canvas->MoveTo(PMPoint[0].x,PMPoint[0].y);
   Fild->Canvas->Polyline(PMPoint,QPoint-1);

 //отметить исходные точки
  Fild->Canvas->Brush->Color=clRed; //для эллипса

     for (i=0;i<QPoint;i++)
     {

Fild->Canvas->Ellipse(PMPoint[i].x-2,PMPoint[i].y-2,PMPoint[i].x+2,PMPoint[i].y+2);
 //Fild->Canvas->Rectangle(PMPoint[i].x-2,PMPoint[i].y-2,PMPoint[i].x+2,PMPoint[i].y+2);
      }

   //отрисовка границ убрана
   /*
    Fild->Canvas->Pen->Color = clBlue;
    Fild->Canvas->Pen->Width = 1;
        */
         //  fclose(ff); //ris.txt
  }//ReDraw

/***************************************************************
получение исходных данных из файла
****************************************************************/
bool  TForm1::FromFile()
{ //rp читаю в WINMAIN
  QPoint = 0;   //член -данное класса
  //colt=0;//число точек для ВУ
      double z1,z2,z3,z4;
 // ff=fopen("otlris.txt","wt");
  ifstream Fin;
  Fin.open(igl_name);//исходная углограмма своя на участке
//пикет, плюс, хорда, угол поворота-Это не углограмма а съёмка
  if(!Fin)   //открыть файл
      {
        ShowMessage ("Не удалось открыть файл исходной съёмки .igl");
       ShowMessage (igl_name);
        return false;
      }
     // получить количество точек
    Fin>>QPoint;
    MPoint=new DTPoint[QPoint+1];
    ExPoint= new DTPoint[QPoint+1];
    PMPoint=new TPoint[QPoint];// перенёс из DRAW()

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
   {    ВЕРНУЛ в рисовку из-за масштабов
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
    Fin.open(ini_name); //число точек и Х,У в строке
  //создаётся при открытии создании участка в кнопке "углограмма"
  if(!Fin)   //
      {
        ShowMessage ("Не открылся файл ini_name ");
  delete []MPoint;
  delete[] ExPoint;delete []PMPoint;//delete[] VUPoint;
        return false;
      }

//    // получить число точек
    Fin>>QPoint;
    xt=new double[QPoint+1];
    yt=new double[QPoint+1];
   // тип TPoint в системе
    for (j=0;j< QPoint;j++)
    {  //заполняет лишний j=Qpoint.,надо -1
     Fin>>z3>>z4;//>>z1;  //x,y,нет beta
               xt[j]=z3;
               yt[j]=z4;
    }//for
     Fin.close();
// при работе в координатах  меняю MP через xt,yt  чтобы нарисовать
    if(regime)
  {  //не углограмма  будем высвечивать график в координатах
   // SizeKofX=2.0;  SizeKofY=30.;;

   //   ini_name уже выставлен на начальный нуль
  for (j=0;j< QPoint;j++)
     {
     MPoint[j].x1=xt[j]-xt[0];  //
     MPoint[j].y1=yt[j]-yt[0];
      //fprintf(ff,"\nj=%d, mx1=%10.2lf, my1=%10.2lf",
       // j,MPoint[j].x1,MPoint[j].y1);//otlris.txt <-
     }//for
      //Xmax = MPoint[QPoint-1].x1; x может уменьшаться
  }//rezime !=0
                 cxema();
   //снова читает съёмку: хорда+угол,вычисляет абсциссы, сумм. угол и кривизны
          //} rezime==0

   return true;
}

/****************************************************************
вывод значений по умолчанию на форму
*****************************************************************/
 void TForm1:: ToForm()
 {
   Lsh=0.;
 //расст от начала по ломаной после последнего шага
    napr=1;
   shag=Asd(Edit2->Text);
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
Обработка события двойного нажатия клавиши мыши  убрана
**************************************************************/

/***********************************************************
Обработка события нажатия клавиши мыши
Возможно -перемещение, если стоит флаг isMove = true
-добавление границы, если был двойной щелчек (флаг isDbClick = true;)
-активания границы для редактирования
**************************************************************/

void __fastcall TForm1::FildMouseDown(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
   MousePressed = true;
   xprev = X;  yprev = Y;
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

//if(MessageDlg("Продолжить работу?",
  // mtConfirmation, TMsgDlgButtons() << mbYes<< mbNo,0) == mrYes)    //	  выход
   fclose(ff);
    delete []ExPoint;
    delete[] PMPoint;
    delete[] MPoint;
  //  delete[] VUPoint;
    delete []kriv;delete []ugls;
    delete[] xt; delete []yt; delete []x;
Form1->Close();  Form1->Release();
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
/*************************************************************************
Изменение маштаба по у
**************************************************************************/
 void __fastcall TForm1::ButSizeYClick(TObject *Sender, TUDBtnType Button)
{
   double d;
   if(!regime)d=10.0; else d=0.2;//0-углограмма
if (Button==btNext)
 {  SizeKofY = SizeKofY+d; }
 if ((Button== btPrev)&&(SizeKofY-d>0.00001) )
 { SizeKofY = SizeKofY-d; }
   SizeY->Text = SizeKofY;
   ReDraw();
}
/*********************************************************************
Вызов инструкции
**********************************************************************/

void __fastcall TForm1::btnRefClick(TObject *Sender)
{
FILE *imd;
imd=fopen("..//imd.txt","rt");
char st[180], put[180];
fscanf(imd,"%s",st);
fclose(imd);
 ShowMessage(AnsiString(st));
 strcpy(put,"..//AkelPad.exe ");
 strcat(put,st);
 strcat(put, "\\INSTR_INI.txt");
 ShowMessage(put);
/*
char put[160]; char curdir[255];
GetCurrentDirectory(128,curdir);
//посылает текущую в curdir
 ShowMessage("текущая");
 ShowMessage(curdir);
 strcpy(put,"..//AkelPad.exe ");
 strcat(put, " ..//INSTR_INI.txt");
 ShowMessage(put);
*/
 //vizov(put);
  WinExec(put,SW_SHOW);
 return;


}

/*********************************************************************
Изменение размера формы
**********************************************************************/

void __fastcall TForm1::FormResize(TObject *Sender)
{ //похоже не вызывается

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
     if (FromFile())
  { //получение данных из файла  съёмки .igl
    //  ToForm();  //вывод значений по умолчанию на форму

  if(!regime){SizeKofX=2.0;SizeKofY=300.0;
   //            Xmin=0.; Xmax = MPoint[QPoint-1].x1;
    //Ymin=-9.; Ymax =9; //лучше после поиска Ymin,Ymax
              Form1->Caption=" Углограмма";
              }//углограмма
     else
     { SizeKofX=2.0;SizeKofY=2.0;
       Form1->Caption=" Трасса по координатам  Х,У";
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
  {  ShowMessage("FormShow. Не прочитан файл углограммы или ХУ" );
        Form1->Close();
        Form1->Release();
     return;
  }
  return;
    }
//---------------------------------------------------------------------------

void __fastcall TForm1::Button2Click(TObject *Sender)
{  //Смена режима
 //fclose(ff);
  delete []ExPoint;
    delete[] PMPoint;
    delete[] MPoint;
  //  delete[] VUPoint;  //убрать его это ВУ вручную
    delete[] xt;
    delete []yt; delete []x;
    delete []kriv;delete []ugls;
TForm1::FormShow(Sender);
//Form1->Hide();
//Form1->Show();
   return;
  }
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

void __fastcall TForm1::FildMouseUp(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{  MousePressed = false; }
//---------------------------------------------------------------------------

void __fastcall TForm1::ButCancelClick(TObject *Sender)
{        //кнопка В начало
    xg0 = 20 ; yg0 = 0;
    Lsh=20.;
    ReDraw();
}
void __fastcall TForm1::Button1Click(TObject *Sender)
{     //В конец
  xg0=0; yg0=0; //надо использую в Xp
 xg0= -Xp(MPoint[QPoint-1].x1)+250;
  yg0=-Yp(MPoint[QPoint-1].y1)+Fild->Height/2;
 //xg0=-Xp(Xmax)+50;
 //всё сдвинуто влево по горизонтали осталось 50 пикселей
 //yg0= 0; // yg0=-Yp(Ymax);
 Lsh=Xmax;
 ReDraw();
}
//---------------------------------------------------
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
   //fprintf(ff,"\n i=%d,s2=%15.1f,s3=%15.lf",  i,s2,s3);
    s=sqrt(s2+s3);

  if(i==0)s1=s; else if(s<s1){s1=s;j=i; continue;}
  }//for

//if(fabs(kriv[j])>1.e-6)
//{Edit1->Text=FloatToStr(1.0/kriv[j]);}else
//{Edit1->Text="Прямая";}
  if(CheckBox1->Checked) //радиус показать
  {    if(regime)j=j-1;
  //давал радиус в правой точке а в углограмме верно
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
void __fastcall TForm1::CheckBox2Click(TObject *Sender)
{   napr=1;//шаги в км вправо
 if (CheckBox2->Checked)CheckBox2->Caption="Шаги вправо в км ";
    else { CheckBox2->Caption="Шаги влево в км";napr=-1;}
}


//---------------------------------------------------------------------------

 void cxema()
 {  FILE *fp,*fu;
double *ugl;
//координаты и прочее     читал в FromFile
// в kriv кривизна в точках начиная с первой и кончая предпоследней
// и нумерация снова с нуля
kriv=new double[n];
 if(kriv==NULL){ShowMessage("Нет памяти для KRIV");return;}
  for(int j=1;j<n-1;j++)
  kriv[j-1]=krt(j);  //это функция даёт кривзну по трём точкам
  // n-2 вершина пойдёт в n-3 кривизну
  // в массиве kriv заполнены  с нулевой по n-3 было по n-2,
  //в конце была хорда и после неё угол
  kriv[n-2]=0.0;

  fp=fopen("testugl.txt","wt");

for( int i=0;i<n-1;i++)
{ double ss;
if(fabs(kriv[i])>1.e-7)ss=1./kriv[i]; else ss=0.0;
fprintf(fp,"\ni=%d,kriv[i]= %9.6lf,R[i]= %12.2lf", i,kriv[i],ss);
 }
 fclose(fp);

long pk,pkn; float pl, pln;
 // при повторном вызове глобальная инициализация не работает!!!
 //ne=0;

 fp=fopen("testugl_2.txt","wt");

    // igl_name уже читал в FromFile но в другие массивы
 fu=fopen(igl_name,"rt");
   if(fu==NULL){ShowMessage("Нет файла igl_name");  return;}
   fscanf(fu,"%d",&n);

   x= new double[n+1];  // хорды global
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
   for(int i=0;i<n;i++)
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
     for(int i=1;i<n;i++)
          {if(i<n-1) kriv[i]=ugl[i]/((x[i+1]-x[i-1])/2.0);
           else kriv[i]=ugl[i]/(x[n-1]-x[n-2]);
           }
   // fprintf(fp,"\nначало n=%d,x[n-1]=%8.2lf",n,x[n-1]);
           ugls[n]=ugls[n-1];// надо для вычисления конца тангенса
 for(int i=0;i<n;i++)fprintf(fp,"\ni=%d,kriv[i]= %9.6lf,ugl=%9.6lf,ugls=%9.6lf",
i,kriv[i],ugl[i],ugls[i]);
            delete[]ugl;//больше не нужен
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


//---------------------------------------------------------------------------
  double rast(double x1,double y1,double x2,double y2)
 {return (sqrt((x1-x2) *(x1-x2)+(y1-y2)*(y1-y2)));}

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

//---------------------------------------------------------------------------

 void __fastcall TForm1::UpDown1Click(TObject *Sender, TUDBtnType Button)
 {  // всё автоматом из-за привязки к Edit2
  //      стрелки вверх-вниз
 }
 //перевод мировых координат в пиксели для х
//************************************************
  int TForm1::Xp (double x)
 {return (xg0+x*SizeKofX); }
   // зачем +20, это просто отступ оси от края
/************************************************
перевод мировых координат в пиксели для y
*************************************************/
int TForm1::Yp (double y)
 {return (yg0+Fild->Height/2-y*SizeKofY);}
/*******************************************************

Перевод из пикселей в мировые координаты Х
***************************************************************/
 double TForm1::pX(int xp)//double xp
  {return ((xp-xg0)/SizeKofX);}
   // return (xp/SizeKofX);}//зачем -20?

   /*************************************************************/
 double TForm1::pY(int yp)//double yp
{return ((yg0+Fild->Height/2-yp)/SizeKofY);}
 //---------------------------------------------------------------------------

void __fastcall TForm1::Button3Click(TObject *Sender)
{//ШАГИ
 //ButCancel->Visible  = ButCenter->Down;
 double s1,s2,s3;  int j,k;
 shag=Asd(Edit2->Text);
  if(!regime)//углограмма  шаги от текущего положения
  {
  j=shag*1000.*SizeKofX;
 //на столько пикселей будет сдвиг  по X т.е.длине
 //как найти yg0,чтобы видеть график?
  if(napr>0)j=-j;
  xg0=xg0+j;
  if((j>0)&&(Xp(Xmin)<20)||(j<0&&(Xp(Xmax)>20)))
   ;
  else { ShowMessage("Ошибка. Шаг за границу");return;}
  //Иначе по Xmax нечего смотреть,а по Xmin пусто до Xmin
  s1=pX(0);// что теперь в нуле по X это минус xg0 в метрах
    k=0;
   s3= fabs( MPoint[0].x1-s1);
     for (int i=1;i<QPoint; i++)
    {  s2= fabs( MPoint[i].x1-s1);
     if(s2<0.001){k=i; break;};
     if(s2<s3){s3=s2; k=i;}
    } //for
      s1=MPoint[k].y1; // Это y при x в  нуле
    j=s1*SizeKofY;//это в пикселях
  yg0= j;//точку в центр экрана
  ReDraw();
   return;
 }//конец по углограмме
 //Lsh-длина от начала после последнего шага
  //новые шаги от неё    она объявлена сверху double
//Lsh=Lsh+shag*1000.*napr;  нельзя текущую делать недопустимой
 //   if(Lsh<0. || Lsh>x[QPoint-2])
s1=Lsh+shag*1000.*napr;
if(s1<0. || s1>x[QPoint-2])
 {ShowMessage("Шаг за границу по длине");
 return;}
 Lsh=s1;
    j=0;// по ней и массиву x найду точку
     for (int i=0;i<QPoint-1; i++)
     {
    if(x[i]>Lsh-0.01)
        {j=i+1; break;}
     }//for;  x[0]-длина первой хорды
  //x[Qpoint-2]сумма длин   номер точки
  //на 1 больше номера хорды
    xg0=-MPoint[j].x1*SizeKofX; // в пикселях
    yg0=MPoint[j].y1*SizeKofY;
    ReDraw();
   return;
   }
//---------------------------------------------------------------------------
    /***********************************************************
  Обработка события перемещения мыши
 ***********************************************************/
 void __fastcall TForm1::FildMouseMove(TObject *Sender,
                TShiftState Shift,int X, int Y)
 {  int j,k;

  //  if( ButCenter->Down && MousePressed )
    if( MousePressed )
 {
 j=Xp(Xmin);   k=Xp(Xmax);
   if(((X>xprev)&&(j<20))||((X<xprev)&&(k>20)))
 // X>xprev-пойдём вправо и и при j>20 в нуле будет пусто;
 //левее Xmin ничего нет
 {  // fprintf(ff,"\nj=%d,k=%d,xg0=%d,X=%d,xprev=%d",j,k,xg0,X,xprev);
   //fprintf(ff,"\nXmin=%7.2lf,Xp(Xmin)=%d",Xmin,Xp(Xmin));
//if( xg0 + X-xprev <= j)
   xg0 = xg0 + X - xprev;  //почему нет скобок? Х по условию а Y нет
   }
   yg0 = yg0  + Y - yprev;
    //Почему сдвиг по У без проверки по мин и макс???
   ReDraw();
   xprev = X; yprev = Y;
  }
   Form1->x1 = X;  //считываем координаты мыши на поле графика
   Form1->y1 = Y; //добавил надо ли?
}
