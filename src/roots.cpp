#include "roots.hpp"
#include "core.hpp"
#include "exprtk.hpp"
#include <iomanip>
#include <iostream>

namespace roots {

double calculateMidpoint(double &a, double &b) { return ((a + b) / 2); }

string calculateNumOfInterations(double &a, double &b, double epsilon) {
  using exprtk::details::numeric::sgn;
  using std::to_string;
  std::stringstream latex_result;

  // Create the expression that will be needed by the parser
  string expression_string = "(log10(b-a)-log10(epsilon))/log10(2)";
  // Create symbols table and add variables
  exprtk::symbol_table<double> symbolTable;
  symbolTable.add_variable("a", a);
  symbolTable.add_variable("b", b);
  symbolTable.add_variable("epsilon", epsilon);

  exprtk::function_compositor<double> natural_log;
  expression<double> expression;
  expression.register_symbol_table(symbolTable);

  // Create the parser and compile the expression
  parser<double> parser;

  if (!parser.compile(expression_string, expression)) {
    std::cerr << __LINE__
              << " Parser Error: Failed to calculate number of iterations ("
              << parser.error() << ")\n";
    return "";
  }
  number_of_iterations = std::ceil(expression.value());
  string result = to_string(expression.value());
  expression_string += " \\]\n";
  latex_result << expression_string << "\\[" << "n\\geq \\frac{\\log_{10}(("
               << b << ")-(" << a << ")) - \\log_{10}(" << epsilon
               << ")}{\\log_{10}(2)}=" << result << "\\]\n"
               << "\\[n\\simeq" << number_of_iterations;
  return latex_result.str();
}

string bisectionMethod(std::string_view equation, double &a, double &b,
                       double epsilon) {
  using exprtk::details::numeric::sgn;
  std::string eq = std::string(equation.data());

  eq = core::convertToParser(roots::eq);
  symbol_table<double> symbol_table;
  double x; // variable used by the parser
  double c = calculateMidpoint(a, b);

  // symbol table
  symbol_table.add_variable("x", x);

  expression<double> expression;
  expression.register_symbol_table(symbol_table);

  // set up the parser
  parser<double> parser;

  // error checking
  if (!parser.compile(eq, expression)) {
    std::cerr << __LINE__
              << " Parser Error: Failed to Parse Equation For Root Finding ("
              << parser.error() << ")\n";
    return "";
  }

  std::stringstream latex_result;
  latex_result << "\\section*{Finding the root}\n";
  latex_result << "We start with the interval $[" << a << "," << b << "]$.\n";
  latex_result << "The function is given by:\n";
  latex_result << "\\[\n";
  latex_result << "f(x) = " << core::convertToLatex(eq) << "\n";
  latex_result << "\\]\n";

  // Start a table for neater bisection method root finding
  latex_result << "\\begin{tabular}{|c|c|c|c|c|c|c|}\n"
               << "\\hline\n"
               << "n & a & b & c & $f(a)$ & $f(b)$ & $f(c)$ \\\\\n"
               << "\\hline \n";
  for (int i = 0; i < number_of_iterations; i++) {
    c = calculateMidpoint(a, b);
    x = c;
    double fc = expression.value();
    x = a;
    double fa = expression.value();
    x = b;
    double fb = expression.value();

    latex_result << i + 1 << "&" << a << "&" << b << "&" << c << "&"
                 << std::setprecision(5) << fa << "&" << std::setprecision(5)
                 << fb << "&" << std::setprecision(5) << fc << " \\\\\n"
                 << "\\hline\n";
    if (sgn(fa) * sgn(fc) < 0) {
      b = c;
      // latex_result << "Since $f(a)\\cdot f(c)<0$, we set $b=c$.\n"
      // << "\\]\n";
    } else if (sgn(fb) * sgn(fc) < 0) {
      a = c;
      // latex_result << "Since $f(b)\\cdot f(c)<0$, we set $a=c$.\n"
      // << "\\]\n";
    } else {
      // latex_result << "The exact root is found at $c=" << c << "$.\n"
      // << "\\]\n";
      break;
    }
  }
  // Close the table
  latex_result << "\\end{tabular}\n"
               << "\\]\n";
  latex_result << "After " << number_of_iterations
               << " iterations, the approximate root is $c=" << c
               << "$ with tolerance $" << epsilon << "$.\n";
  return latex_result.str();
}

} // namespace roots
