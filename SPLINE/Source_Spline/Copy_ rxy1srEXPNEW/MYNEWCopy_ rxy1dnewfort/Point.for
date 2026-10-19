

      SUBROUTINE POINT(X,vis,it)
      INCLUDE 'INTERFCS.INC'
C----------------------------------------------------------------------
C
C       СТРУЧЕНКОВ В.И. ОКТ. 2004 КООРДИНАТЫ
C
C      OПTИMИЗAЦИЯ ЭЛEMEHTOB ПЛAHA PEKOHCTPУИPУEMOГO ИЛИ BTOPOГO ПУTИ
C      BEPCИЯ 3.0
C        ПOДГOTOBKA K BЫЧИCЛEHИЮ KPИTEPИЯ, OГPAHИЧEHИЙ И ГPAДИEHTOB -
C      OПPEДEЛEHИE ПAPAMETPOB ЛИHИИ
C
C      BXOД: X,/POINTS/,/VARPTR/
C
C      BЫXOД: /LINE/,/SHIFTS/
C
C____vis_=-1из ALMO, it from ALDFP, теперь нет -2 ALREGM
C
      REAL *8 X(1)
      integer vis,np,it
C     np=0 при первом заходе на окружность и при удаче он =1
C     нужен в crossokr при отсутствии пересечения дуги если не нуль 
       !то конец
 
cvis не нуль при аварийном выходе в самом начале приходим с -1 для рихтовок
c      external conlf,conlaf,crossokr,crossclo
c       real *8 crossokr,crossclo
C------------------->
C      X(NVAR) - ПEPEMEHHЫE(исходные поделенные!!! на масштабы)
C______________________________________________________________________
C
!на прямых рихтовки вычисляются по  текущей нормали и тут же 
!пересчитываются на прямую под углами бета, которые вычислены 
!на нулевой итерации в программе point_new0. рихтовка по нормали 
!( катет)делится на косинус разности углов текущей и исходной нормали
!это не так как в оригинале.Клотоиды и окр.через cross со старым углом	
	INCLUDE 'COMMONS.INC'
      common/dob1/ beta,ugk,lpclo
	common /dob22/dx,dy,dugl
	real*8 dx,dy,dugl
c     dx=xlpr-xl;аналогично; всё из nach_ugl.txt
      common/dobpech/ipc ! счётчик аварийных пеяатей в ERROR.txt
	integer ipc
	real*8 beta(1000),ugk(1000),lpclo(1000)
! верно lplo= длина клотоиды от начала до перес с j-м попереч их 1000
      real *8 x5,y5,b2,b3,PI,PI2,s1,s2
c x5,y5 точка пересечения с поперечником 
      REAL *8 KSI1,ETA1,PHI1,SIG1,scon,cross
      INTEGER NC1
      integer i1,i2 !! использую в циклах
	!integer i3 !i3 под вопросом
	PI=3.141592652
	PI2=PI/2.0
 	!if(it.lt.1) write(1,5354)vis,it
 5354  format(/2x,'вход  point vis=',i5,' it=',i4)
	!in ALMO ipc=0 ! признак найденных ошибок для записи 
	!при проверке нач прибл из ALMO cделал  признак печати 0
	!i2=0 ! нужен далее, vis это параметр при вызове -2,  
	
C------ HOBЫE ЗHAЧEHИЯ ПAPAMETPOB-восстановление 
C не варьируемые не входят в NVAR и не восстанавливаются, но рихтовки на них нужны!

  902 FORMAT(/' point:     ПEPEMEHHЫE'/(2X,5F14.6))
      ! начальная прямая не фиксируется!
      DO 109 I=1,NVAR
         J1 = TYPVAR(I)
	   GO TO (101,102,103,104),J1
  101       CONTINUE
            XL = X(I)*WGTVAR(J1)
         GO TO 109
  102       CONTINUE
            SIG(NUMVAR(I)) = X(I)*WGTVAR(J1)
         GO TO 109
  103       CONTINUE
            LA(NUMVAR(I)) = X(I)*WGTVAR(J1)
    1 format(2x,'i=',i4,' numvar(i)=',i4,' tip=',i7)
         GO TO 109
  104       CONTINUE
            L(NUMVAR(I)) = X(I)*WGTVAR(J1)
  109 CONTINUE
    2 format (2x,'it= ',i3,' i= ', i3,' num=',i4, ' l=',f12.2)

