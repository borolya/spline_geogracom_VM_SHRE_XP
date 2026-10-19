      

      SUBROUTINE ALR(X,S,FUNC,N,EP,LAMAX,MN,ME,SIGMA,TH,LA,Y,C,it,LAM)
	INCLUDE 'INTERFCS.INC'
	
      EXTERNAL FUNC 
	 REAL *8 SHIFT(1000),SHMIN(1000),SHMAX(1000)
      COMMON /SHIFTS/ SHIFT,SHMIN,SHMAX
 !LAM==LAMAX1-это шаг до ЕЕ,LAMAX,LAMAX0==L0- до границы в прямом и обратном
	REAL *8 X(N),S(N),SIGMA(1),TH(1),Y(N),C(1)
	real*8 EP,LAMAX,LA,LAM
	integer N,MN,ME,it
C_______________________________________________________________________
C ЕР==ЕЕ теперь  шаг изменения рихтовки в текущем направлении 
   ! Пока 0.005м
!LAM-шаг по направлению при котором линейная часть приращения 
 !всех рихтовок не более ЕЕ. LAM по EE вычисляю в ALDFP. EE приходит
 ! как EP, но она здесь не нужна  
 !LAMAX- шаг до ближайшей границы. Перебором с дискретом u,т.е. 
 !с сответствующим шагом определяю оптимум по направлению (LA)
C     СТРУЧЕНКОВ В.И.                                    08.09.88
C
C     ПOИCK MИHИMУMA MOДИФИЦИPOBAHHOЙ ФУHKЦИИ ЛAГPAHЖA
C     F(X) = FUNC(X) + 1/2*SIG*(C(X)-TH)**2
C        (KBAДPAT HEBЯЗKИ ДЛЯ OГPAHИЧEHИЙ-HEPABEHCTB BXOДИT
C         B CУMMУ TOЛЬKO ПPИ  C > TH)
C     HA ЛУЧE  X+LA*S; 0 <= LA <= LAMAX.
C     PEГУЛЯPИЗOBAHHAЯ ПPOЦEДУPA C KBAДPATИЧHOЙ AППPOKCИMAЦИEЙ.
C     (CM. Ф.ГИЛЛ, У.MЮPPEЙ, M.PAЙT. ПPAKTИЧECKAЯ OПTИMИЗAЦИЯ. C.123).
C     Нет просто перебором
C     ДBOЙHAЯ TOЧHOCTЬ.
C
C     BXOД: X,S,FUNC,N,EPS,LAMAX,MN,ME,SIGMA,TH
C     ----
C     BЫXOД: LA
C     -----
C     PAБOЧAЯ ПAMЯTЬ: Y
C     --------------
C
C     ПOДПPOГPAMMЫ:
C     ------------
C        FUNC(X,F,C) - OПИCAHA HИЖE.
C        POINT(X) - ПOДГOTOBKA BЫЧИCЛEHИЯ KPИTEPИЯ И OГPAHИЧEHИЙ
C                   B TOЧKE  X.
C it-номер итерации для отладки
C     AЛГOPИTM
C     --------
C_______________________________________________________________________

       common/dobpech/ipc
	
	 !ipc Используется признак возврата из POINT не нуль
      
       integer vis,i,j,k,k2,ipc  
      REAL *8 A,B,FA,FB,FLA,FLA0,U,U1
C----------------->
C      N - PAЗMEPHOCTЬ
C      MN - ЧИCЛO OГPAHИЧEHИЙ-HEPABEHCTB
C      ME - ЧИCЛO OГPAHИЧEHИЙ-PABEHCTB 0
C      X(N) - ИCXOДHAЯ TOЧKA N-MEPHOГO ПPOCTPAHCTBA
C      S(N) - HAПPABЛEHИE ПOИCKA
C      EPS - TOЧHOCTЬ ПOИCKA ! не нужна перешёл на перебор 0.005 рихтовок
C      LAMAX - MAKCИMAЛЬHOE ЗHAЧEHИE ШAГA ДО ГРАНИЦЫ
C      SIGMA(MN+ME),TH(MN+ME) - KOЭФФИЦИEHTЫ MФЛ
C      LA - OПTИMAЛЬHOE ЗHAЧEHИE ШAГA
C      Y(N) - PAБOЧИЙ MACCИB ДЛЯ XPAHEHИЯ TOЧEK  X+LA*S
C      C(MN+ME) - ЗHAЧEHИЯ OГPAHИЧEHИЙ
C      FUNC(X,F,C) - ПOДПPOГPAMMA, BЫЧИCЛЯЮЩAЯ ЗHAЧEHИЯ ФУHKЦИИ
C                    FUNC(X)  И OГPAHИЧEHИЙ  C(X)
C       EP=EE - шаг изменения рихтовок  в данном направлении
!      пока это 0.005 м ему соответствует LAM полученный как параметр
C_______________________________________________________________________
C
 
