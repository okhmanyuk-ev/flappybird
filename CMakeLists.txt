cmake_minimum_required(VERSION 3.8.0)
project(flappybird)

set(PRODUCT_NAME "Flappy Bird")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# main

if(BUILD_DEVELOPER)
	add_definitions(-DBUILD_DEVELOPER)
endif()

add_definitions(-DPROJECT_NAME="${PROJECT_NAME}")
add_definitions(-DPRODUCT_NAME="${PRODUCT_NAME}")

file(GLOB MAIN_SRC
	src/*.cpp
	src/*.h
)

source_group("all" FILES ${MAIN_SRC})

file(GLOB ALL_SRC
	${MAIN_SRC}
)

if(WIN32)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
    add_definitions(-DNOMINMAX)

    add_executable(${PROJECT_NAME}
        ${ALL_SRC}
    )

    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
    set_target_properties(${PROJECT_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
elseif(APPLE)
    set(CMAKE_MACOSX_BUNDLE YES)
    set(CMAKE_OSX_SYSROOT iphoneos)

    file(GLOB MAIN_SRC
        src/*.cpp
        src/*.h
        ios/*.m
        ios/*.mm
        ios/*.h
    )

    set(RESOURCES
        ios/Main.storyboard
        ios/LaunchScreen.storyboard
        assets
    )

    add_executable(${PROJECT_NAME}
        ${ALL_SRC}
        ${RESOURCES}
    )

    set_target_properties(${PROJECT_NAME} PROPERTIES
        RESOURCE "${RESOURCES}"
        XCODE_ATTRIBUTE_IPHONEOS_DEPLOYMENT_TARGET "12.0"
        XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "iPhone Developer"
        XCODE_ATTRIBUTE_DEVELOPMENT_TEAM "69RWHMDZA7"
        XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER "com.skymunge.${PROJECT_NAME}"
        XCODE_ATTRIBUTE_TARGETED_DEVICE_FAMILY "1,2"
        MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/ios/Info.plist
    )

    target_link_libraries(${PROJECT_NAME}
        "-framework UIKit"
        "-framework CoreGraphics"
        "-framework GLKit"
        "-framework OpenGLES"
        "-framework Foundation"
        "-framework AVFoundation"
        "-framework AudioToolbox"
    )
elseif(EMSCRIPTEN)
	set(ASSETS_FOLDER_NAME assets)
	set(ASSETS_FOLDER_PATH ${CMAKE_SOURCE_DIR}/${ASSETS_FOLDER_NAME})

	add_executable(${PROJECT_NAME} ${ALL_SRC} ${ASSETS_FOLDER_PATH})

	set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "index")
	set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/output)
	set(CMAKE_EXECUTABLE_SUFFIX .html)

	target_link_options(${PROJECT_NAME} PUBLIC --preload-file ${ASSETS_FOLDER_PATH}@${ASSETS_FOLDER_NAME})
endif()

# sky

add_subdirectory(sky)
target_link_libraries(${PROJECT_NAME} sky)
copy_required_libs(${PROJECT_NAME})