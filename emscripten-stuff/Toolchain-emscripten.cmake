set(KDE_PREFIX    $ENV{EMSCRIPTEN_KDE_INSTALL})
set(KDE4_SRC_DIR  $ENV{EMSCRIPTEN_KDE_SRC})
set(EMSCRIPTEN_SYSTEM $ENV{EMSCRIPTEN_ROOT_PATH}/system/)
 
# the name of the target operating system
SET(CMAKE_SYSTEM_NAME Emscripten)
SET(CMAKE_SYSTEM_PROCESSOR Emscripten)
SET (CMAKE_CROSSCOMPILING   TRUE)
 
# which compilers to use for C and C++
#SET(CMAKE_C_COMPILER ccache clang)
#SET(CMAKE_CXX_COMPILER ccache clang++)
SET(CMAKE_C_COMPILER $ENV{EMSCRIPTEN_ROOT_PATH}/emcc)
SET(CMAKE_CXX_COMPILER $ENV{EMSCRIPTEN_ROOT_PATH}/em++)
SET(CMAKE_C_LINK_EXECUTABLE "llvm-link -o <TARGET> <LINK_FLAGS> <OBJECTS> <LINK_LIBRARIES>")
SET(CMAKE_CXX_LINK_EXECUTABLE ${CMAKE_C_LINK_EXECUTABLE})
SET(CMAKE_CXX_CREATE_SHARED_LIBRARY "false")
#SET(CMAKE_CXX_CREATE_STATIC_LIBRARY  "llvm-ar q <TARGET> <OBJECTS> ")
SET(CMAKE_CXX_CREATE_STATIC_LIBRARY  "llvm-link -o <TARGET> <OBJECTS> ")
#set(CMAKE_CXX_FLAGS "-strip-debug -emit-llvm -nostdinc++ -nostdinc -Qunused-arguments -fcolor-diagnostics -m32 -O0 -isystem${EMSCRIPTEN_SYSTEM}/include -isystem${EMSCRIPTEN_SYSTEM}/include/bsd -isystem${EMSCRIPTEN_SYSTEM}/include/libcxx -isystem${EMSCRIPTEN_SYSTEM}/include/libc -isystem${EMSCRIPTEN_SYSTEM}/include/sys -isystem${EMSCRIPTEN_SYSTEM}/include/net -isystem${EMSCRIPTEN_SYSTEM}/lib/libcxxabi/include/" CACHE STRING "" FORCE)
#set(CMAKE_C_FLAGS "-strip-debug -emit-llvm -nostdinc -Qunused-arguments -fcolor-diagnostics -m32 -isystem${EMSCRIPTEN_SYSTEM}/include -isystem${EMSCRIPTEN_SYSTEM}/include/bsd -isystem${EMSCRIPTEN_SYSTEM}/include/libcxx -isystem${EMSCRIPTEN_SYSTEM}/include/libc -isystem${EMSCRIPTEN_SYSTEM}/include/sys " CACHE STRING "" FORCE)
#add_definitions("-DEMSCRIPTEN") 

# here is the target environment located
SET(CMAKE_FIND_ROOT_PATH  ${ENV}{EMSCRIPTEN_ROOT_PATH} ${KDE_PREFIX} )
 
# adjust the default behaviour of the FIND_XXX() commands:
# search headers and libraries in the target environment, search
# programs in the host environment
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
 
# FindQt4.cmake querys qmake to get information, this doesn't work when crosscompiling
set(QT_BINARY_DIR   /usr/bin)
set(QT_LIBRARY_DIR  ${KDE_PREFIX}/lib)
set(QT_HEADERS_DIR  ${KDE_PREFIX}/include)
set(QT_QTCORE_LIBRARY   ${KDE_PREFIX}/lib/QtCore.a)
set(QT_QTCORE_INCLUDE_DIR ${KDE_PREFIX}/include/QtCore)
set(QT_MKSPECS_DIR  ${KDE_PREFIX}/mkspecs)
set(QT_MOC_EXECUTABLE  ${QT_BINARY_DIR}/moc)
set(QT_QMAKE_EXECUTABLE  ${QT_BINARY_DIR}/qmake)
set(QT_UIC_EXECUTABLE  ${QT_BINARY_DIR}/uic)
set(QT_DBUSXML2CPP_EXECUTABLE ${QT_BINARY_DIR}/qdbusxml2cpp)
set(ENV{QMAKESPEC} $ENV{EMSCRIPTEN_KDE_INSTALL}/mkspecs/qws/emscripten-clang)
