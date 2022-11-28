from conans import ConanFile, CMake
from os import environ

class DiophantConan(ConanFile):
    name = "Diophant"
    version = "0.0.1"
    license = "MIT"
    author = "Daniel Krawisz danielkrawisz@protonmail.com"
    url = "<Package recipe repository url here, for issues about the package>"
    description = "A pure functional programming language that is good for mundane programming tasks as well as for writing math proofs that can be checked by the computer."
    topics = ("functional programming", "Curry-Howard correspondence")
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "fPIC": [True, False]}
    default_options = {"shared": False, "fPIC": True}
    generators = "cmake"
    exports_sources = "*"
    requires = "boost/1.76.0", "openssl/1.1.1k", "cryptopp/8.5.0", "nlohmann_json/3.10.0", "gmp/6.2.1", "SECP256K1/0.2@proofofwork/stable", "data/v0.0.17@proofofwork/stable", "gtest/1.12.1"
    
    def set_version(self):
        if "CIRCLE_TAG" in environ:
            self.version = environ.get("CIRCLE_TAG")[1:]
        if "CURRENT_VERSION" in environ:
            self.version = environ['CURRENT_VERSION']
        else:
            self.version = "0.0.1"

    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC

    def configure_cmake(self):
        if "CMAKE_BUILD_CORES_COUNT" in environ:
            cmake = CMake(self, parallel=False)
        else:
            cmake = CMake(self)
        cmake.definitions["PACKAGE_TESTS"] = "Off"
        cmake.configure()
        return cmake

    def build(self):
        cmake = self.configure_cmake()
        if "CMAKE_BUILD_CORES_COUNT" in environ:
            cmake.build(args=["--", environ.get("CMAKE_BUILD_CORES_COUNT")])
        else:
            cmake.build()

    def package(self):
        self.copy("*.h", dst="include", src="include")
        self.copy("*.hpp", dst="include", src="include")
        self.copy("libgigamonkey.a", src="lib", dst="lib", keep_path=False)

    def package_info(self):
        self.cpp_info.libdirs = ["lib"]  # Default value is 'lib'
        self.cpp_info.libs = self.collect_libs()

