/usr/bin/clang++ -c -fPIC ./sharedlib/add.cpp -o ./sharedlib/add.o
/usr/bin/clang++ -shared -o ./sharedlib/shrsub.so ./sharedlib/add.o