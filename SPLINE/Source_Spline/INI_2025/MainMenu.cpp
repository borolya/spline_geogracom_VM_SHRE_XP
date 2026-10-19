//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "MainMenu.h"
#include "Praktika.h"
#include <stdio.h>
#include <math.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm4 *Form4;
//extern double lmin,lmax,lkmin,lprmin,Rmin, Rmax;//по длинам клотоиды и окружности
//extern double rast(double x1,double y1,double x2,double y2);

//---------------------------------------------------------------------------
__fastcall TForm4::TForm4(TComponent* Owner)
        : TForm(Owner)
{

}
//---------------------------------------------------------------------------
void __fastcall TForm4::Button1Click(TObject *Sender)
{
//Form3->ShowModal();  //инструкция

         Form1->regime = 0;
        Form4->Close();

}
//--------------------------------------------------------------------

void __fastcall TForm4::Button2Click(TObject *Sender)
{
 //ShowMessage (" Трасса по координатам Х,У");
     Form1->regime = 1;
     Form4->Close();

}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------

void __fastcall TForm4::Button4Click(TObject *Sender)
{       //     выход
      Form1->regime = 3;
      Form4->Close();
      Form4->Hide();

}

//---------------------------------------------------------------------------
extern struct rp1
  { long pk;
    float pl;
    double km;
  }rp[50];
extern int krp;
  int read_rp()
  {
    FILE *rpfile;
  rpfile=fopen("..//RP1.lst","rt");//"..//rp1.lst" читать  сверху
  if(rpfile==NULL)
    {
    ShowMessage("Не задано соответствие пикетажа.ENTER");
    return 1;
    }
  int i=0;  double s;
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
  return 0;
 } //read_rp
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

