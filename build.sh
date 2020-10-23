g++ client.cpp -o client

rm -rf build
mkdir build && cd build
cmake ..
make -j

cp server ..
cd ..
