


      SUBROUTINE POINTG(it)
      INCLUDE 'INTERFCS.INC'
C----------------------------------------------------------------------
C
C                                       03.03.89
C
C      OПTИMИЗAЦИЯ ЭЛEMEHTOB ПЛAHA PEKOHCTPУИPУEMOГO ИЛИ BTOPOГO ПУTИ
C      BEPCИЯ 3.0
C
C      ПOДГOTOBKA K BЫЧИCЛEHИЮ ГPAДИEHTOB - OПPEДEЛEHИE
C      ЧACTHЫX ПPOИЗBOДHЫX.
C
C      BXOД: /LINE/,/SHIFT/,/POINTS/,/VARPTR/
C
C      BЫXOД: /DERIV/,/DSHIFT/
C
C______________________________________________________________________
C
c      REAL *8 X(*)

      INCLUDE 'COMMONS.INC' 
	!в нём есть ksi(1,i),eta(i,i),phi(1,i) в начале i-ой клотоиды, c 2,i-в конце
	integer it
      common/dob1/ beta,ugk,lpclo
	real*8 beta(1000),ugk(1000),lpclo(1000),sdv,pov,nap,xa,ya,x,y
c      real beta(300),ugk(300),lpclo(300),sdv,pov,nap,xa,ya,x,y
	real*8 a,b,c,d,d1,sdvig,naprav
c углы с осью Х поперечников и касательных к плану трассы в точках пересечения
c sdv,pov,nap- соответственно производные сдвигов,поворотов
c варьируемой переменной и направления сдвигов может быть 1,2,3 если сложная
c вариация, например изменение кривизны меняет клотоиду слева и окружность, а справа????
c inach-  точка правее варьируемого элемента с неё начнем сдвиг и поворот
c  x,y- ее координаты, а xa1,ya1,xa2,ya2
c- координаты центра поворота ( конец клотоиды или окружности) 
      INTEGER NC1,JVAR,inach,i,j,i1,i2,j1,j2,jit,zn
      real*8 s,lp,s1
c	integer pr
c	pr=0
	jit=-22
	 

c- lp-длина от начала клотоиды до пересеч с поперечником
      NC1 = NCURV+1
c       write(1,77)phifin,phi(2,ncurv+1)
   77 format(2x,' phifin=',f9.7,' phi(2,ncurv+1)=',f9.7)
C------ BЫЧИCЛEHИE ПPOИЗBOДHЫX PИXTOBOK ПO  X
C
         DO 202 I=1,npoint
            DO 201 J=1,NVAR
         DSHX(I,J) = 0D0 ! производная рихтовки по переменной
  201    continue
  202    CONTINUE
C это от всех рихтовок  по всем переменным на начальной прямой они и останутся
c- для каждой переменной вычислить производимые ею сдвиги и повороты
c (точнее производные), разобраться с варьируемыми элементами, а затем всё, что
c правее обработать по сдвигам и поворотам
            
		  if(it.eq.jit) then
	do j=1,3
      write(1,7671) sig(j),k(j),l(j),la(j),phi(1,j),phi(2,j)
       enddo
	endif
 7671	 format(/2x,' sig=',f9.7,' k=',f9.7,' l=',f12.6,/' la=',f12.6,
     @ 'phi1=',f9.6,' phi2=',f9.6)
		  
		  DO 249 J=1,NVAR
               J1 = TYPVAR(J)
         JVAR = NUMVAR(J) !NUMVAR(1)==1 был не определён и не используется
       !if(it.eq.26) then
	  write(1,7675) j,j1,jvar,WGTVAR(J1)
 7675 format(2x,'j=',i4,' tip=',i4,' jvar=',i4,' WGTVAR(J1)=',f9.4)
       ! if(j1.eq.4) 
	  write(1,7670)jvar,JFIRST(1,jvar), JFIRST(2,jvar) 
 7670  format(2x,'jvar=',i4,' нач=',i4,' кон=',i4) 
	 !	 endif
c- jvar-номер варьируемого элемента в начальной нумерации, j- номер
c- переменной при фиксации некоторых элементов
c- jfirst(1,i)-первая точка на клотоиде jfirst(2,i)- первая точка за клотоидой
       
               GO TO (210,220,230,240),J1
C-----------------------ПPOИЗBOДHAЯ ПO  XL
  210             CONTINUE
       
       
      inach=JFIRST(1,1) 
      !sdv=1.0
      nap=ugk(1)
