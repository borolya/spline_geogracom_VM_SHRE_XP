//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include<stdio.h>
#include <math.h>
#include "UI.h"
#include "Unit2.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfGonik *fGonik;
//---------------------------------------------------------------------------
   Gonik_str* G_point;
 double CONV_ANGLE(int grad,int min,double sec,char sign )
 { double s;
 s=((sec/60.0+min)/60+abs(grad))/180.0*PI;
 if(sign!=0)s=-s;
 return s;
 }
 void probel(char*st)
 {char c, s[80],s1[80];
 //strcpy(s,st);
  sscanf(st,"%s",s);
  if(s[0]!='-') return ;
  sscanf(st,"%s%s",s,s1);
  strcat(s,s1);
  strcpy(st,s);
  return;
 }
 __fastcall TfGonik::TfGonik(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TfGonik::FormCreate(TObject *Sender)
{
  sgGonik->Cells[0][0] = " є ";
  sgGonik->Cells[1][0] = "    ѕикет ";
  sgGonik->Cells[2][0] = "   ѕлюс ";
  sgGonik->Cells[3][0] = " √радусы ";
  sgGonik->Cells[4][0] = " ћинуты ";
  sgGonik->Cells[5][0] = " —екунды ";
  sgGonik->Cells[6][0] = " в сантиметрах ";

  // очистка в USZD


}
//---------------------------------------------------------------------------

 int write_gonik(char*fp)
 {
   CHAINAGE ch;
    FILE*f;
    int j; double y;
        char st[80];
/*           j=1;
  while( j<fGonik->sgGonik->RowCount)
   {

    if(fGonik->sgGonik->Cells[6][j].Trim().IsEmpty())
    {ShowMessage(" Ќе задана стрела");fGonik->Close();return 1;}
      j++;//есть пустые углы и пустые стрелы, где углы не пустые.
   }//while j   нельз€. пустые углы кроме некоторых
  */
   f=fopen(fp,"wt");
    type_survey=GONI;
    fprintf(f,"%d",type_survey);
    n_points=fGonik->sgGonik->RowCount-1;
    fprintf(f,"\n%5d",n_points);

     for (int i=1;i<=n_points;i++)//нумерую элементы с 1 до n_points
                                // в соответствии с номером строки в таблице
   {
       ch.chain=fGonik->sgGonik->Cells[1][i].ToInt();
       ch.plus=Asd( fGonik->sgGonik->Cells[2][i]);
       fprintf(f,"\n%9ld%9.3lf",ch.chain,ch.plus);
        for (int j=3;j<6;j++)
     {
       if(fGonik->sgGonik->Cells[j][i].Trim().IsEmpty())
       fprintf(f,"\n%c",'0');
        else
    { switch (j)
      {
      case 3:
      strcpy(st, fGonik->sgGonik->Cells[3][i].c_str());
       probel(st);// ликвидирует пробелы между минусом и числом и ведущие
       if(st[0]!='-'){fprintf (f,"\n%c",' ');fprintf (f,"%6d",atoi(st));break;}
       if(st[1]=='0')fprintf(f,"\n%s","-0");
   else
    fprintf (f,"\n%6d",atoi(st));    //градусы
        break;
        case 4:
         fprintf(f,"\n%7d",fGonik->sgGonik->Cells[4][i].ToInt()); // min
        break;
        case 5:
      y=Asd(fGonik->sgGonik->Cells[5][i]);//секунды убрал зап€тую
          fprintf(f,"\n%7.2f",y);
      }//switch
     }//else
    }//for j
      if(fGonik->sgGonik->Cells[6][i].Trim().IsEmpty())
       fprintf(f,"\n%c",'0');    else
       {

     strcpy(st, fGonik->sgGonik->Cells[6][i].c_str());
       probel(st);// ликвидирует пробелы между минусом и числом
     y=Asd( AnsiString(st));//стрела
     fprintf(f,"\n%7.2f",y);
       }
    }//for i
 fclose(f);
 return 0;
 }

 int read_gonik(char*fp)
 {

  CHAINAGE ch;
    FILE*f;
    char st[11],s[8],st1[8], c;

    int j; double y;
    f=fopen(fp,"rt");
   fscanf(f,"%d",&type_survey);
    fscanf(f,"\n%5d",&n_points);
    fGonik->sgGonik->RowCount=n_points+1;

     for (int i=1;i<=n_points;i++)//нумерую элементы с 1 до n_points
                                // в соответствии с номером строки в таблице
   {   fGonik->sgGonik->Cells[0][i]=AnsiString(i);
       fscanf(f,"\n%ld%lf",&ch.chain,&ch.plus);//нельз€ в хвост \n
       fGonik->sgGonik->Cells[1][i]=AnsiString(ch.chain);
       fGonik->sgGonik->Cells[2][i]=AnsiString(ch.plus);
  fscanf(f,"%c",&c);//дл€ перевода на новую строку в начало, а не на непробел
        for (int j=3;j<7;j++)
       {
     //  fscanf(f,"%s\n",st);   //читает до пробела
       fgets(st,10,f);  //содержит не нужный символ \n
     if(st[0]=='0'){ fGonik->sgGonik->Cells[j][i]=""; }
     else {       sscanf(st,"%c%s",&c,st1);
    fGonik->sgGonik->Cells[j][i]=AnsiString(c)+AnsiString(st1);
          }//else
       }//for j
    }//for i
 fclose(f);
 return 0;
 }
void __fastcall TfGonik::Button4Click(TObject *Sender)
{
//записать
n_points=sgGonik->RowCount-1;
if( write_gonik(fname))
ShowMessage(" рива€ не записана");
sgGonik->RowCount=2;
sgGonik->Rows[1]->Clear();
        //ModalResult=mrOk;
        fGonik->Close();

}
//---------------------------------------------------------------------------
void __fastcall TfGonik::Button1Click(TObject *Sender)
{
//добавить
 sgGonik->RowCount++;
 for (int i=0;i<sgGonik->ColCount;i++)
 sgGonik->Cells[i][sgGonik->RowCount-1]="";
 sgGonik->Row=sgGonik->RowCount-1;
 sgGonik->Col=1;
 int j=sgGonik->RowCount;
 //sprintf(buf,"%d",j-1);
 sgGonik->Cells[0][j-1]=AnsiString(j-1);
}
//---------------------------------------------------------------------------
void __fastcall TfGonik::Button2Click(TObject *Sender)
{
// вставить
sgGonik->RowCount++;
  for(int i=sgGonik->RowCount-1; i>=sgGonik->Row+1; i--)
  {
    sgGonik->Rows[i]=sgGonik->Rows[i-1];
  }
 for (int i=0;i<sgGonik->ColCount;i++)
 sgGonik->Cells[i][sgGonik->Row]="";
 sgGonik->Col=1;

for (int i=1;i< sgGonik->RowCount;i++)
 sgGonik->Cells[0][i]=AnsiString(i);// нова€ нумераци€
 }
//---------------------------------------------------------------------------
void __fastcall TfGonik::Button3Click(TObject *Sender)
{
//удалить
  int R = sgGonik->Row;// исходна€ где стоим
  if(sgGonik->RowCount>1)
  {
  if(sgGonik->Row != sgGonik->RowCount-1)
 {
  for(int i=R+1;i<sgGonik->RowCount; i++)
  {
    sgGonik->Rows[i-1]=sgGonik->Rows[i];

  }//for
  } //if не последн€€
  sgGonik->RowCount--;
  }//>1
  for (int i=1;i< sgGonik->RowCount;i++)
 sgGonik->Cells[0][i]=AnsiString(i);// нова€ нумераци€
}
//---------------------------------------------------------------------------
void __fastcall TfGonik::Button5Click(TObject *Sender)
{
// не записывать
     sgGonik->RowCount=2;
     sgGonik->Rows[1]->Clear();
           fGonik->Close();
}

//---------------------------------------------------------------------------
int from_gonik ( void )
/*ѕеределан на  ≠† arcsin сделать по массиву G_point без source_arrow*/
{
   //G_POINT_STR *cur_point;
   int i_point;
   double phi_bas,
	  phi_lin,
	  phi_lin_prev;
   phi_bas = 0;
   phi_lin = 0;
     //++++++++++++
     dist=0.0;// до начальной точки от не существующей предыдущей
     for(i_point=0;i_point<n_points;i_point++)
     {
     line[i_point].l = dist;
     line[i_point].ch =G_point[i_point].ch;

      phi_bas += CONV_ANGLE ( G_point[i_point].grad, G_point[i_point].min,
		      G_point[i_point].sec,G_point[i_point].sing );
      phi_lin_prev = phi_lin;
      phi_lin = phi_bas;
      if (i_point!= n_points-1)
   {
      count_dist(G_point[i_point].ch,G_point[i_point+1].ch,&dist);
 phi_lin += asin((G_point[i_point].arrow - G_point[i_point+1].arrow)/dist/100.0);
    }
      line[i_point].alpha = phi_lin - phi_lin_prev;
    }//for
      return (0 );

     }
   /*====================================================================*/
  /* for ( cur_point = first_point, i_point = 0;
	 (cur_point != NULL) && (i_point < MAXPOINTS);
	 cur_point = cur_point->next, i_point++ )
   {
      line[i_point].ch = cur_point->ch;
      if( cur_point->grad == 0 && cur_point->min == 0 && cur_point->sec == 0 )
	 strcpy( line[i_point].angle, "           " );
      else
      {
	 sprintf ( line[i_point].angle, "%4dш%2d'%2d\"",
		   abs(cur_point->grad), abs(cur_point->min),
		   abs(cur_point->sec) );
	 if( cur_point->grad < 0 ||
	     cur_point->min < 0 ||
	     cur_point->sec < 0 )
	    line[i_point].angle[ strspn( line[i_point].angle, " " )-1 ] = '-';
      }
      if( cur_point->arrow == 0 )
	 strcpy( line[i_point].arrow, "       " );
      else
	 sprintf ( line[i_point].arrow, "%7.3lf",  cur_point->arrow );
      phi_bas += CONV_ANGLE ( cur_point->grad, cur_point->min,
			      cur_point->sec );
      phi_lin_prev = phi_lin;
      phi_lin = phi_bas;
      if (cur_point->next != NULL )
	 phi_lin += asin((cur_point->arrow - cur_point->next->arrow)/
		    cur_point->next->dist);
      line[i_point].alpha = phi_lin - phi_lin_prev;
      line[i_point].l =  cur_point->dist;
   } //for
   n_points = i_point;
   return ( TRUE );
}
    */
