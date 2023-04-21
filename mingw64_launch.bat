cmake -G "MinGW Makefiles" -B "./build" -DX64_BITS=ON .
cd build
make
cp ./libneat.a ../lib/libneat.a
cd ..
