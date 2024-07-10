set -e
mkdir build
cd build
cmake ..
make p4c-pie && ./backends/pie/p4c-pie ../testdata/demo/test.p4 -I ../p4include
# ref 'test.ll' and 'hello'