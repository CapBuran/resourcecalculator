function(download_three_party_tar_gz2 url filedst)
  if(NOT EXISTS ${CMAKE_SOURCE_DIR}/ThreeParty/External)
    file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/ThreeParty/External)
  endif(NOT EXISTS ${CMAKE_SOURCE_DIR}/ThreeParty/External)
  if(NOT EXISTS ${CMAKE_SOURCE_DIR}/ThreeParty/External/${filedst})
    file(DOWNLOAD
      ${url}
      ${CMAKE_SOURCE_DIR}/ThreeParty/External/${filedst}
      SHOW_PROGRESS
    )
    execute_process(
      COMMAND ${CMAKE_COMMAND} -E tar xvzf ${filedst}
      WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/ThreeParty/External
    )
  endif(NOT EXISTS ${CMAKE_SOURCE_DIR}/ThreeParty/External/${filedst})
endfunction()

function(download_three_party_tar_gz url filedst)
  if(NOT EXISTS ${CMAKE_BINARY_DIR}/ThreeParty/External)
    file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/ThreeParty/External)
  endif(NOT EXISTS ${CMAKE_BINARY_DIR}/ThreeParty/External)
  if(NOT EXISTS ${CMAKE_BINARY_DIR}/ThreeParty/External/${filedst})
    file(DOWNLOAD
      ${url}
      ${CMAKE_BINARY_DIR}/ThreeParty/External/${filedst}
      SHOW_PROGRESS
    )
    execute_process(
      COMMAND ${CMAKE_COMMAND} -E tar xvzf ${filedst}
      WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/ThreeParty/External
    )
  endif(NOT EXISTS ${CMAKE_BINARY_DIR}/ThreeParty/External/${filedst})
endfunction()
