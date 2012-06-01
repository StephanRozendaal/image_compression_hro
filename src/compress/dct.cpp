/*
 * dct.cpp
 *
 *  Created on: May 16, 2012
 *      Author: stephan
 */
#include "dct.h"
#include <armadillo>
#include <cmath>
#include <fftw3.h>
static int comprr_ar [] = {
0, 
1, 8, 
16, 9, 2, 
3, 10, 17, 24, 
32, 25, 18, 11, 4, 
5, 12, 19, 26, 33, 40, 
48, 41, 34, 27, 20, 13, 6, 
7, 14, 21, 28, 35, 42, 49, 56, 
57, 50, 43, 36, 29, 22, 15, 
23, 30, 37, 44, 51, 58, 
59, 52, 45, 38, 31, 
39, 46, 53, 60, 
61, 54, 47, 
55, 62, 
63};

arma::mat quantisation_mat(int quality) {
  arma::mat mat;
  if (quality == 50) {
    mat << 16 << 11 << 10 << 16 << 24 << 40 << 51 << 61 << arma::endr << 12
	<< 12 << 14 << 19 << 26 << 58 << 60 << 55 << arma::endr << 14
	<< 13 << 16 << 24 << 40 << 57 << 69 << 56 << arma::endr << 14
	<< 17 << 22 << 29 << 51 << 87 << 80 << 62 << arma::endr << 18
	<< 22 << 37 << 56 << 68 << 109 << 103 << 77 << arma::endr << 24
	<< 35 << 55 << 64 << 81 << 104 << 113 << 92 << arma::endr << 49
	<< 64 << 78 << 87 << 103 << 121 << 120 << 101 << arma::endr
	<< 72 << 92 << 95 << 98 << 112 << 100 << 103 << 99
	<< arma::endr;
  }
  return mat;
}
#define COEFFS(Cu,Cv,u,v) {				\
    if (u == 0) Cu = 1.0 / sqrt(2.0); else Cu = 1.0;	\
    if (v == 0) Cv = 1.0 / sqrt(2.0); else Cv = 1.0;	\
  }
arma::mat compute_dct(arma::mat& inp) {
  arma::mat out(8,8);
  for(int v = 0; v < 8; v++) {
    for(int u = 0; u < 8; u++) {
      double Cu, Cv, z = 0.0;
      COEFFS(Cu,Cv,u,v);
      for(int y = 0; y < 8; y++) {
	for(int x = 0; x < 8; x++) {
	  double s, q;
	  s = inp.at(x,y);
	  q = s * cos((double)(2*x+1) * (double)u * M_PI/16.0) *
	    cos((double)(2*y+1) * (double)v * M_PI/16.0);
	  z += q;
	}}
      out.at(u,v) = 0.25 * Cu * Cv * z;
    }}
  return out;
}

std::list<int> dct(arma::mat& inp, DCT_T type) {
  //level-off van pixel-blok 128 afhalen van elk element.
  arma::mat M(inp);
  for (arma::mat::iterator it = M.begin(); it != M.end(); it++)
    *it -= 128;
  //lambda functie met keuze voor type transformatie.
  auto func = [&]() {
    switch(type) {
      /** ************************************************************************
       *  doe de DCT transform gelijk als 2D, output is matrix T.
       **/
    case D2 : {
      arma::mat T(8, 8);
      plan = fftw_plan_r2r_2d(8, 8, M.memptr(), T.memptr(),
			      FFTW_REDFT00, FFTW_REDFT00, FFTW_MEASURE);
      fftw_execute(plan);
      fftw_destroy_plan(plan);
      return T;
    }
    /** *************************************************************************
     *Doe de DCT transform in 2 stappen, eerst vertical en daarna horizontaal als 1D transformaties, combineer de eindresultaten.
     **/
    case D1XD1 : {
      arma::mat Ttemp(8,8);
      for(int i = 0; i < 8; i++) {
	plan = fftw_plan_r2r_1d(8, M.colptr(i), Ttemp.colptr(i), FFTW_REDFT10, FFTW_MEASURE);
	fftw_execute(plan);
	fftw_destroy_plan(plan);
      }
      Ttemp = trans(Ttemp);
      arma::mat T(8,8);
      for(int i = 0; i < 8; i++) {
	plan = fftw_plan_r2r_1d(8, Ttemp.colptr(i), T.colptr(i), FFTW_REDFT10, FFTW_MEASURE);
	fftw_execute(plan);
	fftw_destroy_plan(plan);
      }
      Ttemp = trans(Ttemp);
      T = T + Ttemp;
      return T;
    }
    /** *************************************************************************
     *  eigen DCT.
     **/
    case EIGEN : {
      arma::mat T = compute_dct(M);
      return T;
    }
    default:
    break;
    }};
  //voer de lambda-functie uit
  arma::mat T = func();
  //matrix Tt is T getransponeerd
  //arma::mat Tt = arma::trans(T);
  //matrix D bevat 64 DCT coefficienten.
  // arma::mat D = T * M * Tt;
  arma::mat D = T;
  //geef een quantization matrix voor een bepaalde kwaliteit.
  arma::mat Q = quantisation_mat(50);
  // bouw matrix C op door af te ronden van (D/Q).
  arma::imat C(8, 8);
  arma::mat::iterator Dit = D.begin();
  arma::imat::iterator Cit = C.begin();
  for (arma::mat::iterator Qit = Q.begin(); Dit != D.end();
       Dit++ && Qit++ && Cit++) {
    *Cit = round((*Dit / *Qit));
  }
  //comprimeer matrix C door nullen op het eind weg te laten.
  std::list<int> output;
  bool first_not_zero = false;
  for(int i = 63; i >= 0; i--) { // begin aan het eind van het matrix
    if(C[comprr_ar[i]] != 0 && !first_not_zero) //is geen waarde nul
      first_not_zero = true;
    if(first_not_zero) // waardes nul gehad
      output.push_front((int)C[comprr_ar[i]]); // waardes voorin de list pushen zodat de waardes linksboven in de matrix als eerste komen.
  }
  // std::cout << "matrix M" << std::endl << M << "matrix T" << std::endl << T
  //  	    << "matrix T inverse:" << std::endl << Tt << "matrix D:"
  //  	    << std::endl << D << "matrix C:" << std::endl << C << std::endl;
  //std::cout << "matrix C: " << C << std::endl;
  return output;
  //output naar het scherm, voor controle.
  //   std::cout.precision(5);
}
