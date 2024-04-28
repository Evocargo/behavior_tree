mkdir build
cd build
rm *.deb
cmake -DBUILD_TESTS=ON -DCATKIN_BUILD_BINARY_PACKAGE=OFF .. || exit
cmake --build . -j8 || exit
ctest || exit
for PACKAGE_CONFIG in configs/*; do cpack -D CPACK_CMAKE_GENERATOR=Ninja --config ${PACKAGE_CONFIG}; done || exit
sudo apt install --reinstall ./*.deb
