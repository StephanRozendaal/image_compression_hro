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
  std::unique_ptr<my_file> fl = io.load_from_disk(fname);
  if(!fl->has_hdr()) {
    std::cout << "file has no header" << std::endl;
    return -1;
  }
  my_file_header* hdr = fl->mutable_hdr();
  if(fl->blcks_size() <= 0) {
    std::cout << "file has no encoded blocks" << std::endl;
    return -1;
  }
  int no_blcks = hdr->no_blocks();
  int no_rows = hdr->no_rows();
  int no_columns = hdr->no_columns();
  //haal kleur X op van positie Y uit de block-datastructuur.
  auto func = [&](int i, PIX_COL color) {
    my_encoded_block* blck = fl->mutable_blcks(i);
    std::list<int> list;
    switch(color) {
    case COLOR_BLUE :
      std::copy(blck->color_blue().begin(), blck->color_blue().end(), list.begin());
    case COLOR_RED:
      std::copy(blck->color_red().begin(), blck->color_red().end(), list.begin());
    case COLOR_GREEN:
      std::copy(blck->color_green().begin(), blck->color_green().end(), list.begin());
    }
    return std::move(list);
  };
  std::list<int> blauw = func(1, COLOR_BLUE);
  std::cout << "block size: " << blauw.size() << std::endl;

  //uiteindelijk: laat het plaatjes zien met image.display().
}
