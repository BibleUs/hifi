include(ExternalProject)
include(SelectLibraryConfigurations)

set(EXTERNAL_NAME boost)

string(TOUPPER ${EXTERNAL_NAME} EXTERNAL_NAME_UPPER)

if (WIN32)
  ExternalProject_Add(
    ${EXTERNAL_NAME}
    URL http://public.highfidelity.com/dependencies/boost_win_1_57_0.zip
    URL_MD5 7efc87038bd93a4acc31e37cf0bd01fa
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    LOG_DOWNLOAD 1
  )

  ExternalProject_Get_Property(${EXTERNAL_NAME} SOURCE_DIR)
  set(BOOST_LIB_DIR ${SOURCE_DIR}/lib64-msvc-12.0)

  set(${EXTERNAL_NAME_UPPER}_INCLUDE_DIRS ${SOURCE_DIR} CACHE TYPE INTERNAL)
  set(${EXTERNAL_NAME_UPPER}_LIBRARIES ${BOOST_LIB_DIR}/*.lib CACHE TYPE INTERNAL)

  # Hide this external target (for IDE users)
  set_target_properties(${EXTERNAL_NAME} PROPERTIES FOLDER "hidden/externals")

endif()