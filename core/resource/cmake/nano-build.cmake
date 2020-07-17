################################################################################
#
# (c) 2020 Copyright, Real-Time Innovations, Inc. (RTI)
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
################################################################################

################################################################################
# Update NANO_BUILD_SYS_DIR to the directory of this file
################################################################################

get_filename_component(
    NANO_BUILD_SYS_DIR  "${CMAKE_CURRENT_LIST_FILE}" PATH CACHE FORCE)

################################################################################
# Helper functions & macros
################################################################################

macro(set_if_undefined var)
    if(NOT DEFINED ${var})
        set(${var} ${ARGN})
    endif()
endmacro()

function(list_prefix var prefix)
   set(result_list "")
   foreach(f ${ARGN})
      list(APPEND result_list "${prefix}${f}")
   endforeach(f)
   set(${var} "${result_list}" PARENT_SCOPE)
endfunction()

function(list_suffix var suffix)
   set(result_list "")
   foreach(f ${ARGN})
      list(APPEND result_list "${f}${suffix")
   endforeach(f)
   set(${var} "${result_list}" PARENT_SCOPE)
endfunction()

function(strip_path_prefix list_var_in list_var_out)
    set(list_out "")
    foreach(path_el IN LISTS ${list_var_in})
        get_filename_component(stripped_el "${path_el}" NAME)
        list(APPEND list_out ${stripped_el})
    endforeach()
    set(${list_var_out} ${list_out} PARENT_SCOPE)
endfunction()

function(strip_target_prefix list_var_in list_var_out)
    set(list_out "")
    foreach(path_el IN LISTS ${list_var_in})
        string(REPLACE "::" ";" path_el_list "${path_el}")
        list(LENGTH path_el_list path_el_list_len)
        math(EXPR path_el_i "${path_el_list_len} - 1")
        list(GET path_el_list ${path_el_i} path_el_last)
        list(APPEND list_out ${path_el_last})
    endforeach()
    set(${list_var_out} ${list_out} PARENT_SCOPE)
endfunction()

function(make_fmt_list list_var_in list_var_out)
    list(LENGTH ${list_var_in} list_var_in_len)
    if (list_var_in_len GREATER 0)
        string(REPLACE ";" " \\\n    " list_out "${${list_var_in}}" )
    else()
        set(list_out ${${list_var_in}})
    endif()
    set(${list_var_out} ${list_out} PARENT_SCOPE)

endfunction()

macro(define_option var_prefix opt opt_name def_val desc)
    set_if_undefined(${opt} ${def_val})
    option(${var_prefix}_${opt} "${desc}" ${${opt}})
    add_feature_info(
        "${var_prefix}: ${opt}" ${var_prefix}_${opt} "${desc}")
    set(${var_prefix}_${opt}        ${${var_prefix}_${opt}}
        CACHE BOOL "${desc}" FORCE)
endmacro()

################################################################################
################################################################################
# nano_project functions
################################################################################
################################################################################

################################################################################
# nano_project_option(
#     _opt :
#     -opt_name :
#     _opt_default :
#     _opt_desc :
# )
################################################################################
macro(nano_project_option    _opt _opt_name _opt_default _opt_desc)
    define_option(${PROJECT_NAME}
        ${_opt}  ${_opt_name} ${_opt_default} ${_opt_desc})
endmacro()


