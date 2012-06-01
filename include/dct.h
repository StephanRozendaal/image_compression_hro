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
typedef enum { COLOR_BLUE = 0, COLOR_RED, COLOR_GREEN} PIX_COL; // Pixel kleur type
std::list<int> dct(arma::mat& inp, DCT_T type);
arma::mat pixelpacket2mat (const Magick::PixelPacket* input, PIX_COL kleur);
static fftw_plan plan;

#endif /* DCT_H_ */
