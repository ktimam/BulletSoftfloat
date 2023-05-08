# common definitions
include ../Makefile.common

!ifdef MINGDIR
TOUCH=echo >
!else
TOUCH=touch
!endif
TOUCH=echo >

TARGETS = flttarget dbltarget
!ifndef STREFLOP_SSE
#TARGETS = $(TARGETS) ldbltarget
!endif

all: $(TARGETS)

flttarget:
	pushd flt32 && nmake all /F Makefile /D /A & popd
	$(TOUCH) flttarget

dbltarget:
	pushd dbl64 && nmake all /F Makefile /D /A & popd
	$(TOUCH) dbltarget

ldbltarget:
	pushd ldbl96 && nmake all /F Makefile /D /A & popd
	$(TOUCH) ldbltarget

clean:
	rm -f flt32/*.o dbl64/*.o ldbl96/*.o flttarget dbltarget ldbltarget

clean-import:
	rm -rf flt32 dbl64 ldbl96 headers