################################################################################
# nano_project()
################################################################################
function(nano_project)
    
    message(STATUS "Configuring build for ${PROJECT_NAME}")

    # This will allow us to print a feature summary.
    # https://cmake.org/cmake/help/v3.11/module/FeatureSummary.html
    include(FeatureSummary)

    # This include provides some helper functions that simplify packaging.
    # https://cmake.org/cmake/help/v3.11/module/CMakePackageConfigHelpers.html
    include(CMakePackageConfigHelpers)

    # If no build type was specified, set it to Release.
    if(NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE        Release
            CACHE STRING
            "Build configuration to use"
            FORCE)
    endif()

    # If no installation prefix is given manually, install locally.
    if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
        set(CMAKE_INSTALL_PREFIX    "install"
            CACHE PATH
            "Directory where files will be copied by the install target"
            FORCE)
    endif()

    if(NOT DEFINED BUILD_SHARED_LIBS)
        set(BUILD_SHARED_LIBS       false
            CACHE BOOL
            "Wheter static or dynamic libraries and executables should be built"
            FORCE)
    endif()

    set(${PROJECT_NAME}_INSTALL_PREFIX        ""
        CACHE STRING "Prefix which will be prepended to installation paths ")

    nano_project_option(
        INSTALL_SOURCE
        "Install source"
        OFF
        "Copy source files for ${PROJECT_NAME} to install directory")

    nano_project_option(
        BUILD_LIBRARIES
        "Build libraries"
        ON
        "Build libraries for ${PROJECT_NAME}")

    nano_project_option(
        BUILD_EXECUTABLES
        "Build executables"
        ${${PROJECT_NAME}_BUILD_LIBRARIES}
        "Build executables for ${PROJECT_NAME}")
    
    nano_project_option(
        BUILD_DOCS
        "Build documentation"
        OFF
        "Build API and user manual documentation for ${PROJECT_NAME} (requires Sphinx and Doxygen)")

    nano_project_option(
        BUILD_DOCS_API
        "Build API documentation"
        ${${PROJECT_NAME}_BUILD_DOCS}
        "Build API documentation for ${PROJECT_NAME} (requires Doxygen)")
    
    nano_project_option(
        BUILD_DOCS_MANUAL
        "Build user manual"
        ${${PROJECT_NAME}_BUILD_DOCS}
        "Build user manual for ${PROJECT_NAME} (requires Sphinx)")

    if (NOT DEFINED INCLUDE_PREFIX)
        set(INCLUDE_PREFIX      "nano")
    endif()
    set(${PROJECT_NAME}_INCLUDE_PREFIX     ${INCLUDE_PREFIX}
        CACHE INTERNAL "Sub-directory of include/ where all header files are stored")

    if (NOT DEFINED TARGET_NAMESPACE)
        set(TARGET_NAMESPACE    "${${PROJECT_NAME}_INCLUDE_PREFIX}::")
    endif()
    set(${PROJECT_NAME}_TARGET_NAMESPACE     ${TARGET_NAMESPACE}
        CACHE INTERNAL "Prefix for all targets generated by ${PROJECT_NAME}")

    if (NOT DEFINED LIBRARY_PREFIX)
        set(LIBRARY_PREFIX    "${${PROJECT_NAME}_INCLUDE_PREFIX}")
    endif()
    set(${PROJECT_NAME}_LIBRARY_PREFIX     ${LIBRARY_PREFIX}
        CACHE INTERNAL "Prefix for all library targets generated by ${PROJECT_NAME}")

    set(${PROJECT_NAME}_DEPS            ${DEPS}
        CACHE INTERNAL "Dependencies for ${PROJECT_NAME}")
    
    set(${PROJECT_NAME}_EXAMPLES        ${EXAMPLES}
        CACHE INTERNAL "Examples for ${PROJECT_NAME}")

    set(${PROJECT_NAME}_PREFIX_PATH     ${PROJECT_BINARY_DIR}
                                        ${PREFIX_PATH}
        CACHE INTERNAL "Directories that will be added to CMAKE_PREFIX_PATH after the project has been configured")

    set(${PROJECT_NAME}_AUTHOR          ${AUTHOR}
        CACHE STRING "Author of ${PROJECT_NAME}")
    
    set(${PROJECT_NAME}_COPYRIGHT       ${COPYRIGHT}
        CACHE STRING "Copyright header for ${PROJECT_NAME}")

    set(${PROJECT_NAME}_GIT_REPO        ${GIT_REPO}
        CACHE STRING "URL of git repository for ${PROJECT_NAME}")

    set(${PROJECT_NAME}_LICENSE        ${LICENSE}
        CACHE STRING "License for ${PROJECT_NAME}")
    
    set(${PROJECT_NAME}_LICENSE_FILE   ${CMAKE_CURRENT_LIST_DIR}/LICENSE
        CACHE STRING "License file for ${PROJECT_NAME}")

    set(${PROJECT_NAME}_THIRD_PARTY_FILE ${CMAKE_CURRENT_LIST_DIR}/THIRD-PARTY-NOTICES.md
        CACHE STRING "License file for ${PROJECT_NAME}")

endfunction()

