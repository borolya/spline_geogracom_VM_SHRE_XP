      SUBROUTINE ALMO(N,MN,X,XMIN,XMAX,SIGMA,EPSC,EPSF,EPSX,DFPMAX,
     #                ALMMAX,FUNC,GRAD,C,TH,IACT,A,INDACT,ISTOP,pr)
      !pr=0 при обсчёте сразу без оптимизации 
       use abdx
	 use user32
       use kernel32
         
	 INCLUDE 'INTERFCS.INC'
	     type (T_MSG)            mesg

C----------------------------------------------------------------------
C
C                                    29.06.89
C
C      OПTИMИЗAЦИЯ ФУHKЦИИ  F(X)  C OГPAHИЧEHИЯMИ  C(X)<=0 
C      METOДOM MOДИФИЦИPOBAHHOЙ ФУHKЦИИ ЛAГPAHЖA (POKAФEЛЛAPA).
C
C        MФЛ = F(X) + 1/2*(SIGMA*(C(X)-TH)**2) ПPИ C>TH ДЛЯ HEPABEHCTB.
C                            Здесь TH>0   ??? ой ли
C      ПPOCTЫE OГPAHИЧEHИЯ BИДA  XMIN <= X <= XMAX  HE BKЛЮЧAЮTCЯ
C      B  MФЛ  И УЧИTЫBAЮTCЯ ПPИ EE OПTИMИЗAЦИИ.
C      AЛГOPИTM CM. B KHИГE  Ф.ГИЛЛ, У.MЮPPEЙ, M.PAЙT "ПPAKTИЧECKAЯ
C      OПTИMИЗAЦИЯ", M.:MИP,1985,C.311,318.+ ГИЛЛ Мюррей 1977 с 83-
C      BXOД:
C      ----
C        N [I*4] - PAЗMEPHOCTЬ  X
C        MN [I*4] - ЧИCЛO OГPAHИЧEHИЙ-HEPABEHCTB ==2npoint
C        X(N) [R*8] - HAЧAЛЬHOE ПPИБЛИЖEHИE  X
C        XMIN(N) [R*8] - HИЖHЯЯ ГPAHИЦA  X      : ПPOCTЫE
C        XMAX(N) [R*8] - BEPXHЯЯ ГPAHИЦA  X     : OГPAHИЧEHИЯ
C        SIGMA(MN) [R*8] - HAЧAЛЬHЫE ЗHAЧEHИЯ ШTPAФHЫX K-TOB
C                   (CM. AЛГOPИTM, K-TЫ  SIGMA)
C        EPSC [R*8] - TPEБУEMAЯ TOЧHOCTЬ ПO OГPAHИЧEHИЯM
C        EPSF [R*8] - TOЧHOCTЬ БEЗУCЛOBHOЙ OПTИMИЗAЦИИ ПO KBAДPATУ
C                   HOPMЫ ГPAДИEHTA
C        EPSX [R*8] - TOЧHOCTЬ БEЗУCЛ. OПTИMИЗAЦИИ ПO MAKC.MOДУЛЯ
C                   ИЗMEHEHИЯ  X  ЗA OДHУ ИTEPAЦИЮ
C        DFPMAX [I*4] - CУMMAPHЫЙ PECУPC BHУTPEHHИX ИTEPAЦИЙ
C        ALMMAX [I*4] - MAKCИMAЛЬHOE ЧИCЛO BHEШHИX ИTEPAЦИЙ
C        FUNC(X,F,C) - ПOДПPOГPAMMA BЫЧИCЛEHИЯ ФУHKЦИЙ  F(X)  И  C(X).
C        GRAD(IFUNC,X,G) - ПOДПPOГPAMMA BЫЧИCЛEHИЯ ГPAДИEHTOB  F(X)  И
C                   C(X).  ПPИ  IFUNC=0  BЫЧИCЛЯET ГPAДИEHT  F,  ПPИ
C                   IFUNC^=0  - ГPAДИEHT  IFUNC-ГO OГPAHИЧEHИЯ.
C        C(MN,3) [R*8] -    PAБOЧAЯ ПAMЯTЬ
c использовано что в памяти массив С по столбцам!!
C        TH(MN) [R*8] -     PAБOЧAЯ ПAMЯTЬ
C        IACT(MN,2) [L*1] -    PAБOЧAЯ ПAMЯTЬ
C        A(N,N+4) [R*8] -      PAБOЧAЯ ПAMЯTЬ

C        INDACT(N) [I*4] -      PAБOЧAЯ ПAMЯTЬ
C        X
C        ISTOP - ПPИЗHAK BЫXOДA:
C                  0 - BЫXOД ПO  EPSF
C                  1 - BЫXOД ПO  EPSX
C                  2 - BЫXOД ПO  DFPMAX
C                  3 - BЫXOД ПO  ALMMAX
C
C      KOHTPOЛЬ BXOДA:
C      --------------
C MN=2point- число ограничений  пришёл из REO как параметр 
C      ПPOMEЖУTOЧHAЯ ИHФOPMAЦИЯ:
C      ------------------------
C    C(MN,3) [R*8] - ЗHAЧEHИЯ  C  ДЛЯ ДBУX ПOCЛEДHИX ИTEPAЦИЙ:
C                C(I,INEW) - ДЛЯ TEKУЩEЙ ИTEPAЦИИ,
C                C(I,IOLD) - ДЛЯ ПPEДЫДУЩEЙ ИTEPAЦИИ;
C                C(I,3) - PAБOЧAЯ ПAMЯTЬ ДЛЯ  DFP
C   IACT(MN,2) [L*1] - ПPИЗHAK AKTИBHOCTИ OГPAHИЧEHИЙ-HEPABEHCTB
C             ДЛЯ ДBУX ПOCЛEДHИX ИTEPAЦИЙ (AHAЛOГИЧHO  C)
C  TH(MN) [R*8] - CMEЩEHИЯ ГPAHИЦ (K-TЫ  THETA, CM. AЛГOPИTM)
C     A(N,N+4) [R*8] - PAБOЧAЯ ПAMЯTЬ ДЛЯ OПTИMИЗAЦИИ  MФЛ;
C B HEЙ ПEPBЫE  N*N  ЭЛEMEHTOB - MATPИЦA ПEPEMEHHOЙ METPИKИ
C        INDACT(N) [I*4] - PAБOЧAЯ ПAMЯTЬ ДЛЯ OПTИMИЗAЦИИ  MФЛ
C        ITOUT [I*4] - HOMEP BHEШHEЙ ИTEPAЦИИ
C        MN [I*4] - ЧИCЛO OГPAHИЧEHИЙ ==NCON==2NPOINT 
C        F [R*8] - ЗHAЧEHИE ЦEЛEBOЙ ФУHKЦИИ
C        ICNEW [I*4] - CMEЩEHИE MACCИBA  C(I,INEW)  B MACCИBE  C
C        ICOLD [I*4] - CMEЩEHИE MACCИBA  C(I,IOLD)  B MACCИBE  C
C       ICDFP [I*4] - AДPEC 1-ГO ЭЛ-TA PAБOЧEГO MACCИBA ДЛЯ  DFP
C                     B MACCИBE  C   =2*M+1
C        IANEW [I*4] - CMEЩEHИE MACCИBA  IACT(I,INEW)  B  IACT
C        IAOLD [I*4] - CMEЩEHИE MACCИBA  IACT(I,IOLD)  B  IACT
C        I [I*4]     - ПAPAMETP ЦИKЛOB
C
C      ПOДПPOГPAMMЫ:
C      ------------
C
C        ALDFP(N,MN,X,XMIN,XMAX,SIGMA,TH,EPSF,EPSX,DFPMAX,ALMMAX,
C             FUNC,GRAD,A,C1,INDACT,ISTOP1) -
C               OПTИMИЗAЦИЯ MФЛ C ПPOCTЫMИ OГPAHИЧEHИЯMИ
C           METOДOM ПEPEMEHHOЙ METPИKИ ДЭBИДOHA-ФЛETЧEPA-ПAУЭЛЛA
C     BXOД: N,MN,X,XMIN,XMAX,SIGMA,TH,EPSF,EPSX,DFPMAX,ALMMAX
C     BЫXOД: X,DFPMAX(УMEHЬШAETCЯ HA ЧИCЛO BЫПOЛHEHHЫX ИTEPAЦИЙ
C             DFP),A,ISTOP1(УKAЗATEЛЬ ПPИЧИHЫ BЫXOДA);
C           ISTOP1=0 - BЫXOД ПO  EPSF
C                 =1 - BЫXOД ПO  EPSX
C                 =2 - BЫXOД ПO  DFPMAX
C        POINT(X) - ПOДГOTOBKA K BЫЧИCЛEHИЮ KPИTEPИЯ И OГPAHИЧEHИЙ
C                   B TOЧKE  X
C        FUNC(IFUNC,X,F) - OПИCAHA B PAЗДEЛE "BXOД"
C        GRAD(IFUNC,X,G) - OПИCAHA B PAЗДEЛE "BXOД"
C
C      ПPOMEЖУTOЧHAЯ ПEЧATЬ:
C      --------------------
C
C        HA KAЖДOЙ BHEШHEЙ ИTEPAЦИИ BЫBOДЯTCЯ B 1-Й ФAЙЛ ЗHAЧEHИЯ
C          X,F,C(I,INEW),SIGMA,TH,DFPMAX.
C
C----------------------------------------------------------------------
C не было  common
c      INCLUDE 'COMMONS.INC'
       common/bbb/ioss
       integer ioss
      COMMON /CONPTR/ WGTCON,NCON
	REAL*8 WGTCON
	INTEGER NCON
      common /crit/typecr
      integer typecr
	common/dobpech/ipc
	integer ipc ! для печати в POINT чтобы один раз печатать
      INTEGER N,MN,DFPMAX,ALMMAX,INDACT(N),ISTOP,IOLD,INEW,ITOUT,
     #        ICNEW,ICOLD,ICDFP,IANEW,IAOLD,ISTOP1,I,pr
C
      REAL *8 X(N),XMIN(N),XMAX(N),SIGMA(1),EPSC,EPSF,EPSX,
     #        C(1),TH(1),A(N,1),F,epsc1
C
      LOGICAL *1 IACT(1)
      integer povt,j
C
      EXTERNAL FUNC,GRAD !,point,aldfp
C
C
  901 FORMAT(//' ALMO:',15X,'BHEШHЯЯ ИTEPAЦИЯ ',I10)
  902 FORMAT(/' ALMO:     ПEPEMEHHЫE'/(1X,5F14.6))
  903 FORMAT(/' ALMO:     ЦEЛEBAЯ ФУHKЦИЯ',G17.8)
  904 FORMAT(/' ALMO:     OГPAHИЧEHИЯ-HEPABEHCTBA'/(5X,10G12.4))
  905 FORMAT(/' ALMO:     OГPAHИЧEHИЯ-PABEHCTBA'/(5X,10G12.4))
  906 FORMAT(/' ALMO:     SIGMA'/(5X,10G12.4))
  907 FORMAT(/' ALMO:     THETA'/(5X,10G12.4))
  908 FORMAT(/' ALMO:     OCTATOK PECУPCA BHУTPEHHИX ИTEPAЦИЙ ',I10)
  910 FORMAT(' ALMO:     N=',I10,'  HE > 0')
  911 FORMAT(' ALMO:     MN=',I10,'  HE > 0')
  912 FORMAT(' ALMO:     EPSC=',D10.3,'  HE > 0')
  913 FORMAT(' ALMO:     EPSF  И  EPSX =',2D11.3,'  HE > 0')
  914 FORMAT(' ALMO:     DFPMAX=',I10,'  HE > 0')
  915 FORMAT(' ALMO:     BЫXOД ПO ЧИCЛУ ИTEPAЦИЙ',i10)
C
      ipc=0! нужно счётчик числа печатий в ERROR.txt
       write(1,11112)n,mn
11112  format(/2x,'n=',i4,' mn=',i4)
      ICDFP=2*MN+1 !ДЛЯ с
c       write(1,*)' ALMO'
	 
C-------- KOHTPOЛЬ BXOДA
      povt=0
	j=-1 ! При самом первом заходе в POINT был    контроль начального 
! приближения по -1 и возможность получить сдвижки при аварии
   16 continue ! сюда иду для повтора
       !TYPECR=2 по сумме квадратов в FUNC только так
      DO 21 I=1,MN
      sigma(i)=1d0 
   21 TH(I)=0D0
Cв reo.for sigma был 1 передан сюда но для повтора  надо восстановить
C-------- HAЧAЛЬHOE ПPИБЛИЖEHИE
C
      epsx=0.01
c передаю в ALDFP  и при каждом вызове уменьшаю
      INEW=1
      IOLD=2

  !     WRITE (1,902) (X(I),I=1,N)
       ! При повторе j=3  
	       
!углы бета делаю по нормалям и считаю сдвижки 
! новый вызов в конце  а по итерациям по старому считаю сдвижки в point,
! но по этим новым углам бета          
       if(pr.ne.0) go to 17 ! 0 при полоса <0 обсчёт начала
	ioss=-1
       istop1=0
       go to 300! там посчитаем бета и рихтовки по исходному  при вводе
   17  continue
       i=0 !здесь как параметр для вычисления углов beta
      CALL POINT_NEW0(X,j,i)
!      WRITE (1,902) (X(I),I=1,N)
      CALL FUNC(X,F,C)
C было вычислены С как левые части неравенств C(X)<0! 
C проверено надо C+TH(I), но с нул TH(I) станут 
! отрицательныи после пересчета
C моё вычислены С как левые части неравенств C(X)<0 без TH
C проверено надо C-TH(I), но с нуля TH(I) станут
! отрицательныи после пересчета это  сдвиг оврага внутрь области
 !!! КАК ПРАВИЛЬНО, C(X)-TH<0 и TH<0 или TH>0????     
         DO 31 I=1,MN
            IACT(I)=(C(I).GT.TH(I))! сначала TH =0 потом<0
   31    CONTINUE
  
  922 FORMAT(/' ALMO:    IACT'/(1X,20i3))
c       WRITE (1,901) ITOUT
      !WRITE (1,902) (X(I),I=1,N)
c      WRITE (1,903) F
      
  !      WRITE (1,904) (C(I),I=1,MN)
!	WRITE (1,922) (IACT(I),I=1,MN)
    !   WRITE (1,906) (SIGMA(I),I=1,MN)
    !  WRITE (1,907) (TH(I),I=1,MM)
c      WRITE (1,908) DFPMAX
C
C-------- ИTEPAЦИOHHЫЙ ЦИKЛ
C
            
      IF (ALMMAX.LE.0) GO TO 710
      DO 200 ITOUT=1,ALMMAX
cc new
	   write(string,'(i4)')itout
	  flag=dlgset(dxy,vne,adjustl(string))

	 do while( PeekMessage (mesg, NULL, 0, 0,1) ) 
       if ( DlgIsDlgMessage(mesg) .EQV. .TRUE. ) then
           lret = TranslateMessage( mesg )
           ret  = DispatchMessage( mesg )
       end if
        end do
 
cc new 
c         WRITE (1,901) ITOUT
C
C----------- MOДИФИKAЦИЯ  INEW, IOLD
C до цикла С вычислил с первой , в ALDFP всё в третьем столбце,
! а после неё со второго столбца  INEW ==2 при новом входе в цикл смена
         I=IOLD !2
         IOLD=INEW  !1
         INEW=I    !2
         ICNEW=MN*(INEW-1)!MN
         ICOLD=MN*(IOLD-1)!0
         IANEW=MN*(INEW-1)!MN
         IAOLD=MN*(IOLD-1) !0
C
C----------- OПTИMИЗAЦИЯ MOДИФ. ФУHKЦИИ ЛAГPAHЖA
C            C ПPOCTЫMИ OГPAHИЧEHИЯMИ
C
       
       CALL ALDFP(N,MN,X,XMIN,XMAX,SIGMA,TH,EPSF,EPSX,
     #              DFPMAX,FUNC,GRAD,A,C(ICDFP),INDACT,ISTOP1)
      !ioss=11 не знаю зачем но это прерывание
	
	 write(1,2)ioss,povt,istop1,ITOUT,ALMMAX
    2 format(/2x,'after ALDFP  ioss=',i3,' povt=',i3,' istop1=',i3,
     #' itout=',i5,' almmax=',i5)
	 if(ioss.eq.11) go to 710

c         go to 300
         epsx=epsx/1.5
	i=0
 ! иначе будет зацикливание при плохом начале (параллельность)
         CALL POINT(X,i,i)
         CALL FUNC(X,F,C(ICNEW+1))!даёт все С без TH
        
c         WRITE (1,902) (X(I),I=1,N)
       WRITE (1,903) F  !ALMO: целевая функция
C
C----------- BЫЧИCЛEHИE ЗHAЧEHИЙ OГPAHИЧEHИЙ И BЫДEЛEHИE
C            AKTИBHЫX  (C > THETA) надо больше нуля?!
C 
            DO 111 I=1,MN
               IACT(IANEW+I)=(C(ICNEW+I).GT.TH(I))
c IACT(IANEW+I)=(C(ICNEW+I).GT.0.0) активность по TH, а контроль без них
  111       CONTINUE
        !  WRITE(1,*)"    C(ICNEW+I)"
	 !	 WRITE (1,904) (C(ICNEW+I),I=1,MN)
       !     write(1,1904) (th(i),i=1,mn)
 1904 FORMAT(/' ALMO: th   OГPAHИЧEHИЯ-HEPABEHCTBA'/(5X,10G12.4))
       !    WRITE(1,922)(IACT(IANEW+I),i=1,mn) 
C----------- ПPOBEPKA УCЛOBИЯ BЫXOДA ПO  DFPMAX
         IF(ISTOP1.EQ.2) flag=dlgset(dxy,err,'Мало внутренних итераций')
C
C         IF(ISTOP1.EQ.2) GO TO 300
C
C----------- ПPOBEPKA УCЛOBИЙ BЫXOДA: ДЛЯ OГPAHИЧEHИЙ-PABEHCTB
C            И AKTИBHЫX HEPABEHCTB  DABS(C) <= EPSC
C
        epsc1=0.004*wgtcon ! Остались только неравенства на сдвижки
	  DO 121 I=1,MN
	 IF(.NOT.IACT(IANEW+I)) GO TO 121
       IF(DABS(C(ICNEW+I)).GT.EPSC1)then
	!write(1,923)i,ICNEW,C(ICNEW+I),TH(i)
  923 format(/2x,' ALMO i=',i5,' ICNEW=',i5,' C=',F12.7,' TH=',f8.6)  
	  go to 130 ! не выполнено 
	endif
  121    CONTINUE
C
C----------- УCЛOBИЯ BЫXOДA BЫПOЛHEHЫ
C
       write(1,1) povt
    1 format(/2x,'ALMO 121  УCЛOBИЯ BЫXOДA BЫПOЛHEHЫ',' povt=',i4)
         if(povt.eq.1) GO TO 300
	write(1,*)'На повторение'
          povt=1
       j=3 ! это признак vis в POINT при -1 там при прерывании STOP
          go to 16

C----------- УCЛOBИЯ BЫXOДA HE BЫПOЛHEHЫ.
C
  130    CONTINUE
       write(1,*) ' ALMO не выполнены ограничения'
C----------- MOДИФИKAЦИЯ  THETA:
C               ДЛЯ HEPABEHCTB: 
C            ECЛИ  C <= TH, TO  TH=0
C     ECЛИ  C  > TH, TO  TH=TH-C и TH(I) станет <0 что и требуется
C      
             DO 135 I=1,MN
         IF(C(ICNEW+I).GT.TH(I)) GO TO 132
              TH(I)=0D0
               GO TO 135
  132             TH(I)=TH(I)-C(ICNEW+I)
  135  CONTINUE !Ограничений равенств теперь нет
C
C----------- MOДИФИKAЦИЯ  SIGMA  для
C             AKTИBHЫX HA ДBУX ПOCЛEДHИX ИTEPAЦИЯX HEPABEHCTB:
C            ECЛИ MOДУЛЬ  C  УMEHЬШИЛCЯ MEHEE ЧEM B 4 PAЗA
C            И  DABS(C) > EPSC, TO  SIGMA=SIGMA*10, TH=TH/10
Cпочему меняем тету? так в теории а вот знак её не ясен
              
  !    Write(1,18)IANEW,IAOLD,ICNEW,ICOLD
   18 format(/2x,' IANEW=',i5,' IAOLD=',i5,' ICNEw=',i5,' ICOLD=',i5)	     
   19  format(/2x,'i=',i4,' IACT(IANEW+I)=',i4,' IACT(IAOLD+I)=',i3)
	     DO 145 I=1,MN
       IF(.NOT.(IACT(IANEW+I).AND.IACT(IAOLD+I))) GO TO 145
   !    write(1,19)I, IACT(IANEW+I),IACT(IAOLD+I)  
		 IF(DABS(C(ICNEW+I)).LE.EPSC1 .OR.
     #            DABS(C(ICNEW+I))*4.LE.DABS(C(ICOLD+I)))then
       ! write(1,20)i, C(ICNEW+I),C(ICOLD+I)
   20 format(/2x,'i=',i4,' C(ICNEW+I)=',f12.6,' C(ICOLD+I)=',f12.6)
	              GO TO 145
	   endif
      !write(1,15) i,sigma(i),th(i)
	           SIGMA(I)=SIGMA(I)*10d0
                TH(I) =TH(I)/10d0
      
   15 format(/2x,' ALMO 15 i=',i4,' sigma(i)=',f7.3,' th(i)=',f9.6)  
  145    CONTINUE
       !write(1,*)'ALMO 145 '

c         WRITE (1,906) (SIGMA(I),I=1,MN)
       !  WRITE (1,907) (TH(I),I=1,MN)
c         WRITE (1,908) DFPMAX
  200 CONTINUE  ! ЦИКЛ ПО ITOUT внешние итерации 
      GO TO 710
C
C-------- HOPMAЛЬHЫЙ BЫXOД
C
  300 CONTINUE
       write(1,*)' На выходе ALNO 300 '
	write(1,311)ioss
  311 format(2x,' ALMo ioss= ',i3)
       	  WRITE (1,902) (X(I),I=1,N)
		 !write(string,'(i4)') vnesh
	 flag=dlgset(dxy,err,'Решение получено')
      i=3
	it=0 ! числа нельзя в вызов
	CALL POINT_NEW0(X,i,it)
	!даст новые бета и рихтовки по ним ( нормалям) REO запишет
	!обсчёт уже выправлен под них
	write(1,*)'After POINT_New0'
	if(ioss.eq.-1) return
       
	! do while( PeekMessage (mesg, NULL, 0, 0,1) ) 
      ! if ( DlgIsDlgMessage(mesg) .EQV. .TRUE. ) then
       !    lret = TranslateMessage( mesg )
       !    ret  = DispatchMessage( mesg )
       !end if
       ! end do
    
      ISTOP=ISTOP1
      write(1,717) istop,typecr
   
  717 format(/2x,'istop=',i3,' typecr=',i3)
c      CLOSE(1)
c      i=0
c      CALL POINT(X,i,it)
      
       write(1,311)ioss
      RETURN  !to REO.for
C
C-------- BЫXOД ПO  DFPMAX
C
  710 CONTINUE
       if(ioss.eq.11 ) then
	ioss=-1 !Для немедленного выхода без нажатия Выход
	!flag=dlgset(dxy,err,'Расчёт прерван')
	return
       else	 
	ioss=11
	 flag=dlgset(dxy,err,'Ограничения несовместны')
	       endif
	  ISTOP=3
      WRITE (1,915) ALMMAX
c      CLOSE(1)
      RETURN
      END

  