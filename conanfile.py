from conans import ConanFile, CMake 

class QEJsonS11nConan(ConanFile):
    name = "QEJson"
    version = "1.0.0"
    requires = "QEEntity/1.0.0@fmiguelgarcia/stable"
    settings = "os", "compiler", "build_type", "arch"
    license = "https://www.gnu.org/licenses/lgpl-3.0-standalone.html"
    generators = "cmake"
    url = "https://github.com/fmiguelgarcia/QEJsonS11n.git"
    description = "JSon Serialization library based on Annotations"
    exports_sources = [ "src/*", "test/*", "tools/*", "CMakeLists.txt"] 

    def build(self):
        cmake = CMake( self)
        cmake.configure()
        cmake.build()

    def package(self):
        self.copy( pattern="*.hpp", dst="include/qe/json", src="src/qe/json")
        self.copy( pattern="LICENSE.LGPLv3", dst="share/qe/json")
        self.copy( pattern="libQEJson.so*", dst="lib", src="src/qe/json", links=True)
        if self.settings.os == "Windows":
            libNames = ["QEJson", "libQEJson"]
            libExts = [".dll", ".lib", ".dll.a", ".pdb"]
            for libName in libNames:
                for libExt in libExts:
                    filePattern = "**/" + libName + libExt
                    self.copy( pattern=filePattern, dst="lib", src="src/qe/json", keep_path=False)

    def package_info(self):
        self.cpp_info.libs.extend(["QEJson"])
