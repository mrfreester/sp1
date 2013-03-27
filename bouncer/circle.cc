//Brandon Free
//Rory Savage
//Circle drawing program
/*utahenc*/
//#include "libavutil/avassert.h"
//#include "avcodec.h"
//#include "bytestream.h"
//#include "bmp.h"
//#include "internal.h"
#include <stdio.h>
#include <math.h>
//#include "utah.h"
#include <iostream>
#include "circle.h"

#ifndef INT64_C
#define INT64_C(c) (c ## LL)
#define UINT64_C(c) (c ## ULL)
#endif

extern "C"

{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
//#include <../ffmpeg/libavcodec/bytestream.h>

}

namespace circlespace
{
  circle::circle(){}
  circle::~circle(){}

//static int UTAH_encode_frame(AVCodecContext *avctx, AVPacket *pkt, const AVFrame *pict, int *got_packet)
  AVFrame* circle::drawCircle(AVFrame *p, AVCodecContext *c,  int start)
{
  
  // UTAHContext *s = avctx->priv_data;
  //AVFrame * const p = &s->picture;
  uint8_t *buf;
  //*p = *pict;
  //p->pict_type = AV_PICTURE_TYPE_I;
  //p->key_frame = 1;
  // AVFrame * const p=*pict;
  //*p=*pict;
  //int ret;
  int height;
  int width;
  height = p->height;
  width = p->width;
  //int n_bytes;
  // height = avctx->height;
  //width = avctx->width;
  //n_bytes =12+ height * width;
  // printf("Width: %d, height: %d, Linesize: %d\n",avctx->width, avctx->height);
  
  int radius=0;
  radius=c->width*.10;
  int center=0;
  center=(width/2);
  //int centerH=(c->height/2);

  int centerH=50*(sin(start/3))+height/2;
  int diamater=radius*2;

  int left=0;
  int right=0;


  ;
 
  int x,y=0,i;

            std::cout<<radius<<y<<std::endl;
            for(y=0;y<p->height;y++) {


            for(x=0;x<p->width;x++) {

              if(!(radius<sqrt(pow(center-x,2)+pow(centerH-y,2)))){
                //p->data[0][y * p->linesize[0] + x] = x + y + i * 3;
                uint8_t * pix = p->data[0] + p->linesize[0]*y + x*3;
                int scale, r, g, b;
                scale = (.05*(center-x)*(centerH-y));
                r = 150+scale;
                g = 150+scale;
                b = 100+scale;
                *(pix++) = r;
                *(pix++) = g;
                *(pix++) = b;
              }

		
            }
        }


 
  return p;
}

};
