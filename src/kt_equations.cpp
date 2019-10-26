// Class which calculates and assembles the KT solution
//
// Author:       Daniel Winney (2019)
// Affiliation:  Joint Physics Analysis Center (JPAC)
// Email:        dwinney@iu.edu
// ---------------------------------------------------------------------------

#include "kt_equations.hpp"

// ----------------------------------------------------------------------------
// Take in a pointer to a previous iteration (because all previous isobars are required too get next one)
// Produces a new iteration with the new values.
iteration kt_equations::iterate(iteration * prev)
{
  vector<isobar> isobars;

  // sum over spins
  for (int i = 0; 2*i+1 <= options.max_spin; i++)
  {
    cout << " -> Calculating isobar with spin (" << 2*i+1 << "/" << options.max_spin << ")... " << endl;
    isobars.push_back(iterate_isobar(prev, i));

    // add a space in the terminal output if theres more isobars for aesthetic purposes :)
    if (2*(i+1)+1 <= options.max_spin)
    {
    cout << endl;
    }
  }

  iteration next(prev->N_iteration + 1, isobars);

  return next;
};

// ----------------------------------------------------------------------------
// This functions calculates the next iteration of the jth isobar.
isobar kt_equations::iterate_isobar(iteration * prev, int bar_index)
{
  // Dont forget to pass the pointer to the dispersion intergral!!
  disp.pass_iteration(prev);

  // each isobar has n subtractions that need to be solved for independently
  // here lambda = 1 and I = 1 are fixed
  isobar next(1, 2*bar_index+1, 1, options, kinematics);

  // Calculate each 'fundamental solution' in the basis of subtractions
  for (int sub_index = 0; sub_index <= prev->isobars[bar_index].n_subs; sub_index++)
  {
      cout << " --> Calculating fundamental solution of order (" << sub_index << "/" << prev->isobars[bar_index].n_subs << ")... " << endl;

      // exclude a small interval around the singulartiy at pseudo_threshold
      // the interpolation will make up for it
      double a = kinematics.pseudo_threshold();
      double frac = ((a - exc) - (sthPi + EPS )) / (options.interp_cutoff - sthPi - EPS);
      int num = int(frac * double(interpolation::N_interp));

      vector<double> s;
      vector<complex<double>> above, below;

      // store values from threshold to a
      for (int i = 0; i < num; i ++)
      {
        double s_i = (sthPi + EPS) + double(i) * ((a - exc) - (sthPi + EPS)) / double(num - 1);
        s.push_back(s_i);

        // above unitarity cut (ieps = +1)
        complex<double> ab = poly(sub_index, s_i, +1) + disp(bar_index, sub_index, s_i, +1);
        ab *= prev->isobars[bar_index].omega(s_i, +1);
        above.push_back(ab);

        // below unitarity cut (ieps = -1)
        complex<double> be = poly(sub_index, s_i, -1) + disp(bar_index, sub_index, s_i, -1);
        be *= prev->isobars[bar_index].omega(s_i, -1);
        below.push_back(be);
      }

      // and then from a to the cutoff
      for (int i = 0; i < (interpolation::N_interp - num) ; i++)
      {
        double s_i = (a + exc) + double(i) * (options.interp_cutoff - (a + exc)) / double(interpolation::N_interp - num - 1);
        s.push_back(s_i);

        complex<double> ab = poly(sub_index, s_i, +1) + disp(bar_index, sub_index, s_i, +1);
        ab *= prev->isobars[bar_index].omega(s_i, +1);
        above.push_back(ab);

        complex<double> be = poly(sub_index, s_i, -1) + disp(bar_index, sub_index, s_i, -1);
        be = prev->isobars[bar_index].omega(s_i, -1);
        below.push_back(be);
      }

      subtraction nth(sub_index, s, above, below);
      next.subtractions.push_back(nth);
  }

  return next;
};
