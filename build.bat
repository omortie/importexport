@IF DEFINED CMAKE (
	echo Using outsource build
) ELSE (
	echo Using internal build
	@SET CMAKE=cmake -G "MinGW Makefiles" -DMY_FLAGS=""
	@SET CORES=-j 1
)
@mkdir build
@cd build
@mkdir build-debug
@cd build-debug
%CMAKE% -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=ToolChain.cmake ../..
mingw32-make -f Makefile %CORES%
@cd ..
@mkdir build-release
@cd build-release
%CMAKE% -DCMAKE_TOOLCHAIN_FILE=ToolChain.cmake ../..
mingw32-make -f Makefile %CORES%
@cd ../..