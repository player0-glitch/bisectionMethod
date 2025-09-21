#!/bin/bash

echo "[INFO] Creating build directory"

#Create build directory
mkdir build
if [ -d "build" ]; then
  echo "[INFO] Build directory created successfully"
else
  echo "[ERROR] Failed to create build directory"
  exit 1
fi

#Give a mf a chance to atleast read the outputs
sleep 2
echo "[INFO] Running cmake"

#Cmake should output it's own errors if it anything fails
cmake . -B build

#Go to where the executable will be just to have 1 less thing to type
cd build/
if [[ $PWD == */bisectionMethod/build ]]; then
  echo "[ERROR] Failed to change directory to build"
  exit 1
else
  echo "[INFO] Changed directory to build directory"
fi

#Give a mf a chance to atleast read the outputs
sleep 2

echo "[INFO] Building executable using Make"

make -j$(nproc)

#checks the return code of the last executed command (make)
if [ $? -eq 0 ]; then
  echo "[INFO] Build successful"
  echo "[INFO] You can run the executable using ./bisection_method"
else
  echo "[ERROR] Build failed"
  exit 1
fi
