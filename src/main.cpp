#include "core.hpp"
#include "exprtk.hpp"
#include "roots.hpp"
#include <fstream>
#include <iomanip>
#include <iostream>
#include <print>
#include <string>
#include <vector>

using std::string, std::vector, std::println;

int main() {
  core::splash();
  // Generate LaTeX output of the equation
  std::ofstream LaTex("function.tex");
  core::openLatexDoc(LaTex);
  bool keep_running = false;
  char cont = '\0';
  // make it so that the user can 'render' multiple equations
  do {
    std::cout << "Enter your equation (as a function of x)" << std::endl;
    std::print("f(x)=");
    std::getline(std::cin, roots::eq); // I need to read the whole line of this
                                       // string this is for later

    // get the endpoints of the function
    double a = {0}, b = {0}, epsilon{};
    println("Enter the left endpoint (a)");
    std::cin >> a;

    println("Enter the right endpoint (b)");
    std::cin >> b;

    println("Use default tolerance of 10^(-5)? (y/n)");
    char choice = '\0';
    std::cin >> choice;

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    choice = tolower(choice);

    switch (choice) {
    case 'y':
      roots::str_interations = roots::calculateNumOfInterations(a, b);
      break;
    case 'n':
      // std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      println("Enter the tolerance (epsilon);");

      std::getline(std::cin, roots::epsilon_str);

      // std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      epsilon = roots::scientificToFloat(roots::epsilon_str);
      println("Epsilon is now={}", epsilon);
      // std::cin.ignore(std::numeric_limits<std::streamsize::max(), '\n');
      roots::str_interations = roots::calculateNumOfInterations(a, b, epsilon);
      // println("Iterations calculated={}", str_interations);
      break;
    default:
      // deal with invalid input later
      epsilon = EPSILON;
      break;
    }
    println("Broken from default with flag {}", keep_running);
    // set up the parser
    symbol_table<double> symbol_table;
    double x; // variable used by the parser
    double c = roots::calculateMidpoint(a, b);
    // symbol table
    symbol_table.add_variable("x", x);

    expression<double> expression;
    expression.register_symbol_table(symbol_table);

    // convert the equarion to what the parser understands
    roots::eq = core::convertToParser(roots::eq);
    // error checking
    parser<double> parser;
    if (!parser.compile(roots::eq, expression)) {
      std::cerr << __LINE__ << " Parser Error: Failed to Parser User Input ("
                << parser.error() << ")\n";
      return 1;
    }

    // evaluate the expression
    vector<double> results;
    // midpoint, [a,b]
    vector<double> variables = {c, a, b};
    for (double var : variables) {
      x = var;
      auto result = expression.value(); // evaluate at current c
      // expression.results();
      results.emplace_back(result);
    }

    LaTex << "\nThe function you entered is:\n"
          << "\\[\n"
          << "f(x) = " << core::convertToLatex(roots::eq) << "\n"
          << "\\]\n"
          // Just creates a bulleted list of the evaluated points
          << "\\begin{itemize} \n"
          << "\\item Evaluated at midpoint = " << c << ", $f(" << c
          << ") = " << results[0] << "$ \n";
    for (uint i = 1; i < results.size(); i++) {
      LaTex << "\\item Endpoint " << (i) << "= " << variables.at(i)
            << ", \\quad  $f(" << variables.at(i)
            << ") = " << std::setprecision(5) << results.at(i) << "$"
            << "\n";
    }
    // LaTex << "\n";

    // Verifying if the equation satisfies the IVT
    if (results.at(1) * results.at(2) < 0) [[__likely__]] {
      LaTex
          << "\\item Since $f(a)\\cdot f(b)<0$, there is atleast 1 root in the "
             "interval $["
          << a << "," << b << "]$.\n";

      LaTex << "\\end{itemize}\\]\n";
    } else if (results.at(1) * results.at(2) > 0) {
      LaTex << "\\item Since $f(a)\\cdot f(b)>0$, there is no root in the "
               "interval $["
            << a << "," << b << "]$.\n"
            << "The Bisection method cannot be applied.\n";

      LaTex << "\\end{itemize}\\]\n";
      // closeLatexDoc(LaTex);
      // continue;
    }

    LaTex << "\\]\n"
          << "The number of iterations required given by a  tolerance of "
             "$10^{-5}$ "
             "is calculated using the formula: "
          << roots::str_interations << "\n"
          << roots::bisectionMethod(roots::eq, a, b);
    println("Would you like to render another equation? (y/n)");
    std::cin >> cont;
    println("Continue? {}", cont);
    if (tolower(cont) == 'y') {
      keep_running = true;
      // clear the input buffer. This might be necessary since I dont use to
      // endl which flushes the buffer in itself
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    } else {
      keep_running = false;
    }
  } while (keep_running);
  core::closeLatexDoc(LaTex);

  return 0;
}
