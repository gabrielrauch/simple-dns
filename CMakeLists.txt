cmake_minimum_required(VERSION 3.0)

project(simple-dns)

set(SOURCES
    src/main.cpp
    src/dns_server.cpp
    )

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)

configure_file(data/dns_records.txt data/dns_records.txt COPYONLY)
