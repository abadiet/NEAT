cmake -G "MinGW Makefiles" -B "./build" -DX64_BITS=OFF .
cd build
make
cp ./libneat.a ../lib/libneat.a
cd ..
