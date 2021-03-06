# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set (OBOE_DIR ${CMAKE_CURRENT_LIST_DIR}/../../../../external_libraries/oboe)
set (FLUIDSYNTH-PD ${CMAKE_CURRENT_LIST_DIR}/../../../../external_libraries/fluidsynth-pd)

# Debug utilities
set (DEBUG_UTILS_PATH "${OBOE_DIR}/samples/debug-utils")
set (DEBUG_UTILS_SOURCES ${DEBUG_UTILS_PATH}/trace.cpp)
include_directories(${DEBUG_UTILS_PATH})


### END OBOE INCLUDE SECTION ###


# App specific sources
set (APP_SOURCES
        jni_bridge.cpp
        HelloOboeEngine.cpp
        SoundGenerator.cpp
        LatencyTuningCallback.cpp
        FluidSynthesizer.cpp
        MidiMessage.cpp
        )


add_subdirectory(${OBOE_DIR} ./oboe)
add_subdirectory(${FLUIDSYNTH-PD} ./fluidsynthpd)

include_directories( ${OBOE_DIR}/include ${OBOE_DIR}/samples/shared)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

add_library( # Sets the name of the library.
        native-lib

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        native-lib.cpp
        ${APP_SOURCES}
        ${DEBUG_UTILS_SOURCES}

        )

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
        log-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        log)

add_library( fluidsynth
        SHARED
        IMPORTED )

set_target_properties( # Specifies the target library.
        fluidsynth

        # Specifies the parameter you want to define.
        PROPERTIES IMPORTED_LOCATION

        # Provides the path to the library you want to import.
       ${CMAKE_CURRENT_LIST_DIR}/../jniLibs/${ANDROID_ABI}/libfluidsynth.so )


# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
        native-lib

        # Links the target library to the log library
        # included in the NDK.
        ${log-lib}

        #fluidsynth build
        fluidsynth

        #oboe
        oboe
        )