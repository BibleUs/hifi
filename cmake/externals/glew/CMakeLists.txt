set(EXTERNAL_NAME glew)

if (ANDROID)
  set(ANDROID_CMAKE_ARGS "-DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}" "-DANDROID_NATIVE_API_LEVEL=19")
endif ()

include(ExternalProject)
ExternalProject_Add(
  ${EXTERNAL_NAME}
  URL https://github.com/nigels-com/glew/releases/download/glew-2.1.0/glew-2.1.0.tgz
  URL_MD5 b2ab12331033ddfaa50dc39345343980
  SOURCE_SUBDIR build/cmake
  CONFIGURE_COMMAND CMAKE_ARGS  ${ANDROID_CMAKE_ARGS} -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} -DCMAKE_CXX_FLAGS=-fpic -DCMAKE_C_FLAGS=-fpic -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
  LOG_DOWNLOAD 1
  LOG_CONFIGURE 1
  LOG_BUILD 1
)

# Hide this external target (for ide users)
set_target_properties(${EXTERNAL_NAME} PROPERTIES FOLDER "hidden/externals")

ExternalProject_Get_Property(${EXTERNAL_NAME} INSTALL_DIR)

string(TOUPPER ${EXTERNAL_NAME} EXTERNAL_NAME_UPPER)
set(${EXTERNAL_NAME_UPPER}_INCLUDE_DIRS ${INSTALL_DIR}/include CACHE PATH "List of glew include directories")

if (UNIX)
  set(LIB_PREFIX "lib")
  set(LIB_EXT "a")
elseif (WIN32)
  set(LIB_EXT "lib")
endif ()

set(${EXTERNAL_NAME_UPPER}_LIBRARY_DEBUG ${INSTALL_DIR}/lib/${LIB_PREFIX}GLEW_d.${LIB_EXT} CACHE FILEPATH "Path to glew debug library")
set(${EXTERNAL_NAME_UPPER}_LIBRARY_RELEASE ${INSTALL_DIR}/lib/${LIB_PREFIX}GLEW.${LIB_EXT} CACHE FILEPATH "Path to glew release library")
