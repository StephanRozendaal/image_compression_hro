#include "file_io.h"
#include <fstream>
#include <iostream>
#include <list>
#include "file_description.pb.h"

void File_io::add_block(std::list<int>& blck_red, std::list<int>& blck_green, std::list<int>& blck_blue) {
  my_encoded_block temp_block;
  //eerst kleur rood.
  for(auto it = blck_red.begin(); it != blck_red.end(); it++) {
    temp_block.add_color_red(*it);
  }
  //daarna kleur groen.
  for(auto it = blck_green.begin(); it != blck_green.end(); it++) {
    temp_block.add_color_green(*it);
  }
  //als laatste kleur blauw.
  for(auto it = blck_blue.begin(); it != blck_blue.end(); it++) {
    temp_block.add_color_blue(*it);
  }
  blocks.push_back(temp_block);
}

void File_io::save_to_disk(std::string& fname, int no_blocks, int no_rows, int no_columns) {
  my_file fl;
  my_file_header* hdr;
  hdr = fl.mutable_hdr();
  hdr->set_no_blocks(no_blocks);
  hdr->set_no_rows(no_rows);
  hdr->set_no_columns(no_columns);

  std::fstream output(fname, std::ios::out | std::ios::binary);
 
  for(auto it = blocks.begin(); it != blocks.end(); it++) {
    auto temp_block = fl.add_blcks();
    *temp_block = *it;
  }
  if(!fl.SerializeToOstream(&output)) {
    std::cerr << "failed to write to file" << std::endl;
  }
  google::protobuf::ShutdownProtobufLibrary();
  output.close();
}

void File_io::load_from_disk(std::string& fname) {

}
