from conans import ConanFile, CMake 
import multiprocessing

class QEJsonS11nConan(ConanFile):
    name = "QEJsonS11n"
    version = "0.1.0"
    requires = "QEAnnotation/0.1.0@fmiguelgarcia/stable"
    settings = "os", "compiler", "build_type", "arch"
    license = "https://www.gnu.org/licenses/lgpl-3.0-standalone.html"
    generators = "cmake"
    url = "ssh://miguel@migroslinx.no-ip.org/home/miguel/git/QE/QEJsonS11n"
    description = "JSon Serialization library based on Annotations"

    def source(self):
        self.run("git clone %s" % self.url) 

    def build(self):
        cmake = CMake( self.settings)
        self.run( "conan install %s/QEJsonS11n" % self.conanfile_directory)
        self.run( "cmake %s/QEJsonS11n %s" % (self.conanfile_directory, cmake.command_line))
        self.run( "cmake --build . %s"  % cmake.build_config ) 

    def package(self):
        self.copy( pattern="*.hpp", dst="include/QEJsonS11n/", src="QEJsonS11n/src")
        self.copy( pattern="LICENSE.LGPLv3", dst="share/QEJsonS11n")
        self.copy( pattern="libQEJsonS11n.so*", dst="lib", src="src")

    def package_info(self):
        self.cpp_info.libs.extend(["QEJsonS11n"])
        self.cpp_info.includedirs.extend(["include/QEJsonS11n"])
