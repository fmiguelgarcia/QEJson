# Requirements 
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()
find_package( Qt5Core REQUIRED)
set( CMAKE_AUTOMOC ON)
set( CMAKE_INCLUDE_CURRENT_DIR ON)

# Source
set( QEJsonS11n_MAYOR_VERSION 0)
set( QEJsonS11n_MINOR_VERSION 1)
set( QEJsonS11n_BUILD_VERSION 0)

set( QEJsonS11n_SRCS
	QEJsonS11n.cpp)

# Link
add_library( QEJsonS11n SHARED ${QEJsonS11n_SRCS})
target_link_libraries( QEJsonS11n Qt5::Core ${CONAN_LIBS}) 
set_target_properties( QEJsonS11n PROPERTIES 
	VERSION ${QEJsonS11n_MAYOR_VERSION}.${QEJsonS11n_MINOR_VERSION}.${QEJsonS11n_BUILD_VERSION}
	SOVERSION ${QEJsonS11n_MAYOR_VERSION}
	CXX_STANDARD 11)
