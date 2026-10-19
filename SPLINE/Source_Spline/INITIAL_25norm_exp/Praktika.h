//---------------------------------------------------------------------------

#ifndef PraktikaH
#define PraktikaH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <ComCtrls.hpp>
#include <Menus.hpp>
#include <Buttons.hpp>
#include <ActnList.hpp>
//---------------------------------------------------------------------------

struct DTPoint
 {
  double x1;
  double y1;
 } ; //структура для элементов массива точек

class TForm1 : public TForm
{
__published:	// IDE-managed Components
        TScrollBox *MainBox;
        TPaintBox *Fild;
        TPanel *Panel1;
        TButton *btnDelete;
        TButton *btnSave;
        TButton *btnRef;
        TLabel *Label1;
        TEdit *SizeX;
        TUpDown *ButSizeX;
        TLabel *Label4;
        TEdit *SizeY;
        TUpDown *ButSizeY;
        TButton *Button2;
        TSpeedButton *ButCenter;
        TButton *ButCancel;
        TButton *Button3;
        TEdit *Edit1;
        TCheckBox *CheckBox1;
        TProgressBar *PRB;
        TLabel *Label2;
        TButton *Button1;
       // TActionList *ActionList1;

        void __fastcall FildPaint(TObject *Sender);

        void __fastcall FildMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
        void __fastcall FildDblClick(TObject *Sender);


        void __fastcall FildMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);


        void __fastcall btnSaveClick(TObject *Sender);
        void __fastcall ButSizeXClick(TObject *Sender, TUDBtnType Button);
        void __fastcall ButSizeYClick(TObject *Sender, TUDBtnType Button);

        void __fastcall btnRefClick(TObject *Sender);
        void __fastcall FormResize(TObject *Sender);
       // void __fastcall isShowClick(TObject *Sender);

        void __fastcall FormShow(TObject *Sender);
       // void __fastcall Button1Click(TObject *Sender);
        void __fastcall Button2Click(TObject *Sender);
        void __fastcall ButCenterClick(TObject *Sender);
        void __fastcall FildMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
        void __fastcall ButCancelClick(TObject *Sender);
        void __fastcall Button3Click(TObject *Sender);
        void __fastcall btnDeleteClick(TObject *Sender);
        void __fastcall FildClick(TObject *Sender);
        void __fastcall CheckBox1Click(TObject *Sender);
        void __fastcall Button1Click(TObject *Sender);
        //void __fastcall FormPaint(TObject *Sender);
        //void __fastcall FormCreate(TObject *Sender);
        //void __fastcall FildClick(TObject *Sender);


private:
       // TProgressBar *PRB;
       //----new------
        bool MousePressed;
        int xg0, yg0, xprev, yprev;
      //-----new------
	// User declarations
public:
	// User declarations
        friend struct DTPoint;
        __fastcall TForm1(TComponent* Owner);
        int Xp (double x);
        int Yp (double y);
        int QPoint;  //количество точек исходной съемки
        double pX(double xp);
         double pY(double yp);
        void ReDraw();
        //void Project();
        bool FromFile();

        //void VeiwForm();
        void ToForm();
        //bool WriteToOutputFile();
        //void ToSysKoord(region *t); //перевод координваты х в пикет +
        //void Delete(); //функция удаления границы
        //void Disactive();
        //void Move();
        //void OutputText(region *temp); //вывод подписи к границам на рисунок
        DTPoint *MPoint; //указатель на массив точек
        int rQuantity; //количество границ
        int x1; //текущая координата по х в пикселях
        int y1;//добавил
        int regime; //режим открытия формы: 0- рассчитать схему по углограмме
        //1-по координатам 2 - рассчитать автоматически, 3-признак закрытия
        //struct region *begin; //первое звено списка
        //struct region *end; //конец списка
 };
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;

//---------------------------------------------------------------------------
#endif
 
