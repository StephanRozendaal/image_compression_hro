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
  if(argc >= 1)
    fname = argv[1];
  else {
    std::cout << "no filename specified" << std::endl;
    return -1;
  }
  //laad de file naar my_file object.
  std::unique_ptr<my_file> fl = io.load_from_disk(fname);
  //heeft my_file een header?
  if(!fl->has_hdr()) {
    std::cout << "file has no header" << std::endl;
    return -1;
  }
  my_file_header* hdr = fl->mutable_hdr();
  //heeft deze file wel geencodeerde blokken?
  if(fl->blcks_size() <= 0) {
    std::cout << "file has no encoded blocks" << std::endl;
    return -1;
  }
  //haal kleur X op van positie Y uit de block-datastructuur.
  auto func = [&](int i, PIX_COL color) {
    const my_encoded_block blck = fl->blcks(i);
    std::list<int32_t> list;
    if(color == COLOR_BLUE) {
      for(auto it = blck.color_blue().begin(); it != blck.color_blue().end(); it++)
      	list.push_back(*it);
    } else if(color == COLOR_RED) {
      for(auto it = blck.color_red().begin(); it != blck.color_red().end(); it++)
      	list.push_back(*it);
    } else if(color == COLOR_GREEN) {
      for(auto it = blck.color_green().begin(); it != blck.color_green().end(); it++)
     	list.push_back(*it);
    } else {}
   
    return list;
  };
  int no_blcks = hdr->no_blocks();
  int no_rows = hdr->no_rows();
  int no_columns = hdr->no_columns();
  Image out_image(Geometry((no_blcks/no_rows)*8, (no_blcks/no_columns)*8), "white");
  out_image.type(TrueColorType);
  out_image.depth(8);
  out_image.modifyImage();
  Pixels image(out_image);

  std::list<int32_t> lst;
  arma::mat mat;
  int blck_count = 0;
  for(int x = 0; x+8 <= out_image.columns(); x+=8) {
    for(int y = 0; y+8 <= out_image.rows(); y+=8) {
      PixelPacket* cache = image.set(x, y, 8, 8);
      lst = func(blck_count, COLOR_BLUE);
      mat = reverse_dct(lst, type);
      mat2pixelpacket(mat, cache, COLOR_BLUE);
      lst = func(blck_count, COLOR_RED);
      mat = reverse_dct(lst, type);
      mat2pixelpacket(mat, cache, COLOR_RED);
      lst = func(blck_count, COLOR_GREEN);
      mat = reverse_dct(lst, type);
      mat2pixelpacket(mat, cache, COLOR_GREEN);
      blck_count++;
      image.sync();
    }
    
  }
  std::cout << "image grootte: " << (no_blcks/no_columns)*8 << "bij " << (no_blcks/no_rows)*8 << std::endl;

  std::cout << "totaal: " << no_blcks << "rijen: " << no_rows << "kolommen: " << no_columns << std::endl;
  //uiteindelijk: laat het plaatjes zien met image.display().
  out_image.display();
}
