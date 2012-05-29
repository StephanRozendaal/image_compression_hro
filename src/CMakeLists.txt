 file(GLOB COMPRESSIE "./compress/*.cpp")
add_executable(dct_compress ${COMPRESSIE})
target_link_libraries(dct_compress ${ImageMagick_LIBRARIES} ${ARMADILLO_LIBRARIES} ${FFTW_LIBRARIES})
