//---------------------------------------------------------------------------

#ifndef UIH
#define UIH
#include <system.hpp>
//---------------------------------------------------------------------------
#define CURVE -2
#define PSKA -1
#define GONI 0
#define ELEM 1
#define CORNER 2
#define FALSE 0
#define TRUE  1
#define PI 3.1415926
#define MAXPOINTS 20000

  typedef struct
 {
  long chain;
  double plus;
 } CHAINAGE;

  typedef struct
  {
  int type;
  CHAINAGE ch;
double drs;
double phi0;
double sig0;
double x0;
double y0;
double l;
  }STYLE;

  typedef struct TRACE_ELTS_TAG
{

   CHAINAGE cl_ch0, cl_ch1;
   double	 rad;           /*  радиус кривой                   */
 }
   TRACE_ELTS;

   typedef struct CORNER_POINT_STR_TAG
{
   struct CORNER_POINT_STR_TAG *next, *prev;
   int num;
   CHAINAGE ch; /*доб*/
   double x,y;
   int set_x,set_y;
}
   CORNER_POINT_STR;

 typedef struct
{
   CHAINAGE ch;
   double l, alpha;
   char angle[12], arrow[8];
   double x,y;// новое для передачи в оптимизатор
} LINK;
 typedef struct TRACE_PTS_TAG
{
   struct TRACE_PTS_TAG *prev, *next;
   CHAINAGE ch;
   double dist,             /*  Хорда                         */
	  angle,            /*  Угол                          */
	  min_shift,        /*  Мин. отклонение               */
	  max_shift,        /*  Макс. отклонение              */
	  ev_x,             /*  \    Координаты               */
	  ev_y,             /*  / в эвольвентах нет декартовы */
	  shift;            /*  Сдвижка                       */
}
   TRACE_PTS;
extern char fname[80];// имя кривой вместе с crv
extern AnsiString trackfilename;
extern AnsiString asdfilename;
extern   LINK line[MAXPOINTS]; //  UI.cpp без extern

extern   CORNER_POINT_STR *cur_corner,*first_corner,*last_corner;//UI.cpp без extern
extern   TRACE_PTS *trace_pts_first,*trace_pts_last;  //  UI.cpp без extern
extern   TRACE_PTS *curr_pts,*last_pts,*end_pts,*next_pts ;
//functions
extern void probel(char*st);

extern int km_to_pic(double km,long *pic,double *plus);
extern int pic_to_km(  long pic, double plus,double *km);

extern int read_inp_corners( char * );
extern int write_inp_corners( char *);


extern int read_trace( char *);
extern int write_trace( char *file_name );
extern int read_inp_asd( char *,char );
extern int write_inp_asd( char *);

extern int count_chain( CHAINAGE ch1,  double dist, CHAINAGE *ch2 );
extern int count_dist( CHAINAGE ch1,CHAINAGE ch2,double *dist );
extern   int prev_round(CHAINAGE ch1,CHAINAGE *ch2, double step);
extern int next_round(CHAINAGE ch1,CHAINAGE *ch2, double step);
extern int set_beg(void);
extern int set_end(void);
extern void restore_to_empty(void);
extern void restore_to_begin(void);
extern void restore_to_end(void);
extern void evolvents(TRACE_PTS *  );
extern double Asd(AnsiString s);//unit5
extern CHAINAGE StrToChainage(char *sp);
extern void free_pts(TRACE_PTS *);
extern void freeCorners();
extern void simple_write_trace();
extern int n_points;//UI.cpp без extern
extern int n_pts; // trace points  в UI.cpp без extern
extern int n_asd, n_elts, n_fix;
extern CHAINAGE *fix;
extern TRACE_ELTS *elts;  // новая структура без указателей
extern int type_survey;
extern double dist,step;
extern double ugl,uglt,ugp,ugpt,xl,xp,xlt,xpt,yl,ylt,yp,ypt;
extern int set_ugl;
extern char uch_name[80];
#endif
