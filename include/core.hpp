#ifndef CORE_H
#define CORE_H

#include <string>

namespace core {

enum FONTS { BIG = 0, BANNER, SLANT, SMSCRIPT, BUBBLE };

inline std::array<std::string, 5> fonts{"big", "banner", "slant", "smscript",
                                        "bubble"};
// convert user input and later input for parser friendly format
std::string convertToParser(const std::string &expression);
void openLatexDoc(std::ofstream &LaTex);
void closeLatexDoc(std::ofstream &Latex);
void splash();
std::string convertToLatex(const std::string &expression);
} // namespace core
#endif // CORE_H
