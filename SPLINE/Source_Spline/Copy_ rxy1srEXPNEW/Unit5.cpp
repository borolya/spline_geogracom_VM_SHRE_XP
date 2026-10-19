//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "extern.h"
#include "Unit5.h"
#include <math.h>
#include <stdio.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm5 *Form5;
  int inah=0,ikon; /* для элемента расчета, т.е. простой или составной кривой*/
  int j=0;// признак отсутствия нарушений
  AnsiString Par1;
  AnsiString Par2;


//---------------------------------------------------------------------------
__fastcall TForm5::TForm5(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm5::Button1Click(TObject *Sender)
{ Button1->Caption="Продолжить";

   /* ===========Проверка скоростей========================    */
  int i;//,i1,k1;
  char str[80],str1[80];
  double s;
  double vpas,vgruz,h0,hmax,amin,amax,k,h,a,l,lmin;
  double hdmax; /*ввел для увеличения h при грузовом если резерв есть*/
  double rmin,rmax,r,r1,r2;

//int elts_changed=1; /*для записи возвышений*/
 vpas=Asd(Edit1->Text); vgruz=Asd(Edit2->Text);
 h0=Asd(Edit3->Text); hmax=Asd(Edit4->Text);
 amin=Asd(Edit5->Text); amax=Asd(Edit6->Text);
 k=Asd(Edit7->Text);
 //Исходные нормативы до начала корректировки и после

     while (TRUE)
  {

 ikon=inah;
 r=elts[inah].rad; /* r1 cо знаком останется с min клотоидой только для выдачи*/
 r1=r;
 r2=r;
 rmin=fabs(r);
 rmax=rmin;
 lmin=elts[inah].l_clot;
 if(lmin<0.01) lmin=2000;

 while ( (ikon<n_elts-1) && (fabs(elts[ikon+1].rad)>0.1))
 { ikon++;
    if(fabs(elts[ikon].rad)< rmin) { r=elts[ikon].rad; rmin=fabs(r);}
   if(fabs(elts[ikon].rad)>rmax) { r2=elts[ikon].rad; rmax=fabs(r2);}
 /*  if((cur->l_clot>0.01) && (cur->l_clot< lmin))
   {
    lmin=cur->l_clot;
    r1=cur->rad;
   } на внутренних переходных одно и тоже возвышение!!
Учитываем только концевые переходные. Участок до прямой!!!
 */
 }   /*Это на составной нашли минимальный и максимальный радиусы*/

  h=12.5*vpas*vpas/rmin+h0;
/*  fprintf(fp1,"\n vpas=%7.2lf,rmin=%7.2lf, hдо=%7.2lf,lmin=%7.2lf,r1=%7.2lf",
  vpas,rmin,h,lmin,r1);
  */
  if(h<1.)
  {        j=1;
  if(MessageDlg("h<1мм Ошибка в Vпасс или h(0)! Исправить-Yes,Продолжить-No",mtWarning,TMsgDlgButtons() << mbYes<<mbNo,0)==mrYes)
    return;
    h=0.0; //Проверять ускорения надо и при нулевом h!!!
   }
   s=h/10; i=s;s=s-i;
   if(s<0.1) h=i*10; else {if(s<=0.5) h=i*10+5; else h=(i+1)*10;}
/* fprintf(fp1,"\n hpo=%7.2lf,s=%7.2lf, i=%d",h,s,i);*/
  if(h>hmax+0.01)
  {
sprintf(str,"%6.2lf",h);
sprintf(str1,"%6.2lf",rmin);
// ShowMessage("Превышено hmax= "+ Edit4->Text+ "; h="+ AnsiString(h)+"; R= "+AnsiString(rmin));
  ShowMessage("Превышено hmax= "+ Edit4->Text+ "; h="+ str+"; R= "+str1);
 s=(hmax-h0)*rmin/12.5;
      s=sqrt(s);
      sprintf(str,"%6.2lf",s);
 ShowMessage("Уменьшить Vпас<=" +AnsiString(str) + " или пересчитать план");
      j=1;
      return;
  } /* h>hmax*/
      hdmax=hmax;
      /*добавить  правую клотоиду !!!*/
    if((ikon<n_elts)&& (elts[ikon+1].l_clot>0.001)&&(elts[ikon+1].l_clot<lmin))
    {
     lmin=  elts[ikon+1].l_clot;//правая короче левой
     r1=elts[ikon].rad;//для выдачи информации о короткой клотоиде
    // ikon++;//Надо ли??? Нет См ниже
    }
     if(h*k>lmin+0.001)
  /* проверка возвышения на самой короткой клотоиде если она есть*/
   { /*fprintf(fp1,"\nh=%10.6lf,k=%7.4lf,lmin=%10.6lf",h,k,lmin);*/

      s=h*k;
      sprintf(str,"%6.2lf",s);
      strcat(str,"; Есть ");
      sprintf(str1,"%6.2lf",lmin);
      strcat(str,str1);
      sprintf(str1,"%7.2lf",r1);
//ShowMessage("Короткая переходная. Надо "+AnsiString(h*k)+ " Есть"+AnsiString(lmin)+" R="+AnsiString(r1));
ShowMessage("Короткая переходная. Надо "+AnsiString(str)+ "; R="+ AnsiString(str1));
      s=(lmin/k-h0)*rmin/12.5; /*s-квадрат скорости*/
      s=sqrt(s);
      sprintf(str,"%6.2lf",s);
ShowMessage("Уменьшить Vпасс<="+AnsiString(str)+" или пересчитать план");
      j=1;
     return;//возврат на форму для исправлений или ухода
   }
     if(lmin/k<hdmax) hdmax=lmin/k; /* это проходит но больше нельзя!!*/

   /* с переходными все теперь грузовой*/

    a=vgruz*vgruz/13/rmax-0.0061*h;
/*   fprintf(fp1,"\n inah=%d,rmax=%7.2f,vgruz=%6.2f,h=%6.2f,a=%9.5f,amin=%9.5f,amax=%9.5f",
    inah,rmax,vgruz,h,a,amin,amax);
  */
    if(a<amin-0.0001)
    {
sprintf(str,"%8.2lf",a);
sprintf(str1,"%7.2lf",r2);
//ShowMessage( "Aнп<amin="+ AnsiString(amin)+" Aнп="+AnsiString(a)+" R="+AnsiString(r2));
ShowMessage( "Aнп<amin="+  Edit5->Text+" Aнп="+AnsiString(str)+" R="+AnsiString(str1));
 	s=13.0*rmax*(amin+0.0061*h);
 if(s<0){ShowMessage("Ошибка в данных: Vпасс, h(0) или amin"); return;}
        s=sqrt(s);
        sprintf(str,"%7.2lf",s);
ShowMessage( "Скорость груз. поезда должна быть >="+AnsiString(str));
j=1;
return;
    }     /*a<amin*/
   a=vgruz*vgruz/13/rmin-0.0061*h;
/*   fprintf(fp1,"\n inah=%d,rmin=%7.2f,vgruz=%6.2f,h=%6.2f,a=%9.5f,amin=%9.5f,amax=%9.5f",
    inah,rmin,vgruz,h,a,amin,amax);
 Увеличение возвышения уменьшает Анп!!!!, но
увеличивать можно только до hdmax за клотоиду и Hmax
*/
    if(a>amax+0.0001)
   {
      if((vgruz*vgruz/13.0/rmin-amax)/0.0061>hdmax+0.00001)
     {
       sprintf(str,"%8.2lf",a);
       sprintf(str1,"%8.2lf",rmin);
ShowMessage("Aнп>amax="+ Edit6->Text+" Aнп="+AnsiString(str)+" R="+AnsiString(rmin));
       s=13.0*rmin*(amax+0.0061*hdmax);
  if(s<0) {ShowMessage("Ошибка в данных: Vпасс,h(0) или amax"); return;
           }
       s=sqrt(s);
       sprintf(str,"%7.2lf",s);
       ShowMessage("Скорость груз. поезда должна быть <="+AnsiString(str));
             j=1;
       return;
      } else { h=(vgruz*vgruz/13.0/rmin-amax)/0.0061;
                s=h/10; i=s; s=s-i;
	      if(s<0.1) h=i*10; else { if(s<=0.5) h=i*10+5; else h=(i+1)*10;}
	     }
    }   /* a>amax*/
 /* конец проверки ускорения грузового поезда
    надо все расписать и продолжить*/

       for (int i=inah;i<=ikon;i++)
     {
      elts[i].h=h; elts[i].vpas=vpas; elts[i].vgruz=vgruz;
     }
     if(ikon<n_elts)
     {ikon++;elts[ikon].h=0.0; elts[ikon].vpas=vpas;elts[ikon].vgruz=vgruz;
     }
      inah=ikon+1;
     if(inah>=n_elts)break;
Edit1->Text=Par1;
Edit2->Text=Par2;
 //Убрать это восстановление скоростей
 //и будет по минимальным скоростям далее без возврата
      }  /* это внутренний while */

  /* break сюда*/
    Edit1->Text=Par1;
    Edit2->Text=Par2;
  if(j==0)
{
   ShowMessage ("Несоответствия плана и скоростей не обнаружено");

}   //j==0
ShowMessage ("Требуемые скорости и  план не согласованы");
Form5->Close();
 Button1->Caption="Начать проверку";
return;
} //end Button1Click

//---------------------------------------------------------------------------

void __fastcall TForm5::Button2Click(TObject *Sender)
{
Edit1->Text=Par1;
Edit2->Text=Par2;
Button1->Caption="Начать проверку";
ModalResult=mrOk;
Form5->Close();

}
//---------------------------------------------------------------------------

void __fastcall TForm5::FormCreate(TObject *Sender)
{
Par1=Edit1->Text;
Par2=Edit2->Text;
}
//---------------------------------------------------------------------------

