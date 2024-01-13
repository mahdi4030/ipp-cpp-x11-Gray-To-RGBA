# General options
cmake_minimum_required(VERSION 3.15)
project(cpp-x11-gray-to-rgba)
set(CMAKE_BUILD_TYPE Release)

find_package(IPP)
find_package(OpenCV REQUIRED )

include_directories(${OpenCV_INCLUDE_DIRS} include)
file(GLOB SOURCES "src/*.cpp")

add_executable(makeshot src/main.cpp)
target_link_libraries(makeshot X11 ${OpenCV_LIBS})
set_property(TARGET makeshot PROPERTY CXX_STANDARD 11)

target_include_directories(makeshot PRIVATE ${IPP_INCLUDE_DIR} include)
target_link_libraries(makeshot rt pthread dl ${IPP_LIBRARIES})

install(TARGETS makeshot DESTINATION bin)

# CPack
INCLUDE(InstallRequiredSystemLibraries)
SET(CPACK_GENERATOR "DEB")
SET(CPACK_PACKAGE_INSTALL_DIRECTORY "makeshot 17.08.15.1")
SET(CPACK_PACKAGE_INSTALL_REGISTRY_KEY "makeshot 17.08.15.1")
SET(CPACK_PACKAGE_NAME "makeshot")
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Tool to make screenshots and save them to files")
SET(CPACK_PACKAGE_VENDOR "Butataki")
SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
SET(CPACK_PACKAGE_VERSION_MAJOR "17")
SET(CPACK_PACKAGE_VERSION_MINOR "08")
SET(CPACK_PACKAGE_VERSION_PATCH "15.1")
SET(CPACK_PACKAGE_INSTALL_DIRECTORY "CMake ${CMake_VERSION_MAJOR}.${CMake_VERSION_MINOR}")
SET(CPACK_PACKAGE_EXECUTABLES "makeshot")
SET(CPACK_SOURCE_GENERATOR "TGZ")
INCLUDE(CPack)
