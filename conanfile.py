from conans import ConanFile, CMake

class PocoTimerConan(ConanFile):
   settings = "os", "compiler", "build_type", "arch"
   requires = "googletest/master@Blubb/stable", "google-benchmark/1.0.0@TyRoXx/stable", "GSL/0.1@Blubb/alpha"
   generators = "cmake"