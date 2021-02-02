#export PATH=/home/vk/Qt/5.12.2/gcc_64/bin:$PATH
export Qt5_DIR=/home/vk/Qt/5.12.2/gcc_64
mkdir bin
cd bin
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build . -- -j4
#cmake --build .
