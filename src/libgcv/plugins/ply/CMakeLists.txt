set(PLY_SRCS ply_read.c ply_write.c rply.c)

gcv_plugin_library(gcv-ply SHARED ${PLY_SRCS})
target_link_libraries(gcv-ply libgcv libwdb librt)
set_property(TARGET gcv-ply APPEND PROPERTY COMPILE_DEFINITIONS BRLCADBUILD HAVE_CONFIG_H)
validate_style(gcv-ply "${PLY_SRCS}")
plugin_setup(gcv-ply gcv)

cmakefiles(
   ${PLY_SRCS}
   CMakeLists.txt
   rply.h
   rplyfile.h
   rply_license.txt
)

# Local Variables:
# tab-width: 8
# mode: cmake
# indent-tabs-mode: t
# End:
# ex: shiftwidth=2 tabstop=8
