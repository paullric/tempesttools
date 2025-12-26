# Copyright (c) 2025      Kian Huang, Hongyu Chen
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

list(APPEND ACCUMULATE_STAGE_IV_FILES
  AccumulateStageIV.cpp
)

list(APPEND EFFECTIVE_RESOLUTION_FILES
  EffectiveResolution.cpp
)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/../base
  ${CMAKE_CURRENT_SOURCE_DIR}/../netcdf-cxx-4.2
  ${NetCDF_C_INCLUDE_DIR}
  ${MPI_CXX_INCLUDE_DIRS}
)

add_executable(AccumulateStageIV ${ACCUMULATE_STAGE_IV_FILES})
target_link_libraries(AccumulateStageIV PUBLIC extremesbase netcdf_c++ ${MPI_CXX_LIBRARIES})

add_executable(EffectiveResolution ${EFFECTIVE_RESOLUTION_FILES})
target_link_libraries(EffectiveResolution PUBLIC extremesbase netcdf_c++ ${MPI_CXX_LIBRARIES})

install(
  TARGETS
    AccumulateStageIV
    EffectiveResolution
  RUNTIME DESTINATION bin
)