C--- Контроль  ЛИHИИ При vis>0 найти близкие радиусы и обнулить переходную
C  При равенстве радиусов объединить окружности в одну    
C Смотреть только варьируемые т.е. Х(i)
! Не проходит менять массивы T1,T2 и размерность N не просто !!!
        if (vis.le.0) go to 1110 
	 ! в начале с -1, при повторе с 0, alregm c -2, в процессе с it
         i=MOD(vis,100)
	if(i.ne.0) go to 1110
        DO 1109 I=1,NVAR
        J1 = TYPVAR(I)
	! 2-кривизна,4 длина клотоиды,3-длина.окружн
		if(j1.ne.2) go to 1109
		j1=NUMVAR(I) ! теперь это номер элемента
	if(L(j1).lt.0.001) go to 1109
		! корректировка клотоиды и числа переменных
	 if(j1.ne. NUMVAR(i-1)) go to 1109
!  Впереди переменная на этом же злементе и это длина клотоиды
!  значит это длина клотоиды, и её убрать из списка
!сдвинуть 3 массива
       if((j1.eq.1).or.(j1.eq. NCURV+1)) go to 1109
	if(dabs(sig(j1)).lt.1.e-7 .or.dabs(sig(j1-1)).lt.1.e-7)
     *go to 1109
	if(dabs(sig(j1)-sig(j1-1)).gt.dabs(sigmin(j1))) go to 1109
	if(ipc.eq.0) OPEN(UNIT=7,FILE='ERROR.txt',STATUS='UNKNOWN')
	  write(1,1111) j1, L(j1)  
      write(7,1111) j1, L(j1) 
	ipc=ipc+1 
 1109  continue
 1110  continue
 1111 FORMAT(2x,'ИСКЛЮЧИТЬ КЛОТОИДУ на эл-те' ,i4,' L=',f7.2)
   
C------ OБCЧET ЛИHИИ
C xev,yev-точка съемки на поперечнике,beta-угол с осью х,кроме первого
      NC1 = NCURV+1
!NCURV- число элементов, то есть строк в таблице без последней клотоиды
       
   22 format(2x,' l13=',f7.2,' la12=',f7.2,' l14=',f7.2,' la12=',f8.3)
       ugk(1)=dugl
       !write(1,10010) dugl
10010  format(/2x,' dugl=',f10.7)
       
      DO 299 I=1,NC1
C
C--------- HAЧAЛO  I-0Й ПEPEXOДHOЙ
C
         IF (I.NE.1) GO TO 202
            KSI1 = dx
            ETA1 = dy
            PHI1 = dugl
            SIG1 = 0D0
c         J1 = 1 первую точку не сдвигаем и первый поперечник не задаём
          shift(1)=dsqrt(dx*dx+dy*dy)
	    if(dugl.lt.0) shift(1)=-shift(1)
            J1 = 2
            j=2
c иначе при нулевой начальной нельзя на 219
            KSI(1,I)=dx+XL*COS(dugl)
            ETA(1,I)=dy+XL*SIN(dugl)
            PHI(1,I)=dugl
c вычислили конец прямой в beta(1) умышленно был угол касательной-начало
         GO TO 205
  202       CONTINUE
            KSI1 = KSI(2,I-1)
            ETA1 = ETA(2,I-1)
            PHI1 = PHI(2,I-1)
            SIG1 = SIG(I-1)
            J1 = JFIRST(2,I-1)
