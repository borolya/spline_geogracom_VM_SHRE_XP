

      SUBROUTINE FUNC(X,F,C)
      INCLUDE 'INTERFCS.INC'
C----------------------------------------------------------------------
C
C      Д.M. ШEЙДBACCEP                                  27.02.89
C
C      OПTИMИЗAЦИЯ ЭЛEMEHTOB ПЛAHA PEKOHCTPУИPУEMOГO ИЛИ BTOPOГO ПУTИ
C      BЫЧИCЛEHИE ЗHAЧEHИЙ KPИTEPИЯ И OГPAHИЧEHИЙ.
C
C      BXOД: /LINE/,/SHIFTS/,/POINTS/,/CONPTR/,BOUNDS/,/FINAL/,/CRIT/
C
C      BЫXOД: F,C
C
C______________________________________________________________________
C
      REAL *8 X(1),F,C(1)
C------------------->
C      X(NVAR) - ЗHAЧEHИЯ ПEPEMEHHЫX
C      F - ЗHAЧEHИE KPИTEPИЯ OПTИMИЗAЦИИ
C      C(NCON) - ЗHAЧEHИЯ OГPAHИЧEHИЙ без TH!!!
C______typecr=1 оптимизация по сумме рихт.,=2 по сумме квадратов_______
C
      INCLUDE 'COMMONS.INC'
c pr=1,если второй путь  относительно первого идет влево иначе минус 1
      REAL *8 ss1,s,s1
  904  FORMAT(/'FUNC : OГPAHИЧEHИЯ-HEPABEHCTBA'/(5X,10G12.4))
      !if(abs(shmin(2)).gt.shmax(2)) pr=-1
c второй abs не нужен -1 oзначает что прижиматься к max иначе min
      s=0.0010
        F=0.0
c      GO TO (10,20),TYPECR
       typecr=2
       GO TO 20
c   10    CONTINUE
    
         DO 12 I=1,Npoint
         s1=SHIFT(I)
         if(dabs(s1).gt.s) go to 118
         F=F+(s1*s1/s+s)/2.0
         go to 12
  118  continue
         F=F+dabs(s1)
   12    CONTINUE
         F = F*MULTCR ! multcr==1
      GO TO 90
   20    CONTINUE
         DO 22 I=1,Npoint
         F =F+ SHIFT(I)*SHIFT(I)
   22    CONTINUE
         F=F*10.0 ! Зачем умножил? Не помню!!!
   90 CONTINUE
      DO 200 I=1,Npoint
         C(2*i-1) = (SHMIN(i)-SHIFT(i))*WGTCON
         C(2*I) = (SHIFT(i)-SHMAX(i))*WGTCON
  200 CONTINUE
       RETURN
      END
