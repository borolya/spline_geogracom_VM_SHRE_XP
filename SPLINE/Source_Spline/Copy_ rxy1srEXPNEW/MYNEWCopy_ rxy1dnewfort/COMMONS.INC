C==================================================================
C
C                         /POINTS/
C
C------------------------------------------------------------------
      REAL *8 XEV(1000),YEV(1000)
      COMMON /POINTS/ XEV,YEV
C------------------->
C      XEV(NPOINT),YEV(NPOINT) - KOOPÄÈHATÛ AÏÏPOKCÈMÈPÓEMÛX TO×EK
C                               
C      NPOINT - ×ÈCËO TO×EK
C==================================================================
C
C                         /LINE/
C
C------------------------------------------------------------------
      REAL *8 XL,L(150),K(150),SIG(150),LA(150),PHI(2,150),
     #        KSI(2,150),ETA(2,150)
       INTEGER NCURV
      COMMON /LINE/ XL,L,K,SIG,LA,PHI,KSI,ETA,NCURV
	 
C------------------->
C      XL - ÄËÈHA HA×AËÜHOÉ ÏPßMOÉ
C      L(NCURV+1) - ÄËÈHÛ ÏEPEXOÄHÛX
C      K(NCURV+1) - KOÝÔÔÈÖÈEHTÛ ÏEPEXOÄHÛX
C      SIG(NCURV+1) - PAÄÈÓCÛ KPÈBÛX ( =0 ÄËß ÏPßMÛX); SIG(NCURV+1)=0
C      LA(NCURV) - ÄËÈHÛ KPÈBÛX È ÏPßMÛX (ÁEÇ ÏEPEXOÄHÛX)
C      PHI(1,I) - ÓÃOË B HA×AËE I-OÉ ÏEPEXOÄHOÉ
C      PHI(2,I) - ÓÃOË B HA×AËE I-OÉ KPÈBOÉ (ÏPßMOÉ)
C      KSI(1,I),ETA(1,I) - KOOPÄÈHATÛ HA×AËA I-OÉ ÏEPEXOÄHOÉ (ÝBOËÜB.)
C      KSI(2,I),ETA(2,I) - KOOPÄÈHATÛ HA×AËA I-OÉ KPÈBOÉ (ÏPßMOÉ)
C      JFIRST(1,I) - HOMEP ÏEPBOÉ TO×KÈ ÏOCËE HA×AËA I-OÉ ÏEPEXOÄHOÉ
C      JFIRST(2,I) - HOMEP ÏEPBOÉ TO×KÈ ÏOCËE HA×AËA I-OÉ KPÈBOÉ
C      NCURV - ×ÈCËO KPÈBÛX È ÏPßMÛX BCTABOK
C==================================================================
C
C                         /VARPTR/
C
C------------------------------------------------------------------
      INTEGER *2 TYPVAR(350),NUMVAR(350)
      REAL*8 WGTVAR(4)
      INTEGER NVAR
      COMMON /VARPTR/ TYPVAR,NUMVAR,WGTVAR,NVAR
C------------------->
C      TYPVAR(I) - TÈÏ ÏEPEMEHHOÉ  X(I):
C                  1 - XL,
C                  2 - SIG,
C                  3 - LA,
C                  4 - L
C      NUMVAR(I) - HOMEP ÏEPEMEHHOÉ  X(I)  B TÈÏE
C      WGTVAR(4) - MACØTAÁHÛE MHOÆÈTEËÈ ÄËß PAÇËÈ×HÛX TÈÏOB ÏEPEMEHHÛX
C            ÏPÈMEP:
C              ÏPÈ  TYPVAR(I)=2  -  X(I)=SIG(NUMVAR(I))/WGTVAR(2)
C      NVAR - ×ÈCËO ÏEPEMEHHÛX
C==================================================================
C
C                         /CONPTR/
C
C------------------------------------------------------------------
   
      REAL*8 WGTCON
      INTEGER NCON
      COMMON /CONPTR/ WGTCON,NCON
c  îñòàâèë òîëüêî îãðàíè÷åíèÿ íà ñäâèæêè è óáðàë òèïû   
C      WGTCON - MACØTAÁHÛÉ MHOÆÈTEËÜ ÄËß  OÃPAHÈ×EHÈÉ =10

