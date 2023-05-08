######################################################
# You should not need to change anything in this file
# See the Makefile.common file
######################################################

include Makefile.common
include Makefile.libm_objects

#CXX=cl

!ifdef MINGDIR
EXE_SUFFIX=.exe
!else
EXE_SUFFIX=
!endif

!ifdef STREFLOP_SOFT
USE_SOFT_BINARY=SoftFloatWrapperSimple.o SoftFloatWrapperDouble.o SoftFloatWrapperExtended.o softfloat/softfloat.o
!else
USE_SOFT_BINARY=
!endif

TARGETS = libm/flttarget libm/dbltarget
LIBM_OBJECTS = $(flt32objects) $(dbl64objects)
!ifndef STREFLOP_SSE
TARGETS = $(TARGETS) libm/ldbltarget
LIBM_OBJECTS = $(LIBM_OBJECTS) $(ldbl96objects)
!endif

all: streflop.a

Random.o: Random.cpp Random.h Makefile FPUSettings.h Math.h streflop.h
	$(CXX) -c $(CXXFLAGS) $(CPPFLAGS) Random.cpp /FoRandom.o

Math.o: Math.cpp Math.h Makefile FPUSettings.h streflop.h
	$(CXX) -c $(CXXFLAGS) $(CPPFLAGS) Math.cpp /FoMath.o

SoftFloatWrapperSimple.o: SoftFloatWrapper.cpp SoftFloatWrapper.h Makefile FPUSettings.h streflop.h
	$(CXX) -c $(CXXFLAGS) $(CPPFLAGS) /DN_SPECIALIZED=32 SoftFloatWrapper.cpp /Fo$@

SoftFloatWrapperDouble.o: SoftFloatWrapper.cpp SoftFloatWrapper.h Makefile FPUSettings.h streflop.h
	$(CXX) -c $(CXXFLAGS) $(CPPFLAGS) /DN_SPECIALIZED=64 SoftFloatWrapper.cpp /Fo$@

SoftFloatWrapperExtended.o: SoftFloatWrapper.cpp SoftFloatWrapper.h Makefile FPUSettings.h streflop.h
	$(CXX) -c $(CXXFLAGS) $(CPPFLAGS) /DN_SPECIALIZED=96 SoftFloatWrapper.cpp /Fo$@

streflop.a: Math.o Random.o $(USE_SOFT_BINARY)
	pushd libm && nmake all /F Makefile /D /A & popd
	@del /q /f streflop.a
	@ar r streflop.a $(LIBM_OBJECTS) Math.o Random.o $(USE_SOFT_BINARY)
!ifdef MINGDIR
	@copy streflop.a libstreflop.a
!else
	@copy streflop.a libstreflop.a
!endif

arithmeticTest$(EXE_SUFFIX): arithmeticTest.cpp streflop.a
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(LDFLAGS) arithmeticTest.cpp streflop.a /Fo$@

randomTest$(EXE_SUFFIX): randomTest.cpp streflop.a
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(LDFLAGS) randomTest.cpp streflop.a /Fo$@

.PHONY : clean package
clean:
	-rm -f *.o streflop.a libstreflop.a arithmeticTest$(EXE_SUFFIX) randomTest$(EXE_SUFFIX) $(USE_SOFT_BINARY)
	$(MAKE) -C libm clean


# Prepare source files, so it's possible to make a package even when the directory is cluttered

LIBM_STREFLOP = libm/import.pl libm/Makefile libm/streflop_libm_bridge.h libm/README.txt libm/e_expf.c libm/w_expf.c

SOFTFLOAT_STREFLOP = softfloat/milieu.h softfloat/softfloat.h softfloat/SoftFloat-README.txt softfloat/SoftFloat.txt softfloat/README.txt softfloat/SoftFloat-history.txt softfloat/SoftFloat-source.txt softfloat/softfloat.cpp softfloat/softfloat-macros softfloat/softfloat-specialize

BASE_STREFLOP = arithmeticTest.cpp randomTest.cpp FPUSettings.h IntegerTypes.h LGPL.txt Makefile Makefile.common Makefile.libm_objects Math.cpp Math.h Random.cpp Random.h README.txt SoftFloatWrapper.cpp SoftFloatWrapper.h streflop.h System.h X87DenormalSquasher.h

# Tar only once for both archive formats
package:
	@echo "preparing temporary subdir streflop-$(STREFLOP_VERSION)"
	@rm -rf streflop-$(STREFLOP_VERSION)
	@mkdir streflop-$(STREFLOP_VERSION)
	@echo "use tar to create subdirs"
	@tar cf - $(BASE_STREFLOP) $(SOFTFLOAT_STREFLOP) $(LIBM_STREFLOP) $(libmsrc) | tar xf - -C streflop-$(STREFLOP_VERSION)
	@echo $(STREFLOP_VERSION) > streflop-$(STREFLOP_VERSION)/VERSION.txt
	@echo "use tar again to create package, with full directory name included"
	@tar cf streflop-$(STREFLOP_VERSION).tar streflop-$(STREFLOP_VERSION)
	@echo "remove temporary directory"
	@rm -rf streflop-$(STREFLOP_VERSION)
	@echo "make best gzip archive"
	@cat streflop-$(STREFLOP_VERSION).tar | gzip --best > streflop-$(STREFLOP_VERSION).tar.gz
	@echo "convert tar to bz2"
	@bzip2 -f streflop-$(STREFLOP_VERSION).tar


# debug rules

arithmeticTest_x870$(EXE_SUFFIX): arithmeticTest.cpp
	$(CXX) $(CXXFLAGS) -DSTREFLOP_X87=1 $(LDFLAGS) arithmeticTest.cpp streflop_x870.a /Fo$@

arithmeticTest_soft0$(EXE_SUFFIX): arithmeticTest.cpp
	$(CXX) $(CXXFLAGS) -DSTREFLOP_SOFT=1 $(LDFLAGS) arithmeticTest.cpp streflop_soft0.a /Fo$@

%.hex : %.bin
	hexdump -C $< > $@

