FILE(GLOB_RECURSE UtilFiles "generate_paths.py")
add_custom_target(headers SOURCES ${UtilFiles})
execute_process(COMMAND "./generate_paths.py" "")

