//---------------------------------------------------------------------------
#include <string.h>
#include <vcl.h>
#include <stdio.h>
#include <math.h>
#pragma hdrstop

#include "Unit5.h" //только своё
#include "UI.h"  // extern
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfCorners *fCorners;
int read_inp_asd(char*,char);
int write_inp_asd(char*);
void freeCorners();
FILE* fp;


//---------------------------------------------------------------------------
__fastcall TfCorners::TfCorners(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------

       double Asd(AnsiString s)
  {
    char sd[128];
    strcpy(sd,s.c_str());
     for(unsigned int j=0;j<strlen(sd);j++)
   {if(sd[j]==','){sd[j]='.'; break;}}
    return atof(sd);
  }

  int read_inp_asd(char* file_name,char pr)
  { long chain;
    //float plus;
    double x,y,plus;//исправил 23
    int i;
   first_corner=NULL;
   cur_corner=NULL;
   fp = fopen ( file_name, "rt" );
   if(fp==NULL) return 1;
    n_asd=0;

//   n_corners=0;
/*fseek(file,n_corners,SEEK_SET);*/
fscanf(fp,"%d",&n_asd);
while( fscanf(fp,"\n%ld%f%lf%lf",&chain,&plus,&x,&y)==4)
{
   last_corner=cur_corner;
   if ( (cur_corner = new CORNER_POINT_STR) == NULL )
	       return 1;
   cur_corner->num=n_asd;
   cur_corner->ch.chain=chain;
   cur_corner->ch.plus=plus;
   cur_corner->x = x; cur_corner->y = y;
//   cur_corner->set_x = TRUE; cur_corner->set_y = TRUE;

   n_asd++;
   if (first_corner==NULL)  first_corner = cur_corner;

   if (last_corner != NULL) last_corner->next = cur_corner;
   cur_corner->prev = last_corner;      /*переставил*/
  }// while
  fclose(fp);
    last_corner=cur_corner;
   cur_corner->next=NULL;
   if (pr) return 0;
          ///
  //        char str[40];
            cur_corner = first_corner;

     fCorners->sgCorners->RowCount=2;//  число строк

    fCorners-> sgCorners->Rows[1]->Clear();
      i=1;
    while( cur_corner !=  NULL )
   {

fCorners->sgCorners->Cells[0][i]=AnsiString(cur_corner->ch.chain);
fCorners->sgCorners->Cells[1][i]=AnsiString(cur_corner->ch.plus);
//   sprintf(str,"%18.3lf",cur_corner->x);
     fCorners->sgCorners->Cells[2][i] = AnsiString(cur_corner->x);
     fCorners->sgCorners->Cells[3][i] = AnsiString(cur_corner->y);

     cur_corner = cur_corner->next;
     i ++ ;
     fCorners->sgCorners->RowCount++;// увеличил число строк
   }
  fCorners->sgCorners->RowCount--;
//из списка загнали в форму список больше не нужен его надо удалить
// при выходе из формы будет новый список
freeCorners();//удалил список
   return 0;

  }
  //
  write_inp_asd(char*fname)
  {
  double x,y;
    fp = fopen ( fname, "wt" );
  cur_corner = first_corner;
 fprintf(fp,"%4d",n_asd);
  while  (cur_corner !=  NULL )
  {
     x = cur_corner->x; y = cur_corner->y;

 fprintf(fp,"\n%9ld%9.4lf%18.6lf%18.6lf",cur_corner->ch.chain,cur_corner->ch.plus,x,y);//!=55)
	   cur_corner=cur_corner->next;
  }/*while*/
  fclose(fp);

 return(0);     /*!*/

  }//write_inp_asd


  void freeCorners()
{CORNER_POINT_STR * cur;
if(last_corner)
  {first_corner->prev=NULL;
  cur=last_corner->prev;
  while(cur!=NULL)
    {delete(cur->next);
     cur=cur->prev;
    }
    delete first_corner;
  }
}


void __fastcall TfCorners::FormCreate(TObject *Sender)
{   for (int i=0;i<sgCorners->RowCount;i++)
    for (int j=0;j<sgCorners->ColCount; j++)
sgCorners->Cells[j][i] =' ';//очистка

 sgCorners->Cells[0][0] = "   Пикет";
 sgCorners->Cells[1][0] = "   Плюс";
 sgCorners->Cells[2][0] = "   X";
 sgCorners->Cells[3][0] = "   Y";
}


/*void __fastcall TfCorners::sgCornersGetEditText(TObject *Sender, int ACol,
      int ARow, AnsiString &Value)
{
 // if( (ACol == 1) && sgCorners->Cells[ ACol ][ ARow ].Trim().IsEmpty() )
   // Value = "        +        ";
}
*/
//---------------------------------------------------------------------------

//void __fastcall TfCorners::FormClose(TObject *Sender, TCloseAction &Action)

void __fastcall TfCorners::Button4Click(TObject *Sender)
//записать
//создаёт список из содержимого и вызывает запись в файл и удаляет список
{
    int i = 0;
    int j;
    for (int k=1;k<sgCorners->RowCount;k++)
   {
    for (int j=2;j<sgCorners->ColCount; j++)
   {
    if(sgCorners->Cells[j][k].Trim().IsEmpty() )
      {ShowMessage("Пустые поля");
     // fCorners->Close();return;
      }
   }//j
   }//k

    //начало записи с пересчётом пикетажа
    double x,y,x1,y1,pl,s;long pk; char str[60];
    first_corner = NULL;
    cur_corner = NULL;
    cur_corner = (first_corner = new CORNER_POINT_STR);
    first_corner->prev = last_corner = NULL;
  pk=  first_corner->ch.chain = sgCorners->Cells[0][1].ToInt();
  pl=  first_corner->ch.plus =Asd(sgCorners->Cells[1][1]);
     //first_corner->ch.plus =sgCorners->Cells[1][1].ToDouble();
  x=first_corner->x = Asd( sgCorners->Cells[2][1]);
  y=first_corner->y = Asd( sgCorners->Cells[3][1]);
   s=pk*100.+pl;//начальная абсцисса
     i = 1;
         //  while( !(sgCorners->Cells[2][i+1].Trim().IsEmpty()) )
    while (i< sgCorners->RowCount-1)
{
    last_corner = cur_corner;
    cur_corner = (cur_corner->next = new CORNER_POINT_STR);
    cur_corner->prev = last_corner;

    i ++;
 //  cur_corner->ch.chain =sgCorners->Cells[0][i].ToInt();
   // cur_corner->ch.plus =Asd(sgCorners->Cells[1][i]);

  x1=  cur_corner->x = Asd( sgCorners->Cells[2][i]);
  y1=  cur_corner->y = Asd( sgCorners->Cells[3][i]);
 s=s+sqrt((x1-x)*(x1-x)+(y1-y)*(y1-y));
 j=cur_corner->ch.chain=s/100.;
   cur_corner->ch.plus=s-j*100.;
    sprintf(str,"%7.3lf", cur_corner->ch.plus);
    fCorners->sgCorners->Cells[1][i]=str;
 fCorners->sgCorners->Cells[0][i]=AnsiString(cur_corner->ch.chain);
 //fCorners->sgCorners->Cells[1][i]=AnsiString(cur_corner->ch.plus);
   x=x1; y=y1;
 }//while i
    last_corner = cur_corner;//добавил для удаления списка

    cur_corner->next = NULL;
  if(fCorners->Tag==0)   // 0- работа с кривой 1- с коридором
  {
    n_points = i;
   if( write_inp_corners(fname))//запись в файл .crv
   ShowMessage("Кривая не записана");
  } else
  {
   n_asd=i;
   if(write_inp_asd(asdfilename.c_str()))
   ShowMessage("Коридор не записан");
  }
   freeCorners();
    // надо удалить список
    first_corner = NULL;

   //очистка
   /*
    sgCorners->RowCount=2;
    for(int i = 1; i < sgCorners->RowCount ; i ++)
           sgCorners->Rows[i]->Clear();
   */
   fCorners->sgCorners->Repaint();
        ModalResult=mrOk;
        fCorners->Close();

  }
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

void __fastcall TfCorners::Button1Click(TObject *Sender)
{

sgCorners->RowCount++;
 for (int i=0;i<sgCorners->ColCount;i++)
 sgCorners->Cells[i][sgCorners->RowCount-1]="";
 sgCorners->Row=sgCorners->RowCount-1;
   sgCorners->Col=0;

}
//---------------------------------------------------------------------------

void __fastcall TfCorners::Button2Click(TObject *Sender)
{
  // Вставить
 sgCorners->RowCount++;
  for(int i=sgCorners->RowCount-1; i>=sgCorners->Row+1; i--)
  {
//    RP->Rows[i]-> Assign(RP->Rows[i-1]);
    sgCorners->Rows[i]=sgCorners->Rows[i-1];
  }
 for (int i=0;i<sgCorners->ColCount;i++)
 sgCorners->Cells[i][sgCorners->Row]="";
 sgCorners->Col=0;
}
//---------------------------------------------------------------------------

void __fastcall TfCorners::Button3Click(TObject *Sender)
{
// удалить

  int R = sgCorners->Row;
  if(sgCorners->RowCount>1)
  {
  if(sgCorners->Row == sgCorners->RowCount-1) sgCorners->Row= sgCorners->Row -1;

  for(int i=R+1;i<sgCorners->RowCount; i++)
  {
//    RP->Rows[i-1]->Assign(RP->Rows[i]);
    sgCorners->Rows[i-1]=sgCorners->Rows[i];

  }
  sgCorners->RowCount--;
  }//>1

 //  RP->Col=0;
}

//---------------------------------------------------------------------------


void __fastcall TfCorners::Button5Click(TObject *Sender)
{
  //Не записывать
        ModalResult=mrOk;
        fCorners->Close();

     // списка не создавали а тот что при чтении там и удалён
}

/*void __fastcall TfCorners::Button6Click(TObject *Sender)
{
sgCorners->RowCount=2;
 for(int i = 1; i < sgCorners->RowCount ; i ++)
           sgCorners->Rows[i]->Clear();

ModalResult=mrOk;
fCorners->Close();

} */
//---------------------------------------------------------------------------

