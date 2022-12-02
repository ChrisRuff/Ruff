import os
from conans import ConanFile, CMake


class ruffConan(ConanFile):
    name = "ruff"
    version = "1.0"
    license = "MIT"
    author = "Chris Ruff"
    url = "https://github.com/ChrisRuff/Ruff"
    description = "Package for any utility I may require in the future."
    settings = "os", "compiler", "build_type", "arch"
    options = \
    {
        "build_docs": [False, True],
        "build_tests": [False, True],
        "build_examples": [False, True],
        "build_geometry": [False, True],
        "build_ui": [False, True],
        "build_ai": [False, True],
        "build_nlp": [False, True],
        "build_security": [False, True],
    }
    generators = ["cmake_find_package", "cmake_paths", "cmake"]
    default_options = \
    {
            "build_docs": False, 
            "build_tests": False, 
            "build_examples": False,
            "build_geometry": False,
            "build_ui": True,
            "build_ai": True,
            "build_nlp": True,
            "build_security": True
    }
    exports_sources = ["src/*", "include/*", "CMakeLists.txt", "cmake/*", "tests/*"]

    def build_requirements(self):
        self.build_requires("cmake/[>=3.20.4]")

    def configure_cmake(self):
        cmake = CMake(self)
        cmake.definitions["BUILD_DOXY_DOC"] = "ON" if self.options.build_docs else "OFF"
        cmake.definitions["BUILD_TESTERS"] = "ON" if self.options.build_tests else "OFF"
        cmake.definitions["BUILD_EXAMPLES"] = "ON" if self.options.build_examples else "OFF"
        cmake.definitions["BUILD_GEOMETRY"] = "ON" if self.options.build_geometry else "OFF"
        cmake.definitions["BUILD_UI"] = "ON" if self.options.build_ui else "OFF"
        cmake.definitions["BUILD_AI"] = "ON" if self.options.build_ai else "OFF"
        cmake.definitions["BUILD_NLP"] = "ON" if self.options.build_nlp else "OFF"
        cmake.definitions["BUILD_SECURITY"] = "ON" if self.options.build_security else "OFF"
        cmake.configure()
        return cmake

    def config_options(self):
        if self.options.build_ui:
            self.options["sdl"].pulse = False
            self.options["sdl"].nas = False

    def requirements(self):
        if self.options.build_ui:
            self.requires("zlib/1.2.12", override=True)
            self.requires("sdl/2.0.16")
            self.requires("libpng/1.6.37")
        if self.options.build_security:
            self.requires("gmp/6.1.2")
            self.requires("openssl/1.1.1q")
        if self.options.build_nlp:
            self.requires("ctre/3.7")
        if self.options.build_tests:
            self.requires("doctest/2.4.8")
        if self.options.build_docs:
            self.requires("doxygen/1.9.1")

    def build(self):
        cmake = self.configure_cmake()
        cmake.build()

    def package(self):
        self.copy("*.hpp", dst="include", src="include")
        self.copy("*{name}.lib".format(name=self.name), dst="lib", keep_path=False)
        self.copy("*.dll", dst="bin", keep_path=False)
        self.copy("*.so", dst="lib", keep_path=False)
        self.copy("*.dylib", dst="lib", keep_path=False)
        self.copy("*.a", dst="lib", keep_path=False)

    def package_info(self):
        self.cpp_info.libs = ["ruff-core", "ruff-geometry", "ruff-ui",
                              "ruff-nlp", "ruff-ai", "ruff-security"]
