set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(TOAST_PUBLIC_HEADERS
    Toast.h
    toast_global.h
)

set(TOAST_PRIVATE_HEADERS
    ToastImpl.h
)

set(TOAST_SOURCES
    Toast.cpp
    ToastImpl.cpp
)

set(TOAST_FORMS
    Toast.ui
)

add_library(Toast SHARED
    ${TOAST_SOURCES}
    ${TOAST_PUBLIC_HEADERS}
    ${TOAST_PRIVATE_HEADERS}
    ${TOAST_FORMS}
)

add_library(Toast::Toast ALIAS Toast)

target_compile_definitions(Toast
    PRIVATE
        TOAST_LIB
)

target_link_libraries(Toast
    PRIVATE
        Qt${QT_VERSION_MAJOR}::Widgets
)

target_include_directories(Toast
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/Toast>
)

install(TARGETS Toast
    EXPORT ToastTargets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(FILES
    ${TOAST_PUBLIC_HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/Toast
)

install(EXPORT ToastTargets
    FILE ToastTargets.cmake
    NAMESPACE Toast::
    DESTINATION ${TOAST_INSTALL_CMAKEDIR}
)

