//---------------------------------------------------------------------------
#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include "Unit5.h"
#include "Unit1.h"  // там extern AnsiString Dir
#include "Unit6.h"
#include "myfun.h"
#include <string.h>
         /*
#include <FileCtrl.hpp>
#include <windows.h>
#include <process.h>
#include <dir.h>
#include "myfun.h"
*/

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm5 *Form5;

    STARTUPINFO si;
    PROCESS_INFORMATION pi;

  void vizov(char*);
  void vizov(char* st)
  {
    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    ZeroMemory( &pi, sizeof(pi) );
    // Start the child process.
    if( !CreateProcess( NULL, // No module name (use command line).
        st, // Command line.
        NULL,             // Process handle not inheritable.
        NULL,             // Thread handle not inheritable.
        FALSE,            // Set handle inheritance to FALSE.
        0,                // No creation flags.
        NULL,             // Use parent's environment block.
        NULL,             // Use parent's starting directory.
        &si,              // Pointer to STARTUPINFO structure.
        &pi          )   // Pointer to PROCESS_INFORMATION structure.
    )
    {
        //ErrorExit( "CreateProcess failed prof1." ); нет такой функции?
        Application->MessageBox("CreateProcess failed.",st,MB_OK|MB_ICONERROR);
    }

    // Wait until child process exits.
    WaitForSingleObject( pi.hProcess, INFINITE );

    // Close process and thread handles.
    CloseHandle( pi.hProcess );
    CloseHandle( pi.hThread );

  }


//---------------------------------------------------------------------------
__fastcall TForm5::TForm5(TComponent* Owner)
        : TForm(Owner)
{


}
//---------------------------------------------------------------------------

/*void __fastcall TForm5::Button1Click(TObject *Sender)
{   //был пикетаж теперь он в выборе перегона
   Form1->AlphaBlendValue=2; //зачем это
 Form6->ShowModal();
}
*/
//---------------------------------------------------------------------------
     /*
void __fastcall TForm5::Button8Click(TObject *Sender) // вызов диалога открыти€ файла.
{
OpenDialog1->InitialDir=Dir;
OpenDialog1->Execute();
AnsiString tmp=OpenDialog1->Files[0].Text.SubString(0,OpenDialog1->Files[0].Text.Length()-2);
Form11->Memo1->Lines->LoadFromFile(tmp);
// заполнение Memo1 на вызываемой форме текстом из открытого файла
Form11->ShowModal();
}      */
//---------------------------------------------------------------------------

void __fastcall TForm5::Button9Click(TObject *Sender)
{
 Form5->Close();
}
//---------------------------------------------------------------------------

void __fastcall TForm5::Button4Click(TObject *Sender)
{// по координатам
 char s[128];
 //Dir.c_str() и текуща€ полный путь к участку с данными
//strcpy(s,Dir.c_str());//общий путь C:/Omcopy/Yber
strcpy(s,"..\\rxt1_sr_win.exe ");
strcat(s,Dir.c_str());
ShowMessage(s);
vizov(s);

}
//---------------------------------------------------------------------------

void __fastcall TForm5::Button2Click(TObject *Sender)
{ // ввод и обработка данных
char s[128];
// лишн€€ strcpy(s,Dir.c_str());//общий путь к данным
//текуща€ не мен€етс€, всегда на конце участок с данными
strcpy(s,"..\\i.exe ");
strcat(s,Dir.c_str());
ShowMessage(s);
vizov(s);
}
//---------------------------------------------------------------------------

