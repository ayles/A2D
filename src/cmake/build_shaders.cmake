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

file(MAKE_DIRECTORY ${SHADERS_GENERATION_PATH})

subdirlist(SHADERS_DIRS ${SHADERS_ROOT_DIR})

foreach(SHADER_DIR ${SHADERS_DIRS})
    message(STATUS "Shader \"${SHADER_DIR}\":")
    execute_process(
            COMMAND ${GLSLCC_EXECUTABLE}
            --vert=${SHADERS_ROOT_DIR}/${SHADER_DIR}/vertex.glsl
            --frag=${SHADERS_ROOT_DIR}/${SHADER_DIR}/fragment.glsl
            --output=${SHADERS_GENERATION_PATH}/${SHADER_DIR}.glsl
            --lang=${SHADERS_TARGET_LANG}
            --profile=${SHADERS_TARGET_PROFILE})

    file(READ "${SHADERS_GENERATION_PATH}/${SHADER_DIR}_vs.glsl" VERTEX_SHADER)
    file(READ "${SHADERS_GENERATION_PATH}/${SHADER_DIR}_fs.glsl" FRAGMENT_SHADER)
    set(SHADERS_INIT_CODE
            "${SHADERS_INIT_CODE}SHADER_LOAD_MACRO(${SHADER_DIR},\n\nR\"(${VERTEX_SHADER})\",\n\nR\"(${FRAGMENT_SHADER})\"\n\n)\n\n")
endforeach()

message(STATUS "Shaders compilation done")

configure_file(${SHADERS_TEMPLATE_FILE} ${SHADERS_GENERATED_FILE})