

      SUBROUTINE GRAD(ICON,X,G)
      INCLUDE 'INTERFCS.INC'
C----------------------------------------------------------------------
C      OПTИMИЗAЦИЯ ЭЛEMEHTOB ПЛAHA PEKOHCTPУИPУEMOГO ИЛИ BTOPOГO ПУTИ
C      BEPCИЯ 3.0
C
C      BЫЧИCЛEHИE ГPAДИEHTA KPИTEPИЯ ИЛИ OГPAHИЧEHИЯ.
C
C      BXOД: /LINE/,/SHIFTS/,/POINTS/,/CONPTR/,BOUNDS/,/FINAL/,
C            /VARPTR/,/DERIV/,/DSHIFT/,/CRIT/
C
C      BЫXOД: G
C
C______________________________________________________________________
C
      REAL *8 X(*),G(*)
      INTEGER ICON
C------------------->
C      X(NVAR) - ЗHAЧEHИЯ ПEPEMEHHЫX
C      G(NVAR) - ГPAДИEHT KPИTEPИЯ ИЛИ OГPAHИЧEHИЯ
C      ICON - УKAЗATEЛЬ:
C           ПPИ  ICON=0  BЫЧИCЛЯETCЯ ГPAДИEHT KPИTEPИЯ,
C           ПPИ  ICON>0  - ГPAДИEHT OГPAHИЧEHИЯ HOMEP  ICON
C______________________________________________________________________
C
      INCLUDE 'COMMONS.INC'
      REAL *8 SS2,s,s1,s2,ss1
      INTEGER TYPEC,NUMC,TYPEV,NUMV,IVAR
	integer j,j1
C------------------->
C      ss2 была - ПOЛУCУMMA KPИBИЗH CMEЖHЫX KPИBЫX
C      TYPEC - TИП OЧEPEДHOГO OГPAHИЧEHИЯ
C      NUMC - HOMEP B OЧEPEДHOM OГPAHИЧEHИИ
C      TYPEV - TИП OЧEPEДHOЙ ПEPEMEHHOЙ
C      NUMV - HOMEP OЧEPEДHOЙ ПEPEMEHHOЙ B TИПE
C      IVAR - HOMEP ПEPEMEHHOЙ
C______________________________________________________________________
C
      IF (ICON.GT.0) GO TO 100
	! при 0 только от целевой функции иначе по одному ограничению с номером ICON
C
C------ ПPOИЗBOДHЫE ЦEЛEBOЙ ФУHKЦИИ
C
      s=0.10
      GO TO (10,20),TYPECR
   10    CONTINUE
         DO 15 IVAR=1,NVAR
	!j1=typvar(ivar)
	      G(IVAR) = 0D0
            DO 13 J=1,NPOINT
c         write(1,1000)j,ss1,shmax(j)
 1000  format(2x,'j=',i3,' min=',f8.3,' max=',f8.3)
         s1=SHIFT(j)        
         if(dabs(s1).gt.s) go to 118
         s2=s1/s
         go to 119
  118  continue
         if(s1.gt.s) s2=1.0
         if(s1.lt.-s) s2=-1.0
  119  continue
        G(IVAR) = G(IVAR)+s2*DSHX(J,IVAR)
	! умножение на WGTVAR(i)  в POINTG из-за ALDFP
   13       CONTINUE
       ! G(IVAR) = G(IVAR)*MULTCR*WGTVAR(J1)
	   ! MULTCR==1 WGTVAR(1)==1
   15    CONTINUE
      GO TO 700
   20    CONTINUE
         DO 25 IVAR=1,NVAR
	!j1=typvar(ivar)
            G(IVAR) = 0D0
            DO 23 J=1,NPOINT
       !     ss2=shmin(j)
               G(IVAR) = G(IVAR)+2.0*SHIFT(J)*DSHX(J,IVAR)
   23       CONTINUE
      !  G(IVAR) = G(IVAR)*WGTVAR(J1)
   25    CONTINUE
      GO TO 700
C
C------ ПPOИЗBOДHЫE OГPAHИЧEHИЙ
C
  100 CONTINUE
! ICON-номер ограничения от 1до 2npoint=NCON
! ICON нечётный -ограничение на  min shift j1=номер точки съёмки
! j=MOD(i+1,2) ! даёт ноль при нечётном i и 1 при чётном 


       j=MOD(ICON+1,2)! 0 при нечётном ICON
	if(j.eq.0) then
	 j1=(ICON+1)/2
	else
	j1=ICON/2! это номер точки ( рихтовки)
	endif
      
 1323 format(2x,'i=',i3,' typec=',i4,' numc=',i4)
      DO 200 IVAR=1,NVAR
         TYPEV = TYPVAR(IVAR)
         NUMV = NUMVAR(IVAR)
        if(j.eq.0) then
	 G(IVAR) = -DSHX(j1,IVAR)*WGTCON*WGTVAR(TYPEV)
        else 
	  G(IVAR) = DSHX(j1,IVAR)*WGTCON*WGTVAR(TYPEV) 
	  endif 
       
  200 CONTINUE
C
  700 CONTINUE
      RETURN
      END
