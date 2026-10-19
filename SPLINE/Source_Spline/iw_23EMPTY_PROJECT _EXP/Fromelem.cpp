
#include <vcl.h>
#include <stdio.h>
#include <math.h>
#include "UI.h" //chainage здесь нет

/*-----------------------------------------------------
  Построение трассы в стандартном представлении по заданным
  элементам
-----------------------------------------------------*/
/*----------------------------------------------------------*
 *      формирование массива line по заданным элементам     *
 *----------------------------------------------------------*/
int cont(double xl,double yl,double phi,double l,double *xr,double *yr); /*!*/
int  conla(double xl,double yl,double phil,double sig,double l,
			 double *xr,double *yr,double *phir);      /*!*/
int conl(double ksi,double eta,double phi,double sig,double k,double l,
	  double *ksi1,double *eta1,double *phi1,double *sig1);
double clox(double k,double l);
double cloy(double k,double l);

int next_point();
int load_point();
int next_style();
double sig_elt(double rad);
double dbet(double xa,double ya,double xb,double yb,double xc,double yc);
double ro(double x1, double y1, double x2,double y2)
{ double dx,dy,d;
  dx=x1-x2; dy=y1-y2;
  d=sqrt(dx*dx+dy*dy);
  return(d);
}


//=========
   CHAINAGE ch_c,ch_r, cch[2];
  // double dist;   extern
 //  double h_c,h_r,x_l,y_l,x_c,y_c,x_r,y_r;
      double h_r,x_c,y_c,x_r,y_r;//h_c
   double phi_c,phi_r,sig_c,sig_r;
   double x[3],y[3];
   int beg_line_flag;
STYLE  cur_style;
//TRACE_ELTS *cur_elt;
//FIX_POINT_STR *first_fixp, *cur_fixp;
int cur_elt=0,cur_fixp=0;
int num_elt=0;// признак начала
//,num_fix=0; количество пройденных элементов и фикс точек



 int load_point()
 {
		 x[2] = x_r; y[2] = y_r;
		 line[n_points].alpha = dbet(x[0],y[0],x[1],y[1],x[2],y[2]);
		 line[n_points].l = ro(x[0],y[0],x[1],y[1]);
		 x[0] = x[1];  y[0] = y[1];
		 x[1] = x[2];  y[1] = y[2];

			      cch[1] = ch_r;
		 line[n_points].ch = cch[0];
			      cch[0] = cch[1];
                      // похоже и ch[1] лишний cch[0]=ch_r; после line=
		 n_points++;
		 if (n_points>MAXPOINTS)
	{	 ShowMessage ("Слишком много точек");   return 1;}
	      return(0);   /*!*/
}
int next_point()
{
/* даёт признак перехода на новый элемент и параметры конца x_r,y_r,phi_r,sig_r
вычисленные по началу x_c,y_c,phi_c,sig_c,cur_style.drs
 координаты угол кривизна параметр клотоиды
и расстоянию h_r с учётом фиксированных точек
*/


  int k,cut;
  k=0;
  cut=FALSE;//накрыли сделав шаг фиксированную точку, ch_c- начало шага,
            // ch_r-пикетаж его конца после округления

if(  next_round(ch_c, &ch_r,step)) ShowMessage(" ошибка next_round в  next point");
  // вычислили предварительно ch_r
  //if (cur_fixp!=NULL)
  if ((n_fix!=0)&& (cur_fixp<n_fix))
  {
//    count_dist(ch_r,cur_fixp->ch,&dist);
    count_dist(ch_r,fix[cur_fixp],&dist);
    if (dist<=0.)   /*<*/
    {
	//ch_r=cur_fixp->ch;накрыли фиксированную точку и выставили конец на неё
     ch_r=fix[cur_fixp];
	cut=TRUE;
    }
  }
     //cur_style.ch- конец текущего элемента
  count_dist(ch_r,cur_style.ch,&dist);//расстояние  от конца текущего элемента
  if (dist<0.)
  {
    ch_r = cur_style.ch;
    k=-1; // правее конца текущего элемента встали в его конец
  }
  else
   // if (cut==TRUE)  cur_fixp=cur_fixp->next;
    if (cut==TRUE)  cur_fixp++;
    //на след. фикс точку если её накрыли и элемент не кончился
//ch_r- новая точка или шаг после округления
// или фикс или конец эл-та что ближе к исходной
   count_dist(ch_c,ch_r,&h_r);
  //h_r- нужная длина для вычислений это шаг после округлений и фикс. точек

  switch (cur_style.type)
 {
  case -1: //идём по клотоиде
	      conl(x_c,y_c,phi_c,sig_c,cur_style.drs,h_r,
				&x_r,&y_r,&phi_r,&sig_r);
				sig_c = sig_r;
				phi_c = phi_r;
			    break;
   case 0:
	    cont(x_c,y_c,phi_c,h_r,&x_r,&y_r);
  // по прямой
			    break;

   case 1:
	    conla(x_c,y_c,phi_c,sig_c,h_r,&x_r,&y_r,&phi_r);
// по окружности
	    phi_c = phi_r;
			    break;
 }
 return(k);// k  покажет был ли переход на новый элемент -1 был 0- не был
}

