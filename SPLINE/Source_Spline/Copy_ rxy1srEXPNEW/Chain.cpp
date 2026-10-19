


/*-----------------------------------------------------*
 *--------  Railway horizontal alignment CAD  ---------*
 *-----------------------------------------------------*
 *                Chainage handling                    *
 *-----------------------------------------------------*
 * count_chain                                         *
 * count_dist                                          *
 * next_round                                          *
 * prev_round                                          *
 * calc_ch                                             *
 * calc_inp_elt                                        *
 * def_chain2                                          *
 * calc_arc                                            *
 *-----------------------------------------------------*
 */
#include <process.h>
#include "extern.h"
/*================== INNER FUNCTIONS ======================*/
int next_round_(CHAINAGE ch1,CHAINAGE *ch2, double step);
int prev_round_(CHAINAGE ch1,CHAINAGE *ch2, double step);
 int trans_rad_to_gm(double angle,
		long *grad, long *min);    /* не было*/

/*------ ѕреобразование целочисленного (см) представлени€ пикетажа
	  в реальное --*/
/*--------------------------------------------------------------*
 * Define 2-nd point chainage by 1-st point chainage & distance *
 * Return TRUE if OK                                            *
 *--------------------------------------------------------------*
 */
int count_chain( CHAINAGE ch1,  double dist, CHAINAGE *ch2 )
{
   double km;                   int km_to_pic(double km,long *pic,float *plus);

   char buf[40];
   float y;
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
      ShowMessage(" count_chain Ўаг за пределы перегона!");
      return(1);

   }
}

/*-----------------------------------------------------*
 * Define distance between 2 points by their chainage  *
 * Return TRUE if OK                                   *
 *-----------------------------------------------------*
 */
