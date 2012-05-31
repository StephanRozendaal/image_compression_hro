#include "file_io.h"
#include <fstream>
#include "file_description.pb.h"

void File_io::save_to_disk(std::string& fname) {
  my_file fl;
  std::fstream output(fname, std::ios::out | std::ios::binary);

  google::protobuf::ShutdownProtobufLibrary();
}

void File_io::load_from_disk(std::string& fname) {

}
