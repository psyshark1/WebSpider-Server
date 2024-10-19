cmake_minimum_required(VERSION 3.8)

project(httpserver)
set(CMAKE_CXX_STANDARD 17)
add_compile_definitions(_WIN32_WINNT=0x0A01)

set(libpqxx_DIR "D:\\dev\\libpqxx-7.9.2")
set(OPENSSL_ROOT_DIR "D:\\dev\\openssl-3.3.2\\x64")

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)

set(BOOST_ROOT "D:\\dev\\boost_1_85")

find_package(Boost REQUIRED COMPONENTS system thread)

if(NOT Boost_FOUND)
    message(FATAL_ERROR "Could not find Boost")
endif()

find_package(OpenSSL REQUIRED)
add_subdirectory(${libpqxx_DIR} "libpqxx-build") 
set(PostgreSQL_ADDITIONAL_VERSIONS "11")

add_executable(httpserver http_server.h http_server.cpp main.cpp)

#add_library(httpserver STATIC ${HTTPSERV})
target_link_libraries(httpserver PUBLIC ${Boost_LIBRARIES} OpenSSL::SSL libpqxx::pqxx)
target_include_directories(httpserver PRIVATE ${Boost_INCLUDE_DIRS})
target_compile_features(httpserver PRIVATE cxx_std_17) 