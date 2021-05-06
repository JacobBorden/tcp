cmake_minimum_required(VERSION 3.0.0)
project(websocket-libtcp VERSION 0.1.0)

set(CMAKE_CXX_VISABILITY_PRESET hidden)
set(CMAKE_VISABILITY_INLINES_HIDDEN yes)

include(CTest)
enable_testing()

set(CMAKE_INSTALL_RPATH $ORIGIN)

add_library(${PROJECT_NAME} SHARED src/${PROJECT_NAME}.cpp)

include(GenerateExportHeader)
target_include_directories (${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
generate_export_header(${PROJECT_NAME} EXPORT_FILE_NAME ${CMAKE_CURRENT_SOURCE_DIR}/src/${PROJECT_NAME}_export.h)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

install(TARGETS ${PROJECT_NAME})

include(CMakePackageConfigHelpers)
write_basic_package_version_file(${PROJECT_NAME}ConfigVersion.cmake VERSION ${PROJECT_VERSION} COMPATIBILITY SameMajorVersion)
export(PACKAGE ${PROJECT_NAME})