cmake_minimum_required(VERSION 3.16)
project(cutievolume VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt6 REQUIRED COMPONENTS Core Quick DBus)
pkg_check_modules(LIBPULSE libpulse REQUIRED IMPORTED_TARGET)
qt_standard_project_setup()

set(lib_SOURCES
    src/cutievolume.h
    src/cutievolume.cpp)

qt6_add_library(cutievolume
    SHARED ${lib_SOURCES}
)

set_target_properties(cutievolume PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(cutievolume PROPERTIES SOVERSION 0)
set_target_properties(cutievolume PROPERTIES PUBLIC_HEADER src/cutievolume.h)

target_link_libraries(cutievolume PUBLIC
    Qt::Core
    Qt::Quick
    Qt::DBus
    PkgConfig::LIBPULSE
)

configure_file(src/cutievolume.pc.in cutievolume.pc @ONLY)

install(TARGETS cutievolume
    RUNTIME DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    BUNDLE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")

install(FILES ${CMAKE_BINARY_DIR}/cutievolume.pc DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)
