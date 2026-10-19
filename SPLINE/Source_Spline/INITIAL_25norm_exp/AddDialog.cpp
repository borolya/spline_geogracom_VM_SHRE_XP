//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "AddDialog.h"
#include "Praktika.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm2 *Form2;
//---------------------------------------------------------------------------
struct region *rex = new region;

void insert(region *r);
//---------------------------------------------------------------------------
__fastcall TForm2::TForm2(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TForm2::OkClick(TObject *Sender)
{
  if (Form1->end ==NULL)   //инициализируем конец списка границей через последнююю точку
    {
       region *end = new region;
       end->prev = NULL;
       end->next = NULL;
       end->regionX = Form1->MPoint[Form1->QPoint-1].x1;
       end->type = 0; //последн€€ граница
       end->krivizna = 0;
       end->active = false;
     //  Form1->ToSysKoord(end);
       Form1->end = end;
       Form1->rQuantity = 1;
    }

   struct region *rex = new region;
   //заполн€ем структуру
    if (List->Text=="")
       {
         ShowMessage("Ќе выбран тип отрезка!");
         return;
       }

    rex->regionX = Form1->pX(Form1->x1);

    rex->active = false;
    Form1->ToSysKoord(rex);

    insert(rex);
     if (List->Text == "ѕр€ма€")
          {
                rex->type = 0;
                rex->krivizna = 0;
          }

    if (List->Text == " лотоида")
        {
                rex->type = 1;
                rex->krivizna = 0;
        }
   if (List->Text == "ќкружность")
        {
                rex->type = 2;
                if (!Form2->krivizna(rex))
                   {ShowMessage ("ќшибка в рассчете кривизны");} //здесь нужно рассчитать кривизну
        }

    Form1->Fild->Canvas->Pen->Color = clBlue;
    //Form1->Fild->Canvas->MoveTo(Form1->x1 , Form1->Fild->Height);
    //Form1->Fild->Canvas->LineTo(Form1->x1 , 0);
    Form1->ReDraw();
    Form1->rQuantity++;
    //Form1->OutputText(rex);
    Form2->ModalResult = mrOk;
    //AddToList(List->Text);
}
//---------------------------------------------------------------------------


void __fastcall TForm2::CancelClick(TObject *Sender)
{
   Form2->ModalResult = mrCancel;     
}
//---------------------------------------------------------------------------
bool TForm2::krivizna (region* r)
{
  int i;
  int nBeg; //номенр точки наход€ща€с€ слева от пр€мой
  int nEnd; //номер  точки наход€ща€с€ справа от пр€мой
  double yRegRight; //y правой границы
  for (i=0;i<Form1->QPoint-1;i++)
    {
      if ((r->regionX >=Form1->MPoint[i].x1)&&(r->regionX<=Form1->MPoint[i+1].x1))
        {   //находим отрезок внутри которого проходит граница
           nBeg = i;
           nEnd = i+1;
           break;
        }
    }
   yRegRight = Form1->MPoint[nBeg].y1 + (Form1->MPoint[nEnd].y1-Form1->MPoint[nBeg].y1)/(Form1->MPoint[nEnd].x1-Form1->MPoint[nBeg].x1)*(r->regionX-Form1->MPoint[nBeg].x1);
   double xRegLeft;
   double yRegLeft;


   if (r->prev!=NULL)
   {
     xRegLeft = r->prev->regionX;
     for (i=0;i<Form1->QPoint-1;i++)
     {
      if ((xRegLeft>=Form1->MPoint[i].x1)&&(xRegLeft<=Form1->MPoint[i+1].x1))
        {   //находим отрезок внутри которого проходитс€ предидуща€ граница(начало участка)
           nBeg = i;
           nEnd = i+1;
           break;
        }
     }
      yRegLeft = Form1->MPoint[nBeg].y1 + (Form1->MPoint[nEnd].y1-Form1->MPoint[nBeg].y1)/(Form1->MPoint[nEnd].x1-Form1->MPoint[nBeg].x1)*(xRegLeft-Form1->MPoint[nBeg].x1);
   }
   else
        {
                xRegLeft = 0;
                yRegLeft = 0;
        }
   r->krivizna = (yRegRight-yRegLeft)/(r->regionX-xRegLeft);
   return true;
}

//---------------------------------------------------------------------
void insert(region *r)
{
  struct region *curr = new region;
   if (Form1->begin == NULL)//список пуст
       {
        r->prev = NULL;
        Form1->begin = r;
        r->next = Form1->end;
        Form1->end->prev = r;
        return;
       }
  curr = Form1->begin;

        while (curr!=NULL)
        {
           if (curr->regionX > r->regionX)
             {
             if (curr==Form1->begin)
             {
               r->prev = NULL;
               curr->prev = r;
               r->next = curr;
               Form1->begin = r;
               return;
             }
             else
             {
              r->prev = curr->prev;
              curr->prev->next = r;
              curr->prev = r;
              r->next = curr;
              if (r->prev->type == 2)  //если окружность пересчитать кривизну
                 {
                   if (!Form2->krivizna(r->prev))
                     {ShowMessage ("ќшибка в рассчете кривизны");}
                 }
              return;
             }
           }
           curr = curr->next;
        }
}

