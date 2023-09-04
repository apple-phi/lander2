cmake_minimum_required(VERSION 3.14 FATAL_ERROR) # Required by GoogleTest
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

# Subdirs
add_subdirectory(src)

# App
file(GLOB_RECURSE SRC_SOURCES LIST_DIRECTORIES true ./src/**)
message("App *.*pp: ${SRC_SOURCES}")
add_executable(LanderApp ${SRC_SOURCES})
include_directories(./include)
target_include_directories(LanderApp PRIVATE ${glm_SOURCE_DIR})
target_link_libraries(LanderApp PRIVATE glfw glm::glm glbinding::glbinding glbinding::glbinding-aux)
# target_compile_definitions(LanderApp PRIVATE GLM_FORCE_MESSAGES)
