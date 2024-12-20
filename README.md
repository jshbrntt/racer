# `racer`

SDL2 game project with cross-compilation toolchains for all major desktop platforms.

## Requirements
-   [Make](https://www.gnu.org/software/make/)
-   [Docker](https://docs.docker.com/get-docker/)

## Getting Started

1. Clone the repository.

1.  Run `make` to start the dev container.

### Windows

1.  Run `make WIN32=1`

1.  Run the built executable.

    ```
    $ ./build/win32/snake.exe 
    ```

### macOS

1.  Run `make MACOS=1`

1.  Run the built executable.

    ```
    $ ./build/macos/snake
    ```

### Linux

1.  Run `make LINUX=1`

1.  Run the built executable.

    ```
    $ ./build/linux/snake
    ```