c      call sdvig(1.0,ugk(1))
       do  i=inach,npoint
       dshx(i,j)=dshx(i,j)+dsin(nap-ugk(i))/dsin(beta(i)-ugk(i))
        ! if(it.eq.26) then
	!s=nap-ugk(i)
	!s1=beta(i)-ugk(i)
       !write(1,400) i,j,  dshx(i,j) 
	 !write(1,500) nap,ugk(i),beta(i),s,s1,shift(i)
	!endif
c   обработкa от jfirst(1,1) до конца от сдвига
  400 format(/2x,'нач прямая i=',i4,' j=',i4,' dshx=',f10.3)
  500 format(/2x,' nap=',f9.6,' ugk(i)=',f9.6,' beta=',f9.6,
     @/' nap-ugk=',f9.6,' beta-ugk=',f9.6,' shift=',f9.4) 
       enddo
       !if(it.eq.26) go to 250 !
	 go to 249 ! временно 250 изучал XL
  220 CONTINUE
C -------------------ПРОИЗВОДНАЯ ПО КРИВИЗНЕ
       
c  производная по  sig меняются клотоиды слева и справа и окружность
c по 3  сдвига и поворота jvar-номер варьируемого элемента
       if(la(jvar).lt.0.0001) go to 249
c нет окружности -нет и кривизны
      if(l(jvar).lt.0.002) go to 221
c на окружность, начнем внутри левой разбираться
      i1=jfirst(1,jvar)      
      i2=jfirst(2,jvar)-1
c-это номера поперечников на концах клотоиды разобраться когда их нет????      
      if(i2.lt.i1) go to 222
c-вычисляем длину от начала клотоиды теперь она в массиве lpclo
       a=phi(1,jvar) ! угол касательной в начале  левой клотоиды
      ! ошибка кривизна нужна в начале клотоиды!!!
	  b=0.0
	 if(jvar.gt.1) b=sig(jvar-1)
	 c=k(jvar)
      do 223 j2=i1,i2
      !lp=lpclo(j2)