################################################################################
# nano_project_vars()
################################################################################
macro(nano_project_vars)
    set(prj_name                ${PROJECT_NAME})
    set(prj_version             ${PROJECT_VERSION})
    set(prj_description         ${PROJECT_DESCRIPTION})
    set(prj_author              ${${PROJECT_NAME}_AUTHOR})
    set(prj_license             ${${PROJECT_NAME}_LICENSE})
    set(prj_license_file        ${${PROJECT_NAME}_LICENSE_FILE})
    set(prj_third_party_file    ${${PROJECT_NAME}_THIRD_PARTY_FILE})
    set(prj_git_repo            ${${PROJECT_NAME}_GIT_REPO})
    set(prj_copyright           ${${PROJECT_NAME}_COPYRIGHT})
    set(prj_description         ${${PROJECT_NAME}_DESCRIPTION})
    set(prj_components          ${${PROJECT_NAME}_COMPONENTS})
    set(prj_deps                ${${PROJECT_NAME}_DEPS})
    set(prj_tgt_ns              ${${PROJECT_NAME}_TARGET_NAMESPACE})
    set(prj_lib_prefix          ${${PROJECT_NAME}_LIBRARY_PREFIX})
    set(prj_include_prefix      ${${PROJECT_NAME}_INCLUDE_PREFIX})
    set(prj_install_prefix      ${${PROJECT_NAME}_INSTALL_PREFIX})
    set(prj_install_source      ${${PROJECT_NAME}_INSTALL_SOURCE})
    set(prj_build_libraries     ${${PROJECT_NAME}_BUILD_LIBRARIES})
    set(prj_build_executables   ${${PROJECT_NAME}_BUILD_EXECUTABLES})
    set(prj_examples            ${${PROJECT_NAME}_EXAMPLES})
    set(prj_prefix_path         ${${PROJECT_NAME}_PREFIX_PATH})
    
    set(prj_config_version_file "${prj_name}-config-version.cmake")
    set(prj_config_file         "${prj_name}-config.cmake")
    string(REPLACE "-" "_"
        prj_prefix              "${prj_name}")

    set(prj_components_name      "")
    foreach(comp_pfx IN LISTS prj_components)
        list(APPEND prj_components_name     "${${comp_pfx}_NAME}")
    endforeach()

    set(prj_components_tgt      "")
    foreach(comp_pfx IN LISTS prj_components)
        list(APPEND prj_components_tgt      "${${comp_pfx}_LIBRARY_TARGET}")
    endforeach()

    set(prj_src_makefile        ${PROJECT_NAME}/makefile)
    set(prj_targets_file        ${PROJECT_NAME}-targets.cmake)

    set(prj_dir                 ${CMAKE_CURRENT_SOURCE_DIR})

    set(prj_include_dir_build   ${prj_dir}/include)
    set(prj_include_dir_install ${prj_install_prefix}include)
    set(prj_install_dir_lib     ${prj_install_prefix}lib)
    set(prj_install_dir_include ${prj_install_prefix}include/${prj_include_prefix})
    set(prj_install_dir_src_base ${prj_install_prefix}src)
    set(prj_install_dir_src     ${prj_install_dir_src_base}/${PROJECT_NAME})
    set(prj_install_dir_example ${prj_install_prefix}examples/${PROJECT_NAME})
    set(prj_install_dir_cmake   ${prj_install_prefix}cmake)
    set(prj_install_dir_bin     ${prj_install_prefix}bin)
    set(prj_install_dir_doc_rel doc)
    set(prj_install_dir_doc_man_rel ${prj_install_dir_doc_rel}/manual/${PROJECT_NAME})
    set(prj_install_dir_doc_api_rel ${prj_install_dir_doc_rel}/api/${PROJECT_NAME})
    set(prj_install_dir_doc     ${prj_install_prefix}${prj_install_dir_doc_rel})
    set(prj_install_dir_doc_man ${prj_install_prefix}${prj_install_dir_doc_man_rel})
    set(prj_install_dir_doc_api ${prj_install_prefix}${prj_install_dir_doc_api_rel})
    
endmacro()


macro(nano_project_doc_vars)
    nano_project_vars()

    set(prj_build_docs_api      ${${PROJECT_NAME}_BUILD_DOCS_API})
    set(prj_build_docs_man      ${${PROJECT_NAME}_BUILD_DOCS_MANUAL})
    set(prj_doc_deps            ${${PROJECT_NAME}_DOC_DEPS})
    set(prj_doc_root_index_rel  ${${PROJECT_NAME}_DOC_ROOT_INDEX})
    set(prj_doc_sources         ${${PROJECT_NAME}_DOC_SOURCES})
    set(prj_doc_input_dirs      ${${PROJECT_NAME}_DOC_INPUT_DIRS})
    set(prj_doc_input_src       ${${PROJECT_NAME}_DOC_INPUT_SRC})
    string(REPLACE ";" " " 
    prj_doc_input_dirs_dox  "${prj_doc_input_dirs}")
    string(REPLACE ";" " \\\n" 
    prj_doc_input_src_dox   "${prj_doc_input_src}")
    
    set(prj_doc_tagfile         ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-docs.tag)
    set(prj_doc_dir             ${prj_dir}/doc)
    set(prj_doc_build_dir       ${PROJECT_BINARY_DIR}/doc)
    set(prj_doc_doxygen_dir     ${prj_doc_build_dir}/doxygen)
    set(prj_doc_doxygen_html_dir ${prj_doc_doxygen_dir}/html)
    set(prj_doc_html_dir        ${prj_doc_build_dir}/html)
    set(prj_doc_doxyfile        ${prj_doc_build_dir}/Doxyfile)
    set(prj_doc_sphinxconf      ${prj_doc_build_dir}/conf.py)
    set(prj_doc_extlinks        ${prj_doc_build_dir}/extlinks.py)
    set(prj_doc_file_patterns   "*.c *.cpp *.h *.hpp")
    set(prj_doc_tgt             ${PROJECT_NAME}-docs)
    set(prj_doc_doxygen_toc     ${prj_doc_dir}/${prj_name}-toc.dox)
    set(prj_doc_tagfile_deps)
    set(prj_doc_tagfile_deps_tgt_index)
    set(prj_doc_tagfile_deps_dox)
    foreach(dep IN LISTS prj_doc_deps)
        set(dep_tagfile "${${dep}_DOC_TAGFILE}")
        set(dep_apidir  "../../../api/${dep}/html")
        list(APPEND prj_doc_tagfile_deps "${dep_tagfile}")
        list(APPEND prj_doc_deps_tgt     "${dep}-docs")
        if (TARGET "${dep}-docs-index")
            list(APPEND prj_doc_tagfile_deps_tgt_index "${dep}-docs-index")
        endif()
        list(APPEND prj_doc_tagfile_deps_dox "${dep_tagfile}=${dep_apidir}")
    endforeach()
    string(REPLACE ";" " \\\n" 
        prj_doc_tagfile_deps_dox  "${prj_doc_tagfile_deps_dox}")
    if (NOT "${prj_doc_root_index_rel}" STREQUAL "")
        set(prj_doc_root_index  ${prj_doc_dir}/${prj_doc_root_index_rel})
    endif()
