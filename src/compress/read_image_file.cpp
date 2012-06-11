#include "dct.h"
/**
 * opmerking hier: de waardes worden met 257 vermenigvuldigd of gedeeld om zo de kleurwaardes binnen de 0-256 te brengen.
 * Het schijnt dat dit een bekend algoritme is, staat op internet meer over.
 **/
arma::mat pixelpacket2mat (const Magick::PixelPacket* input, PIX_COL kleur) {
  arma::mat output(8,8);
  switch(kleur) {
  case COLOR_BLUE :
    for(int i = 0; i < 64; i++)
      output[i] = (input[i].blue / 257);
  case COLOR_RED :
    for(int i = 0; i < 64; i++) 
      output[i] = (input[i].red / 257);
  case COLOR_GREEN :
    for(int i = 0; i < 64; i++)
      output[i] = (input[i].green / 257);
  }
  return output;
}

void mat2pixelpacket(const arma::mat& input, Magick::PixelPacket* output, PIX_COL kleur) {
  switch(kleur) {
  case COLOR_BLUE :
    for(int i = 0; i < 64; i++)
      output[i].blue = (input[i] * 257);
  case COLOR_RED :
    for(int i = 0; i < 64; i++)
      output[i].red = (input[i] * 257);
  case COLOR_GREEN :
    for(int i = 0; i < 64; i++)
      output[i].green = (input[i] * 257);
  }
}
