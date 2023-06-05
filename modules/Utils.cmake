

function(target_copy_assets target dir_name)
    add_custom_target("${target}_copy_assets" ALL DEPENDS copy_assets)
    add_custom_command(OUTPUT copy_assets
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_LIST_DIR}/${dir_name}
            $<TARGET_FILE_DIR:${target}>/${dir_name}
        DEPENDS always_rebuild
    )
    add_custom_command(OUTPUT always_rebuild COMMAND ${CMAKE_COMMAND} -E echo "Copying assets to $<TARGET_FILE_DIR:${target}>/${dir_name}")
endfunction()


function(target_kill_executable target_name)
	if (WIN32)
		set(KILL_EXE_COMMAND START taskkill -f -im "${target_name}*")
	else()
		# set(KILL_EXE_COMMAND "/bin/bash -c '/usr/bin/pkill ${target_name}*; exit 0'")
	endif()
	add_custom_target("${target_name}_target_kill_executable" COMMAND ${KILL_EXE_COMMAND})
	add_dependencies(${target_name} "${target_name}_target_kill_executable")	
endfunction(target_kill_executable)


# not used

function(compile_shaders target_name)

	find_package(Vulkan REQUIRED COMPONENTS glslc)
	find_program(glslc_executable NAMES glslc HINTS Vulkan::glslc)

	set(SHADER_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/shaders)
	set(SHADER_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CONFIGURATION_TYPE}/shaders)

	file(GLOB SHADERS
	${SHADER_SOURCE_DIR}/*.vert
	${SHADER_SOURCE_DIR}/*.frag
	${SHADER_SOURCE_DIR}/*.comp
	${SHADER_SOURCE_DIR}/*.geom
	${SHADER_SOURCE_DIR}/*.tesc
	${SHADER_SOURCE_DIR}/*.tese
	${SHADER_SOURCE_DIR}/*.mesh
	${SHADER_SOURCE_DIR}/*.task
	${SHADER_SOURCE_DIR}/*.rgen
	${SHADER_SOURCE_DIR}/*.rchit
	${SHADER_SOURCE_DIR}/*.rmiss
	)
	
	set_source_files_properties(${SHADER_BINARY_DIR} PROPERTIES SYMBOLIC 1)
	add_custom_command(
	COMMAND
		${CMAKE_COMMAND} -E make_directory ${SHADER_BINARY_DIR}
	OUTPUT ${SHADER_BINARY_DIR}
	COMMENT "Creating ${SHADER_BINARY_DIR}"
	)

	foreach(source IN LISTS SHADERS)
	get_filename_component(FILENAME ${source} NAME)

	set_source_files_properties(${SHADER_BINARY_DIR}/${FILENAME}.spv PROPERTIES SYMBOLIC 1)
	add_custom_command(
		COMMAND
		${glslc_executable}
		-MD -MF ${SHADER_BINARY_DIR}/${FILENAME}.d
		-o ${SHADER_BINARY_DIR}/${FILENAME}.spv
		${source}
		OUTPUT ${SHADER_BINARY_DIR}/${FILENAME}.spv
		DEPENDS ${source} ${SHADER_BINARY_DIR}
		COMMENT "Compiling ${FILENAME}"
	)
	
	list(APPEND SPV_SHADERS ${SHADER_BINARY_DIR}/${FILENAME}.spv)
	endforeach()

	add_custom_target("${target_name}_shaders" DEPENDS ${SPV_SHADERS})
	add_dependencies(${target_name} "${target_name}_shaders")
	
endfunction(compile_shaders)