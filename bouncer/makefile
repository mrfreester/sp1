all: a.out

a.out: bouncer.cc circle.h circle.cc
	g++ -I ../include/ -L../lib *.cc *.h `pkg-config --cflags --libs libavutil libavformat libavcodec libswscale`

clean:
	rm -f *.o *.out *.cc~ *.utah frame*
