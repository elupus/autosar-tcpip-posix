INCLUDES += ../../source/
INCLUDES += include/

VPATH     = ../../source/


TESTS    = suite_1

SOURCES  = $(addsuffix /main.c,$(TESTS))
OBJECTS  = $(SOURCES:.c=.o)
DEPS     = $(SOURCES:.c=.d)
BINS     = $(SOURCES:.c=)
XMLS     = $(addsuffix /CUnitAutomated-Results.xml,$(TESTS))

CFLAGS+=-MMD -g -std=c99 $(addprefix -I,$(INCLUDES))
LDLIBS+= -lcunit

%/main: %/main.c
	$(CC) $(CFLAGS)  -I$* $< $(LDLIBS) -o $@

%/CUnitAutomated-Results.xml: %/main
	cd $*; ./$(<F)

all: $(BINS) $(XMLS)

clean:
	$(RM) $(OBJECTS) $(DEPS) $(BINS)
	
-include $(DEPS)
