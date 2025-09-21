#ifndef CORE_H
#define CORE_H

#include <string>

namespace core {

enum FONTS { BIG = 0, BLOCK, SLANT };

inline std::array<std::string, 3> fonts{"big", "block", "slant"};
// convert user input and later input for parser friendly format
std::string convertToParser(const std::string &expression);
void openLatexDoc(std::ofstream &LaTex);
void closeLatexDoc(std::ofstream &Latex);
void splash();
std::string convertToLatex(const std::string &expression);
} // namespace core
#endif // CORE_H
