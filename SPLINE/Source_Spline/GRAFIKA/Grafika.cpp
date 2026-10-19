//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include<string.h>
USEFORM("Praktika.cpp", Form1);
USEFORM("AddDialog.cpp", Form2);
USEFORM("Reference.cpp", Form3);
USEFORM("MainMenu.cpp", Form4);
USEFORM("Unit5.cpp", Form5);
//---------------------------------------------------------------------------
char uch_name[80]; //для записи сохранённого проекта
char elt_name[80];//готовое имя файла для записи  элементов *.e??
char igl_name[80];//имя файла исходной углограммы вмeсто Ugol.ugl
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR pDirPath, int)
{

//pDirPath="KD3.e92";
ShowMessage(pDirPath);
//сохранённый для воспроизведения тот же но вместо е стоит g
strcpy(elt_name,pDirPath); //вместе с номером варианта в него запись

unsigned int j=strlen(pDirPath);//кол-во символов в *.e01
	strncpy(uch_name,pDirPath,j-4);// убрал  копировании при j-4 символов.e01
    uch_name[j-4]='\0';  //это лишнее и так ставит нулевой байт при копировании
	strcpy(igl_name,uch_name);
    strcat(igl_name,".igl"); //исходная углограмма съёмки на участке
   strcpy(uch_name,elt_name);
     uch_name[j-3]='g';
    ShowMessage(uch_name);
        try
        {
                 Application->Initialize();
                 Application->CreateForm(__classid(TForm1), &Form1);
                 Application->CreateForm(__classid(TForm4), &Form4);
                 Application->CreateForm(__classid(TForm2), &Form2);
                 Application->CreateForm(__classid(TForm3), &Form3);
                 Application->CreateForm(__classid(TForm5), &Form5);
                 Application->Run();
         }
        catch (Exception &exception)
        {
                 Application->ShowException(&exception);
        }
        return 0;
}
//---------------------------------------------------------------------------
