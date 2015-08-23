if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build type" FORCE)
endif()

find_package(Qt5Widgets REQUIRED)
find_package(Qt5Core REQUIRED)

if (LRCX_ENABLE_TESTS)
	find_package(Qt5Test REQUIRED)
endif()

