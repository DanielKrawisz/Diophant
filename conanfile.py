from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMake, cmake_layout, CMakeDeps
from os import environ

class DiophantConan (ConanFile):
    name = "diophant"
    license = "MIT"
    author = "Daniel Krawisz danielkrawisz@protonmail.com"
    url = "https://github.com/DanielKrawisz/Diophant"
    description = "A pure functional programming language that is good for mundane tasks as well as writing math proofs as programs via the Curry Howard correspondence."
    topics = ("functional programming", "Curry-Howard correspondence")
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "fPIC": [True, False]}
    default_options = {"shared": False, "fPIC": True}
    exports_sources = "*"
    requires = [
        "boost/1.80.0",
        "gmp/6.2.1",
        "data/v0.0.27@proofofwork/stable",
        "gtest/1.12.1"]
    
    def set_version (self):
        if "CIRCLE_TAG" in environ:
            self.version = environ.get ("CIRCLE_TAG")[1:]
        if "CURRENT_VERSION" in environ:
            self.version = environ['CURRENT_VERSION']
        else:
            self.version = "0.0.1"

    def config_options (self):
        if self.settings.os == "Windows":
            del self.options.fPIC

    def configure_cmake (self):
        cmake = CMake (self)
        cmake.configure()
        return cmake
    
    def layout(self):
        cmake_layout(self)

    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()
        tc = CMakeToolchain(self)
        tc.generate()

    def build (self):
        cmake = self.configure_cmake ()
        cmake.build ()

    def package (self):
        cmake = CMake(self)
        cmake.install()

    def package_info (self):
        self.cpp_info.libs = ["Diophant"]


