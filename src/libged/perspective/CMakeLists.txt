include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${BRLCAD_BINARY_DIR}/include
  ${BRLCAD_SOURCE_DIR}/include
  ${BU_INCLUDE_DIRS}
  ${GED_INCLUDE_DIRS}
)

add_definitions(-DGED_PLUGIN)
ged_plugin_library(ged-perspective SHARED perspective.c)
target_link_libraries(ged-perspective libged libbu)
set_property(TARGET ged-perspective APPEND PROPERTY COMPILE_DEFINITIONS BRLCADBUILD HAVE_CONFIG_H)
validate_style(ged-perspective perspective.c)
plugin_setup(ged-perspective ged)

cmakefiles(
  CMakeLists.txt
  perspective.c
)

# Local Variables:
# tab-width: 8
# mode: cmake
# indent-tabs-mode: t
# End:
# ex: shiftwidth=2 tabstop=8
