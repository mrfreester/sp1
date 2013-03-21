all: a.out

a.out: bouncer.cc
	g++ -I ../include/ -L../lib *.cc `pkg-config --cflags --libs libavutil libavformat libavcodec`

clean:
	rm -f *.o *.out *.cc~ *.utah