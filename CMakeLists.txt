cmake_minimum_required(VERSION 3.16)
project(socket)

enable_language(CXX)
set(CMAKE_CXX_STANDARD 11)

file(GLOB sources "${CMAKE_CURRENT_LIST_DIR}/src/*.cpp")
message(STATUS "This is BINARY dir " ${PROJECT_BINARY_DIR})

file(GLOB headers "${CMAKE_CURRENT_LIST_DIR}/src/*.h")

add_executable(server ${CMAKE_CURRENT_LIST_DIR}/src/server.cpp ${sources} ${headers})
#add_executable(client ${CMAKE_CURRENT_LIST_DIR}/src/client.cpp)

if (WIN32)
    set(OpenCV_DIR "C:\\MyProgram\\opencv-3.4.11\\mingw-build")
endif ()

find_package(OpenCV REQUIRED)
if (OpenCV_FOUND)
    include_directories(${OpenCV_INCLUDE_DIRS})
    target_link_libraries(server ${OpenCV_LIBS})
else (OpenCV_FOUND)
    message(FATAL_ERROR "OpenCV library not found!")
endif (OpenCV_FOUND)