int next_style()
{
  CHAINAGE ch_tmp;
  int exit_flag;
  //if (cur_elt == NULL)
    if(num_elt==0)
     { // всё начинается с прямой
//       cur_elt = trace_elts_first;  то есть не нуль
          num_elt++;
       cur_style.type = 0;
       //cur_style.ch = cur_elt->cl_ch0;
  cur_style.ch = elts[cur_elt].cl_ch0;
       beg_line_flag=TRUE;
       return(0);
      }
  exit_flag=FALSE;
  while(!exit_flag)
  {
  switch (cur_style.type)
     {
     case -1: /* переход с переходной на кривую  или прямую*/
	    beg_line_flag=FALSE;
	    //if (cur_elt->next != NULL)
            if (cur_elt !=n_elts-1 )
	      { // не последний элемент
		if (elts[cur_elt].rad == 0.) cur_style.type = 0;
		else cur_style.type =1;
                // стояли на клотоиде далее прямая или окружность
		/*cur_style.ch = cur_elt-> next->cl_ch0;
		sig_c=sig_elt(cur_elt->rad);
                */
                cur_style.ch = elts[cur_elt+1].cl_ch0;
		sig_c=sig_elt(elts[cur_elt].rad);
		exit_flag=TRUE;
	       }
	     else
	       {   // последний элемент
		cur_style.type = 0;
	       //next_round(cur_elt->cl_ch1,&cur_style.ch, step);
               next_round(elts[cur_elt].cl_ch1,&cur_style.ch, step);
		ch_tmp = cur_style.ch;
		count_chain(ch_tmp,step,&cur_style.ch);
// прямая её конец округлить на шаг и сделать шаг это конец прямой
	       return(-1);
	       }
		break;
     default : /* переход с кривой на переходную нового элемента */
	     //	if (cur_elt == trace_elts_first && beg_line_flag)
             	if (cur_elt == 0 && beg_line_flag)
		   {
		     cur_style.type = -1;
		     //cur_style.ch = cur_elt->cl_ch1;
                     cur_style.ch = elts[cur_elt].cl_ch1;
                   count_dist(elts[cur_elt].cl_ch0,elts[cur_elt].cl_ch1,&dist);
		     //if (cur_elt->l_clot>0.01)
                     if(dist>0.01)
		     {
		     //cur_style.drs = 1/(cur_elt->rad)/(cur_elt->l_clot);
                cur_style.drs = 1/(elts[cur_elt].rad)/dist;
		     exit_flag=TRUE;
		     }
		     else {  exit_flag=FALSE;}
          	    }  //начало
		else
		   {
		     //cur_elt = cur_elt->next;
                     cur_elt ++;
		     cur_style.type = -1;//встали на клотоиду
		     //cur_style.ch = cur_elt->cl_ch1;её конец
                     cur_style.ch = elts[cur_elt].cl_ch1;
                 count_dist(elts[cur_elt].cl_ch0,elts[cur_elt].cl_ch1,&dist);
		  /*   if (cur_elt->l_clot>0.01)

		     {
		     if (cur_elt->next != NULL)
  cur_style.drs =( sig_elt(cur_elt->rad)- sig_elt(cur_elt->prev->rad))/
						       cur_elt->l_clot;
		     else
 cur_style.drs = -sig_elt(cur_elt->prev->rad)/cur_elt->l_clot;
		       exit_flag=TRUE;
		      }//dist>0.01
                      */
                    if (dist>0.01)
                    {
     if(cur_elt!=n_elts-1)
   cur_style.drs =( sig_elt(elts[cur_elt].rad)- sig_elt(elts[cur_elt-1].rad))/
						       dist;
		     else
 cur_style.drs = -sig_elt(elts[cur_elt-1].rad)/dist;
		       exit_flag=TRUE;

                    }//dist>0
		     else { exit_flag=FALSE;}
		   } // не начало
		   break;
	       }  /*case*/
  } /*цикл*/
  return(0);
}

