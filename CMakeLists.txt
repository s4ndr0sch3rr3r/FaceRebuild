cmake_minimum_required(VERSION 3.16)

project(FaceRebuild VERSION 0.1 LANGUAGES CXX)

# Enable automatic Qt features
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use Ninja response files to avoid long command lines issues
set(CMAKE_NINJA_FORCE_RESPONSE_FILE 1)

# Find Qt libraries
find_package(Qt6 REQUIRED COMPONENTS Widgets OpenGL)

# Set VTK directory
set(VTK_DIR "C:/GitRepo/VTK/vtk/build")

# Find VTK libraries and specify required components
find_package(VTK REQUIRED COMPONENTS
  CommonCore
  FiltersSources
  IOImage
  RenderingCore
  InteractionStyle
  RenderingFreeType
  RenderingOpenGL2
  GUISupportQt
)

# Find ITK libraries
find_package(ITK REQUIRED)

# Include VTK and ITK headers
include_directories(${VTK_INCLUDE_DIRS} ${ITK_INCLUDE_DIRS} include)

# Set source files and UI files paths
set(PROJECT_SOURCES
    src/main.cpp
    src/mainwindow.cpp
    src/filemanager.cpp
    src/imageprocessor.cpp
    src/itktovtkconverter.cpp
    src/renderermanager.cpp
    src/meshgenerator.cpp
    src/serialmeshgenerator.cpp
    src/parallelmeshgenerator.cpp
    src/scrollinteractorstyle.cpp
)

set(PROJECT_HEADERS
    include/mainwindow.h
    include/filemanager.h
    include/imageprocessor.h
    include/itktovtkconverter.h
    include/renderermanager.h
    include/meshgenerator.h
    include/IMeshGenerator.h
    include/serialmeshgenerator.h
    include/parallelmeshgenerator.h
    include/scrollinteractorstyle.h
)

set(PROJECT_UIS
    ui/mainwindow.ui
)

# Conditionally create the executable based on Qt version and platform
if(QT_VERSION_MAJOR GREATER 5)
    qt_add_executable(FaceRebuild
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES} ${PROJECT_HEADERS} ${PROJECT_UIS}
    )
else()
    if(ANDROID)
        add_library(FaceRebuild SHARED
            ${PROJECT_SOURCES} ${PROJECT_HEADERS} ${PROJECT_UIS}
        )
    else()
        add_executable(FaceRebuild
            ${PROJECT_SOURCES} ${PROJECT_HEADERS} ${PROJECT_UIS}


        )
    endif()
endif()

# Check if we are using MinGW, if so, remove the /bigobj flag
if(MINGW)
    string(REPLACE "/bigobj" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
endif()

# Link Qt, VTK, and ITK libraries
target_link_libraries(FaceRebuild PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(FaceRebuild PRIVATE ${VTK_LIBRARIES})
target_link_libraries(FaceRebuild PRIVATE ${ITK_LIBRARIES})

# Set properties for macOS and Windows executables
if(QT_VERSION_MAJOR LESS 6 OR (QT_VERSION_MAJOR EQUAL 6 AND QT_VERSION_MINOR LESS 1))
    set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.FaceRebuild)
endif()
set_target_properties(FaceRebuild PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# Install target
include(GNUInstallDirs)
install(TARGETS FaceRebuild
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Automatically copy required DLLs to the build directory
add_custom_command(TARGET FaceRebuild POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "Copying required DLLs..."
)

# Identify the build directory
set(BUILD_OUTPUT_DIR $<TARGET_FILE_DIR:FaceRebuild>)

# Helper function to copy DLLs from library paths
function(copy_dlls TARGET_NAME)
    foreach(lib ${ARGN})
        add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${lib} ${BUILD_OUTPUT_DIR}
            COMMENT "Copying ${lib} to build directory."
        )
    endforeach()
endfunction()

# Ensure DLLs from linked libraries are copied
foreach(lib ${QT_LIBRARIES} ${VTK_LIBRARIES} ${ITK_LIBRARIES})
    get_target_property(lib_path ${lib} IMPORTED_LOCATION)
    if(lib_path)
        add_custom_command(TARGET FaceRebuild POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${lib_path} ${BUILD_OUTPUT_DIR}
            COMMENT "Copying ${lib_path} to build directory."
        )
    endif()
endforeach()

# Finalize executable for Qt 6
if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(FaceRebuild)
endif()
