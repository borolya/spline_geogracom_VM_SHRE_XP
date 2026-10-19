//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "extern.h"
#include <stdio.h>
//---------------------------------------------------------------------------
USEFORM("Unit1.cpp", Form1);
USEFORM("UnFManFiles.cpp", FManFiles);
USEFORM("Unit2.cpp", el);
USEFORM("Unit4.cpp", Form4);
USEFORM("Unit5.cpp", Form5);
//---------------------------------------------------------------------------
char pereg[30];

 struct rp1
  { long pk;
    float pl;
    double km;
  }rp[50];
 int krp;
  FILE* rpfile;
  double a1;

  bool SetCurDir( char * pDirPath )
{//Не нужна, получаем полный путь
  char buf_path[ 128 ];

   GetCurrentDirectory(128, buf_path);
  return SetCurrentDirectory( strcat( strcat(buf_path, "\\"),
   pDirPath ) );
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR PDirPath , int)
{
char s[128];
//PDirPath="TESTMold";
 if(! SetCurrentDirectory(PDirPath))
 ShowMessage("Невозможно найти путь" );
         else
 { ShowMessage (PDirPath);//отладка убрать эту строку
   rpfile=fopen("..\\rp1.lst","rt");
 if(rpfile==NULL)
{ShowMessage("Не задано соответствие пикетажа, файл RP1.lst"); exit(0);
return 1;
}
  int i=0;
   while(!feof(rpfile))
  {
 if ( fscanf(rpfile,"%lf%f%lf",&a1,&rp[i].pl,&rp[i].km)==3)
    { rp[i].pk=(long)a1;   i++;}
  }
  krp=i;
   fclose(rpfile);
}
strcpy(s, PDirPath);
int j=0;
for (int i=strlen(s)-1;i>0;i--)
{if(s[i]=='\\'){j=i+1; break;}
}
strcpy(pereg,&s[j]);
        try
        {
                 Application->Initialize();
                 Application->CreateForm(__classid(TForm1), &Form1);
                 Application->CreateForm(__classid(TFManFiles), &FManFiles);
                 Application->CreateForm(__classid(Tel), &el);
                 Application->CreateForm(__classid(TForm4), &Form4);
                 Application->CreateForm(__classid(TForm5), &Form5);
                 Application->Run();
        }
        catch (Exception &exception)
        {
                 Application->ShowException(&exception);
        }
        catch (...)
        {
                 try
                 {
                         throw Exception("");
                 }
                 catch (Exception &exception)
                 {
                         Application->ShowException(&exception);
                 }
        }

        return 0;
}
//---------------------------------------------------------------------------
