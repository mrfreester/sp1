all: bouncer.out

bouncer.out: bouncer.cc circle.h circle.cc
	g++ -o bouncer -I ../include/ -L../lib *.cc *.h `pkg-config --cflags --libs libavutil libavformat libavcodec libswscale`

clean:
	rm -f *.o *.out *.cc~ *.utah frame* *.mp4 bouncer *.gch

movie: *.utah
	ffmpeg -f image2 -r 30 -pattern_type glob -i '*.utah' movie.mp4; ffplay movie.mp4
frame: 
	./bouncer street.jpg

full:
	make clean; make; make frame; make movie