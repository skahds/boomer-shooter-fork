mkdir -p bin/$1
lua build/lmake.lua -config=$1
make -f bin/$1/Makefile $2
