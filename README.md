# racer (Topdown Racing)
A project created as part of the [GRAPHICS 1 (CMPC2G04)](https://www2.uea.ac.uk/study/undergraduate/degree/detail/bsc-computing-science) module I did during my university degree.

It was the first time I had ever programmed in C/C++ and was found it to be an enlightening experience.

That being said this is a far from perfect example of how to program a game in C/C++.

As of 2019-09-29 I decided to dig out the original source code and get it compiling and running on macOS (10.13.6 High Sierra) using [Visual Studio Code](https://code.visualstudio.com/) and [Clang](https://clang.llvm.org/).

![Demo](https://github.com/synthecypher/racer/blob/master/demo.gif?raw=true)

# Building

# macOS (10.13.6 High Sierra)

1. Install compiler

```shell
# Complete the Xcode installation process if you have not already.
$ xcode-select --install
xcode-select: error: command line tools are already installed, use "Software Update" to install updates

# Ensure Clange is ready to use.
$ clang++ -v
Apple LLVM version 10.0.0 (clang-1000.10.44.4)
Target: x86_64-apple-darwin17.7.0
Thread model: posix
InstalledDir: /Library/Developer/CommandLineTools/usr/bin
```

2. Install Cmake via [Homebrew](https://brew.sh/)

```
$ brew install cmake
$ cmake --version
cmake version 3.15.3

CMake suite maintained and supported by Kitware (kitware.com/cmake).
```

3. Install [Visual Studio Code](https://code.visualstudio.com/)

4. Install the [C/C++ extension](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools)

4. Clone the repository

```shell
# You'll need '--recursive' to get the macOS version of the SOIL library
# It's used for loading PNG image files into GL textures.
$ git clone --recursive git@bitbucket.org:synthecypher/racer.git
```

5. Compile `SOIL.framework` and copy it to the `/Library/Frameworks` directory

```shell
$ pwd
/Users/synthecypher/Projects/racer

$ cd SOIL/build
$ cmake .. -G "Unix Makefiles"
$ make
$ make SOIL_Framework
$ sudo cp -R SOIL.framework /Library/Frameworks
```

6. Open Visual Studio Code

```shell
$ cd ../..
$ pwd
/Users/synthecypher/Projects/racer
$ code .
```

7. Build executable `Terminal > Run Build Task...`

8. Launch executable `Debug > Start Debugging`