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
    if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/input.txt")
        configure_file(${CMAKE_CURRENT_SOURCE_DIR}/input.txt ${CMAKE_CURRENT_BINARY_DIR}/input.txt)
    endif ()

    if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/input_example.txt")
        configure_file(${CMAKE_CURRENT_SOURCE_DIR}/input_example.txt ${CMAKE_CURRENT_BINARY_DIR}/input_example.txt)
    endif ()
endmacro()