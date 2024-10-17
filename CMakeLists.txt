cmake_minimum_required(VERSION 3.5)

project(bulk LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(bulk-async
    main.cpp
    commandhandler.h commandhandler.cpp
    asyncsoc.h asyncsoc.cpp
    loggerinterface.h
    consolelogger.h consolelogger.cpp
    )
