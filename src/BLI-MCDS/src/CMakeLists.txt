cmake_minimum_required(VERSION 3.1)

# project name
project(BLI-MCDS)

#folders
add_subdirectory(realdata)

# C++ 14 requirement
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Create variables with files to compile
set(SRCS
	01_Main.cpp
	02_Run_BAI_MCLS.cpp
	Backpropagate.cpp
	Build_Graph.cpp
	class.cpp
	Evaluation.cpp
	Expand_graph.cpp
	print.cpp
	Pull_representative_leaf.cpp
	Random_phase.cpp
	UGapE.cpp
	Update.cpp
	)
	
set(HEADERS
    class_Node.h
	class_Params.h
	class_Results.h
	class_Tree.h
	class_Variables.h
	functions.h
	print.h
    )

	
# executable	
add_executable(BLI-MCDS ${SRCS} ${HEADERS})	
	
	
# Adding libraries	
target_link_libraries(BLI-MCDS realdata)

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