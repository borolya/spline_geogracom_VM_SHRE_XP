

      SUBROUTINE POINT(X,vis)
      INCLUDE 'INTERFCS.INC'
C----------------------------------------------------------------------
C
C       СТРУЧЕНКОВ В.И. ОКТ. 2004 КООРДИНАТЫ
C
C      OПTИMИЗAЦИЯ ЭЛEMEHTOB ПЛAHA PEKOHCTPУИPУEMOГO ИЛИ BTOPOГO ПУTИ
C      BEPCИЯ 3.0
C    
C        ПOДГOTOBKA K BЫЧИCЛEHИЮ KPИTEPИЯ, OГPAHИЧEHИЙ И ГPAДИEHTOB -
C      OПPEДEЛEHИE ПAPAMETPOB ЛИHИИ
C
C      BXOД: X,/POINTS/,/VARPTR/
C
C      BЫXOД: /LINE/,/SHIFTS/
C
C______________________________________________________________________
C
      REAL *8 X(1)
      integer vis,np
C     np=0 при первом заходе на окружность и при удаче он =1
C     нужен в crossokr при отсутствии пересечения дуги если не нуль то конец
 
cvis не нуль при аварийном выходе в самом начале приходим с -1 для рихтовок
c      external conlf,conlaf,crossokr,crossclo
c       real *8 crossokr,crossclo
C------------------->
C      X(NVAR) - ПEPEMEHHЫE(исходные поделенные!!! на масштабы)
C______________________________________________________________________
C
      INCLUDE 'COMMONS.INC'
      common/dob1/ beta,ugk,lpclo
	common /dob22/dx,dy,dugl
	real*8 dx,dy,dugl
c     dx=xlpr-xl;аналогично; всё из nach_ugl.txt
      common/dobpech/ipc
	integer ipc
	real*8 beta(1000),ugk(1000),lpclo(1000)
c      real beta(300),ugk(300),lpclo(300)
	
      real *8 x5,y5,b2,b3
c x5,y5 точка пересечения с поперечником 
      REAL *8 KSI1,ETA1,PHI1,SIG1,scon,cross
      INTEGER NC1
      integer i2 !! временно
	integer i3 ! временно счётчик для alregm
	!i2=vis
	
	i2=0
	 !if(vis.eq.300) i2=38
	!if(vis.lt.18)i3=0
	!if(vis.eq.18)then
	!	i3=i3+1
	!endif
	 !if(i3.eq.78) i2=38
	 !if(i3.ge.65)then
	 !i2=38
	 !endif
C
C------ HOBЫE ЗHAЧEHИЯ ПAPAMETPOB-восстановление 
C
c       vis=0
  902 FORMAT(/' point:     ПEPEMEHHЫE'/(2X,5F14.6))
      if(vis.eq.-1) WRITE (1,902) (X(I),I=1,Nvar)
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
	 !if(i.eq.153) write(1,1) i,numvar(i),j1
    1 format(2x,'i=',i4,' numvar(i)=',i4,' tip=',i3)
         GO TO 109
  104       CONTINUE
            L(NUMVAR(I)) = X(I)*WGTVAR(J1)
	!if(vis.eq.18) write(1,1) i,numvar(i),l(numvar(i))
  109 CONTINUE
C
C------ OБCЧET ЛИHИИ
C xev,yev-точка съемки на поперечнике,beta-угол с осью х,кроме первого
      NC1 = NCURV+1
       !if (i2.eq.38) write(1,22) l(13),la(12),l(14),la(13)
   22 format(2x,' l13=',f7.2,' la12=',f7.2,' l14=',f7.2,' la12=',f8.3)
c      ugk(1)=beta(1)
       ugk(1)=dugl
      DO 299 I=1,NC1
C
C--------- HAЧAЛO  I-0Й ПEPEXOДHOЙ
C
         IF (I.NE.1) GO TO 202
