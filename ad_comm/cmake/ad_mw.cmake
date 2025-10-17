macro(ADD_AD_MW_DEPENDENCIES)
    find_package(cyber REQUIRED PATH ${SENSEAUTO_DEV_INSTALLED_ROOT}/${FIND_SENSEAUTO_PACKAGE_PATH}/senseauto_active/senseauto-rscl/3rdparty/cmake/)

    target_compile_definitions(ad_rscl_interface INTERFACE MW_IMPL_CYBER)
    target_link_libraries(ad_rscl_interface INTERFACE ${CYBER_LIBRARIES} ad_mw_cyber ad_mw_pb ad_base_common)
    target_include_directories(ad_rscl_interface INTERFACE ${CYBER_INCLUDE_DIRS})
    target_link_directories(ad_rscl_interface INTERFACE
        ${CYBER_LIBRARY_DIRS})
    # target_link_options(ad_rscl_interface PRIVATE
    #     ${CYBER_LINK_OPTIONS}
    # )
endmacro()
