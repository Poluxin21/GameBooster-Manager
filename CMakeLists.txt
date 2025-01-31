cmake_minimum_required(VERSION 3.10)

project(GameBooster-Manager LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SOURCE_DIR "${CMAKE_SOURCE_DIR}/")
set(CORE_DIR "${CMAKE_SOURCE_DIR}/core")
set(UTILS_DIR "${CMAKE_SOURCE_DIR}/utils")
set(HEADER_DIR "${CMAKE_SOURCE_DIR}/header")

file(GLOB_RECURSE SOURCES
    "${SOURCE_DIR}/main.cpp"
    "${CORE_DIR}/*.cpp"
    "${UTILS_DIR}/*.cpp"
)

include_directories(
    ${HEADER_DIR}
)

add_executable(GameBoosterManager ${SOURCES})

# target_link_libraries(GameBoosterManager PRIVATE ${OpenCV_LIBS})

message(STATUS "Projeto configurado com sucesso. Use 'make' para compilar.")
