/*
 * dct.h
 *
 *  Created on: May 16, 2012
 *      Author: stephan
 */

#ifndef DCT_H_
#define DCT_H_
#include <armadillo>
#include <Magick++.h>
#include <fftw3.h>

typedef enum { D2= 0, D1XD1, EIGEN} DCT_T; // DCT type
std::list<int> dct(arma::mat& inp, DCT_T type);
arma::mat pixelpacket2mat (const Magick::PixelPacket* input);
static fftw_plan plan;

#endif /* DCT_H_ */