endmacro()

################################################################################
# nano_project_generate_src_makefile()
################################################################################
function(nano_project_generate_src_makefile)
    
    nano_project_vars()

    make_fmt_list(prj_components_name         prj_components_name)

    configure_file(
        ${NANO_BUILD_SYS_DIR}/nano-prj-src-makefile.in
        ${PROJECT_BINARY_DIR}/${prj_src_makefile}
        @ONLY
        NEWLINE_STYLE UNIX)
    
endfunction()

################################################################################
# nano_project_install()
################################################################################
function(nano_project_install)

    nano_project_vars()

    if (prj_build_libraries)

        configure_package_config_file(
                ${PROJECT_SOURCE_DIR}/${prj_name}-config.cmake.in
                ${PROJECT_BINARY_DIR}/${prj_config_file}
                INSTALL_DESTINATION ${prj_install_dir_cmake})

        write_basic_package_version_file(
                ${PROJECT_BINARY_DIR}/${prj_config_version_file}
                COMPATIBILITY SameMajorVersion)

        install(FILES
            ${PROJECT_BINARY_DIR}/${prj_config_file}
            ${PROJECT_BINARY_DIR}/${prj_config_version_file}
            DESTINATION ${prj_install_dir_cmake})
    endif()

    if (prj_install_source)
        nano_project_generate_src_makefile()

        install(FILES
            ${PROJECT_BINARY_DIR}/${prj_src_makefile}
            DESTINATION ${prj_install_dir_src})
        
        install(FILES
            ${NANO_BUILD_SYS_DIR}/src/makefile
            DESTINATION ${prj_install_dir_src_base})

        if(EXISTS ${prj_license_file})
            install(FILES
                ${prj_license_file}
                DESTINATION ${prj_install_dir_src})
        endif()

        if(EXISTS ${prj_third_party_file})
            install(FILES
                ${prj_third_party_file}
                DESTINATION ${prj_install_dir_src})
        endif()
    endif()

endfunction()

################################################################################
# nano_project_summary()
################################################################################
function(nano_project_summary)
    
    nano_project_vars()

    if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
        feature_summary(WHAT ALL)
    endif()
endfunction()

################################################################################
# nano_project_component(
#     var_prefix
# )
################################################################################
function(nano_project_component     var_prefix)
    nano_project_vars()

    set(project_components          ${${PROJECT_NAME}_COMPONENTS})
    list(APPEND project_components  ${var_prefix})
    set(${PROJECT_NAME}_COMPONENTS  ${project_components}
            CACHE INTERNAL "List of components for ${PROJECT_NAME}")

endfunction()

################################################################################
# nano_project_examples()
################################################################################
function(nano_project_examples)

    nano_project_vars()

    # Add build directories to CMAKE_PREFIX_PATH so that the packages
    # can be found by following calls to find_package().
    # The entries are added to the beginning of the list to force packages
    # to be picked up from the build directory, rather than from other
    # directories that might have been added by the caller.
    list(INSERT CMAKE_PREFIX_PATH 0 ${prj_prefix_path})

    foreach(example IN LISTS prj_examples)
        install(DIRECTORY   examples/${example}
            DESTINATION ${prj_install_dir_example})
        
        if (prj_build_libraries)
            add_subdirectory(examples/${example})
        endif()
    endforeach()
endfunction()

