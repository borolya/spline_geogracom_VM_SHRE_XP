//---------------------------------------------------------------------------

#include <vcl.h>//new
#include <math.h>
#pragma hdrstop
#include "UI.h"// extern типы функции указатели переменные
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#pragma package(smart_init)
#define PI 3.1415926//new
TRACE_ELTS * alloc_elts( int );
TRACE_PTS *alloc_pts(  );
CHAINAGE StrToChainage(char *);
     void simple_write_trace(void);
// функции местного применения
char fname[80];

LINK line[MAXPOINTS];
CORNER_POINT_STR *cur_corner,*first_corner,*last_corner;
TRACE_PTS *trace_pts_first,*trace_pts_last;
int n_points;//  в  extern
int n_pts = -1; //почему -1?
double ugl,ugp,uglt,ugpt,xl,xp,xlt,yl,ylt,xpt,yp,ypt;// в extern
int set_ugl;// extern в UI.h
//---------------------------------------------------------------------------
   int read_inp_corners( char *file_name )
{/*читает .crv,делает стандарт, но не записывает его
  line[0].l=0,a line[0].alfa=0 при uglt=100 иначе разность в нуле*/
 //double pi;
   FILE *fp;
   FILE *g;
   FILE *fcor;
   fcor = fopen ("LINE.txt" , "wt" );
   int i,type_survey;
   double a1,a2,x1,y1,x,y;
   long chain;
   double plus;
   //float xmin,xmax,ymin,ymax;
    double xmin,xmax,ymin,ymax;
   long int  n_corners;
//   CORNER_POINT_STR *last_corner;

   first_corner=NULL;
   cur_corner=NULL;
   fp = fopen ( file_name, "rt" );  //.crv

   n_corners=0;
/*fseek(file,n_corners,SEEK_SET);*/

fscanf(fp,"%3d%10lf%10lf",&type_survey,&uglt,&ugpt);
//тип=2, углы в начале и в конце обычно они по 100 в файле crv
// при формировании LINE учтены 100,
//в конце добавил вычисленные uglt,xlt,ylt они станут ugl,xl,yl
while( fscanf(fp,"%ld%lf%lf%lf",&chain,&plus,&x,&y)==4)
{

   last_corner=cur_corner;
   if ( (cur_corner = new CORNER_POINT_STR) == NULL )
	       return -1;
   cur_corner->num=n_corners;
   cur_corner->ch.chain=chain;
   cur_corner->ch.plus=plus;
   cur_corner->x = x; cur_corner->y = y;
   cur_corner->set_x = TRUE; cur_corner->set_y = TRUE;
   /*
   fprintf(fcor,"\n chain=%7ld,plus=%7.2lf",
 cur_corner->ch.chain,cur_corner->ch.plus);
 fprintf(fcor,"\n x=%12.3f,y=%12.3f", cur_corner->x, cur_corner->y);
  */

   n_corners++;
   if (  (n_corners)> MAXPOINTS)
	      	      {
	     ShowMessage("Больше 20000 точек. Нельзя работать");
	      	       return(-1);
	     	      }
   if (first_corner==NULL)  first_corner = cur_corner;

   if (last_corner != NULL) last_corner->next = cur_corner;
   cur_corner->prev = last_corner;      /*переставил*/
  }          //while ДАЛЕЕ ОТЛАДОЧНАЯ ПЕЧАТЬ
//НАЧАЛО ПЕЧАТИ
    last_corner=cur_corner;
  xlt=first_corner->x; ylt=first_corner->y;
 xpt=last_corner->x; ypt=last_corner->y;
 fprintf(fcor,"\nread; n_corners=%d,xlt=%10.3lf,ylt=%10.3lf,xpt=%10.3lf,ypt=%10.3lf",
                    n_corners,xlt,ylt,xpt,ypt);
  //конец печати
   cur_corner->next=NULL;
 fclose(fp);   //crv


 // временно для записи пикетажа при его отсутствии
 /*    g = fopen ( file_name, "rt" );
fscanf(g,"%d%lf%lf",&type_survey,&uglt,&ugpt);
 long*chain1; double*plus1;//
 double s;
 double *x2,*y2;
 x2=new double[n_corners]; y2=new double[n_corners];
 chain1=new long[n_corners]; plus1=new double[n_corners];
 fscanf(g,"%ld%lf%lf%lf",&chain1[0],&plus1[0],&x,&y);
 x2[0]=x; y2[0]=y;
 s=0.0;
  for( int i=1; i<n_corners; i++)
  {
    fscanf(g,"%ld%lf%lf%lf",&chain1[i],&plus1[i],&x1,&y1);
    x2[i]=x1; y2[i]=y1;
   s=s+sqrt((x-x1)*(x-x1)+(y-y1)*(y-y1));
   chain1[i]=s/100;   plus1[i]=s-chain1[i]*100;
   x=x1; y=y1;
  }//end for
  fclose(g);
  fflush(g);

  g = fopen ( "UROS.crv", "wt" );
  fprintf(g,"%3d%12.6lf%12.6lf",type_survey,uglt,ugpt);
  for(int i=0; i<n_corners; i++)
  fprintf(g,"\n%9ld%9.4lf%18.6lf%18.6lf",chain1[i],plus1[i],x2[i],y2[i]);
  fclose(g);
  delete chain1; delete plus1; delete x2; delete y2;

 // end vremenno
 */
    if (n_corners<=3)
		      {
		   ShowMessage("Меньше 4 точек.  Нельзя работать");
		      return(-1);
		      }
 /* формирование стандарта*/
   cur_corner= first_corner;
   line[0].l=0.;
/*   line[0].alpha=0;
   line[0].ch.chain= cur_corner->ch.chain;
   line[0].ch.plus= cur_corner->ch.plus;
*/
   n_points=n_corners;
    i=1;
    a1=uglt;
   while( cur_corner->next!=NULL)
  {
  //  fprintf(fcor,"\n i=%d",i);

  x1=cur_corner->next->x-cur_corner->x;
  y1=cur_corner->next->y-cur_corner->y;

    line[i].l=sqrt(x1*x1+y1*y1);
/*     count_dist(cur_corner->ch,cur_corner->next->ch,&line[i].l);*/

   if(line[i].l<0.001)
 {ShowMessage("Повтор точки" +IntToStr(i)+" x1="+FloatToStr(x1)+" y1="+FloatToStr(y1));
   fclose(fcor);
   exit(0);
 }
 fprintf(fcor,"\n i=%d,x1=%7.2lf,y1=%7.2lf,line[i].l=%7.2lf",
 i,x1,y1,line[i].l);
 a2=asin(y1/line[i].l);
fprintf(fcor,"\na1=%10.6lf,a2=asin=%10.6lf,x1=%7.2lf",a1,a2,x1);
if(x1<0.) a2=PI-a2;else if(y1<0.) a2=a2+2.0*PI;
  /* if(uglt>10.0 && i==1) {uglt=a2; a1=a2;}*/
   line[i-1].alpha=a2-a1;

 fprintf(fcor,"\n PI-a2=%10.6lf,line[i-1].alpha=%12.7lf",a2,line[i-1].alpha);

  if(a2-a1>PI) line[i-1].alpha=line[i-1].alpha-PI*2.;
  else if(a2-a1<-PI) line[i-1].alpha=line[i-1].alpha+PI*2.;
/*
Сделал все углы положительными в одном круге, а разности любые
*/

   line[i-1].ch.chain= cur_corner->ch.chain;
   line[i-1].ch.plus= cur_corner->ch.plus;
 line[i-1].x=cur_corner->x; line[i-1].y=cur_corner->y;
fprintf(fcor,"\npik-1=%7ld,pl-1=%7.2lf", line[i-1].ch.chain, line[i-1].ch.plus);
fprintf(fcor,"\n alfi-1=%9.5lf", line[i-1].alpha);
fprintf(fcor,"\n i=%d,x=%12.4lf,y=%12.4lf",i,line[i-1].x,line[i-1].y);
 a1=a2;
 i++;
cur_corner=cur_corner->next;
if(cur_corner->next==NULL)
{
 line[i-1].alpha=ugpt-a2;
 if(line[i-1].alpha>PI) line[i-1].alpha=line[i-1].alpha-PI*2.;
 else if(line[i-1].alpha<-PI) line[i-1].alpha=line[i-1].alpha+PI*2.;
 if(ugpt>10.) line[i-1].alpha=0.;
 line[i].l=0;
  line[i-1].ch.chain= cur_corner->ch.chain;
  line[i-1].ch.plus= cur_corner->ch.plus;
 if(uglt >10.) line[0].alpha=0.;
    line[i-1].x=cur_corner->x; line[i-1].y=cur_corner->y;
 fprintf(fcor,"\n i=%3d alpha0=%9.5lf",i, line[0].alpha);
fprintf(fcor,"\npik-1=%7ld,pl-1=%7.2lf", line[i-1].ch.chain, line[i-1].ch.plus);
fprintf(fcor,"\n a1=%9.5lf,a2=%9.5lf",a1,a2);
fprintf(fcor,"\n alfi-1=%9.5lf", line[i-1].alpha);
fprintf(fcor,"\nend i=%d,x=%12.4lf,y=%12.4lf",i,line[i-1].x,line[i-1].y);
} //if  конец
      }//while

    fclose(fcor);
//ДОБАВКА
g = fopen ("nach_ugl_corner.txt", "wt" );
 xlt=x=first_corner->x; ylt= y=first_corner->y;
 xpt=cur_corner->x;  ypt=cur_corner->y;
 x1=first_corner->next->x; y1=first_corner->next->y;
 a2=y1-y; a1=x1-x;

fprintf(g,"\nx=%12.6lf, y=%12.6lf,x1=%12.6lf,y1=%12.6lf, dy=%12.9lf,dx=%12.9lf",
x,y,x1,y1,a2,a1);
 uglt=atan2(a2,a1);
 x1=cur_corner->prev->x; y1=cur_corner->prev->y;
 a2=y-y1; a1=x-x1;
  ugpt=atan2(a2,a1);//В пределах -180  + 180 
fprintf(g,"\nuglt=%12.8lf,xlt=%12.6lf,ylt=%12.6lf",uglt,xlt,ylt);
fprintf(g,"\nuglp=%12.8lf,xpt=%12.6lf,ypt=%12.6lf",ugpt,xpt,ypt);
 //PI=3.1415926;     есть в  DEFINE
a1=uglt*180.0/PI;
i=a1;
a2=a1-i;
fprintf(g,"\nugol a1=%12.7lf,grad i=%d,ost a2=%9.7lf",a1,i,a2);
a1=a2*60.0;
i=a1;
a2=a1-i;
fprintf(g,"\nugol a1=%12.7lf,min i=%d,ost a2=%9.7lf",a1,i,a2);
a1=a2*60.0;
i=a1;
a2=a1-i;
fprintf(g,"\nugol a1=%12.7lf,sec i=%d,ost a2=%9.7lf",a1,i,a2);

fclose(g);
  return 0;
}
  int write_inp_corners( char *file_name )
{/*пишет type=2,нач кон углы (100) пикет,плюс и координаты в .crv
     устанавливает ugl,xl,yl,если они не установлены set_ugl*/
   FILE *file;
   long chain;
   double plus;
    double x,y;
   int type;
   file = fopen ( file_name, "wt" );
   type = CORNER;
  cur_corner = first_corner;

 fprintf(file,"%3d %10.6lf %10.6lf",type, double(100.0),double(100.0) );///uglt,ugpt);
  while  (cur_corner !=  NULL )
  {
     x = cur_corner->x; y = cur_corner->y;

       //	    if (cur_corner->set_x &&  cur_corner->set_y)
	  //  {
 // if(
 fprintf(file,"\n%9ld%9.4lf%18.6lf%18.6lf",
              cur_corner->ch.chain,cur_corner->ch.plus,x,y);//!=55)
	   cur_corner=cur_corner->next;
  }/*while*/
 // xlt=first_corner->x; ylt=first_corner->y;
 // xp=xpt; yl=ypt;
//if(!set_ugl){set_ugl=1; ugl=uglt; xl=xlt;yl=ylt; }
/*при первой записи установил начало
 учитывалось и раньше при каждой новой кривой если она слева*/
/*  fprintf(fcor,"\nwrite xlt=%10.3lf,ylt=%10.3lf,xpt=%10.3lf,ypt=%10.3lf");*/
  fclose(file);

 return(0);     /*!*/

}

  int read_pts ( char *file_name )
{   // не используется это чтение LINE
   FILE *file = fopen(file_name, "rt");
   int i,j,type_survey;
   int chain;
   char c;

 fscanf(file,"%3d",&type_survey);

fscanf(file,"%4d",&n_points);

/* fprintf(file_r,"\n n_points=%d ",n_points);*/
   for ( i = 0; i < n_points; i++ )
 {
//fscanf(file,"%7lf %9lf",&line[i].l,&line[i].alpha);
//fscanf(file,"%7ld %7lf",&line[i].ch.chain,&line[i].ch.plus);
fscanf(file,"%lf %lf",&line[i].l,&line[i].alpha);
fscanf(file,"%ld %lf",&line[i].ch.chain,&line[i].ch.plus);

/* fprintf(file_r,"\n l= %7.2lf, alpha= %9.6lf ",line[i].l,line[i].alpha);
 fprintf(file_r,"\n i=%d,chain=%d ",i,line[i].ch.chain);
 fprintf(file_r,"plus=%7.2lf", line[i].ch.plus);*/
 for (j=0;j<=10;j++)
{ fscanf(file,"%c",&line[i].angle[j]);
/*fprintf(file_r,"\nj=%d,angle=%c",j,line[i].angle[j]);*/
}
 for (j=0;j<=6;j++)
{ fscanf(file,"%c",&line[i].arrow[j]);
/*fprintf(file_r,"\nj=%d,arrow=%c",j,line[i].arrow[j]);*/
}

 }
   fclose ( file );
/* fclose(file_r); */
   return (0);
}



