#include "dct.h"

arma::mat pixelpacket2mat (const Magick::PixelPacket* input, PIX_COL kleur) {
  arma::mat output(8,8);
  switch(kleur) {
  case COLOR_BLUE :
    for(int i = 0; i < 64; i++)
      output[i] = input[i].blue;
  case COLOR_RED :
    for(int i = 0; i < 64; i++) 
      output[i] = input[i].red;
  case COLOR_GREEN :
    for(int i = 0; i < 64; i++)
      output[i] = input[i].green;
  }
  return output;
}
