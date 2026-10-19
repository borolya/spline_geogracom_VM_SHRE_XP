
      SUBROUTINE START(ERROR1)
      INCLUDE 'INTERFCS.INC'
C----------------------------------------------------------------------
C
C      
C      OПTИMИЗAЦИЯ ЭЛEMEHTOB ПЛAHA PEKOHCTPУИPУEMOГO ИЛИ BTOPOГO ПУTИ
C      BEPCИЯ 3.0
C
C      BBOД ИCXOДHЫX ДAHHЫX.
C
C      BЫXOД: /POINTS/,XL,L,SIG,LA,NCURV,/VARPTR/,/CONPTR/,
C             SHMIN,SHMAX,/FINAL/,/BOUNDS/,/CRIT/,/PAROPT/
C             ERROR - КОД ЗАВЕРШЕНИЯ:
C               0 - OK,
C              -1 - ПРЕВЫШЕНО МАКС. ЧИСЛО ПЕРЕМЕННЫХ
C
C______________________________________________________________________
C
      common/dob22/dx,dy,dugl
	real *8 dx,dy,dugl
	common/dob/ t1,r,j
	common/dob3/sl,sp
      common/dob1/beta
      real*8 beta(1000)
	common/dob2/pr
      integer pr
      integer t1(350),r,j
	 ! это номера не фикс.аеременных до клотоиды r, j- их число 
	 !sl,sp- малый интервал абсцисс конца этой клотоиды с номером r
      real *8 sl,sp
      INTEGER ERROR1
C
      INCLUDE 'COMMONS.INC'

	 INTEGER MAXVAR/350/ 
	 INTEGER I,IVAR,II1

C------- BBOД ДАННЫХ ИЗ ОБОЛОЧКИ
C
c      inquire(FILE='C:\DOS\mwavdlh.dll',exist=ex)
c      if(.not.ex) stop 'ошибка чтени'
c      r=4 getdat==reo_strt  в reo_c.c
c===========
       open(unit=8,FILE='nach_ugl.txt',STATUS='UNKNOWN')
       read(8,997)dx
	read(8,997)dy
	read(8,998)dugl
  997 format(f15.6)
  998 format(f12.8)
      close(8)
	write(1,996)dx,dy,dugl
  996 format(/2x,'dx=',f15.6,' dy=',f15.6,' dugl=',f12.8) 
       read (2,71) r,r,sl,sp
! это про мост признак, номер клотоиды с фиксированным правым концом 
!и интервал для этого конца
   71  format(2i5,2f10.2)
       write(1,69)r,sl,sp
   69  format(/5x,'r =',i5,' sl=',f10.2,' sp=',f10.2)
       read (2,73) NPOINT,NCURV
	 write (1,73) NPOINT,NCURV 
   73 format(2i5)
c     обработать минус в  npoint
       pr=1
       if(npoint.lt.0) then
       npoint=-npoint
       pr=0
       endif
      do 75 i=1,NPOINT 
      read(2,74) shmin(i),shmax(i),xev(i),yev(i),beta(i)      
c	write(1,74) shmin(i),shmax(i),xev(i),yev(i),beta(i)      
   75 continue
   74 format(2f10.3,2f16.8,f12.8)
c      format(2f10.3,2f12.3,f12.8)
      
      read(2,76)phifin,ksifin,etafin
   76 format(f20.8,2f20.5)
      read(2,77)XLMIN,XL,XLMAX
   77 format(3f12.6)
      do 78 i=1,NCURV
      read(2,77) LMIN(i),L(i),LMAX(i)
      read(2,77) LAMIN(i),LA(i),LAMAX(i)
      read(2,79) SIGMIN(i), SIG(i), SIGMAX(i)
   78 continue
   79 format(f20.12,f20.12,f20.12) 
      i=NCURV+1
      read(2,77) LMIN(i),L(i),LMAX(i)
	 close(2)
c      обнуление кривизны для последней клотоиды на метке 700
      j=0
 
c========выяснить верно ли сформированы конечные условия
      EPSC = 1D-2
      EPSF = 1D-1
      EPSX = 1D-6
cбыло -6 в aldfp они переустановлены
        wgtvar(1)=1.0 ! WGTX = 1E+0
      !WGTSIG = 1E-4 было раньше
      wgtvar(2)=1E-6      !WGTSIG=1E-6
      wgtvar(3)= 1.0 ! WGTLA = 1E+0
      wgtvar(4)= 1.0 !WGTL = 1E+0
      
      DFPMAX=5000
