# http-server

A basic HTTP server in C using Winsock2, with testing implemented via the Unity framework.

## Table of Contents
- Prerequisites
- Usage
- License

## Prerequisites
- C compiler (e.g., GCC)
- Winsock2 library

*Note: GCC is the default compiler. To use a different compiler, change the `CC` variable in the Makefile (e.g., `CC = clang`).*

## Usage
Builds server
```bash
make
```
Starts the server
```bash
./http-server
# Then open localhost:8080 in a browser
```
Removes build files
```bash
make clean
```
Builds and runs tests
```bash
make test
```

## License
This project is licensed under the MIT License - see the LICENSE file for details.
