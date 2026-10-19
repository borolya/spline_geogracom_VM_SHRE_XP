      INTERFACE TO
     #SUBROUTINE ALDFP(N,MN,X,XMIN,XMAX,SIGMA,TH,EPSF,EPSX,
     #                 ITMAX,FUNC,GRAD,A,C,INDACT,ISTOP)
      INTEGER N,MN,ME,ITMAX,INDACT(N),ISTOP
      REAL *8 X(N),XMIN(N),XMAX(N),SIGMA(1),TH(1),EPSF,EPSX,A(N,1),C(1)
      EXTERNAL FUNC,GRAD
      END

      INTERFACE TO
     #SUBROUTINE MULTMV(A,B,C,N,INDACT)
      INTEGER N,INDACT(*)
      REAL *8 A(*),B(*),C(*)
      END

      INTERFACE TO
     #SUBROUTINE ALMO(N,MN,X,XMIN,XMAX,SIGMA,EPSC,EPSF,EPSX,DFPMAX,
     #                ALMMAX,FUNC,GRAD,C,TH,IACT,A,INDACT,ISTOP,pr)
      INTEGER N,MN,DFPMAX,ALMMAX,INDACT(N),ISTOP,pr
      REAL *8 X(N),XMIN(N),XMAX(N),SIGMA(1),EPSC,EPSF,EPSX,
     #        C(1),TH(1),A(N,1)
      LOGICAL *1 IACT(1)
      EXTERNAL FUNC,GRAD
      END
      INTERFACE TO
     #SUBROUTINE POINT(X,vis,it)
      REAL *8 X(1)
      integer vis,it
	  external func
      END
      INTERFACE TO
     #SUBROUTINE POINT_NEW0(X,vis,it)
      REAL *8 X(1)
      integer vis,it
	  external func
      END
      INTERFACE TO
     #SUBROUTINE ALR(X,S,FUNC,N,EPS,LAMAX,MN,ME,SIGMA,TH,LA,Y,C,it,LAM)
      INTEGER N,MN,ME,it
      REAL*8 X(N),S(N),EPS,LAMAX,SIGMA(1),TH(1),LA,Y(N),C(1),LAM
c     ! LAM do EE
      EXTERNAL FUNC
      END

      INTERFACE TO
     #SUBROUTINE FUNC(X,F,C)
      REAL *8 X(1),F,C(1)
      END

      INTERFACE TO
     #SUBROUTINE GRAD(ICON,X,G)
      REAL *8 X(*),G(*)
      INTEGER ICON
      END

      INTERFACE TO
     #SUBROUTINE LVIOL(EPSC,NFILE)
      REAL *8 EPSC
      INTEGER NFILE
      END



      INTERFACE TO
     #SUBROUTINE POINTG(it)
c      REAL *8 X(*)
      integer it
      END

      INTERFACE TO
     #SUBROUTINE PRSUM
      END

      INTERFACE TO
     #SUBROUTINE START(ERROR)
      INTEGER ERROR
      END
      INTERFACE TO
     #function cloxf(k,l)
        real*8 k,l
      end
      INTERFACE TO
     #function cloyf(k,l)
        real*8 k,l
      end
       INTERFACE TO
     #function cloxfs_1(x0,k,sig, phi,l)
        real*8 x0,k,sig, phi,l
      end
      INTERFACE TO
     #function cloyfs_1(y0,k,sig, phi,l)
        real*8 y0,k,sig, phi,l
      end
      INTERFACE TO
     # subroutine conlaf (xl,yl,phil,sig,l,xr,yr,phir)
       real*8 xl,yl,phil,sig,l,xr,yr,phir
       end
      INTERFACE TO
     #subroutine conlf(ksi,eta,phi,sig,k,l,ksi1,eta1,phi1,sig1)
      real*8  ksi,eta,phi,sig,k,l,ksi1,eta1,phi1,sig1
      end
      INTERFACE TO
     #subroutine crossokr(ksi,eta,psi,xk,yk,r,u,v,alf,cross,ugkas,np)
      real*8 ksi,eta,psi,xk,yk,r,u,v,alf,cross
      real*8 ugkas
	  integer np
      end
      INTERFACE TO
     #subroutine crossclo(ksi,eta,phi,sig,k,l,u,v,alf,cross,lp,np)
      real*8 ksi,eta,phi,sig,k,l,u,v,alf,cross,lp
	  integer np
      end
      INTERFACE TO
     #function crossln(ksi,eta,x1,y1,u,v,alf,np)
       real*8 ksi,eta,x1,y1,u,v,alf
	   integer np
       end
c      INTERFACE TO
c     #SUBROUTINE sdvig(sdv,nap)
c      REAL sdv,nap
c      END
c      INTERFACE TO
c     #SUBROUTINE rot(xa,ya,pov)
c       real xa,ya,pov
c      END
       