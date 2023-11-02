cmake_minimum_required(VERSION 3.0)  

project(simple-dns)


set(SOURCES
    src/main.cpp
    src/dns_server.cpp
    )


set(HEADERS
    include/dns_server.h
    )


configure_file(data/dns_records.json data/dns_records.json COPYONLY)


add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
