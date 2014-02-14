# -*- mode: Makefile; -*-
# -----------------------------------------
# project PKVI


export PATH := /opt/wx/2.8/bin:$(PATH)
export LD_LIBRARY_PATH := /opt/wx/2.8/lib:$(LD_LIBRARY_PATH)

_WX = /home/gr/projects/gui/codeblocks/wx
_WX.LIB = $(_WX)/lib
_WX.INCLUDE = $(_WX)/include

_CB = /home/gr/projects/gui/codeblocks/cb/src
_CB.INCLUDE = $(_CB)/include
_CB.LIB = $(_CB)/devel



CFLAGS_C = $(filter-out -include "sdk.h",$(CFLAGS))

# -----------------------------------------

# MAKE_DEP = -MMD -MT $@ -MF $(@:.o=.d)

#XTOOLS=/opt/buildroot/buildroot-2010.05/output/target
XTOOLS=/opt/work/uak/buildroot/output/target
#XTOOLS=/opt/uak/buildroot/output/target
GCC   =$(XTOOLS)/../staging/usr/bin/i486-unknown-linux-uclibc-gcc
GPP   =$(XTOOLS)/../staging/usr/bin/i486-unknown-linux-uclibc-g++

#CFLAGS = -Wall `wx-config --cxxflags` -Winvalid-pch -include wx_pch.h -DWX_PRECOMP -DGL_GLEXT_PROTOTYPES 

WX_CONFIG_CFLAGS=-I$(XTOOLS)/usr/local/lib/wx/include/i486-linux-gtk2-unicode-2.9 \
	-I$(XTOOLS)/usr/local/include/wx-2.9 -D_FILE_OFFSET_BITS=64 \
	-DWXUSINGDLL -D__WXGTK__ -pthread
CFLAGS = -Wall $(WX_CONFIG_CFLAGS) \
	-Winvalid-pch -include wx_pch.h -DWX_PRECOMP -DGL_GLEXT_PROTOTYPES 

WX_CONFIG_LIBS_ALL=-L$(XTOOLS)/usr/local/lib -pthread \
	-lwx_gtk2u_xrc-2.9-i486-linux \
	-lwx_gtk2u_stc-2.9-i486-linux \
	-lwx_gtk2u_richtext-2.9-i486-linux \
	-lwx_gtk2u_ribbon-2.9-i486-linux \
	-lwx_gtk2u_propgrid-2.9-i486-linux \
	-lwx_gtk2u_aui-2.9-i486-linux \
	-lwx_gtk2u_gl-2.9-i486-linux \
	-lwx_gtk2u_html-2.9-i486-linux \
	-lwx_gtk2u_qa-2.9-i486-linux \
	-lwx_gtk2u_adv-2.9-i486-linux \
	-lwx_gtk2u_core-2.9-i486-linux \
	-lwx_baseu_xml-2.9-i486-linux \
	-lwx_baseu_net-2.9-i486-linux \
	-lwx_baseu-2.9-i486-linux

INCLUDES = -I$(XTOOLS)/usr/local/include \
	-I$(XTOOLS)/usr/lib/wx/include/gtk2-unicode-2.9 \
	-I$(XTOOLS)../staging/usr/include \
	-I$(XTOOLS)/usr/include/wx-2.9 
#LDFLAGS = `wx-config --libs all` -L/usr/local/lib  -s
LDFLAGS = $(WX_CONFIG_LIBS_ALL) -L$(XTOOLS)/usr/local/lib \
	-L$(XTOOLS)../staging/usr/lib
RCFLAGS = 
LDLIBS = $(T_LDLIBS)  -lstdc++

