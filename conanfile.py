from conans import ConanFile, CMake

class PocoTimerConan(ConanFile):
   settings = "os", "compiler", "build_type", "arch"
   requires = "googlemock/1.7.0@Blubbz0r/stable", "google-benchmark/1.0.0@TyRoXx/stable"
   generators = "cmake"