from conans import ConanFile, CMake

class Racer(ConanFile):
    name = "Racer"
    version = "1.0.0"
    license = "MIT"
    settings = "os", "compiler", "build_type", "arch"
    url = "https://github.com/joshua-barnett/racer.git"
    generators = "cmake"
    requires = [
        "sdl2/2.0.14@bincrafters/stable",
        "sdl2_ttf/2.0.15@bincrafters/stable",
        "sdl2_image/2.0.5@bincrafters/stable",
        "entt/3.2.2"
    ]

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
