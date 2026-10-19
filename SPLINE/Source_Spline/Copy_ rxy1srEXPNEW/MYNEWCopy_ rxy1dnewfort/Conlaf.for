 !Ошибка в crossokr при работе с дальней дугой ugk=угол касательной не верно
       !INCLUDE 'INTERFCS.INC'
       real*8 function cloxf(k,l)
       real*8 k,l
        real*8 eps,q,a,b,t_clox
!	 integer*4 j !нельзя 8 а 4 можно
        integer i
        eps = 0.000001
	   if ((l.lt.-eps).or.(l.gt.eps)) go to 1
         cloxf=0
         return
    1   continue
	    q = k*l*l/2
            q = -q*q
	    b = l
	    t_clox = l
             do 2 i=1,100 
	      b = b*q/(2*i)/(2*i-1)
	      a = b/(4*i+1)
	      t_clox = t_clox + a
	    if ((a.gt.eps).or.(a.lt.-eps)) go to 2
	    cloxf=t_clox
            return
    2 continue  
      write(1,*) "Зацикливание в СLOX"
	Write(1,10) k,l
   10 format(/2x,' k=',f12.9,' l=',f12.3)
      stop
      cloxf=t_clox
      return
      end

      real *8  function cloyf(k,l)
      real*8 k,l
      real*8 eps,a,b,q,t_cloy
      integer i
      eps = 0.000001
      if((l.lt.-eps).or. (l.gt.eps)) go to 1
         cloyf=0
         return
    1   continue
        q = k*l*l/2
        q = -q*q
	b = k*l*l*l/2
	t_cloy = b/3
             do 2 i=1,100 
	      b = b*q/(2*i+1)/(2*i)
	      a = b/(4*i+3)
	      t_cloy = t_cloy + a
              if ((a.gt.eps).or.(a.lt.-eps)) go to 2
              cloyf=t_cloy
	     return
    2  continue 
      write(1,*) "Зацикливание в СLOY"
      cloyf=t_cloy
      return
      end

      real *8 function cloxsf_1(x0,k,sig,phi,l)
       real*8 x0,k,sig,phi,l
      
      ! Входные данные этой функции : x0- абсцисса начала куска клотоиды, К- параметр
      ! клотоиды, sig-кривизна, phi-угол, l-длина куска клотоиды
      ! Возвращает абсциссу конца и печатает i- порядок производной -1
       !В clox и в cloxs- i имеет разный смысл.
       !Алгоритм: хранится последняя из вычисленных строка коэффициентов по степеням sig
       !но без синусов( косинусов) это массив prev в начале есть
      !prev[0]=1 так как первая производная равна cosp.
      !При дифференцировании множителя с sig (шаг влево) степень понижается
      !и поэтому новый элемент тот же, но умноженный на k и на показатель степени и делённый на sig, а при
      !дифференцировании sin или cos ( шаг вправо) умножаем на sig*prt. prt=1 если
      !в текущем столбце синус, если косинус то -1. Слагаемое в сумму для производной
      !вычисляется с учётом синусов и косинусов ( множитель sc) Шаг вправо меняет prt
      !а влево не меняет
       integer i; !i+1-порядок вычисляемой производной то есть степени l ???!!!
       integer prn,prt ! // признак равен 1 работа с синусом
       real*8 prev (100)!// массив слагаемых в строке
       integer j,j1,j2,j3 ! рабочие j2 для while
       real*8 st,s,s1,sc ! // рабочие: st- степень делённая на факториал,
                     !s- сумма в строке,sc- множитель синус или косинус
       real *8 x !// результат
       real*8 cosp,sinp,E
		
	if(dabs(sig).lt.0.000001) then
	
	 ! нулевая кривизна в начале и делить нельзя
	cloxsf_1=0.0
	write(1,77) sig
   77 format(/2x,'нулевая кривизна в начале  делить нельзя, sig=',f12.8)
      close (1)
	stop
	endif
       E=0.000001
       cosp=cos(phi)
	 sinp=sin(phi)
       x=x0
       prn=-1 !1-есть  синус в начальном элементе исходной строки  иначе -1 то есть строка начинается с косинуса
       !// prt наличие синуса в текущем элементе исходной строки если нет то косинус
       !//  prev=new double [1];
      st=l ! это степень l делённая на факториал
	x=x+cosp*st !это первый член в ряду то есть с первой производной
      !for(j1=0;j1<100;j1++)
	 do 2 j1=1,100
	 prev(j1)=0.0
    2 continue
       ! В фортране как м в Си строки нумерую с нуля и начинаю со второй i- номер последней вычисленной ==1
	! Но столбцы ( номера слагаемых в строке) вынужден нумеровать с 1 Теперь номер слагаемого это степень sig+1
      !prev[0]=1.0; !  при нулевой степени sig коэфф равен 1 (косинусы и синусы как множители учитываются отдельно)
	 prev(1)=1 ! это коэфф при нулевой степени sig в первой строке то есть для первой производной
	 i=1 ! i- как и в СИ  номер последней заполненной строки, то есть уже вычисленной производной
	 
      !//начинаем вычислять со второй производной но исходная строка первая
	!строка содержит коэффициенты при соответствующей степени  sig начиная с нулевой и кончая номер производной (i)-1
	 ! новую строку делаем из предыдущей ( шаг влево при дифф множителя с sig и вправо при дифф синуса или косинуса
      !while (1)
	 do 3 j2=1,100 ! вместо while (1)
       
      !// для вычисления и хранения новой строки слагаемых упорядоченных по степеням sig
      !j=!(i%2);
	 j=MOD(i+1,2) ! даёт ноль при нечётном i и 1 при чётном 
	 !теперь это степень sig  в первом не нулевом элементе строки его номер на 1 больше
	
	!//в Си было j- номер начального не нулевого элемента ( степени sig) в ранее заполненной (i-ой) строке
      ! // в нечётных заполненных строках это нулевой элемент, в четных >0 он равен 1, а строки нумерую с нуля)
      !// j показывает с какого столбца идём влево- вправо в исходной строке при нумерации столбцов с нуля
       ! В фортране номер в строке на 1 больше степени sig нумерация столбцов теперь с 1 то есть при нечётных i
	! начать с первого а при чётных со второго столбца но степень sig это по-прежнему j
       prt=prn !// синуса не было, был косинус поэтому prt равен минус 1
      st=st*l/(i+1) !// заготовка под степени по l на неё будем умножать сумму по строке с учётом синусов и косинусов
       s=0.0 !//сумма по строке в конце её умножим на степень l делённую на факториал
	! степени sig идут в каждой строке через один поэтому j1=j1+2
      !for (j1=j;j1<i; j1=j1+2) //j1-это степень sig!!! она же была номер столбца но в фортране минус 1
	 j3=i-1
	 DO 4 j1=j,j3,2

       if(j1.gt.0) then !// можно влево
       sc=cosp
      if(prt.eq.1) sc=sinp
	s1=prev(j1+1)*j1*k/sig	! это добавка при шаге влево
       prev(j1)=prev(j1)+s1 !// это без синусов и косинусов реализация добавки
       s=s+s1*sc
       ! в каждом столбце или синус или косинус
       endif
       !}// шаг влево j1- очередной столбец сделали один шаг влево 
       !теперь один шаг вправо
       !if (prt==1) sc=cosp; else sc=sinp;// меняем sc при шаге вправо так как дифф синус или косинус
	sc=sinp
	if(prt.eq.1)sc=cosp
      s1=prev(j1+1)*prt !// здесь учтён минус если дифференцировали косинус
	s1=s1*sig
      prev(j1+2)=prev(j1+2)+s1 ! при шаге вправо растёт степень sig и идёт умножение на sig
      s=s+s1*sc ! это сумма в строке ( производная ) которая будет умножена на степень l и поделена на факториал
      prt=-prt !;// меняем признак наличия синуса при переходе к новому столбцу *( через 1)
      prev(j1+1)=0.0 !;//сходили налево и направо а сам в новой строке стал нулём
    4 continue  
	  !}//for  по завершении цикла вычислена новая строка и производная то есть s
      s1= s*st !// st- ранее вычислена степень и факториал
      x=x+s1 ! s1- это новый член в ряду x- сумма ряда то есть текущая координата x
       
	 if((dabs(s1).lt.E).and.(i.gt.5)) go to 5
       !if(!j)prn=-prn; // наличие синуса в начале строки меняется если начинаем с нулевой степени sig иначе нет
       if(j.eq.0) prn=-prn
       if(i.gt.98)then
       write(1,*) "Зацикливание в СLOXSF_1"
	Write(1,10)x0, x,sig,k,phi,l,sinp,cosp
   10 format(/2x,' x0=',f11.3,' x=',f19.3' sig=',f9.6,' k=',f12.9,
     $' phi=',f19.6,' l=',f12.3,' sinp=',f19.5,' cosp=',f19.5)
      close(1)
      stop
      endif
       i=i+1 ! на новую строку то есть новый член ряда
       !end while
    3 continue
      !printf("\n IN CLOXS_1  i=%d",i);
      !getch();
    5 continue
      cloxsf_1=x
	 ! Write(1,10) x,sig,k,l
       return 
       end

        real*8 function cloysf_1 (y0,k,sig, phi,l)
       real*8 y0,k,sig, phi,l
       
       !Входные данные этой функции : y0- ордината начала куска клотоиды, К- параметр
       !клотоиды, sig-кривизна, phi-угол в начале клотоиды, l-длина куска клотоиды
       ! Возвращает ординату конца .
       !Алгоритм: хранится последняя из вычисленных строка коэффициентов по степеням sig
       !но без синусов( косинусов) это массив prev 
	! В фортране нумерация слагаемых в строке ( столбцов с 1), а строки остались с нуля и степени sig  с нуля
        !так как первая производная равна sinp.
       !При дифференцировании множителя с sig (шаг влево) степень понижается
       !и поэтому новый элемент тот же, но умноженный на k и на показатель степени
       !и делённый на sig, а при  дифференцировании sin или cos ( шаг вправо)
       !умножаем на sig*prt. prt=1 если  в текущем столбце синус, если косинус то -1.
       !Слагаемое в сумму для производной  вычисляется с учётом синусов и косинусов
       !( множитель sc) Шаг вправо меняет prt а влево не меняет
       
       integer i !i+1-порядок вычисляемой производной,  степени L???!!!
       integer  prn,prt ! признак prt равен 1 работа с синусом , минус 1 с косинусом в иекущем столбце
	! prn==1 строка начинается с синуса минус 1 с косинуса Слагаемых без sin или cos не бывает
       real*8 prev (100) !// массив слагаемых в  строке
        integer j,j1,j2 !// рабочие
       real*8 st,s,s1,sc !// рабочие: st- степень делённая на факториал,
                     !//s- сумма в строке,sc- множитель синус или косинус
       real*8 y !// результат
       real*8 cosp,sinp,E
       integer pr
	if(dabs(sig).lt.0.000001) then ! нулевая кривизна в начале и делить нельзя
       cloysf_1=0.0
	write(1,77) sig
   77 format(/2x,'нулевая кривизна в начале делить нельзя, sig=',f12.8)
      close (1)
	stop
	endif
	 E=0.000001
       cosp=cos(phi) 
	 sinp=sin(phi)
       y=y0
       prn=1;!//1 -есть  синус в начальном элементе исходной строки  иначе -1
       ! prt==1; наличие синуса в текущем элементе исходной строки если нет то косинус и минус  1
 
       st=l
	 y=y+sinp*st ! первый член ряда
       !for(j1=0;j1<60;j1++) prev[j1]=0.0;
       do 2 j1=1,100
	 prev(j1)=0.0
    2 continue
      prev(1)=1.0
       i=1 !// i- номер последней заполненной строки, то есть уже вычисленной производной
        !начинаем со второй производной но исходная строка первая
       do 3 j2=1,100 ! вместо while (1)
      ! prev для вычисления и хранения новой строки слагаемых упорядоченных по степеням sig
       !j=!(i%2);// j- номер начального не нулевого элемента ( степени sig) в заполненной (i-ой) строке
       !// в нечётных заполненных это нулевой элемент, а строки нумерую с нуля)
       !  с какого столбца идём влево- вправо в исходной строке
	j=MOD(i+1,2) ! это степень sig в первом ненулевом слагаемом в последней заполненной строке
       prt=prn  !// косинуса не было, был синус
       st=st*l/(i+1) !// заготовка под степени по l
       s=0.0 !//сумма по строке в конце её умножим на степень делённую на факториал
       !for (j1=j;j1<i; j1=j1+2) //j1-это степень sig!!!
	j3=i-1
	 DO 4 j1=j,j3,2
      
       if(j1.gt.0) then !// можно влево
	sc=cosp
       if(prt.eq.1)sc=sinp 
	 s1=prev(j1+1)*j1*k/sig
	 prev(j1)=prev(j1)+s1 !// это без синусов и косинусов
       s=s+s1*sc
	             
       endif !j1>0
       ! сделан шаг влево j1- очередной столбец теперь шаг вправо
       sc=sinp
      if (prt.eq.1) sc=cosp  ! меняем sc
      s1=prev(j1+1)*sig*prt  ! // здесь учтён минус если дифференцировали косинус
      prev(j1+2)=prev(j1+2)+s1
        s=s+s1*sc ! при шаге вправо и нулевой sig в итоге будет ноль
      prt=-prt !// меняем признак наличия синуса при переходе к новому столбцу *( через1)
       prev(j1+1)=0.0
    4 continue       
	s1= s*st
      y=y+s1

       if((dabs(s1).lt.E) .and. (i.gt.5))  go to 5

       if(j.eq.0)prn=-prn  ! наличие синуса в начале строки меняется если начало с нулевой степени sig 
       if(i.gt.98) then
	write(1,*) "Зацикливание в СLOYSF_1"
	 	Write(1,10)y0,y, sig,k,phi,l
   10 format(/2x,'y0=',f8.2,' y=',f11.3,' sig=',f9.6,' k=',f12.9,
     $ 'phi=',f9.6,' l=',f12.3)
      close(1)
      stop
	endif
        i=i+1 ! на новую строку для новой производной
    3 continue
      !//while
    5 continue
      !printf("\n IN CLOYS_1  i=%d",i);
      !getch();
	cloysf_1=y
       return 
       end
       
       subroutine conlaf(xl,yl,phil,sig,l,xr,yr,phir)
       real *8 xl,yl,phil,sig,l,xr,yr,phir
       real *8 cos_phi,sin_phi,dphi,dx,dy,dxn,dyn
	   cos_phi =dcos(phil)
	   sin_phi =dsin(phil)
      dphi= l*sig
