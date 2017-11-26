ROOT=.
CC=g++

OCVLIB=-lopencv_core -lopencv_imgproc -lopencv_highgui

CFLAGS  = -Wall -Wextra -Werror -I$(ROOT) -I/opt/opencv/include
LDFLAGS = -Wl,-R/opt/opencv/lib -L/opt/opencv/lib $(OCVLIB)

#modify data path or diplay command if applicable
DATA=../data
DISPLAY=pvisu

OBJ = morphology.o
BIN = \
	make-se\
	dilation\
	erosion\
	opening\
	closing\
	labeling\
	labeling-color

all: $(OBJ) $(BIN)

test-deploy: all
	@./make-se ; true
	@./dilation ; true
	@./labeling ; true
	@./labeling-color ; true
	make extract-gear
	make extract-cell

morphology.o: morphology.cpp morphology.hpp
	$(CC) -c $(CFLAGS) -o $@ morphology.cpp
%: %.cpp morphology.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

extract-gear:
	$(DISPLAY) $(DATA)/gear.png;

extract-cell:
	$(DISPLAY) $(DATA)/cell.png;

clean:
	$(RM) *~ *.png
cleanall: clean
	$(RM) $(BIN) *.o *.pdf

.PHONY: all clean cleanall test-deploy extract-gear extract-gear
