
# root allocator

cmake_minimum_required(VERSION 3.2)

set (THIS prj_allocator)

if (DEFINED ENV{TRAVIS_BUILD_NUMBER})
    project(${THIS} VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})
else()
    project(${THIS} VERSION 0.0.1)
endif()

add_subdirectory(src)
add_subdirectory(lib)

if (NOT DEFINED ENV{PC_00330_80000_00000_AA472})
    
    add_subdirectory(test)

    set(CPACK_GENERATOR DEB)

    set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
    set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
    set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

    set(CPACK_PACKAGE_CONTACT cdandrey@gmail.com)

    include(CPack)

endif()

