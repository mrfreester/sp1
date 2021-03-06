/*
 *Once we get linked with ffmpeg, we'll
 * likely need to use avformat_open_input().
 * to open a jpg image to an avframe for editing.
 *
 */

 /*
  *Modified by Brandon Free
  *and Rory Savage
 */


#include <iostream>
#include "circle.h"
#include <typeinfo>

#ifndef INT64_C
#define INT64_C(c) (c ## LL)
#define UINT64_C(c) (c ## ULL)
#endif

 extern "C"
 {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
 }


 using namespace std;


//Takes in an AVFrame and converts it to a new one with
//a specified pixel format
AVFrame * Convert(AVFrame * in, AVPixelFormat out_fmt)
 {
   //holds the frame we're converting to
  AVFrame * outFrame;
  uint8_t *buffer = NULL;
  struct SwsContext *sws_ctx = NULL;

  outFrame=avcodec_alloc_frame();
  // Determine required buffer size and allocate buffer
  int numBytes=avpicture_get_size(out_fmt, in->width,
in->height);
  buffer=(uint8_t *)av_malloc(numBytes*sizeof(uint8_t));

  sws_ctx =
    sws_getContext
    (
        in->width,
        in->height,
        (AVPixelFormat)in->format,
        in->width,
        in->height,
        out_fmt,
        SWS_BILINEAR,
        NULL,
        NULL,
        NULL
    );

    avpicture_fill((AVPicture *)outFrame, buffer, out_fmt, in->width, in->height);

            sws_scale
        (
            sws_ctx,
            (uint8_t const * const *)in->data,
            in->linesize,
            0,
            in->height,
            outFrame->data,
            outFrame->linesize
        );

        outFrame->width = in->width;
        outFrame->height = in->height;
        outFrame->format = (int)out_fmt;

        return outFrame;
 }


 void SaveFrame(AVFrame *pFrame, int width, int height, int iFrame) {
  FILE *pFile;
  char szFilename[32];
  int y;

  AVCodec *codec;
  AVCodecContext *c= NULL;
   int ret, got_output;
  AVPacket pkt;
  AVFrame *circFrame;
  AVFrame *outFrame;
  circlespace::circle circ;

  //Our circle will always be RGB24
  //but the utah encoder may be RGB8 or RGB24
  //so we need to allocate one frame for each
  circFrame = avcodec_alloc_frame();
  outFrame = avcodec_alloc_frame();

  // Open file
  sprintf(szFilename, "frame%03d.utah", iFrame);
  pFile=fopen(szFilename, "wb");
  if(pFile==NULL)
    return;


   av_init_packet(&pkt);
   pkt.data = NULL;    // packet data will be allocated by the encoder
   pkt.size = 0;

   //Convert the frame to RGB24 so we can properly draw a circle
   circFrame = Convert(pFrame, PIX_FMT_RGB24);
    
   //Draw the circle and put it in the frame
   circFrame=circ.drawCircle(circFrame, iFrame);
    
    	//Find the codec information of the UTAH encoder 
	//so we can have access to the correct format (Likely RGB8 or RGB24)
   codec = avcodec_find_encoder(AV_CODEC_ID_UTAH);
   if (!codec) {
     fprintf(stderr, "Codec not found\n");
     exit(1);
   }
  
   c = avcodec_alloc_context3(codec);
   if (!c) {
     fprintf(stderr, "Could not allocate video codec context\n");
     exit(1);
   }

   c->width=width;
   c->height=height;
    //codec->pix_fmts[0] gets the first pixel format frome the set encoder
   c->pix_fmt = codec->pix_fmts[0];

   //Convert the circFrame to a frame in our utah format
   outFrame = Convert(circFrame, codec->pix_fmts[0]);

   
   if (avcodec_open2(c, codec, NULL) < 0) {
     fprintf(stderr, "Could not open codec\n");
     exit(1);
   }

   ret = avcodec_encode_video2(c, &pkt, outFrame, &got_output);
   if (ret < 0) {
     fprintf(stderr, "Error encoding frame\n");
     exit(1);
   }

   if (got_output) {
          
     fwrite(pkt.data, 1, pkt.size, pFile);
     av_free_packet(&pkt);
   }
 
  // Close file
  fclose(pFile);
  avcodec_close(c);
  av_free(c);
  av_free(circFrame);
  av_free(outFrame);
  //printf("\n");
}

