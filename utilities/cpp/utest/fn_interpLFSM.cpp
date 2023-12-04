#include "catch.hpp"
#include <range/v3/all.hpp>

#include <armadillo>
#include <iostream>
#include <ostream>

std::pair<arma::vec, arma::vec>
create_data(int n);
arma::uword
find_index(const arma::vec & coll, double freq_measure);

TEST_CASE("fn_interpLFSM")
{

  const int n   = 20;
  auto [xt, yt] = create_data(n);

  arma::vec xfine = arma::linspace<arma::vec>(50.01, 52.0, n * 100);
  arma::vec y1;

  interp1(xt, yt, xfine, y1, "linear");

  arma::vec yi_gabarit = {0.869474, 0.743158, 0.995789};

  arma::uword N = find_index(xfine, 50.5263);
  arma::uword M = find_index(xfine, 50.8421);
  arma::uword K = find_index(xfine, 50.2105);

  REQUIRE(
      arma::accu(std::abs(y1(N) - yi_gabarit[0])) == Approx(0.0).margin(0.001));
  REQUIRE(
      arma::accu(std::abs(y1(M) - yi_gabarit[1])) == Approx(0.0).margin(0.001));
  REQUIRE(
      arma::accu(std::abs(y1(K) - yi_gabarit[2])) == Approx(0.0).margin(0.001));
}

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