int count_dist( CHAINAGE ch1,CHAINAGE ch2,
		double *dist )
{
 double km1, km2;
 char buf[40];
 if   (!from_ch_to_km(ch1, &km1))
  {
     sprintf(buf,"%6ld+%3.0lf",ch1);
     strcat(buf," за пределами перегона");
     message(buf);
    return(FALSE);
  }

     if(!from_ch_to_km(ch2, &km2) )
  {
     sprintf(buf,"%6ld+%3.0lf",ch2);
     strcat(buf," за пределами перегона");
     message(buf);
    return(FALSE);
  }
  /*-----
fprintf(file_dbg,"\n CHAIN.C ch1=%ld+%5.2lf   ch2=%ld+%5.2lf  km1=%lf km2=%lf",
			     ch1,ch2,km1,km2);
  -----*/
 *dist = km2 - km1;
  if(km2<km1) /* нельз€==*/
  {
/*   message("ќтрицательное рассто€ние");*/
   return(FALSE);
   } else
  return( TRUE );  
   
}
/*-----------------------------------------------------*/
int next_round(CHAINAGE ch1,CHAINAGE *ch2, double step)
{
 double km1, km2,km3,km4,dl,pl2;
 CHAINAGE ch3;
 int dir,i,j;
/* FILE *fne;*/
/*if( (fne= fopen("testnex.dbg","wt"))==NULL){putchar(7);exit(0);};*/
  j=0;
 if (!from_ch_to_km(ch1,&km1))
 {
  /* fprintf(fne,"\nch1=%7ld+%6.2lf",ch1);
   fclose(fne);*/
   terror("next_round обратились с ch1 вне перегона");
 }
    km2=km1+step;
 if(! from_km_to_ch(ch2,km2))
 {
   /* fprintf(fne,"\nch1=%7ld+%6.2lf,step=%5.2lf,ch2=%7ld+%6.2lf",
		  ch1,step,(*ch2));
   fclose(fne);*/
   terror("next_round шаг вне перегона");
 }
  if((*ch2).plus<0.005) {/* fclose(fne);*/ return(TRUE);}
 ch3.chain=(*ch2).chain;
 ch3.plus=0.0;
if(!  from_ch_to_km(ch3,&km3))
 {
  /* fprintf(fne,"\nch3=%7ld+%6.2lf,ch1=%7ld+%6.2lf",ch3,ch1);*/
/*   fclose(fne);*/
   dir=-1;
   j=1;
   message("next_round обратились с ch3 вне перегона");
 }

 ch3.chain++;
if(!  from_ch_to_km(ch3,&km4))
 {
   /*fprintf(fne,"\nch1=%7ld+%6.2lf, ch3=%7ld+%6.2lf",ch1,ch3);*/
/*   fclose(fne);*/
   dir=1;
   j=2;
   message("next_round обратились с ch3 вне перегона");
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

 if (from_ch_to_km(*ch2,&km2) && km2>km1) {/*fclose(fne);*/return(TRUE);}


/*fprintf(fne,"\nstep=%16.12lf,km1=%12.5lf,km2=%12.5lf",step,km1,km2);
fprintf(fne,"\nch1=%7ld+%14.10lf,ch2=%7ld+%14.10lf",ch1,*ch2);
fprintf(fne,"\ndir=%d,dl=%7.2lf,i=%d,pl2=%7.2lf,km3=%7.2lf,km4=%7.2lf",
              dir,dl,i,pl2,km3,km4);
fclose(fne);
*/
  message("next_round,смотри testnex.dbg");
   exit(0);

 return(FALSE);
}

int prev_round(CHAINAGE ch1,CHAINAGE *ch2, double step)
{
 double km1, km2,km3,km4,dl,pl2;
 CHAINAGE ch3;
 int dir,i,j;
 FILE *fne;
/*if( (fne= fopen("testprev.dbg","wt"))==NULL){putchar(7);exit(0);};*/
  j=0;
 if (!from_ch_to_km(ch1,&km1))
 {
/*   fprintf(fne,"\nch1=%7ld+%6.2lf",ch1);
   fclose(fne);*/
   terror("prev_round обратились с ch1 вне перегона");
 }
if(ch1.plus<0.005)
 {
    km2=km1-step;
 if(! from_km_to_ch(ch2,km2))
 {
/*    fprintf(fne,"\nch1=%7ld+%6.2lf,step=%5.2lf,ch2=%7ld+%6.2lf",
		  ch1,step,(*ch2));
*/
   terror("prev_round шаг вне перегона");
 }
   /*    fclose(fne);*/
 return(TRUE);
 }
(*ch2).chain=ch1.chain;
 ch3.chain=(*ch2).chain;
 ch3.plus=0.0;
if(!  from_ch_to_km(ch3,&km3))
 {
/*   fprintf(fne,"\nch3=%7ld+%6.2lf",ch3);*/
/*   fclose(fne);*/
     dir=1;
     j=1;
   message("prev_round обратились с ch3 вне перегона");
 }
 ch3.chain++;
if(!  from_ch_to_km(ch3,&km4))
 {
  /* fprintf(fne,"\nch3=%7ld+%6.2lf",ch3);*/
/*   fclose(fne);*/
     dir=-1;
     j=2;
   message("prev_round обратились с ch3 вне перегона");
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
    { pl2=pl2-step*dir; if(pl2<0.) pl2=0.;}/*округлени€ не было*/
    if(pl2>dl-0.005) { pl2=0.; (*ch2).chain++;}/*надо иначе за пикетом!!!*/
    (*ch2).plus=pl2;
 if (from_ch_to_km(*ch2,&km2) && km2<km1) {/*fclose(fne);*/return(TRUE);}

/*fprintf(fne,"\nstep=%16.12lf,km1=%12.5lf,km2=%12.5lf",step,km1,km2);
fprintf(fne,"\nch1=%7ld+%14.10lf,ch2=%7ld+%14.10lf",ch1,*ch2);
fprintf(fne,"\ndir=%d,dl=%7.2lf,i=%d,pl2=%7.2lf,km3=%7.2lf,km4=%7.2lf",
              dir,dl,i,pl2,km3,km4);
fclose(fne);*/
  message("prev_round,смотри testprev.dbg");
   exit(0);

 return(FALSE);
}

/*-----------------------------------------------------*
 * Find next round chainage after point 1              *
 * Return TRUE if OK                                   *
 *-----------------------------------------------------*
 */
int next_round_(CHAINAGE ch1,CHAINAGE *ch2, double step)
{
/* теперь в next_round не нужна*/
/*   short n_steps;*/
    int  n_steps;
/*   FILE *fn;*/
   double a;
   (*ch2).chain = ch1.chain;
   a =ch1.plus/step;
   n_steps=a;
   (*ch2).plus = n_steps*step;
   if(ch1.plus-(*ch2).plus>step-0.007) (*ch2).plus+=step;/*добавил*/
/*  {fn=fopen("test_fn.dbg","wt");
   fprintf(fn,"\nch1=%7ld+%19.15lf",ch1);
  fprintf(fn,"\na=%18.16lf,step=%20.17lf",a,step);
   fprintf(fn,"\nplus=%20.17lf,n=%ld",ch1.plus,n_steps);

   fclose(fn);
  }*/
   (*ch2).plus += step;
   if( (*ch2).plus >= 99.995 )
   {
      (*ch2).chain += 1;
      (*ch2).plus -= 100.;
      if( (*ch2).plus < 0. )
	 (*ch2).plus = 0.;
   }
   return( TRUE );
}

/*-----------------------------------------------------*
 * Find previous round chainage after point 1          *
 * Return TRUE if OK                                   *
 *-----------------------------------------------------*
 */
int prev_round_(CHAINAGE ch1,CHAINAGE *ch2, double step)
{
/* теперь в prev_round не нужна*/
   short n_steps;

   (*ch2).chain = ch1.chain;
   n_steps =(short) (ch1.plus/step);
   (*ch2).plus = n_steps*step;
   if( (*ch2).plus == ch1.plus )
   {
      (*ch2).plus -= step;
      if( (*ch2).plus < -0.005 )
      {
	 (*ch2).plus += 100.;
	 (*ch2).chain -= 1;
      }
      if( (*ch2).plus < 0. )
	 (*ch2).plus = 0.;
   }

   return( TRUE );
}

/*-----------------------------------------------------*
 * Calculate elts chainage for i_elt to n_elts+1       *
 *-----------------------------------------------------*
 */
int calc_ch( TRACE_ELTS *trace_elts_curr )
{
   CHAINAGE ch0;
   double dist;
   TRACE_ELTS *trace_elts_temp;
   int i;

   if( trace_elts_curr == trace_elts_first )
   {
      ch0 = trace_pts_first->ch;
      dist = l_first;
      count_chain( ch0,  dist, &ch0);
   }
   else
   {
      ch0 = trace_elts_curr->prev->cl_ch1;
      dist = trace_elts_curr->prev->l_circ;
      count_chain( ch0, dist, &ch0 );
   }
   for( trace_elts_temp = trace_elts_curr;
	trace_elts_temp->next != NULL;
	trace_elts_temp = trace_elts_temp->next )
   {
      trace_elts_temp->cl_ch0 = ch0;
      dist = trace_elts_temp->l_clot;
      count_chain( ch0, dist, &ch0 );
      trace_elts_temp->cl_ch1 = ch0;
      dist = trace_elts_temp->l_circ;
      count_chain( ch0, dist, &ch0 );
   }
   trace_elts_temp->cl_ch0 = ch0;
   dist = trace_elts_temp->l_clot;
   count_chain( ch0,  dist, &ch0 );
   trace_elts_temp->cl_ch1 = ch0;

   return( TRUE );
}

CHAINAGE from_long_to_CHAINAGE(long ich)
{
  CHAINAGE ch;
      ch.plus=((ich % 10000))/100.;
      ch.chain=ich/10000;
  return(ch);
}

/*----------------------------------------------------------------------*/
int calc_inp_elts( TRACE_ELTS *trace_elts_curr )
{
   long ch0;
   double pl0, dist;
   TRACE_ELTS *trace_elts_temp;
   int i;
/*   if (trace_elts_curr->cl_ch1.chain==0 && trace_elts_curr->cl_ch1.plus==0.)
   {
     trace_elts_curr->cl_ch1=trace_elts_curr->cl_ch0;
   }  */
   trace_elts_temp = trace_elts_curr->prev;
   if (trace_elts_temp != NULL)
      {
      if (!count_dist(trace_elts_temp -> cl_ch1,
		      trace_elts_curr -> cl_ch0,
		       &dist))
	      {
		message("ќтрицательна€ длина предыдущей пр€мой/круговой");
		return(FALSE);
	      }
	  trace_elts_temp -> l_circ =(float) dist;
       }

      if (!count_dist(trace_elts_curr -> cl_ch0,
		      trace_elts_curr -> cl_ch1,
		       &dist))
		{
		 message("ќтрицательна€ длина переходной");
		 return(FALSE);
		}
    /*		if (dist<0.)
	       { trace_elts_curr -> cl_ch1=trace_elts_curr -> cl_ch0;
		  dist=0.;
		}*/
	   trace_elts_curr -> l_clot = (float) dist;
    trace_elts_temp = trace_elts_curr->next;

    if (trace_elts_temp != NULL)
      {
       if (!count_dist(trace_elts_curr -> cl_ch1,
		       trace_elts_temp -> cl_ch0,
		       &dist))
	      {
	       message("ќтрицательна€ длина  пр€мой/круговой");
	       return(FALSE);
	      }
	  /*trace_elts_temp -> l_circ = (float)dist;*/
           trace_elts_curr -> l_circ = (float)dist;
	}
       return(TRUE);
}

/*----------------------------------------------------------------------*/
int calc_arc (TRACE_PTS *cur_pts, double *sig, double *arc1)
{
 double h1,h2,h3,alpha,phi;
 if (cur_pts->prev==NULL || cur_pts->next==NULL
     ||ABS(cur_pts->angle)<(float) 0.0001 || cur_pts->dist< (float) 0.001)
 {
   *sig=0.;
   *arc1=cur_pts->dist;
   return(TRUE);
 }


 alpha = cur_pts->angle;
 h1=cur_pts->dist;
 h2=cur_pts->next->dist;
 h3=sqrt(h1*h1 + h2*h2 + 2*h1*h2*cos(alpha));

 *sig = 2*sin(alpha)/h3;  /*1/R*/

 phi = 2*asin(h1*(*sig)/2); /*угол поворота касательной на хорде h1*/
 *arc1 = phi/(*sig);       /*дуга*/
 return(TRUE);
}
/*----------------------------------------------------------------------*/
int calc_append_information (int num,TRACE_ELTS *cur,
	       double *p, int *np,double *t, int *nt,double *tk, int *ntk)
{
 double rad_c,rad_l,rad_r,sig_c,sig_l,sig_r;
 double alf;
 static  double  tk_first,tk_last;
 if (cur->prev != NULL)
     rad_l  = cur->prev->rad;
 else rad_l = 0.;
 if (cur->next != NULL)
     rad_r  = cur->next->rad;
 else rad_r= 0.;
 rad_c  = cur->rad;

 sig_l = 0.; sig_c = 0.; sig_r=0.;
 if (rad_l !=0.) sig_l = 1./rad_l;
 if (rad_c !=0.) sig_c = 1./rad_c;
 if (rad_r !=0.) sig_r = 1./rad_r;


 *p = (cur->l_clot)*(cur->l_clot)/24.*(sig_c-sig_l);
 *np = num;
 if (sig_c==0.)  *ntk=-1;
 else
 {
   alf = sig_c*((cur->l_clot+cur->next->l_clot)/2. + cur->l_circ);
   *tk = (rad_c+ (*p))*tan(alf/2);
   *ntk = num;
   if (sig_l == 0.)  tk_first = *tk;
   if (sig_r == 0.) tk_last = *tk;
 }
 if (sig_l ==  0. && sig_r == 0.) *ntk=-1;
 *nt = -1;
 if (sig_l == 0.)
 {
  *nt =  num;
  *t = tk_first + mt(cur->l_clot,sig_c);
 }
 if (sig_c == 0.)
 {
  *ntk =  -1;
  *nt = num;
  *t = tk_last + mt(cur->l_clot,sig_l);
 }
return(TRUE);
}
/*----------------------------------------------------------------------*/
double mt(double LM, double SM)
{  double res;

  res=(((1./17280.*(LM*SM*LM*SM)-1./120.)*(LM*SM*LM*SM))+1)*LM/2.;
  return(res);
}
/*----------------------------------------------------------------------*/
 int trans_rad_to_gm(double angle,
		long *grad, long *min)
{
 long allmin;
 int sig;
 if  (angle<0.)
 {
     sig=-1;
     angle*=-1;
 }
 allmin =(long)( KRADMIN*angle+0.5);
 *grad= allmin/60;
	allmin-=*grad*60;
 *min = allmin;
 if (sig==-1)
 {
    if (*grad>0) {  *grad*=-1; return(0);}
    if (*min>0) {  *min*=-1; return(0);}
 }
 return(0);
}
/*----------------------------------------------------------------------*/
 int erase_set_curves(TRACE_CURVES *cur)  /*int*/
{
cur->set_beg_ch=FALSE;
cur->set_end_ch=FALSE;
cur->set_clot1=FALSE;
cur->set_clot2=FALSE;
cur->set_rad=FALSE;
cur->set_angle=FALSE;
cur->set_curve=FALSE;
cur->set_line=FALSE;
return(1);   /*не было*/
}
int calc_inp_curves(TRACE_CURVES *cur_curves)
{
   double dist,angle;
   TRACE_CURVES *temp_curves;
   int i;
      if (cur_curves->prev != NULL)
      {
	if (cur_curves->prev->set_end_ch &&  cur_curves->set_beg_ch)
	{
	count_dist(cur_curves->prev->end_ch,
		 cur_curves->beg_ch,&dist);
	 cur_curves->prev->l_line =(float) dist;
	 cur_curves->prev->set_line = TRUE;
	 }
      }
      if (cur_curves->next != NULL)
      {
	if (cur_curves->next->set_beg_ch &&  cur_curves->set_end_ch)
	{
	count_dist(cur_curves->end_ch,
		 cur_curves->next->beg_ch,&dist);
	 cur_curves->l_line =(float) dist;
	 cur_curves->set_line = TRUE;
	 }
      }

      if (!cur_curves->set_beg_ch || !cur_curves->set_end_ch
	 || !cur_curves->set_clot1  || !cur_curves->set_clot2
				    ||!cur_curves->set_rad)
      return(TRUE);/*пока нечего провер€ть */


      count_dist(cur_curves->beg_ch,
		 cur_curves->end_ch,&dist);
      if (dist<0.)
      {
       message("ќтрицательна€ длина кривой");
       return(FALSE);
      }
      cur_curves->l_curve =
	      (float)   dist-cur_curves->l_clot1-cur_curves->l_clot2;

      if (cur_curves->l_curve<= (float) 0.)
      {
       message("ќтрицательна€ длина чистой круговой кривой");
       return(FALSE);
      }
      cur_curves->set_curve=TRUE;

      angle=(dist-0.5*(cur_curves->l_clot1+cur_curves->l_clot2))/cur_curves->rad;
      trans_rad_to_gm(angle,&cur_curves->g,&cur_curves->m);
      cur_curves->set_angle=TRUE;
       return(TRUE);
}
/*----------------------------------------------------------------------*/
int test_inp_curves()
{
 TRACE_CURVES *cur;
 cur=trace_curves_first;
 while (cur!=NULL)
 {
  if  (!cur->set_angle)
   {
      message(" ривые недоопределены... ");
      return(FALSE);
   }
   cur=cur->next;
 }
   return(TRUE);
}
int calc_n_elts()
{
TRACE_CURVES *cur;
 cur=trace_curves_first;
 n_elts=0;
 while (cur!=NULL)
 {
   n_elts++;
   if (cur->next!=NULL && cur->l_line>(float)0.005) n_elts++;
   if (n_elts>MAX_TRACE_ELTS)
       { message("Ѕолее 20 элементов. —ократите участок");
	 return(FALSE);
       }
   cur=cur->next;
  }
   return(TRUE);
}
/*----------------------------------------------------------------------*/
int from_curves_to_elts()
{
TRACE_ELTS *cur_elt;
TRACE_CURVES *cur_curve;
if  (!test_inp_curves()) { message("ѕреобразование не проходит");return(FALSE);}
calc_n_elts();
trace_elts_first=alloc_elts((int)n_elts);
cur_elt=trace_elts_first;
cur_curve=trace_curves_first;

cur_elt->l_clot=cur_curve->l_clot1;
while (cur_curve!=NULL)
{
 if (cur_curve->prev==NULL || cur_curve->prev->l_line>(float)0.005)
 cur_elt->l_clot=cur_curve->l_clot1;
 cur_elt->rad =  cur_curve->rad;
 cur_elt->l_circ = cur_curve->l_curve;
 cur_elt->next->l_clot=cur_curve->l_clot2;
 if (cur_curve->next!=NULL && cur_curve->l_line>(float)0.005)
 { /*пр€ма€*/
  cur_elt=cur_elt->next;
  cur_elt->rad=(float)0.;
  cur_elt->l_circ=cur_curve->l_line;
 }
  cur_elt=cur_elt->next;
  if (cur_elt==NULL) message("chain.c  –азладка");
  cur_curve=cur_curve->next;
}

trace_elts_first->cl_ch0=trace_curves_first->beg_ch;
cur_elt=trace_elts_first;

while ( cur_elt->next!=NULL)
{
 count_chain( cur_elt->cl_ch0,
	       (double) cur_elt->l_clot,
	      &cur_elt->cl_ch1);
 count_chain( cur_elt->cl_ch1,
	       (double) cur_elt->l_circ,
	      &cur_elt->next->cl_ch0);
 cur_elt=cur_elt->next;
}
 count_chain( cur_elt->cl_ch0,
	       (double) cur_elt->l_clot,
	      &cur_elt->cl_ch1);


return(TRUE);
}

/*------------------------------------------------------------------
  ¬озвращаетс€ среднее значение
-------------------------------------------------------------------*/
CHAINAGE middle_ch( CHAINAGE ch1, CHAINAGE ch2)
{
  long ich1, ich2;
  ich1 = from_CHAINAGE_to_long(ch1);
  ich2 = from_CHAINAGE_to_long(ch2);
  return(
	from_long_to_CHAINAGE((ich1+ich2)/2)
	);

}
/*-----------------------------------------------------------
 —равнение значений соответствующих пикетов.
Returns:    a negative value if chainage1 < chainage2,
	    0 if chainage1 = chainage2,
	    or a positive value if chainage1 > chainage2.
-----------------------------------------------------------*/
int ch_cmp(CHAINAGE a, CHAINAGE b)
{
 int ret;
 long chainage1, chainage2;
 chainage1 = from_CHAINAGE_to_long(a);
 chainage2 = from_CHAINAGE_to_long(b);

 if (chainage1<chainage2) return(-1);
 if (chainage1>chainage2) return(1);
 if (chainage1==chainage2) return(0);

}

/*----- ѕреобразование реального представлени€ в целочисленное(см) ----*/
long from_CHAINAGE_to_long(CHAINAGE ch)
{
return((long) ((ch.chain % 10000)*10000+  (ch.plus+0.005)*100.));
}
/*----------------------------------------------------------*/
int double_proekcia(double *a, double a0, double a1)
{
/* FALSE возвращаетс€ если проекции не было  */
 if ( (*a)< a0)
 {
    (*a) = a0;
    return(TRUE);
 }
 if ( (*a)>  a1)
 {
    (*a) = a1;
    return(TRUE);
 }
 return(FALSE);

}
