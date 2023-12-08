macro(add_include)
    include_directories(${CMAKE_SOURCE_DIR}/include)

    file(GLOB children RELATIVE ${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/include/*)
    foreach(child ${children})
        if(NOT IS_DIRECTORY ${CMAKE_SOURCE_DIR}/include/${child})
            list(APPEND SOURCE_FILES ${CMAKE_SOURCE_DIR}/include/${child})
        endif()
    endforeach()
endmacro()


macro(add_inputs)
    file(GLOB FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/ ${CMAKE_CURRENT_SOURCE_DIR}/input*.txt)

    foreach(FILE ${FILES})
        get_filename_component(FILE_NAME ${FILE} NAME)
        configure_file(${CMAKE_CURRENT_SOURCE_DIR}/${FILE_NAME} ${CMAKE_CURRENT_BINARY_DIR}/${FILE_NAME})
    endforeach()

endmacro()