c     /* вставить обработку плохой кривизны */
      dx  = dsin(dphi)/sig
      dy  = (1- dcos(dphi))/sig
	   dxn  = dx*cos_phi - dy*sin_phi
	   dyn  = dy*cos_phi + dx*sin_phi
      xr  = xl+dxn
      yr  = yl+dyn
      phir = phil + dphi
c     /*похоже что здесь кривизна со знаком и все верно*/
      return
      end

      subroutine conlf(ksi,eta,phi,sig,k,l,ksi1,eta1,phi1,sig1)
      double precision cloxf,cloyf
      real *8  ksi,eta,phi,sig,k,l,ksi1,eta1,phi1,sig1
      real *8 l0,phi0,x0,y0,x1,y1,l1,sin0,cos0
	real *8 eps,k1
       eps=0.000001

	if ( (k.lt.-eps).or. (k.gt.eps)) go to 1
	call conlaf(ksi,eta,phi,sig,l,ksi1,eta1,phi1)
        go to 7
    1 continue
	l0=sig/k
	phi0=phi-k*l0*l0/2
        k1=k
	x0=cloxf(k1,l0)
        y0=cloyf(k1,l0)
	sin0=dsin(phi0)
        cos0=dcos(phi0)
	l1=l0+l
	x1=cloxf(k,l1)
        y1=cloyf(k,l1)
	ksi1=ksi+((x1-x0)*cos0 - (y1-y0)*sin0)
	eta1=eta+((x1-x0)*sin0 + (y1-y0)*cos0)
    7   continue	
	sig1=sig+k*l
	phi1=phi+l*(sig+l*k/2)
	return
        end

      subroutine conlf_1(ksi,eta,phi,sig,k,l,ksi1,eta1,phi1,sig1)
      double precision cloxf,cloyf,cloxsf_1,cloysf_1
      real *8  ksi,eta,phi,sig,k,l,ksi1,eta1,phi1,sig1
      real *8 eps,x1,y1
       eps=0.000001
      sig1=sig+k*l
	phi1=phi+l*(sig+k*l/2)
	!if ( (k.lt.-eps).or. (k.gt.eps)) go to 1
	!call conlaf(ksi,eta,phi,sig,l,ksi1,eta1,phi1)
       ! go to 7
    1 continue
	
	if(dabs(sig).gt.0.000001) then
      ksi1=cloxsf_1(ksi,k,sig,phi,l)
	eta1=cloysf_1(eta,k,sig,phi,l)
	return
	endif
      x1=cloxf(k,l)
	y1=cloyf(k,l) ! это в повёрнутой и сдвинутой системе (новые)
	ksi1=ksi+x1*cos(phi)-y1*sin(phi)
	eta1=eta+x1*sin(phi)+y1*cos(phi) ! в старой системе 
    7 continue
	return
        end

      subroutine crossokr(ksi,eta,psi,xk,yk,r,u,v,alf,cross,ugkas,np)
	!исправил! вычисление ugk(j) при np=2 т.е. 
	 !при работе с дальней дугой det1 2 раза
      common/dobpech/ipc
	integer ipc
      real *8 ksi,eta,psi,xk,yk,r,u,v,alf,cross
      real *8 ugkas,det,det1,x3
      real *8 a,b,s,s1,x1,x2,x0,y0,xp,yp,xp1,yp1
	!np=0-не было пересечения, 1-по ближней, 2- по верхней
      x0=ksi-r*dsin(psi)
      y0=eta+r*dcos(psi)
      a=v-y0
      b=u-x0
