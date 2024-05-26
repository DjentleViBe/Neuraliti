/usr/bin/clang++ -c ./sharedlib/add.cpp -o ./sharedlib/add.o
/usr/bin/clang++ -shared -o ./sharedlib/shrsub.so ./sharedlib/add.o
make clean
make
make setup
./bin/Neuraliti