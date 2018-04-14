# Web-server with cpp
 Tiny web-server over HTTP

## Getting start
 Tested g++ version :
 - Apple LLVM version 9.0.0 (clang-900.0.39.2)
 - Target: x86_64-apple-darwin17.4.0
 - Thread model: posix
 
    
    $ git clone https://github.com/yk1028/http-server
    $ g++ -c http-server.cpp requestinfo.cpp web.cpp
    $ g++ http-server.o requestinfo.o web.o
    $ ./a.out <port>
    
## Features
 - serve static file over HTTP