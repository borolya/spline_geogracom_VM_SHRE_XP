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
//---------------------------------------------------------------------------
struct region
  {
    region *prev; //ссылка на предыдущий элемент списка
    region *next;  //ссылка на следующий элемент списка
    double regionX; // координата Х границы относ первого элемента
    int type; //0 - прямая 1 - клотойда 2 - окружность
    double krivizna; //кривизна рассчитывается если тип - оружность, иначе =0
    bool active; //признак выбрана ли граница
    long picet; //номер пикета
    double plus; //добавка к пикету
  //  double Xnorm; //координата относительно общей системы коорд

   };
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
        TPopupMenu *PopupMenu1;
        TMenuItem *N4;
        TMenuItem *N3;
        TMenuItem *N2;
        TPanel *Panel1;
        TButton *btnDisactive;
        TButton *btnMove;
        TButton *btnDelete;
        TButton *btnSave;
        TButton *btnRef;
        TLabel *Label1;
        TEdit *SizeX;
        TUpDown *ButSizeX;
        TLabel *Label4;
        TEdit *SizeY;
        TUpDown *ButSizeY;
        TCheckBox *isShow;
        TButton *Button1;
        TButton *Button2;
        TSpeedButton *ButCenter;
        TButton *ButCancel;
        TButton *Button3;

        void __fastcall FildPaint(TObject *Sender);

        void __fastcall FildMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
        void __fastcall FildDblClick(TObject *Sender);


        void __fastcall FildMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
        void __fastcall btnDelete1Click(TObject *Sender);

        void __fastcall N1Click(TObject *Sender);
        void __fastcall N4Click(TObject *Sender);
        void __fastcall N2Click(TObject *Sender);
        void __fastcall N3Click(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);

        void __fastcall btnDisactiveClick(TObject *Sender);
        void __fastcall btnMoveClick(TObject *Sender);
        void __fastcall btnDeleteClick(TObject *Sender);
        void __fastcall btnSaveClick(TObject *Sender);
        void __fastcall ButSizeXClick(TObject *Sender, TUDBtnType Button);
        void __fastcall ButSizeYClick(TObject *Sender, TUDBtnType Button);

        void __fastcall btnRefClick(TObject *Sender);
        void __fastcall FormResize(TObject *Sender);
        void __fastcall isShowClick(TObject *Sender);

        void __fastcall FormShow(TObject *Sender);
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall Button2Click(TObject *Sender);
        void __fastcall ButCenterClick(TObject *Sender);
        void __fastcall FildMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
        void __fastcall ButCancelClick(TObject *Sender);
        void __fastcall Button3Click(TObject *Sender);


private:
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
        int QPoint;  //колличество точек исходной съемки
        double pX(double xp);
        void ReDraw();
        void Project();
        bool FromFile();

        //void VeiwForm();
        void ToForm();
        bool WriteToOutputFile();
        void ToSysKoord(region *t); //перевод координваты х в пикет +
        void Delete(); //функция удаления границы
        void Disactive();
        void Move();
        void OutputText(region *temp); //вывод подписи к границам на рисунок
        DTPoint *MPoint; //указатель на массив точек
        int rQuantity; //количество границ
        int x1; //текущая координата по х в пикселях
        int regime; //режим открытия формы: 0- рассчитать схему автоматически
        //1-записанный вариант 2 - рассчитать вручную  3-признак закрытия
        struct region *begin; //первое звено списка
        struct region *end; //конец списка


};







//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif


