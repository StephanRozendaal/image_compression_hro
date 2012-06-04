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
  auto func = [&](int i, PIX_COL kleur) {
    my_encoded_block* blck = fl->mutable_blcks(i);
    std::unique_ptr<std::list<int>> list;
    int* col;
    switch(kleur) {
    case COLOR_BLUE :
      col = blck->color_blue();
      list = new std::list<int>(col, col+sizeof(col) / sizeof(int));
    case COLOR_RED:
      col = blck->color_red();
      list = std::list<int>(col, col+sizeof(col) / sizeof(int));
    case COLOR_GREEN:
      col = blck->color_green();
      list = std::list<int>(col, col+sizeof(col) / sizeof(int));
    case default:
      list = nullptr;
    }
    return list;
  };

}
