

      SUBROUTINE REO
C----------------------------------------------------------------------
C                                       26.10.90
C
C      OПTИMИЗAЦИЯ ЭЛEMEHTOB ПЛAHA PEKOHCTPУИPУEMOГO ИЛИ BTOPOГO ПУTИ
C
C      ГOЛOBHAЯ ПPOГPAMMA БЛОКА ОПТИМИЗАЦИИ
C
C______________________________________________________________________
       use abdx

       INCLUDE 'INTERFCS.INC'

       INCLUDE 'COMMONS.INC'
      INTEGER N,MN,INDACT(350),ISTOP,TYPV,NUMV,I
      REAL *8 X(350),XMIN(350),XMAX(350),
     #        SIGMA(2002),C(2002,3),TH(2002),A(350,354)
      LOGICAL *1 IACT(2002,2)
      !COMMON /MEM1/ A,C,TH ! нет нигде  MEM1
      common/dob/ t1,r,j 
	common/dob3/sl,sp
	integer t1(350),r,j! 350 много это номера не фикс эл-тов до моста
      real *8 sl,sp
      common /dob1/ beta
	real*8 beta(1000),xp,yp

	common/dob2/pr  !приходит из start 0 при npoint.lt.0 отрицат полоса
      integer pr
	common/dobpech/ipc
	integer ipc
c-это углы поперечников в точках съёмки с осью Х, но первый-нач. угол к
      EXTERNAL FUNC,GRAD
C     j-кол-во переменных в доп ограничении
      REAL WGTV
      INTEGER ITMAX0,ERROR1
C------------------->
C      MN  - ЧИCЛO OГPAHИЧEHИЙ-HEPABEHCTB
C      INDACT(N)  -    PAБOЧAЯ ПAMЯTЬ ДЛЯ  ALMO
C      ISTOP - ПPИЗHAK BЫXOДA ИЗ  ALMO:
C            0 - ПO  EPSF
C            1 - ПO  EPSX
C            2 - ПO  ITMAX
C      X(NVAR) - ПEPEMEHHЫE
C      XMIN(NVAR),XMAX(NVAR) - ГPAHИЦЫ ИЗMEHEHИЯ ПEPEMEHHЫX
C                              (ПPOCTЫE OГPAHИЧEHИЯ)
C      SIGMA(MN) - HAЧAЛЬHЫE ЗHAЧEHИЯ ШTPAФHЫX K-TOB MФЛ
C и кривизна тоже SIG(250) в COMMON LINE!!!
C      EPSC(MN) [R*8] - TPEБУEMAЯ TOЧHOCTЬ ПO OГPAHИЧEHИЯM
C      FUNC(X,F,C) - ПOДПPOГPAMMA BЫЧИCЛEHИЯ ФУHKЦИЙ  F(X)  И  C(X).
C      GRAD(IFUNC,X,G) - ПOДПPOГPAMMA BЫЧИCЛEHИЯ ГPAДИEHTOB  F(X)  И
C                 C(X).  ПPИ  IFUNC=0  BЫЧИCЛЯET ГPAДИEHT  F,  ПPИ
C                 IFUNC^=0  - ГPAДИEHT  IFUNC-ГO OГPAHИЧEHИЯ.
C      C(MN,3) -    PAБOЧAЯ ПAMЯTЬ ДЛЯ  ALMO
C      TH(MN)  -    PAБOЧAЯ ПAMЯTЬ ДЛЯ  ALMO
C      IACT(MN,2) -    PAБOЧAЯ ПAMЯTЬ ДЛЯ  ALMO
C      A(N,N+4)   -    PAБOЧAЯ ПAMЯTЬ ДЛЯ  ALMO
C      ERROR      -    КОД ЗАВЕРШЕНИЯ  START
C______________________________________________________________________
C
C------- BBOД
Cпри работе без моста убрать read.tmp
       ipc=0
      OPEN(UNIT=1,FILE='OPT.txt',STATUS='UNKNOWN')
      OPEN(UNIT=2,FILE='fortxy1.txt',STATUS='UNKNOWN')
	open(unit=3,FILE='bufxy1.txt',STATUS='UNKNOWN')
       rewind 2
      CALL START(ERROR1)
	IF (ERROR1.NE.0) GO TO 710
	 !превышение числа переменных обработать красиво
!j,j1,s1,s2-Нет! пришли нулевые по common из start.for
! sh=ksifin- абсцисса конца минус фикс длины,sl,sp из start
       j=0 ! число  не фикс переменных в ограничении про мост
	
