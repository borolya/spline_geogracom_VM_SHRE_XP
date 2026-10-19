!****************************************************************************
!  Global data, parameters, and structures 
!****************************************************************************

module abdx !rewfortGlobals
use dflogm

implicit none

!  Parameters

integer*4, parameter, public :: SIZEOFAPPNAME = 100
	integer(4):: vnesh,ostatok
	real(4)::criter,nevaz
	character(50)::string
	logical(4)::flag
	integer(4)::status,ios
	integer(4)::  ret
      logical(4) :: lret
	 
!  Global data

integer		ghInstance
integer		ghModule
integer		ghwndMain
type (dialog) dxy ! gdlg


end module abdx
