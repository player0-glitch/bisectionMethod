# Bisection Method

Root finding algorithm which repeatedly bisects an interval and then selects a subinterval in which a root exists. This algorithm depends on the Intermediate Value Theorem.

---
### Table of Contents
1. [Overview](#overview)
2. [Features](#features)
3. [Installation and Usage](#installation-and-usage)
4. [Demo](#demo)
5. [Contacts](#contacts)
---
### Overview
This project started as a way to help me quickly perform the bisection method on a given function for my applied mathematics course. As the months went on I've extended it to be a lot more user friendly and added a report generation feature to it. 
The report generated is a pdf document in LaTeX format with the results of the algorithm and a table showing the steps taken to reach the root.

### Features
Makes use of the ExprTK math C++ library for parsing and computing of mathematical expressions.
The program then uses regex (hate this stuff) to convert from ExprTK format to LaTeX format for generating a report of the bisection method steps using pdflatex.

### Installation and usage
#### Pre-requisites
- CMake 
- C++20 compatible compiler
- figlet 
- pdflatex
> **Note:** This program assumes you have figlet and pdflatex installed as this was only tested on a Linux system. Fedora Workstation 42 to be precise.

#### Steps 

```
git clone https://github.com/player0-glitch/bisectionMethod.git 
clone this repo
cd bisectionMethod/

#Give executable permissions to the build script in the project and run it 
chmod +x build.sh
source ./build.sh #allows the script changes to persist in the current shell
```

### Demo
A little splash screen.
```
  ____  _            _        _   _                  _   _           _   
 |  _ \| |          | |      | | (_)                | | (_)         | |  
 | |_) | | ___   ___| | _____| |_ _  ___  _ __   ___| |_ _  ___ __ _| |_ 
 |  _ <| |/ _ \ / __| |/ / _ \ __| |/ _ \| '_ \ / _ \ __| |/ __/ _` | __|
 | |_) | | (_) | (__|   <  __/ |_| | (_) | | | |  __/ |_| | (_| (_| | |_ 
 |____/|_|\___/ \___|_|\_\___|\__|_|\___/|_| |_|\___|\__|_|\___\__,_|\__|
                                                                         
                                                                         
```

### Roadmap
- [ ] Add a GUI to make the program even more user friendly.
- [ ] Find a way to run it on the web.
> **Hack** By using the Clay C UI library i may be able to cross out both tasks in 1 go as Clay supposedly supports web assembly.