C------- CЧET
C
       do i=1,2002
        sigma(i)=1D0
         enddo
	N=NVAR !!!!
       MN=2*NPOINT  
c      write(1,111) xlmax,xlmin,xl
111   format('111  xlmax=',f10.2,'  xlmin=',f10.2,' XL=',f10.5)
      DO 51 I=1,NVAR
         TYPV = TYPVAR(I)
         NUMV = NUMVAR(I)
         WGTV = WGTVAR(TYPV)
         GO TO (41,42,43,44),TYPV
   41       CONTINUE
            X(I) = XL/WGTV
            XMIN(I) = XLMIN/WGTV
            XMAX(I) = XLMAX/WGTV
         GO TO 50
   42       CONTINUE
            X(I) = SIG(NUMV)/WGTV !SIG-кривизна из COMMOM
            XMIN(I) = SIGMIN(NUMV)/WGTV
            XMAX(I) = SIGMAX(NUMV)/WGTV
         GO TO 50
   43       CONTINUE
            X(I) = LA(NUMV)/WGTV
            XMIN(I) = LAMIN(NUMV)/WGTV
            XMAX(I) = LAMAX(NUMV)/WGTV
         GO TO 50
   44       CONTINUE
            X(I) = L(NUMV)/WGTV
            XMIN(I) = LMIN(NUMV)/WGTV
            XMAX(I) = LMAX(NUMV)/WGTV
   50 CONTINUE
            if(typv.eq.2) go to 51 ! кривизна не участвует в длинах
            if(r.eq.0) go to 51
            if(numv.gt.r) go to 51
            if((numv.eq.r).and.(typv.eq.3)) go to 51
! ограничение на конец клотоиды элемента номер r
             j=j+1
             t1(j)=i ! номера переменных ( кроме кривизны) в доп. ограничении
   51 continue 
      write(1,222) r,j,sl,sp
222   format(/5x,'  r=',i3,'j=',i3,' sl=',f9.2,' sp=',f9.2/)
      !write(1,223) (t1(i),i=1,j)
223   format(5x,' t1',/2x,10i4)
      
C
      ITMAX0 = DFPMAX
        	      write(1,80)NVAR,MN,NCURV,NPOINT
   80 format(/5x,'REO NVAR=',i3,' MN=',i4,' NC=',i3,' NP=',i4)
!      do i=1,Nvar
 !     write(1,81) xmin(i),x(i),xmax(i)
   81 format(/2x,'xmin=', f10.2,' x=',f10.2,' xmax=',f10.2)
  !    enddo
   !   write(1,82) DFPMAX,ALMMAX
   82 format(2x,'DFPMAX=',i5,' ALMMAX=',i5)
      CALL ALMO(N,MN,X,XMIN,XMAX,SIGMA,EPSC,EPSF,EPSX,DFPMAX,
     #          ALMMAX,FUNC,GRAD,C,TH,IACT,A,INDACT,ISTOP,pr)
C       запись результата в bufxy1.txt 3
        write(3,85) xl
   85 format(f10.4)

	  do 87 i=1,NCURV
	write(3,86)L(i),LA(i),SIG(i)
   87  continue
   86  format(2f20.12,f20.12)
      write(3,88)L(NCURV+1)
   88  format(f15.6)
          close(2)
      write(3,90) (SHIFT(i),i=1,NPOINT)
   90  format (5f12.3)
      write(3,91) (beta(i),i=1,NPOINT)
   91  format (5f12.8)
      close(3)
  700 CONTINUE

      write(1,*)"REO shift ИТОГ"
      do 321 j=1,npoint
      write(1,322) j,shmin(j),shift(j),shmax(j)
  321 continue
  322 format(2x,'j=',i4,' min=',f9.4,' shift(j)=',f14.8,' max=',f9.4)
      ! write(1,*)'       Координаты'
      !do 323 j=1,npoint
      !xp=xev(j)+shift(j)*cos(beta(j))
      !yp=yev(j)+shift(j)*sin(beta(j))
      !write(1,324) j,xev(j),yev(j),xp,yp
  323 continue
  324 format(2x,'j=',i4,' xn=',f9.3,' yn=',f9.3,' xp=',f9.3,' yp=',f9.3)
      write(1,10777)phifin,phi(2,ncurv+1)
10777 format(2x,' phifin=',f9.5,' phi(2,ncurv+1)=',f9.5)
c           
	close (1)
		!close (3)! закрыт выше
	if(ipc.ne.0) close(7)
  710 CONTINUE
      RETURN
      END
