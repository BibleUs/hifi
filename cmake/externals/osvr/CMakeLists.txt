include(ExternalProject)
include(SelectLibraryConfigurations)

set(EXTERNAL_NAME osvr)

string(TOUPPER ${EXTERNAL_NAME} EXTERNAL_NAME_UPPER)

if (WIN32)
  ExternalProject_Add(
    ${EXTERNAL_NAME}
    URL http://ctrlaltstudio.com/downloads/hifi/osvr_sdk_win_0_6_1311_192.zip
    URL_MD5 afdfeaac1d4bad5155ffadd55577ba02
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    LOG_DOWNLOAD 1
  )

  ExternalProject_Get_Property(${EXTERNAL_NAME} SOURCE_DIR)
  set(OSVR_LIB_DIR ${SOURCE_DIR}/lib)

  set(${EXTERNAL_NAME_UPPER}_INCLUDE_DIRS ${SOURCE_DIR}/include CACHE TYPE INTERNAL)
  set(${EXTERNAL_NAME_UPPER}_LIBRARIES ${OSVR_LIB_DIR}/*.lib CACHE TYPE INTERNAL)
  set(${EXTERNAL_NAME_UPPER}_DLL_PATH ${SOURCE_DIR}/bin CACHE TYPE INTERNAL)

  add_paths_to_fixup_libs(${${EXTERNAL_NAME_UPPER}_DLL_PATH})

  # Hide this external target (for IDE users)
  set_target_properties(${EXTERNAL_NAME} PROPERTIES FOLDER "hidden/externals")

endif()