c      s1=phi(1,jvar)+lp*(s+k(jvar)*lp/2.0)
c-это угол касательной в точке пересечения он же ugk(j2)

      ! s1=lp*lp/l(jvar)*lp/(6.0*(sin(beta(j2)-ugk(j2))))

       
	 d=lpclo(j2)
       call dsdvpodk(a,b,c,d,sdvig,naprav)
	nap=naprav
       call znak(nap,ugk(j2),i)
	if(it.eq.jit) write (1,11) i
   11 format(2x,' znak i=',i5) 
	!if((k(jvar)*b.gt.0).and.(i.eq.0)) nap=nap+3.1415926
	!if((k(jvar)*b.lt.0).and.(i.eq.1)) nap=nap+3.1415926
	 if(i.eq.0) nap=nap+3.1415926
	s1=sdvig/l(jvar)*sin(nap-ugk(j2))/sin(beta(j2)-ugk(j2))
	dshx(j2,j)=dshx(j2,j)+s1
	!if(it.eq.jit) 
      if((j.ge.40).and.(j.lt.44))
     $ write(1,401) j,j2,s1,d,sdvig,nap,i,beta(j2),ugk(j2)
  223  continue

  401 format(2x,'добавка внутри левой за кривизну j=',i4,' j2=',i4,
     #' s1=',f12.5,' lp=',f7.2,/'sdv=',f12.3,' nap=',f9.6,' zna=',i5,
     #' beta=',f12.6,' ugk=',f9.6)
c- закончили внутри левой клотоиды теперь ее влияние на правую часть

  222 continue
       inach=jfirst(2,jvar)
       !nap=phi(2,jvar)+3.1415926/2.0
       !sdv=l(jvar)*l(jvar)/6.0
        a=phi(1,jvar) ! угол касательной в начале левой клотоиды
	b=0.0
	if(jvar.gt.1) b=sig(jvar-1)
	 c=k(jvar)
	 d=l(jvar)
       call dsdvpodk(a,b,c,d,sdvig,naprav)
	nap=naprav
	s=phi(2,jvar)
	 call znak(nap,s,zn)
	!if((k(jvar)*sig(jvar).gt.0).and.(zn.eq.0)) nap=nap+3.1415926
	!if((k(jvar)*sig(jvar).lt.0).and.(zn.eq.1)) nap=nap+3.1415926
       if(zn.eq.0) nap=nap+3.1415926
	sdv=sdvig/d ! это производная по сигме справа
      	
c       call sdvig(sdv,nap)
       do  i=inach,npoint
	 
       s1=dsin(nap-ugk(i))/dsin(beta(i)-ugk(i))*sdv
	!if(it.eq.jit) 
	if((j.ge.40).and.(j.lt.44))
     $ write(1,402) i,j,sdvig,sdv,nap,zn,ugk(i),beta(i),s1
  402 format (/2x,' по сигме от левой сдвиг i=',i3,' j=',i3,
     @/' sdvig=',f12.3,' sdv=',f12.3,
     #' nap=',f9.6,' zn=',i5,' ugk=',f9.6,' beta=',f9.6,' s1=',f12.4)   

       dshx(i,j)=dshx(i,j)+s1
        enddo
       xa=ksi(2,jvar)
       ya=eta(2,jvar)
       pov=l(jvar)/2.0
c       call rot(xa,ya,pov)
       do  i=inach,npoint
       x=xev(i)+shift(i)*dcos(beta(i))
       y=yev(i)+shift(i)*dsin(beta(i))
       s1=((x-xa)*dcos(ugk(i))+(y-ya)*dsin(ugk(i)))/
     # dsin(beta(i)-ugk(i))*pov
       dshx(i,j)=dshx(i,j)+s1
       !if(it.eq.jit)       
	if((j.ge.40).and.(j.lt.44))
     $   write(1,403) i,j,s1,pov,ugk(i),beta(i),x,xa,y,ya
  403  format(/2x,'rot от левой клотоиды при сигме i=',i3,' j=',i3,
     #' s1=',f12.4,' pov=', f7.2,' ugk=',f9.6,' beta=', f9.6,
     #/' x=',f9.2,' xa=',f9.2,' y=',f8.3,' ya=',f8.3) 
       enddo
  221  continue
c-закончили с левой переходим к круговой сначала внутри
      i1=jfirst(2,jvar)      
      i2=jfirst(1,jvar+1)-1
c-это номера поперечников jfirst(1,jvar) первый внутри клотоиды jfirst(2,jvar) первый за ней
c   разобраться когда их нет????      
      if(i2.lt.i1) go to 252       
      !if(it.eq.jit)
	if((j.ge.40).and.(j.lt.44))
     $	 write(1,5403) sig(jvar),la(jvar),phi(2,jvar)
 5403  format(/2x,'окружность sig=',f12.8,' la=',f12.6,' phi=',f12.8)
      do 253 j2=i1,i2
c      s1=-(1-cos(ugk(j2)-phi(2,jvar))) неверно в конечной точке должно давать общий сдвиг
c     #sin(ugk(j2)-beta(j2))/sig(jvar)/sig(jvar)



cc      s=ugk(j2)-phi(2,jvar) !это угол поворота от начала кривой
cc	
cc      s1=2.0-2.0*cos(s)-2.0*s*sin(s)+s*s 
cc       if(s1.lt.0.0) then
cc      write(1,7676) j,i1,jvar,j2,ugk(j2),phi(2,jvar),s,s1
 7676 format(/2x,'минус в pointg внутри круговой j=',i4,/
     #' i1=',i3, ' jvar=',i4,' j2=',i4,' ugk=',f9.5,' phi=',f9.5,
     #	' s=',f12.10,' s1=',f14.12)
cc	endif
       a=ugk(j2)-phi(2,jvar)
       b=2.0-2.0*dcos(a)-2.0*a*dsin(a)+a*a
	if(b.lt.-0.000001) then
      write(1,7676) j,i1,jvar,j2,ugk(j2),phi(2,jvar),a,b
	close(1)
	stop
	endif
	if(b.lt.1.e-12) b=0.0 
       ! при  b<-0.000001 уже ушли!!!
      sdv=dsqrt(b)
	s=a
      sdv=sdv/sig(jvar)/sig(jvar)! это абсолютная величина сдвига
      s1=(1-dcos(s)-s*dsin(s))/(dsin(s)-s*dcos(s))
	if(dabs(dsin(s)-s*dcos(s)).lt.0.000000001) then
	nap=3.1415926/2.0
	if((1-dcos(s)-s*dsin(s)).gt.0.0) nap=nap+3.1415926
	write(1,*)' M1 ALREGM'
	GO TO 1
	endif
	
      nap=datan(s1)       
       !if(it.eq.jit) 
	if((j.ge.40).and.(j.lt.44))
     $	 write(1,1403) s1,ugk(j2),sdv
 1403  format (/2x,'внутри окружн при сигме s1='f12.6,' ugk=',f12.8,
     #'sdv=',f12.6)
	
      if((dsin(s)-s*dcos(s)).gt.0.0) nap=nap+3.1415926
	 !это направление без учёта начального угла
    1	nap=nap+phi(2,jvar)
      s1=dsin(nap-ugk(j2))/dsin(beta(j2)-ugk(j2))*sdv
      dshx(j2,j)=dshx(j2,j)+s1
	 !if(it.eq.jit)       
	if((j.ge.40).and.(j.lt.44))
     $ write(1,404) j2,j,s1,nap,sdv,beta(j2),ugk(j2),s,jvar  
c       знак проверить зависит ли от знака кривизны?
       ! похоже всё верно  beta-ugkпримерно 90
  404  format(/2x,'внутри окружности  при сигме j2=',i3,' j=',i3,
     #' s1=',f12.6,' nap=',f9.6,' sdv=',f15.8,' beta=',f9.6,
     #/' ugk=',f9.6,' s=',f9.6,' jvar=',i3) 
c j- номер переменной по которой производная, но номер элемента jvar
  253 continue
  252  continue
c на 252  когда нет точек на кривой теперь влияние круговой на правую часть      
       inach=i2+1
       if(inach.gt.npoint) go to 254
      s=sig(jvar)*la(jvar)
c      if(s.lt.0.0) s=-s
      s1=2.0-2.0*dcos(s)-2.0*s*dsin(s)+s*s
      if(s1.lt.0.0)then
	write(1,1801)s1
 1801	format(/2x,' pointg s1<0 s1=',f16.8)
      close(1)
	 stop 5
	endif
      sdv=sqrt(s1)
      sdv=sdv/sig(jvar)/sig(jvar)
      s1=(1-dcos(s)-s*dsin(s))/(dsin(s)-s*dcos(s))
	if(dabs(dsin(s)-s*dcos(s)).lt.0.00000001) then
	nap=3.1415926/2.0
	if((1-dcos(s)-s*dsin(s)).gt.0.0) nap=nap+3.1415926
	GO TO 2
	endif
      nap=datan(s1)       
	     !if(it.eq.jit)
	if((j.ge.40).and.(j.lt.44))
     $		  write(1,4403) s1,nap
 4403  format (/2x,'сдвиг от окруж при сигме  s1=',f12.6,' nap=',f12.8)
	
      if((dsin(s)-s*dcos(s)).gt.0.0) nap=nap+3.1415926
    2	nap=nap+phi(2,jvar) ! Нужно учесть начальный угол пси
c сомнительны nap,sdv. dR<0, dx=dR*(sin(s)-scos(s))
c       call sdvig(sdv,nap)

       do  i=inach,npoint
      s1=dsin(nap-ugk(i))/dsin(beta(i)-ugk(i))*sdv
       dshx(i,j)=dshx(i,j)+s1
	 !if(it.eq.jit)       
	if((j.ge.40).and.(j.lt.44))
     $  write(1,405) i,j,s1,nap,sdv,s,ugk(i),beta(i)
  405  format(/2x,'sdv от окружн при сигме i=',i3,' j=',i3,
     #' s1=',f12.5,' nap=',f9.6,/' sdv=', f12.3,' s=',f9.5,
     #' ugk=',f9.6,' beta=',f9.6) 
        enddo
      !xa=ksi(2,jvar)
      !ya=eta(2,jvar) не верно это конец клотоиды а надо конец окружности
	 xa=ksi(1,jvar+1)
	ya=eta(1,jvar+1)
      pov=la(jvar)
c      call rot(xa,ya,pov)
       do  i=inach,npoint
       x=xev(i)+shift(i)*dcos(beta(i))
       y=yev(i)+shift(i)*dsin(beta(i))
       s1=((x-xa)*dcos(ugk(i))+(y-ya)*dsin(ugk(i)))/
     # dsin(beta(i)-ugk(i))*pov
       dshx(i,j)=dshx(i,j)+s1
	 !if(it.eq.jit)       
	if((j.ge.40).and.(j.lt.44))
     $  write(1,406) i,j,s1,pov,ugk(i),beta(i),x,xa,y,ya
  406  format(/2x,'rot от окружн при сигмеi=',i3,' j=',i3,/
     #' s1=',f12.3,' pov=',f7.2,' ugk=',f9.6,' beta=',f9.6,/
     #' x=',f9.2,' xa=',f9.2,' y=',f9.4,' ya=',f9.4) 
       enddo
  254 continue
   
c- теперь правая клотоида, если она есть
      if(l(jvar+1).lt.0.002) go to 249
c на следуюший элемент, начнем внутри правой разбираться
      i1=jfirst(1,jvar+1)      
      i2=jfirst(2,jvar+1)-1
c      if(jvar.eq.NCURV) i2=npoint   !!! ПОЧЕМУ
c-это номера поперечников на концах клотоиды разобраться когда их нет????      
      if(i2.lt.i1) go to 2221

      do 2231 j2=i1,i2
      !lp=lpclo(j2)
c      s1=phi(1,jvar+1)+lp*(s+k(jvar+1)*lp/2.0)
c-это угол касательной в точке пересечения он же ugk(j)
c      s1=-lp*lp/(6.0*sin(beta(j2)-ugk(j2)))
       !s1=lp*lp/2.0*(1-lp/(3.0*l(jvar+1))/sin(beta(j2)-ugk(j2)))
       a=phi(1,jvar+1) ! угол касательной на  правой клотоидe
       b=sig(jvar) ! кривизна в начале клотоиды
	 c=k(jvar+1)
	 d=lpclo(j2)
	d1=l(jvar+1)
       call dsdvpodsig(a,b,c,d,d1,sdvig,naprav)
	!!call dsdvpodk(a,b,c,d,sdvig,naprav)
	!!sdvig=sdvig/l(jvar+1)
	!!замена на две строки
	nap=naprav
	  call znak(nap,ugk(j2),zn)
	!if((k(jvar+1)*b).gt.0).and.(zn.eq.1)) nap=nap+3.1415926
	!if((k(jvar+1)*b).lt.0).and.(zn.eq.0)) nap=nap+3.1415926
	if(zn.eq.0) nap=nap+3.1415926
	s1=sdvig*sin(nap-ugk(j2))/sin(beta(j2)-ugk(j2))

      dshx(j2,j)=dshx(j2,j)+s1
	! if(it.eq.jit)       
	if((j.ge.40).and.(j.lt.44))
     $write(1,407)j2,j,d1,s1,d,sdvig,nap,ugk(j2),
     *beta(j2),naprav,sig(jvar)
  407  format(/2x,'внутри правой при сигме j2=',i3,' j=',i3,' d1=',f9.5
     #/' s1=',f12.5,' lp=', f10.4,' sdvig=',f12.3,' nap=',f9.6,
     #/' ugk=',f9.6,' beta=',f9.6,' naprav=',f9.6,' sig=',f12.8) 
c знак ,,,???
 2231 continue
c- закончили внутри правой клотоиды теперь ее влияние на правую часть
 2221 continue      
c=========
c       if(jvar.eq.NCURV) go to 249
       inach=jfirst(2,jvar+1)
      ! nap=phi(2,jvar+1)+3.1415926/2.0
       !sdv=l(jvar+1)*l(jvar+1)/3.0
c- в левой клотоиде было /6 ???
c       call sdvig(sdv,nap)
       a=phi(1,jvar+1) ! угол касательной  в начале  правой клотоиды
       b=sig(jvar) ! кривизна в начале клотоиды
	 c=k(jvar+1)
	 d=l(jvar+1)
      if((j.ge.40).and.(j.lt.44))
     $write(1,4407)jvar,a,b,c,d
 4407 format(\2x,'do dsdvpodsig jvar=',i4,' a=',f9.6,' b=',f9.6,
     *' c=',f15.12,' d=',f8.2)
       call dsdvpodsig(a,b,c,d,d,sdvig,naprav)
	!call dsdvpodk(a,b,c,d,sdvig,naprav)
	!sdvig=sdvig/l(jvar+1)
	!!замена на две строки
	nap=naprav
	s=phi(2,jvar+1)
	call znak(nap,s,zn)
	!if((k(jvar+1)*sig(jvar+1).gt.0).and.(zn.eq.1)) nap=nap+3.1415926
	!if((k(jvar+1)*sig(jvar+1).lt.0).and.(zn.eq.0)) nap=nap+3.1415926
	if(zn.eq.0) nap=nap+3.1415926
	sdv=sdvig
       do  i=inach,npoint
       s1=dsin(nap-ugk(i))/dsin(beta(i)-ugk(i))*sdv
       dshx(i,j)=dshx(i,j)+s1
	 !if(it.eq.jit)       
	if((j.ge.40).and.(j.lt.44))
     $  write(1,408) i,j,s1,sdv,nap,ugk(i),beta(i),naprav,zn
  408  format(/2x,'sdv от правой при сигме i=',i3,' j=',i3,' s1=',f12.5,
     #/' sdv=',f12.5,' nap=',f9.6,' ugk=',f9.6,' beta=',f9.6,
     #' naprav=',f9.6,' zn=',i4) 
        enddo
       xa=ksi(2,jvar+1)
       ya=eta(2,jvar+1)
       pov=l(jvar+1)/2.0
c       call rot(xa,ya,pov)
       do  i=inach,npoint
       x=xev(i)+shift(i)*dcos(beta(i))
       y=yev(i)+shift(i)*dsin(beta(i))
       s1=((x-xa)*dcos(ugk(i))+(y-ya)*dsin(ugk(i)))/
     # dsin(beta(i)-ugk(i))*pov
       dshx(i,j)=dshx(i,j)+s1
	 !if(it.eq.jit)       
	if((j.ge.40).and.(j.lt.44))
     $  write(1,409) i,j,s1,pov,ugk(i),beta(i),x,xa,y,ya
  409  format(/2x,'rot от правой при сигме i=',i3,' j=',i3,' s1=',f12.5,
     #/' pov=',f12.5,' ugk=',f9.6,'beta=',f9.6,
     #/' x=',f9.4,' xa=',f9.4,'y= ',f9.4,' ya=',f9.4) 
       enddo
       go to 249
c - закончили с правой      
C-----------------------ПPOИЗBOДHAЯ ПO  LA
  230 CONTINUE
       if(la(jvar).lt.0.0001) go to 249
       inach=jfirst(1,jvar+1)
       nap=phi(1,jvar+1)
       sdv=1.0
c       call sdvig(sdv,nap)
       do  i=inach,npoint
       s1=dsin(nap-ugk(i))/dsin(beta(i)-ugk(i))*sdv
       dshx(i,j)=dshx(i,j)+s1
       if(it.eq.jit) write(1,410) i,j,s1,ugk(i),nap,beta(i)
  410  format(2x,'sdv от самой окружности i=',i3,' j=',i3,
     #/' s1=',f12.5,' ugk(i)=',f7.3,' nap=',f9.6,' beta=',f9.6) 
        enddo
       if(dabs(sig(jvar)).lt.1.E-9) go to 249
       xa=ksi(1,jvar+1)
       ya=eta(1,jvar+1)
       pov=sig(jvar)
c       call rot(xa,ya,pov)
       do  i=inach,npoint
       x=xev(i)+shift(i)*dcos(beta(i))
       y=yev(i)+shift(i)*dsin(beta(i))
       s1=((x-xa)*dcos(ugk(i))+(y-ya)*dsin(ugk(i)))/
     # dsin(beta(i)-ugk(i))*pov
       dshx(i,j)=dshx(i,j)+s1
	 if(it.eq.jit)       
     # write(1,411) i,j,s1,shift(i),pov,ugk(i),beta(i),x,xa,y,ya
  411  format(2x,'rot от самой окружности i=',i3,' j=',i3,
     #' s1=',f12.5,' shift(i)=',f7.3,/' pov=',f9.6,' ugk=',f9.6,
     #' beta=',f9.6,' x=',f9.4,' xa=',f9.4,' y=',f9.4,' ya=' f9.4) 
       enddo
       go to 249
c- закончили с  окружностью
  240  CONTINUE
C-----------------------ПPOИЗBOДHAЯ ПO  L
       !if(it.eq.jit) 
	if((j.ge.40).and.(j.lt.44))
     $	 write(1,7674) sig(jvar),k(jvar),l(jvar),la(jvar)
 7674	 format(/2x,' sig=',f9.7,' k=',f9.7,' l=',f9.6,' la=',f10.6)

      if(l(jvar).lt.0.002) go to 249
c-- сначала внутри клотоиды
      i1=jfirst(1,jvar)      
      i2=jfirst(2,jvar)-1
c-это номера поперечников на концах клотоиды разобраться когда их нет????      
      if(i2.lt.i1) go to 2222
c- это кривизна в начале клотоиды она больше не нужна дальше
      a=phi(1,jvar) ! угол касательной в начале  клотоиды
	b=0.0
	 if(jvar.gt.1) b=sig(jvar-1) ! кривизна в начале клотоиды
	 c=k(jvar)
	 !if(it.eq.jit)
	if((j.ge.40).and.(j.lt.44))
     $	   write(1,1415)a,b,c
 1415  format(/2x,'a=phi=', f9.6,' b=sig=',f10.7,' c=k=',f9.6)
        do 2232 j2=i1,i2
      !lp=lpclo(j2)
c      s1=phi(1,jvar)+lp*(s+k(jvar)*lp/2.0)
c-это угол касательной в точке пересечени он же ugk(j2)
c     s1=-k(jvar)*lp*lp/(6.0*sin(beta(j2)-ugk(j2)))
      !s1=-k(jvar)*lp*lp/l(jvar)*lp/(6.0*sin(beta(j2)-ugk(j2)))
       d=lpclo(j2)
       call dsdvpodk(a,b,c,d,sdvig,naprav)
	 
	 !if(k(jvar).gt.0) naprav = naprav+3.1415926
       nap=naprav
         call znak(nap,ugk(j2),zn)
	if((k(jvar).gt.0).and.(zn.eq.1)) nap=nap+3.1415926
	if((k(jvar).lt.0).and.(zn.eq.0)) nap=nap+3.1415926
	 ! zn=0 nap идёт против нормали надо при sig>0 идти против при sig<0 по нормали
	 !s1=sdvig*k(jvar)/l(jvar)*sin(nap-ugk(j2))/sin(beta(j2)-ugk(j2))
	s1=sdvig*k(jvar)/l(jvar)
	
	 if (s1.lt.0)s1=-s1 ! знак не работает есть направление
	! производная k по l равна -k/l старые dx и dy умножаются на минус k
      s1=s1*dsin(nap-ugk(j2))/dsin(beta(j2)-ugk(j2))
      dshx(j2,j)=dshx(j2,j)+s1
	 !if(it.eq.jit)       
	if((j.ge.40).and.(j.lt.44))
     $ write(1,415)j2,j,s1,d,sdvig,nap,jvar,
     #l(jvar),k(jvar),ugk(j2),beta(j2)
  415  format(/2x,' внутри клотоиды при ее длинеj2=',i3,' j=',i3,
     #' s1=',f12.5,' lp=', f9.4,/' sdv=',f12.5,' nap=',f9.6,' jvar=',i3,
     #/' l=',f7.2,' k=',f12.9,' ugk=',f9.6,' beta=',f9.6) 
 2232 continue
c- закончили внутри  клотоиды теперь ее влияние на правую часть
 2222 continue

      inach=jfirst(2,jvar)
      nap=phi(2,jvar) 
      !sdv=1.
c      call sdvig(sdv,nap)
       do  i=inach,npoint
       s1=dsin(nap-ugk(i))/dsin(beta(i)-ugk(i))
       dshx(i,j)=dshx(i,j)+s1
	 !if(it.eq.jit)       
	if((j.ge.40).and.(j.lt.44))
     $  write(1,412) i,j,s1,nap,ugk(i),beta(i)
  412  format(/2x,'sdv от клотоиды без к i=',i3,' j=',i3,' s1=',f12.5,
     #/' nap=',f9.6,' ugk=',f9.6,' beta=',f9.6) 
        enddo
c- это сдвиг по касательной за изменение длины без учетa изменения к
       xa=ksi(2,jvar)
       ya=eta(2,jvar)
       s=0.0
       if(jvar.ne.1) s=sig(jvar-1)
       pov=(s+sig(jvar))/2.0      
c       call rot(xa,ya,pov)
       do  i=inach,npoint
       x=xev(i)+shift(i)*dcos(beta(i))
       y=yev(i)+shift(i)*dsin(beta(i))
       s1=((x-xa)*dcos(ugk(i))+(y-ya)*dsin(ugk(i)))/
     # dsin(beta(i)-ugk(i))*pov
       dshx(i,j)=dshx(i,j)+s1
	 !if(it.eq.jit)       
	if((j.ge.40).and.(j.lt.44))
     $  write(1,413) i,j,s1,pov,shift(i),ugk(i),beta(i),x,xa,y,ya
  413  format(/2x,'rot от клотоиды суммар i=',i3,' j=',i3,' s1=',f12.5,
     #/' pov=',f9.6,' shift=',f7.3,' ugk=',f9.6,' beta=',f9.6,
     #/' x=',f8.2,' xa=',f8.2,' y=',f8.3,' ya=',f8.3) 
       enddo
c-  поворот за удлинение без учета изменения к =sig плюс к дает (s-sig)/2

       !nap=phi(2,jvar)+3.1415926/2.0
       !sdv=-k(jvar)*l(jvar)*l(jvar)/6.0

         a=phi(1,jvar) ! угол касательной  в начале клотоиды
	b=0.0
	 if(jvar.gt.1) b=sig(jvar-1)
	 c=k(jvar)
	 d=l(jvar)
       call dsdvpodk(a,b,c,d,sdvig,naprav)
	nap=naprav
	!if(k(jvar).gt.0.0) nap=nap + 3.1415926
	s=phi(2,jvar)
	call znak(nap,s,zn)
	if((k(jvar).gt.0).and.(zn.eq.1)) nap=nap+3.1415926
	if((k(jvar).lt.0).and.(zn.eq.0)) nap=nap+3.1415926
	 
	sdv=sdvig /l(jvar)*k(jvar)
	if(sdv.lt.0)sdv=-sdv ! знак не работает  есть направление
       !if(it.eq.jit)       
	if((j.ge.40).and.(j.lt.44))
     $  write(1,420)jvar,phi(2,jvar),l(jvar),nap,k(jvar),sdv,sdvig
  420 format(2x,'за k jvar=',i4,' phi(2,jvar)=',f12.7,' l=',f12.4,
     #/' nap=',f9.5,' k(jvar)=',f12.8,' sdv=',f18.1,' sdvig=',f18.1)
c       call sdvig(sdv,nap)
       do  i=inach,npoint
       s1=dsin(nap-ugk(i))/dsin(beta(i)-ugk(i))*sdv
       dshx(i,j)=dshx(i,j)+s1
	 !if(it.eq.jit)       
	if((j.ge.40).and.(j.lt.44))
     $  write(1,421) i,j,s1,nap,ugk(i),b
  421  format(2x,'sdv от клотоиды за к по нормали i=',i4,' j=',i3,
     #/' s1=',f12.8,' nap=',f9.5,
     #/' ugk(i)=',f9.5,' sig=',f12.9)
c       write(1,414) i,j,s1
  414  format(/2x,'sdv от клотоиды за к по нормали i=',i3,' j=',i3,
     #' dshx(i,j)=',f12.5) 
        enddo
c-это сдвиг по нормали за счет к
  249 CONTINUE
cгде это делать               DSHX(I,J) = DSHX(I,J)*WGTVAR(J1)
  250 continue 
         !   DO 239 J=1,NVAR
          !     J1 = TYPVAR(J)
           ! do 238 i=1,npoint
            !DSHX(I,J) = DSHX(I,J)*WGTVAR(J1)
  !238 continue 
  !239 continue 
             
      RETURN
      END
       
	subroutine znak(nap,ugkas,i22)
c возвращает 1 если нормаль и направление составляют острый угол
	 real*8 nap,ugkas,pi,nap1,ugk,s
	integer i22
	i22=0
	pi=3.1415926
	nap1=nap
	ugk=ugkas
       if(nap.lt.0) nap1=nap+pi
	if(ugk.lt.0) ugk=ugk+pi
       s=dabs(ugk+pi/2.0-nap)
	if(s.gt.pi) s=s-pi
	if(s.lt.pi/2.0) i22=1
	
	! write (1,1)i22
    1 format(/2x,' znak i22=',i5)
	return
	end

c       subroutine sdvig(sdv,nap)
c       real sdv,nap
c       do 200 i=inach,npoint
c       dshx(i,j)=dshx(i,j)+sin(nap-ugk(i))/sin(beta(i)-ugk(i))*sdv
c  200 continue
c       return
c       end

c       subroutine rot(xa,ya,pov)
c       INCLUDE 'COMMONS.INC'
c       real xa,ya,pov
c       real x,y
c       do 200 i=inach,npoint
c       x=xev(i)+shift(i)*cos(beta(i))
c       y=yev(i)+shift(i)*sin(beta(i))
c       dshx(i,j)=dshx(i,j)+((x-xa)*cos(ugk(i))+(y-ya)*sin(ugk(i)))/
c     # sin(beta(i)-ugk(i))*pov
c  200  continue
c       return
c       end

c=================