################################################################################
# nano_project_load_connextdds()
################################################################################
function(nano_project_load_connextdds)
    if (NOT DEFINED CONNEXTDDS_DIR)
        file(TO_CMAKE_PATH "$ENV{CONNEXTDDS_DIR}" CONNEXTDDS_DIR)
    endif()
    if (NOT EXISTS ${CONNEXTDDS_DIR})
        message(FATAL_ERROR "Invalid CONNEXTDDS_DIR: '${CONNEXTDDS_DIR}'")
    endif()
    if (NOT DEFINED CONNEXTDDS_ARCH AND
       NOT "$ENV{CONNEXTDDS_ARCH}" STREQUAL "")
        set(CONNEXTDDS_ARCH         "$ENV{CONNEXTDDS_ARCH}")
    endif()
    # On Darwin, FindRTIConnextDDS.cmake has a bug which prevents it from
    # detecting CONNEXTDDS_ARCH. Print a warning if no CONNEXTDDS_ARCH was
    # specified by the user.
    if ("${CONNEXTDDS_ARCH}" STREQUAL "" AND APPLE)
        message(WARNING "No CONNEXTDDS_ARCH specified. "
            "You may have to set this variable to your target architecture if "
            "FindRTIConnextDDS.cmake reports error: "
            "\"string sub-command REGEX, mode REPLACE needs at least 6 "
            "arguments total to command\".")
    endif()
    if (NOT DEFINED CONNEXTDDS_VERSION)
        set(CONNEXTDDS_VERSION              "6.0.0")
    endif()

    list(APPEND CMAKE_MODULE_PATH   ${CONNEXTDDS_DIR}/resource/cmake)

    find_package(RTIConnextDDS  "${CONNEXTDDS_VERSION}"
        REQUIRED COMPONENTS     core)
    
    if (NOT RTIConnextDDS_FOUND)
        message(FATAL_ERROR "Failed to load RTI Connext DDS libraries")
    else()
        message(STATUS "Loaded RTI Connext DDS libraries")
    endif()
endfunction()

################################################################################
################################################################################
# nano_component functions
################################################################################
################################################################################

################################################################################
# nano_component(
#     var_prefix:
# )
################################################################################
function(nano_component  var_prefix)
    nano_project_vars()

    # Cache configuration variables
    set(${var_prefix}_NAME      ${${var_prefix}_LIBRARY}
        CACHE INTERNAL "Short-hand name for ${var_prefix}")

    set(${var_prefix}_DIR      ${prj_dir}
        CACHE INTERNAL "Base directory for ${var_prefix}")

    set(${var_prefix}_LIBRARY_TARGET     ${prj_lib_prefix}${${var_prefix}_LIBRARY}
        CACHE INTERNAL "Target for ${var_prefix}'s library")

    list_prefix(
        ${var_prefix}_SOURCE_FILES
        ${prj_dir}/
        ${${var_prefix}_SOURCE_FILES})
    
    set(${var_prefix}_SOURCE_FILES  ${${var_prefix}_SOURCE_FILES}
        CACHE INTERNAL "List of source files for ${var_prefix}")
    
    list_prefix(
        ${var_prefix}_INCLUDE_FILES_PUBLIC
        ${prj_dir}/
        ${${var_prefix}_INCLUDE_FILES_PUBLIC})
    
    set(${var_prefix}_INCLUDE_FILES_PUBLIC  ${${var_prefix}_INCLUDE_FILES_PUBLIC}
        CACHE INTERNAL "List of public header files for ${var_prefix}")
    
    list_prefix(
        ${var_prefix}_INCLUDE_FILES_PRIVATE
        ${prj_dir}/
        ${${var_prefix}_INCLUDE_FILES_PRIVATE})
    
    set(${var_prefix}_INCLUDE_FILES_PRIVATE  ${${var_prefix}_INCLUDE_FILES_PRIVATE}
        CACHE INTERNAL "List of private header files for ${var_prefix}")
    
    list_prefix(
        ${var_prefix}_INCLUDE_DIRS_PRIVATE
        ${prj_dir}/
        ${${var_prefix}_INCLUDE_DIRS_PRIVATE})

    set(${var_prefix}_INCLUDE_DIRS_PRIVATE  ${${var_prefix}_INCLUDE_DIRS_PRIVATE}
        CACHE INTERNAL "List of private include directories for ${var_prefix}")
    
    if("${CMAKE_BUILD_TYPE}" MATCHES "[dD]ebug")
        list(APPEND ${var_prefix}_DEFINES      NANO_ENABLE_PRECONDITION)
    endif()
    set(${var_prefix}_DEFINES  ${${var_prefix}_DEFINES}
        CACHE INTERNAL "List of compiler definitions for ${var_prefix}")
    
    set(${var_prefix}_LIBS  ${${var_prefix}_LIBS}
        CACHE INTERNAL "List of linked libraries for ${var_prefix}")
    
    set(${var_prefix}_SRC_MAKEFILE      ${PROJECT_NAME}-${${var_prefix}_NAME}.mk
        CACHE INTERNAL "Makefile to load source information about ${var_prefix}")

    set(${var_prefix}_TARGETS_FILE      ${PROJECT_NAME}-${${var_prefix}_NAME}-targets.cmake
        CACHE INTERNAL "CMake targets file for ${var_prefix}")

    set(${var_prefix}_TARGET_EXPORT     ${PROJECT_NAME}-${${var_prefix}_NAME}
        CACHE INTERNAL "Export target group for ${var_prefix}")
    
    set(lib_name_suffix         "")
    if (NOT BUILD_SHARED_LIBS)
        set(lib_name_suffix     "${lib_name_suffix}z")
    endif()
    set(${var_prefix}_LIBRARY_OUTNAME       ${${var_prefix}_LIBRARY_TARGET}${lib_name_suffix}
        CACHE INTERNAL "Output Name for ${var_prefix}'s library")

    set(${var_prefix}_EXAMPLES      ${${var_prefix}_EXAMPLES}
        CACHE INTERNAL "List of examples for ${var_prefix}")

    nano_project_component(${var_prefix})