C==================================================================
C
C                         /SHIFTS/
C
C------------------------------------------------------------------
      REAL *8 SHIFT(1000),SHMIN(1000),SHMAX(1000)
      COMMON /SHIFTS/ SHIFT,SHMIN,SHMAX
C------------------->
C      SHIFT(NPOINT) - ÏPOEKTHÛE CÄBÈÆKÈ
C      SHMIN(NPOINT),SHMAX(NPOINT) - ÏPEÄEËÜHÛE ÇHA×EHÈß CÄBÈÆEK
C==================================================================
C

C==================================================================
C
C                         /DSHIFT/
C
C------------------------------------------------------------------
      REAL *8 DSHX(1000,350)
      INTEGER   JFIRST(2,150) ,NPOINT ! 250 ìíîãî
      COMMON /DSHIFT/ DSHX,JFIRST ,NPOINT 
	
C------------------->
C      DSHX(I,J) = DSHIFT(I)/DX(J)
C==================================================================
C
C                         /FINAL/
C
C------------------------------------------------------------------

      REAL *8 KSIFIN,ETAFIN,PHIFIN
      COMMON /FINAL/ KSIFIN,ETAFIN,PHIFIN
C------------------->
C      KSIFIN,ETAFIN - KOOPÄÈHATÛ KOHE×HOÉ TO×KÈ
C      PHIFIN - KOHE×HÛÉ ÓÃOË
C==================================================================
C
C                         /BOUNDS/
C
C------------------------------------------------------------------
      REAL *8 XLMIN,KMIN(150),KMAX(150),SIGMIN(150),SIGMAX(150),
     #  LAMIN(150),LAMAX(150),LMIN(150),LMAX(150),XLMAX

      COMMON /BOUNDS/ XLMIN,KMIN,KMAX,SIGMIN,SIGMAX,LAMIN,LAMAX,
     #                LMIN,LMAX,XLMAX
C-------------------äîáàâëåí XLMAX>
C      XLMIN - MÈHÈMAËÜHOE ÇHA×EHÈE  XL
C      KMIN(NCURV+1),KMAX(NCURV+1) - ÃPAHÈÖÛ  K
C      SIGMIN(NCURV),SIGMAX(NCURV) - ÃPAHÈÖÛ  SIG
C      LAMIN(NCURV),LAMAX(NCURV) - ÃPAHÈÖÛ  LA
C      LMIN(NCURV+1),LMAX(NCURV+1) - ÃPAHÈÖÛ  L
C óáðàë LLAMIN(NCURV) - MÈHÈMÓM ÄËÈHÛ KPÈBOÉ C ÏOËÓCÓMMOÉ ÏEPEXOÄHÛX
C==================================================================
C
C                         /CRIT/
C
C------------------------------------------------------------------
      INTEGER TYPECR,MULTCR
      COMMON /CRIT/ TYPECR,MULTCR
C------------------->
C      TYPECR - TÈÏ KPÈTEPÈß OÏTÈMÈÇAÖÈÈ:
C         1 - CÓMMA PÈXTOBOK
C         2 - CÓMMA KBAÄPATOB PÈXTOBOK
C      MULTCR - MHOÆÈTEËÜ ÏPÈ CÓMME PÈXTOBOK ÄËß  TYPECR=1
C==================================================================
C
C                         /PAROPT/
C
C------------------------------------------------------------------
      REAL *8 EPSC,EPSF,EPSX
      INTEGER DFPMAX,ALMMAX
      COMMON /PAROPT/ EPSC,EPSF,EPSX,DFPMAX,ALMMAX
C------------------->
C      EPSC - TO×HOCTÜ ÏO OÃPAHÈ×EHÈßM
C      EPSF - TO×HOCTÜ ÁEÇÓCËOBHOÉ OÏTÈMÈÇAÖÈÈ ÏO KBAÄPATÓ
C             HOPMÛ ÃPAÄÈEHTA
C      EPSX - TO×HOCTÜ ÁEÇÓCË. OÏTÈMÈÇAÖÈÈ ÏO MAKC.MOÄÓËß
C             ÈÇMEHEHÈß  X  ÇA OÄHÓ ÈTEPAÖÈÞ
C      DFPMAX - CÓMMAPHÛÉ PECÓPC BHÓTPEHHÈX ÈTEPAÖÈÉ
C      ALMMAX - PECÓPC BHEØHÈX ÈTEPAÖÈÉ
C==================================================================
