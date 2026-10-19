//extern void AnsiString Par1,Par2;
   struct CHAINAGE
    {
  long chain;
  double plus;
     } ;


  typedef struct TRACE_ELTS_TAG
{
   CHAINAGE cl_ch0, cl_ch1;//начало и конец переходной вычисляемые? или записаны?
    double min_l_clot,l_clot,max_l_clot,min_rad,rad,max_rad;
    double min_l_circ,l_circ,max_l_circ,h,vpas,vgruz;

 }
   TRACE_ELTS;


 typedef struct TRACE_PTS_TAG
{
 //  struct TRACE_PTS_TAG *prev, *next;
   CHAINAGE ch;
   double dist,             /*  Хорда                         */
	  angle,            /*  Угол                          */
	  min_shift,        /*  Мин. отклонение               */
	  max_shift,        /*  Макс. отклонение              */
	  ev_x,             /*  \    Координаты               */
	  ev_y,             /*  /    в эвольвентах            */
	  shift;            /*  Сдвижка                       */
}
   TRACE_PTS;
extern   struct TRACE_PTS_TAG *tr;
extern struct  TRACE_ELTS_TAG *elts;
////
extern char pereg[30];
extern char uch_name[80];
extern char elt_name [80];
extern char num_var[4];
extern AnsiString trackfilename;
//extern AnsiString asdfilename;
extern AnsiString eltfilename;
 extern char set_uch;
 extern char set_var;
// extern rp1[50];
 extern int krp;
 extern  double Asd(AnsiString s);
 extern CHAINAGE StrToChainage(char *sp);
 extern void formugl(void);
 extern  int count_chain( CHAINAGE ch1,  double dist, CHAINAGE *ch2);
 extern  int km_to_pic(double km,long *pic,double *plus);
 extern  int pic_to_km(  long pic, double plus,double *km);
extern void evolvents( void );
extern void new_shifts(void);
extern void trace_show(void);
extern  void write_trace( char *file_name );
extern void write_elts(char*file_name);
extern int count_dist( CHAINAGE ch1,CHAINAGE ch2,double *dist );
extern double pravxy1(double x0,double y0,double phi0 );
extern int print_trace_p( void );
extern int  conla(double xl,double yl,double phil,double sig,double l,
			 double *xr,double *yr,double *phir);
extern int conl(double ksi,double eta,double phi,double sig,double k,double l,
	  double *ksi1,double *eta1,double *phi1,double *sig1);
extern  double clox(double k,double l);
extern double cloy(double k,double l);
extern void CONV_GRAD( double ugl,char &sig,int& grad,int& min,double& sec);//добавил
 extern double per5;// начальная абсцисса участка
 extern  int n_elts;
 extern int n_pts;
 extern double ugl;
 extern double xl,yl;
 extern double xlpr,ylpr,uglpr;// проектное начало может не совпадать с существующим
 extern double phifin;
 extern double min_l_first;
 extern double max_l_first;
 extern double l_first;
 extern double min_l_last;

// extern double deo=0.5;extern double plus; extern long picet;// оптимизация
// extern int numper;
