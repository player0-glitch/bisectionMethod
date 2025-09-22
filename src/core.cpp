#include "core.hpp"
#include <fstream>
#include <iostream>
#include <print>
#include <random>
#include <regex>

namespace core {

using std::string;
// convert user input and later input for parser friendly format
std::string convertToParser(const string &expression) {
  using std::regex;
  std::string equation = expression;
  // converts any instace of e^a to exp(a) because e isn't defined in the exprtk
  // library
  regex e_to_exp(R"(e\^([a-zA-Z0-9]+))");
  equation = regex_replace(equation, e_to_exp, "exp($1)");

  // converts ln(a) from user input to log(a) for the parser
  regex ln_to_log(R"(ln\(([^)]+)\))");
  equation = regex_replace(equation, ln_to_log, "log($1)");

  return equation;
}
// Show a splash screen to help the user understand the formatting rules they
// should follow
void splash() {

  // create a random index for the splash screen font
  std::random_device rand;
  std::mt19937 mt(rand());                                      // Modern stuff
  std::uniform_int_distribution<int> dist(0, fonts.size() - 1); // range [0,3]

  int font_index = dist(mt);
  // create the splash screen to use figlet fonts
  using std::println;
  println("=================================================================");
  int figlet = std::system(
      ("figlet  -cf " + (fonts[font_index]) + " Bisection Method").c_str());
  if (figlet != 0) {
    std::cerr << "Error: figlet command failed to execute. " << figlet
              << std::endl;
    // exit(1); // exit the program
  }
  const char *bold_italics = "\033[1;3m";      // \033[1;3m -> Bold + Italics
  const char *reset_terminal_font = "\033[0m"; // \003[0m -> reset formatting
  println("{}\tNumerical Method Solver (Root Finding Program)", bold_italics,
          reset_terminal_font);
  println("{}\t\tMethod: Bisection Method{}", bold_italics,
          reset_terminal_font);
  println("Natural Log: ln(x)\nExponential: e^x");
  println("Log: log(x)\nx raised to: x^2");
  println("=================================================================");
}
// converts the user input to latex format
string convertToLatex(const string &expression) {
  using std::regex;
  std::string latex = expression;

  /// tryna convert ^ to ^{} exponent
  regex pattern1("\\^\\(([^)]+)\\)");
  regex pattern2("\\^([a-zA-Z0-9]+)\\)");
  regex pattern3("\\^\\(?([a-zA-Z0-9+\\-*/]+)\\)?");
  regex pattern4("\\^\\(\\s*([a-zA-Z0-9+\\-*/\\s]+?)\\s*\\)|\\^([a-zA-Z0-9]+)");

  // pattern 1-2
  latex = regex_replace(latex, pattern2, "^{$1}");
  // latex = regex_replace(latex, regex("\\^(\\d+|[a-zA-Z])"), "^{\\1}");
  // pattern 3-4
  // latex = regex_replace(latex, pattern3, "^{\\1\\2}");

  // this is optional but idk why it's here (removing *)
  latex = regex_replace(latex, regex("\\*"), "");

  // LaTex and exprtk have a conflict with how they handle e^x
  //  convert from exp(a) to e^a
  regex clean_logn(R"(\([^,]*,)");
  latex = regex_replace(latex, clean_logn, "(");

  regex logn_to_ln(R"(log\(([^)]+)\))");
  latex = regex_replace(latex, logn_to_ln, "\\ln{$1}");

  regex exp_to_e(R"(exp\(([^)]+)\))");
  latex = regex_replace(latex, exp_to_e, "e^{$1}");

  return latex;
}

//
void openLatexDoc(std::ofstream &LaTex) {
  // Open the document class and packages
  LaTex << "\\documentclass{article}\n"
        << "\\usepackage{amsmath}\n"
        << "\\usepackage{hyperref}\n"
        << "\\begin{document}\n";
  // Start a new unnumbered section
  LaTex << "\\section*{Bisection Method}\n"
        << "This is a numerical method used to find approximate roots "
           "(solutions) of equations of the form: \\newline"
        << "\\[\n f(x)=0\\]\n"
        << "This method is used when an exact algebraic solution is difficult "
           "or impossible to obtain. The Bisection Method is based on the "
           "\\textbf{Intermediate Value Theorem, "
           "(\\href{https://en.wikipedia.org/wiki/"
           "Intermediate_value_theorem}{IVT})}."
        << " If $f(x)$ is continuous on an interval $[a,b]$ and if $f(a), f(b)$"
           " satisfy the IVT such that $f(a)\\cdot f(b)<0$, then there is "
           "atleast one root in the interval $[a,b]$.\n";
  // Representing the Bisection Method algorithm in LaTex
  LaTex << "\\subsection*{Algorithm}\n"
        << "\\begin{enumerate}\n"
        // 1 Choose The Initial Interval
        << "\\item \\textbf{Choose The Initial Interval}:\n"
        << "The initial interval needs to satisfy the Intermediate Value "
           "Theorem, that is: \\[f(a)\\cdot f(b)<0\\]\n"
        // 2 Compute the Midpoint
        << "\\item \\textbf{Compute the Midpoint}:\n"
        << "Let c be an integer such that\n"
        << "\\[c=\\frac{a+b}{2}\\]\n"
        << "Now c is considered the midpoint of the interval [a,b].\n"
        // 3 Check Signs
        << "\\item \\textbf{Check Signs}: \\newline\n"
        << "If $f(a)\\cdot f(c)<0$, then root is in $[a,c]$.\\newline\n"
        << "If $f(b)\\cdot f(c)<0$, then set $[c,b]$.\\newline\n"
        << "If $f(c)=0$, then c is the exact root.\\newline\n"
        // 4 Update the Interval
        << "\\item \\textbf{Update the Interval}:\n"
        << "Replace the interval $[a,b]$ with the interval that contains the "
           "root\n"
        // 5 Repeat
        << "\\item \\textbf{Repeat Until Convergence}:\n"
        << "Continue until the interval is small enough (within the desired "
           "tolerace $\\varepsilon$ ).\n"
        << "\\end{enumerate}\n" // end the numbered list
        << "\\newline\n";
}
// Close the Latex document
void closeLatexDoc(std::ofstream &Latex) {
  Latex << "\\]\n"
        << "\\end{document}\n";
  Latex.close();

  // Check if the file successfully closed
  if (Latex.fail()) {
    std::cerr << "Error closing the file.\n";
  } else {
    std::println("File closed successfully.");
  }
  // Convert the latex to pdf
  int ret = std::system(
      "pdflatex -interaction=nonstopmode function.tex > /dev/null 2>&1");

  if (ret != 0) {
    std::cerr << "Error: pdflatex command failed with return code " << ret
              << "\n";
    exit(1); // exit the program
  }
  exit(0); // return 0;
}
} // namespace core
