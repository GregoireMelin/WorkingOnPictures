ROOT=.
CC=g++

OCVLIB=-lopencv_core -lopencv_imgproc -lopencv_highgui

CFLAGS  = -Wall -Wextra -Werror -I$(ROOT) -I/opt/opencv/include
LDFLAGS = -Wl,-R/opt/opencv/lib -L/opt/opencv/lib $(OCVLIB)

#modify data path or diplay command if applicable
DATA=../data
SHOW=pvisu

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
	#$(SHOW) $(DATA)/gear.png;
	cp $(DATA)/gear.png gear.png
	./make-se 2 10 disk-10.png
	./erosion disk-10.png gear.png gear-res.png
	./dilation disk-10.png gear-res.png  gear-res.png
	  composite gear.png gear-res.png\
	            -compose difference  difference.png
	 xdg-open difference.png
	 ./labeling-color difference.png gear-res.png gear-res-color.png
	 xdg-open gear-res.png
	 xdg-open gear-res-color.png





extract-cell:
	#$(SHOW) $(DATA)/cell.png;

clean:
	$(RM) *~ *.png
cleanall: clean
	$(RM) $(BIN) *.o *.pdf

.PHONY: all clean cleanall test-deploy extract-gear extract-gear
