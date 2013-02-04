set(KDE_PREFIX    $ENV{EMSCRIPTEN_KDE_INSTALL})
set(KDE4_SRC_DIR  $ENV{EMSCRIPTEN_KDE_SRC})
 
# the name of the target operating system
SET(CMAKE_SYSTEM_NAME Emscripten)
 
# which compilers to use for C and C++
SET(CMAKE_C_COMPILER clang)
SET(CMAKE_CXX_COMPILER clang++)
 
# here is the target environment located
SET(CMAKE_FIND_ROOT_PATH  ${ENV}{"EMSCRIPTEN_ROOT"} ${KDE_PREFIX} )
 
# adjust the default behaviour of the FIND_XXX() commands:
# search headers and libraries in the target environment, search
# programs in the host environment
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
 
# FindQt4.cmake querys qmake to get information, this doesn't work when crosscompiling
set(QT_BINARY_DIR   ${KDE_PREFIX}/bin)
set(QT_LIBRARY_DIR  ${KDE_PREFIX}/lib)
set(QT_QTCORE_LIBRARY   ${KDE_PREFIX}/lib/QtCore.a)
set(QT_QTCORE_INCLUDE_DIR ${KDE_PREFIX}/include/QtCore)
set(QT_MKSPECS_DIR  ${KDE_PREFIX}/mkspecs)
set(QT_MOC_EXECUTABLE  ${QT_BINARY_DIR}/moc)
set(QT_QMAKE_EXECUTABLE  ${QT_BINARY_DIR}/qmake)
set(QT_UIC_EXECUTABLE  ${QT_BINARY_DIR}/uic)
