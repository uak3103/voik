PROG=kpst
GCC=g++
CXXFLAGS=-c -I. -w -fpic

all: kpst sys.o rv.o \
	easy_vu.o \
	easy_csn.o \
	apoi.o \
	globst.o \
	ki.o \
	st.o \
	vdol.o \
	dialog.o \
	common.o

common.o: ../common/common.cpp
	$(GCC) $(CXXFLAGS) ../common/common.cpp

sys.o: sys/sys.c ../common/common.h sys/sys.h globst.h
	$(GCC) $(CXXFLAGS) sys/sys.c

rv.o: rv/rv.c ../common/common.h sys/sys.h globst.h rv/rv.h inter.h rv/dump.c rv/candev.c rv/ipx.c
	$(GCC) $(CXXFLAGS) rv/rv.c

easy_vu.o: vu/easy_vu.c sys/sys.h rv/rv.h vu/vu.h
	$(GCC) $(CXXFLAGS) vu/easy_vu.c

easy_csn.o: csn/easy_csn.c sys/sys.h rv/rv.h vu/vu.h globst.h
	$(GCC) $(CXXFLAGS) csn/easy_csn.c

apoi.o: apoi/apoi.c sys/sys.h rv/rv.h globst.h 
	$(GCC) $(CXXFLAGS) apoi/apoi.c

globst.o: globst.c sys/sys.h globst.h coord.c reductio.c paspst.c am.c naved.c zkt.c omr.c
	$(GCC) $(CXXFLAGS) globst.c

ki.o: ki/ki.c ki/ki.h globst.h rv/rv.h st.h ki/prognozt.c ki/orlova.c ki/moon&sun.c ki/readts.c
	$(GCC) $(CXXFLAGS) ki/ki.c

st.o: st.c sys/sys.h rv/rv.h vu/vu.h apoi/apoi.h csn/csn.h inter.h globst.h st.h ki/ki.h inter.c uprctk.c uprst.c uprki.c uprtest.c config.c
	$(GCC) $(CXXFLAGS) st.c

vdol.o: vdol.c rv/rv.h csn/csn.h inter.h globst.h st.h 
	$(GCC) $(CXXFLAGS) vdol.c

dialog.o: dialog.c sys/sys.h rv/rv.h vu/vu.h apoi/apoi.h csn/csn.h inter.h globst.h st.h ki/ki.h
	$(GCC) $(CXXFLAGS) dialog.c

kpst: sys.o rv.o easy_vu.o easy_csn.o apoi.o globst.o ki.o st.o vdol.o dialog.o common.o
	$(GCC) -lm -lpthread -lrt -shared -o libkpst.so sys.o rv.o easy_vu.o easy_csn.o apoi.o globst.o ki.o st.o vdol.o dialog.o common.o

clean:
	rm kpst *.o core*

dist:
	cd .. && tar cvfz $(PROG).tgz $(PROG)
