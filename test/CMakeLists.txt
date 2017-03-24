include(${CMAKE_SOURCE_DIR}/tools/cmake/common.cmake)
find_package( Qt5Test)

set( QEJsonTest_SRCS 
	entity/book.cpp entity/chapter.cpp
	QEJsonTest.cpp)

add_executable( QEJsonTest ${QEJsonTest_SRCS})
target_link_libraries( QEJsonTest 
	QEJson 
	Qt5::Core Qt5::Test 
	${CONAN_LIBS})

add_test( NAME QEJsonTest COMMAND  $<TARGET_FILE:QEJsonTest>)
