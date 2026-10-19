//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include"extern.h"
#include "Unit2.h"
#include "Unit1.h"
#include<stdio.h>
#include<math.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
Tel *el;
extern int newvar;
extern int razpech;
//0 при выборе варианта, чтобы вызвать оптимиз. как при newvar
//---------------------------------------------------------------------------
__fastcall Tel::Tel(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void formugl(void);//формирует файл проектной углограммы из массива elts

void Refresh_Elem(void);//вычисляет пикетаж и др.
  void write_elts(char*file_name);
  void Refresh_elts(void);//создаёт новый массив из таблицы элементов

//---------------------------------------------------------------------------
void Refresh_elts(void)
  {
  //заполняет новый массив элементов из таблицы редактирования
  n_elts=el->sgEl->RowCount-1;
 delete (elts);
 elts=new TRACE_ELTS_TAG[n_elts+1];
 min_l_first=Asd(el->Lmin->Text);
 max_l_first=Asd(el->Lmax->Text);
 l_first=Asd(el->Lfact->Text);
 min_l_last=Asd(el->Lend->Text);
  for(int i=0;i<=n_elts;i++)
    {
    elts[i].min_l_clot=Asd(el->sgEl->Cells[1][i]);
    elts[i].max_l_clot=Asd(el->sgEl->Cells[2][i]);
    elts[i].l_clot=Asd(el->sgEl->Cells[3][i]);
   elts[i].cl_ch0= StrToChainage(el->sgEl->Cells[4][i].c_str());
   elts[i].cl_ch1 = StrToChainage(el->sgEl->Cells[5][i].c_str());
   elts[i].vpas=elts[i].vgruz= 0.0;
   if(i==n_elts) break;
   elts[i].min_rad=Asd(el->sgEl->Cells[6][i]);
   elts[i].max_rad=Asd(el->sgEl->Cells[7][i]);
   elts[i].rad=Asd(el->sgEl->Cells[8][i]);
   elts[i].min_l_circ=Asd(el->sgEl->Cells[9][i]);
   elts[i].max_l_circ=Asd(el->sgEl->Cells[10][i]);
   elts[i].l_circ=Asd(el->sgEl->Cells[11][i]);
    }//for
   }//refresh_elts
//--------------------------------------------------------------------
void write_elts(char*file_name)
 {

   FILE *fp;
   fp = fopen ( file_name, "wt" );
   fprintf (fp,"%d", n_elts );
   fprintf(fp,"\n%12.6lf",l_first);
   fprintf(fp,"\n%12.6lf%12.6lf%12.6lf",min_l_first,max_l_first,min_l_last);

     for (int i=0;i<=n_elts;i++)
     {
 fprintf(fp,"\n%12.6lf%12.6lf%12.6lf",elts[i].l_clot,elts[i].min_l_clot,elts[i].max_l_clot);
 if(i==n_elts) break;
fprintf(fp,"\n%12.4lf%12.4lf%12.4lf",elts[i].rad,elts[i].min_rad,elts[i].max_rad);
fprintf(fp,"\n%12.6lf%12.6lf%12.6lf",elts[i].l_circ,elts[i].min_l_circ,elts[i].max_l_circ);
fprintf(fp,"\n%12.6lf", elts[i].h);
     }//for
     fclose(fp);
 }

//---------------------------------------------------------------------

void __fastcall Tel::sgElClick(TObject *Sender)
{
    //запрет на редактирование вычисляемых полей
      //prinst при вставить равен 1
 if(sgEl->Col<1)sgEl->Col=1;
 if((sgEl->Col<6)&&(sgEl->Col>3))sgEl->Col=6;
// if((sgEl->Col==8)||(sgEl->Col==11))sgEl->Col--;

//  if((sgEl->Col==6)&&(prinst))Refresh_Elem();
}

//---------------------------------------------------------------------------
void Refresh_Elem(void)
{    char str[127];
int j; double s=per5; long chain;
double  plus;//float
   s=s+Asd(el->Lfact->Text);
     j=0;
  while(j< el->sgEl->RowCount)
  {
 el->sgEl->Cells[0][j]=IntToStr(j+1);
 km_to_pic(s,&chain,&plus);
 sprintf(str,"%6.2f",plus);
 el->sgEl->Cells[4][j]=AnsiString(chain)+"+"+AnsiString(str);
 s=s+Asd( el->sgEl->Cells[3][j]);
km_to_pic(s,&chain,&plus);
 sprintf(str,"%6.2f",plus);
 el->sgEl->Cells[5][j]=AnsiString(chain)+"+"+AnsiString(str);
if(j<el->sgEl->RowCount-1)  s=s+Asd( el->sgEl->Cells[11][j]);
  j++;
  }//while сделать другой контроль!!!

}


void __fastcall Tel::dobClick(TObject *Sender)
{
//Добавить
 int i,j;
 //ShowMessage(fname);
 //prinst=0;
 //Refresh_Elem();
 el->MClose->Tag=1;  //для записи при открытой оптимизации
 sgEl->RowCount++;
 for ( i=0;i<sgEl->ColCount;i++)
 sgEl->Cells[i][sgEl->RowCount-1]="";
 sgEl->Row=sgEl->RowCount-1;
 sgEl->Col=1;
 j=sgEl->RowCount;
 //sprintf(buf,"%d",j-1);
// sgEl->Cells[0][j-1]=buf;
 sgEl->Cells[0][j-1]=AnsiString(j);;
//  j=StrToInt(nkriv->Text); j++;
 // nkriv->Text=IntToStr(j);
  nkriv->Text=  IntToStr(sgEl->RowCount);
ShowMessage("После редактирования сформировать пикетаж");

}
//---------------------------------------------------------------------------

void __fastcall Tel::udalClick(TObject *Sender)
{
// удалить
   double s=0.0; //float s=0.0;
  int i,j;
  int R = sgEl->Row;
el->MClose->Tag=1;  //для записи при открытой оптимизации
  if(sgEl->RowCount>2)
  {
   if(sgEl->Row ==sgEl->RowCount-1)
    { sgEl->Row--;
     for( j=6;j<12;j++)
     sgEl->Cells[j][sgEl->Row]="";
     } else//не последняя
     {
     //вычисление удаляемой длины
     s=Asd(sgEl->Cells[3][R])+Asd(sgEl->Cells[11][R]);
     // при удалении первой кривой надо менять l_first
// при остальных же менять длину перед удаляемой
     if(R==0)
     {
       l_first+=s;
       Lfact->Text=FloatToStr(l_first);
     }
     else
     {
 //удаляется не первая
 sgEl->Cells[11][R-1]=FloatToStr(s+Asd(sgEl->Cells[11][R-1]));

     }
  for( i=R+1;i<sgEl->RowCount; i++)
  {
  for( j=1;j<sgEl->ColCount;j++)// от 1 для сохранения номеров
    sgEl->Cells[j][i-1]=sgEl->Cells[j][i];
   }
      } //else
  sgEl->RowCount--;

  nkriv->Text=  IntToStr(sgEl->RowCount);
  }//>2
  else{ ShowMessage("Единственная кривая не удаляется. Редактируйте!");
       return;
      }
   //ShowMessage("После редактирования сформировать пикетаж");
}
//---------------------------------------------------------------------------

void __fastcall Tel::vstClick(TObject *Sender)
{
  // Вставить
//  prinst=1;
el->MClose->Tag=1;  //для записи при открытой оптимизации
 sgEl->RowCount++;
  for(int i=sgEl->RowCount-1; i>=sgEl->Row+1; i--)
  {
    sgEl->Rows[i]=sgEl->Rows[i-1];

  }
 for (int i=0;i<sgEl->ColCount;i++)
 sgEl->Cells[i][sgEl->Row]="";
 sgEl->Col=1;
ShowMessage("После редактирования сформировать пикетаж");

  // Refresh_Elem();

}
void formugl(void)
{  int i,j;
     char * ima=new char[80];
      strcpy(ima,elt_name);
      j=strlen(elt_name);
     ima[j-3]='g';
     //ShowMessage(ima); формирует файл .g??
     FILE *f;
     f=fopen(ima,"wt");
     i=(n_elts+1)*2+1;
     fprintf(f,"%5d",i);
     double s=l_first,sig;
     int pr;
     fprintf(f,"\n%12.6lf%5d%12.9lf",
                   s,0,0.0); //граница, тип, кривизна

      fprintf(f,"%10ld%10.4lf",
               elts[0].cl_ch0.chain,elts[0].cl_ch0.plus);
     for (j=0;j<=n_elts;j++)
     {
     s=s+elts[j].l_clot;
/*     fprintf(f,"\n%10.2lf%5d%10.6lf",
                  s,1,0.0);
     fprintf(f,"%10ld%10.2lf",
                  elts[j].cl_ch1.chain,elts[j].cl_ch1.plus);
*/
    fprintf(f,"\n%12.5lf%5d%10.6lf%10ld%10.4lf",
                  s,1,0.0,elts[j].cl_ch1.chain,elts[j].cl_ch1.plus);
//     fprintf(f,"%10ld%10.2lf",
  //                elts[j].cl_ch1.chain,elts[j].cl_ch1.plus);


     if(j==n_elts) break;
     if(fabs(elts[j].rad)>1.0){pr=2;sig=1.0/elts[j].rad;} else
                              {pr=0; sig=0.0;}
                       s=s+elts[j].l_circ;
     fprintf(f,"\n%12.5lf%5d%12.8lf%10ld%10.4lf",
                  s,pr,sig,elts[j+1].cl_ch0.chain,elts[j+1].cl_ch0.plus);

     }
     count_dist(elts[n_elts].cl_ch1, tr[n_pts-1].ch,&sig );
     s=s+sig;
fprintf(f,"\n%12.6lf%5d%10.6lf%10ld%10.4lf",
                  s,0,0.0,tr[n_pts-1].ch.chain,tr[n_pts-1].ch.plus);
     fclose(f);
     delete(ima);

 }

//---------------------------------------------------------------------------
void __fastcall Tel::MCloseClick(TObject *Sender)
{
//Записать
int i,j;
if(MClose->Tag==1)//1 при редактировании полей формы el для формирования
//по кнопке Записать оказался 1
//по записи мин и макс начальной длины УБРАЛ и теперь ноль
//1 после обнаружения редактирования элементов
          {
Refresh_Elem(); //ФОРМИРОВАНИЕ ТАБЛИЦЫ повтор не страшен
for(i=0;i<sgEl->ColCount;i++)
{
for(j=0;j<sgEl->RowCount-1;j++)
   { if(sgEl->Cells[i][j].IsEmpty())
    { ShowMessage("Есть пустые поля");
      return;
    }  //if
   }   //j
 }//i
Refresh_elts();  //формирование массива elts
write_elts (elt_name);
//   new_shifts(); по эвольвентам на новые элементы плохо !!!
    trace_show();
    write_trace(trackfilename.c_str());
     //запись uch_name.ugl для Praktika
  formugl();//формирует проектную углограмму для Praktika файл .g??
  MClose->Tag=0;
            } //if   if(MClose->Tag==1)
 //el->Tag=1; // теперь это по кнопке Старт
 el->Close();
 //Для нового варианта вызываем оптимизатор
  if(newvar==1|| razpech==0)
  {  el->Tag=1;
  Form1->Button5Click(NULL);
  }
}
//---------------------------------------------------------------------------
void __fastcall Tel::Button1Click(TObject *Sender)
{
//set_var=0;
//Не записывать

//el->Tag=0; теперь по кнопке Возврат
el->Close();
}
void __fastcall Tel::Button4Click(TObject *Sender)
{
  //Сформировать
  int i,j;
  for(i=1;i<sgEl->ColCount;i++)
 { if((i==4)||(i==5))continue;
  for(j=0;j<sgEl->RowCount-1;j++)
 {
 if(sgEl->Cells[i][j].IsEmpty())
{ ShowMessage("Есть пустые поля");
return;
}
}//j
}//i

Refresh_Elem(); //ФОРМИРОВАНИЕ ТАБЛИЦЫ
}//void
//---------------------------------------------------------------------------




void __fastcall Tel::sgElSetEditText(TObject *Sender, int ACol, int ARow,
      const AnsiString Value)
{     MClose->Tag=1;//если останется нуль значит не редактировали
//случается при редактировании ACol ARow всякий раз
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------

void __fastcall Tel::Button2Click(TObject *Sender)
{  //старт на форме элементов
el->Tag=1;
el->Close();
}
//---------------------------------------------------------------------------

void __fastcall Tel::Button3Click(TObject *Sender)
{
el->Tag=0;
el->Close();  //выход на форме элементов
}
//---------------------------------------------------------------------------

void __fastcall Tel::LminChange(TObject *Sender)
{
//el->MClose->Tag=1;
}
//---------------------------------------------------------------------------

void __fastcall Tel::LmaxChange(TObject *Sender)
{
//el->MClose->Tag=1;
}
//---------------------------------------------------------------------------

void __fastcall Tel::LfactChange(TObject *Sender)
{
//el->MClose->Tag=1;
}
//---------------------------------------------------------------------------

