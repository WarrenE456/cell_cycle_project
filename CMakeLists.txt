cmake_minimum_required(VERSION 3.25.1)
# If the project name is changed also update the run.sh
project(example)

# Add all files in src and glad.c to source files for executable
file(GLOB SRC_FILES src/*)
add_executable(${PROJECT_NAME} ${SRC_FILES} external/glad/src/glad.c)

#       DOWNLOAD ALL SUBMODULES

option(GIT_SUBMODULE "Download and check submodules during build" ON)
if (GIT_SUBMODULE)
    message(STATUS "Submodule update")
    execute_process(
        COMMAND git submodule update --init --recursive
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        RESULT_VARIABLE SUBMODULE_RESULT)
    if (NOT ${SUBMODULE_RESULT} EQUAL "0")
        MESSAGE(FATAL_ERROR "git submodule update --init --recursive failed with ${GIT_SUBMODULE_RESULT}")
    endif()
endif()

#       CHECK ALL SUBMODULES

if (NOT EXISTS ${PROJECT_SOURCE_DIR}/external/glfw/CMakeLists.txt)
    message(FATAL_ERROR "The glfw submodule was not downloaded. GIT_SUBMODULE turned OFF.")
endif()

# Add glfw cmake directory
add_subdirectory(external/glfw)

# Add directory with library fies
target_link_directories(${PROJECT_NAME} PUBLIC external/glfw/src)
# Link the library files
target_link_libraries(${PROJECT_NAME} glfw)
