#ifndef ROOTS_HPP
#define ROOTS_HPP

#include <print>
#include <regex>
#include <string>
#include <string_view>
using std::string;
using namespace exprtk;
// #define EPSILON 10e-5
static constexpr double EPSILON = 10e-5;
namespace roots {

inline std::string str_interations{}, eq{}, epsilon_str{};
inline int number_of_iterations;
// calculate midpoint
double calculateMidpoint(double &a, double &b);
// inline double epsilon = EPSILON; // default epsilon value
string calculateNumOfInterations(double &a, double &b,
                                 double epsilon = EPSILON);

// Perform Bisection Method
string bisectionMethod(std::string_view equation, double &a, double &b,
                       double epsilon = EPSILON);

// Lamda
inline auto scientificToFloat = [](const string ep) {
  using std::regex;
  regex exp_to_float(R"((\d+)\^(-?\d+))");
  string result = regex_replace(ep, exp_to_float, "$1e$2");
  std::println("Converted {} to {}", ep, result);
  return std::stod(result);
};
} // namespace roots
#endif // BSC_METHOD_HPP
