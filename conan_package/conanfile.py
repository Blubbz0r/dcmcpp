from conans import ConanFile, CMake, tools

class DcmcppConan(ConanFile):
    name = "dcmcpp"
    version = "1.0.0"
    settings = "os", "compiler", "build_type", "arch"
    options = { "with_apps" : [True, False] }
    default_options = "with_apps=False"
    generators = "cmake"
    exports_sources = "../CMakeLists.txt", "../conanfile.py", "../include*", "../src*"
    no_copy_source = True
    install_dir = ""

    def requirements(self):
        self.requires("GSL/0.1@Blubb/alpha")

    def build(self):
        cmake = CMake(self)
        self.install_dir = "%s/%s" % (self.conanfile_directory, "install")
        cmake.definitions["CMAKE_INSTALL_PREFIX"] = self.install_dir
        cmake.definitions["WITH_APPS"] = "ON" if self.options.with_apps else "OFF"
        cmake.definitions["WITH_TESTS"] = "OFF"
        cmake.configure()
        cmake.build()
        cmake.install()
        
    def package(self):
        self.copy("*.h", src=self.install_dir)
        self.copy("*.lib", src=self.install_dir, keep_path=True)

    def package_info(self):
        self.cpp_info.libs = ["dcmcpp"]