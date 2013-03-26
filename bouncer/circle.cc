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
  std::cout<<"//////////////////////////////////////////////////"<<std::endl;
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
  radius=width*.20;
  int center=0;
  center=width/2;
  int diamater=radius*2;

  int left=0;
  int right=0;

  ;
 ////////////////////////////////////////////////////////////////////////////////////////////////////
  int x,y,i;
AVFrame *frame; 
//AVCodecContext *c= NULL;
//AVCodec *codec;


//codec = avcodec_find_encoder(AV_CODEC_ID_UTAH);
    //c = avcodec_alloc_context3(codec);

 frame = p;
 //c->height = 20;
 //c->width = 20;
 /*for(start=0;start<height;start++) {
            for(left=0;left<width;left++) {
<<<<<<< HEAD
                p->data[0][start * p->linesize[0] + left] = 150;
=======
                p->data[0][start * p->linesize[0] + left] = left + start + 1 * 3;
		std::cout << "imhere" << std::endl;
>>>>>>> 2f4437f177cc21da732b9b58257d0a55e88df93f
            }
	    }*/


        /* prepare a dummy image */
        /* Y */
  for(y=0;y<c->height;y++) {
            for(x=0;x<c->width;x++) {
                frame->data[0][y * frame->linesize[0] + x] = x + y + i * 3;
		std::cout << y << std::endl;
            }
        }

        /* Cb and Cr */
        /*for(y=0;y<c->height/2;y++) {
            for(x=0;x<c->width/2;x++) {
                frame->data[1][y * frame->linesize[1] + x] = 128 + y + i * 2;
                frame->data[2][y * frame->linesize[2] + x] = 64 + x + i * 5;
            }
	    }*/
  //////////////////////////////////////////////////////////////////////////////////////////////////
  //For loop to draw circle
 /* for (int row=start; row<=diamater; row++){
    // define y value
    int y=row;
    //define x value
    int x=0;
    //Get x position on circle
    x=sqrt(radius*radius - y*y);
    //get point on left side of circle
    int left=center-x;
    //get point on right side of circle
    int right=center+x;
    //define color
    uint8_t  color=0;
    //for loop to draw one line of the circle
    for(left;left<=right; left++){
      //get position of pixel
      uint8_t *mypixel = p->data[0]+row*p->linesize[0]+left;
      //changer color of pixel
      mypixel=&color;
      //place pixel into picture
      bytestream_put_byte(&buf, mypixel[0]);
			  //increment color
			  color++;
    }*/


	/* for(int col=0;col<width; col++){
      uint8_t *mypixel = p->data[0]+row*p->linesize[0]+col;
       bytestream_put_byte(&buf, mypixel[0]);

       }
       }*/

 
  return p;
}

};
