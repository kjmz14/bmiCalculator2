cmake_minimum_required(VERSION 3.24)

project(bmiCalculator2 VERSION 1.2.0)

set(CMAKE_C_STANDARD 90)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

add_executable(
        bmiCalculator2 WIN32
        res/bmiCalculator2.exe.manifest # Manifest
        res/res.h # Resources header
        # Sources
        src/main.c
        src/bmi.c src/bmi.h
        src/dlgproc.c src/dlgproc.h
        src/strings.c src/strings.h
        src/aboutproc.c src/aboutproc.h
)

target_sources(
        bmiCalculator2 PRIVATE
        res/icons/icons.rc
        res/version.rc
        res/strings.rc
        res/dialog.rc
        res/menu.rc
)

add_compile_definitions(
        UNICODE # Macros use Unicode (W) versions of Windows API functions
        WIN32_LEAN_AND_MEAN # Strips windows.h from rarely used parts
)

configure_file(
        res/bc2version.in.h
        ${CMAKE_BINARY_DIR}/bc2version.h
        @ONLY
)

target_include_directories(
        bmiCalculator2 PRIVATE
        ${CMAKE_BINARY_DIR}
)

if (MSVC) # MSVC, Clang-cl

    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /W4 /MP")

    # This /arch flag is used only for x86 builds
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /WX /O2 /GL /arch:IA32")

    if (MSVC_VERSION GREATER_EQUAL 1920) # Visual Studio 2019 or newer
        set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /Ob3")
    endif ()

    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /LTCG /SUBSYSTEM:WINDOWS /OSVERSION:6.1")

else () # MinGW

    target_sources(
            bmiCalculator2 PRIVATE
            res/mingw_manifest.rc
    )

    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wno-unused-value -municode -mwindows")

    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Werror -O3 -s -static -Wl,--major-subsystem-version,6,--minor-subsystem-version,1")

endif ()
