//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include<string.h>
#include <stdio.h>
USEFORM("Praktika.cpp", Form1);
USEFORM("MainMenu.cpp", Form4);
//---------------------------------------------------------------------------
char igl_name[40];//имя файла исходной углограммы
char ini_name[40];//X,Y в новой системе  для показа кривой
extern int read_rp();
FILE*ff;
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR pDirPath, int)
{  ff=fopen("ris.txt","wt");
  if ( read_rp())
  {ShowMessage("INI WinMain Нет RP"); return 1;}
 
 //pDirPath="Reg2";
 //ShowMessage( pDirPath); только имя участка
 strcpy(igl_name,pDirPath);
 strcat(igl_name,".igl");

 strcpy(ini_name,pDirPath);
 strcat(ini_name,".ini");
  ShowMessage(ini_name);
        try
        {
                 Application->Initialize();
                 Application->Title = "INI";
                 Application->CreateForm(__classid(TForm1), &Form1);
                 Application->CreateForm(__classid(TForm4), &Form4);
                 Application->Run();
         }
        catch (Exception &exception)
        {
                 Application->ShowException(&exception);
        }
                return 0;
}
//---------------------------------------------------------------------------
