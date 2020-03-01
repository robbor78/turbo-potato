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

LIBS=-lX11

ALL: DIRECTORIES $(EPATH)

DIRECTORIES: $(BDIR) $(ODIR)

$(BDIR):
	$(MKDIR_P) $(BDIR)
$(ODIR):
	$(MKDIR_P) $(ODIR)

_DEPS=fire.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = fire.o
OBJ=$(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: $(SDIR)/%.cc $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(EPATH): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o $(BDIR)/*