/*------------------------------------------
	    Curve file output
------------------------------------------*/

int write_pts ( char *file_name )
{   // не используется это запись LINE
   FILE *file;
   int i,j,type_survey;
   int chain;
/*   double ord=0;*/


 file = fopen ( file_name, "wt" );
 fprintf(file,"\n%3d",type_survey);
 fprintf(file,"\n%4d",n_points);
    for ( i = 0; i < n_points; i++ )
   {
 fprintf(file,"\n%7.3lf %9.6lf",line[i].l,line[i].alpha);
 fprintf(file,"\n%7ld %7.2lf",line[i].ch.chain,line[i].ch.plus);
 for (j=0;j<=10;j++)
 fprintf(file,"%c",line[i].angle[j]);
 for (j=0;j<=6;j++)
fprintf(file,"%c",line[i].arrow[j]);
   }
   fclose ( file );

   return (0);
}

  int read_trace( char *file_name )
{
   FILE *fp;
   int i;
   long chain;
   double plus;
   TRACE_PTS *trace_pts_curr;
//-----------------

//   char source_angle_r[12],source_arrow_r[8];
//------------------
   fp = fopen ( file_name, "rt" );
   if( fscanf (fp,"%d", &n_pts ) != 1 )
   { fclose( fp );return( -2 );}
   if( fscanf (fp,"\n%lf%lf%lf",&ugl,&xl,&yl) != 3 )
   { fclose( fp );return( -2 );}
   if( n_pts <= 3 )
   {
      fclose( fp );
      return( -3 );
   }
      if( ( trace_pts_first = alloc_pts() )== NULL )
   { fclose( fp ); return( -4 );}

   trace_pts_curr = trace_pts_first;
   for( i=0; i<n_pts; i++, trace_pts_curr = trace_pts_curr->next )
   {
      if( fscanf(fp,"\n%ld%lf", &chain,&plus) != 2 )
      { fclose( fp );return( -5 );}

      trace_pts_curr->ch.chain = chain;
      trace_pts_curr->ch.plus = plus;
    if( fscanf (fp,"\n%lf%lf",
     &trace_pts_curr->dist,&trace_pts_curr->angle  ) != 2 )
      { fclose( fp );return( -5 );}
      if( fscanf (fp,"\n%lf%lf",
       &trace_pts_curr->min_shift,&trace_pts_curr->max_shift) != 2 )
      {	 fclose( fp );	 return( -5 ); }

      if( fscanf (fp,"\n%lf%lf",
      &trace_pts_curr->ev_x,&trace_pts_curr->ev_y ) != 2 )
      {	 fclose( fp );	 return( -5 );  }
      if( fscanf (fp,"\n%lf", &trace_pts_curr->shift ) != 1 )
      {	 fclose( fp );	 return( -5 );}
  }//for

   fclose( fp );
   return 0 ;
}