endfunction()

################################################################################
# nano_component_vars(
#     _pfx:
# )
################################################################################
macro(nano_component_vars    _pfx)
    
    nano_project_vars()

    set(lib_prefix              ${_pfx})
    set(lib_name                ${${_pfx}_NAME})
    set(lib_library             ${${_pfx}_LIBRARY})
    set(lib_tgt                 ${${_pfx}_LIBRARY_TARGET})
    set(lib_outname             ${${_pfx}_LIBRARY_OUTNAME})
    set(lib_sources             ${${_pfx}_SOURCE_FILES})
    set(lib_include_pub         ${${_pfx}_INCLUDE_FILES_PUBLIC})
    set(lib_include_priv        ${${_pfx}_INCLUDE_FILES_PRIVATE})
    set(lib_include_dirs_priv   ${${_pfx}_INCLUDE_DIRS_PRIVATE})
    set(lib_defines             ${${_pfx}_DEFINES})
    set(lib_linked_libs         ${${_pfx}_LIBS})
    set(lib_src_makefile        ${${_pfx}_SRC_MAKEFILE})
    set(lib_tgt_export          ${${_pfx}_TARGET_EXPORT})
    set(lib_tgt_export_file     ${${_pfx}_TARGETS_FILE})
    set(lib_examples            ${${_pfx}_EXAMPLES})

    set(lib_tgt_alias           ${prj_tgt_ns}${lib_tgt})
    

    if (BUILD_SHARED_LIBS)
        set(lib_link_type       SHARED)
    else()
        set(lib_link_type       STATIC)
    endif()

endmacro()

################################################################################
# nano_component_generate_src_makefile(
#     var_prefix:
# )
################################################################################
function(nano_component_generate_src_makefile var_prefix)

    nano_component_vars(${var_prefix})
    
    strip_path_prefix(lib_sources           lib_sources)
    make_fmt_list(lib_sources               lib_sources)

    strip_path_prefix(lib_include_priv      lib_include_priv)
    make_fmt_list(lib_include_priv          lib_include_priv)

    strip_path_prefix(lib_include_pub       lib_include_pub)
    make_fmt_list(lib_include_pub           lib_include_pub)

    make_fmt_list(lib_defines               lib_defines)
    
    strip_target_prefix(lib_linked_libs     lib_linked_libs)
    make_fmt_list(lib_linked_libs           lib_linked_libs)

    configure_file(
        ${NANO_BUILD_SYS_DIR}/nano-lib-src-makefile.in
        ${PROJECT_BINARY_DIR}/${lib_src_makefile}
        @ONLY
        NEWLINE_STYLE UNIX)
    
endfunction()

################################################################################
# nano_component_library(
#     var_prefix:
# )
################################################################################
function(nano_component_library var_prefix)

    nano_component_vars(${var_prefix})

    if (prj_build_libraries)
    
        message(STATUS "Configuring library: ${lib_tgt_alias}")

        add_library(${lib_tgt} ${lib_link_type}
            ${lib_include_pub}
            ${lib_include_priv}
            ${lib_sources})

        set_target_properties(${lib_tgt} PROPERTIES
            POSITION_INDEPENDENT_CODE   ON
            FRAMEWORK                   false
            PUBLIC_HEADER               "${lib_include_pub}"
            OUTPUT_NAME                 "${lib_outname}"
            OUTPUT_NAME_RELEASE         "${lib_outname}"
            OUTPUT_NAME_DEBUG           "${lib_outname}d")

        target_include_directories(${lib_tgt}
                PUBLIC
                    $<INSTALL_INTERFACE:${prj_include_dir_install}>
                    $<BUILD_INTERFACE:${prj_include_dir_build}>)

        # Add MSVC-specific libraries and definition
        if (MSVC)
            target_compile_definitions(${lib_tgt}
                PRIVATE NANO_DLL_EXPORT NANO_DLL_VARIABLE
                PUBLIC WIN32_LEAN_AND_MEAN)
            target_link_libraries(${lib_tgt} PUBLIC ws2_32)
        endif()

        if(NOT "${lib_include_dirs_priv}" STREQUAL "")
            target_include_directories(${lib_tgt}
                PRIVATE     ${lib_include_dirs_priv})
        endif()

        if(NOT "${lib_defines}" STREQUAL "")
            target_compile_definitions(${lib_tgt}
                    PUBLIC     ${lib_defines})
        endif()

        if(NOT "${lib_linked_libs}" STREQUAL "")
            target_link_libraries(${lib_tgt}
                    PUBLIC     ${lib_linked_libs})
        endif()

        add_library(${lib_tgt_alias}   ALIAS   ${lib_tgt})
    
    endif()

    if(prj_install_source)
        nano_component_generate_src_makefile(${var_prefix})
    endif()

