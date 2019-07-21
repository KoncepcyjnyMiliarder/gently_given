cmake_minimum_required(VERSION 3.0)

project(gently_given)

set(CMAKE_CXX_STANDARD 14)

set(INCLUDES
  inc/hasher.hpp
  inc/structs.hpp
  inc/export_table.hpp
  inc/find_module.hpp
  inc/find_export.hpp
)

set(SOURCES
  src/export_table.cpp
  src/find_module.cpp
  src/find_export.cpp
)

add_library(${PROJECT_NAME} STATIC ${SOURCES} ${INCLUDES})
target_include_directories(${PROJECT_NAME} PUBLIC inc)

if(BUILD_TESTING)
find_package(GTest REQUIRED)

set(SOURCES_UT
  ut/hasher_ut.cpp
  ut/export_table_ut.cpp  
  ut/find_module_ut.cpp
  ut/find_export_ut.cpp
)

add_executable(${PROJECT_NAME}_ut ${SOURCES_UT} ${INCLUDES})
target_include_directories(${PROJECT_NAME}_ut PRIVATE ${GTEST_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME}_ut PRIVATE ${PROJECT_NAME} ${GTEST_BOTH_LIBRARIES})
endif()