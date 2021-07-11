cmake_minimum_required(VERSION 3.16)
project(ejdb)

set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
SET(CMAKE_C_FLAGS  "-O2 -pipe -fPIC -Wall -fmessage-length=0")

include_directories(D:/open-open/openresty-1.19.3.1-win64/include/luajit-2.1)
include_directories(E:/ejdb2-2.0.59-Release-Windows-x86_64/usr/local/include/)
include_directories(E:/ejdb2-2.0.59-Release-Windows-x86_64/usr/local/bin)
include_directories(E:/c_workspace/ejdb-lua-v2/src)



LINK_DIRECTORIES(E:/ejdb2-2.0.59-Release-Windows-x86_64/usr/local/lib)
LINK_DIRECTORIES(D:/open-open/openresty-1.19.3.1-win64)


add_library(ejdb SHARED src/ejdb.c)

set_target_properties(ejdb PROPERTIES COMPILE_FLAGS "-DIW_STATIC")
target_link_libraries(ejdb
        lua51.dll
        libiowow-1.a
        libejdb2.dll
        )

#add_executable(example  src/example1.c)
#set_target_properties(example PROPERTIES COMPILE_FLAGS "-DIW_STATIC")
#target_link_libraries(example
#        lua51.dll
#        libiowow-1.a
#        libejdb2.dll
#        )


