cmake_minimum_required(VERSION 3.16)

project(Toast VERSION 1.0.0 LANGUAGES CXX)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

set(TOAST_INSTALL_CMAKEDIR "${CMAKE_INSTALL_LIBDIR}/cmake/Toast")

add_subdirectory(Toast)

configure_package_config_file(
	"${CMAKE_CURRENT_SOURCE_DIR}/cmake/ToastConfig.cmake.in"
	"${CMAKE_CURRENT_BINARY_DIR}/ToastConfig.cmake"
	INSTALL_DESTINATION "${TOAST_INSTALL_CMAKEDIR}"
)

file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/ToastConfigVersion.cmake"
"set(PACKAGE_VERSION \"${PROJECT_VERSION}\")\n"
"\n"
"if(PACKAGE_FIND_VERSION_MAJOR STREQUAL \"${PROJECT_VERSION_MAJOR}\")\n"
"  set(PACKAGE_VERSION_COMPATIBLE TRUE)\n"
"else()\n"
"  set(PACKAGE_VERSION_COMPATIBLE FALSE)\n"
"endif()\n"
"\n"
"if(PACKAGE_FIND_VERSION STREQUAL PACKAGE_VERSION)\n"
"  set(PACKAGE_VERSION_EXACT TRUE)\n"
"endif()\n"
)

install(FILES
	"${CMAKE_CURRENT_BINARY_DIR}/ToastConfig.cmake"
	"${CMAKE_CURRENT_BINARY_DIR}/ToastConfigVersion.cmake"
	DESTINATION "${TOAST_INSTALL_CMAKEDIR}"
)
