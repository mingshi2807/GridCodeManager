#include <range/v3/all.hpp> //use range-v3 lib c++17

#include <armadillo>
#include <iostream>
#include <ostream>

/* #include <range/v3/view/zip.hpp> */

std::pair<arma::vec, arma::vec>
create_data(int n)
{
  const double xmax = 52.0;
  const double s    = 0.05;
  const double fn   = 50.0;
  const double g    = 1.0 / (s * fn);
  const double f1   = 50.2;
  const double Pmin = 0.6;

  arma::vec x = arma::linspace(50.0, xmax, n);

  // Adding the condition for power (y) values
  arma::vec y = arma::ones(n); // Initialize power with Pmax normalized value

  try {
    // Update y values only for elements where x is greater than or equal to f1
    // Traditional for loop
    /* for (int i = 0; i < n; ++i) { */
    /*   if (x.at(i) >= f1) { */
    /*     y.at(i) = 1.0 + g * (f1 - x.at(i)); */
    /*   } */
    /* } */

    auto mask = [&](double xi)
    { return (xi > f1) ? 1.0 + g * (f1 - xi) : 1.0; };

    auto result = ranges::views::zip_with(mask, x);
    ranges::copy(result, y.begin());

    // Check if any y value is less than Pmin threshold
    if (min(y) < Pmin) {
      throw std::runtime_error(
          "Linear variation cannot be confirmed for y < Pmin. Set y to a "
          "default value if no instruction");
    }
  }
  catch (const std::runtime_error & e) {
    std::cerr << "Error: " << e.what() << std::endl;
    // Handle the error. For example: set y values to a default value
    /* y = ones(n); */
  }

  return {x, y};
}

arma::uword
find_index(const arma::vec & coll, double freq_measure)
{
  // Absolute differences between freq_measure and each element
  arma::vec abs_diff = arma::abs(coll - freq_measure);

  // Find the index of the minimum absolute difference
  arma::uword index = abs_diff.index_min();

  return index;
}

int
main()
{

  const int n   = 20;
  auto [xt, yt] = create_data(n);
  const double fmeasure =
      50.5105217; // simulate mHz accuracy Siemens Q200 Power Analyzer measure

  arma::vec xfine = arma::linspace<arma::vec>(50.01, 52.0, n * 100);
  arma::vec y1;

  /* interp1(xt, yt, xfine, y1, "linear"); */
  /* interp1(xt, yt, xfine, y1, "quadratic"); */
  interp1(xt, yt, xfine, y1, "*linear");
  /* interp1(xt, yt, xfine, y1, "nearest"); */
  std::cout << "Y measured : " << y1(find_index(xfine, fmeasure)) << '\n';

  for (const auto & [x_val, y_val] : ranges::views::zip(xt, yt)) {
    std::cout << "x: " << x_val << ", y: " << y_val << std::endl;
  }
  /* std::cout << "=====================" << std::endl; */
  /* std::cout << "Interpolated data " << '\n'; */
  /* for (const auto &[x_val, y_val] : ranges::views::zip(xfine, y1)) { */
  /*   std::cout << "x: " << x_val << ", y: " << y_val << std::endl; */
  /* } */

  return 0;
}

/* interp1(X, Y, XI, YI) */
/* given a 1D function specified in vectors X (locations) and Y (values), */
/* generate vector YI containing interpolated values at given locations XI */

/* Todo Add unit test  */
/* Add temporal constrains bound from Enedis file */