endfunction()

################################################################################
# nano_component_executable(
#     var_prefix:
#     exec_name:
# )
################################################################################
function(nano_component_executable var_prefix exec_name)
    nano_component_vars(${var_prefix})

    if (prj_build_executables)
        set(exec_sources        ${${var_prefix}_${exec_name}_SOURCE_FILES})
        set(exec_headers        ${${var_prefix}_${exec_name}_HEADER_FILES})
        set(exec_include_dirs   ${${var_prefix}_${exec_name}_INCLUDE_DIRS})

        add_executable(${exec_name}     ${exec_sources} ${exec_headers})

        if (NOT "${exec_include_dirs}" STREQUAL "")
            target_include_directories(${exec_name} PRIVATE ${exec_include_dirs})
        endif()

        target_link_libraries(${exec_name}        ${prj_tgt_ns}${lib_tgt})

        install(TARGETS     ${exec_name}
            EXPORT ${lib_tgt_export}
            RUNTIME DESTINATION    ${prj_install_dir_bin})
    endif()
    
endfunction()

################################################################################
# nano_component_install(
#     var_prefix:
# )
################################################################################
function(nano_component_install var_prefix)

    nano_component_vars(${var_prefix})

    if (prj_build_libraries)
        install(TARGETS                 ${lib_tgt}
            EXPORT                      ${lib_tgt_export}
            ARCHIVE DESTINATION         ${prj_install_dir_lib}
            LIBRARY DESTINATION         ${prj_install_dir_lib}
            RUNTIME DESTINATION         ${prj_install_dir_lib}
            FRAMEWORK DESTINATION       ${prj_install_dir_lib}
            PUBLIC_HEADER DESTINATION   ${prj_install_dir_include}
            INCLUDES DESTINATION        ${prj_install_dir_include})
    
        install(EXPORT ${lib_tgt_export}
            FILE ${lib_tgt_export_file}
            NAMESPACE ${prj_tgt_ns}
            DESTINATION ${prj_install_dir_cmake})

        export(EXPORT ${lib_tgt_export}
            FILE ${lib_tgt_export_file}
            NAMESPACE ${prj_tgt_ns})
    endif()

    if(prj_install_source)
        install(FILES
            ${lib_sources}
            ${lib_include_priv}
            DESTINATION ${prj_install_dir_src})
        
        install(FILES
            ${lib_include_pub}
            DESTINATION ${prj_install_dir_src}/include)

        install(FILES
            ${PROJECT_BINARY_DIR}/${lib_src_makefile}
            DESTINATION ${prj_install_dir_src})
    endif()
    
endfunction()

