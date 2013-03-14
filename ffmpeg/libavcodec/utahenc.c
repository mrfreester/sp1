/*utahenc*/
#include "libavutil/avassert.h"
#include "avcodec.h"
#include "bytestream.h"
#include "bmp.h"
#include "internal.h"
#include "utah.h"

static av_cold int UTAH_enc_init(AVCodecContext *avctx){
  UTAHContext *s = avctx->priv_data;

  avcodec_get_frame_defaults(&s->picture);
  avctx->coded_frame = &s->picture;
  // av_log(avctx, AV_LOG_INFO, "Inside Utah_enc_init\n");
  return 0;
}
static int UTAH_encode_frame(AVCodecContext *avctx, AVPacket *pkt, const AVFrame *pict, int *got_packet)
{
  UTAHContext *s = avctx->priv_data;
  AVFrame * const p = &s->picture;
  uint8_t *buf;
  *p = *pict;
  p->pict_type = AV_PICTURE_TYPE_I;
  p->key_frame = 1;
  int ret;
  int height;
  int width;
  int n_bytes;
  height = avctx->height;
  width = avctx->width;
  n_bytes =12+ height * width;
  // printf("Width: %d, height: %d, Linesize: %d\n",avctx->width, avctx->height);
  
 if ((ret = ff_alloc_packet2(avctx, pkt, n_bytes)) < 0)
        return ret;
 
  buf=pkt->data;
  bytestream_put_byte(&buf, 'U');
  bytestream_put_byte(&buf, 'T');
  bytestream_put_byte(&buf, 'A');
  bytestream_put_byte(&buf, 'H');
  bytestream_put_le32(&buf, height);
  bytestream_put_le32(&buf, width);


 

  for (int row=0; row<height; row++){
    for(int col=0;col<width; col++){
      uint8_t *mypixel = p->data[0]+row*p->linesize[0]+col;
       bytestream_put_byte(&buf, mypixel[0]);

    }
  }

  // av_log(avctx, AV_LOG_INFO, "Inside Utah_encode_frame\n");
  pkt->flags |= AV_PKT_FLAG_KEY;
  *got_packet = 1;
  return 0;
}
AVCodec ff_utah_encoder = {
    .name           = "utah",
    .type           = AVMEDIA_TYPE_VIDEO,
    .id             = AV_CODEC_ID_UTAH,
    .priv_data_size = sizeof(UTAHContext),
    .init           = UTAH_enc_init,
    .encode2        = UTAH_encode_frame,
    .capabilities   = CODEC_CAP_INTRA_ONLY,
    .pix_fmts       = (const enum AVPixelFormat[]){
        AV_PIX_FMT_RGB8, AV_PIX_FMT_NONE
    },
    .long_name = NULL_IF_CONFIG_SMALL("UTAH (Built for CS 3505 in U of U) image"),
};
