#!/bin/bash

set -e

if [ "`uname -s`" == "Darwin" ]; then
  CFLAGS="-I/opt/X11/include `pkg-config --cflags glfw3` `pkg-config --libs glfw3` -framework OpenGL"
else
  CFLAGS="-lGL -lglfw -lX11 -lpthread -lXrandr -lXi -ldl"
fi


DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
pushd "$DIR" #> /dev/null
mkdir -p ./build
pushd ./build #> /dev/null
c++ -std=c++11 -Wall -o learngl ../src/glad.c ../src/main.cpp -I../include -I/usr/local/include -g $CFLAGS
