file(GLOB COMPRESSIE "./compress/*.cpp")
include_directories(${CMAKE_CURRENT_BINARY_DIR})
PROTOBUF_GENERATE_CPP(PROTO_SRCS PROTO_HDRS file_description.proto)

add_executable(dct_compress ${COMPRESSIE} ${PROTO_SRCS} ${PROTO_HDRS})
target_link_libraries(dct_compress ${PTHREADS_LIBRARY} ${ImageMagick_LIBRARIES} ${ARMADILLO_LIBRARIES} ${FFTW_LIBRARIES} ${PROTOBUF_LIBRARIES})