//////
      int write_trace( char *file_name )
{
   FILE *fp;
   int i;
   long chain;
   double plus;/*на диске float а в списке double и chainage double*/
   /*double dist;*/
   //float dist,angle;
   double dist,angle;
   double km,km1,km2;
   TRACE_PTS *trace_pts_curr;
    int j,k,ndob;
    CHAINAGE ch;
    double kmnah,kmkon,lsum; /*доб для длины*/

/*при разбивке длинных прямых приходится ставить точку на конец
   пикета и идти с плюсами кратными 20, для чего
 пришлось сначала определить число вставляемых точек из-за ранней
 записи общего числа точек, а затем при записи еще раз искать эти точки
 */
 double a1,s,s1;//float;
   ndob=0;
     simple_write_trace();
       return 0; //без разбивки и вставки
 //временно для проверки
    /*
 double x,y,ugl1;  FILE *fpr;
 fpr=fopen("Nevjzka.txt","wt");
 x=xl; y=yl; ugl1=ugl;
  fprintf(fpr,"pk=%5ld pl=%7.3lf,x=%12.4lf,y=%12.4lf",
   trace_pts_first->ch.chain,trace_pts_first->ch.plus,x,y);
  trace_pts_curr=trace_pts_first->next;
 for(i=0;i<n_pts-1;i++)
 {
   x+=trace_pts_curr->dist*cos(ugl1); y+=trace_pts_curr->dist*sin(ugl1);
   fprintf(fpr,"\npk=%5ld pl=%7.3lf,x=%12.4lf,y=%12.4lf",
   trace_pts_curr->ch.chain,trace_pts_curr->ch.plus,x,y);
   ugl1+=trace_pts_curr->angle;
   trace_pts_curr=trace_pts_curr->next;
  }
simple_write_trace();//записал
 free_pts(trace_pts_first);//уничтожил старый список
  read_trace( file_name );     // создал новый список

  x=xl; y=yl; ugl1=ugl;
  fprintf(fpr,"pk=%5ld pl=%7.3lf,x=%12.4lf,y=%12.4lf",
   trace_pts_first->ch.chain,trace_pts_first->ch.plus,x,y);
  trace_pts_curr=trace_pts_first->next;
 for(i=0;i<n_pts-1;i++)
 {
   x+=trace_pts_curr->dist*cos(ugl1); y+=trace_pts_curr->dist*sin(ugl1);
   fprintf(fpr,"\npk=%5ld pl=%7.3lf,x=%12.4lf,y=%12.4lf",
   trace_pts_curr->ch.chain,trace_pts_curr->ch.plus,x,y);
   ugl1+=trace_pts_curr->angle;
   trace_pts_curr=trace_pts_curr->next;
 }

 fclose(fpr); exit(0);
// return 0;конец проверки
      */
 //
/* определяю ndob-число добавляемых точек при разбивке
 на 20 длинных прямых*/
/*dist-хорда слева от точки, ->ch*/
for (trace_pts_curr=trace_pts_first;trace_pts_curr;
				    trace_pts_curr=trace_pts_curr->next)
 {
if(trace_pts_curr->dist > 30.0 )
 {
  a1=0.0;/*пройдено от начала прямой вставки*/
chain= ch.chain=trace_pts_curr->prev->ch.chain;
       ch.plus=trace_pts_curr->prev->ch.plus;
  pic_to_km(ch.chain,ch.plus,&km);
   do
  {
  dist=20.0;
  km1=km+20;

  km_to_pic(km1,&ch.chain,&s1);
  ch.plus=s1;
 if(chain!=ch.chain)/*перескочили пикет и его надо вставить,если не рядом*/
  {

    if(ch.plus>21)ch.chain=chain;/*перескочили в убывание пикетажа*/
    ch.plus=0;
    pic_to_km(ch.chain,ch.plus,&km2);/*километраж пикета слева*/
    if(km1-km2>1) dist=km2-km;else
    // from_km_to_ch(&ch,km1);/*восстановил ch*/
  km_to_pic(km1,&ch.chain,&s1);
  ch.plus=s1;

  }//if chain
   if(dist>19.999)/*перескочили на малую величину или не перескочили вообще*/
   {/*пикет тот же*/
    j=ch.plus/20;
    s=ch.plus-j*20;
    if(s<10.0)
      {/*остаток мал*/
          km2=km1-s;
         km_to_pic(km2,&ch.chain,&s1);
         ch.plus=s1;
       //  from_km_to_ch(&ch,km2);
       k=ch.plus/20;
       if(fabs(ch.plus-k*20)<0.01 )
       dist=km2-km; else dist=dist+s;
       /*при обратном пикетаже надо прибавить*/
      }/*остаток от деления плюса на 20 меньше 10м*/
      else
       {
          /*добавляем 20-остаток что меньше 10м*/
         km2=km1+(20-s);
         km_to_pic(km2,&ch.chain,&s1);
         ch.plus=s1;
       //from_km_to_ch(&ch,km2);
       k=ch.plus/20;
       if(fabs(ch.plus-k*20)<0.01 )
         dist=dist+20-s; else  dist=dist-(20-s);
       }
    }/*тот же пикет*/
   if(trace_pts_curr->dist-(a1+dist)<1.0) break;
     a1=a1+dist;
     km=km+dist;
      km_to_pic(km,&ch.chain,&s1);
      ch.plus=s1;
     chain=ch.chain;/*мог измениться пикет*/
     ndob++;
}  while(trace_pts_curr->dist-a1>30.0);

 }/*>30*/
}/*for*/

   n_pts=n_pts+ndob;
   if (n_pts > MAXPOINTS)
    { ShowMessage ("Переполнение участка"); restore_to_begin();
				return(0);}
   if(ugl>10.) {xl=0.; yl=0.;}
   fp = fopen ( file_name, "wt" );

 //=======
    fprintf (fp,"%5d", n_pts ) ;
    fprintf (fp,"\n%15.8lf%18.8lf%18.8lf",ugl,xl,yl);

///========
   trace_pts_curr = trace_pts_first;

                /*n_pts-  в цикле нужно восстановить старый*/

   for( i=0; i<n_pts-ndob; i++, trace_pts_curr = trace_pts_curr->next )
   {
    //иду по старому списку но пишу новый файл
      if(trace_pts_curr->dist>30.0)
    {

  a1=0.0;/*пройдено от начала прямой вставки*/
  chain= ch.chain=trace_pts_curr->prev->ch.chain;
       ch.plus=trace_pts_curr->prev->ch.plus;

     pic_to_km(ch.chain,ch.plus,&km);
   do
  {

  dist=20.0;
  km1=km+20;

  km_to_pic(km1,&ch.chain,&s1);
  ch.plus=s1;
 if(chain!=ch.chain)/*перескочили пикет и его надо вставить,если не рядом*/
  { if(ch.plus>21)ch.chain=chain;/*перескочили в убывание пикетажа*/
    ch.plus=0;
/*километраж пикета слева*/
    pic_to_km(ch.chain,ch.plus,&km2);
    if(km1-km2>1) dist=km2-km;else
/*восстановил ch*/
      km_to_pic(km1,&ch.chain,&s1);
      ch.plus=s1;
  }//if >21
   if(dist>19.999)/*перескочили на малую величину или не перескочили вообще*/
   {/*пикет тот же*/
    j=ch.plus/20;
    s=ch.plus-j*20;
    if(s<10.0)
      {/*остаток мал*/
          km2=km1-s;
          km_to_pic(km2,&ch.chain,&s1);
      ch.plus=s1;
       k=ch.plus/20;
       if(fabs(ch.plus-k*20)<0.01 )
       dist=km2-km; else dist=dist+s;
       /*при обратном пикетаже надо прибавить*/
      }/*остаток от деления плюса на 20 меньше 10м*/
      else
       {
          /*добавляем 20-остаток что меньше 10м*/
         km2=km1+(20-s);
         km_to_pic(km2,&ch.chain,&s1);
         ch.plus=s1;
       k=ch.plus/20;
       if(fabs(ch.plus-k*20)<0.01 )
         dist=dist+20-s; else  dist=dist-(20-s);
       }
    }/*тот же пикет или перескочили мало*/
   if(trace_pts_curr->dist-(a1+dist)<1.0)
{
break;
}
     a1=a1+dist;
     km=km+dist;
       km_to_pic(km,&ch.chain,&s1);
        ch.plus=s1;
     chain=ch.chain; plus=ch.plus;
      fprintf (fp,"\n%10ld%10.5lf",chain,plus) ;
	 angle=0.0;
      fprintf (fp,"\n%15.9lf%15.9lf",dist,angle) ;
 fprintf (fp,"\n%12.6lf%12.6lf",trace_pts_curr->min_shift,trace_pts_curr->max_shift);
 fprintf (fp,"\n%12.lf%12.6lf",trace_pts_curr->ev_x,trace_pts_curr->ev_y);
   /* добавил рихтовки*/
 fprintf (fp,"\n%12.6lf",trace_pts_curr->shift);

}  while(trace_pts_curr->dist-a1>31.0);


/*=================*/

      trace_pts_curr-> dist=trace_pts_curr->dist-a1;
     }/*->dist>30*/
     //++++++++++
           chain = trace_pts_curr->ch.chain;
           plus = trace_pts_curr->ch.plus;
       fprintf (fp,"\n%10ld%10.5lf",chain,plus);
 fprintf (fp,"\n%15.9lf%15.9lf",trace_pts_curr->dist,trace_pts_curr->angle) ;
 fprintf (fp,"\n%12.6lf%12.6lf",trace_pts_curr->min_shift,trace_pts_curr->max_shift);
    fprintf (fp,"\n%12.6lf%12.6lf",trace_pts_curr->ev_x,trace_pts_curr->ev_y);
   /* добавил рихтовки*/
    fprintf (fp,"\n%12.6lf",trace_pts_curr->shift);
  }//for вероятно
// разбиты длинные, исправлены близкие но не равные 20, вписаны в файл новые хорды
// нулевые углы, но повторены координаты точек эвольвенты что неверно!!!
// если читать заново будет новый список старый надо бы уничтожить
   fclose( fp );
  if(ndob)
  { free_pts(trace_pts_first);//уничтожил старый список
   read_trace( file_name );     // создал новый список
  // чтобы дальше работать с новым списком и n_pt
  // но в новом списке плохие эвольвенты
  //надо снова их вычислить по списку и снова всё записать
   //evolvents(trace_pts_first); //в списке новые эвольвенты а координаты???
   // запись нового файла отдельной функцией
   simple_write_trace();
  }
  /*
//запись для углограммы по новому списку после разбивки длинных прямых
    // при вызове из меню УГЛОГРАМММА идёт её запись по текущему списку
// ??  можно ли до добавления  точек вызвать то есть до записи
    char* name1=new char [80];
      strcpy(name1,uch_name);
      strcat(name1,".igl");// углограмма а uch_name.ugl проект текущий
//      fp = fopen ( "UGOL.UGL", "wt" );
  fp = fopen ( name1, "wt" );
    fprintf (fp,"%5d", n_pts ) ;
for (trace_pts_curr=trace_pts_first;trace_pts_curr;
				    trace_pts_curr=trace_pts_curr->next)
 {
 fprintf(fp,"\n%8ld%7.2lf%7.3lf%9.6lf",trace_pts_curr->ch.chain,trace_pts_curr->ch.plus,
             trace_pts_curr->dist,trace_pts_curr->angle);

 }
   fclose( fp );
   delete name1;
   */
   return( 0 );

}

     void simple_write_trace(void)
     {
 FILE *fp;
 TRACE_PTS *trace_pts_curr;
   long chain; double plus;
  //на диске float а в списке double и chainage double
  char* name1=new char [80];
    strcpy(name1,uch_name);
    strcat(name1,".tra");//
  fp = fopen ( name1, "wt" );
   fprintf (fp,"%5d", n_pts ) ;
    fprintf (fp,"\n%15.8lf%18.8lf%18.8lf",ugl,xl,yl);
for (trace_pts_curr=trace_pts_first;trace_pts_curr;
				    trace_pts_curr=trace_pts_curr->next)
 {
           chain = trace_pts_curr->ch.chain;
           plus = trace_pts_curr->ch.plus;
       fprintf (fp,"\n%10ld %10.5lf",chain,plus);
 fprintf (fp,"\n%15.9lf %15.9lf",trace_pts_curr->dist,trace_pts_curr->angle) ;
 fprintf (fp,"\n%12.6lf%12.6lf",trace_pts_curr->min_shift,trace_pts_curr->max_shift);
    fprintf (fp,"\n%12.6lf %12.6lf",trace_pts_curr->ev_x,trace_pts_curr->ev_y);

  // добавил рихтовки ev_x, ev_y теперь координаты x,y
    fprintf (fp,"\n%12.6lf",trace_pts_curr->shift);

 }//for
   fclose( fp );
   delete name1;
   }