function(nano_project_docs)
        
    set(${PROJECT_NAME}_DOC_INPUT_DIRS  ${DOC_INPUT_DIRS}
        CACHE INTERNAL "List of input directories to Doxygen for ${PROJECT_NAME}")

    set(${PROJECT_NAME}_DOC_INPUT_SRC  ${DOC_INPUT_SRC}
        CACHE INTERNAL "List of input source files to Doxygen for ${PROJECT_NAME}")

    set(${PROJECT_NAME}_DOC_SOURCES     ${DOC_SOURCES}
        CACHE INTERNAL "List of Sphinx source files for ${PROJECT_NAME}")

    set(${PROJECT_NAME}_DOC_DEPS        ${DOC_DEPS}
        CACHE INTERNAL "List of other projects which are documentation dependencies for ${PROJECT_NAME}")

    set(${PROJECT_NAME}_DOC_ROOT_INDEX  ${DOC_ROOT_INDEX}
        CACHE INTERNAL "HTML index page to install in doc/")

    set(DOXYGEN                 doxygen
        CACHE STRING "Doxygen executable")
    
    set(SPHINX                  sphinx-build
        CACHE STRING "Sphinx executable")
    
    set(SPHINX_OPTS             ""
        CACHE STRING "Additional arguments passed to Sphinx")

    nano_project_doc_vars()

    if (TARGET ${prj_doc_tgt})
        message(STATUS "Documentation target already configured for ${PROJECT_NAME}")
        return()
    endif()

    set(${PROJECT_NAME}_DOC_TAGFILE     ${prj_doc_tagfile}
        CACHE INTERNAL "Doxygen tagfile generated by ${PROJECT_NAME}")

    message(STATUS "Configuring documentation target for ${PROJECT_NAME}")

    list_prefix(
        prj_doc_sources_bin
        ${prj_doc_build_dir}/
        ${prj_doc_sources})
    
    list_prefix(
        prj_doc_sources_src
        ${prj_doc_dir}/
        ${prj_doc_sources})

    set(prj_dox_deps)
    set(prj_doc_tgt_deps)

    if (NOT prj_build_docs_api AND NOT prj_build_docs_man)
        message(STATUS "Documentation generation DISABLED")
        return()
    endif()
    
    if(prj_build_docs_api)
        configure_file(
            ${NANO_BUILD_SYS_DIR}/doc/Doxyfile.in
            ${prj_doc_doxyfile})
        
        add_custom_command(OUTPUT ${prj_doc_doxygen_dir} ${prj_doc_tagfile}
                COMMAND ${CMAKE_COMMAND} -E make_directory ${prj_doc_doxygen_dir}
                COMMAND ${DOXYGEN} ${prj_doc_doxyfile}
                WORKING_DIRECTORY ${prj_doc_build_dir}
                DEPENDS ${prj_doc_doxyfile}
                        ${prj_doc_doxygen_toc}
                        ${prj_doc_input_src}
                        ${prj_doc_input_dirs}
                        ${prj_doc_tagfile_deps}
                VERBATIM)
        
        add_custom_target(${prj_doc_tgt}-dox
                DEPENDS ${prj_doc_doxygen_dir}
                        ${prj_doc_tagfile}
                        ${prj_doc_deps_tgt})

        if (NOT TARGET doxygen)
            add_custom_target(doxygen)
        endif()

        add_dependencies(doxygen    ${prj_doc_tgt}-dox)

        set(prj_dox_deps        ${prj_doc_doxygen_dir}
                                ${prj_doc_doxyfile}
                                ${prj_doc_doxygen_toc})
        
        install(DIRECTORY ${prj_doc_doxygen_html_dir} 
            DESTINATION ${prj_install_dir_doc_api})
        
        list(APPEND prj_doc_tgt_deps ${prj_doc_tgt}-dox)
    endif()
    
    if(prj_build_docs_man)
        configure_file(
            ${NANO_BUILD_SYS_DIR}/doc/sphinx-conf.py.in
            ${prj_doc_sphinxconf})
        
        add_custom_command(OUTPUT ${prj_doc_sources_bin}
            COMMAND ${CMAKE_COMMAND} -E make_directory ${prj_doc_build_dir}
            COMMAND $(CMAKE_COMMAND) -E copy ${NANO_BUILD_SYS_DIR}/doc/sphinx-extlinks.py ${prj_doc_build_dir}
            COMMAND $(CMAKE_COMMAND) -E rename ${prj_doc_build_dir}/sphinx-extlinks.py ${prj_doc_build_dir}/extlinks.py
            COMMAND $(CMAKE_COMMAND) -E copy ${prj_doc_sources_src} ${prj_doc_build_dir}
            COMMAND $(CMAKE_COMMAND) -E copy_directory ${prj_dir}/doc/static ${prj_doc_build_dir}
            DEPENDS ${prj_doc_sources_src} ${extlinks_src}
            VERBATIM)
    
        add_custom_command(OUTPUT ${prj_doc_html_dir}
                        COMMAND ${SPHINX}   ${prj_doc_build_dir}
                                            ${prj_doc_html_dir}
                                            ${SPHINX_OPTS}
                        DEPENDS ${prj_dox_deps}
                                ${prj_doc_sphinxconf}
                                ${prj_doc_sources_bin}
                                ${NANO_BUILD_SYS_DIR}/doc/sphinx-extlinks.py
                                ${prj_doc_input_src}
                        VERBATIM)
    
        install(DIRECTORY ${prj_doc_html_dir} 
                DESTINATION ${prj_install_dir_doc_man})
        
        list(APPEND prj_doc_tgt_deps ${prj_doc_html_dir})
    endif()

    if (NOT "${prj_doc_root_index}" STREQUAL "" AND
        EXISTS "${prj_doc_root_index}")
        add_custom_command(OUTPUT ${prj_doc_build_dir}/index.html
                COMMAND ${CMAKE_COMMAND} -E copy ${prj_doc_root_index} ${prj_doc_build_dir}
                COMMAND $(CMAKE_COMMAND) -E rename ${prj_doc_build_dir}/${prj_doc_root_index_rel} ${prj_doc_build_dir}/index.html
                DEPENDS ${prj_doc_tagfile_deps_tgt_index}
                VERBATIM)

        install(FILES ${prj_doc_build_dir}/index.html
            DESTINATION ${prj_install_dir_doc})

        add_custom_target(${prj_doc_tgt}-index
            DEPENDS ${prj_doc_build_dir}/index.html)

        list(APPEND prj_doc_tgt_deps ${prj_doc_tgt}-index)
    endif()
   
    add_custom_target(${prj_doc_tgt} ALL DEPENDS ${prj_doc_tgt_deps})

endfunction()