!KSI(1,I)-координата Х начала клотоиды,
!KSI(2,I) -начала круговой ( прямой)
!JFIRST(1,I) - номер первой точки на клотоиде)
! а JFIRST(2,I)- первой за клотоидой
!элементы состоят из кривая (прямая )+клотоида а не наоборот как  в таблице
      !if(i.eq.9 .and.it.ge.16)
      ! *write(1,127) it,ksi(2,i-1),eta(2,i-1),phi(2,i-1),sig1,la(i-1),l(i)
  127  format(/2x,'127 it=',i3,' ks1=',f8.2,' eta1=',f8.2,
     #/2x,'phi1=',f8.5,' sig1=',f12.8,' la(i-1)=',f7.2,' l(i)=',f9.2)
       if(dabs(sig1).gt.0.00000001) go to 44
! прямая её конец как начало клотоиды
       ksi(1,i)=ksi1+la(i-1)*cos(phi1)
       eta(1,i)=eta1+la(i-1)*sin(phi1)
       phi(1,i)=phi1
c         if(i.ge.1)
       !if(i.eq.54)
      !# write(1,23)i, ksi(1,i),eta(1,i),phi(1,i),j1  
       go to 205
   44 continue
         call conlaf(ksi(2,i-1),eta(2,i-1),phi(2,i-1),
     #sig(i-1),la(i-1),ksi(1,i),eta(1,i),phi(1,i))
   !даёт начало клотоиды
      !if(i.eq.9 .and.it.ge.16)
      !  #  write(1,23) i,j1, ksi(1,i),eta(1,i),phi(1,i),sig(i-1)
   23  format(/2x,'M23окрнач i=',i3,' j1=',i4,
     #' ksi(1,i)=',f9.2,' eta(1,i)=',f9.2,
     #' phi(1,i)=',f9.6,' sig(i-1)=',f12.6)
  205    CONTINUE
C
C--------- HAЧAЛO  I-0Й KPУГOBOЙ (ПPЯMOЙ) Отсюда  начинаю с первой прямой
C       !  if(abs(SIG(I)-SIG1).lt.0.00000001) l(i)=0
         if(l(i).ge.0.001) go to 31
         ksi(2,i)=ksi(1,i)
         eta(2,i)=eta(1,i)
         phi(2,i)=phi(1,i)
c sig(i)- кривизна следующей окружности известна
         k(i)=0.0
         go to 32
   31  continue
       i1=i
	if(dabs(SIG(i)).lt.dabs(SIG1))i1=i-1
         K(I) = (SIG(I)-SIG1)/L(I)
	if(sigmin(i1).gt.0.)then
       x5=dabs(1./sigmin(i1))! Rmax
	 y5=dabs(1./sigmax(i1))! Rmin 
	 else 
	x5=dabs(1./sigmax(i1))! Rmax
	y5=dabs(1./sigmin(i1)) ! Rmin
	 endif
	x5=1./(x5*Lmax(i))! 1/(RMAX*LMAX)
	y5=1./(y5*Lmin(i))
	if(dabs(K(i)).lt.(x5-1.e-8).or.dabs(K(i)).gt.y5+1.e-8) then
      if(ipc.eq.0) OPEN(UNIT=7,FILE='ERROR.txt',STATUS='UNKNOWN')
	write(1,*) ' Не доп.пар.клотоиды k. Увеличить Rmax или ум.Rmin'  
	write(1,7778)it,i1,K(i),sigmin(i1),sigmax(i1),
     $	Lmin(i),Lmax(i),x5,y5          
 7778 format(2x,'it= ',i4,' i1=',i4,' k= ',f15.8,' sl= ',f12.6, 
     #/'sp= ',f12.6,' L1= ',f7.2,' L2= ',f7.2,'x5=',f12.8,'y5=',f12.8)	
	write(1,*) ' Не доп.пар.клотоиды k. Увеличить Rmax или ум.Rmin'  
	write(7,7778)it,i1,K(i),sigmin(i1),sigmax(i1),
     $	Lmin(i),Lmax(i),x5,y5           
      write(1,7779)i,sig(i),sig1,L(i)
7779	format(2x,'i=',i5,' sig(i)=',f10.7,' sig1=',f10.7,' Li= ',f15.8)
	!close(1)
	!close(7)
	!stop
	endif
C  пересчёт координат  по клотоиде при l=0 не надо call
       !if(i.eq.9 .and.it.ge.16)
       !#write(1,30) i,k(i),l(i),sig1,sig(i)
   30 format(/2x,'30 i=',i3,' k=', f15.11,' l=',f7.2, 
     #' sig1=',f13.11,' sig(i)=', f13.11)
      
	!!новая conlf_1

	call conlf_1(ksi(1,i),eta(1,i),phi(1,i),sig1,k(i),l(i),
     #ksi(2,i),eta(2,i),phi(2,i),scon)
       !scon==кривизна в конце она не используется и правильно
       !if(i.eq.9 .and.it.ge.16)
       !$  write(1,5)i, ksi(1,i),eta(1,i),phi(1,i),sig1,
       !# ksi(2,i),eta(2,i),phi(2,i),scon,l(i),k(i),j1
    5 format(/2x,'M5 i=',i4,'ksi(1,i)=',f12.5,' eta(1,i)=',f12.5,
     #' phi(1,i)=',f9.5,' sig1=',f12.8,
     #/2x,' ksi(2,i)=',f12.5,' eta(2,i)=',f12.5,' phi(2,i)=',f9.5,
     #' scon=',f12.5,' l(i)=',f9.2,' k=',f12.9,' j1=',i4)
Cполучил начало круговой ( прямой) как конец клотоиды
C--------- CДBИЖKИ HA  I-1-0Й KPУГOBOЙ (ПPЯMOЙ)
      !sig1=scon !  не было  и не надо
   32  continue
      
         IF (J1.LE.NPOINT) GO TO 215
	!J1  начинался с 2 потом с первой точки за клотоидой
            JFIRST(1,I) = NPOINT+2
       !Здесь бываем при первой точке за клотоидой вне участка 
      if(ipc.eq.0) OPEN(UNIT=7,FILE='ERROR.txt',STATUS='UNKNOWN')      
	write(1,*)'Нет последней прямой.Трасса ушла за правую точку'  
	write(7,*)'Нет последней прямой.Трасса ушла за правую точку'  
	ipc=ipc+1
	if(ipc.lt.5)return
	close(1)
	close(7)
	stop
  215    CONTINUE
c KSI(1,I) X-НАЧАЛА СЛЕДУЮЩЕЙ ПЕРЕХОДНОЙ для 1-ого эл-та-конец прямой длиной XL
c KSI1-x- начала прямой(круговой)
c а если прямой нет! сразу на клотоиду на 219
       ! if(i.eq.9 .and.it.ge.16)write(1,6543)sig1,scon,la(i-1)
 6543 format(/2x,'6543 sig1=',f9.6,' scon=',f9.6,' la(i-1)=',f7.2)
	 
	  if((i.ne.1).and.(la(i-1).le.0.0001)) go to 219
       if((i.eq.1).and.(xl.lt.0.01)) go to 219
      np=0 ! число пересечений с окр нужно в crossokr
	     ! если были то справа выход
          
	   DO 218 J=J1,NPOINT
c если прямая то с ней, если окружность то с ней с проверкой
         if(dabs(sig1).gt.0.000000001) go to 11 !на окружность слева
c пересечение с прямой слева клотоиды
!sig1-кривизна в начале  клотоиды
      x5=(xev(j)-ksi1)*cos(phi1)+(yev(j)-eta1)*sin(phi1)! с нормалью
	s1=(ksi(1,i)-ksi1)*cos(phi1)+(eta(1,i)-eta1)*sin(phi1)!конец прямой
	s2=(xev(j)-ksi1)*sin(phi1)-(yev(j)-eta1)*cos(phi1)! это -y
	b3=beta(j)-(phi1+PI2)
	x5=x5-s2*dtan(b3)!добавка ==y*tg(beta-alf) пересечение
	! угол с ОХ поперечника и нормали к прямой
! дал поправку на точку пересеения поперечника и прямой
	 if(x5.gt.s1+0.001)go to 219 ! на пересечение клотоиды
	 shift(j)=s2 !это минус y
     	if(it.eq.0)	 beta(j)=phi1+PI2 !normal только в начале
      shift(j)=shift(j)/dcos(b3) !ПЕРЕСЧЁТ на прямой
	!При старом beta(j) рихтовка по старому а при новом по новому	
	ugk(j)=phi1 ! угол касательной нужен
	go to 218 ! следующий поперечник  
		        
   11  continue
c     пересечение с окружностью 
       scon=beta(j)
      !if((i.ge.9).and.((j.eq.160).or.(j.eq.161)))
      !# write(1,28) i,j,ksi1,eta1,phi1,sig1,xev(j),yev(j),scon,
       !#ksi(1,i),eta(1,i),np
      call crossokr(ksi1,eta1,phi1,ksi(1,i),eta(1,i),1./sig1,
     #xev(j),yev(j),scon,cross,x5,np)
       ugk(j)=x5
       
       ! if(i.eq.9 .and.it.ge.16 .and.j.ge.26)
       !# write(1,71)j,cross,scon,phi1,sig1,ugk(j),np
   71  format(/2x,' posle окр j=',i4,'cross=',f12.2,' scon=',f12.6,
     *' phi1=',f12.9,' sig1=',f12.7,' ugk(j)=',f12.9,' np=',i4)	    
      if(cross.gt.1.E+4) go to 219 !проскочили окружность
       
!ugkas в crossokr на 219 при нет пересечения т.е.вышли с окружности
      shift(j)=cross
	 go to 218
   28 format(/2x,'M28 do окр i=',i3,' j=',i3,
     #' ksi1=',f12.4,' eta1=',f12.4,' phi1=',f9.5,
     #/2x,' sig1=',f9.6,' xev=',f12.4,' yev=',f12.4,
     #' scon=',f9.6,' ksi(1,i)=',f12.4,' eta(1,i)=',f12.4,' np=',i4)
       
  218    CONTINUE
      ! КОНЕЦ ЦИКЛА ПО ПОПЕРЕЧНИКАМ  j Кончились поперечники а правее этого элемента нет поперечника
c      pause 'Нет последней прямой.Трасса ушла за правую точку'  
      if(ipc.eq.0) OPEN(UNIT=7,FILE='ERROR.txt',STATUS='UNKNOWN')      
	if(ipc.lt.6)then
	write(1,*)'POINT  Окружность до конца трассы. Большой шаг'  
	write(1,777)it,i,la(i-1),j
	write(7,777)it,i,la(i-1),j
	ipc=ipc+1
  777  format(/2x,'point it=',i4,' i=',i3,' la(i-1)=',f12.6,' j=',i3)
       endif
	 ! обрыв трассы на i-ом элементе прямой ( окружностью)
	! далее все клотоиды отметить
       do i2=i,NC1
         JFIRST(1,I2) = NPOINT+2
	  JFIRST(2,I2) = NPOINT+2
      enddo
	vis=0
      go to 320
            
       !  GO TO 220
  219       CONTINUE
      
            JFIRST(1,I) = J
      ! J-номер поперечника первого после прямой ( окружности),
	! то есть  потенциально начало клотоиды
	! есть ли эта клотоида и является ли она варьируемой???
       ! if(i.eq.9 .and.it.ge.16)
       ! #write(1,191)i,j,it,xev(j),yev(j),xev(j-1),yev(j-1)
  191 format(/2x,' 191 ход с прям. или окр. i=',i3,' j=',i4,' it=',i4,
     #/'xev(j)=',f12.6,' yev(j)=',f12.6,' xj-1=',f12.6,' yj-1=',f12.6)
  220    CONTINUE
C
C--------- CДBИЖKИ HA  I-0Й ПEPEXOДHOЙ
C
         J1 = JFIRST(1,I)
         IF (J1.LE.NPOINT) GO TO 225
            JFIRST(2,I) = NPOINT+2
      if(ipc.eq.0) OPEN(UNIT=7,FILE='ERROR.txt',STATUS='UNKNOWN')      
       ipc=ipc+1
	 if(ipc.lt.6) then 
	 write(1,588) i,j1,npoint
	write(7,588) i,j1,npoint
  588 format(2x,'клотоида ушла i=',i4,'j1=',i4,' npoint=',i4)
       endif
        go to 320
  225    CONTINUE
         if(l(i).le.0.001) go to 229
         np=0 ! 
	  DO 228 J=J1,NPOINT
c-scon как рабочая  ячейка без постоянной роли
      b2=beta(j)
 	!! новая  crossclo1
	call crossclo1(ksi(1,i),eta(1,i),phi(1,i),sig1,k(i),l(i),
     #xev(j),yev(j),b2,scon,x5,np)
   27 format(2x,'пер c кл i=',i4,' j=',i4,' scon=',f9.2,' l(i)=',f9.2,
     #/' xev=',f9.2,' yev=',f9.2,' beta=',f9.5,' lp=',f9.2,
     #/,' ksi1=',f14.6,' eta1=',f9.2,' ksi2=',f14.6,' eta2=',f14.6,
     *' it=',i4)
	
       ! SCON- сдвижка если пересечение за концом клотоиды то -2000
	 ! если до начала то -3000, если в пределах то сдвижка
	 if (scon.lt.-2500) then ! пересечение до начала клотоиды
	 if(ipc.eq.0)OPEN(UNIT=7,FILE='ERROR.txt',STATUS='UNKNOWN')
	  write(1,*)' Пересечение левее клотоиды. Стоп.'  
	write(1,27) i,j,scon,l(i),xev(j),yev(j),beta(j),x5,
     * ksi(1,i),eta(1,i),ksi(2,i),eta(2,i)
	 write(7,*)' Пересечение левее клотоиды. Стоп.'
	write(17,27) i,j,scon,l(i),xev(j),yev(j),beta(j),x5,
     * ksi(1,i),eta(1,i),ksi(2,i),eta(2,i)
	 close(1)
	 close (7)
       stop 
	endif           
       
      if(scon.lt.-1990) go to 229 ! поперечник правее клотоиды
       shift(j)=scon
      ugk(j)=phi(1,i)+x5*(sig1+x5*k(i)/2.0)
      lpclo(j)=x5
  228    CONTINUE !конец цикла по поперечникам
  !поперечники кончились до конца клотоиды
      if(ipc.eq.0) OPEN(UNIT=7,FILE='ERROR.txt',STATUS='UNKNOWN')
	ipc=ipc+1
	 if(ipc.lt.6) then
	 write(7,*)'POINT  Клотоида до конца трассы. Большой шаг'  
	write(7,778)it,i,l(i),j
	write(1,*)'POINT  Клотоида до конца трассы. Большой шаг'  
	write(1,778)it,i,l(i),j
  778  format(/2x,'point it=',i3,' i=',i3,' l(i)=',f12.6,' j=',i3)
       write(1,779) vis,ipc
  779  format(/2x,'point 779 vis=',i3,' ipc=',i3)
       endif 
       do i2=i,NC1
         if(i2.gt.i) JFIRST(1,I2) = NPOINT+2
	  JFIRST(2,I2) = NPOINT+2
      enddo
       go to 320
 229   CONTINUE
      !if(i.ge.9 .and.it.ge.16.and.j.ge.26)
      ! *  write(1,22222)i,j,xev(j),yev(j),xev(j+1),yev(j+1)
     	      JFIRST(2,I) = J
      ! если первой?(любой)  клотоиды нет, то была прямая или нет имеем JFIRST(1,I)= JFIRST(2,I) = J
      !if(i.ge.50)
       !#  write(1,22222)i,j,xev(j),yev(j),xev(j+1),yev(j+1)
22222 format(/2x, '229 с клотоиды  i=',i3,' j=',i4,
     #/' xevj=',f9.4,' yevj=',f9.4,' xev1=', f9.4,'yev1=',f9.4)
  299 CONTINUE
c     конец главного цикла по элементам  write(1,*)' point za 299'
C конец цикла по элементам
C--------- CДBИЖKИ HA ПOCЛEДHEЙ ПPЯMOЙ
C
       !write(1,1212)NCURV,Npoint,JFIRST(2,NCURV+1),nc1,i,j
 1212 FORMAT(2x,'1212 NCURV=',i4,' NPOINT=',i4,' JF(2,)=',i4,'nc1=',i4,
     * /' i=',i4,' j=',i4)
      ! close(1)
	!stop
      IF (JFIRST(2,NCURV+1).GT.NPOINT) GO TO 320
      J1 = JFIRST(2,NCURV+1)
    
      DO 310 J=J1,NPOINT
!++++++++++
       x5=(xev(j)-KSI(2,NCURV+1))*dcos(phi(2,ncurv+1))+
     # (yev(j)-ETA(2,NCURV+1))*dsin(phi(2,ncurv+1))
	 if(x5.lt.-0.01)then
	write(1,*) 'поперечник левее последней прямой'
	 write(1,212)j,x5,xev(j),yev(j),KSI(2,NCURV+1),ETA(2,NCURV+1),
     #phi(2,ncurv+1),beta(j)
  212	format(2x,' j= ',i4,' do x5= ',f7.2,' xev=',f12.4,' yev= ',f12.4,
     #/' ksi= ',f12.4,' eta=',f12.4,' phi= ',f9.6,' beta= ',f9.6)
       endif
	 shift(j)=(xev(j)-KSI(2,NCURV+1))*dsin(phi(2,ncurv+1))-
     #	 (yev(j)-ETA(2,NCURV+1))*dcos(PHI(2,NCURV+1))!-y
	if(it.eq.0)	 beta(j)=PHI(2,NCURV+1)+PI2 !normal только в начале
      shift(j)=shift(j)/dcos(beta(j)-(PHI(2,NCURV+1)+PI2))

!++++++++++++++
      
	ugk(j)=phi(2,ncurv+1)
      	         
  310 CONTINUE
  320 CONTINUE ! аварийная
  
      !if(it.le.1) then
      ! write(1,*)"POINT shift"
      !do 321 j=1,npoint
      !write(1,322)it, j,shift(j),lpclo(j)
  321 continue
          
  322 format(2x,'it= ',i3,' j=',i4,' shift(j)=',f9.4,' lp=',f7.3)
 !     do 323 i=1,nc1
 !      write(1,324) i,JFIRST(1,I),JFIRST(2,I)
! 323 continue
  324 format(2x,'i=',i4,' JFIRST(1,I)=',I4,' JFIRST(2,I)=',I4)

      ! write(1,77)phifin,phi(2,ncurv+1)
   77 format(2x,' phifin=',f9.5,' phi(2,ncurv+1)=',f9.5)
       !   endif !vis=-10
          	
      vis=0 ! Для ALREGM нормальная работа
       RETURN
      END