///////
  TRACE_PTS * alloc_pts( )
{
   TRACE_PTS *first,
	     *curr;
   int i;

   if( n_pts < 1 )
      return( NULL );
   if( ( first = new TRACE_PTS ) == NULL )
      return( NULL );

   first->prev = NULL;
   curr = first;

   for( i=2; i<=n_pts; i++ )
   {
      if( ( curr->next = new TRACE_PTS  ) == NULL )
      {
	 while( curr->prev != NULL)
	 {
	    curr = curr->prev;
	    delete  curr->next;
	 }
	 delete  first ;
	 return NULL ;
      }
      curr->next->prev = curr;
      curr = curr->next;
   }
   trace_pts_last=curr;//конец трассы
   curr->next = NULL;
   return first;
}

void free_pts( TRACE_PTS *first )
{
   TRACE_PTS *curr;

   while( first != NULL )
   {
      curr = first;
      first = first->next;
      delete curr;
   }
  trace_pts_first=NULL;
  n_pts=0;
}

  CHAINAGE StrToChainage(char *sp)
{
 CHAINAGE ch; int i=0;
 while (sp[i]!='\0')
 {
 if( sp[i]==',') {sp[i]='.'; break;}
 i++;
 }
  ch.chain =  atol( sp );
  ch.plus  =  atof( strchr(sp, '+') + 1 );
//strchr даёт адрес первого вхождения символа этот адрес увеличили
//и с него начала работу atof в дробной части точку понимает,
//а запятую нет и прекращает перевод
 return ch;
}


