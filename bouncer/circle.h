/*
 *Brandon Free
 *Rory Savage
 */



#ifndef CIRCLE_H
#define CIRCLE_H
#include <stdio.h>
#include <math.h>
#include <iostream>
extern "C"

#ifndef INT64_C
#define INT64_C(c) (c ## LL)
#define UINT64_C(c) (c ## ULL)
#endif

{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>

}

namespace circlespace
{
class circle
{
public:
	circle();
	~circle();
	AVFrame* drawCircle(AVFrame *p, int start);
};

}
#endif
