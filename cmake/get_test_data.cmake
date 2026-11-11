function(get_test_data TARGET)

  set(ZPP_URL
      "https://www.zimo.at/web2010/scripts/download_zpp.php?b=soundprojekte%2Fzimo%2FDA%5FDI%5FR%5FEU%5FColl1%2F&f=DA_DI_R_EU_Coll1.zpp"
  )
  set(MS_ZSU_URL "https://www.zimo.at/update/FlashFiles/MS-5.29.0.zip")
  set(MX_ZSU_URL "https://www.zimo.at/update/FlashFiles/DS240307.zip")

  set(EXTERNAL_FILES_DIR ${CMAKE_BINARY_DIR}/extern/files)

  include(FetchContent)

  FetchContent_Declare(
    zpp_file
    URL ${ZPP_URL} DOWNLOAD_DIR ${EXTERNAL_FILES_DIR}/zpp
    DOWNLOAD_NAME test.zpp
    DOWNLOAD_NO_EXTRACT TRUE)

  FetchContent_Declare(ms_zsu_file URL ${MS_ZSU_URL} SOURCE_DIR
                                       ${EXTERNAL_FILES_DIR}/zsu/ms)
  FetchContent_Declare(mx_zsu_file URL ${MX_ZSU_URL} SOURCE_DIR
                                       ${EXTERNAL_FILES_DIR}/zsu/mx)
  FetchContent_MakeAvailable(zpp_file ms_zsu_file mx_zsu_file)

  file(GLOB ZPP_PATH "${zpp_file_SOURCE_DIR}/*.zpp")
  file(GLOB MS_ZSU_PATH "${ms_zsu_file_SOURCE_DIR}/*.zsu")
  file(GLOB MX_ZSU_PATH "${mx_zsu_file_SOURCE_DIR}/*.zsu")

  configure_file(${PROJECT_SOURCE_DIR}/template/paths.hpp.in
                 "${CMAKE_BINARY_DIR}/generated/paths.hpp")
  target_include_directories(${TARGET} PRIVATE ${CMAKE_BINARY_DIR}/generated/)

endfunction()
