/*utahdec.c*/
#include "libavutil/bprint.h"
#include "libavutil/imgutils.h"
#include "avcodec.h"
#include "bytestream.h"
#include "internal.h"
#include "utah.h"

static av_cold int UTAH_dec_init(AVCodecContext *avctx)
{
 UTAHContext *s = avctx->priv_data;

 avcodec_get_frame_defaults(&s->picture);
    avctx->coded_frame = &s->picture;

    return 0;
 
}

static int UTAH_decode_frame(AVCodecContext *avctx, void *data, int *got_frame, AVPacket *avpkt)
{

  const uint8_t *buf = avpkt->data;
    int buf_size       = avpkt->size;
    UTAHContext *s      = avctx->priv_data;
    AVFrame *picture   = data;
    AVFrame *p         = &s->picture;
    int height;
    int width;
    int ret;


    if (buf_size < 14) {
        av_log(avctx, AV_LOG_ERROR, "buf size too small (%d)\n", buf_size);
        return AVERROR_INVALIDDATA;
    }

    if (bytestream_get_byte(&buf) != 'U' ||
        bytestream_get_byte(&buf) != 'T' ||
	bytestream_get_byte(&buf) != 'A' ||
        bytestream_get_byte(&buf) != 'H' ) {
        av_log(avctx, AV_LOG_ERROR, "bad magic number\n");
        return AVERROR_INVALIDDATA;
    }

    height = bytestream_get_le32(&buf);
    width =  bytestream_get_le32(&buf);


     avctx->width  = width;
     avctx->height = height;

    avctx->pix_fmt =  AV_PIX_FMT_RGB8;

   
    if (p->data[0])
        avctx->release_buffer(avctx, p);

    p->reference = 0;
    if ((ret = ff_get_buffer(avctx, p)) < 0) {
        av_log(avctx, AV_LOG_ERROR, "get_buffer() failed\n");
        return ret;
    }
    
    p->pict_type = AV_PICTURE_TYPE_I;
    p->key_frame = 1;

  
    

  for (int row=0; row<height; row++){
    
    bytestream_get_buffer(&buf, &p->data[0][row*p->linesize[0]], width);

}

   
       

    *picture = s->picture;
    *got_frame = 1;

    return buf_size;

}
static av_cold int UTAH_dec_end(AVCodecContext *avctx)
{
  UTAHContext* c = avctx->priv_data;

    if (c->picture.data[0])
        avctx->release_buffer(avctx, &c->picture);
  return 0;
}
AVCodec ff_utah_decoder = {
    .name           = "utah",
    .type           = AVMEDIA_TYPE_VIDEO,
    .id             = AV_CODEC_ID_UTAH,
    .priv_data_size = sizeof(UTAHContext),
    .init           = UTAH_dec_init,
    .close          = UTAH_dec_end,
    .decode         = UTAH_decode_frame,
    .capabilities   = CODEC_CAP_DR1 /*| CODEC_CAP_DRAW_HORIZ_BAND*/,
    .long_name      = NULL_IF_CONFIG_SMALL("UTAH (Built for CS 3505 in U of U) image"),
};

