project(QAnnotationTest)
cmake_minimum_required(VERSION 3.0.0)
include(${CMAKE_SOURCE_DIR}/../tools/cmake/common.cmake)

add_executable( packageTest main.cpp)
target_link_libraries( packageTest Qt5::Core ${CONAN_LIBS})
target_compile_features( packageTest PUBLIC cxx_auto_type cxx_nullptr cxx_lambdas)
