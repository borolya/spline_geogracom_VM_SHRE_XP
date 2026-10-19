
       
      SUBROUTINE POINT_new0(X,vis,it)
      INCLUDE 'INTERFCS.INC'
C----------------------------------------------------------------------
C на нулевой итерации (it=0)меняет углы поперечников (beta)на нормали
C и эти новые углы при оптимизации идут вместо прочитанных бета
CНа прямых рихтовки всегда по нормали с делением на косинус разности 
! нового и старого угла. beta вычисляет при первом  и при последнем вызове 
!из ALMO это при it=0 vis==3 даёт печать рихтовок и beta
! REo.for пишет в конец bufxy1 углы бета по 5 штук итоговые 
! там же и рихтовки Обсчёт всё учитывает
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
C____vis_=-1из ALMO, it from ALDFP, теперь нет -2 ALREGM
C
      REAL *8 X(1)
      integer vis,np,it
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
	real*8 dx,dy,dugl,PI,PI2
c     dx=xlpr-xl;аналогично; всё из nach_ugl.txt
      common/dobpech/ipc ! счётчик аварийных печатий в ERROR.txt
	integer ipc
	real*8 beta(1000),ugk(1000),lpclo(1000)
      real *8 x5,y5,b2,b3,s1,s2,s3,r,xc,yc,x1,y1,x2,y2
c x5,y5 точка пересечения с поперечником 
      REAL *8 KSI1,ETA1,PHI1,SIG1,scon,cross
      real *8 xkt,ykt,phikt,sigkt,xkt1,ykt1,phikt1,sigkt1
      INTEGER NC1
      integer i2 !! временно
	integer i1,i3 ! временно счётчик? для alregm
      write(1,5354)vis,it
 5354  format(/2x,'вход MYNEW point_new0 vis=',i5,' it=',i4)
	!if(vis.eq.-1) then
	!write(1,*)'pointnew0 beta  begin'
	!WRITE (1,903) (beta(j),j=1,npoint) 
	!endif
	!ipc=0 ! количество записей в файл ERROR,txt  обнулил в ALMO 
	!при проверке нач прибл из ALMO cделал  признак печати 0
	i2=0 ! нужен? и vis да
	PI=3.141592652
	PI2=PI/2.0
C------ HOBЫE ЗHAЧEHИЯ ПAPAMETPOB-восстановление 
C не варьируемые не входят в NVAR и не восстанавливаются,
! но рихтовки на них нужны!

  902 FORMAT(/' point: ПЕРЕМЕННЫЕ   '/(2X,5F12.8))
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

    1 format(2x,'i=',i4,' numvar(i)=',i4,' tip=',i3)
         GO TO 109
  104       CONTINUE
            L(NUMVAR(I)) = X(I)*WGTVAR(J1)
  109 CONTINUE
       WRITE (1,903) (beta(j),j=1,npoint) !903 в конце   
C--- Контроль  ЛИHИИ при равенстве радиусов на составной!!
! объединить окружности в одну в point.for  Здесь бвываем 2 раза     

       if(vis.eq.3) then
	 do i=1,npoint
      write(1,321) i,shmin(i),shift(i),shmax(i)
      enddo
  321 format(2x,'j=',i4,' min=',f9.4,' shift(j)=',f14.8,' max=',f9.4)
	 endif
C------ OБCЧET ЛИHИИ
C xev,yev-точка съемки на поперечнике,beta-угол с осью х,кроме первого
      NC1 = NCURV+1
!NCURV-число элементов, то есть строк в таблице без последней клотоиды
   22 format(2x,' l13=',f7.2,' la12=',f7.2,' l14=',f7.2,' la12=',f8.3)
       ugk(1)=dugl
10010  format(/2x,' dugl=',f10.7) !free

      DO 299 I=1,NC1
C
C--------- HAЧAЛO  I-0Й ПEPEXOДHOЙ
C
         IF (I.NE.1) GO TO 202
            KSI1 = dx
            ETA1 = dy
            PHI1 = dugl
            SIG1 = 0D0
c       J1 = 1 первую точку не сдвигаем и первый поперечник не задаём
          shift(1)=dsqrt(dx*dx+dy*dy)
	    if(dugl.lt.0) shift(1)=-shift(1)
            J1 = 2
	!  j1=2 верно  внутри цикла?
            j=2 
