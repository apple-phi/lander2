cmake_minimum_required(VERSION 3.14.7 FATAL_ERROR)
project(lander2 VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 23)  
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_VERBOSE_MAKEFILE ON)
cmake_policy(SET CMP0135 NEW) # Fix download issues for external libs

find_package(glbinding REQUIRED)
find_package(glfw3 REQUIRED)

# Enable CTest before adding subdirectories
# https://stackoverflow.com/questions/54550939/ctest-add-tests-in-subdirectories#comment109191766_55835742
include(CTest)
enable_testing()

include(FetchContent)
set(FETCHCONTENT_QUIET OFF)

# CPack
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

# GLM
FetchContent_Declare(
  glm
  URL https://github.com/g-truc/glm/releases/download/0.9.9.8/glm-0.9.9.8.zip
)
FetchContent_MakeAvailable(glm)
message(STATUS "Found GLM at: " ${glm_SOURCE_DIR})

# stb
FetchContent_Declare(
  stb
  URL https://github.com/nothings/stb/archive/refs/heads/master.zip
)
FetchContent_MakeAvailable(stb)
message(STATUS "Found STB at: " ${stb_SOURCE_DIR})
include_directories(${stb_SOURCE_DIR})

# assets
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})


# Subdirs
add_subdirectory(src)
add_subdirectory(src/graphics/assets)

# App
file(GLOB_RECURSE SRC_SOURCES LIST_DIRECTORIES true src/**)
message("App *.*pp: ${SRC_SOURCES}")
add_executable(LanderApp ${SRC_SOURCES} src/graphics/atmosphere/model.cc src/graphics/helper.cpp)
include_directories(./include ./src/graphics)
target_include_directories(LanderApp PRIVATE ${glm_SOURCE_DIR})
target_link_libraries(LanderApp PRIVATE glfw glm::glm glbinding::glbinding) # glbinding::glbinding-aux
# target_compile_definitions(LanderApp PRIVATE GLM_FORCE_MESSAGES)
