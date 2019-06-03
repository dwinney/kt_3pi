// General purpose classes for Dalitz plot generation.
// for fitting to polynomial expansion see param_fit.hpp
//
// Dependencies: constants.hpp aux_math.hpp
//
// Author:       Daniel Winney (2019)
// Affiliation:  Joint Physics Analysis Center (JPAC)
// Email:        dwinney@iu.edu
// ---------------------------------------------------------------------------

#ifndef _DALITZ_
#define _DALITZ_

#include "constants.hpp"
#include "aux_math.hpp"

#include <vector>
#include <iostream>
#include <fstream>
#include <iomanip>

#include "TGraph2D.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TError.h"

using std::vector;
using std::setw;
using std::cout;
using std::endl;

//-----------------------------------------------------------------------------
// Define a Dalitz plot object with some other amplitude object containing a model.
// Object argument must inherit from 'amplitude' class and be callable with the signature:
// with the signature:
//
//  complex<double> operator() (double, double)
//  or
//  double operator() (double,double)
//
// Initiate a dalitz plot object with:
// model_amp my_model;
// dalitz<model_amp> my_dalitz(my_model);
//-----------------------------------------------------------------------------

template <class T>
class dalitz
{
protected:
  T amp;
  double normalization = 32. * pow(2.* M_PI * amp.get_decayMass(), 3.);
  double offset = 0.00001;
//-----------------------------------------------------------------------------
// For integration
  int n = 60; // Number of integration points
  int N_int(){
    return n;
  };

  bool S_WG_GENERATED, T_WG_GENERATED;
  vector<double> s_wgt, s_abs;
  vector< vector<double> > t_wgt, t_abs; //"two-dimensional" vectors
  void generate_s_weights();
  void generate_t_weights(vector<double> s);
  void generate_weights();

  double dalitz_area();
//-----------------------------------------------------------------------------
public:
  // Default Constructor
  dalitz(T& my_amp) : amp(my_amp){};

//--------------------------------------------------------------------------
  // Double differential cross section
  double d2Gamma(double s, double t);

//--------------------------------------------------------------------------
  void plot();
  void set_integration_points(int n);
};

//-----------------------------------------------------------------------------
#endif
