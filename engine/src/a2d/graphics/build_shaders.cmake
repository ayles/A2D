macro(SUBDIRLIST result curdir)
    file(GLOB children RELATIVE ${curdir} ${curdir}/*)
    set(dirlist "")
    foreach(child ${children})
        if(IS_DIRECTORY ${curdir}/${child})
            list(APPEND dirlist ${child})
        endif()
    endforeach()
    set(${result} ${dirlist})
endmacro()

message(STATUS "Generating shaders init file")
subdirlist(SHADERS_DIRS ${SHADERS_PATH})

file(MAKE_DIRECTORY ${SHADERS_BUILD_PATH})

foreach(SHADER_DIR ${SHADERS_DIRS})
    execute_process(
            COMMAND ${GLSLCC_PATH}
            --vert=${SHADERS_PATH}/${SHADER_DIR}/vertex.glsl
            --frag=${SHADERS_PATH}/${SHADER_DIR}/fragment.glsl
            --output=${SHADERS_BUILD_PATH}/${SHADER_DIR}.glsl
            --lang=${GLSL_LANG}
            OUTPUT_VARIABLE out ERROR_VARIABLE err)

    message(STATUS "Shader \"${SHADER_DIR}\":\n${out} ${err}")

    file(READ "${SHADERS_BUILD_PATH}/${SHADER_DIR}_vs.glsl" VERTEX_SHADER)
    file(READ "${SHADERS_BUILD_PATH}/${SHADER_DIR}_fs.glsl" FRAGMENT_SHADER)
    set(DEFAULT_SHADERS_INIT_CODE
            "${DEFAULT_SHADERS_INIT_CODE}shaders[\"${SHADER_DIR}\"] = new Shader(\n\nR\"(${VERTEX_SHADER})\",\n\nR\"(${FRAGMENT_SHADER})\"\n\n);\n\n")
endforeach()

configure_file(${SHADERS_CREATE_IN_FILE_PATH} ${SHADERS_CREATE_CPP_FILE_PATH})