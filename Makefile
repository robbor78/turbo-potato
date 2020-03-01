# $@ left side of :
# $^ right side of :
# $< first item in dependency list
# automatic variables:  https://www.gnu.org/software/make/manual/html_node/Automatic-Variables.html#Automatic-Variables

MKDIR_P=mkdir -p

IDIR=include
CC=g++
CFLAGS=-I$(IDIR)

SDIR=src
ODIR=obj
LDIR=lib
BDIR=build
EXE=fire
EPATH=$(BDIR)/$(EXE)

INC_TEST=-I .
SDIR_TEST=test
EXE_TEST=firetest
EPATH_TEST=$(BDIR)/$(EXE_TEST)


LIBS=-lX11 -lspdlog

ALL: DIRECTORIES $(EPATH) $(EPATH_TEST)

DIRECTORIES: $(BDIR) $(ODIR)

$(BDIR):
	$(MKDIR_P) $(BDIR)
$(ODIR):
	$(MKDIR_P) $(ODIR)

_DEPS=fire_algo.h constants.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = fire.o fire_algo.o
OBJ=$(patsubst %,$(ODIR)/%,$(_OBJ))

_OBJ_TEST = test.o $(filter-out fire.o, $(_OBJ))
OBJ_TEST=$(patsubst %,$(ODIR)/%,$(_OBJ_TEST))

$(ODIR)/%.o: $(SDIR)/%.cc $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR)/%.o: $(SDIR_TEST)/%.cc $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(EPATH): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

$(EPATH_TEST): $(OBJ_TEST)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o $(BDIR)/*
