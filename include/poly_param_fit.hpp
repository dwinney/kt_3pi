// Routines to fit a lineshape to a polynomial (z, theta) expansion around center of dalitz plot.
//
// Dependencies: dalitz.cpp
//
// Author:       Daniel Winney (2019)
// Affiliation:  Joint Physics Analysis Center (JPAC)
// Email:        dwinney@iu.edu
// ---------------------------------------------------------------------------

#ifndef _POLY_FIT_
#define _POLY_FIT_

#include <vector>
#include "dalitz.hpp"
#include "aux_math.hpp"
#include <iomanip>

using std::vector;
using std::setw;

class poly_param_fit : public dalitz
{
protected:
  // Polynomial expansion parameters up to order 5/2 in z
  int N_params = 2;
  double Norm = 1., alpha = 0., beta = 0., gamma = 0., delta = 0.;
  double scale = 1.e-3;

  // For integration
  int n = 60; // Number of integration points
  int N_int(){
    return n;
  };

  bool S_WG_GENERATED, T_WG_GENERATED;
  vector<double> s_wgt, s_abs;
  vector< vector<double>> t_wgt, t_abs;
  void generate_s_weights();
  void generate_t_weights(vector<double> s);
  void generate_weights();

// ---------------------------------------------------------------------------
public:
  poly_param_fit(complex<double> (*my_amp) (double, double)) : dalitz(my_amp){};

  void set_params(int n, double *par);
  void print_params(int a = 0);
  void set_integration_points(int n);

  double F_poly(double s, double t);

  double kin_kernel(double s, double t); // Kinematic Kernal in dalitz region integral
  double chi_squared(); // Chi-squared between input line-shape and polynomial.
  double dalitz_area();

  void fit_params();
};

#endif
