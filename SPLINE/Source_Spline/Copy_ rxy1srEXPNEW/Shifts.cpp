#include <vcl.h>

/*-----------------------------------------------------*
 * evolvents                                           *
 * new_shifts                                          *
 *-----------------------------------------------------*
 */

#include "extern.h"


void evolvents( void )
{
  // В Finkriwi.cpp есть она же с вычислением длины по дуге а не по ломаной;
   double x, y, phi;
   int i;


   for( i=0, x = 0, y = 0, phi = 0; i<n_pts; i++ )
   {
  /*    fprintf(f,"\nx=%9.4lf,y=%9.4lf,phi=%7.4lf,dist=%7.4lf",
      x,y,phi,trace_pts_curr->dist);
  */  tr[i].ev_x=x +=tr[i].dist;
      tr[i].ev_y=y +=phi*tr[i].dist;
      //trace_pts_curr->ev_x = x += trace_pts_curr->dist;
      //trace_pts_curr->ev_y = y += phi*trace_pts_curr->dist;
/* fprintf(f,"\nev_x=%7.2lf,ev_y=%7.2lf",
       trace_pts_curr->ev_x, trace_pts_curr->ev_y);
*/
//      phi += trace_pts_curr->angle;
         phi +=tr[i].angle;
   }
   phifin = phi;
/*fclose(f)*/
}

/*--------------------------------------------------------------*
 * Calculate shifts for given elements                          *
 *--------------------------------------------------------------*
 */

void new_shifts( void )
{//вычисляет рихтовки по эвольвентам
  //это плохо можно только при загрузке варианта
   double x, y, phi, sig, k;
   int i,j,i1;
    i=0;//номер точки j- номер элемента i1-элемент за пределами
    while ((i<n_pts) && (tr[i].ev_x <= l_first))
   {
      tr[i].shift = -tr[i].ev_y;
      i++;
   }
   x = l_first;
   y = 0;
   phi = 0;
   sig = 0;
   j=0, i1 = 1;
    while(  j<n_elts )
   {
      if( elts[j].l_clot == 0 )
	 k = 0;
      else if( elts[j].rad == 0 )
	 k = -sig/elts[j].l_clot;
      else
	 k = (1/elts[j].rad-sig)/elts[j].l_clot;
      while ( (i<n_pts) && (tr[i].ev_x <= x+elts[j].l_clot))

      {
          tr[i].shift = y+(tr[i].ev_x-x)* (phi+(tr[i].ev_x-x)*
				   (sig/2+k/6*(tr[i].ev_x-x)))
				  -tr[i].ev_y;
//        if( x+elts[j].l_clot-tr[i].ev_x<0.01) break;
        i++;
      } //внутренний  while
      if( i==n_pts )
      {
         ShowMessage(AnsiString(i1)+ "-ый элемент за пределами уч-ка");
          return;
      }
      x += elts[j].l_clot;
      y += elts[j].l_clot*(phi+elts[j].l_clot*(sig/2+k/6*elts[j].l_clot));
      phi += elts[j].l_clot*(sig+k/2*elts[j].l_clot);
      if( elts[j].rad == 0 )
	 sig = 0;
      else
	 sig = 1/elts[j].rad;
      while( (i<n_pts) && (tr[i].ev_x <= x+elts[j].l_circ) )
      {
	 tr[i].shift = y+(tr[i].ev_x-x)* (phi+(tr[i].ev_x-x)*sig/2) -tr[i].ev_y;
//        if( x+elts[j].l_circ-tr[i].ev_x<0.1) break;

        i++;
       }//while
      if( i==n_pts )
       {
       ShowMessage(AnsiString(i1)+ "-ый элемент за пределами уч-ка");
	 return;
      }
      x += elts[j].l_circ;
      y += elts[j].l_circ*(phi+elts[j].l_circ*sig/2);
      phi += elts[j].l_circ*sig;
      j++; i1++;
   }// внешний while j
    //на последней клотоиде
   if( elts[j].l_clot == 0 )
      k = 0;
   else
      k = -sig/elts[j].l_clot;
      while ( (i<n_pts) && (tr[i].ev_x <= x+elts[j].l_clot) )
    {
      tr[i].shift = y+(tr[i].ev_x-x)*
				(phi+(tr[i].ev_x-x)*
				(sig/2+k/6*(tr[i].ev_x-x)))
			       -tr[i].ev_y;
  //      if( x+elts[j].l_clot-tr[i].ev_x<0.1) break;
        i++;
     }
           if( i==n_pts )
       {
       ShowMessage(AnsiString(i1)+ "-ый элемент за пределами уч-ка");
	 return;
      }
// на последней прямой
   x += elts[j].l_clot;
   y += elts[j].l_clot*(phi+elts[j].l_clot*(sig/2+k/6*elts[j].l_clot));
   phi += elts[j].l_clot*(sig+k/2*elts[j].l_clot);
   sig = 0;

while(i<n_pts)

   {
      tr[i].shift = y+(tr[i].ev_x-x)*phi-tr[i].ev_y;
                               i++;
   }
}
/*
void put_crit()
{
   double crit;
   double d1,d2;
   char str[8];
   TRACE_PTS *curr;
   if (crit_flag) del_crit();
   curr=trace_pts_first;
   crit=0.;
   while(curr!=NULL)
   {
     d1 = dabs((double)(curr->shift));
     d2 = dabs(crit);
     if (d1 > d2) crit=curr->shift;
     curr=curr->next;
   }
   crit_flag=TRUE;
   if  (safe_open( &main_window[CRIT_W] )==2)
      {
	 message( "ЌҐв Ї ¬пвЁ ¤«п ®Є­  ЄаЁвҐаЁп" );
	 terror( "" );
      }
   wshow( main_window[CRIT_W].wno );
   wselect( main_window[CRIT_W].wno );
   sprintf( str, "%7.2lf", crit);
   atsay( 0, 0, str );
}

void del_crit()
{
 if (crit_flag)
 {
 crit_flag=FALSE;
 safe_close( &main_window[CRIT_W] );
 }
}
int fix_last_point()
{
  TRACE_PTS *last ;
  double dsh;
  last=trace_pts_first;
  while (last->next!=NULL) last=last->next;
  dsh = last->shift;
  if (dabs(dsh)  <  0.005) return(TRUE);
  return(FALSE);
}

  */
