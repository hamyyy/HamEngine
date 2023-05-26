

function(target_copy_assets target dir_name)

    # add_custom_command(dummyTarget ALL
    #     COMMAND ${CMAKE_COMMAND} -E copy_directory
    #     ${CMAKE_CURRENT_LIST_DIR}/${dir_name} $<TARGET_FILE_DIR:${target}>/${dir_name}
    #     DEPENDS always_rebuild
    # )

    add_custom_target("${target}_copy_assets" ALL DEPENDS copy_assets)
    add_custom_command(OUTPUT copy_assets
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_LIST_DIR}/${dir_name} $<TARGET_FILE_DIR:${target}>/${dir_name}
        DEPENDS always_rebuild
    )

    add_custom_command(OUTPUT always_rebuild COMMAND cmake -E echo)
    
endfunction()
