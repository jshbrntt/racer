# conan-hello-world

Dockerized Conan hello world project which can build for Windows and Linux.

## Requirements
-   [Make](https://www.gnu.org/software/make/)
-   [Docker](https://docs.docker.com/get-docker/)

## Getting Started

1. Clone the repository.

### Windows

1.  Run `make`

2.  Run the built executable.

    ```
    $ ./build_Windows_x86_64_Release/bin/greet.exe 
    Hello World Release!
    ```

### Linux
1.  Run `make OS=Linux`

2.  Run the built executable.

    ```
    $ ./build_Linux_x86_64_Release/bin/greet
    Hello World Release!
    ```

### Debug Build

1.  Run `make BUILD_TYPE=Debug`

2.  Run the built executable.

    ```
    $ ./build_Windows_x86_64_Debug/bin/greet.exe 
    Hello World Debug
    ```