c нужен j=2 иначе при нулевой начальной нельзя на 219 клотоида
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
      ! конец клотоиды стал началом круговой или прямой
       !KSI(1,I)-начало клотоиды KSI(2,I) -начало круговой(прямой)
!JFIRST(1,I) - первая точка на клотоиде а JFIRST(2,I)- первая за клотоидой
 ! элементы состоят из кривая (или  прямая ) + клотоида а не наоборот
 ! как есть в таблице
      !if(i.eq.54)
      !*write(1,127)i,ksi(2,i-1),eta(2,i-1),phi(2,i-1),sig1,la(i-1),l(i)
  127  format(/2x,'i=',i3,' ks1=',f8.2,' eta1=',f8.2,
     #/2x,'phi1=',f8.5,' sig1=',f12.8,' la(i-1)=',f7.2,' l(i)=',f9.2)
       if(dabs(sig1).gt.0.00000001) go to 44 ! на окружность
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
c       if((i2.eq.38).and.(i.ge.8).and.(i.lt.22))
       !if(i.eq.54)
      !#  write(1,23) i,j1, ksi(1,i),eta(1,i),phi(1,i),sig(i-1)
   23  format(/2x,'M23окрнач i=',i3,' j1=',i4,
     #' ksi(1,i)=',f9.2,' eta(1,i)=',f9.2,
     #' phi(1,i)=',f9.6,' sig(i-1)=',f12.6)

  205    CONTINUE
        
C--------- HAЧAЛO  I-0Й KPУГOBOЙ (ПPЯMOЙ) Отсюда  начинаю с первой прямой
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
	i1=i
	if(dabs(SIG(i)).lt.dabs(SIG1))i1=i-1
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
	ipc=ipc+1
	 write(1,*) ' Не доп.пар.клотоиды k. Увеличить Rmax или ум.Rmin'  
	write(1,7778)it,i,K(i),sigmin(i1),sigmax(i1),
     #	Lmin(i),Lmax(i),x5,y5          
 7778 format(2x,'it= ',i4,' i=',i4,' k= ',f15.13,' sl= ',f12.6, 
     #/'sp= ',f12.6,' L1=',f7.2,' L2= ',f7.2,'x5=',f12.8,'y5=',f15.13)	
	write(1,*) ' Не доп.пар.клотоиды k. Увеличить Rmax или ум.Rmin'   
	write(7,7778)it,i,K(i),sigmin(i1),sigmax(i1),
     #Lmin(i),Lmax(i),x5,y5          
      !close(1)
	!close(7)
	!stop
	endif
C  пересчёт координат  по клотоиде при l=0не надо call
       !if(i.eq.54)
      !#write(1,30) i,k(i),l(i),sig1,sig(i)
   30 format(/2x,'i=',i3,' k=', f15.11,' l=',f7.2, 
     #' sig1=',f13.11,' sig(i)=', f13.11)
      
	!!новая conlf_1
 !     write(1,*) 'do conlf_1 point_new0'
!	write(1,18)scon, ksi(2,i),eta(2,i),phi(2,i)
!	write(1,17)i,ksi(1,i),eta(1,i),phi(1,i),sig1,k(i),l(i)
   17 format(/2x,'i=',i4,' ks1=',f13.5,' et1=',f13.5,
     *' ph1=',f9.6,' sig1=',f12.7,' k=',f12.8,' l=',f7.4)
   18 format(/2x,' scon=',f14.9,' ks2=',f13.5,' et2=',f13.5,
     *' ph2=',f9.6)

	call conlf_1(ksi(1,i),eta(1,i),phi(1,i),sig1,k(i),l(i),
     #ksi(2,i),eta(2,i),phi(2,i),scon)!sig1 scon кривизна по концам
 
       ! if(i.eq.54)
      !$  write(1,5)i, ksi(1,i),eta(1,i),phi(1,i),sig1,
      !# ksi(2,i),eta(2,i),phi(2,i),scon,l(i),k(i)
    5 format(/2x,'M5 i=',i4,'ksi(1,i)=',f12.5,' eta(1,i)=',f12.5,
     #/2x,' ksi(2,i)=',f12.5,' eta(2,i)=',f12.5,' phi(2,i)=',f9.5,
     #' scon=',f12.5,' l(i)=',f9.2,' k=',f12.9)
Cполучил начало круговой ( прямой) как конец клотоиды
C--------- CДBИЖKИ HA  I-1-0Й KPУГOBOЙ (ПPЯMOЙ)
C
   32  continue
       
         IF (J1.LE.NPOINT) GO TO 215
	!J1  начинался с 2 потом с первой точки за клотоидой
            JFIRST(1,I) = NPOINT+2
       !Здесь бываем!!??при первой точке за клотоидой вне участка 
      if(ipc.eq.0) OPEN(UNIT=7,FILE='ERROR.txt',STATUS='UNKNOWN')
	ipc=ipc+1
	  if(ipc.lt.4) then
      write(1,*)'Point_new0. Трасса ушла за правую точку'  
      write(7,*)'Point_new0.Трасса ушла за правую точку'                      
				  endif
	return
  215    CONTINUE
c KSI(1,I)X-НАЧАЛА СЛЕДУЮЩЕЙ ПЕРЕХОДНОЙ для 1-ого эл-та-конец прямой длиной XL
c KSI1-x- начала прямой(круговой)
c а если прямой нет! сразу на клотоиду на 219
        if((i.ne.1).and.(la(i-1).le.0.0001)) go to 219
       if((i.eq.1).and.(xl.lt.0.01)) go to 219
      np=0 ! число пересечений с окр нужно в crossokr 
	!если были то справа выход
 !      write(1,*) 'до 218 point_new0'
!	close(1)
!	stop  
	   DO 218 J=J1,NPOINT
c если прямая то с ней, если окружность то с ней с проверкой
         if(dabs(sig1).gt.0.000000001) go to 11  !на окружность
c пересечение с прямой ksi1,eta1,phi1- начало; ksi(1,i),eta(1,i) -конец
       
	!  x=(a-x0)*cos+(b-y0)*sin;это формулы для проверки
       ! y=-(a-x0)*sin+(b-y0)*cos;
	
	x5=(xev(j)-ksi1)*cos(phi1)+(yev(j)-eta1)*sin(phi1)
	s1=(ksi(1,i)-ksi1)*cos(phi1)+(eta(1,i)-eta1)*sin(phi1)!конец прямой
	if(x5.gt.s1+0.001)go to 219 ! на пересечение клотоиды
	
	 shift(j)=(xev(j)-ksi1)*sin(phi1)-(yev(j)-eta1)*cos(phi1)!-y
	if(it.eq.0)	 beta(j)=phi1+PI2 !normal только в начале
	ugk(j)=phi1 ! угол касательной нужен
	go to 218 ! следующий поперечник
   11  continue
      
	s2=(xev(j)-ksi1)* dcos(phi1)+(yev(j)-eta1)* dsin(phi1)
	!на касательную в начале ЭТО ПРОВЕРЯТЬ ТОЛЬКО ДЛЯ ПЕРВОГО!!!
       
	 if(s2.lt.-0.001) then ! был -0.01
 	write(1,111)i,j,s2,xev(j),yev(j)
  111 format(2x,'до окр i=',i3,' j=',i4,'s2= ',f7.3,'ksi1=',f9.3,
     #' eta1=',f9.3)
       if(ipc.eq.0) OPEN(UNIT=7,FILE='ERROR.txt',STATUS='UNKNOWN')
	!ipc=ipc+1
	write(7,111)i,j,s2,xev(j),yev(j)
	  close(1)
	close(7)
	stop
	endif
      s=(xev(j)-ksi(1,i))*dcos(phi(1,i))+
     #  (yev(j)-eta(1,i))*dsin(phi(1,i))
	!на касательную в конце
       if(s.gt.0.001) go to 219 !окр кончилась
      r=1./sig1  !>0 надо? нет
      xc=ksi1-r*dsin(phi1)
	yc=eta1+r*dcos(phi1) ! центр окружности 
!нужны сдвижки в пересеч с нормалями и углы касательных в ugk(j)
 !при полож кривизне точка вне -рихтовк плюс, внутри минус 
!при поиске угла нормали к окружности при пол из точки в центр 
! и далее из двух  в пределах 2 пи (120 или минус 240) взять
! ближайший к нормали в j-1 ой точке
      s=dsqrt((yc-yev(j))*(yc-yev(j))+(xc-xev(j))*(xc-xev(j)))
	! очень плохо при различии расстояний по Х и по У
      if(r.gt.0.0)then !нормаль в центр
	x1=xev(j)
	y1=yev(j)
	x2=xc
	y2=yc 
	 else
	x2=xev(j)! нормаль из центра это чтобы всегда -pi2 для касательной
	y2=yev(j)
	x1=xc
	y1=yc 
	endif 
! найти угол нормали с ОХ и ему альтернативу (120 или минус 240?   
       if(dabs(x2-x1).lt.0.001) then
      write(1,*) 'близкие Х1,Х2'
	s3=pi2
	if((y2-y1).lt.0.0)s3=-s3 ! нормаль
       ugk(j)=s3-pi2 ! касательная
	 x5=s-dabs(r) !отклонение точки съёмки от кривой
       if(r.lt.0.0)x5=-x5
	 shift(j)=x5
	if(it.eq.0)	beta(j)=s3
	go to 218
	endif ! вертикаль

      s1=datan((y2-y1)/(x2-x1))
      s3=s1 ! нужен дубликат
      if(x2.lt.x1)then ! 2+3 четверть
      s1=pi+s3
	s2=-pi+s3
	           else
       if(y2.ge.y1)then 
	    s2=s3-2.0*pi !альтернатива для 1-0й
	             else 
          s2=s3+2.0*pi   !альтернатива для 2-0й
		         endif
			  endif	 
	 !if(j.eq.65)write(1,14)j,phi1,s1,s2,s3				    
   14 format(2x,'j=',i4,' phi1=',f15.9,' s1=',f14.9,
     #' s2=',f14.9,' s3= ',f14.9)
	 !if(j.eq.65)write(1,179)x1,x2,y1,y2				    
  179 format(2x,'x1=',f12.7,' x2=',f12.7,' y1=',f12.7,' y2=',f12.7)
		 !!!!!!!!!!!!!!!!!
 	!scon=beta(j-1)! можно взять нормаль по касат в начале phi1+pi2
       scon=phi1+pi2
	 s3=s1
	  if(dabs(scon-s2).lt.dabs(scon-s1))s3=s2 	  ! ближайшая к j-1 
	 ugk(j)=s3-pi2 ! касательная
	 x5=s-dabs(r) !отклонение точки съёмки от кривой
       if(r.lt.0.0)x5=-x5
	 shift(j)=x5
	
	 !if(j.eq.65) write(1,115) scon,ugk(j),x5,r
  115 format(2x,'sc= ',f14.9,' ugkj= ',f14.9,' x5= ',f9.5,' r= ',f8.2) 
	!write(1,126) s1,s2,scon,beta(65) 
  126 format(2x,' s1= ',f12.7,' s2= ',f12.7,/' scon= ',f12.7,
     #' beta65= ',f12.7)
	if(it.eq.0) 	beta(j)=s3
	! # write(1,71)j,cross,scon,phi1,sig1,ugk(j),np
   71  format(/2x,' posle окр j=',i4,'cross=',f12.2,' scon=',f12.6,
     *' phi1=',f12.9,' sig1=',f12.7,' ugk(j)=',f12.9,' np=',i4)	    
       
	! go to 218
   28 format(/2x,'M28 do окр i=',i3,' j=',i3,
     #' ksi1=',f12.4,' eta1=',f12.4,' phi1=',f9.5,
     #/2x,' sig1=',f9.6,' xev=',f12.4,' yev=',f12.4,
     #' scon=',f9.6,' ksi(1,i)=',f12.4,' eta(1,i)=',f12.4,' np=',i4)
       
  218    CONTINUE
! КОНЕЦ ЦИКЛА ПО ПОПЕРЕЧНИКАМ  j ! 
!!ОНИ Кончились  а правее этого элемента нет поперечника
      if(ipc.eq.0) OPEN(UNIT=7,FILE='ERROR.txt',STATUS='UNKNOWN')
	ipc=ipc+1
      if(ipc.lt.5)then
	write(1,*)'POINTnew0  Окружность до конца трассы. Большой шаг'  
	write(1,777)it,i,la(i-1),j
  777  format(/2x,'point it=',i4,' i=',i3,' la(i-1)=',f12.6,' j=',i3)
       write(7,*)'POINTnew0  Окружность до конца трассы. Большой шаг'  
	write(7,777)it,i,la(i-1),j
	 endif
	 ! обрыв трассы на i-ом элементе прямой ( окружностью)
	! далее все клотоиды отметить
       do i2=i,NC1
         JFIRST(1,I2) = NPOINT+2
	  JFIRST(2,I2) = NPOINT+2
      enddo
	vis=0
      go to 320
	0 ! на конец для печати временно
  219       CONTINUE
      
       JFIRST(1,I) = J ! это параметр цикла из него вышли 218
      ! J-номер поперечника первого после прямой ( окружности),
	! то есть  потенциально начало клотоиды
	! есть ли эта клотоида и является ли она варьируемой???
	!if((i2.eq.6).and.(i.eq.24)) write(1,11111) i,j
	
	!if((i2.eq.38).and.(i.ge.8))
	 !if((i.eq.54).and.(j.gt.548).and.(j.lt.552))     
       !#write(1,11111) i,j,xev(j),yev(j),xev(j-1),yev(j-1)
11111 format(/2x,' 219 переход  с прям. или окр. i=',i3,' j=',i4,
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
	 if(ipc.lt.5)  write(1,588) i,j1,npoint	
	 if(ipc.lt.5)  write(7,588) i,j1,npoint	  
  588 format(2x,'588 клотоида ушла i=',i4,'j1=',i4,' npoint=',i4)
c            GO TO 299
        go to 320
  225    CONTINUE
         if(l(i).le.0.0001) go to 229
        ! np=0 ! зачем он ? проверить пересечение в начале
 7779	 continue
	s1=cos(phi(1,i))
      xc=(xev(J1)-ksi(1,i))*s1+(yev(J1)-eta(1,i))*sin(phi(1,i))
	if(xc.lt.-0.01) then
	 if(ipc.eq.0)OPEN(UNIT=7,FILE='ERROR.txt',STATUS='UNKNOWN')
	 write(7,*)' Пересечение левее клотоиды. Стоп.'
      write(1,12)i,J1,s
	write(7,12)i,J1,s
   12 format(2x,'12 pointnew левее клотоиды i= ',i3,' J1=',i3,'s=',f7.3)
      write(1,13),xev(J1),yev(J1),ksi(1,i),eta(1,i)
	write(7,13),xev(J1),yev(J1),ksi(1,i),eta(1,i)
   13 format(2x,' xj=',f9.3,' yj=',f9.3,' xk=',f9.3,' yk=',f9.3)
   	close(1)
	close (7)
	stop !пересечение первого левее клотоиды
	endif
     
       DO 228 J=J1,NPOINT
  119 format(2x,' клотоида до 228  i= ',i3,' j= ',i3,' j1=',i3)
	s1=cos(phi(2,i))
      s=(xev(j)-ksi(2,i))*s1+(yev(j)-eta(2,i))*sin(phi(2,i))
	if(s.gt.0.01)then 
      !if(j.eq.52) write(1,121)s,beta(j),xev(j),yev(j),
  !  $	 ksi(2,i),eta(2,i)
  121 format(2x,' за клотоиду 52 s= ',f7.4,' beta= ',f9.6,
     $' x= ',f12.6,' y= ',f12.6,' xk= ',f12.6,' yk= ',f12.6)
	 go to 229 ! за цикл прошли клотоиду
       endif
      xkt=ksi(1,i)
	 ykt=eta(1,i)
	 phikt=phi(1,i)
	 sigkt=sig1
	   do 230 i1=1,20
      call conlf_1(xkt,ykt,phikt,sigkt, k(i),xc,
     #xkt1,ykt1,phikt1,sigkt1)!
      xc= (xev(j)-xkt1)*cos(phikt1)+(yev(j)-ykt1)*sin(phikt1)
       !в новой точке на новую касат проекция 
       if(dabs(xc).lt.0.001)then
       yc=(xev(j)-xkt1)*sin(phikt1)-(yev(j)-ykt1)*cos(phikt1)
	  !ордината с минусом ==рихтовка
      !if(vis.eq.3 .and. j.eq.52)write(1,116)i,j,j1,yc
  116 format(2x,'на клотоидеi= ',i3,' j=',i3,' j1=' i3,' yc=',f8.5)  
	shift(j)=yc !рихтовка
       ugk(j)=phikt1
       if(it.eq.0)beta(j)=phikt1 +pi2
	lpclo(j)=(sigkt1-sig1)/k(i)  !x5 от начала клотоиды
	go to 228
      else
      xkt=xkt1
	 ykt=ykt1
	  phikt=phikt1 
	  sigkt=sigkt1 
      end if
  230 continue 
      if(ipc.eq.0)OPEN(UNIT=7,FILE='ERROR.txt',STATUS='UNKNOWN')
	ipc=ipc+1
	write(7,58)i1,i,j1,j 
	write(1,58)i1,i,j1,j
   58 format(2x,'point 58 Мало 20 i1=',i3,' i=',i3,' j1= ',i3,'j=',i3)
      if(vis.eq.0)write(1,*) 'metka 58'
	close(1)
	close(7)
	stop
	          
       ! *          write(1,27) i,j,scon,l(i),xev(j),yev(j),beta(j),x5,
       !* ksi(1,i),eta(1,i),ksi(2,i),eta(2,i)
   27 format(2x,'пер c кл i=',i4,' j=',i4,' scon=',f9.2,' l(i)=',f9.2,
     #/' xev=',f9.2,' yev=',f9.2,' beta=',f9.5,' lp=',f9.2,
     #/,' ksi1=',f14.6,' eta1=',f9.2,' ksi2=',f14.6,' eta2=',f14.6)
	
  228   CONTINUE ! конец цикла по точкам на пересечение клотоиды
      if(ipc.eq.0)OPEN(UNIT=7,FILE='ERROR.txt',STATUS='UNKNOWN') 
	ipc=ipc+1
	 if(ipc.lt.5) then
	 	write(1,*)'point_new0  Клотоида до конца трассы. Большой шаг'  
		write(1,778)it,i,l(i),j
  778  format(/2x,'point_new0 it=',i3,' i=',i3,' l(i)=',f12.6,' j=',i3)
       write(1,779) vis,ipc
  779  format(/2x,'pointnew0 779 vis=',i3,' ipc=',i3)
      write(7,*)'point_new0  Клотоида до конца трассы. Большой шаг'  
		write(7,778)it,i,l(i),j
       endif !ipc
       do i2=i,NC1
         if(i2.gt.i) JFIRST(1,I2) = NPOINT+2
	  JFIRST(2,I2) = NPOINT+2
      enddo
	go to 320
  229       CONTINUE
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
!       write(1,1212)NCURV,Npoint,JFIRST(2,NCURV+1),nc1,i,j
 1212 FORMAT(2x,'1212 NCURV=',i4,' NPOINT=',i4,' JF(2,)=',i4,'nc1=',i4,
     * /' i=',i4,' j=',i4)
      ! close(1)
	!stop
      IF (JFIRST(2,NCURV+1).GT.NPOINT) GO TO 320
      J1 = JFIRST(2,NCURV+1)
      DO 310 J=J1,NPOINT
         yc=(xev(j)-KSI(2,NCURV+1))*sin(phi(2,ncurv+1))-
     & (yev(j)-ETA(2,NCURV+1))*cos(phi(2,ncurv+1))
	  !ордината с минусом ==рихтовка
	 shift(j)=yc
	   ugk(j)=phi(2,ncurv+1)
	if(it.eq.0)	beta(j)= phi(2,ncurv+1)+pi2
	  	  
  310 CONTINUE
  320 CONTINUE 
      !if(vis.eq.-1) then
	!write(1,*)'pointnew0 beta + shifts end'
	!WRITE (1,903) (beta(j),j=1,npoint) 
	!do i=1,npoint
      !write(1,321) i,shmin(i),shift(i),shmax(i)
      !enddo
	!endif
	
      !if(vis.eq.3) then
      !write(1,*)"Point_new0 shift"
      !	 do i=1,npoint
      !write(1,321) i,shmin(i),shift(i),shmax(i)
      !enddo
	!do  j=1,npoint
      !write(1,322) j,shift(j),ugk(j),lpclo(j)
      !enddo 
          
  322 format(2x,'j=',i4,' shift(j)=',f9.4,' ugk=',f9.5,'lp=',f7.3)
 !     do 323 i=1,nc1
 !      write(1,324) i,JFIRST(1,I),JFIRST(2,I)
! 323 continue
  324 format(2x,'i=',i4,' JFIRST(1,I)=',I4,' JFIRST(2,I)=',I4)

      write(1,77)phifin,phi(2,ncurv+1)
   77 format(2x,' phifin=',f9.5,' phi(2,ncurv+1)=',f9.5)
         ! endif !vis=-10
  903 FORMAT(/'Point_new0: BETA'/(1X,5F14.6))     
!      WRITE (1,903) (beta(j),j=1,npoint)    
       !close(1)
	!stop
      !endif !vis==3
	vis=0 ! Для ALREGM нормальная работа
	 if(it.eq.0) then
	write(1,*)"Point_new0 shift it=0"
      	 do i=1,npoint
      write(1,321) i,shmin(i),shift(i),shmax(i)
      enddo
	endif
      RETURN
      END
