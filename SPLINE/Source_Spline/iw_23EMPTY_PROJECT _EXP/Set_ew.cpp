#include <vcl.h>
#include"UI.h"
#include "UFTRACKS.H"
#include <stdio.h>
#include<math.h>
//extern PACKAGE TfTrack *fTrack;
extern TfTrack *fTrack;
//extern double shf_l,shf_r;
extern int next_round(CHAINAGE ch1,CHAINAGE *ch2, double step);
int set_end( void )
{
//double kmnah,kmkon,lsum;
 double x1,x2,y1,y2,d,dlev,dprav,dist;

  CHAINAGE ch_end;
  char buf[40];

      /*     if (n_pts != 0)
       {
	  for( trace_pts_last = trace_pts_first;
	       trace_pts_last->next != NULL;
	       trace_pts_last = trace_pts_last->next );
        } last вычислен при компоновке
        */
 ch_end=StrToChainage(fTrack->Edit2->Text.c_str());
// fTrack->Edit2->Text=" пк  +  . ";   Надо точку если хотим сантиметры

//  ShowMessage(fTrack->Edit2->Text);

 if   (pic_to_km(ch_end.chain,ch_end.plus, &x1))
  {
     sprintf(buf,"%6ld+%6.2lf",ch_end);
     strcat(buf,"    за пределами объекта");
     ShowMessage(buf);
    return(1);
  }
 count_dist(trace_pts_last->ch,ch_end,&d);
    if ( d>-0.004 && d<0.004)  return (0);// совпал с последним

      count_dist(trace_pts_first->ch,ch_end,&dist);
			   if(dist<=0.)
		     { ShowMessage("Пикет конца до начала участка");
		      return(1);
                    }

    if ( d> 0. )  // ADDING POINTS
      {  ShowMessage("Пикет конца за концом участка");
		      return(1);
      }
         // DELETING POINTS

   if(MessageDlg("Удаляем точки ?",mtWarning,TMsgDlgButtons() << mbYes<<mbNo,0)==mrNo)
      return(1);
   //++ d<=0  новый пикет внутри участкп
        while (d<=0.)//
   { count_dist(trace_pts_last->prev->ch,ch_end,&d);
           if(d<=0.)// end левее или совпал с последним текущим
            {n_pts--;
              trace_pts_last=trace_pts_last->prev;
              delete(trace_pts_last->next);//бывший последний
            }//d<=0
   }//while end правее prev но левее last d расст от prev do end >=0
      if(d<0.1)
      {
        n_pts--;
              trace_pts_last=trace_pts_last->prev;
              delete(trace_pts_last->next);//бывший последний
               trace_pts_last->next=NULL;
        return (0);
      }//совпал с левым
       count_dist(trace_pts_last->ch,ch_end,&d);
       dprav=-d;
       if(dprav<0.004){trace_pts_last->next=NULL; return (0);}
       //здесь внутри отрезка
       d=trace_pts_last->dist; dlev=d-dprav;
     x1= trace_pts_last->prev->ev_x; y1= trace_pts_last->prev->ev_y;
     x2= trace_pts_last->ev_x; y2= trace_pts_last->ev_y;   //+++
      trace_pts_last->ev_x=x1+(x2-x1)*dlev/d;
      trace_pts_last->ev_y=y1+(y2-y1)*dlev/d;
      trace_pts_last->dist=dlev;  trace_pts_last->angle=0.0;
      trace_pts_last->ch.chain=ch_end.chain;
      trace_pts_last->ch.plus=ch_end.plus;
      trace_pts_last->next=NULL;
      return (0);
}//funkcii
