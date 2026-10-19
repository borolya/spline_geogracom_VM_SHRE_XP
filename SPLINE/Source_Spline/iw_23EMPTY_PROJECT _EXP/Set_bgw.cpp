#include <vcl.h>
#include"UI.h"//extern типы функции указатели переменные
#include "UFTRACKS.H"
#include <stdio.h>
#include<math.h>
//extern PACKAGE TfTrack *fTrack;
extern TfTrack *fTrack;
extern double shf_l,shf_r;
 double kmnah;// не зная зачем
int set_beg( void )
{

 //char filename[MAX_DIR];
   double dist,km;
   CHAINAGE ch_oldbeg,ch_beg,ch_curr,ch_last;
 double km1;
 char buf[50];
 // double angle_beg=0.;//float
 ch_beg=StrToChainage(fTrack->Edit1->Text.c_str());
  // ShowMessage(fTrack->Edit1->Text);
  //  fTrack->Edit1->Text="   +   ";
/*         if (n_pts != 0)
       {
	  for( trace_pts_last = trace_pts_first;
	       trace_pts_last->next != NULL;
	       trace_pts_last = trace_pts_last->next );
        }
*/
   if( pic_to_km(ch_beg.chain,ch_beg.plus, &kmnah))
   {
   sprintf(buf,"%6ld+%6.3lf",ch_beg);
   strcat(buf," Новое начало за пределами участка");
   ShowMessage(buf);
   return(1);
   }
   //printf("\nначало kmnah=%7.2lf,ch_beg=%7ld+%5.2lf",kmnah,ch_beg);
  //dist<0 если километраж первого больше чем второго ( правее от начала)
   count_dist(ch_beg,trace_pts_last->ch,&dist);
      if (dist<=0.)
     { ShowMessage("Ошибка. Hачало за концом участка");
		       return(1);
     }
	   ch_oldbeg=trace_pts_first->ch;
 count_dist(ch_beg,ch_oldbeg,&dist); //доб ранее
	   if (dist>-0.004 && dist<0.004) return(0);
	   if ( dist > 0)
   {ShowMessage("Ошибка. Начало вне участка"); return(1);}
        //доп точек
	//    ADDING POINTS
    	  // DELETING POINTS
 if(MessageDlg("Удаляем точки ?",mtWarning,TMsgDlgButtons() << mbYes<<mbNo,0)==mrNo)
		  return(1);

         //   ugl=100;//не вычисляем касательную
          double x,y,d,dlev,dprav;;
        //+++
         curr_pts=trace_pts_first;
         count_dist(curr_pts->next->ch,ch_beg,&dist);
         while ( dist>=0.)//оба не дошли
           {ugl=ugl+curr_pts->next->angle;

            curr_pts= curr_pts->next;
            delete (curr_pts->prev);
              n_pts=n_pts-1;
            count_dist(curr_pts->next->ch,ch_beg,&dist);
           }//while  итог левый curr,правый next dist<0 до правого
          dist=-dist; dprav=dist;
          if(dist<0.1)//совпал с правым
          {
             n_pts=n_pts-1;
            curr_pts->next->dist = 0.;
            curr_pts->next->angle=0.0;
             curr_pts->next->prev=NULL;
          ugl=ugl+curr_pts->next->angle;
           trace_pts_first= curr_pts->next;
           xl=curr_pts->next->ev_x;yl=curr_pts->next->ev_y;
          delete (curr_pts);
          return (0);
          } else
         {
          count_dist(curr_pts->ch,ch_beg,&dist);
          if(dist<0.004)//совпал с левым
          {
          curr_pts->dist = 0.;
          curr_pts->angle = 0.;
          curr_pts->prev=NULL;
          trace_pts_first= curr_pts;
           xl=curr_pts->ev_x;yl=curr_pts->ev_y;
       return (0);
          }//левый
      else {double x1,y1,x2,y2;
          d= curr_pts->next->dist;
          dlev=d-dprav;
          x1=curr_pts->ev_x;  y1=curr_pts->ev_y;
          x2=curr_pts->next->ev_x;  y2=curr_pts->next->ev_y;
          curr_pts->ev_x=x1+(x2-x1)*dlev/d;
          curr_pts->ev_y=y1+ (y2-y1)*dlev/d;
          curr_pts->ch=ch_beg;//поправил пикетаж
          curr_pts->dist = 0.;
          curr_pts->next->dist = dprav;
          curr_pts->prev=NULL;
          xl=curr_pts->ev_x;yl=curr_pts->ev_y;
           trace_pts_first= curr_pts;
       return (0);
            }
          }
        //deleting

  // return(0);
}

