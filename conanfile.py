from conans import ConanFile, CMake 
from conans.tools import os_info
import multiprocessing

class QEJsonS11nConan(ConanFile):
    name = "QEJson"
    version = "0.1.0"
    requires = "QEEntity/0.1.0@fmiguelgarcia/stable"
    settings = "os", "compiler", "build_type", "arch"
    license = "https://www.gnu.org/licenses/lgpl-3.0-standalone.html"
    generators = "cmake"
    url = "https://github.com/fmiguelgarcia/QEJsonS11n.git"
    description = "JSon Serialization library based on Annotations"
    exports_sources = [ "src/*", "test/*", "tools/*", "CMakeLists.txt"] 

    def build(self):
        cmake = CMake( self.settings)
        parallel_build_flags = ("-- -j %d " % multiprocessing.cpu_count()) if os_info.is_linux else ""
        self.run( "cmake %s %s" % (self.conanfile_directory, cmake.command_line))
        self.run( "cmake --build . %s %s"  % (cmake.build_config, parallel_build_flags))

    def package(self):
        self.copy( pattern="*.hpp", dst="include/qe/json", src="src/qe/json")
        self.copy( pattern="LICENSE.LGPLv3", dst="share/qe/json")
        self.copy( pattern="libQEJson.so*", dst="lib", src="src/qe/json", links=True)
        self.copy( pattern="libQEJson.dll", dst="lib", src="src/qe/json/bin")
        self.copy( pattern="libQEJson.dll.a", dst="lib", src="src/qe/json/lib")

    def package_info(self):
        self.cpp_info.libs.extend(["QEJson"])
