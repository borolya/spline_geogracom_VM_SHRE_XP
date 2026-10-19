//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include<stdio.h>
#include "Unit4.h"
#include "Unit5.h"
#include "UI.h"
#include "pppUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfElems *fElems;
extern  TfCorners *fCorners;
void Refresh_Elem(void);
char buf[128];
// int pic_to_km(  long pic, float plus,double *km);
//CHAINAGE StrToChainage(char *sp)
  int prinst=0;// при заходе вo вставить станет 1
double km,km1;
CHAINAGE ch;
CHAINAGE *fix;
TRACE_ELTS *elts;
int n_elts,n_fix;
int write_inp_elts(char*);
int read_inp_elts(char*);
//---------------------------------------------------------------------------
__fastcall TfElems::TfElems(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TfElems::FormCreate(TObject *Sender)
{ sgElems->Cells[0][0] = " № ";
  sgElems->Cells[1][0] = " Длина ";
  sgElems->Cells[2][0] = " Пикет ";
  sgElems->Cells[3][0] = " Плюс ";
  sgElems->Cells[4][0] = " Пикет ";
  sgElems->Cells[5][0] = " Плюс ";
  sgElems->Cells[6][0] = " Радиус ";
  sgElems->Cells[7][0] = " Длина ";
  // очистка в USZD

}
//---------------------------------------------------------------------------
void __fastcall TfElems::sgElemsClick(TObject *Sender)
{     //запрет на редактирование вычисляемых полей
      //prinst при вставить равен 1
 if(sgElems->Col<2)sgElems->Col=2;
 if(sgElems->Col==7)sgElems->Col=6;
  if((sgElems->Col==6)&&(prinst))Refresh_Elem();
}
//---------------------------------------------------------------------------
void Refresh_Elem(void)
{
int j; double s;
  j=1;
  while(j< fElems->sgElems->RowCount)
  {
  strcpy(buf, fElems->sgElems->Cells[2][j].c_str());
//  if(strlen(buf)==0){fElems-> sgElems->RowCount=j+1; break;}
  sscanf(buf,"%ld",&ch.chain);

//  strcpy(buf, fElems->sgElems->Cells[2][j].c_str());
 // sscanf(buf,"%lf",&ch.plus);
ch.plus=Asd(fElems->sgElems->Cells[3][j].c_str());
  pic_to_km(ch.chain,ch.plus,&km);//начало

 if(j>1)
 {//Длина круговой или прямой предшествующего элемента
  s=km-km1;
  sprintf(buf,"%10.3lf",s);
 fElems->sgElems->Cells[7][j-1]=buf;
 }
  strcpy(buf,fElems->sgElems->Cells[4][j].c_str());
  //if(strlen(buf)==0){ fElems->sgElems->RowCount=j+1; break;}
  sscanf(buf,"%ld",&ch.chain);
  //strcpy(buf, fElems->sgElems->Cells[5][j].c_str());
  //sscanf(buf,"%lf",&ch.plus);

  ch.plus=Asd(fElems->sgElems->Cells[5][j].c_str());
  pic_to_km(ch.chain,ch.plus,&km1);
 //конец оставил для вычисления длины круговой
 s=km1-km;
 sprintf(buf,"%10.3lf",s);
 fElems->sgElems->Cells[1][j]=buf; //Вычислили длину переходной в последней строке
 sprintf(buf,"%d",j);
 fElems->sgElems->Cells[0][j]=buf;
  j++;
  }//while сделать другой контроль!!!


}
void __fastcall TfElems::Button1Click(TObject *Sender)
{//    int km_to_pic(double km,long *pic,float *plus);
  //int pic_to_km(  long pic, float plus,double *km);
//Добавить
 int i,j;
 ShowMessage(fname);
 prinst=0;
 Refresh_Elem();
 sgElems->RowCount++;
 for ( i=0;i<sgElems->ColCount;i++)
 sgElems->Cells[i][sgElems->RowCount-1]="";
 sgElems->Row=sgElems->RowCount-1;
 sgElems->Col=1;
 j=sgElems->RowCount;
 sprintf(buf,"%d",j-1);
 sgElems->Cells[0][j-1]=buf;


}
//---------------------------------------------------------------------------

void __fastcall TfElems::Button2Click(TObject *Sender)
{
  // Вставить
  prinst=1;
 sgElems->RowCount++;
  for(int i=sgElems->RowCount-1; i>=sgElems->Row+1; i--)
  {
    sgElems->Rows[i]=sgElems->Rows[i-1];
  }
 for (int i=0;i<sgElems->ColCount;i++)
 sgElems->Cells[i][sgElems->Row]="";
 sgElems->Col=1;
  if(sgElems->Row>1) sgElems->Cells[7][sgElems->Row-1]="";

}
//---------------------------------------------------------------------------

void __fastcall TfElems::Button3Click(TObject *Sender)
{
// удалить
 prinst=0;
  int R = sgElems->Row;
  if(sgElems->RowCount>1)
  {
  if(sgElems->Row ==sgElems->RowCount-1) sgElems->Row--;

  for(int i=R+1;i<sgElems->RowCount; i++)
  {
    sgElems->Rows[i-1]=sgElems->Rows[i];
  }
  sgElems->RowCount--;
  Refresh_Elem();
      sgElems->Cells[7][sgElems->RowCount-1]="";
  }//>1

 //  RP->Col=0;

}
//---------------------------------------------------------------------------

void __fastcall TfElems::Button4Click(TObject *Sender)
{   ShowMessage(fname);
  //Записать
  Refresh_Elem();
/*  strcpy(buf,fname);fmane- имя кривой вместе с сrv
  strcat(buf,".crv");
  */

if(MessageDlg("Есть дополнительные точки?",mtWarning,TMsgDlgButtons() << mbYes<<mbNo,0)==mrYes)
  { fElems->Close();

    if(!n_fix)
        {fCorners->sgCorners->RowCount=2;
         fCorners->sgCorners->Rows[1]->Clear();// очистка первой строки
        }
         else
               {
               fCorners->sgCorners->RowCount=n_fix+1;
               for(int i=0;i<n_fix;i++)
                  {
         fCorners->sgCorners->Cells[0][i+1]=AnsiString(fix[i].chain);
         fCorners->sgCorners->Cells[1][i+1]=AnsiString(fix[i].plus);
                   }
                }//n_fix
      fCorners->sgCorners->ColCount=2;
       fCorners->Caption="Доп. точки";
       fCorners->Tag=2;
       fCorners->ShowModal();


        fCorners-> sgCorners->RowCount=2;
        fCorners-> sgCorners->ColCount=4;
    for (int k=1;k<fCorners->sgCorners->RowCount;k++)
   {
     fCorners->sgCorners->Rows[k]->Clear();
   }
               fCorners->Tag=0;

//        fCorners->Close();
  }//dop

  if( write_inp_elts(fname))
 ShowMessage("Таблица не заполнена. Данные не записаны");
 else {
     sgElems->RowCount=2;
     fElems->Close();
      }
}
//---------------------------------------------------------------------------

void __fastcall TfElems::Button5Click(TObject *Sender)
{
  //Не записывать
     sgElems->RowCount=2;

    for(int i = 1; i < sgElems->RowCount ; i ++)
           sgElems->Rows[i]->Clear();
           fElems->Close();


}
//---------------------------------------------------------------------------

   int read_inp_elts(char*fp)
   {
   FILE*f;
    int j;
    double rad;
//            j=1;

   f=fopen(fp,"rt");
    //type_survey=ELEM;
    fscanf(f,"%d",&type_survey);
   // n_elts=fElems->sgElems->RowCount-1;
    fscanf(f,"\n%d%d",&n_elts,&n_fix);//добавил n_fix
    fElems->sgElems->RowCount=n_elts+1;
     for (int i=1;i<=n_elts;i++)//нумерую элементы с 1 до n_elts
                                // в соответствии с номером строки в таблице
   {
     fscanf(f,"\n%ld%lf",&ch.chain,&ch.plus);
     fElems->sgElems->Cells[0][i]=AnsiString(i);
     fElems->sgElems->Cells[2][i]=AnsiString(ch.chain);
     fElems->sgElems->Cells[3][i]=AnsiString(ch.plus);
    fscanf(f,"\n%ld%lf",&ch.chain,&ch.plus);
     fElems->sgElems->Cells[4][i]=AnsiString(ch.chain);
     fElems->sgElems->Cells[5][i]=AnsiString(ch.plus);
    fscanf(f,"\n%lf",&rad);
     fElems->sgElems->Cells[6][i]=AnsiString(rad);
    }//for i
    Refresh_Elem();
    if(n_fix)
    {fix=new CHAINAGE[n_fix];
     for (int i=0;i<n_fix;i++)
     fscanf(f,"\n%ld%lf",&fix[i].chain,&fix[i].plus);//прочитал массив фиксированных точек
     }
      fclose(f);

   return 0;
   }//read_int_elts

   int write_inp_elts(char*fp)
   {
   FILE*f;
    int j;
    double rad;

//            j=1;
/*  while( j<fElems->sgElems->RowCount)
   {
     for ( int i=1;i<8;i++)
    if(fElems->sgElems->Cells[i][j].Trim().IsEmpty())
    {ShowMessage("Пустые поля");fElems->Close();return 1;}
      j++;
   }//while j
  */
   f=fopen(fp,"wt");
    type_survey=ELEM;
    fprintf(f,"%d",type_survey);
    n_elts=fElems->sgElems->RowCount-1;
    fprintf(f,"\n%5d%5d",n_elts,n_fix);
    //fElems->sgElems->RowCount=n_elts;
     for (int i=1;i<=n_elts;i++)//нумерую элементы с 1 до n_elts
                                // в соответствии с номером строки в таблице
   {
       ch.chain=fElems->sgElems->Cells[2][i].ToInt();
       ch.plus=Asd( fElems->sgElems->Cells[3][i]);
     fprintf(f,"\n%9ld%9.3lf",ch.chain,ch.plus);
    ch.chain=fElems->sgElems->Cells[4][i].ToInt();
       ch.plus=Asd( fElems->sgElems->Cells[5][i]);
     fprintf(f,"\n%9ld%9.3lf",ch.chain,ch.plus);
       rad=Asd( fElems->sgElems->Cells[6][i]);
     fprintf(f,"\n%9.3lf",rad);
    }//for i
//    Refresh_Elem();
    for (int i=0;i<n_fix;i++)
    fprintf(f,"\n%9ld%9.3lf",fix[i].chain,fix[i].plus);

  fclose(f);
   return 0;
   }//write_int_elts


/*void __fastcall TfElems::sgElemsGetEditText(TObject *Sender, int ACol,
      int ARow, AnsiString &Value)
{
  if(((ACol == 2)||(ACol == 3))&& sgElems->Cells[ACol][ARow].Trim().IsEmpty())
    Value = "      +       ";

}
*/
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------