C             
      M = MN
       EE=EP
 !ЕЕ дискрет поиска в 0.005м по рихтовкам задан в ALDFP здесь не нужен 
	 !LAM дискрет поиска по направлению вычислен в ALDFP
	 k2=0 ! нужен для повтора при малом шаге	
      k=LAMAX/LAM ! число дискретов до границы
	!if(LAMAX.lt.Lam ) then 
	!write(1,571)it,k,Lamax,Lam
  571 format(2x,'it= ',i4,'k= ',i4,' Lamax= ',f12.8,' Lam= ',f12.8)
      
	u=LAM ! дискрет поиска по направлению
      a=0.
	b=LAMAX ! границы поиска шага
      if(k.lt.10) then ! близкая граница 
      k=10
	u=LAMAX/10 ! дискрет уменьшил а граница та же
      go to 177	
	endif
	if(k.gt.300)then
      k=300
	 b=300*LAM ! далеко не пойду дискрет тот же
	endif
  177 continue

     
	!if(it.ge.10)write(1,2222) it,k,LAMAX,LAM,shift(57)
 2222 format(2x,' ALR it=',i4,' k=',i4,' LAMAX=',f12.8,' LAM=',f15.8,
     # ' shif57= ',f12.8)
      	!+++++
	
! Нельзя не вводить ЕЕ и LAM так как не с чем идти от начала
! с увеличением вдвое ! k1=1 !было убрать
	 ! vis=-2 !в POINT при -1 и отсутствия пересечений
	 ! выход(расчёт рихтовок) 
	 CALL FUNC(X,FA,C)
   
         DO 1807 I=1,MN
	  IF (C(I).LE.TH(I)) GO TO 1807
       FA = FA+SIGMA(I)*(C(I)-TH(I))*(C(I)-TH(I))/2.0
 1807   CONTINUE
 1808  continue
         
   	
!+++++++++++++++++++++++++++++++++++++==
    7  continue
	  ! левая граница шага для перебора a=0,правая =b
	 DO  I=1,N
      Y(I)=X(I)+b*S(I)!test
      enddo 
	 vis=-2
       CALL POINT(Y,vis,it)
	
   23 format(2x,'ALR 23 it= ',i3,' LAMAX =',f12.8,' LAMOLD =',f12.8,
     */' LAM =',f12.8,' FA =',f12.8)
	
   33  format(2x,'33ALR New norm it= ',i3,' vis= ',i3,' LAMAX=',f12.8)  
       ! плохие ситуации отслеживаю в POINT vis ==1 не реагирую
	
    	 CALL FUNC(Y,FB,C)
       DO 1809 I=1,MN
       IF (C(I).LE.TH(I)) GO TO 1809
        FB = FB+SIGMA(I)*(C(I)-TH(I))*(C(I)-TH(I))/2.0
 1809   CONTINUE 
       
      
   12 CONTINUE
	!if(it.ge.18) write(1,1811) it,LAMAX,LAM,fa,fb,a,b
 1811  FORMAT(/2x,'alr 1811 it=',i4,' lamax=',f18.10,' LAM=',f18.10,
     #/' fa=',f13.6,' fb=',f13.6,' a=',f15.6,' b=',f12.8)
	!+++++есть fa,fb,b=LAM- до границы или первого >fa
 777  continue
      
   34 format(2x,'34 algerm posle ogr FB=',g15.7,' B=LAM= ',g15.7)
       u1=a 
       la=a
       fla0=fa  !fla0, u1-минимум при переборе
       if(fa.gt.fb) then
         fla0=fb
          u1=b !зачем это? затем если не найдём на интервале меньше
         endif
  700 continue
      
	!if(it.ge.10)write(1,701)it,a,b,u1,u,k,fla0
  701 format(2x,'Alr do it=',i3,' a=',f12.8,' b=',f12.8,' u1=',f12.8,
     &/' u=',f12.8,' k=',i4,' fla0=',f12.8)
      
	do 800 j=1,k
       la=la+u
            DO 110 I=1,N
  110       Y(I) = X(I)+la*S(I)
          vis=-2
             CALL POINT(Y,vis,it)
            CALL FUNC(Y,FLA,C)
               DO 111 I=1,MN
                  IF (C(I).LE.TH(I)) GO TO 111
                  FLA = FLA+SIGMA(I)*(C(I)-TH(I))*(C(I)-TH(I))/2.0
  111          CONTINUE
       
   70 format(2x,' la=',f12.9,' fla=',f14.8,' fla0=',f14.8,' u1=',f12.8) 
        if(fla.le.fla0) then
         u1=la
         fla0=fla
	    endif
	!if(it.ge.4 .and.it.le.10)  write(1,70) la,fla,fla0,u1
  800 continue !После деления интервала на 20 нашли минимум 
       !на нём включая концы перебором на исходном интервале
      !  делённом на 20 частей нашли миним fla0
       ! и шаг до этой точки u1
      !if(it.ge.10 )write(1,71) it,u,u1,fla,la,fa,fb,fla0
	
   71  format(2x,'800 it=',i3,' u=',f13.9,' u1=',f15.8,' fla=',f13.8,
     &/' la=',f13.8,' fa=',f15.7,' fb=',f15.7,' fla0=',f13.8)
         la=u1
	 if(la.ge.u) return
	!write(1,71) it,u,u1,fla,la,fa,fb,fla0
	if(k2.ne.1) then
      k2=1
      la=0.
	k=40
	u=u/40.
	go to 700
	endif
	
	  RETURN
      END