int main(int argc, char *argv[]) {
  AVFormatContext *pFormatCtx = NULL;
  int i, videoStream;
  AVCodecContext *pCodecCtx = NULL;
  AVCodec *pCodec = NULL;
  AVFrame *pFrame = NULL;
  AVFrame *nFRAMERGB = NULL;
  AVPacket packet;
  int frameFinished;
  int numBytes;
  uint8_t *buffer = NULL;

  AVDictionary *optionsDict = NULL;
  struct SwsContext *sws_ctx = NULL;
  
  /*if(argc < 2) {
    printf("Please provide a movie file\n");
    return -1;
    }*/

  if(argc < 2 || strlen(argv[1]) < 5 || strcmp(argv[1] + strlen(argv[1])-4, ".jpg") != 0)
    {
      std::cout << "Please provide a .jpg image" << std::endl;
      return -1;
    }
  // Register all formats and codecs
  av_register_all();

  
  
  // Open video file
  if(avformat_open_input(&pFormatCtx, argv[1], NULL, NULL)!=0)
    return -1; // Couldn't open file
  
  // Retrieve stream information
  if(avformat_find_stream_info(pFormatCtx, NULL)<0)
    return -1; // Couldn't find stream information
  
  // Dump information about file onto standard error
  av_dump_format(pFormatCtx, 0, argv[1], 0);
  
  // Find the first video stream
  videoStream=-1;
  for(i=0; i<pFormatCtx->nb_streams; i++)
    if(pFormatCtx->streams[i]->codec->codec_type==AVMEDIA_TYPE_VIDEO) {
      videoStream=i;
      break;
    }
  if(videoStream==-1)
    return -1; // Didn't find a video stream
  
  // Get a pointer to the codec context for the video stream
  pCodecCtx=pFormatCtx->streams[videoStream]->codec;
  
  // Find the decoder for the video stream
  pCodec=avcodec_find_decoder(pCodecCtx->codec_id);
  if(pCodec==NULL) {
    fprintf(stderr, "Unsupported codec!\n");
    return -1; // Codec not found
  }
  // Open codec
  if(avcodec_open2(pCodecCtx, pCodec, &optionsDict)<0)
    return -1; // Could not open codec
  
  // Allocate video frame
  pFrame=avcodec_alloc_frame();
  
  // Read frames and save first five frames to disk
  i=0;
  int gotFrame = 0;
  while(av_read_frame(pFormatCtx, &packet)>=0 && gotFrame!=1) {
    // Is this a packet from the video stream?
    if(packet.stream_index==videoStream) {
      // Decode video frame
      avcodec_decode_video2(pCodecCtx, pFrame, &frameFinished,
&packet);
      
      // Did we get a video frame?
      if(frameFinished) {

// Save the frame to disk
        for(int j=0; j<300; j++){
          SaveFrame(pFrame, pCodecCtx->width, pCodecCtx->height, j);
        }

	gotFrame = 1;
      }
    }
    
    // Free the packet that was allocated by av_read_frame
    av_free_packet(&packet);
  }
  
  av_free(buffer);
  // Free the YUV frame
  av_free(pFrame);
  
  // Close the codec
  avcodec_close(pCodecCtx);
  
  // Close the video file
  avformat_close_input(&pFormatCtx);
  
  return 0;
}
/*
 * Copyright (c) 2001 Fabrice Bellard
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

/**
 * @file
 * libavcodec API use example.
 *
 * Note that libavcodec only handles codecs (mpeg, mpeg4, etc...),
 * not file formats (avi, vob, mp4, mov, mkv, mxf, flv, mpegts, mpegps, etc...). See library 'libavformat' for the
 * format handling
 * @example doc/examples/decoding_encoding.c
 */
 // tutorial01.c
//
// This tutorial was written by Stephen Dranger (dranger@gmail.com).
//
// Code based on a tutorial by Martin Bohme (boehme@inb.uni-luebeckREMOVETHIS.de)
// Tested on Gentoo, CVS version 5/01/07 compiled with GCC 4.1.1

// A small sample program that shows how to use libavformat and libavcodec to
// read video from a file.
//
// Use the Makefile to build all examples.
//
// Run using
//
// tutorial01 myvideofile.mpg
//
// to write the first five frames from "myvideofile.mpg" to disk in PPM
// format.