cбыло 1000 потом 2000
      ALMMAX=12
c было плохо с малыми шагами и большими штрафами typecr=1 по сумме рихт
      TYPECR=1 ! в FUNC он всегда 2  т.е. по сумме квадратов
      MULTCR=1
        ! осталлись только огран по рихтовкам 
         
   15 CONTINUE
!В одной строке есть или нет одна или несколько фикс переменных
!(не только длин)
!поэтому в списке переменных каждой из них дан тип и номер элемента 
!то есть строки таблицы, к которому она относится(NUMVAR)
C------ ИHИЦИAЦИЯ ПEPEMEHHЫX
      IVAR = 1
      TYPVAR(1) = 1 ! длина начальной прямой
	!NUMVAR(1) оставался нулевым ( не определён) что плохо
	NUMVAR(1)=1 ! он не работает но нуль там нельзя

      DO 150 I=1,NCURV ! по строкам таблицы
         IF (LMIN(I).GE.LMAX(I)-0.01) GO TO 98
            IVAR = IVAR+1
	    IF (IVAR.GT.MAXVAR) GO TO 710 ! не включать она фиксирована
	    TYPVAR(IVAR) = 4
            NUMVAR(IVAR) = I
            go to 198
   98    CONTINUE
cкорректировка расстояний так как есть фиксированная длина        
         l(i)=lmin(i)
         if(i.le.r) sl=sl-lmin(i)
         if(i.le.r) sp=sp-lmin(i)
  198    continue
	 IF (SIGMIN(I).GE.SIGMAX(I)-1.e-7) GO TO 110
	    IVAR = IVAR+1
	    IF (IVAR.GT.MAXVAR) GO TO 710
	    TYPVAR(IVAR) = 2
	    NUMVAR(IVAR) = I
	   go to 199
  110    CONTINUE
         sig(i)=sigmin(i)
  199  continue  
         IF (LAMIN(I).GE.LAMAX(I)-0.01)   GO TO 118
            IVAR = IVAR+1
	    IF (IVAR.GT.MAXVAR) GO TO 710
            TYPVAR(IVAR) = 3
            NUMVAR(IVAR) = I
             go to 1118
  118    CONTINUE
         la(i)=lamin(i)
	   if(i.ge.r) go to 1118   
       ! при i==r длину клотоиды вычитаем а кривой (прямой) нет так как
	 !! ограничение на конец клотоиды элемента номер r
      sl=sl-lamin(i)! минус фиксированные длины из ограничения по мосту
      sp=sp-lamin(i)
       !NVAR-номер переменной в общем массиве 
	 !всех не фиксированных переменных
 1118    continue     
  150 CONTINUE
      IF (LMIN(NCURV+1).GE.LMAX(NCURV+1)-0.01) GO TO 155
         IVAR = IVAR+1
	 IF (IVAR.GT.MAXVAR) GO TO 710
         TYPVAR(IVAR) = 4
        NUMVAR(IVAR) = NCURV+1
	go to 156
  155 CONTINUE
      L(NCURV+1)=LMIN(NCURV+1)! если фикс последняя клотоида
  156 continue
	NVAR = IVAR
C NVAR- число переменных,NCURV- число прямых и кривых== строк в таблице элементов без концевой кл.
C TYPVAR-типы переменных: 4-длина клотоиды, 3-длина кривой(прямой).2- кривизна
C NUMVAR- номер элемента ( строки таблицы) для переменной
     
C------ ИHИЦИAЦИЯ OГPAHИЧEHИЙ
      NCON=2*NPOINT
       WGTCon = 1E+1  
      SIG(NCURV+1) = 0
  700 CONTINUE
      ERROR1 = 0
       !DO 249 J=1,NVAR ! только для печати
        !       J1 = TYPVAR(J)
         !      JVAR = NUMVAR(J)
C      write(1,7675) j,j1,jvar,WGTVAR(J1)
 7675 format(2x,'j=',i4,' tip=',i4,' jvar=',i4,' WGTVAR(J1)=',f12.9)
  249 continue
       write(1,*)'переменных не больше maxvar'
      RETURN
  710 CONTINUE
      ERROR1 = -1
	write(1,*)'переменных больше maxvar=350'
	close(1)
	stop
c      RETURN
      END
