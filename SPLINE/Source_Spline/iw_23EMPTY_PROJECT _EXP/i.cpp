//---------------------------------------------------------------------------

#include <vcl.h>
#include <stdio.h>
#pragma hdrstop
//---------------------------------------------------------------------------
USEFORM("pppUnit.cpp", MainForm);
USEFORM("UFTRACKS.CPP", fTrack);
USEFORM("Unit5.cpp", fCorners);
USEFORM("USZD.cpp", ftype);
USEFORM("UnFManFiles.cpp", FManFiles);
//---------------------------------------------------------------------------
extern CHAINAGE;  //   UI.h
 extern struct rp1 //Finkriwi
  { long pk;
    float pl;
    double km;
  }rp[50];
  extern int krp;  //Finkriwi
  FILE* rpfile;
  double a1;
  char pereg[30];
  int newvvod(void)
  {
    rpfile=fopen("..//RP1.lst","rt");
 if(rpfile==NULL)
{ShowMessage("Не задано соответствие пикетажа, файл RP1.lst ");
  return 1;
 }
  int i=0;
   while(!feof(rpfile))
  {
    if( fscanf(rpfile,"%lf%f%lf",&a1,&rp[i].pl,&rp[i].km)==3)
     { rp[i].pk=(long)a1; i++;}
  } //while
  krp=i;
 //for(i=0;i<krp;i++)
  //printf("\n pk=%6ld,pl=%6.2f,km=%10.2lf",rp[i].pk,rp[i].pl,rp[i].km);
  //getch();
     fclose(rpfile);
     return 0;
  }//newvvod
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR pDirPath, int)
{
 //pDirPath="Test1";
 char s[128];
int j;

strcpy(s, pDirPath);
ShowMessage(pDirPath); //полный путь с участком
   j=-1;
for(int i=0;s[i];i++)
if((s[i]=='\\')&&(s[i+1]!='\0')) j=i;
strcpy(pereg,&s[j+1]);
//pereg  нужен  в шапку формы
 //strcpy(pereg,pDirPath);//убрать в системе!!!

//ShowMessage(pereg);//  имя перегона

   if(! SetCurrentDirectory(pDirPath))
   {ShowMessage("Невозможно найти путь" );return 1;}

if(newvvod())
   {ShowMessage("Не читается соответствие пикетажа rp1.lst" );
      return 1;}
//===

        try
        {
  Application->Initialize();
  Application->Title="i";
  Application->CreateForm(__classid(TMainForm), &MainForm);
                 Application->CreateForm(__classid(TfTrack), &fTrack);
                 Application->CreateForm(__classid(TfCorners), &fCorners);
                 Application->CreateForm(__classid(Tftype), &ftype);
                 Application->CreateForm(__classid(TFManFiles), &FManFiles);
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
//-------------------------------------------------------------------------
bool SetCurDir( char * pDirPath )
{// Не нужна получает полный путьдаёт переход на уровень выше
//и при вызове уже установлена директория
  char buf_path[ 128 ];

   GetCurrentDirectory(128, buf_path);

  return  SetCurrentDirectory( strcat( strcat(buf_path, "\\"), pDirPath ) );
}
