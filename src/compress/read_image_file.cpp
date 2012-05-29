#include "dct.h"

arma::mat pixelpacket2mat (const Magick::PixelPacket* input) {
  arma::mat output(8,8);
  for(int i = 0; i < 64; i++) {
    output[i] = input[i].blue;
  }
  return output;
}
