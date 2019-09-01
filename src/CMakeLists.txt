
# src allocator

cmake_minimum_required(VERSION 3.2)

set(THIS allocator)
set(SOURCES main.cpp)
set(LIBS liballoc)

if (DEFINED ENV{TRAVIS_BUILD_NUMBER})
    project(${THIS} VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})
else()
    project(${THIS} VERSION 0.0.1)
endif()

add_executable(${THIS} ${SOURCES})

set_target_properties(
        ${THIS}
        PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
)

target_include_directories(
        ${THIS}
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../lib
)

target_link_libraries(
        ${THIS}
        ${LIBS}
)

if (MSVC)

    target_compile_options(
        ${THIS}
        PRIVATE
        /W4
    )

else()

    target_compile_options(
        ${THIS}
        PRIVATE
        -Wall -Wextra -pedantic -Werror
    )

endif()

install(TARGETS ${THIS} RUNTIME DESTINATION bin)


