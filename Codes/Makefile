.PHONY: clean all

BINDIR = ./bin
#BINDIR = $(HOME)/bin
LIBDIR = ./lib

ROOTCFLAGS	:= $(shell root-config --cflags)
ROOTLIBS	:= $(shell root-config --libs)
ROOTVER		:= $(shell root-config --version | head -c1)

CPP         := $(shell root-config --cxx)
CFLAGS      := -Wall -g -std=c++11 $(ROOTCFLAGS) -fPIC

INCLUDES    := -I./

LIBS        := $(ROOTLIBS)

ifeq ($(ROOTVER),5)
	ROOTDICT  := rootcint
	DICTEXT   := .h
else
	ROOTDICT  := rootcling
	DICTEXT   := _rdict.pcm
endif

all: GaussianFitter

OBJECTS = ReadFile.o \
					Settings.o \
					ReadNTuple.o \
					ReadMatrix.o \
					FitGaussians.o \
					WriteOutput.o \
					DrawFigure.o \
          GaussianFitter_dict.o

GaussianFitter: GaussianFitter.cxx $(OBJECTS)
	$(CPP) $(CFLAGS) $(INCLUDES) $< $(OBJECTS) -o $@ $(LIBS)
	$(AR) cru lib$@.a $(OBJECTS)
	cp $@ $(BINDIR)/
	cp lib$@.a $(LIBDIR)/

%.o: %.cxx %.h
	$(CPP) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf *.o *Dict.cxx *.a *.dSYM *.pcm *$(DICTEXT) GaussianFitter $(BINDIR)/GaussianFitter $(LIBDIR)/libGaussianFitter.a $(LIBDIR)/GaussianFitter_dict.h libGaussianFitter.a

# Root stuff
DEPENDENCIES = ReadFile.h \
			   Settings.h\
			   ReadNTuple.h\
							 ReadMatrix.h\
							 GlobalFunctions.h\
							 FitGaussians.h\
							 WriteOutput.h\
							 DrawFigure.h\
               Linkdef.h

%_dict.o: %_dict.cxx
	$(CPP) $(CFLAGS) $(INCLUDES) -c $<

%_dict.cxx: $(DEPENDENCIES)
	rm -f $@ *_dict$(DICTEXT)
	$(ROOTDICT) -f $@ -c $(INCLUDES) $(DEPENDENCIES)
	cp $*_dict$(DICTEXT) $(LIBDIR)/