double sig_elt(double rad)
{
  if( (rad >- 0.01) && (rad<0.01) )
 { if(rad==0.) return(0.);else{ShowMessage("Округление радиуса"); return(0);}
 }
   else return(1/rad);
}
double dbet(double xa,double ya,double xb,double yb,double xc,double yc)
{
  double ab,bc,det,sin;
  double xba,xcb,yba,ycb;

  xba = xb-xa; yba = yb-ya;
  xcb = xc-xb; ycb = yc-yb;

  ab = sqrt( xba*xba + yba*yba);
  bc = sqrt( xcb*xcb + ycb*ycb);

  det = xba*ycb - xcb*yba;
   return(asin(det/ab/bc));
}

int cont(double xl,double yl,double phi,double l,double *xr,double *yr)
{
    double x,y;

      *xr = xl + l*cos(phi);
      *yr = yl + l*sin(phi);

      return(0);
}
int  conla(double xl,double yl,double phil,double sig,double l,
			 double *xr,double *yr,double *phir)
/*      *********** ДВИЖЕНИЕ ПО КРИВОЙ  *************/
 {
     double cos_phi,sin_phi,dphi,dx,dy,dxn,dyn;
	   cos_phi =cos(phil);
	   sin_phi =sin(phil);
     dphi= l*sig;
     /* вставить обработку плохой кривизны */
     dx  = sin(dphi)/sig;
     dy  = (1- cos(dphi))/sig;

	   dxn  = dx*cos_phi - dy*sin_phi;
	   dyn  = dy*cos_phi + dx*sin_phi;
     *xr  = xl+dxn;
     *yr  = yl+dyn;

     *phir = phil + dphi;
     return(0);
 }

 int conl(double ksi,double eta,double phi,double sig,double k,double l,
	  double *ksi1,double *eta1,double *phi1,double *sig1)
 {
/*        *********** ДВИЖЕНИЕ ПО КЛОТОИДЕ  при малом k по кривой*************/
	double l0,phi0,x0,y0,x1,y1,l1,sin0,cos0;
	double eps=0.000000001;
	if ( k>-eps && k<eps)
	{
       //  conla(ksi,eta,phi,sig,l,&(*ksi1),&(*eta1),&(*phi1)); &*- Зачем??
           conla(ksi,eta,phi,sig,l,ksi1,eta1,phi1);
	}
	else
	{

	l0=sig/k;
	phi0=phi-k*l0*l0/2;
	x0=clox(k,l0);   y0=cloy(k,l0);
	sin0=sin(phi0);  cos0=cos(phi0);
	l1=l0+l;
	x1=clox(k,l1);   y1=cloy(k,l1);
	*ksi1=ksi+((x1-x0)*cos0 - (y1-y0)*sin0);
	*eta1=eta+((x1-x0)*sin0 + (y1-y0)*cos0);
	}
	*sig1=sig+k*l;
	*phi1=phi+l*(sig+l*k/2);

	return(0);
 }

 double clox(double k,double l)
 {
  double eps = 0.00001;

  double q,a,b,t_clox;
  int i;
	   if (l<eps && l>-eps) return(0.);
	    q = k*l*l/2;  q = -q*q;
	    b = l;
	    t_clox = l;
           i=0;
	    do
	    {
              i++;
	      if (i>=100) { ShowMessage("*** ЗAЦИKЛИBAHИE B CLOX");}

	      b = b*q/(2*i)/(2*i-1);
	      a = b/(4*i+1);
	      t_clox = t_clox + a;
	     }
	    while (a>eps || a<-eps);
	     return(t_clox);
 }

 double cloy(double k,double l)
 {
  double eps = 0.00001;
  double a,b,q,t_cloy;
  int i;
	   if (l<eps && l>-eps) return(0.);
	    q = k*l*l/2;  q = -q*q;
	    b = k*l*l*l/2;
	    t_cloy = b/3;
            i=0;
	    do
          {
                i++;
       if (i>=100){ ShowMessage("*** ЗAЦИKЛИBAHИE B CLOY");   }
	      b = b*q/(2*i+1)/(2*i);
	      a = b/(4*i+3);
	      t_cloy = t_cloy + a;
          }
              while (a>eps || a<-eps);
	     return(t_cloy);
  }

