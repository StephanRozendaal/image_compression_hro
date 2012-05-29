#define ARMA_USE_CXX11
#include <armadillo>
#include <Magick++.h>
#include <iostream>
#include "dct.h"

using namespace Magick;
int main(int argc, char** argv) {

  DCT_T type = D1XD1;
  // char* input = {"alien.png"};
  InitializeMagick(*argv);
  Image image;
  try {
    image.read(argv[1]);
  } catch ( std::exception er) {
    std::cerr << "exception: " << er.what() << std::endl;
    return -1;
  }
  const PixelPacket *cache = 0;

  /**
   * waarde cache heeft een PixelPacket array van 64 posities.
   * de 2 for-loopen itereren over de hele input image.
   **/
  for(int x = 0; x+8 < image.columns(); x+=8) {
    for(int y = 0; y+8 < image.rows(); y+=8) {
      cache = image.getConstPixels(x,y,8,8);
      auto temp = pixelpacket2mat(cache);
      dct(temp, type);
    }}

  //fftw_destroy_plan(plan);
  fftw_cleanup();
}
