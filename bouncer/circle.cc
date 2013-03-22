//Brandon Free
//Rory Savage
//Circle drawing program
/*utahenc*/
//#include "libavutil/avassert.h"
//#include "avcodec.h"
//#include "bytestream.h"
//#include "bmp.h"
//#include "internal.h"
//#include <stdio.h>
//#include <math.h>
//#include "utah.h"
#ifndef INT64_c
#define INT64_C(c)(c##LL)
#define UNIT64_C(c)(c##ULL)
#endif

extern "C"

{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavcodec/bytestream.h>

}




//static int UTAH_encode_frame(AVCodecContext *avctx, AVPacket *pkt, const AVFrame *pict, int *got_packet)
AVFrame drawCircle(AVFrame *p, int start)
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
  radius=width*.20;
  int center=0;
  center=width/2;


 
  //For loop to draw circle
  for (int row=start; row<height; row++){
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
    }


	/* for(int col=0;col<width; col++){
      uint8_t *mypixel = p->data[0]+row*p->linesize[0]+col;
       bytestream_put_byte(&buf, mypixel[0]);

       }*/
  }

 
  return p;
}
