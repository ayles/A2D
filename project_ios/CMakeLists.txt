cmake_minimum_required(VERSION 3.3.2)

set(CMAKE_CXX_STANDARD 11)
project(A2D)

enable_language(Swift)
include(BundleUtilities)

set(IOS TRUE)

set(CODESIGNIDENTITY "iPhone Developer")
set(DEPLOYMENT_TARGET 9.0)

set(APP_NAME A2D_IOS)
set(BUNDLE_IDENTIFIER "com.selya")
set(PRODUCT_NAME ${APP_NAME})
set(EXECUTABLE_NAME ${APP_NAME})
set(MACOSX_BUNDLE_EXECUTABLE_NAME ${APP_NAME})
set(MACOSX_BUNDLE_INFO_STRING ${BUNDLE_IDENTIFIER})
set(MACOSX_BUNDLE_GUI_IDENTIFIER ${BUNDLE_IDENTIFIER})
set(MACOSX_BUNDLE_BUNDLE_NAME ${BUNDLE_IDENTIFIER})
set(MACOSX_BUNDLE_ICON_FILE "")
set(MACOSX_BUNDLE_LONG_VERSION_STRING "1.0")
set(MACOSX_BUNDLE_SHORT_VERSION_STRING "1.0")
set(MACOSX_BUNDLE_BUNDLE_VERSION "1.0")
set(MACOSX_BUNDLE_COPYRIGHT "Copyright Selya")
set(MACOSX_DEPLOYMENT_TARGET "8.0")

set(APP_SOURCE_FILES
        AppDelegate.swift
        ViewController.swift
        )

set(RESOURCES
        Main.storyboard
        LaunchScreen.storyboard
        )

# Process resources
set(CORE_PROJECT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../project_core")

file(GLOB RESOURCE_FILES RELATIVE "${CORE_PROJECT_PATH}" "${CORE_PROJECT_PATH}/resources/**/*.*")
foreach (FILE ${RESOURCE_FILES})
    string(REPLACE "/" "_" FILE_NAME ${FILE})
    configure_file("${CORE_PROJECT_PATH}/${FILE}" "${CMAKE_CURRENT_BINARY_DIR}/resources/${FILE_NAME}" COPYONLY)
    set(RESOURCE_FILES_PROCESSED ${RESOURCE_FILES_PROCESSED} "${CMAKE_CURRENT_BINARY_DIR}/resources/${FILE_NAME}")
endforeach ()

set(RESOURCES ${RESOURCES} ${RESOURCE_FILES_PROCESSED})

add_executable(
        ${APP_NAME}
        MACOSX_BUNDLE
        ${APP_SOURCE_FILES}
        ${RESOURCES}
)


# Set the C++ library to the the standard library
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -miphoneos-version-min=${DEPLOYMENT_TARGET} -fobjc-arc -stdlib=libc++ -isysroot ${CMAKE_OSX_SYSROOT}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -miphoneos-version-min=${DEPLOYMENT_TARGET} -std=c++11 -fobjc-arc -stdlib=libc++ -isysroot ${CMAKE_OSX_SYSROOT}")
set(CMAKE_Swift_FLAGS "${CMAKE_Swift_FLAGS} -import-objc-header BindingHeader.h")

# Locate system libraries on iOS
find_library(UIKIT UIKit)
find_library(GLKIT GLKit)
find_library(FOUNDATION Foundation)
find_library(COREGRAPHICS CoreGraphics)
find_library(MOBILECORESERVICES MobileCoreServices)
find_library(CFNETWORK CFNetwork)
find_library(SYSTEMCONFIGURATION SystemConfiguration)
find_library(SECURITY Security)
find_library(AVFOUNDATION AVFoundation)
find_library(AUDIOTOOLBOX AudioToolbox)
find_library(ACCELERATE Accelerate)

# link the libraries loaded above
target_link_libraries(${APP_NAME} ${UIKIT})
target_link_libraries(${APP_NAME} ${GLKIT})
target_link_libraries(${APP_NAME} ${FOUNDATION})
target_link_libraries(${APP_NAME} ${COREGRAPHICS})
target_link_libraries(${APP_NAME} ${MOBILECORESERVICES})
target_link_libraries(${APP_NAME} ${CFNETWORK})
target_link_libraries(${APP_NAME} ${SYSTEMCONFIGURATION})
target_link_libraries(${APP_NAME} ${SECURITY})
target_link_libraries(${APP_NAME} ${AVFOUNDATION})
target_link_libraries(${APP_NAME} ${AUDIOTOOLBOX})
target_link_libraries(${APP_NAME} libicucore.dylib)
target_link_libraries(${APP_NAME} ${ACCELERATE})

# Link in the cpp standard library
target_link_libraries(${APP_NAME} c++)


# Create the app target
set_target_properties(${APP_NAME} PROPERTIES
        XCODE_ATTRIBUTE_DEBUG_INFORMATION_FORMAT "dwarf-with-dsym"
        RESOURCE "${RESOURCES}"
        XCODE_ATTRIBUTE_ENABLE_BITCODE NO
        XCODE_ATTRIBUTE_IPHONEOS_DEPLOYMENT_TARGET ${DEPLOYMENT_TARGET}
        XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY ${CODESIGNIDENTITY}
        MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/Info.plist
        XCODE_ATTRIBUTE_TARGETED_DEVICE_FAMILY "1"
        XCODE_ATTRIBUTE_CLANG_ENABLE_OBJC_ARC YES
        XCODE_ATTRIBUTE_COMBINE_HIDPI_IMAGES NO
        XCODE_ATTRIBUTE_LD_RUNPATH_SEARCH_PATHS "@executable_path/Frameworks"
        XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER ${BUNDLE_IDENTIFIER}
        XCODE_ATTRIBUTE_CODE_SIGNING_REQUIRED "NO")

add_subdirectory(${CORE_PROJECT_PATH} build_core)

add_library(a2d_ios_library EngineInterface.cpp)

target_link_libraries(a2d_ios_library a2d_core_library)
target_link_libraries(${APP_NAME} a2d_ios_library)