!  rewfort.f90 
!
!  FUNCTIONS:
!   WinMain()      - Entry point for the application;
!                    displays the main window; processes the message loop
!   rewfortSub()  - Callback routine for the main dialog box
!   rewfortCancel()- Callback routine for the Cancel button
!

!****************************************************************************
!
!  FUNCTION: WinMain( hInstance, hPrevInstance, lpszCmdLine, nCmdShow )
!
!  PURPOSE:  Entry point for the application
!
!  COMMENTS: Displays the main window and processes the message loop
!
!****************************************************************************

integer*4 function WinMain( hInstance, hPrevInstance, lpszCmdLine, nCmdShow )
!DEC$ IF DEFINED(_X86_)
!DEC$ ATTRIBUTES STDCALL, ALIAS : '_WinMain@16' :: WinMain
!DEC$ ELSE
!DEC$ ATTRIBUTES STDCALL, ALIAS : 'WinMain' :: WinMain
!DEC$ ENDIF

    use user32
    use kernel32
    use dflogm
    use abdx !rewfortGlobals

    implicit none

    integer*4 hInstance
    integer*4 hPrevInstance
    integer*4 lpszCmdLine
    integer*4 nCmdShow

    !include 'resource.fd'! есть в abdx

    external rewfortSub
    external rewfortCancel
        
 !type (dialog) dxy ! есть в abdx
    ! Variables
    type (T_MSG)            mesg
   ! integer*4               ret
   ! logical*4               lret
 common/bbb/ioss  ! для связи с ALMO  и др без завершения REO нельзя!!
   integer ioss
    integer*4 i

    ghInstance = hInstance
    ghModule = GetModuleHandle(NULL)
    ghwndMain = NULL

  lret = DlgInit(idd_txy, dxy) ! инициализация диалога
   
!	if(.not.lret) stop 'Dialog idd_txy not found'
    
    
if (lret == .FALSE.) goto 99999

    flag=dlgset(dxy,vne,'')
    flag=dlgset(dxy,ost,'')
	flag=dlgset(dxy,crit,'')
	flag=dlgset(dxy,nev,'')
	flag=dlgset(dxy,err,'') ! очистка поля результат
	 
	  i=0
	 write(string,'(i4)')i
	 flag=dlgset(dxy,vne,adjustl(string))
	
     flag=dlgsetsub(dxy,cancel,rewfortCancel)!Прерывание

 lret = DlgSetSub(dxy, IDD_txy, rewfortSub)! вызывается при открытии и разрушении диалога

 lret = DlgModeless(dxy, nCmdShow) ! открытие диалога в модальном режиме
  if (lret == .FALSE.) goto 99999
  call reo
   if(ioss==-1) then
  ! call dlgexit(dxy)
	return
	endif

  lret=dlgset(dxy,cancel,'Выход',dlg_title) ! меняю название кнопки
   ioss=11 ! иначе при нормальном завершении нужно два нажатия Выход
!  lret=dlgset(dxy,ok,.true.,dlg_enable) ! было 2 кнопки и делал недоступными одну
 ! lret=dlgset(dxy,cancel,.false.,dlg_enable)

 
    ! Read and process messsages
   do while( GetMessage (mesg, NULL, 0, 0) ) 
   if ( DlgIsDlgMessage(mesg) .EQV. .FALSE. ) then
      lret = TranslateMessage( mesg )
    ret  = DispatchMessage( mesg )
 end if
  end do
  call DlgUninit(dxy) ! разрушение диалога

 WinMain = mesg.wParam

    return

99999 &

    ret = MessageBox(ghwndMain, "Ошибка инициализации диалога"C, &
                  "Error"C, MB_OK)
	WinMain = 0

end 

!****************************************************************************
!
!  FUNCTION: rewfortSub ( dlg, id, callbacktype )
!
!  PURPOSE:  Dialog box callback for initialization and destroy
!
!  COMMENTS:
!
!****************************************************************************

SUBROUTINE rewfortSub( dlg, id, callbacktype )
!DEC$ ATTRIBUTES DEFAULT :: rewfortSub

  use user32
  use dflogm
  use abdx
  implicit none

  type (dialog) dlg
  integer id, callbacktype
  if (callbacktype == dlg_destroy) then
    call PostQuitMessage(0)
  endif

  END SUBROUTINE rewfortSub

!****************************************************************************
!
!  FUNCTION: rewfortApply ( dlg, id, callbacktype )
!
!  PURPOSE:  Dialog box callback for Cancel button
!
!  COMMENTS:
!
!****************************************************************************

 SUBROUTINE rewfortCancel( dlg,c_name,cbtype)
!DEC$ ATTRIBUTES DEFAULT :: rewfortApply
  use user32
  use dflogm
   use abdx
  implicit none

  type (dialog) dlg
  integer c_name,cbtype,i
  common/bbb/ioss  ! для связи с ALMO  и др без завершения REO нельзя!!
  integer ioss
  
 if (cbtype == dlg_clicked) then
 ! if(c_name==cancel) then
   if(ioss==11) then
    call dlgexit(dlg)
	return
	endif

       !ret = MessageBox(ghwndMain, " Расчёт прерван"C, "Внимание!"C, MB_OK)
   ioss=11 !передаю в ALMO для завершения REO
!lret=dlgset(dlg,cancel,'Выход',dlg_title) !было dxy вместо dlg
flag=dlgset(dlg,err,'Расчёт прерван') !не появляется так как dlgexit иначе нужен выход по кнопке
	 !call DlgUninit(dlg) даёт недопустимую операцию
!	call dlgexit(dlg)


	       return
 endif

	
  END SUBROUTINE rewfortCancel

! поместил в ALMO и др. для выдачи сообщений в процессе счёта
	! do while( PeekMessage (mesg, NULL, 0, 0,1) ) 
       !if ( DlgIsDlgMessage(mesg) .EQV. .TRUE. ) then
        !   lret = TranslateMessage( mesg )
         !  ret  = DispatchMessage( mesg )
       !end if
       ! end do

