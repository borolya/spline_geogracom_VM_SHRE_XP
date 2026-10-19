//---------------------------------------------------------------------------

#ifndef Unit21H
#define Unit21H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <dir.h>
#include <windows.h>
//---------------------------------------------------------------------------
char *current_directory(char *path) //функция возвращающая текущую папку.
{			  //необходима для записи/удаления перегонов
  strcpy(path, "X:\\");  /* fill string with form of response: X:\ */
  path[0] = 'A' + getdisk(); /* replace X with current drive letter */
 // getdisk()- даёт номер диска для С это 2 символ с кодом 2 это не С;
 //а код символа 'A' =65 складываем коды получаем 67 то есть С
  getcurdir(0, path+3);  /* fill rest of string with current directory */
 // getcurdir  даёт текущую но без C:\\ поэтому нужен X:\\
  return(path);
}
//---------------------------------------------------------------------------
extern char* curdir;
extern int tabflag;
//---------------------------------------------------------------------------
#endif
