cmake_minimum_required(VERSION 3.1)

# project name
project(realdata)

#folders
add_subdirectory(lib_kdtree)

# Create variables with files to compile
set(SRCS
	dataset.cpp
	kNN.cpp
	quickselect.cpp
	reward.cpp
	select_features.cpp
	)
	
set(HEADERS
    class_Node.h
	class_Params.h
	class_Tree.h	
	dataset.h
	kNN.h	
	reward.h
	quickselect.h
	select_features.h
    )

	
# executable	
add_library(realdata ${SRCS} ${HEADERS})	
	
	
# Adding libraries	
target_link_libraries(realdata kdtree)

# Boost = https://stackoverflow.com/questions/24173330/cmake-is-not-able-to-find-boost-libraries


SET (BOOST_MIN_VERSION "1.55.0")
set (Boost_NO_BOOST_CMAKE ON)
FIND_PACKAGE(Boost ${BOOST_MIN_VERSION} REQUIRED)
if (NOT Boost_FOUND)
  message(FATAL_ERROR "Fatal error: Boost (version >= 1.55) required.")
else()
  message(STATUS "Setting up BOOST")
  message(STATUS " Includes - ${Boost_INCLUDE_DIRS}")
  message(STATUS " Library  - ${Boost_LIBRARY_DIRS}")
  include_directories(${Boost_INCLUDE_DIRS})
  link_directories(${Boost_LIBRARY_DIRS})
endif (NOT Boost_FOUND)