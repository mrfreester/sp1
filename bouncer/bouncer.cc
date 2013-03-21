/*
 *Once we get linked with ffmpeg, we'll
 * likely need to use avformat_open_input().
 * to open a jpg image to an avframe for editing.
 *
 */
#include <iostream>
#include <string>

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

int main( int argc, const char* argv[] )
{
	// Prints each argument on the command line.
	for( int i = 0; i < argc; i++ )
	{
	  cout << "arg" << i << ": " << argv[i] << endl;
	}

	//Get path to jpg passed in
	string path;
	path += argv[1];

	cout << "path = " << path << endl;

	AVFormatContext *pFormatCtx = NULL;
	int i, videoStream;
	AVCodecContext *pCodecCtx = NULL;
	AVCodec *pCodec = NULL;
	AVFrame *pFrame = NULL;
	AVFrame *pFrameRGB = NULL;
	AVPacket packet;
	int frameFinished;
	int numBytes;
	uint8_t *buffer = NULL;

	AVDictionary *optionsDict = NULL;
	struct SwsContext *sws_ctx = NULL;

	av_register_all();

	//open jpg
	if(avformat_open_input(&pFormatCtx, argv[1], NULL, NULL)!=0)
	  return -1;
	
	pCodecCtx = pFormatCtx->streams[0]->codec;

	pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
	if(!pCodec)
	  cout << "Codec not found" << endl;

	if(avcodec_open2(pCodecCtx, pCodec, &optionsDict)<0)
	  cout << "Couldn't open codec" << endl;

	pFrame = avcodec_alloc_frame();

	pFrameRGB = avcodec_alloc_frame();
	if(pFrameRGB == NULL)
	  return -1;

	numBytes = avpicture_get_size(PIX_FMT_RGB24, pCodecCtx->width, pCodecCtx->height);

	buffer=(uint8_t *)av_malloc(numBytes*sizeof(uint8_t));

	/*
	sws_ctx = sws_getContext(pCodecCtx->width,
	   pCodecCtx->height,
	   pCodecCtx->pix_fmt,
	   pCodecCtx->width,
	   pCodecCtx->height,
	   PIX_FMT_RGB24,
	   SWS_BILINEAR,
	   NULL,
	   NULL,
	   NULL
	   );
	*/
	
	avpicture_fill((AVPicture *)pFrame, buffer, PIX_FMT_RGB24, pCodecCtx->width, pCodecCtx->height);

	
	int framesNumber = 0;
	while (av_read_frame(pFormatCtx, &packet) >= 0)
	  {
	    if(packet.stream_index != 0)
	      continue;

	    int ret = avcodec_decode_video2(pCodecCtx, pFrame, &frameFinished,&packet);
	    if (ret > 0)
	      {
		pFrame->quality = 1;
		break;
	      }
	    else {
	      cout << "Woah when decoding frame" << endl;
	    }
	  }


	return 0;
}

