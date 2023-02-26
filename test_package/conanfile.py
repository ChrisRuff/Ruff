import os

from conans import ConanFile, CMake, tools


class ruffTestConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "cmake"

    def build(self):
        cmake = CMake(self)
        # Current dir is "test_package/build/<build_id>" and CMakeLists.txt is
        # in "test_package"
        cmake.configure()
        cmake.build()

    def config_options(self):
        self.options["ruff"].build_geometry = True
        self.options["ruff"].build_ui = True
        self.options["ruff"].build_ai = True
        self.options["ruff"].build_nlp = True
        self.options["ruff"].build_security = True
        self.options["ruff"].build_photogrammetry = True
    def imports(self):
        self.copy("*.dll", dst="bin", src="bin")
        self.copy("*.dylib*", dst="bin", src="lib")
        self.copy('*.so*', dst='bin', src='lib')
        self.copy('*.a*', dst='bin', src='lib')

    def test(self):
        if not tools.cross_building(self):
            os.chdir("bin")
            self.run(".%sexample" % os.sep)
