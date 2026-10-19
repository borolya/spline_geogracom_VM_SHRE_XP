#include <vcl.h>
#include <stdio.h>
#include <math.h>
#include "UI.h" //chainage здесь
double phifin;
   extern int i,j;
    double step=20.0;
/*   typedef struct
 {
  long chain;
  double plus;
 } CHAINAGE;
  */
  struct rp1
  { long pk;
    float pl;
    double km;
  }rp[50];
      int krp;/*число точек в RP, т.е. номер последней krp-1*/
int calc_arc (TRACE_PTS *cur_pts, double *sig, double *arc1);

   int km_to_pic(double km,long *pic,double *plus);
   int pic_to_km(  long pic, double plus,double *km);
   //+++

 //+++
   int count_chain( CHAINAGE ch1,  double dist, CHAINAGE *ch2 )
{
   double km;               //    int km_to_pic(double km,long *pic,float *plus);

   char buf[40];
   //float y;
   double y;
   if (pic_to_km(ch1.chain,ch1.plus,&km))
   {
     sprintf(buf,"%6ld+%3.0lf",ch1);
     strcat(buf," за пределами перегона");
     ShowMessage(buf);
    return(1);
   }
   km=km+dist;
   if( !km_to_pic(km,&(ch2->chain),&y)) {ch2->plus=y; return( 0 );}
   else
  {
      ShowMessage(" count_chain Шаг за пределы перегона!");
      return(1);

   }
}


  int count_dist( CHAINAGE ch1,CHAINAGE ch2,double *dist )
{          //exit(0) когда нельзя преобразовать в километраж
 double km1, km2;
 char buf[40];
 if   (pic_to_km(ch1.chain,ch1.plus, &km1))
  {
     sprintf(buf,"%6ld+%3.0lf",ch1);
     strcat(buf," за пределами перегона ");
     ShowMessage(buf);
    //return(1);
    exit(0);
  }

     if(pic_to_km(ch2.chain,ch2.plus, &km2) )
  {
     sprintf(buf,"%6ld+%3.0lf",ch2);
     strcat(buf," за пределами перегона ");
     ShowMessage(buf);
    //return(1);
    exit(0);
  }
  /*-----
fprintf(file_dbg,"\n CHAIN.C ch1=%ld+%5.2lf   ch2=%ld+%5.2lf  km1=%lf km2=%lf",
			     ch1,ch2,km1,km2);
  -----*/
 *dist = km2 - km1;
  /*(km2<km1)
  {
   ShowMessage("Отрицательное расстояние");
   return(0);
   } else
   */
  return( 0);

}

  int pic_to_km(  long pic, double plus,double *km)
  {
  int  i,con,pp;
  long  a,b;
    if ((pic==rp[krp-1].pk) && (fabs(plus-rp[krp-1].pl)<0.001))
     {
       *km=rp[krp-1].km; return(0);
     }
    pp=0;/*если останется нуль, то не перевели*/
    i=1;
    while (i<krp)
       {
        while (fabs(rp[i].km-rp[i-1].km)<0.001)i++;
          if (i>=krp){
          ShowMessage(AnsiString(krp)+"равные km до"+AnsiString(i)); return(1);
                     }
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
                             return(0);
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
    return(0);
     }
    if (pp==0)
  {
//  ShowMessage("не преобразован pk=%7ld+%5.3f");
  Application->MessageBox(("Не преобразован ПК"+AnsiString(pic)+
   " +"+AnsiString(plus)).c_str(),"pic_to_km",MB_OK | MB_ICONINFORMATION);
  return(1);
   }
   return(0);
    }

   int km_to_pic(double km,long *pic,double *plus)
{
  int   i,j,k;
   double d;
   if ((km<rp[0].km) || (km>rp[krp-1].km))
   {
    //printf("\n %10.2lf за пределами перегона %10.2lf и %10.2lf.ENTER",
      //	       km, rp[0].km,rp[krp-1].km); getch(); exit(0);
        ShowMessage("за пределами перегона"); return(1);
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
int next_round(CHAINAGE ch1,CHAINAGE *ch2, double step)
{// делает плюс кратный шагу после шага вправо

 double km1, km2,km3,km4,dl,pl2;
 CHAINAGE ch3;
 int dir,i,j;
/* FILE *fne;*/
/*if( (fne= fopen("testnex.dbg","wt"))==NULL){putchar(7);exit(0);};*/
  j=0;
 if (pic_to_km(ch1.chain,ch1.plus,&km1))
 {
  /* fprintf(fne,"\nch1=%7ld+%6.2lf",ch1);
   fclose(fne);*/
   ShowMessage("next_round обратились с ch1 вне перегона");
   return(1);
 }
    km2=km1+step;
    //float y;
    double y;
 if( km_to_pic(km2,&(ch2->chain),&y))
 {
   /* fprintf(fne,"\nch1=%7ld+%6.2lf,step=%5.2lf,ch2=%7ld+%6.2lf",
		  ch1,step,(*ch2));
   fclose(fne);*/
   ShowMessage("next_round шаг вне перегона");

 }
 ch2->plus=y;
  if((*ch2).plus<0.005) {/* fclose(fne);*/ return(0);}
 ch3.chain=(*ch2).chain;
 ch3.plus=0.0;
if(  pic_to_km(ch3.chain,ch3.plus,&km3))
 {
  /* fprintf(fne,"\nch3=%7ld+%6.2lf,ch1=%7ld+%6.2lf",ch3,ch1);*/
/*   fclose(fne);*/
   dir=-1;
   j=1;
   ShowMessage("next_round обратились с ch3 вне перегона");
 }

 ch3.chain++;
if(  pic_to_km(ch3.chain,ch3.plus,&km4))
 {
   /*fprintf(fne,"\nch1=%7ld+%6.2lf, ch3=%7ld+%6.2lf",ch1,ch3);*/
/*   fclose(fne);*/
   dir=1;
   j=2;
  ShowMessage("next_round обратились с ch3 вне перегона");
 }
if(j==0)
 {
   dir=1;
   dl=km4-km3;
   if(dl<0.){ dir=-1; dl=-dl;}
  }
  else dl=100;

   i=(int)((*ch2).plus/step+0.001);
   pl2=i*step;
   if((dir<0) && (pl2<(*ch2).plus-0.005)) pl2=pl2+step;
    if(pl2>dl-0.005) { pl2=0.; (*ch2).chain++;}
    (*ch2).plus=pl2;

 if (!pic_to_km((*ch2).chain,(*ch2).plus,&km2) && km2>km1) return(0);
 ShowMessage("next_round встретилась  с ошибкой");
  return(1);

/*fprintf(fne,"\nstep=%16.12lf,km1=%12.5lf,km2=%12.5lf",step,km1,km2);
fprintf(fne,"\nch1=%7ld+%14.10lf,ch2=%7ld+%14.10lf",ch1,*ch2);
fprintf(fne,"\ndir=%d,dl=%7.2lf,i=%d,pl2=%7.2lf,km3=%7.2lf,km4=%7.2lf",
              dir,dl,i,pl2,km3,km4);
fclose(fne);
*/
 // message("next_round,смотри testnex.dbg");
   //exit(0);
}
//======

   int prev_round(CHAINAGE ch1,CHAINAGE *ch2, double step)
{        // делает плюс кратный шагу после шага влево
 double km1, km2,km3,km4,dl,pl2;
 CHAINAGE ch3;
 int dir,i,j;
 //float y;
 double y;
 FILE *fne;
/*if( (fne= fopen("testprev.dbg","wt"))==NULL){putchar(7);exit(0);};*/
  j=0;
  if (pic_to_km(ch1.chain,ch1.plus,&km1))
 {
/*   fprintf(fne,"\nch1=%7ld+%6.2lf",ch1);
   fclose(fne);*/
   ShowMessage("prev_round обратились с ch1 вне перегона");
   return(1);
 }
if(ch1.plus<0.005)
 {
    km2=km1-step;
 //if( from_km_to_ch(ch2,km2))
 if( km_to_pic(km2,&(ch2->chain),&y))

 {
 /*    fprintf(fne,"\nch1=%7ld+%6.2lf,step=%5.2lf,ch2=%7ld+%6.2lf",
		  ch1,step,(*ch2));
*/
  ShowMessage("prev_round шаг вне перегона");
  return 1;
 }
   /*    fclose(fne);*/
   ch2->plus=y;
   return(0);
 }
(*ch2).chain=ch1.chain;
 ch3.chain=(*ch2).chain;
 ch3.plus=0.0;
//if(!  from_ch_to_km(ch3,&km3))
  if (pic_to_km(ch3.chain,ch3.plus,&km3))
   {
/*   fprintf(fne,"\nch3=%7ld+%6.2lf",ch3);*/
/*   fclose(fne);*/
     dir=1;
     j=1;
  ShowMessage("prev_round обратились с ch3 вне перегона");
 }
 ch3.chain++;
//if(!  from_ch_to_km(ch3,&km4))
  if (pic_to_km(ch3.chain,ch3.plus,&km4))

 {
  /* fprintf(fne,"\nch3=%7ld+%6.2lf",ch3);*/
/*   fclose(fne);*/
     dir=-1;
     j=2;
   ShowMessage("prev_round обратились с ch3 вне перегона");
 }
   if(j==0)
  {
   dir=1;
   dl=km4-km3;
   if(dl<0.){ dir=-1; dl=-dl;}
   }
   else dl=100.;
   i=(int)((ch1).plus/step+0.001);
   pl2=i*step;
   if((dir<0) && (pl2<ch1.plus-0.005)) pl2=pl2+step;/*округление было*/
    if(pl2>dl-0.005) { pl2=0.; (*ch2).chain++;}
    if((labs(ch1.chain-(*ch2).chain)<0.001) && (fabs(ch1.plus-pl2)<0.001))
    { pl2=pl2-step*dir; if(pl2<0.) pl2=0.;}/*округления не было*/
    if(pl2>dl-0.005) { pl2=0.; (*ch2).chain++;}/*надо иначе за пикетом!!!*/
    (*ch2).plus=pl2;
  // if (from_ch_to_km(*ch2,&km2) && km2<km1) {/*fclose(fne);*/return(TRUE);}
   if (!pic_to_km(ch2->chain,ch2->plus,&km2)&& km2<km1)
      {/*fclose(fne);*/return(0);}

/*fprintf(fne,"\nstep=%16.12lf,km1=%12.5lf,km2=%12.5lf",step,km1,km2);
fprintf(fne,"\nch1=%7ld+%14.10lf,ch2=%7ld+%14.10lf",ch1,*ch2);
fprintf(fne,"\ndir=%d,dl=%7.2lf,i=%d,pl2=%7.2lf,km3=%7.2lf,km4=%7.2lf",
              dir,dl,i,pl2,km3,km4);
fclose(fne);*/
  ShowMessage("Плохо в prev_round");
   //exit(0);

 return(1);
}

   void evolvents(TRACE_PTS *first  )
{
   TRACE_PTS *trace_pts_curr;
   double x, y, phi,arc,sig;

   for( trace_pts_curr = first, x = 0, y = 0, phi = 0;
	trace_pts_curr != NULL;
	trace_pts_curr = trace_pts_curr->next )
   {
      calc_arc(trace_pts_curr,&sig,&arc);

    /* угол хорды это полусумма углов касательных по концам!!*/
    /*  trace_pts_curr->ev_x = x += trace_pts_curr->dist;
      trace_pts_curr->ev_y = y +=phi*trace_pts_curr->dist;
    */
      trace_pts_curr->ev_x = x += arc;
        trace_pts_curr->ev_y = y += phi*arc;
       phi += trace_pts_curr->angle;

/*fprintf(file_dbg,"\n x=%5.0lf , y=%10.6lf,phi=%10.6lf",x,y,phi);*/
   }
   phifin = phi;

}
int calc_arc (TRACE_PTS *cur_pts, double *sig, double *arc1)
{
 double h1,h2,h3,alpha,phi;
 if (cur_pts->prev==NULL || cur_pts->next==NULL
     ||fabs(cur_pts->angle)< 0.0001 || cur_pts->dist<  0.001)
 {
   *sig=0.;
   *arc1=cur_pts->dist;
   return(0);
 }


 alpha = cur_pts->angle;
 h1=cur_pts->dist;
 h2=cur_pts->next->dist;
 h3=sqrt(h1*h1 + h2*h2 + 2*h1*h2*cos(alpha));

 *sig = 2*sin(alpha)/h3;  /*1/R*/

 phi = 2*asin(h1*(*sig)/2); /*угол поворота касательной на хорде h1*/
 *arc1 = phi/(*sig);       /*дуга  */
 return(0);
}

//==========