c            KSI1 = xev(1)
c            ETA1 = yev(1)
c           PHI1 = beta(1)
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
c            KSI(1,I)=XEV(1)+XL*COS(BETA(1))
c            ETA(1,I)=YEV(1)+XL*SIN(BETA(1))
c            PHI(1,I)=BETA(1)
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

	
       !if(i.gt.4)
      !*write(1,127)i,ksi(2,i-1),eta(2,i-1),phi(2,i-1),sig1,la(i-1),l(i)
  127  format(/2x,'point 127 i=',i3,' ks1=',f8.2,' eta1=',f8.2,
     #/2x,'phi1=',f8.5,' sig1=',f9.6,' la(i-1)=',f7.2,' l(i)=',f9.2)
       if(dabs(sig1).gt.0.00000001) go to 44
       ksi(1,i)=ksi1+la(i-1)*cos(phi1)
       eta(1,i)=eta1+la(i-1)*sin(phi1)
       phi(1,i)=phi1
         !if(i.gt.4)
       !# write(1,23)i, ksi(1,i),eta(1,i),phi(1,i),j1
       go to 205
   44 continue
         call conlaf(ksi(2,i-1),eta(2,i-1),phi(2,i-1),
     #sig(i-1),la(i-1),ksi(1,i),eta(1,i),phi(1,i))
c       if((i2.eq.38).and.(i.ge.8).and.(i.lt.22))
       !if(i.gt.4)
       !#  write(1,23) i, ksi(1,i),eta(1,i),phi(1,i),sig(i-1),j1
   23  format(/2x,'M23окрнач i=',i3,' ksi(1,i)=',f9.2,' eta(1,i)=',f9.2,
     #' phi(1,i)=',f9.6,' sig(i-1)=',f12.6,' j1=',i3)

  205    CONTINUE
      
C
C--------- HAЧAЛO  I-0Й KPУГOBOЙ (ПPЯMOЙ)
C       !  if(abs(SIG(I)-SIG1).lt.0.00000001) l(i)=0
         if(l(i).ge.0.0001) go to 31
         ksi(2,i)=ksi(1,i)
         eta(2,i)=eta(1,i)
         phi(2,i)=phi(1,i)
c sig(i)- кривизна следующей окружности известна
         k(i)=0.0
         go to 32
   31  continue
         K(I) = (SIG(I)-SIG1)/L(I)
C  пересчёт координат не по эвольвентам а по клотоиде при l=0не надо call
c      if(i.ge.1)
       !if(i.gt.4)
       !#write(1,30) i,k(i),l(i),sig1,sig(i)
   30 format(/2x,'i=',i3,' k=', f15.11,' l=',f7.2, 
     #' sig1=',f13.11,' sig(i)=', f13.11)
      call conlf(ksi(1,i),eta(1,i),phi(1,i),sig1,k(i),l(i),
     #ksi(2,i),eta(2,i),phi(2,i),scon)
c       if(i.ge.1)
      ! if(i.gt.4)
      !# write(1,5)i, ksi(1,i),eta(1,i),phi(1,i),
       !# ksi(2,i),eta(2,i),phi(2,i),scon,l(i)
    5 format(/2x,'M5 i=',i4,'ksi(1,i)=',f9.2,' eta(1,i)=',f9.2,
     #' phi(1,i)=',f9.5,
     #/2x,' ksi(2,i)=',f9.2,' eta(2,i)=',f9.2,' phi(2,i)=',f9.5,
     #' scon=',f12.5,' l(i)=',f9.2)
C
C--------- CДBИЖKИ HA  I-1-0Й KPУГOBOЙ (ПPЯMOЙ)
C
   32  continue
      
         IF (J1.LE.NPOINT) GO TO 215
            JFIRST(1,I) = NPOINT+1
       ! pause 'Нет последней прямой.Трасса ушла за правую точку'  
      if(ipc.eq.0) OPEN(UNIT=7,FILE='ERROR.txt',STATUS='UNKNOWN')
	write(7,*)'Нет последней прямой.Трасса ушла за правую точку'  
	write(7,*)'Счёт продолжен'
	ipc=1
c      close(1)
c       stop 7
c            GO TO 299
      vis=1
      return
  215    CONTINUE
c KSI(1,I) X-НАЧАЛА СЛЕДУЮЩЕЙ ПЕРЕХОДНОЙ для 1-ого эл-та-конец прямой длиной XL
c KSI1-x- начала прямой(круговой)
c а если прямой нет! сразу на клотоиду на 219
        if((i.ne.1).and.(la(i-1).le.0.0001)) go to 219
       if((i.eq.1).and.(xl.lt.0.01)) go to 219
      np=0 ! число пересечений с окр нужно в crossokr если были то справа выход
         DO 218 J=J1,NPOINT
c если прямая то с ней, если окружность то с ней с проверкой
         if(dabs(sig1).gt.0.000000001) go to 11
c пересечение с прямой
          b3=beta(j)
c- поставил проверку на вертикальность и поперечника и прямой
           b2=dsin(PHI1)
          if(b2.lt.0.0)b2=-b2
      if(b2.lt.0.99999) go to 12345
c-вертикальная прямая
       !if((i2.eq.38).and.(j.gt.559).and.(j.lt.563))
      !* write(1,1555) i,j,j1,b2,phi1,ksi1,xev(j),sig1
 1555 format(2x,'прi=',i4,' j=',i3,' j1=',i3,' b2=',f9.6,' phi1=',f10.7,
     #/' ksi1=',f12.5,' xev(j)=',f12.5,' sig1=',f13.6)
      x5=ksi1
      shift(j)=(ksi1-xev(j))/cos(beta(j))
cбыла ошибка без j
      y5=yev(j)+(ksi1-xev(j))*tan(beta(j))
      if(eta(1,i).lt.eta1) go to 12334
C- здесь по вертикали вверх идет трасса   
      if(y5.le.eta(1,i)) go to 1224
c-1224 внутри эл-та, 219 на следующий элемент
      go to 219   
12334 continue
C- здесь движение по вертикали вниз трасса
      if(y5.ge.eta(1,i)) go to 1224
      go to 219
12345 continue
c-  невертикальная прямая
          b2=dtan(PHI1)
         if(dabs(b3-3.1415926/2.0).lt.0.00001) goto 12
          b3=dtan(b3)
c          if((i.eq.3).and.(j.eq.34))
       !if((i2.eq.38).and.(j.gt.559).and.(j.lt.563))
      !#write(1,24) b2,b3,j1,j
   24 format(/2x,'прям.b2=',f12.5,' b3=',f13.5,' j1=',i4,' j=',i4)
       if(dabs(b3-b2).gt.0.00001) go to 13
	if(ipc.eq.0) OPEN(UNIT=7,FILE='ERROR.txt',STATUS='UNKNOWN')
	ipc=1
       write(7,6) j, xev(j),yev(j),ksi1,eta1,PHI1
    6  format(/2x,' нет пересечения поперечника j=',i4,' xev=',f7.2,
     #/2x,' yev=',f7.2,
     #' с прямой ksi1=', f7.2,' eta1=',f7.2,' phi=', f7.2)
	 vis=1
	write(7,*)'Счёт продолжен'
	return
c       close(1)
c       stop 
   13  continue   
       x5=(ETA1-YEV(j)+b3*XEV(j)-b2*KSI1)/(b3-b2)
  677 format(/2x,'  пересечение поперечника j=',i4,' xev=',f9.2,
     #/2x,' yev=',f9.2,' с прямой ksi1=', f9.2,' eta1=',f9.2,
     #/2x,' phi1=', f7.2,' x5=',f9.3,' ksi1(1,i)=',f9.3)
c         if(j.gt.71)
       !if((i2.eq.38).and.(j.gt.559).and.(j.lt.563))
	 !if(i.gt.4)
       !#write(1,677) j, xev(j),yev(j),ksi1,eta1,PHI1,x5,ksi(1,i)

       shift(j)=(x5-xev(j))/cos(beta(j))
       go to 14
   12  continue    
c вертикальный поперечник b3-его угол с осью X-b2-тангенс угла прямой с ОХ
       x5=xev(j)
       shift(j)= (ETA1+b2*(XEV(j)-KSI1)-YEV(J))*sin(beta(j)) 
   14  continue
c x5-абсцисса точки пересечения shift -сдвижка, возможно лишняя        
       if(KSI1.gt.ksi(1,i)) go to 1223
c       if(x5.lt.KSI1-0.0001) go to 218 и останется j без рихтовки
       if(x5.gt.ksi(1,i)+0.003) go to 219
       if(x5.ge.KSI1-0.003) go to 1224    
        if(ipc.eq.0) OPEN(UNIT=7,FILE='ERROR.txt',STATUS='UNKNOWN')
	  ipc=1
       write(1,*) 'слева от прямой при движении туда'
       write(7,177)i,j,x5,ksi1,ksi(1,i),xev(j),yev(j)
  177  format(2x,'i=',i3,' j=',i3,' x5=',f9.2,' ksi1=',f9.2,
     */2x,' ksi(1,i)=',f9.2,' xev(j)=',f9.2,' yev(j)=',f9.2)
       write(7,*)' ОШИБКА Счёт прекращён'
	 close(1)
	 close (7)
       stop 
 1223  continue
c - движение в обратную сторону
       if(x5.lt.ksi(1,i)-0.003) go to 219
       if(x5.le.KSI1+0.003) go to 1224

       if(ipc.eq.0) OPEN(UNIT=7,FILE='ERROR.txt',STATUS='UNKNOWN')
	  ipc=1 
       write(7,*) 'слева от прямой при движении  обратнo'
       write(7,177)i,j,x5,ksi1,ksi(1,i),xev(j),yev(j)
	write(7,*)'ОШИБКА! Счёт прекращён'
	close(1)
	close(7)
       stop 
c 218 на следующую точку нельзя! 219 за цикл т.е. новый элемент
 1224  continue
       ugk(j)=phi1
c         if((i.eq.3).and.(j.eq.34))
c       if((i2.eq.6).and.(j.gt.6).and.(j.lt.8))
c     #  write(1,25) j,x5,ksi1,shift(j)
   25  format(/2x,'прj=',i4,' x5=',f7.2,' ksi1=',f7.2,' shift(j)=',f7.2)
         go to 218
   11  continue

c     пересечение с окружностью 

       scon=beta(j)
      !if((i.ge.5).and.(j.eq.190))
	 !if(i.gt.4)
       !# write(1,28) i,j,ksi1,eta1,phi1,sig1,xev(j),yev(j),scon,
       !#ksi(1,i),eta(1,i)
      call crossokr(ksi1,eta1,phi1,ksi(1,i),eta(1,i),1./sig1,
     #xev(j),yev(j),scon,cross,x5,np)
       ugk(j)=x5
      !if((i2.eq.38).and.(i.ge.8).and.(j.eq.190))
	 !if(i.gt.4)
       ! # write(1,71)j,cross,scon,phi1,sig1,ugk(j)
   71  format(/2x,' posle окр j=',i3,'cross=',f9.2,' scon=',f12.6,
     *' phi1=',f12.9,' sig1=',f12.7,' ugk(j)=',f12.9)	    
      if(cross.gt.1.E+4) go to 219
       
	!ugkas в crossokr
      shift(j)=cross
	 go to 218
   28 format(/2x,'M28 do окр i=',i3,' j=',i3,
     #' ksi1=',f12.4,' eta1=',f12.4,' phi1=',f9.5,
     #/2x,' sig1=',f9.6,' xev=',f12.4,' yev=',f12.4,
     #' scon=',f9.6,' ksi(1,i)=',f12.4,' eta(1,i)=',f12.4)

       
  218    CONTINUE
       
c      pause 'Нет последней прямой.Трасса ушла за правую точку'  
      if(ipc.eq.0) OPEN(UNIT=7,FILE='ERROR.txt',STATUS='UNKNOWN')
      ipc=1
      write(7,*)'POINT  Окружность до конца трассы. Большой шаг'  
	write(7,777)i,la(i-1),j
  777  format(/2x,'point i=',i3,' la(i-1)=',f12.6,' j=',i3)
       write(7,*)'Счёт продолжен'
      ! close(1)
      ! stop 7
        vis=1
        return
c            JFIRST(1,I) = NPOINT+1
c         GO TO 220
  219       CONTINUE
      
            JFIRST(1,I) = J
	!if((i2.eq.6).and.(i.eq.24)) write(1,11111) i,j
	
	!if((i2.eq.38).and.(i.ge.8))
	  !if(i.gt.4)
       ! #	write(1,11111) i,j,xev(j),yev(j),xev(j-1),yev(j-1)
11111 format(/2x,' 219 переход  с прям. или окр. i=',i3,' j=',i4,
     #/'xev(j)=',f12.6,' yev(j)=',f12.6,' xj-1=',f12.6,' yj-1=',f12.6)
  220    CONTINUE
C
C--------- CДBИЖKИ HA  I-0Й ПEPEXOДHOЙ
C
         J1 = JFIRST(1,I)
         IF (J1.LE.NPOINT) GO TO 225
            JFIRST(2,I) = NPOINT+1
c       if(i.lt.3) write(1,588) i,j1,npoint
  588 format(2x,' плохой выход i=',i4,'j1=',i4,' npoint=',i4)
c            GO TO 299
        go to 320
  225    CONTINUE
c        if(l(2).le.00001) pause ' l(2)==0'
        
        if(l(i).le.0.0001) go to 229
         np=0
	   DO 228 J=J1,NPOINT
c-scon как рабочая  ячейка без постоянной роли
      b2=beta(j)
        
       if(dabs(k(i)).gt.0.000001) go to 7779
	 if(ipc.eq.0) then
	 OPEN(UNIT=7,FILE='ERROR.txt',STATUS='UNKNOWN')
	 ipc=1
	  
	write(7,7778)i,k(i),l(i)
 7778  format(/2x,'point Мал k i=',i3,' k(i)=',f12.8' l(i)=',f12.6)
       write(7,*)'Счёт продолжен'
      
         endif
	 scon=beta(j)
	 call crossokr(ksi(1,i),eta(1,i),phi(1,i),ksi(2,i),eta(2,i),
     # 1./sig1,xev(j),yev(j),scon,cross,x5,np)
       ugk(j)=x5
      if(cross.gt.1.E+4) go to 229
	shift(j)=cross
	!np=1
	 go to 228
	!ugkas в crossokr
 7779	 continue

      ! if(vis.eq.300) np=-1 ! для печати
      call crossclo(ksi(1,i),eta(1,i),phi(1,i),sig1,k(i),l(i),
     #xev(j),yev(j),b2,scon,x5,np)
   
       !if((i.gt.4).and.(j.lt.250))
       !#write(1,27) i,j,scon,l(i),xev(j),yev(j),beta(j),x5,
      !#ksi(1,i),eta(1,i),ksi(2,i),eta(2,i)
   27 format(2x,'перес кл i=',i4,' j=',i4,' scon=',f9.2,' l(i)=',f9.2,
     #/,' xev=',f9.2,' yev=',f9.2,' beta=',f9.5,' lp=',f9.2,
     #/,' ksi1=',f14.6,' eta1=',f9.2,' ksi2=',f9.2,' eta2=',f9.2)
c      if((j.gt.646).and.(scon.lt.-1900))
c     #write(1,688) i,j
  688 format(2x,' выход с 2-ой клотоиды i=',i4,' j=',i4)
      if(scon.lt.-1900) go to 229
c x5- длина до точки пересечения, scon- shift
      shift(j)=scon
      ugk(j)=phi(1,i)+x5*(sig1+x5*k(i)/2.0)
      lpclo(j)=x5
  228    CONTINUE
            JFIRST(2,I) = NPOINT+1
c         GO TO 299
        if(i.lt.nc1) then
	if(ipc.eq.0) OPEN(UNIT=7,FILE='ERROR.txt',STATUS='UNKNOWN')
	ipc=1
	 write(7,*)'POINT  Клотоида до конца трассы. Большой шаг'  
	write(7,778)i,l(i),j
  778  format(/2x,'point i=',i3,' l(i)=',f12.6,' j=',i3)
       write(7,*)'Счёт продолжен'
      !close(1)
      ! stop 7
        vis=1
        return
        endif
         go to 320
  229       CONTINUE
            JFIRST(2,I) = J
      !if(i.gt.4) write(1,22222)i,j,xev(j),yev(j),xev(j+1),yev(j+1)
22222 format(/2x,'229 с клотоиды  i=',i3,' j=',i4,
     #/' xevj=',f9.4,' yevj=',f9.4,' xev1=', f9.4,'yev1=',f9.4)
  299 CONTINUE
c       write(1,*)' point za 299'
C конец цикла по элементам
C--------- CДBИЖKИ HA ПOCЛEДHEЙ ПPЯMOЙ
C
c       write(1,1212)NCURV,Npoint,JFIRST(2,NCURV+1),nc1,i,j
 1212 FORMAT(2x,'NCURV=',i4,' NPOINT=',i4,' JF(2,)=',i4,'nc1=',i4,
     * /' i=',i4,' j=',i4)
      IF (JFIRST(2,NCURV+1).GT.NPOINT) GO TO 320
      J1 = JFIRST(2,NCURV+1)
      DO 310 J=J1,NPOINT
         if(dabs(PHI(2,NCURV+1)-3.1415926/2.0).gt.0.00001) goto 212
         if(dabs(beta(j)-3.1415926/2.0).lt.0.00001) goto 213
c phi-вертикален а поперечник нет 
       x5=KSI(2,NCURV+1)
       shift(j)=(x5-xev(j))/cos(beta(j))
       ugk(j)=phi(2,ncurv+1)
       go to 310
  212 continue
c касательная не вертикальна
          b3=beta(j)
          b2=dtan(PHI(2,NCURV+1))
c проверить вертикаль поперечника
         if(dabs(b3-3.1415926/2.0).lt.0.00001) goto 112
          b3=dtan(b3)
       if(dabs(b3-b2).gt.0.00001) go to 113
  213  continue
      if(ipc.eq.0) OPEN(UNIT=7,FILE='ERROR.txt',STATUS='UNKNOWN')
	ipc=1
	  write(7,*) 'На последней прямой'
      write(7,66) j, xev(j),yev(j),beta(j),
     # KSI(2,NCURV+1),ETA(2,NCURV+1),PHI(2,NCURV+1),b2,b3
  66  format(/2x,' нет пересечения поперечника j=',i4,' xev=',f7.2,
     #/2x,'yev=',f7.2,' beta=',f9.5,' с прямой x=', f7.2,
     #' y=',f7.2,' phi=',f7.2,' b2=',f12.5,' b3=',f12.5)
	  write(7,*)'Счёт продолжен'
        shift(j)=1.E+5
       ugk(j)=phi(2,ncurv+1)
	vis=1
	return
c        go to 310
c      close(1)
c	close(7)
c      stop 
  113  continue   
       x5=(ETA(2,NCURV+1)-YEV(j)+b3*XEV(j)-b2*KSI(2,NCURV+1))/(b3-b2)
       go to 114
  112  continue    
c вертикальный поперечник а прямая нет
       shift(j)= (ETA(2,NCURV+1)+b2*(XEV(j)-KSI(2,NCURV+1))-YEV(J))*
     # sin(beta(j)) 
c  это разность ординат точки пересечения и съёмки на поперечнике
c знак надо определять что и делаем умножая на sin(beta(j))
       ugk(j)=phi(2,ncurv+1)
c       write(1,*)'На последней прямой'
       go to 310
  114  continue
c       if(x5.lt.KSI(2,NCURV+1)) go to 310
       shift(j)=(x5-xev(j))/cos(beta(j))
       ugk(j)=phi(2,ncurv+1)
c       write(1,*)'На последней прямой'
  310 CONTINUE
  320 CONTINUE 
      if(vis.eq.-10) then
	 do 323 i=1,nc1
       write(1,324) i,JFIRST(1,I),JFIRST(2,I)
  323   continue
        !endif
      write(1,*)"shift"
      do 321 j=1,npoint
      write(1,322) j,shift(j),ugk(j),lpclo(j)
  321 continue
         
  322 format(2x,'j=',i4,' shift(j)=',f9.4,' ugk=',f9.5,'lp=',f7.3)
c      do 323 i=1,nc1
c      write(1,324) i,JFIRST(1,I),JFIRST(2,I)
c  323 continue
  324 format(2x,'Iold=',i4,' JFIRST(1,I)=',I4,' JFIRST(2,I)=',I4)

c      write(1,77)phifin,phi(2,ncurv+1)
   77 format(2x,' phifin=',f9.5,' phi(2,ncurv+1)=',f9.5)
            endif
      
c     close(1)
c	stop	
      vis=0 

      RETURN
      END