cx0,y0-координаты центра, если r со знаком иначе неверно!!!!
       ! if(ipc==1)        @  write(1,10) r,u,v,x0,y0,alf
  10  format(/2x,'окр. r=',f15.9,' u=',f14.9,' v=',f14.9,
     #/' ksi= ',f14.9,' eta= ',f15.9,' psi=',f15.9' alf=',f15.9)
      s=-(a*dsin(alf)+b*dcos(alf))
c  это до корня
      s1=a*dcos(alf)-b*dsin(alf)
      s1=r*r-s1*s1
      if(s1.ge.0.) go to 7
	if(ipc.eq.0) OPEN(UNIT=7,FILE='ERROR.txt',STATUS='UNKNOWN')
	ipc=1
      write(7,*) "Минус под корнем в crossokr"
      write(7,10) r,u,v,ksi,eta,psi,alf
	write(1,*) "Минус под корнем в crossokr"
      write(1,10) r,u,v,ksi,eta,psi,alf
	
      x1=dcos(alf)
      x2=dsin(alf)
      cross=a*x1-b*x2
c x1,x2,cross для печати     
      write(1,77) x0,y0,a,b,x1,x2,s1
	write(7,77) x0,y0,a,b,x1,x2,s1
   77 format(2x,' x0=',f15.2,' y0=',f15.2,' a=',f15.3,' b=',f15.3,
     #/2x,' cos=',f8.5,' sin=',f8.5,' s1=',f15.3) 
	 write(7,*)'Счёт прекращён'
      write(1,*)'Счёт прекращён'
      close(1)
	close(7)
	stop
    7 continue
      s1=dsqrt(s1)
      x1=s+s1
      x2=s-s1

	!if(abs(u-512.5784).lt.0.01 .and. np.eq.-3) then
      !if(ipc.eq.1) write(1,117)x1,x2
c       a=u+x1*dcos(alf) 
c	 b=v+x1*dsin(alf)
c	s=u+x2*dcos(alf)
c	s1=v+x2*dsin(alf)
c	write(1,1117) a,b,s,s1
c	endif
  117  format(2x,'conla  x1=',f14.9,' x2=',f15.8) 
 1117  format(/2x,' x1per=',f12.4,' y1per=',f12.4,
     */' x2per=',f12.4,' y2per=',f12.4)
      if(dabs(x2).gt.dabs(x1)) go to 8
      cross=x2
	x3=x1
      go to 9
    8 continue
      x3=x2
      cross=x1
    9 continue
      ! здесь x3- расст до дальней, cross до ближней 
	 !np.eq.2 может быть только при непервом заходе
	if(np.eq.2) then
	cross=x3
      xp=u+cross*dcos(alf)
      yp=v+cross*dsin(alf)
	det=(xp-ksi)*(yk-yp)-(xk-xp)*(yp-eta) 
	if(det*r.gt.0.0) then
        det1=dsqrt((xp-ksi)*(xp-ksi)+(yp-eta)*(yp-eta))
! БЫЛА ОШИБКА не было в det1 в ugkas стоял det
	 ugkas=psi+2.0*dasin(det1/(2.0*r))
	!это всё по дальнему пересечению с дугой
	 ! если исчезло то кончилась дуга
	 return
	endif
	np=0
      cross=20000	
    	return
	endif
      ! здесь при np=0 или 1
      xp1=u+cross*dcos(alf)
      yp1=v+cross*dsin(alf)
	det=(xp1-ksi)*(yk-yp1)-(xk-xp1)*(yp1-eta) 
	det1=dsqrt((xp1-ksi)*(xp1-ksi)+(yp1-eta)*(yp1-eta))
	!det1 -расст от ближнего пересечения до начала дуги
	if((det*r.ge.0.0).or.(det1.lt.0.01)) then
	ugkas=psi+2.0*dasin(det1/(2.0*r))
	!это всё по ближнему пересечению здесь было верно!
	np=1
	return 
	endif !совпали знаки пересечение внутри своей дуги
	!если есть пересечение с ближним то ушли
	! здесь нет пересечения если было раньше то конец если не было  то выяснять где мы
	if(np.eq.1) then
	np=0 ! на всякий случай
      cross=20000	
    	return
	endif
	! здесь при первом заходе и  ближняя вне дуги det1- расст от начала до пересечения (хорда)
	cross=x3
	xp=u+cross*dcos(alf)
        yp=v+cross*dsin(alf)
	det=(xp-ksi)*(yk-yp)-(xk-xp)*(yp-eta) ! векторное произведение + против часовой
	if(det*r.gt.0.0) then 
        det1=dsqrt((xp-ksi)*(xp-ksi)+(yp-eta)*(yp-eta))
!       БЫЛА ОШИБКА не было  det1 и  в ugkas стоял det
      ugkas=psi+2.0*dasin(det1/(2.0*r))
	!это всё по дальнему пересечению с дугой
	 np=2
	return
	endif
      !здесь при np=0 и обе вне дуги (пересечения на дуге и знак угол поворота и r не совпадают
	! по знакам так будет если оба пересечения на дуге другого знака чем радиус
       det=dsqrt((xp1-xk)*(xp1-xk)+(yp1-yk)*(yp1-yk))
	 	!нет пересечения дуги и не было работаю с ближней и нахожу слева или справа
	 !det- растояние от ближнего пересечения от конца,det1-до начала дуги
	 if(det1.lt.det) then 
!ближе к началу; 
 !по векторному произведению узнали пересечение со своей дугой
! если вне своей дуги то его знак не совпадёт со знаком радиуса

	if(ipc.eq.0) OPEN(UNIT=7,FILE='ERROR.txt',STATUS='UNKNOWN')
	ipc=1
	write(1,*)'crossokr слева от окружности'
	write(1,10) ksi,eta,psi,r,u,v,alf
	write(1,113) det1,det,xp1,yp1,xk,yk,x0,y0,xp,yp,np
	write(7,*)'crossokr слева от окружности'
	write(7,10) ksi,eta,psi,r,u,v,alf
	write(7,113) det1,det,xp1,yp1,xk,yk,x0,y0,xp,yp,np
  113 format(/2x,'до лев det1=',f12.6,' до прав det=',f12.6,' xp1=',f9.2,
     #' yp1=',f9.2,' xk=',f9.2,' yk=',f9.2,' x0=',f12.6,' y0=',f12.6,
     #' xp=',f9.2,' yp=',f9.2,' np=',i3)
	 write(1,*)'Счёт прекращён'
	write(7,*)'Счёт прекращён'
	close(1)
	close(7)
	stop 
	endif
       cross=20000	
    	 return ! проскочили окружность
	 !if(dabs(alf-1.609939).lt.0.001) write(1,111)xp,yp
  111  format(/2x,' xp=',f14.7,' yp=',f14.7)
      end  

      subroutine crossclo1(ksi2,eta2,phi,sig,k,l,u2,v2,alf2,cross,lp,np)
	!np -признак печати при вызове надо дать минус 1
	common/dobpech/ipc
	integer ipc
      real *8 ksi2,eta2,phi,sig,k,l,u2,v2,alf2,cross,lp
      real*8 u,v,alf,ksi,eta,x2,y2     
      real *8 cloxf,cloyf,cloxsf_1,cloysf_1
      integer crossln
      integer i,np ! np=0 не было пересечения с этой клотоидой np<0 для печати
      real *8 l0,phi0,x0,y0,x1,y1,l1,lh,lk,u1,v1,s,x,y
c на выходе  lp- длина клотоиды от реального начала до пересечения
       
	 ksi=ksi2
       eta=eta2
       u=u2
       v=v2
       alf=alf2
c чтобы не портить
	 l1=l
	   l0=0 ! временно
      x0=ksi
	y0=eta
	if(dabs(sig).gt. 0.000001)then
! не с нулевой кривизы нужно считать точно при малом к
      x1=cloxsf_1(ksi,k,sig,phi,l)
	y1=cloysf_1(eta,k,sig,phi,l)
	                      else
      x2=cloxf(k,l)
	y2=cloyf(k,l) ! это в повёрнутой и сдвинутой системе (новые)
	x1=ksi+x2*cos(phi)-y2*sin(phi)
	y1=eta+x2*sin(phi)+y2*cos(phi) ! в старой системе 
	endif
	
c- это конец клотоиды   
      
  !   #   write(1,99)k,sig,phi,phi0
   99 format(2x,'k=',f12.9,' sig=',f9.6,' phi=',f9.5,' phi0=',f9.5)
       !if((u2.ge.2285.487).and.(u2.le.2285.489).and.(ksi2.gt.2260.72300)
      !#.and.(ksi2.lt.2260.72305))  
	 if(np.lt.0)
     #  write(1,100) l,ksi,eta,x1,y1,u,v,alf,np,k
  100  format(/2x,'l=',f7.2,' ksi=',f12.3,' eta=',f12.3,
     #/' x1=',f15.2,' y1=',f15.6,' u=',f9.3,' v=',f9.3,' alf=',f9.6,
     #/' np=',i3,' k=',f12.9)  
        
 
c-ksi,eta,x1,y1- концы хорды поиска,lh,lk-ее нач и кон длины от нуля sig
      
        i= crossln(ksi,eta,x1,y1,u,v,alf,np) ! нужно np для печати и прочего

   13   format(/2x,' i=crossln=',i4)
       if(np.lt.0)  then 
	 write(1,64)i, x0,y0,x1,y1,u,v,alf
	 endif
      !i= 0- параллельность хорды и поперечника, 1- внутри, 2- строго начало, 3-конец.
	! -2 до начала, -3 - за концом
	 if(i.eq.0) then
	if(ipc.eq.0) OPEN(UNIT=7,FILE='ERROR.txt',STATUS='UNKNOWN')
	ipc=1
	write(7,*)' crossclo поперечник и хорда клотоиды параллельны'
	write(7,64)i, x0,y0,x1,y1,u,v,alf
	write(7,*)'Счёт прекращён'
	 close(1)
       close(7)
	stop
	endif
        if(i.eq.1) go to 9
         if(i.eq.2) then
	! пересечение  в начале
      lh=l0
	lk=l0
	x=x0
	y=y0
      ksi=x0
      eta=y0
	go to 77 ! на оформление выхода проверить знак!!!
	endif
      if(i.eq.3) then
	! пересечение в конце
	lh=l1
	lk=l1
	ksi=x1
	eta=y1
	x=x1
	y=y1
	go to 77
	endif
      if(i.eq.-2) then ! до начала
	if(ipc.eq.0) OPEN(UNIT=7,FILE='ERROR.txt',STATUS='UNKNOWN')
	ipc=1
	write(1,*) ' crossclo1 до начала клотоиды '
 	write(1,100) l,x0,y0,x1,y1,u,v,alf,np,k
	write(7,*) ' crossclo1 до начала клотоиды '
	write(7,100) l,x0,y0,x1,y1,u,v,alf,np,k
	cross=-3000 ! ЕСЛИ ЗА КОНЦОМ  ТО -2000 ВЕРНЁМ ИНАЧЕ СДВИЖКА
	return
	!endif
	 !close(1)
      !close (7) 
     	!stop       
	endif
      if(i.eq.-3) then
	  cross=-2000.0 ! за  концом КЛОТОИДЫ
        return
	endif
      ! здесь первое пересечение внутри
       
   64  format(/2x,'i=',i2' x0=',f12.5,' y0=',f12.5,' x1=',f12.5,
     #' y1=',f12.6,' u=',f12.5,' v=',f12.5,' alf=',f12.8)
c- -2 значит за пределами или параллельность хорды и поперечника 0- есть       
    9 continue
        lh=l0
        lk=l1
   11 continue  
       !if((u2.ge.119.980).and.(u2.le.119.9891))
       !# write(1,14)lh,lk
   14 format(/2x,'lh=',f8.4,' lk=',f8.4) 
        
	  if(dabs(lk-lh).lt.0.00001) go to 77	!!dabs
        s=(lh+lk)/2
	if(dabs(sig).gt. 0.000001)then
      x=cloxsf_1(ksi2,k,sig,phi,s)
	y=cloysf_1(eta2,k,sig,phi,s)
              else
      x2=cloxf(k,s)
	y2=cloyf(k,s) ! это в повёрнутой и сдвинутой системе (новые)
	x=ksi2+x2*cos(phi)-y2*sin(phi)
	y=eta2+x2*sin(phi)+y2*cos(phi) ! в старой системе 
	endif
! ++++++++++++
       ! x=cloxf(k,s)
       ! y=cloyf(k,s)
       
	i=crossln(ksi,eta,x,y,u,v,alf,np)
	 if(np.lt.0) then
	write(1,127) i,ksi,eta,x,y,u,v,alf
  127 format(2x,'crossclo i=',i3,' ksi=',f10.4,' eta=',f10.4,
     #/ ' x=',f10.4,' y=',f10.4,' u=',f10.4,' v=',f10.4,' alf=',f10.6)
	endif
	 if (i.eq.1) go to 10 ! меняю lk
c=если не 1 то нет пересечения с половиной, которая начинается всегда с lh надо её менять
c делим пополам и снова с lh до х
      if(i.eq.2)then
      lk=lh
	x=ksi
	y=eta
	  go to 77 
	endif
      if(i.eq.3)then ! в конце
      lh=lk
	ksi=x
      eta=y
	  go to 77 
	endif
       ! слева от начала дальше не будет другой точки
	if(i.eq.-2)then
	write(1,*)' crossclo до начала хорды'
	lk=lh
	x=ksi
	y=eta
	  go to 77 
	
	endif
  101 continue ! меняю lh
        lh=s
        ksi=x
        eta=y
        go to 11
   10   continue ! эдесь пересечение с левой то есть от середины к X1 ( при X1<x0 она правая)
        lk=s
        go to 11
   77   continue               
        u1=(ksi+x)/2.0
        v1=(eta+y)/2.0     
        s=dsqrt((u1-u)*(u1-u)+(v1-v)*(v1-v))
	! здесь всё верно. направление поперечника всегда в сторону положительных рихтовок
	! так установлено при вычислении углов с осью ОХ ( влево по ходу сущ. трассы)
        if(dabs(u1-u).gt.dabs(v1-v)) go to 125
        if((v1-v)*dsin(alf).lt.0)s=-s
        go to 126
  125  continue
       if((u1-u)*dcos(alf).lt.0)s=-s
  126  continue
c        if((u2.ge.619).and.(u2.lt.621))
       !if((u2.ge.119.980).and.(u2.le.119.9891))
       !#   write(1,123)ksi,eta,x,y,u,u1,v,v1,s
	!if((u2.ge.2285.487).and.(u2.le.2285.489).and.(ksi2.gt.2260.72300)
      !#.and.(ksi2.lt.2260.72305))  write(1,123)ksi,eta,x,y,u,u1,v,v1,s
      if(np.lt.0)  write(1,123)ksi,eta,x,y,u,u1,v,v1,s
  123 format(2x,'vix ksi=',f12.6,' eta=',f12.6,' x=',f12.6,' y=',f12.6,
     #/2x,'u=',f12.6,' u1=',f12.6,' v=',f12.6,' v1=',f12.6,' s=',f12.6)        
        cross=s
        lp=(lh+lk)/2.0-l0
	np=np+1
        return
       end



      subroutine crossclo(ksi2,eta2,phi,sig,k,l,u2,v2,alf2,cross,lp,np)
	!np -признак печати при вызове надо дать минус 1
	common/dobpech/ipc
	integer ipc
      real *8 ksi2,eta2,phi,sig,k,l,u2,v2,alf2,cross,lp
      real*8 u,v,alf,ksi,eta     
      real *8 cloxf,cloyf,cloxfs_1,cloyfs_1
      integer crossln
      integer i,np ! np=0 не было пересечения с этой клотоидой np<0 для печати
      real *8 l0,phi0,x0,y0,x1,y1,l1,sin0,cos0,lh,lk,u1,v1,s,x,y
c на выходе  lp- длина клотоиды от реального начала до пересечения
       
	 ksi=ksi2
       eta=eta2
       u=u2
       v=v2
       alf=alf2
c чтобы не портить
	l0=sig/k
	phi0=phi-k*l0*l0/2
1001   format(2x,'l0=',f12.2,' sig=',f12.8,' k=',f12.9,
     #/' phi0=',f12.8,' phi=',f12.8)
	x0=cloxf(k,l0)
        y0=cloyf(k,l0)
	sin0=dsin(phi0)
        cos0=dcos(phi0)
c-это координаты начала клотоиды при кривизне sig  в системе начала клотоиды
c т.е при 0 sig, a phi0-угол новой оси со старой осью х,x0,y0-это ksi,eta
c в новой системе
  
	l1=l0+l
	x1=cloxf(k,l1)
        y1=cloyf(k,l1)
c- это конец клотоиды в новой системе  
    !   # and.(l.lt.115.40))  
  !   #   write(1,99)k,sig,phi,phi0
   99 format(2x,'k=',f12.9,' sig=',f9.6,' phi=',f9.5,' phi0=',f9.5)
       !if((u2.ge.2285.487).and.(u2.le.2285.489).and.(ksi2.gt.2260.72300)
      !#.and.(ksi2.lt.2260.72305))  
	 if(np.lt.0)
     #  write(1,100) l0,l1,x0,y0,x1,y1,u,v,alf,ksi,eta,u2,v2,np,k
  100  format(/2x,'l0=',f12.2,' l1=',f12.3,' x0=',f12.3,' y0=',f12.3,
     #/' x1=',f15.2,' y1=',f15.6,' u=',f9.3,' v=',f9.3,' alf=',f9.6,
     #/' ksi=',f9.3,' eta=',f9.3,' u2=',f9.3,' v2=',f9.3,' np=',i3,
     #/' k=',f12.9)  
        u1=u-ksi
        v1=v-eta
        u=x0+u1*cos0+v1*sin0
        v=y0-u1*sin0+v1*cos0
        alf=alf-phi0
    !  if((u2.ge.5670.980).and.(u2.le.5671.01).and.(l.gt.115.38).
    ! # and.(l.lt.115.40))  
    ! #   write(1,120) sin0,cos0,u1,v1
  120  format(2x,'sin0=',f9.6,' cos0=',f9.6,' u1=',f9.4,' v1=',f9.4) 
      !if((u2.ge.5670.980).and.(u2.le.5671.01).and.(l.gt.117.18).
      !# and.(l.lt.117.20)) 
	  !if((u2.ge.2285.487).and.(u2.le.2285.489).and.(ksi2.gt.2260.72300)
       !# .and.(ksi2.lt.2260.72305)) 
       if(np.lt.0) then 
      write(1,21) u,v,alf
      endif
   21  format(/2x,'новые u=',f12.3,' v=',f12.3,' alf=',f9.6)
c- это начало поперечника и его угол в новой системе  в ней работаем
c- координаты начала клотоиды при кривизне sig в новой системе x0,y0
c-движения обратно нет при угле поворота клотоиды меньше 90 то гр. x1>x0
c и при отрицательном x0 тоже НЕВЕРНО при близких радиусах ПОВОРОТ > 90 гр и x0<0 x1<x0
c- это начало поперечника и его угол в новой системе  в ней работаем
c- координаты начала клотоиды при кривизне sig в новой системе x0,y0
c-движения обратно нет при угле поворота клотоиды меньше 90 то гр. x1>x0
c и при отрицательном x0 тоже НЕВЕРНО при близких радиусах ПОВОРОТ > 90 гр и x0<0 x1<x0

	 
c-ksi,eta,x1,y1- концы хорды поиска,lh,lk-ее нач и кон длины от нуля sig
c не хочy портить x0,y0
        ksi=x0
        eta=y0
      
        i= crossln(x0,y0,x1,y1,u,v,alf,np) ! нужно np для печати и прочего

   13   format(/2x,' i=crossln=',i4)
       if(np.lt.0)  then 
	 write(1,64)i, x0,y0,x1,y1,u,v,alf
	 endif
      !i= 0- параллельность хорды и поперечника, 1- внутри, 2- строго начало, 3-конец.
	! -2 до начала, -3 - за концом
	 if(i.eq.0) then
	if(ipc.eq.0) OPEN(UNIT=7,FILE='ERROR.txt',STATUS='UNKNOWN')
	ipc=1
	write(7,*)' crossclo поперечник и хорда клотоиды параллельны'
	write(7,64)i, x0,y0,x1,y1,u,v,alf
	write(7,*)'Счёт прекращён'
	 close(1)
       close(7)
	stop
	endif
        if(i.eq.1) go to 9
         if(i.eq.2) then
	! пересечение  в начале
      lh=l0
	lk=l0
	x=x0
	y=y0
      ksi=x0
      eta=y0
	go to 77 ! на оформление выхода проверить знак!!!
	endif
      if(i.eq.3) then
	! пересечение в конце
	lh=l1
	lk=l1
	ksi=x1
	eta=y1
	x=x1
	y=y1
	go to 77
	endif
      if(i.eq.-2) then ! до начала
      write(1,*)'левее клотоиды'
	write(1,100) l0,l1,x0,y0,x1,y1,u,v,alf,ksi,eta,u2,v2,np,k
	if(np.ne.0) then
	cross=-2000
	return
	endif
	close(1)
       
     	stop       
	endif
      if(i.eq.-3) then
	  cross=-2000.0 ! за правым концом
        return
	endif
      ! здесь первое пересечение внутри
       
   64  format(/2x,'i=',i2' x0=',f12.5,' y0=',f12.5,' x1=',f12.5,
     #' y1=',f12.6,' u=',f12.5,' v=',f12.5,' alf=',f12.8)
c- -2 значит за пределами или параллельность хорды и поперечника 0- есть       
      
    9 continue
        lh=l0
        lk=l1
   11 continue  
       !if((u2.ge.119.980).and.(u2.le.119.9891))
       !# write(1,14)lh,lk
   14 format(/2x,'lh=',f8.4,' lk=',f8.4) 
        if(dabs(lk-lh).lt.0.00001) go to 77	!!dabs
        s=(lh+lk)/2
        x=cloxf(k,s)
        y=cloyf(k,s)
       
	i=crossln(ksi,eta,x,y,u,v,alf,np)
	 if(np.lt.0) then
	write(1,127) i,ksi,eta,x,y,u,v,alf
  127 format(2x,'crossclo i=',i3,' ksi=',f10.4,' eta=',f10.4,
     #/ ' x=',f10.4,' y=',f10.4,' u=',f10.4,' v=',f10.4,' alf=',f10.6)
	endif
	 if (i.eq.1) go to 10 ! меняю lk
c=если не 1 то нет пересечения с половиной, которая начинается всегда с lh надо её менять
c делим пополам и снова с lh до х
      if(i.eq.2)then
      lk=lh
	x=ksi
	y=eta
	  go to 77 
	endif
      if(i.eq.3)then ! в конце
      lh=lk
	ksi=x
      eta=y
	  go to 77 
	endif
       ! слева от начала дальше не будет другой точки
	if(i.eq.-2)then
	write(1,*)' crossclo до начала хорды'
	lk=lh
	x=ksi
	y=eta
	  go to 77 
	
	endif
  101 continue ! меняю lh
        lh=s
        ksi=x
        eta=y
        go to 11
   10   continue ! эдесь пересечение с левой то есть от середины к X1 ( при X1<x0 она правая)
        lk=s
        go to 11
   77   continue               
        u1=(ksi+x)/2.0
        v1=(eta+y)/2.0     
        s=dsqrt((u1-u)*(u1-u)+(v1-v)*(v1-v))
	! здесь всё верно. направление поперечника всегда в сторону положительных рихтовок
	! так установлено при вычислении углов с осью ОХ ( влево по ходу сущ. трассы)
        if(dabs(u1-u).gt.dabs(v1-v)) go to 125
        if((v1-v)*dsin(alf).lt.0)s=-s
        go to 126
  125  continue
       if((u1-u)*dcos(alf).lt.0)s=-s
  126  continue
c        if((u2.ge.619).and.(u2.lt.621))
       !if((u2.ge.119.980).and.(u2.le.119.9891))
       !#   write(1,123)ksi,eta,x,y,u,u1,v,v1,s
	!if((u2.ge.2285.487).and.(u2.le.2285.489).and.(ksi2.gt.2260.72300)
      !#.and.(ksi2.lt.2260.72305))  write(1,123)ksi,eta,x,y,u,u1,v,v1,s
      if(np.lt.0)  write(1,123)ksi,eta,x,y,u,u1,v,v1,s
  123 format(2x,'vix ksi=',f12.6,' eta=',f12.6,' x=',f12.6,' y=',f12.6,
     #/2x,'u=',f12.6,' u1=',f12.6,' v=',f12.6,' v1=',f12.6,' s=',f12.6)        
        cross=s
        lp=(lh+lk)/2.0-l0
	np=np+1
        return
       end


       integer function crossln(ksi,eta,x1,y1,u,v,alf,np)
	!ksi,eta,x1,y1- начало и конец куска клотоиды,u,v,alf- начало и угол
	!np-здесь только для печати при <0
c- хорда задана концами, а поперечник точкой и углом u,v,alf
c- даёт 1 при  пересечении двух прямых внутри, 2- при пересечении в начале 
c- +3- при пересечении в конце (x1,y1), -2 до начала, -3 за концом, 0- параллельность
       real *8 ksi,eta,x1,y1,u,v,alf
       real *8 s,u1,v1,eps
	integer np
	 eps=0.00001! был 0.000001
       if(dabs(dabs(alf)-3.1415926/2.0).gt. 0.000001) go to 1 
      ! здесь вертикальный поперечник
      if(dabs(ksi-x1).lt.0.0000001) then 
	! оба вертикальны
	crossln=0 ! параллельность криминал!
	return
      endif
      ! поперечник вертикален а хорда куска клотоиды - нет
      if(x1.lt.ksi) go to 2 ! там ход влево
      if(u.lt.ksi-eps) then
	crossln=-2 ! до начала
      return
	endif
      if(u.lt.ksi+eps) then
	crossln=2 ! в начальной точке
      return
	endif
	if(u.gt.x1+eps) then
	crossln=-3 ! за концом
      return
	endif
      if(u.gt.x1-eps) then
	crossln=3 ! в конце
      return
	endif
      crossln=1 ! внутри
	return
    2 continue
    	
	if(u.lt.x1-eps) then ! здесь ход влево конец х1 н1
	crossln=-3 ! за концом
	return
	endif
      if(u.lt.x1+eps) then
	crossln=3 ! в конце
      return
	endif
	if(u.gt.ksi+eps) then
	crossln=-2 !до начала
      return
	endif
      if(u.gt.ksi-eps) then
	crossln=2 ! в начальной точке
      return
	endif
      crossln=1 ! внутри
	return
     
    1  continue
      ! здесь при невертикальном поперечнике с вертикальным закончили
      
	 if(dabs(ksi-x1).gt.0.0000001) go to 4 ! на невертикальность
	! здесь вертикальная хорда и невертикальный поперечник
	! вычислить v1- ординату поперечника при пересечении хорды или её продолжения
       v1=v+dtan(alf)*(x1-u)
       if(y1.lt.eta) go to 3
	! здесь вертикальная хорда идёт вверх
	 if(v1.lt. eta-eps) then
	crossln=-2 ! до начала хорды
	return
	endif
      if(v1.lt. eta+eps) then
	crossln=2 ! в начальной точке
	return
	endif
	if(v1.gt. y1+eps) then
	crossln=-3 ! за концом хорды
	return
	endif
      if(v1.gt. y1-eps) then
	crossln=3 ! в конечной точке
	return
	endif
       crossln=1 ! внутри
	return
    3 continue
       
      ! здесь вертикальная хорда идёт вниз её начало всегда ksi, eta
       if(v1.lt. y1-eps) then
	crossln=-3 ! за концом хорды
	return
	endif
      if(v1.lt. y1+eps) then
	crossln=3 ! в конечной точке ( нижней!)
	return
	endif
	if(v1.gt. eta+eps) then
	crossln=-2 ! до начала хорды
	return
	endif
      if(v1.gt. eta-eps) then
	crossln=2 ! в начальной точке ( верхней)
	return
	endif
       crossln=1 ! внутри
	return
    4 continue
      ! здесь всё не вертикально
		 
	  s=dtan(alf)-(y1-eta)/(x1-ksi)
        if(dabs(s).gt.0.000001) go to 7
        crossln=0 ! параллельность  
        return
       
    7  continue
        u1=(eta-v-(y1-eta)/(x1-ksi)*ksi+dtan(alf)*u)/s
	 ! v1=eta+(y1-eta)/(x1-ksi)*(u1-ksi) ! точка пересечения
      !if(abs(u-2080.56).lt.1)	 write(1,100) ksi,u1,x1,v1,y1
  100 format(2x,'crln нач ksi=',f13.6,' пер u1=',f13.6,' кон x1=',f13.6,
     *' v1=',f13.6,' y1=',f13.6)
       
       ! здесь нет вертикали и параллели
       if(x1.lt.ksi) go to 201 ! там справа на лево
	 if(u1.gt.x1+eps) then
	  crossln=-3 ! за концом хорды
	return
	endif
	if(u1.gt.x1-eps) then
	  crossln=3 ! в конечной точке
	return
	endif

	 if(u1.lt.ksi-eps) then
	s=dtan (alf)
	v1=(y1-eta)/(x1-ksi)
	 write(1,122)u1,ksi,eta
  122 format(/2x,' newcrossln u1=',f15.9,' ksi=',f15.9,' eta=',f9.3)
       write (1,124)x1,y1,u,v
  124 format(/2x,' x1=',f9.3,' y1=',f9.3,' u=',f9.3,' v=',f9.3)
	 write(1,123)alf,s,v1
  123 format(/2x,' alf=',f9.6,' tanalf=',f9.4,' tgxord=',f9.4)
	 crossln=-2 !  до начала хорды
	 return
	endif
      if(u1.lt.ksi+eps) then
	 crossln=2 ! в начале хорды
	 return
	endif

	crossln=1 ! внутри 
	return
  201 continue
       ! здесь справа налево x1<ksi, но начало всегда ksi,eta
       if(u1.lt.x1-eps) then
	  crossln=-3 ! за концом хорды
	return
	endif
	if(u1.lt.x1+eps) then
	  crossln=3 ! в конечной точке
	return
	endif

	 if(u1.gt.ksi+eps) then
	 crossln=-2 !  до начала хорды
	 return
	endif
      if(u1.gt.ksi-eps) then
	 crossln=2 ! в начале хорды
	 return
	endif

	crossln=1 ! внутри 
	
        return
        end

	subroutine dsdvpodk(phi,sig,k,l,sdv,naprav)
	real*8 phi,sig,k,l,sdv,dx,dy ! входные + dx, dy - частные произ по k,naprav
	real*8 l0,l1,x0,x1,y0,y1,phi0,phi1,phi2,a,b,c,d,naprav
       
       l0=sig/k
       phi0=phi-k*l0*l0/2 ! разворот систем координат
	 x0=cloxf(k,l0)
       y0=cloyf(k,l0)
	l1=l0+l
	x1=cloxf(k,l1) 
	y1=cloyf(k,l1)
      ! x0,y0 начало то есть x,y но в системе клотоиды
       ! x1,y1 координаты в системе клотоиды конца т. е. через l от начала

       !printf("\n x0=%8.3lf,y0=%8.3lf,x1=%8.3lf,y1=%8.3lf",x,y,x1,y1);
       !printf("\n l0=%8.3lf,phi0=%12.7lf,k=%12.7lf",l0,phi0,k);
       phi1=k*(l0+l)*(l0+l)/2.0
       phi2=k*l0*l0/2.0
       a=(x1-x0-(l-l0)*dcos(phi1)-l0*dcos(phi2))/(2.0*k)
       b=(y1-y0-(l-l0)*dsin(phi1)-l0*dsin(phi2))/(2.0*k)
       !//это две квадратные скобки/2k
       c=l0*l0*(x1-x0)/2.0
       d=l0*l0*(y1-y0)/2.0
       dx=-c*dsin(phi0)-a*dcos(phi0)-d*dcos(phi0)+b*dsin(phi0)
       dy=c*dcos(phi0)-a*dsin(phi0)-d*dsin(phi0)-b*dcos(phi0)
	 sdv=dsqrt(dx*dx+dy*dy)
	if(dabs(dx).lt.0.0000001) then
	 naprav=3.1415926/2.0
	if(dy.lt.0) naprav=naprav+3.1415926

	return
	endif
        naprav=datan(dy/dx)
	if(dx.lt.0.0) naprav=naprav+3.1415926
       !printf("\n dx=%12.7lf,dy=%12.7lf",dx,dy);
       return
	end
       

	subroutine dsdvpodsig(phi,sig,k,l,d1,sdv,naprav)
	
	real*8 phi,sig,k,l,d1,sdv,dx,dy ! входные + dx, dy - частные произ по k,naprav
	real*8 l0,l1,x0,x1,y0,y1,phi0,phi1,phi2,a,b,c,d,naprav
       !d1-длина всей клотоиды,l -длина до определяемой точки
c	if(pr.gt.6) write(1,78)pr,phi,sig,k,l,d1
   78 format(2x,'p=',i3,' phi=',f12.5,' sig=',f12.7,' k=',f15.12,
     #' l=',f12.3,' d1=',f12.6)
       l0=sig/k
       phi0=phi-k*l0*l0/2 ! разворот систем координат
	 x0=cloxf(k,l0)
       y0=cloyf(k,l0)
	l1=l0+l
	x1=cloxf(k,l1) 
	y1=cloyf(k,l1)
      ! x0,y0 начало то есть x,y но в системе клотоиды
       ! x1,y1 координаты в системе клотоиды конца т. е. через l от начала

       !printf("\n x0=%8.3lf,y0=%8.3lf,x1=%8.3lf,y1=%8.3lf",x,y,x1,y1);
       !printf("\n l0=%8.3lf,phi0=%12.7lf,k=%12.7lf",l0,phi0,k);
       phi1=k*(l0+l)*(l0+l)/2.0
       phi2=k*l0*l0/2.0
      a=((x1-x0)/d1+(2.0-l/d1+l0/d1)*dcos(phi1)-(2.0+l0/d1)*dcos(phi2))
     # /(2.0*k)
      b=((y1-y0)/d1+(2.0-l/d1+l0/d1)*dsin(phi1)-(2.0+l0/d1)*dsin(phi2))
     # /(2.0*k)
       !//это две квадратные скобки/2k
       c=l0*(1.0+l0/d1/2.0)*(x1-x0)
	 d=l0*(1.0+l0/d1/2.0)*(y1-y0)
       dx=c*dsin(phi0)+a*dcos(phi0)+d*dcos(phi0)-b*dsin(phi0)
       dy=-c*dcos(phi0)+a*dsin(phi0)+d*dsin(phi0)+b*dcos(phi0)
c	if(pr.ge.6) write(1,77)pr,dx,dy,a,b,c,phi0,l0,d,d1
   77 format(2x,'p=',i3,' dx=',f12.3,' dy=',f9.3,' a=',f9.3,' b=',f9.3,
     #' c=',f12.3,' phi0=',f10.5,' l0=',f12.3,' d=',f12.3,' d1=',f12.3) 
	 sdv=dsqrt(dx*dx+dy*dy)
	if(dabs(dx).lt.0.0000001) then
	 naprav=3.1415926/2.0
	if(dy.lt.0) naprav=naprav+3.1415926
	return
	endif
        naprav=datan(dy/dx)
	if(dx.lt.0.0) naprav=naprav+3.1415926
       !printf("\n dx=%12.7lf,dy=%12.7lf",dx,dy);
       return
	end
      