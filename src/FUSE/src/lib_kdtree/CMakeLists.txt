cmake_minimum_required(VERSION 3.1)

# library name
project(lib_kdtree)

# Create variables with files to compile
set(SRCS
    alglibinternal.cpp
	alglibmisc.cpp
	ap.cpp
	dataanalysis.cpp
	diffequations.cpp
	fasttransforms.cpp
	integration.cpp
	interpolation.cpp
	linalg.cpp
	optimization.cpp
	solvers.cpp
	specialfunctions.cpp
	statistics.cpp
    )
    
set(HEADERS
    alglibinternal.h
	alglibmisc.h
	ap.h
	dataanalysis.h
	diffequations.h
	fasttransforms.h
	integration.h
	interpolation.h
	linalg.h
	optimization.h
	solvers.h
	specialfunctions.h
	statistics.h
	stdafx.h
    )

add_library(kdtree ${SRCS} ${HEADERS})