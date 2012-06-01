#pragma once
#include <list>
#include "file_description.pb.h"

class File_io {
  std::list<my_encoded_block> blocks; // lijst ge-encodeerde blokken
 public:
  File_io() {

  }
  ~File_io() {

  }
  void set_header();
  void add_block(std::list<int>& blck_red, std::list<int>& blck_green, std::list<int>& blck_blue);
  void save_to_disk(std::string& fname, int no_blocks, int no_rows, int no_columns);
  void load_from_disk(std::string& fname);
};
