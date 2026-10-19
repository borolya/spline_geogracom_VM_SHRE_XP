
c      INCLUDE 'INTERFCS.INC'
c      INCLUDE 'INTERF1.INC'

      SUBROUTINE ALDFP(N,MN,X,XMIN,XMAX,SIGMA,TH,EPSF,EPSX,
     #                 ITMAX,FUNC,GRAD,A,C,INDACT,ISTOP)
       ! SIGMA -коэфф из ALMO а туда из Reo SIGMA SIG -кривизна
	
       use abdx
	 use user32
        use kernel32
       !use dflogm  есть в abdx
           type (T_MSG)            mesg  
	 REAL *8 DSHX(1000,350)
      INTEGER   JFIRST(2,150) ,NPOINT
      COMMON /DSHIFT/ DSHX,JFIRST ,NPOINT 
      COMMON /VARPTR/ TYPVAR,NUMVAR,WGTVAR,NVAR  
	INTEGER *2 TYPVAR(350),NUMVAR(350)
      REAL*8 WGTVAR(4)
      INTEGER NVAR   
      
       common/bbb/ioss
       integer ioss
       common/crit/ typecr
       integer typecr
       common/dob/ t1,r,j
	 common/dob3/sl,sp
	 integer t1(350),r,j,k,j2,j3,ind1,ind2,k1,k2,k3
     *,i11,i15,izm,i4,i5,ii,jj,pm,pk
       real*8 s1,s2,sl,sp,sig1,sig2,smax,sig1r,sig2r,zap,
     *     pis,psj,pss,ns,ro,eps1,beta,g2old,nevv,roold
	 real *8 s3,s4,EE
	 common/dobpech/ipc
	integer ipc
	  
c !nevv- невязки для выдачи на экран
cпроекция на сумму плюс отдельные равна по активным отдельным нуль, а по не
cактивным градиент минус среднее арифметическое неактивных, тк у нормали все
cкомпоненты равны, а у проекции сумма нуль поэтому у нормали среднее
c арифметическое
C      izm-число итераций без изменения границы
c      i11-число отброшенных огр.,i15-число попыток выхода 	real*8 pmax1
	!максимальная компонента направления для поиска шага 
	! текущие значения функции 
c ro-для формулы Гольдфарба eps1- для точности вычисл шага меняется 
       integer iz,im
cim=0еще раз по матрицам иначе по проекции

cпроекция на сумму плюс отдельные равна по активным отдельным нуль, а по не
cактивным градиент минус среднее арифметическое неактивных, тк у нормали все
cкомпоненты равны, а у проекции сумма нуль поэтому у нормали среднее
c арифметическое
C      izm-0 не надо модифицировать по общей формуле так как добавили
c или исключили огр или первая итерация по проекции при изменении границы
c надо ставить нуль мост не исключается он активен
c      i11-число отброшенных огр.,i15-число попыток выхода 
c iz=0 по матрицам,iz=1 по проекции после выхода по шагу
C-----------------------------------------------------------------------
C
C               НАЧАТО                           20.07.88
C      CTPУЧЕНКОВ В.И.                        1997 
C      MИHИMИЗAЦИЯ MOДИФИЦИPOBAHHOЙ ФУHKЦИИ ЛAГPAHЖA
C      C ПPOCTЫMИ OГPAHИЧEHИЯMИ  XMIN <= X <= XMAX
C      METOДOM ПEPEMEHHOЙ METPИKИ ДЭBИДOHA-ФЛETЧEPA-ПAУЭЛЛA
C      ("ЧИCЛEHHЫE METOДЫ УCЛOBHOЙ OПTИMИЗAЦИИ", PEД. Ф.ГИЛЛ,
C      У.MЮPPЭЙ, C.83;  Ф.ГИЛЛ, У.MЮPPEЙ, M.PAЙT "ПPAKTИЧECKAЯ
C      OПTИMИЗAЦИЯ", C.261).
C      ДBOЙHAЯ TOЧHOCTЬ.
C
C      MФЛ = F(X) + 1/2*(SIGMA*(C(X)-TH)**2)
C            KBAДPAT HEBЯЗKИ ДЛЯ OГPAHИЧEHИЙ-HEPABEHCTB
C            BXOДИT B CУMMУ TOЛЬKO ПPИ  C < TH, котора с нул станет  <0
C
C      BXOД:
C      ----
C
C        N [I*4] - PAЗMEPHOCTЬ
C        MN [I*4] - ЧИCЛO OГPAHИЧEHИЙ-HEPABEHCTB (M.Б. 0)
C        ME [I*4] - ЧИCЛO OГPAHИЧEHИЙ PABEHCTB (M.Б. 0)
C                   MN+ME  ДOЛЖHO БЫTЬ > 0
C        X(N) [R*8] - HAЧAЛЬHOE ПPИБЛИЖEHИE
C        XMIN(N) [R*8] - HИЖHЯЯ ГPAHИЦA  X
C        XMAX(N) [R*8] - BEPXHЯЯ ГPAHИЦA  X
C        SIGMA(MN+ME) [R*8] - KOЭФФИЦИEHTЫ MФЛ
C        TH(MN+ME) [R*8] - KOЭФФИЦИEHTЫ MФЛ
C        EPSF [R*8] - TOЧHOCTЬ ПO KBAДPATУ ГPAДИEHTA MФЛ
C        EPSX [R*8] - TOЧHOCTЬ ПO MAKCИMУMУ MOДУЛЯ ИЗMEHEHИЯ  X
C                     HA OДHOЙ ИTEPAЦИИ
C        ITMAX [I*4] - PECУPC ИTEPAЦИЙ
C        FUNC(X,F,C) - ПOДПPOГPAMMA BЫЧИCЛEHИЯ ЦEЛEBOЙ ФУHKЦИИ
C                 И OГPAHИЧEHИЙ.
C        GRAD(IFUNC,X,G) - ПOДПPOГPAMMA BЫЧИCЛEHИЯ ГPAДИEHTA ЦEЛEBOЙ
C                 ФУHKЦИИ И OГPAHИЧEHИЙ. ПPИ  IFUNC=0  BЫЧИCЛЯET
C                 ГPAДИEHT  F, ПPИ  IFUNC^=0 - ГPAДИEHT  IFUNC-ГO
C                 OГPAHИЧEHИЯ.
C        A(N,N+4) [R*8] - PAБOЧAЯ ПAMЯTЬ.
C        C(MN+ME) [R*8] - PAБOЧAЯ ПAMЯTЬ.
C        INDACT(N) [I*2] - PAБOЧAЯ ПAMЯTЬ.
C
C      BЫXOД:
C      -----
C
C        X
C        ITMAX - OCTATOK PECУPCA ИTEPAЦИЙ
C        ISTOP - ПPИЗHAK BЫXOДA:
C                 0 - ПO  EPSF
C                 1 - ПO  EPSX
C                 2 - ПO  ITMAX
C
C      BHУTPEHHЯЯ ИHФOPMAЦИЯ:
C      ---------------------
C
C        A(N,N) - MATPИЦA ПEPEMEHHOЙ METPИKИ
C        A(I,N+1) - ГPAДИEHT MФЛ (G)
C        A(I,N+2) - ПPEДЫДУЩEE ЗHAЧEHИE ГPAДИEHTA (G1);
C                   BEKTOP  Y=G-G1
C        A(I,N+3) - HAПPABЛEHИE CПУCKA  P=-H*G;
C        A(I,N+4) - PAБOЧИЙ MACCИB ДЛЯ BЫЧИCЛEHИЯ ГPAДИEHTA MФЛ;
C                   PAБOЧИЙ MACCИB ДЛЯ ПOДПPOГPAMMЫ  ALREGM;
C                   BEKTOP  HY=H*Y
C        C(MN) - ЗHAЧEHИЯ OГPAHИЧEHИЙ
C        INDACT(N) - УKAЗATEЛИ AKTИBHOCTИ ПPOCTЫX OГPAHИЧEHИЙ:
C           INDACT(I) >0 - HOMEP ИTEPAЦИИ, HA KOTOPOЙ CTAЛO AKTИBHЫM
C                          OГPAHИЧEHИE  X(I) <= XMAX(I),
C           INDACT(I) <0 - TO ЖE ДЛЯ    X(I) >= XMIN(I),
C           INDACT(I) =0 - ПPOCTЫE OГPAHИЧEHИЯ HA  X(I)  HEAKTИBHЫ
C        LA [R*8] - BEЛИЧИHA ШAГA
C        YHY [R*8] - Y*HY; 1/(Y*HY)
C        YP [R*8] - Y*P; AL/(Y*P)
C        ITACT [I*4] - OБЯЗATEЛЬHOE BPEMЯ ПPEБЫBAHИЯ ПPOCTOГO
C                      OГPAHИЧEHИЯ B AKTИBHOM HAБOPE (B ИTEPAЦИЯX)
C        M [I*4] - ЧИCЛO OГPAHИЧEHИЙ  =MN+ME
C        F [R*8] - ЗHAЧEHИE ЦEЛEBOЙ ФУHKЦИИ
C        IT [I*4] - HOMEP ИTEPAЦИИ
C        G2 [R*8] - KBAДPAT HOPMЫ ГPAДИEHTA
C        LAMAX [R*8] - MAKCИMAЛЬHAЯ BEЛИЧИHA ШAГA ПO HAПPABЛEHИЮ
C        EPSLA [R*8] - TOЧHOCTЬ ПOИCKA ШAГA ПO HAПPABЛEHИЮ
C        INFTY [R*8] - ФAKTИЧECKAЯ БECKOHEЧHOCTЬ
C
C      ПOДПPOГPAMMЫ:
C      ------------
C
C        FUNC(X,F,C) - OПИCAHA BЫШE.
C        GRAD(IFUNC,X,G) - OПИCAHA BЫШE.
C        POINT(X) - ПOДГOTOBKA BЫЧИCЛEHИЯ KPИTEPИЯ И OГPAHИЧEHИЙ
C                   B TOЧKE  X.
C        POINTG - ПOДГOTOBKA BЫЧИCЛEHИЯ ГPAДИEHTOB KPИTEPИЯ
C                    И OГPAHИЧEHИЙ B TOЧKE  X. HEOБXOДИMO
C                    ПPEДBAPИTEЛЬHOE BЫПOЛHEHИE  POINT(X).
C        MULTMV(A,X,Z,N,INDACT) - УMHOЖEHИE MATPИЦЫ  A  HA BEKTOP  X.
C                B BЫЧИCЛEHИЯX УЧACTBУЮT TOЛЬKO TE CTPOKИ И CTOЛБЦЫ
C                MATPИЦЫ  A  И ЭЛEMEHTЫ BEKTOPA  X, ДЛЯ KOTOPЫX
C                INDACT(I)=0.  OCTAЛЬHЫE ЭЛEMEHTЫ PEЗУЛЬTATA OБHУЛЯЮTCЯ.
C                PEЗУЛЬTAT - Z. PAЗMEPHOCTЬ - N. ДBOЙHAЯ TOЧHOCTЬ.
C        ALREGM(X,P,FUNC,N,EPS,LAMAX,MN,ME,SIGMA,TH,LA,Z,C) -
C                OДHOMEPHЫЙ ПOИCK MИHИMУMA ФУHKЦИИ  FUNC.
C                X(N) - HAЧAЛЬHAЯ TOЧKA, P(N) - HAПPABЛEHИE CПУCKA,
C                N - PAЗMEPHOCTЬ, EPS - TOЧHOCTЬ ПOИCKA ШAГA,
C                LAMAX - MAKCИMAЛЬHOE ЗHAЧEHИE ШAГA,
C                MN - ЧИCЛO OГPAHИЧEHИЙ-HEPABEHCTB,
C                SIGMA,TH - ПAPAMETPЫ MФЛ,
C                LA - OПTИMAЛЬHOE ЗHAЧEHИE ШAГA,
C                Z(N) - PAБOЧAЯ ПAMЯTЬ,
C                Z(MN+ME) - PAБOЧAЯ ПAMЯTЬ
C
C      ПPOMEЖУTOЧHAЯ ПEЧATЬ:
C      --------------------
C  было
C        B ФAЙЛ 1 BЫBOДЯTCЯ HA KAЖДOЙ ИTEPAЦИИ ЗHAЧEHИЯ
C           ITMAX,IT,LA,G2,F,X;
C        HA BЫXOДE B ФAЙЛ 1 BЫBOДЯTCЯ ЗHAЧEHИЯ
C           ITMAX,IT,DA,G2,ISTOP;
C        ПPИ BЫXOДE ПO  EPSX  BЫBOДИTCЯ TAKЖE HAПPABЛEHИE CПУCKA
C
C-----------------------------------------------------------------------
C
      INTEGER N,MN,ITMAX,INDACT(N),ISTOP,IT,ITACT,BREAK
      REAL *8 X(N),XMIN(N),XMAX(N),SIGMA(1),TH(1),EPSF,EPSX, 
     #A(N,1),C(1),
     #LA,YHY,YP,F,G2,LAMAX,LAMAX1,EPSLA,EPSLA1,INFTY/1D50/
      real *8 fold,lamax2
C
      EXTERNAL FUNC,GRAD
C
c      LOGICAL *1 FINAL
C_______________________________________________________________________
C
C      
            EE=0.005 ! будем искать с  шагом на 0.005м по рихтовкам 
		 icp=0 ! признак печати для POINT чтобы печатать один раз
		  ns=0.01
c используется как критерий при выбрасывании огр.,зависит от itact
       
      i15=0
      EPSF=0.00001  !EPSF=0.00005
c       EPSX=0.1E-4 работает 0.05 1.E-6 не работает 1.E-3 не работает
        EPS1=epsx
c epsx- получаю из ALMO и там меняю
        EPSX=0.1E-3
	eps1=0.001 !временно
C  изменил EPSF
C------ ИHИЦИAЛИЗAЦИЯ  A  И  INDACT
C
     	A(1,1) = 1D0
      DO 1 I=2,N
         A(I,I) = 1D0
         I1 = I-1
         DO 1 Jj=1,I1
            A(I,Jj) = 0D0
            A(Jj,I) = 0D0
    1 CONTINUE

c A можно и не делать единичной потом она выставлется в проекторе
      DO 2 I=1,N
    2 INDACT(I) = 0
      ind1=1
c      ind2=0
Cind2-признак активности ограничения на сумму длин (за мостом) не нужен
c ind1 признак активности моста теперь всегда активен и не выбрасывается
C--------- ПPOEKЦИЯ HAЧAЛЬHOГO ПPИБЛИЖEHИЯ  X  HA ДOПУCTИMУЮ OБЛACTЬ
C

      !write(1,*) 'НАЧАЛО'
       !do i=1,n
       !write(1,712) i,xmin(i),x(i),xmax(i)
       !enddo
      DO 5 I=1,N
         IF (X(I).LT.XMIN(I)) GO TO 3  !LT работает
         IF (X(I).GT.XMAX(I)) GO TO 4
         GO TO 5
    3       CONTINUE
            X(I) = XMIN(I)+EPSX*1.5
         GO TO 5
    4       CONTINUE
            X(I) = XMAX(I)  -EPSX*1.5
    5 CONTINUE
c   itmax 1000; itact 20;epsx 0.1e-5;epsf 0.1
      write(1,*) 'ALDFP НАЧАЛО'
      write(1,711)r, sl,sp,j,r
711   format(/5x,'r=',i3,' sl=',f7.2,' sp=',f7.2, ' j=',i4,' r=',i4)
712   format(3x,'712 i=',i3,' xmin=',f12.5,' x=',f12.5,' xmax=',f12.5) 
      
!       do i=1,n
 !      write(1,712) i,xmin(i),x(i),xmax(i)
  !     enddo
      
       if(r.eq.0) go to 800
        sig1=0.0
        smax=0.0
        sig1r=0.0
c j- кол-во длин до моста,s1-расст до моста t1-номера переменных длин
        DO 801   i=1,j
        sig1=sig1+XMIN(t1(i))
        sig1r=sig1r+X(t1(i))
      smax=smax+XMAX(t1(i))
 801  continue        
 800  continue
      
c      write(1,714) sig1,sig1r,smax
 714  format(/2x,'sig1=',f9.2,' sig1r=',f9.2,' smax=',f9.2) 

      if( (sig1.le.sp).and.(smax.ge.sl)) go to 805
      write(1,715) sig1,smax,sl,sp
      write(*,715) sig1,smax,sl,sp
      !pause 'Доп. ограничение невозможно выполнить.ENTER'
 715  format(/5x,'Ошибка в задании конца переходной ',4f9.2)
      close(1)
      stop
 805  continue
c   построение начального приближени с учетом моста  smax невзка
c при r=0 sp должен быть 0

cделим запас поровну, уменьшаем общую сумму на всю невязку иначе убираем
cmin запас и всё заново 
 806  continue      
       if(r.eq.0) go to 877
c r=0 нет доп. ограничений на сумму длин до моста
       if(sig1r.le.sp) go to 906
       smax=sig1r-sp
 901  continue
cзалезли мостом за правую границу на smax длины нужно уменьшать на невязку
        zap=smax
        j2=0
       do 907 i=1,j 
       k=t1(i)
       if(X(k).le.XMIN(k)) go to  907
       j2=j2+1
       if(X(k)-XMIN(k).ge.zap) go to 907
       zap=X(k)-XMIN(k)
       j3=k
 907  continue
       if(zap.ge.smax/j2) go to 908
        X(j3)=XMIN(j3)
        smax=smax-zap
        go to 901
 908  continue
       do 909 i=1,j 
       k=t1(i)
       if(X(k).le.XMIN(k)) go to  909
        X(k)=X(k)-smax/j2
 909  continue
c      do i=1,n
c      write(1,712) i,xmin(i),x(i),xmax(i)
c       enddo
 906  continue
      if(sig1r.ge.sl) go to 877
       smax=sl-sig1r
 902  continue
c залезли мостом за нижнюю границу длины увеличиваем на невязку
        zap=smax
        j2=0
       do 607 i=1,j 
       k=t1(i)
       if(X(k).ge.XMAX(k)) go to  607
       j2=j2+1
       if(XMAX(k)-X(k).ge.zap) go to 607
       zap=XMAX(k)-X(k)
       j3=k

 607  continue
       if(zap.ge.smax/j2) go to 608
        X(j3)=XMAX(j3)
        smax=smax-zap
        go to 902
 608  continue
       do 609 i=1,j 
       k=t1(i)
       if(X(k).ge.XMAX(k)) go to  609
        X(k)=X(k)+smax/j2
 609  continue
      !write(1,*) 'НАЧАЛО ПОСЛЕ КОРРЕКЦИИ ЗА МОСТ'
       !do i=1,n
       !write(1,712) i,xmin(i),x(i),xmax(i)
       !enddo
 877  continue
      
      write(1,709) r,epsx,ns
  709 format(2x,'признак доп. огр. r=',i3,' epsx=',f9.7,' ns=',f12.3)   
C------ BЫЧИCЛEHИE  F  И  G  ДЛЯ HAЧAЛЬHOГO ПPИБЛИЖEHИЯ
       it=0 ! это  число итераций 
       i=0
       CALL POINT(X,i,it) ! вычислил риxтовки
      
	CALL FUNC(X,F,C) ! Здесь С -третий1 столбец с 2mn+1
	! f- 10*сумма квадратов рихтовок,
	 !с-левые части ограничений без TH, умноженные на веса
	 CALL POINTG(it) 
! даёт с учётом множителей производные рихтовок по переменным X(i)
      CALL GRAD(0,X,A(1,N+1))
      if(it.eq.0) then
	WRITE(1,*) 'начало градиент  без ограничений A(i,n+1)'
   !   write(1,1713) (a(i,n+1), i=1,n)
      write(1,1613) F
	endif
 1613  format(2x,' без огр, Func=',f14.5)
       !close(1)
	 !stop
      DO 8 I=1,MN ! вместо M
c        if((typecr.eq.3).and. (c(i).eq.0.0)) th(i)=0.0
        ! IF(I.LE.MN.AND.C(I).LE.TH(I)) GO TO 8
	 IF(C(I).LE.TH(I)) GO TO 8
            CALL GRAD(I,X,A(1,N+4))
            F = F+SIGMA(I)*(C(I)-TH(I))*(C(I)-TH(I))/2.0
            DO 7 J2=1,N
               A(J2,N+1)=A(J2,N+1)+SIGMA(I)*(C(I)-TH(I))*A(J2,N+4)
       ! A(J2,N+4)-это производная от С(i) по X(j) с учётом множителей
c      if(j2.ne.5) go to 7
c      write(1,1323) i,j2,sigma(i),c(i),th(i),a(j2,N+4)
   7    CONTINUE
    8 CONTINUE
 1323 format(2x,'i=',i3,' j2=',i4,' sigma=',f8.3,' c=',f9.3,' th=',f8.3,
     #' gr=',f10.3)
      
	 WRITE(1,*) 'начало градиент суммарно A(i,n+1)'
    !  write(1,1713) (a(i,n+1), i=1,n)
       write(1,1614) F
 1713   format(2x, 5f14.6)
 1614  format(2x,' ALDFP  суммарно F=',f17.5)
       !close(1)
	 !stop
       ITACT=40
           		 
C------ ИTEPAЦИOHHЫЙ ЦИKЛ
C
c-------------------------
      iz=0 ! при нуле по матрицам иначе по сопряжённым градиентам
      im=0 ! при плохом направлении растёт до 4 потом перереход на сопряжённые
	izm=-2 ! при заходе в цикл увеличивается при добавлении или исключении 
! ограничений он нуль В итоге: при минус 1 строится проектор( единичная матрица)
! и шаг по проекции ,при нуле матрица уже промодифицирована DFP формула не используется
 ! но направление идёт перемножением
       ioss=0
      DO 100 IT=1,ITMAX
c	 if(it.gt.15) then
c	do i=1,n
c       write(1,712) i,xmin(i),x(i),xmax(i)
c       enddo
c       endif
      ! ioss=0
	 !if(it.gt.14) ioss=11
	if(ioss.eq.11) then
	write(1,*)'Счёт прерван'
	go to 702
	endif
       fold=F
       
 1712 format(2x,'1712it=',i4,' ro=',f9.4,' lamax=',f12.7,' epsla=',f10.7,
     * /' ns=',f14.4,' beta=',f15.4,' iz=',i3,' izm=',i3)
      izm=izm+1 ! был -2
      i11=0
C
C--------- OПPEДEЛEHИE AKTИBHOГO HAБOPA ПPOCTЫX OГPAHИЧEHИЙ
C
c       if(it.eq.5) go to 703
         DO 16 I=1,N
            IF (INDACT(I).NE.0) GO TO 16
            IF (X(I).LE.XMIN(I)+EPSX) GO TO 12
            IF (X(I).GE.XMAX(I)-EPSX) GO TO 13
c лишний оператор              INDACT(I) = 0
            GO TO 16
   12             CONTINUE
                  INDACT(I) = -IT
               GO TO 14
   13             CONTINUE
                  INDACT(I) = IT
   14          CONTINUE


C     Ошибка в начале и при обновлении надо иметь проектор с учётом моста и обнулять INDACT

        !  write(1,508)it, i,izm,INDACT(I)
	if(iz.ne.0) then
	izm=-1 !нельзя после добавления идти по сопряжённым
	go to 16 ! по сопряжённым нет работы с А
	endif
c          
          if(izm.lt.0) go to  16 !izm=-1 на 1-ой итерации и при обновлении после 
! плохого шага или направления  активный набор пополняется, 
! но пересчёта матрицы нет , так как идем по проекции и нет выбрасывания
          izm=0 !пересчёт по всем
c при izm=0 не исп DFP   формулу модификации
         ! write(1,508)it, i,izm,INDACT(I)
               
  508 format(3x,'it=',i4,' Добавили  i=',i3,' izm=',i3,' indact=',i4)
c         do 518 j3=1,N
c         write(1,506) j3,(A(j3,j2), j2=1,N)
c  506    format(2x,i2,' ',20f7.2) есть ниже
c  518    continue     
            zap=A(i,i)
           A(1,1)=A(1,1)-A(1,i)*A(i,1)/zap
c         write(1,21) A(i,i),A(1,1),i 
c   21   format(2x,'a(i,i)=',f6.2,' a(1,1)=',f6.2,' i=',i2)       
           do 19  j2=2,N          
            A(j2,j2)=A(j2,j2)-A(j2,i)*A(i,j2)/zap
           if(j2.eq.i) go to 19
            k1=j2-1
c         write(1,3222) j2,k1,A(j2,j2)
c 3222   format(2x,'j2=',i3,' k1=',i3,' A(j2,j2)=',f7.2)
           do 20  j3=1,k1
          if(j3.eq.i) go to 20
            A(j2,j3)=A(j2,j3)-A(j2,i)*A(i,j3)/zap
            A(j3,j2)=A(j2,j3)
c         write(1,23) j2,j3,A(j2,j3)
   23    format(2x,'j2=',i3,' j3=',i3,' A(j2,j3)=',f7.2)
   20 continue
   19 continue 
           do 97 j2=1,N
            A(j2,i)=0D0
            A(i,j2)=0D0
   97 continue 
c сделал точные нули можно и цикле do...  20 но вычисляет приближенно
c         write(1,*) '         НОВАЯ А'
c         do 517 k1=1,N
c         write(1,506) k1,(A(k1,j2), j2=1,N)
c  506    format(2x,i2,' ',20f7.2) есть ниже
c  517    continue     
   16    CONTINUE
c 16- конец цикла поиска активных и перeсчeта A
c пересчитана А с добавками от нового активного простого огр по одному, нет все огран сразу
c  добавка моста в А при построении проектора и далее мост не уходит
c определен активный набор и  сделаем без всяких модификаций 
c шаг по проекции в начальной точке
c А уже пересчитана и ее надо использовать? а izm=0 не даст???

 200  continue
       
C    ВЫВОД ОГРАНИЧЕНИЙ  ПО КОНЕЧНОЙ ТОЧКЕ
c нет  мост не вывожу здесь подготовка к проекции
c        write(1,310) r,j,j1
 310  format(/3x,'r=',i3,' j=',i3,' j1=',i3)
       j2=0
       j3=0

c мост не выводится
        if(r.eq.0) go to 789
cj2,j3- признаки отброшенных огр.j2-мост,j2<0 -низ,k1,k2-кол-во в сумме
c zap-новая проекция градиента после отбрасыв. простого при сумме ind1
c       if(ind1.eq.0) go to 205
       sig1=0.
       k1=0
       do 206 i=1,j
       k=t1(i)
      if(INDACT(k).ne.0) go to 206
       sig1=sig1+A(k,N+1) 
       k1=k1+1
  206  continue
  789  continue
c это нужно при построении проекции на мосту
c       if(it.lt.15)  write(1,306) it,izm,ind1
  306  format(/3x,'it=',i3,' izm=',i5,' ind1=',i3)
      if(izm.le.0) GO TO 777 !ничего не вывожу при обновлении или если уже вывел
c то есть если добавили или после шага по проекции после обновления
c когда izm стал 0 то не выводим ничего ???

  205  continue     
c конец ind1 в k1- число неактивных в сумме j2-послeдний со знаком
  207  continue 
C
C--------- BЫBOД ПPOCTЫX OГPAHИЧEHИЙ ИЗ AKTИBHOГO HAБOPA
c мост не выводится
           k=1
           k3=1
           pm=0
           pk=0
c        WRITE(1,*) 'перед выводом indact(i)'
c        WRITE(1,520) (INDACT(I), I=1,N)
  520  format(/3x,12i5) 
         if((G2.gt.1.0).and.(itact.gt.5)) go to 777
       !уже есть  if(izm.eq.0) go to 778 ! добавил, чтобы не выбрасывать при плохом шаге
 !!!! новое проверить формулы пересчёта А при добавках и исключения
!!! в книге исходная матрица ограничений записана как транспонированная          
c не исключать ничего при большой проекции
         DO 18 I=1,N
            if(r.eq.0) go to 211
            if(i.gt.t1(j)) go to 211 ! j число точек в t1
            if(i.ne.t1(k)) go to 212
            k=k+1
            IF (INDACT(I).EQ.0) GO TO 18
             IF ( IABS(INDACT(I)).GT.IT-ITACT) GO TO 18
  300  format(/3x,'ИСКЛ i=',i3,' ind1=',i3,' j2=',i3,'indact=',i3,
     *' izm=',i3,' A(i,i)=',f6.2,' An+1=',f15.4)
  213 continue
c при ind1<>0 теперь всегда ind1=1 и только одну выбрас на мосту pm
             if(pm.ne.0) go to 18
	if (ns.gt.0.01) ns=0.01 ! он портится 
             zap=A(I,N+1)-(sig1+A(I,N+1))/(k1+1)                        
             if(((INDACT(I).lt.0).and.(zap.ge.-ns)).or.
     *       ((INDACT(I).gt.0).and. (zap.le.ns))) go to 18
       write(1,301) i,ind1,sig1,indact(i),A(I,N+1),zap,k1
  301 format(/3x,'ИСКЛ 301 i=',i3,' ind1=',i3,' sig1=',f10.5,' indact=',i3,
     *         ' An+1=',f10.5,' zap=',f10.5,' k1=',i3)
      
             INDACT(I)=0
           i11=i11+1
             sig1=sig1+A(I,N+1)
             k1=k1+1
            pm=1
c         if(izm.eq.0) go to 99
c пересчет А
        if((k1.eq.0).or.(k1.eq.1)) pause 'k1==0 при pss'
           pss=1D0-1D0/k1
            do 93 ii=1,j
            i4=t1(ii)
            if(INDACT(i4).ne.0) go to 93
            pis=-1D0/k1
            if(i4.eq.i) pis=1D0+pis
            do 94 jj=1,j
            i5=t1(jj)
            if(INDACT(i5).ne.0) go to 94
            psj=-1D0/k1
            if(i5.eq.i) psj=1D0+psj
             A(i4,i5)=A(i4,i5)+pis*psj/pss
   94 continue
   93 continue
           izm=0
c добавил izm=0 чтобы не использовать DFP формулу
   99 continue
              go to 18
c        активен мост  выбросили простое если zap и indact(i) одного знака          
  211 continue
c 212 -ограничение на кривизну переменная X(i) не в списке 
  212 continue           
       if(ns.gt.0.01) ns=0.01 ! он портится 
            IF (INDACT(I).EQ.0) GO TO 18
            IF ( IABS(INDACT(I)).GT.IT-ITACT) GO TO 18
          IF (ISIGN(1,INDACT(I))*A(I,N+1).LE.2.0*ns) GO TO 18
cплохой способ определения отбрасываемого ограничения
        write(1,77) it,i,indact(i),izm,a(i,n+1),x(i),ns,itact,a(I,I)
   77  format(/2x,'ИСК 212 it=',i4,' i=',i3,' indact=',i4,' izm=',i3,
     *' a(i,n+1)=',f13.7,' x=',f13.7,' ns=',f7.4,
     *' itact=',i5,' a(i,i)=',f6.3)
               INDACT(I) = 0
c            if(izm.ne.0) A(i,i)=1D0
              a(i,i)=1D0
cтак правильно достаточно  1d0 без сложения так как был нуль
             i11=i11+1
             izm=0
	go to 777
c не было izm=0 надо ли ДА чтобы не модифицировать матрицу 
c но умножать ее надо
   18    CONTINUE
  777   continue
c сейчас решаем как идти модификатор или уже сделано или проектор
! izm=0 при добавлении или исключении и не нужна модификация но он же нуль после
! первой итерации (был -1) или после прохода по проекции после обновления
! как различать эти случаи ????
         if(izm.le.0) GO TO 778 ! при обновлении минус 1 после изменения нуль
         if(iz.ne.0) go to 778
C--------- MOДИФИKAЦИЯ MATPИЦЫ METPИKИ  H
Cбыло модификация не на месте надо модифицировать после добаления 
c или исключения там другие формулы а так происходит много модификаций
cпри многих исключених и добавках
C--------- Y=G-G1; HY=H*Y; YHY=Y*HY; YP=Y*P
C
         DO 71 I=1,N
  71    A(I,N+2)=A(I,N+1)-A(I,N+2)
         CALL MULTMV(A,A(1,N+2),A(1,N+4),N,INDACT)
         YHY=0
         YP=0
        DO 72 I=1,N
            IF (INDACT(I).NE.0) GO TO 72
               YHY=YHY+A(I,N+2)*A(I,N+4)
               YP=YP+A(I,N+2)*A(I,N+3)
c                 YP=YP-A(I,N+1)*A(I,N+3)
   72    CONTINUE
c        write(1,88) yhy,yp,la
   88  format(2x,'yhy=',g10.4,' yp=',g10.4,' la=',g10.4) 
C--------- H = H - HY*HY/YHY + LA*PP/YP
c YHY =0 если все активны проекци нуль а не выходим на конец пытась выбросить
c=======================старое
        ! if(dabs(YHY).lt.1.E-16) go to 778
c         if(yhy.eq.0) pause 'yhy==0'
         !YHY=1/YHY
        ! YP=LA/YP
       !  DO 79 I=1,N
         !   IF (INDACT(I).NE.0) GO TO 79
        !   A(I,I)=A(I,I)-A(I,N+4)**2*YHY+A(I,N+3)**2*YP
           !    IF(I.EQ.1) GO TO 79
          !     I1=I-1
         !      DO 76 J2=1,I1
        !          IF (INDACT(J2).NE.0) GO TO 76
       !              A(I,J2)=A(I,J2)-A(I,N+4)*A(J2,N+4)*YHY
   !  #                            +A(I,N+3)*A(J2,N+3)*YP
  !                   A(J2,I)=A(I,J2)
  ! 76          CONTINUE
   !79    CONTINUE
c==========================конец старой формулы DFP
c=======================новое
        ! if(dabs(YHY).lt.1.E-16) go to 778
c         if(yhy.eq.0) pause 'yhy==0'
c         YHY=1/YHY !БЫЛ КОММЕНТАРИЙ
c         YP=LA/YP   !БЫЛ КОММЕНТАРИЙ
          ro=LA+YHY/YP
         DO 79 I=1,N
            IF (INDACT(I).NE.0) GO TO 79
          A(I,I)=A(I,I)+(A(I,N+3)**2*ro-2.0*A(I,N+3)*A(I,N+4))/YP
              IF(I.EQ.1) GO TO 79
             I1=I-1
            DO 76 J2=1,I1
              IF (INDACT(J2).NE.0) GO TO 76
         A(I,J2)=A(I,J2)+(-A(I,N+4)*A(J2,N+3)-A(I,N+3)*A(J2,N+4)
     #                 +A(I,N+3)*A(J2,N+3)*ro)/YP
                     A(J2,I)=A(I,J2)
   76          CONTINUE
   79    CONTINUE
c==========================конец новой формулы Гольдфарба
c         write(1,*)' A после модификации '
c         do  i=1,N
c         write(1,506) i,(A(i,j2), j2=1,N)
c         enddo
  778   continue


c          write(1,507) izm,k1,k2,i11,i15
c          WRITE(1,*) 'после вывода indact(i)'
c         WRITE(1,520) (INDACT(I), I=1,N)
  507  format(/3x,'izm=',i4,' k1=',i2,' k2=',i2,' i11=',i3,' i15=',i3/)
        if(i11.eq.0) i15=0
C--------- ПPOEKЦИЯ ГPAДИEHTA И ПPOBEPKA УCЛOBИЙ BЫXOДA ПO  EPSF
C
           DO 32 I=1,N
           A(I,N+3)=A(I,N+1)
   32   CONTINUE
c       if((r.eq.0).or.(ind1.eq.0)) go to 218 ind1==1
       if(r.eq.0) go to 218
       do 219 i=1,j
	  k=t1(i)
       if( INDACT(k).ne.0) go to 219
       A(k,N+3)=A(k,N+3)-sig1/k1
  219  continue
  218  continue
        ! это корректировка проекции за мост       
 2219 format(2x,' it=',i4,' sumx=pss=',f12.6,' sumproekc=psj=',f16.5)	      
  220  continue
       G2=0D0
       DO 25 I=1,N
            IF (INDACT(I).EQ.0) GO TO 22
               A(I,N+3) = 0D0
            GO TO 25
   22          CONTINUE
              G2=G2+A(I,N+3)*A(I,N+3)
   25    CONTINUE
      
              DO 132 I=1,N
           A(I,N+3)=-A(I,N+3)
  132    CONTINUE
       
c теперь есть проекция и градиент       
        ! if(it.lt.18) go to 27
c         if((itmax.gt. 4825).or.(it.lt.30)) go to 27
       ! write(1,771) it,G2,ITACT,i11,i15,izm
       ! write(1,*) '   Проекция градиента '
       ! write(1,1713) (a(i,n+3), i=1,n)
  303 format(/3x,'i=',i3,'  A(I,N+3)=',f14.7)
  312 continue
   27 continue   
         IF (G2.GT.EPSF) GO TO 30 !проекция с 0.00005
         if(i11.eq.0) go to 31
         i15=i15+1
         if(i15.lt.5) go to 30
   31    continue
        WRITE(1,1714) it,ITACT,G2,iz,izm
 1714  format(/2x,'ВЫХОД по проекции it=',i4,' ITACT=',i3,' G2=',f19.6,
     @' iz='i3,' izm=',i3) 
            IF (ITACT.eq.0) GO TO 701
             if(ITACT.ne.40) go to 755
            ITACT=5
            GO TO 30
  755 continue
             ITACT= 0 
	 !GO TO 701 !временно
   30    CONTINUE
       if(iz.ne.0) go to 400 ! на сопряжённое направление А не нужна

C        if(it.gt.1) go to 401
         if(izm.ge.0) go to 401 !на первой итерации izm=-1
          ! и далее при обновлении нуль но матрица есть и проектор не нужен
   !       if(typecr.eq.3) go to 400
c         if(iz.eq.1) go to 400
 
cпроектор нужен на 1-ой итерации а потом при izm=0 он не нужен
         
c       Построение проектора только при izm=-1
            A(1,1) = 1D0
      if(INDACT(1).ne.0) A(1,1)=0D0
            DO 91 I=2,N
               A(I,I) = 1D0
      if(INDACT(i).ne.0) A(I,I)=0D0
               I1 = I-1
               DO 92 J2=1,I1
                  A(I,J2) = 0D0
                  A(J2,I) = 0D0
   92          CONTINUE
   91       CONTINUE
       if(r.eq.0) go to 500
        do 501 i=1,j
        k=t1(i)
        if(INDACT(k).ne.0) go to 501
        do 502 j2=1,j
        k3=t1(j2)
        if(INDACT(k3).ne.0) go to 502
        if(k1.eq.0) pause 'k1==0 при 502'
        A(k,k3)=A(k,k3)-1D0/k1
  502   continue
  501   continue        
c это учет моста в проекции а в А он вообще не учтен
c его активности он всегда активен!!
       ! только при izm=-1 построен проектор с учётом моста для активного наборы   
  500   continue
  401   continue
   29    continue
       !   if(iz.ne.0) go to 400 перенёс
        !  if(it.eq.1) go to 400

c на первой итерации идем по проекции но строили проектор для будущего
      !   write(1,2200)it,izm,iz
 2200 format(2x,'Перед MULTV it=',i4,' izm=',i4,' iz=',i4)
	    if(izm.eq.-1) go to 400 ! не умножаем а идём по проекции
c не надо  if(izm.eq.0) go to 400 то есть
c после выбрасывания или добавки не модифицируем но умножаем и после шага по проекции
c то есть после первой итерацции или посе одного шага после обновления
C--------- BЫЧИCЛEHИE HAПPABЛEHИЯ CПУCKA  P=-H*G

        CALL MULTMV(A,A(1,N+1),A(1,N+3),N,INDACT)
	 

         DO 332 I=1,N
           A(I,N+3)=-A(I,N+3) ! A*F , где f- градиент, А- проектор
  332    CONTINUE
  771  format(/5x,'it=', i3,' G2=',G12.2,'ITACT= ',i3,
     #' i11=',i3,' i15=',i3,' izm=',i3)
	 ! go to 28 нельзя обойти контроль направления
	  ! добавил для завершения построения направления спуска а контроль плохого напр!!
  400  continue
      !  если добавили, то нельзя по сопряжённым
       !  if((iz.ne.0).and.(izm.ne.-1)) then
       !	  do i=1,n
       !a(i,n+3)=a(i,n+3)+g2/g2old*a(i,n+2)
       !enddo
        !  endif ! временно убрал сопряжение
         ro=0d0
        do i=1,n
       ro=ro+a(i,n+3)*a(i,n+1)
       enddo
       ! if((it.eq.201).or. (it.eq.202)) then
       ! do i=1,n
	
       !write(1,1717) i,xmin(i),x(i),xmax(i),a(i,N+3),a(i,N+1)
       !enddo
         
	 !endif

       if(ro.le.0.0) go to 28 
	 ! острый угол с градиентом плохо!
        write(1,772)it,ro,iz,izm,im
  772 format(2x,'Плохое напр,it=',i4,' p*f=',g14.8,
     # ' iz=',i4,' izm=',i3,' im=',i2)

c        write(1,73)it,lamax,la,epsla,i11,iz,F,izm 
c        write(1,1715)iz, ITACT,G2,i15
         do i=1,n
c        write(1,1717) i,xmin(i),x(i),xmax(i),a(i,N+3),a(i,N+1)
c        a(i,N+3)=-a(i,N+3)
         a(i,n+3)=a(i,n+1)
         if(indact(i).ne.0) a(i,n+3)=0d0
! 1717 format(1x,'i=',i4,' xmin=',f13.7,' x=',f13.7,' xmax=',f9.3,
 !    #' a3=',f9.4,' a1=',f9.4)
         enddo
      if(r.eq.0) go to 1218
       do 1219 i=1,j
       k=t1(i)
       if( INDACT(k).ne.0) go to 1219
       A(k,N+3)=A(k,N+3)-sig1/k1
 1219  continue
      
 1218  continue
       do i=1,n
              if( INDACT(i).ne.0) a(i,n+3)=0d0
       a(i,n+3)=-a(i,n+3)
        enddo

            A(1,1) = 1D0
      if(INDACT(1).ne.0) A(1,1)=0D0
            DO 919 I=2,N
               A(I,I) = 1D0
      if(INDACT(i).ne.0) A(I,I)=0D0
               I1 = I-1
               DO 929 J2=1,I1
                  A(I,J2) = 0D0
                  A(J2,I) = 0D0
  929          CONTINUE
  919       CONTINUE
c сюда добавил проекцию с учетом моста
       if(r.eq.0) go to 1500
        do 1501 i=1,j
        k=t1(i)
        if(INDACT(k).ne.0) go to 1501
        do 1502 j2=1,j
        k3=t1(j2)
        if(INDACT(k3).ne.0) go to 1502
        if(k1.eq.0) pause 'k1==0 при 1502'
        A(k,k3)=A(k,k3)-1D0/k1
 1502   continue
 1501   continue        
 1500   continue
         ! восстановил проекцию  и начальный проектор
	  im=im+1
	g2old=g2
        if(im.eq.4)  iz=1 ! переход к сопряжённым градиентам
	 
	  
	 !iz=1 при плохом направлении попытаться ещё раз по матрицам
        izm=0 ! по проекции  пройдем здесь а дальше с модификацией или сопр
	! второго шага по проекции не будет как в начале с izm=-1
	! если здесь дать izm=-1 то будет в точности  обновление: шаг по проекции
	!izm станет 0 и заблокирует модификацию как если бы было исключение или добавка
c       stop 7
       ! конец плохое направление
   28 continue
       g2old=g2
       ! if(it.eq.4)then        
       ! do i=1,n
	 !write(1,1717) i,xmin(i),x(i),xmax(i),a(i,N+3),a(i,N+1)
        !enddo
       !endif  
	!if(it.eq.27)then        
	!write(1,*) ' Коррекция '
      ! do i=1,n
	
C
C--------- OПPEДEЛEHИE MAKCИMAЛЬHOГO ШAГA
C          И TOЧHOCTИ OДHOMEPHOГO ПOИCKA
C
         LAMAX = INFTY
          
         DO 40 I=1,N
       !if(it.eq. 71)  write(1,3313) i,n,a(i,n+3)
 3313 format(/2x,' i=',i3,' n=',i3,' a(i,n+3)=',g20.10)
         if(dabs(a(i,n+3)).lt.1.D-15) a(i,n+3)=0.0
           IF (A(I,N+3))41,40,42
   41          CONTINUE
      ! направление до минимума WGTVAR(2)==1Е-6 X=sig/WGTVAR(2)
 	         LAMAX1 = X(I)-XMIN(I)! идём вниз <0
               LAMAX1 = -LAMAX1/A(I,N+3)
      if(lamax1.lt.1.e-7) write(1,78) it,i,a(i,N+3),XMIN(I),X(I),lamax1
   78 format(/5x,'в границу it=',i3,' i=',i3,' a=',g20.10,
     & ' xmin=',g20.10,' x=',g20.10,' lamax1=',G20.10 ) 
               IF (LAMAX1.LT.LAMAX) LAMAX=LAMAX1
          	  
        GO TO 40
   42          CONTINUE
      		 LAMAX1 = XMAX(I)-X(I)! идём вверх >0
               LAMAX1 = LAMAX1/A(I,N+3)
       if(lamax1.lt.1.e-7) write(1,74) it,i,a(i,N+3),XMAX(I),X(I),lamax1
   74 format(/5x,'в границуit=',i3,' i=',i3,' a=',g20.10,
     &' xmax=',g20.10, ' x=',g20.10,' lamax1=',G20.10 ) 
               IF (LAMAX1.LT.LAMAX) LAMAX=LAMAX1
  40    CONTINUE
       ! LAMAX-минимальный до границы из всех ограничений больше нельзя
!поиск шага при котором линейная часть изменения рихтовки равна EE=0.005м
      ! if(it.eq.123) then
	!write(1,*)'123 направление A(i,n+3) '
      !write(1,1713) (a(i,n+3), i=1,n)
	!write(1,*)'DSHX(57,J)'
	!write(1,1713) (DSHX(57,j),j=1,n)
	!endif
	
	
	i1=JFIRST(1,1) ! первая точка на первой клотоиде
       s1=0.0
	do 11 i=i1,npoint
      s2=0.0
	do  j=1,N
	s2=s2+DSHX(i,j)*A(j,N+3)
      enddo  
      if(dabs(s2).gt.s1)then
	k=i
	s1=dabs(s2)
	endif
   11 continue 
      lamax1=EE/s1 !минимальный шаг по направлению для H=EE=0,005м 
	!if(it.ge.123) 	
     # !write(1,6314)s1,k,LAMAX,LAMAX1,EE,i1
 6314 format(2x,'s1= ',f12.7,' k=',i3,' Do gr LAMAX= ',g20.10,
     &'do EE  LAMAX1=',f12.7,' EE= ',f7.3,' i1= ',i4)
	   !if(it.lt.10) write(1,6313) it,Lamax,s1,LAmax1 
 6313  format(/3x,'6313 после 40 it=',i4,' do gr LAMAX=',f18.12,
     #/' s1=',f12.7,' LAMAX1=',f12.7)
  
C--------- OДHOMEPHЫЙ ПOИCK
  225  continue
c=====

  228  continue

	  ro=0d0
       beta=0d0
	ns=0
       do i=1,n
       beta=beta+a(i,N+1)*a(i,N+1)
	ro=ro+a(i,N+3)*a(i,N+1)
	ns=ns+a(i,N+3)*a(i,N+3)
       enddo
       beta=dsqrt(beta)
	ns=dsqrt(ns)
      ro=ro/ns/beta
	
	ns=0.01
	 CALL ALR(X,A(1,N+3),FUNC,N,EE,LAMAX,MN,ME,
     #SIGMA,TH,LA,A(1,N+4),C,it,LAMAX1)

	 ! до границы LAMAX, дискрет по ee LAMAX1

314   format(/3x,'после ALGERM',' it=',i4,' LAMAX=',f18.10,
     #/' la=',f15.10,' epsla=',f12.7)

 1715 format(/2x,'po шаг iz=',i4,' itact=',i4,' G2=',g10.4,' i15=',i3)


 1717 format(1x,'i=',i3,' xmin=',f10.3,' x=',f14.8,' xmax=',f9.3,
     #' a3=',g12.5,' a1=',g12.5)
       
C--------- BЫЧИCЛEHИE CЛEДУЮЩEЙ TOЧKИ
   60    CONTINUE
       if(la.lt.1.e-10 *LAMAX1) go to 702 ! выход по шагу
	   DO 61 I=1,N
        ! ns=ns+A(i,n+3)*a(i,n+3)
c ns нужен для правильного отбрасывания ограничений
         
            X(I)=X(I)+LA*A(I,N+3)
   61    CONTINUE
C
C--------- BЫЧИCЛEHИE  F  И  G  B HOBOЙ TOЧKE, COXPAHEHИE  G  B  G1
C
               DO 63 I=1,N
           A(I,N+2)=A(I,N+1)
         if(iz.eq.1) a(i,n+2)=a(i,n+3)
c        if(typecr.eq.3) a(i,n+2)=a(i,n+3)
   63  continue
         if(iz.ne.0) g2old=g2
cзапомнил старый градиент или проекцию
       
	!if(it.eq.27) i=300
	!нельзя it так как возвращает его же испорченный vis дал 1 чтобы не печатать
       i=1
	! i=it
	   CALL POINT(X,i,it)
	
         CALL FUNC(X,F,C)
c  new
        
	 i=itmax-it
	 write(string,'(i5)')i
	 flag=dlgset(dxy,ost,adjustl(string))
	 write(string,'(f12.3)')F
	 flag=dlgset(dxy,crit,adjustl(string))
        do while( PeekMessage (mesg, NULL, 0, 0,1) ) 
       if ( DlgIsDlgMessage(mesg) .EQV. .TRUE. ) then
           lret = TranslateMessage( mesg )
           ret  = DispatchMessage( mesg )
	  
       end if
        end do
c  new
       !if(it.gt.150)  write(1,7000) it,izm,F,la
7000  format(2x,'нов.т it=',i4,' izm=',i4,' Fч=',f11.5,' la=',f12.7)
      
	
	   CALL POINTG(it)
         CALL GRAD(0,X,A(1,N+1))
       
       !if(it.gt.150) then
       !write(1,10006) it, A(74,N+1),A(76,N+1)
10006  format(/2x,'after GRAD0 it=',i4,' a(74,N+1)=',f19.1,
     *' a(76,N+1)=',f19.1)
	             !  endif
cc new
	nevv=0d0
	  
	  
	   DO 65 I=1,MN

	if(C(I).le.TH(I)) go to 65
	F = F+SIGMA(I)*(C(I)-TH(I))*(C(I)-TH(I))/2.0
	 !nevv=nevv+(C(I)-TH(I))*(C(I)-TH(I))
      nevv=nevv+C(i)*C(i) ! это больше и правильно TH<0
cc new
               CALL GRAD(I,X,A(1,N+4))
               DO 64 J2=1,N
   64          A(J2,N+1)=A(J2,N+1)+SIGMA(I)*(C(I)-TH(I))*A(J2,N+4)
   65    CONTINUE
        !if(it.ge.150) then
       !write(1,10005) it,F
10005  format(/2x,'after GRAD it=',i4,' Fsum=',f12.4)
	    !          endif
cc new++++++++++++++++++++++++++++++++++==
      
       write(string,'(f12.3)')nevv
	 flag=dlgset(dxy,nev,adjustl(string))
       
	 do while( PeekMessage (mesg, NULL, 0, 0,1) ) 
       if ( DlgIsDlgMessage(mesg) .EQV. .TRUE. ) then
           lret = TranslateMessage( mesg )
           ret  = DispatchMessage( mesg )
       end if
        end do

cc new
       !if(it.ge.150)
      !#write(1,7001)F,fold,izm,lamax,iz,la,g2,it
7001  format(1x,'н F сум=',f17.5,' fold=',f17.5,' izm=',i3,
     #/,' lamax=',f14.7,' iz=',i1,' la=',f15.12,' g2=',g11.4,' it=',i5) 
       
	 
	 
	  if(izm.eq.-3) then !-2 сейчас это вместо комментария
	  ! -2 было при отрицательном шаге сейчас нет, 
!но можно попробовать перенести после малой df и сделать новый проектор всё с начала
             A(1,1) = 1D0
      if(INDACT(1).ne.0) A(1,1)=0D0
            DO 9191 I=2,N
               A(I,I) = 1D0
      if(INDACT(i).ne.0) A(I,I)=0D0
               I1 = I-1
               DO 9292 J2=1,I1
                  A(I,J2) = 0D0
                  A(J2,I) = 0D0
 9292          CONTINUE
 9191       CONTINUE
c сюда добавить проекцию с учетом моста не надо она строитс в цикле
! важно нужно ли модифицировать матрицу за мост выше этого нет ( при первом проходе)
! нет мост всегда активен он вводится  в проектор не исключается и не добавляется
       if(r.eq.0) go to 2500
        do 2501 i=1,j
        k=t1(i)
        if(INDACT(k).ne.0) go to 2501
        do 2502 j2=1,j
        k3=t1(j2)
        if(INDACT(k3).ne.0) go to 2502
        if(k1.eq.0) pause 'k1==0 при 2502'
        A(k,k3)=A(k,k3)-1D0/k1
 2502   continue
 2501   continue        
 2500   continue

       endif !izm=-2 было при отрицательном шаге теперь только вперёд ходим


	roold=ro
       ro=0d0
       beta=0d0
	ns=0.0
       do i=1,n
       beta=beta+a(i,N+1)*a(i,N+1)
	ro=ro+a(i,N+3)*a(i,N+1)
	ns=ns+a(i,N+3)*a(i,N+3)
       enddo
	!if(it.ge.150) then
	!write(1,10007) it,beta,ns
10007  format(/2x,' it=',i4,' beta=',f19.2,' ns=',f19.2)
	!close(1)
	!stop
	 !  endif
       beta=dsqrt(beta)
	ns=dsqrt(ns)
      ro=ro/ns/beta
	!if(it.ge.150) then
     	 !write(1,731) it,roold,ro,ns,beta
       !close(1)
	!stop
	!endif
	ns=0.01
	
  731   format(/2x,'новое it=',i3,' roold=',f12.5,' ro=',f12.5,
     #                  ' ns=',f12.5,' beta=',f12.5)
         if(fold-f.gt.0.00001) go to 100
       if(f.gt.fold+10) then
	write(1,1008) it,fold,f
 1008 format(/2x' Рост критерия it=',i4,' fold=',f13.2,' f=',f13.2)
	close(1)
	stop
	   endif
       write(1,73)it,lamax,la,i11,i15,iz,F,izm,im 
   73 format(/5x,'мало df it=',i4,' lamax=',f15.8,' la=',f15.12,
     #/' i11=',i3,' i15=',i3,' iz=',i3,' F=',f13.5,' izm=',i4,' im=',i2)
       write(1,1712) it,ro,lamax,ee,ns,beta,iz,izm
       write(1,1715)iz, ITACT,g2,i15
       !if(it.eq.4) then
	!close(1)
	!stop
	!endif
c          go to 703
       
         if(i11.eq.0) go to 322
           i15=i15+1
           if(i15.lt.5) go to 100
  322   continue
! если были исключения, то даю 5 попыток с малым шагом что-то сделать

c сюда добавить проекцию с учетом моста не надо она строитс в цикле
c       if(r.eq.0) go to 2500
c        do 2501 i=1,j
c        k=t1(i)
c        if(INDACT(k).ne.0) go to 2501
c        do 2502 j2=1,j
c        k3=t1(j2)
c        if(INDACT(k3).ne.0) go to 2502
c        if(k1.eq.0) pause 'k1==0 при 2502'
c        A(k,k3)=A(k,k3)-1D0/k1
c 2502   continue
c 2501   continue        
c 2500   continue
c k1- всегда вычисляется и не портится это число длин до моста
c  sig1-сумма компонент градиента
       if(iz.eq.1) go to 121
	       A(1,1) = 1D0
      DO 10 I=2,N
         A(I,I) = 1D0
         I1 = I-1
         DO 10 Jj=1,I1
            A(I,Jj) = 0D0
            A(Jj,I) = 0D0
   10 CONTINUE

c A можно и не делать единичной потом она выставлется в проекторе
      DO  I=1,N
      INDACT(I) = 0
	enddo 
       
        im=im+1
         
	if(im.ge.5) then
	iz=1
	go to 121
	endif
	izm=-2 ! при малых шагах пойдём по матрице
        go to 100
  121 continue
       ! что сначала: пробовать исключать или обновление?    
		  IF ((ITACT.eq.0).and.(iz.eq.1)) GO TO 702 ! выход по шагу
             if(itact.ne.0) go to 707
c             iz=1
             itact=40
c	izm=-2
             go to 100
  707 continue
             if(ITACT.ne.40) go to 756
            ITACT=5
c	iz=1 ! на всякий случай
c	izm=-2
            GO TO 100
  756 continue
             ITACT= 0 
          !   iz=1       
	!izm=-2 izm при iz=1 не работает
C вместо j поставил j2 j- занят под число доп. огр.
c       if((typecr.eq.3).and. (it.gt.1)) go to 703
C  конец цикла

  100 CONTINUE ! Конец цикла Далее it не определён !
C
C------ BЫXOД ПO  ITMAX
C
  703 CONTINUE
      write(1,*)'Выход по ITMAX'
      write(1,2199) it,itmax,f,g2,la,im
      ISTOP=2
      ITMAX=0

      write(1,311) ISTOP
  311 format(/8x,'ISTOP=',i3,' 2-ITMAX,0-EPSF,1-EPSX') 
      RETURN
C
C------ BЫXOД ПO  EPSF
C
  701 CONTINUE
      write(1,*)'Выход по проекции g2< EPSF'
      write(1,2199) it,itmax,f,g2,la,im
 2199 format(2x,'it=',i4,' itmax=',i5,' F=',f15.6,
     #' проекция= ',f15.6,' la=',f9.6,' im=',i3)
	 do i=1,n
       write (1,1717) i,xmin(i),x(i),xmax(i),a(i,N+3),a(i,N+1)
       enddo
             ISTOP=0
      ITMAX=ITMAX-IT
      write(1,311) ISTOP
     
      RETURN
C
C------ BЫXOД ПO  EPSX
C
  702 CONTINUE
      if(ioss.ne.11) 	write(1,*)'Выход по шагу ISTOP=1'
	               
      write(1,199) it,itmax,f,g2,iz,itact,la
  199 format(2x,'it=',i4,' itmax=',i5,' F=',f12.3,
     #' проекция= ',f12.4,' iz=',i3,' itact=',i3,' шаг la=',f15.7 )
      do i=1,n
       write (1,1717) i,xmin(i),x(i),xmax(i),a(i,N+3),a(i,N+1)
       enddo
	i=-2
	call point (x,i,it)
	 CALL FUNC(X,F,C)
    !    write(1,1199) f,la,epsf
 1199 format(2x,' чистая f=',f12.3,' шаг=',f15.7,' epsf=',f9.6)
c	 i=0
c       call point (x,i,it)
      istop=1
	      ITMAX=ITMAX-IT

      RETURN
      END
C______________________________________________________________________
C
      SUBROUTINE MULTMV(A,B,C,N,INDACT)
C
C      УMHOЖEHИE MATPИЦЫ  A(N,N)  HA BEKTOP  B(N) = C(N).
C      B BЫЧИCЛEHИЯX УЧACTBУЮT TOЛЬKO TE CTPOKИ И CTOЛБЦЫ MATPИЦЫ  A
C      И ЭЛEMEHTЫ BEKTOPA  B, ДЛЯ KOTOPЫX  INDACT(I)=0.
C      OCTAЛЬHЫE ЭЛEMEHTЫ BEKTOPA-PEЗУЛЬTATA  C  OБHУЛЯЮTCЯ.
C      ДBOЙHAЯ TOЧHOCTЬ.
C
      INTEGER N,INDACT(N),I,J
C
      REAL *8 A(N,N),B(N),C(N)
C
      DO 10 I=1,N
         C(I)=0
         IF (INDACT(I).NE.0) GO TO 10
            DO 1 J=1,N
               IF (INDACT(J).NE.0) GO TO 1
                  C(I)=C(I)+A(I,J)*B(J)
    1       CONTINUE
   10 CONTINUE
      RETURN
      END
