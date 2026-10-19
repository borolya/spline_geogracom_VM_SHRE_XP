#include <stdio.h>
#include <vcl.h>
//#include "extern.h"
//#include <process.h>
#include "UI.h"//chainage

/* -- ¬ключить кривую --- */
/*
//--------------------------------------------------------------*
 // Calculate trace evolvents                                    *
 //--------------------------------------------------------------*

void evolvents(TRACE_PTS *first  )
{
   TRACE_PTS *trace_pts_curr;
   double x, y, phi,arc,sig;

   for( trace_pts_curr = first, x = 0, y = 0, phi = 0;
	trace_pts_curr != NULL;
	trace_pts_curr = trace_pts_curr->next )
   {
      calc_arc(trace_pts_curr,&sig,&arc);

    // угол хорды это полусумма углов касательных по концам!!
    //  было сомм trace_pts_curr->ev_x = x += trace_pts_curr->dist;
    //было  сомм trace_pts_curr->ev_y = y +=phi*trace_pts_curr->dist;

      trace_pts_curr->ev_x = x += arc;
        trace_pts_curr->ev_y = y += phi*arc;
       phi += trace_pts_curr->angle;

//fprintf(file_dbg,"\n x=%5.0lf , y=%10.6lf,phi=%10.6lf",x,y,phi);
   }
   phifin = phi;

} */
