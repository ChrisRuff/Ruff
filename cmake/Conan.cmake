if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
    message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
    file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/0.18.1/conan.cmake"
            "${CMAKE_BINARY_DIR}/conan.cmake"
            TLS_VERIFY ON)
endif()

include(${CMAKE_BINARY_DIR}/conan.cmake)
if(${BUILD_TESTERS})
    set(CONAN_TESTS True)
else()
    set(CONAN_TESTS False)
endif()
if(${BUILD_DOXY_DOC})
    set(CONAN_DOCS True)
else()
    set(CONAN_DOCS False)
endif()
if(${BUILD_UI})
    set(CONAN_UI True)
else()
    set(CONAN_UI False)
endif()
if(${BUILD_GEOMETRY})
    set(CONAN_GEOMETRY True)
else()
    set(CONAN_GEOMETRY False)
endif()
if(${BUILD_AI})
    set(CONAN_AI True)
else()
    set(CONAN_AI False)
endif()
if(${BUILD_SECURITY})
    set(CONAN_SECURITY True)
else()
    set(CONAN_SECURITY False)
endif()
if(${BUILD_NLP})
    set(CONAN_NLP True)
else()
    set(CONAN_NLP False)
endif()
if(${BUILD_PHOTOGRAMMETRY})
    set(CONAN_PHOTOGRAMMETRY True)
else()
    set(CONAN_PHOTOGRAMMETRY False)
endif()

conan_cmake_install(PATH_OR_REFERENCE ${PROJECT_SOURCE_DIR}
        OPTIONS
        build_tests=${CONAN_TESTS}
        build_docs=${CONAN_DOCS}
        build_geometry=${CONAN_GEOMETRY}
        build_ui=${CONAN_UI}
        build_ai=${CONAN_AI}
        build_nlp=${CONAN_NLP}
        build_security=${CONAN_SECURITY}
        build_photogrammetry=${CONAN_PHOTOGRAMMETRY}
        SETTINGS
        compiler.libcxx=libstdc++11
        BUILD missing)


include(${CMAKE_BINARY_DIR}/conan_paths.cmake)
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)
