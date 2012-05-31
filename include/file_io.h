#pragma once
#include "file_description.pb.h"

class File_io {
  File_io() {

  }
  ~File_io() {

  }
  void save_to_disk(std::string& fname);
  void load_from_disk(std::string& fname);
};
