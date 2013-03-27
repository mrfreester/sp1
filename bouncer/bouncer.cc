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

AVFrame * Convert(AVFrame * in, AVPixelFormat out_fmt)
 {
  AVFrame * pFrameRGB;
  uint8_t *buffer = NULL;
  struct SwsContext *sws_ctx = NULL;

  pFrameRGB=avcodec_alloc_frame();
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

    avpicture_fill((AVPicture *)pFrameRGB, buffer, out_fmt, in->width, in->height);

            sws_scale
        (
            sws_ctx,
            (uint8_t const * const *)in->data,
            in->linesize,
            0,
            in->height,
            pFrameRGB->data,
            pFrameRGB->linesize
        );

        pFrameRGB->width = in->width;
        pFrameRGB->height = in->height;
        pFrameRGB->format = (int)out_fmt;

        return pFrameRGB;
 }


 void SaveFrame(AVFrame *pFrame, int width, int height, int iFrame) {
  FILE *pFile;
  char szFilename[32];
  int y;

  AVCodec *codec;
  AVCodecContext *c= NULL;
   int ret, got_output;
  AVPacket pkt;
  AVFrame *frame1;
  frame1 = avcodec_alloc_frame();
  circlespace::circle circ;

  

  
  // Open file
  sprintf(szFilename, "frame%03d.utah", iFrame);
  pFile=fopen(szFilename, "wb");
  if(pFile==NULL)
    return;




   av_init_packet(&pkt);
        pkt.data = NULL;    // packet data will be allocated by the encoder
        pkt.size = 0;

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



 frame1 = Convert(pFrame, AV_PIX_FMT_RGB24);
    c->width=width;
    c->height=height;
    c->pix_fmt = codec->pix_fmts[0];


    
    frame1=circ.drawCircle(frame1, c,  iFrame);

    

    if (avcodec_open2(c, codec, NULL) < 0) {
      fprintf(stderr, "Could not open codec\n");
      exit(1);
    }

    ret = avcodec_encode_video2(c, &pkt, frame1, &got_output);
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
  //printf("\n");
}

int main(int argc, char *argv[]) {
  AVFormatContext *pFormatCtx = NULL;
  int i, videoStream;
  AVCodecContext *pCodecCtx = NULL;
  AVCodec *pCodec = NULL;
  AVFrame *pFrame = NULL;
  //AVFrame *pFrameRGB = NULL;
  AVFrame *nFRAMERGB = NULL;
  AVPacket packet;
  int frameFinished;
  int numBytes;
  uint8_t *buffer = NULL;

  AVDictionary *optionsDict = NULL;
  struct SwsContext *sws_ctx = NULL;
  
  if(argc < 2) {
    printf("Please provide a movie file\n");
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
  
  // Allocate an AVFrame structure
  //pFrameRGB=avcodec_alloc_frame();
  //if(pFrameRGB==NULL)
  //  return -1;
  
 
  
  // Assign appropriate parts of buffer to image planes in pFrameRGB
  // Note that pFrameRGB is an AVFrame, but AVFrame is a superset
  // of AVPicture
  
  
  // Read frames and save first five frames to disk
  i=0;
  while(av_read_frame(pFormatCtx, &packet)>=0) {
    // Is this a packet from the video stream?
    if(packet.stream_index==videoStream) {
      // Decode video frame
      avcodec_decode_video2(pCodecCtx, pFrame, &frameFinished,
&packet);
      
      // Did we get a video frame?
      if(frameFinished) {
// Convert the image from its native format to RGB
        //AVFrame *pFrameRGB = Convert(pFrame);
       

// Save the frame to disk


        
        for(int j=0; j<300; j++){
          SaveFrame(pFrame, pCodecCtx->width, pCodecCtx->height, j);
        }


	//pFrameRGB=nFRAMERGB;
      }
    }
    
    // Free the packet that was allocated by av_read_frame
    av_free_packet(&packet);
  }
  
  // Free the RGB image
  av_free(buffer);
  //av_free(pFrameRGB);
  
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
