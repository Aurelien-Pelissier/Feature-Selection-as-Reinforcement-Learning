cmake_minimum_required(VERSION 3.1)

# project name
project(FUSE)

#folders
add_subdirectory(lib_kdtree)

# C++ 14 requirement
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Create variables with files to compile
set(SRCS
	01_Main.cpp
	02_Main_FEATURE_SELECTION.cpp
	class.cpp
	dataset.cpp
	kNN.cpp
	mainf_iterate.cpp
	mainf_iterate_random.cpp
	mainf_reward.cpp
	mainf_UCB.cpp
	print.cpp
	quickselect.cpp
	select_features.cpp
	update.cpp
	)
	
set(HEADERS
    class_Node.h
	class_Params.h
	class_Path.h
	class_Tree.h
	class_Variables.h
	dataset.h
	kNN.h
	mainf.h
	print.h
	quickselect.h
	select_features.h
	update.h
    )

	
# executable	
add_executable(FUSE ${SRCS} ${HEADERS})	
	
	
# Adding libraries	
target_link_libraries(FUSE kdtree)

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