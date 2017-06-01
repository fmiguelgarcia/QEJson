# Requirements 
include(${CMAKE_SOURCE_DIR}/tools/cmake/common.cmake)

# Source
set( QEJson_MAYOR_VERSION 1)
set( QEJson_MINOR_VERSION 0)
set( QEJson_BUILD_VERSION 0)

set( QEJson_SRCS
	Global.hpp 
	TypeTraits.hpp
	S11nContext.cpp 
	SaveHelper.cpp 
	LoadHelper.cpp
	QEJson.cpp)
add_definitions( -DQEJSON_LIBRARY)

# Link
add_library( QEJson SHARED ${QEJson_SRCS})
target_link_libraries( QEJson 
	Qt5::Core ${CONAN_LIBS}) 
set_target_properties( QEJson PROPERTIES 
	VERSION ${QEJson_MAYOR_VERSION}.${QEJson_MINOR_VERSION}.${QEJson_BUILD_VERSION}
	SOVERSION ${QEJson_MAYOR_VERSION})
target_compile_features( QEJson PUBLIC cxx_auto_type cxx_nullptr cxx_lambdas)
