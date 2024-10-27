cmake_minimum_required(VERSION 3.8)

project(WebSpider_Server)
set(CMAKE_CXX_STANDARD 17)
add_compile_definitions(_WIN32_WINNT=0x0A01)

set(libpqxx_DIR "D:\\dev\\libpqxx-7.9.2")
set(OPENSSL_ROOT_DIR "D:\\dev\\openssl-3.3.2\\x64")

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)

set(BOOST_ROOT "D:\\dev\\boost_1_85")

find_package(Boost REQUIRED COMPONENTS system thread locale)

if(NOT Boost_FOUND)
    message(FATAL_ERROR "Could not find Boost")
endif()

find_package(OpenSSL REQUIRED)
add_subdirectory(${libpqxx_DIR} "libpqxx-build") 
set(PostgreSQL_ADDITIONAL_VERSIONS "11")

include_directories(INI_Parser)
include_directories(ExceptINI)

add_executable(WebSpider_Server stringstructs.h http_server.h http_server.cpp main.cpp)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

add_subdirectory(INI_Parser)
add_subdirectory(ExceptINI)

#add_library(httpserver STATIC ${HTTPSERV})
target_link_libraries(WebSpider_Server PUBLIC iniparser ${Boost_LIBRARIES} OpenSSL::SSL libpqxx::pqxx)
target_include_directories(WebSpider_Server PRIVATE ${Boost_INCLUDE_DIRS})
target_compile_features(WebSpider_Server PRIVATE cxx_std_17) 