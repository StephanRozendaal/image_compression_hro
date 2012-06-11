#define ARMA_USE_CXX11
#include <armadillo>
#include <Magick++.h>
#include <iostream>
#include "dct.h"
#include "file_io.h"

using namespace Magick;
int main(int argc, char** argv) {
  std::string fname;
  File_io io;
  DCT_T type = D2;
  InitializeMagick(*argv);
  Image out_image;
  out_image.magick("RGB");
  try {
    out_image.read(argv[1]);
  } catch ( std::exception er) {
    std::cerr << "exception: " << er.what() << std::endl;
    return -1;
  }
  //check of er een output naam is.
  if(argc > 2)
    fname = argv[2];
  else
    fname = "output.steef";
  const PixelPacket *cache = 0;
  Pixels image(out_image);
  /**
   * waarde cache heeft een PixelPacket array van 64 posities.
   * de 2 for-loopen itereren over de hele input image.
   **/
  std::list<int> block_red, block_blue, block_green;
  int x,y;
  for(x = 0; x+8 <= out_image.columns(); x+=8) {
    for(y = 0; y+8 <= out_image.rows(); y+=8) {
      cache = image.getConst(x,y,8,8); // haal 8x8 pixels op.
      auto mat_red = pixelpacket2mat(cache, COLOR_RED); // zet pixels van kleur rood om in arma::mat
      auto mat_blue = pixelpacket2mat(cache, COLOR_BLUE);
      auto mat_green = pixelpacket2mat(cache, COLOR_GREEN);
      block_red = dct(mat_red, type); // voer DCT berekening op arma::mat uit
      block_blue = dct(mat_blue, type);
      block_green = dct(mat_green, type);
      //toevoegen aan io
      io.add_block(block_red, block_green, block_blue);
    }}
  //io.save_to_disk(fname, block_red.size(), image.rows(), image.columns);
  io.save_to_disk(fname, (x/8)*(y/8), y/8, x/8);
  fftw_cleanup();
}
