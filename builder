mkdir build
cd build
mkdir build-debug
cd build-debug 
cmake -DCMAKE_TOOLCHAIN_FILE=ToolChain.cmake -DCMAKE_BUILD_TYPE=Debug ../..
make -j 8
cd ..
mkdir build-release 
cd build-release
cmake -DCMAKE_TOOLCHAIN_FILE=ToolChain.cmake ../..
make -j 8