LINK_exe = $(GCC) -o $@ $^ $(LDFLAGS) $(LDLIBS)
LINK_con = $(GCC) -o $@ $^ $(LDFLAGS) $(LDLIBS)
LINK_dll = $(GCC) -o $@ $^ $(LDFLAGS) $(LDLIBS) -shared
LINK_lib = rm -f $@ && ar rcs $@ $^
COMPILE_c = $(GCC) $(CFLAGS_C) -o $@ -c $< $(MAKEDEP) $(INCLUDES)
COMPILE_cpp = $(GPP) $(CFLAGS) -o $@ -c $< $(MAKEDEP) $(INCLUDES)
COMPILE_rc = windres $(RCFLAGS) -J rc -O coff -i $< -o $@ -I$(dir $<)

%.o : %.c ; $(COMPILE_c)
%.o : %.cpp ; $(COMPILE_cpp)
%.o : %.cxx ; $(COMPILE_cpp)
%.o : %.rc ; $(COMPILE_rc)
.SUFFIXES: .o .d .c .cpp .cxx .rc

all: all.before all.targets all.after

all.before :
	-
all.after : $(FIRST_TARGET)
	
all.targets : Release_target Debug_target 

clean :
	rm -fv $(clean.OBJ)
	rm -fv $(DEP_FILES)
	rm -rf bin/Debug
	rm -rf bin/Release
	rm -rf wx_pch.h.gch
	rm -rf obj/Debug
	rm -rf obj/Release

.PHONY: all clean distclean

# -----------------------------------------
# Release_target

Release_target.BIN = bin/Release/PKVI.exe
Release_target.OBJ = CanPanel.o DatChoiceMain.o NetworkPanel.o PKVIApp.o PKVIMain.o PrProtMain.o ScreenWindow.o SettingsDlg.o ccan.o pci_7841.o 
DEP_FILES += CanPanel.d DatChoiceMain.d NetworkPanel.d PKVIApp.d PKVIMain.d PrProtMain.d ScreenWindow.d SettingsDlg.d ccan.d pci_7841.d 
clean.OBJ += $(Release_target.BIN) $(Release_target.OBJ)

Release_target : Release_target.before $(Release_target.BIN) Release_target.after_always
Release_target : CFLAGS += -O3 -O2 -O1 -O  -Os
Release_target : INCLUDES += 
Release_target : RCFLAGS += 
Release_target : LDFLAGS += -s
Release_target : T_LDLIBS = 
ifdef LMAKE
Release_target : CFLAGS -= -O1 -O2 -g -pipe
endif

Release_target.before :
	test -d bin/Release || mkdir -p bin/Release
	
Release_target.after_always : $(Release_target.BIN)
	
$(Release_target.BIN) : $(Release_target.OBJ)
	$(LINK_exe)
	

# -----------------------------------------
# Debug_target

Debug_target.BIN = bin/Debug/PKVI.exe
Debug_target.OBJ = CanPanel.o DatChoiceMain.o NetworkPanel.o PKVIApp.o PKVIMain.o PrProtMain.o ScreenWindow.o SettingsDlg.o ccan.o pci_7841.o 
DEP_FILES += CanPanel.d DatChoiceMain.d NetworkPanel.d PKVIApp.d PKVIMain.d PrProtMain.d ScreenWindow.d SettingsDlg.d ccan.d pci_7841.d 
clean.OBJ += $(Debug_target.BIN) $(Debug_target.OBJ)

Debug_target : Debug_target.before $(Debug_target.BIN) Debug_target.after_always
Debug_target : CFLAGS += -g -DwxUSE_DEBUGREPORT -D_WXDEBUG_  -Os
Debug_target : INCLUDES += 
Debug_target : RCFLAGS += 
Debug_target : LDFLAGS +=   
Debug_target : T_LDLIBS = 
ifdef LMAKE
Debug_target : CFLAGS -= -O1 -O2 -g -pipe
endif

Debug_target.before :
	test -d bin/Debug || mkdir -p bin/Debug
	
Debug_target.after_always : $(Debug_target.BIN)
	
$(Debug_target.BIN) : $(Debug_target.OBJ)
	$(LINK_exe)
	

# -----------------------------------------
ifdef MAKE_DEP
-include $(DEP_FILES)
endif
