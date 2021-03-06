cmake_minimum_required (VERSION 2.6)

# compile in Release mode by default
SET(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build,
options are: None(CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release
RelWithDebInfo MinSizeRel.")

project (SparseBench C CXX)

INCLUDE_DIRECTORIES( ./benchmark/lib )
#SET(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/lib CACHE FILEPATH "" FORCE)

IF(CMAKE_COMPILER_IS_GNUCC)
	SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -funroll-loops")
ENDIF(CMAKE_COMPILER_IS_GNUCC)

SET(CMAKE_INSTALL_PREFIX ./bin/)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -funroll-loops")
SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-rg")
SET(CMAKE_CXX_FLAGS_DEBUG "-pg")
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
INCLUDE(FindMKL.cmake)

set(INSTALL_TO_DIR ${CMAKE_SOURCE_DIR}/build)

INCLUDE_DIRECTORIES( ${CMAKE_SOURCE_DIR}/benchmark/lib )
INCLUDE_DIRECTORIES( ${MKL_INCLUDE_DIR} )

SET(common_sources
  ./benchmark/lib/mmio.c
  ./benchmark/lib/common.c
  ./benchmark/lib/timer.cpp
)

ADD_LIBRARY(common SHARED ${common_sources})
SET_TARGET_PROPERTIES(common PROPERTIES DEBUG_POSTFIX -g)
SET_TARGET_PROPERTIES(common PROPERTIES MINSIZEREL_POSTFIX -ms)
SET_TARGET_PROPERTIES(common PROPERTIES RELWITHDEBINFO_POSTFIX -rg)
SET_TARGET_PROPERTIES(common PROPERTIES OUTPUT_NAME common)
IF( CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64")
    SET_TARGET_PROPERTIES(common PROPERTIES COMPILE_FLAGS "-fPIC")
ENDIF( CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" )
INSTALL(TARGETS common DESTINATION ${INSTALL_TO_DIR})


MACRO(ADD_BENCHMARK name filename)
    ADD_EXECUTABLE(${name} ${filename})
    TARGET_LINK_LIBRARIES(${name} ${MKL} common  -lpthread -lm -lrt)
ENDMACRO(ADD_BENCHMARK name filename)


ADD_BENCHMARK(cg-mkl-rci
  ./benchmark/cg/mkl-rci/cg.cpp
)

ADD_BENCHMARK(cg-mkl-explicit
  ./benchmark/cg/mkl-explicit/cg.cpp
)

ADD_BENCHMARK(cg-3term-mkl
  ./benchmark/cg/3term-mkl/cg.cpp
)

# Add MaxCompiler Benchmarks

add_custom_target(
   fpga_naive_simbuild # ALL <--- if to be run always (target adds to a list of defaults)
   COMMAND make runsim # TODO this should only build DFE part of simulation
   WORKING_DIRECTORY ../benchmark/spmv/fpgaNaive/build
)

if (DEFINED ENV{MAXCOMPILERDIR} AND DEFINED ENV{MAXELEROSDIR})
      message(STATUS "MAXCOMPILERDIR and MAXELEROSDIR are set, adding includes!")
      include_directories ($ENV{MAXCOMPILERDIR}/include)
      include_directories ($ENV{MAXCOMPILERDIR}/include/slic)
      include_directories ($ENV{MAXELEROSDIR}/include)
      link_directories($ENV{MAXCOMPILERDIR}/lib)
      link_directories($ENV{MAXELEROSDIR}/lib)
else()
    message(STATUS "MAXCOMPILERDIR not set!")
endif()

add_subdirectory(benchmark/spmv/fpgaNaive)
