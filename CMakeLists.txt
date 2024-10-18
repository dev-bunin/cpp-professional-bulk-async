cmake_minimum_required(VERSION 3.5)

set(PATCH_VERSION "2" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

project(async_static VERSION ${PROJECT_VESRION})
project(async VERSION ${PROJECT_VESRION})

set(FILES
    commandhandler.h commandhandler.cpp
    async.h async.cpp
    loggerinterface.h
    consolelogger.h consolelogger.cpp
    filelogger.h filelogger.cpp
    )

add_executable(bulk main.cpp)
add_library(async_static STATIC ${FILES})
add_library(async SHARED ${FILES})


set_target_properties(bulk async async_static PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    )

target_include_directories(bulk
    PRIVATE "${CMAKE_BINARY_DIR}"
    )

target_link_libraries(bulk PRIVATE
    async
    )

if (MSVC)
    target_compile_options(bulk PRIVATE
        /W4
        )
    target_compile_options(async PRIVATE
        /W4
        )
    target_compile_options(async_static PRIVATE
        /W4
        )
else ()
    target_compile_options(bulk PRIVATE
        -Wall -Wextra -pedantic -Werror
        )
    target_compile_options(async PRIVATE
        -Wall -Wextra -pedantic -Werror
        )
    target_compile_options(async_static PRIVATE
        -Wall -Wextra -pedantic -Werror
        )
endif()

install(TARGETS async_static RUNTIME DESTINATION lib)
install(TARGETS async RUNTIME DESTINATION lib)
install(TARGETS bulk RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)


include(CPack)
