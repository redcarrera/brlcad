set(toplevel_HTML bookmarks.html toc.html)
add_doc(toplevel_HTML html REQUIRED "mged")

set(
  cadwidgets_HTML
  manuals/cadwidgets/Command.html
  manuals/cadwidgets/Database.html
  manuals/cadwidgets/Db.html
  manuals/cadwidgets/Display.html
  manuals/cadwidgets/Dm.html
  manuals/cadwidgets/Drawable.html
  manuals/cadwidgets/Mged.html
  manuals/cadwidgets/QuadDisplay.html
  manuals/cadwidgets/Splash.html
  manuals/cadwidgets/View.html
  manuals/cadwidgets/contents.html
  manuals/cadwidgets/index.html
  manuals/cadwidgets/skeleton.html
)
add_doc(cadwidgets_HTML html/manuals/cadwidgets REQUIRED "mged")

set(libbu_HTML manuals/libbu/cmdhist_obj.html manuals/libbu/contents.html manuals/libbu/index.html)
add_doc(libbu_HTML html/manuals/libbu REQUIRED "libbu")

set(
  libdm_HTML
  manuals/libdm/api.html
  manuals/libdm/contents.html
  manuals/libdm/dm_obj.html
  manuals/libdm/index.html
  manuals/libdm/preface.html
  manuals/libdm/tcl.html
)
add_doc(libdm_HTML html/manuals/libdm REQUIRED "libdm")

set(
  librt_HTML
  manuals/librt/contents.html
  manuals/librt/dg_obj.html
  manuals/librt/index.html
  manuals/librt/view_obj.html
  manuals/librt/wdb_obj.html
)
add_doc(librt_HTML html/manuals/librt REQUIRED "librt")

set(
  manuals_HTML
  manuals/BRL-CAD_gear_logo.ico
  manuals/Install.html
  manuals/Obtain.html
  manuals/Overview.html
  manuals/eagleCAD.gif
  manuals/eagleCAD.bmp
  manuals/index.html
  manuals/small-eagleCAD.gif
)
add_doc(manuals_HTML html/manuals REQUIRED "mged")

set(
  manuals_mged_HTML
  manuals/mged/az_el.gif
  manuals/mged/az_el.jpg
  manuals/mged/az_el_sm.gif
  manuals/mged/azel.jpg
  manuals/mged/base.gif
  manuals/mged/base.jpg
  manuals/mged/bool.gif
  manuals/mged/brlcad_glossary.html
  manuals/mged/brlcad_solids.html
  manuals/mged/build_def_index.sh
  manuals/mged/cmd_line_ed.html
  manuals/mged/contents.html
  manuals/mged/coord-axes.gif
  manuals/mged/cup.g
  manuals/mged/cup.sh
  manuals/mged/cup_and_mug.gif
  manuals/mged/cup_and_mug_small.gif
  manuals/mged/cup_out+in.gif
  manuals/mged/cup_outside.gif
  manuals/mged/cup_w_handle.gif
  manuals/mged/default_key_bindings.html
  manuals/mged/default_mouse_bindings.html
  manuals/mged/faceplate.gif
  manuals/mged/faceplate_menu.gif
  manuals/mged/faceplate_menu_sm.gif
  manuals/mged/faceplate_sm.gif
  manuals/mged/fillet.gif
  manuals/mged/fillet.jpg
  manuals/mged/ged.html
  manuals/mged/ged.tex
  manuals/mged/handle.gif
  manuals/mged/handle.jpg
  manuals/mged/index.html
  manuals/mged/mged.html
  manuals/mged/mged2.html
  manuals/mged/mged3.html
  manuals/mged/mged_callbacks.html
  manuals/mged/mged_cmd_index.html
  manuals/mged/mged_env_vars.html
  manuals/mged/mged_gui.html
  manuals/mged/mged_tcl_vars.html
  manuals/mged/mug
  manuals/mged/mug.gif
  manuals/mged/mug_camo
  manuals/mged/mug_camo.gif
  manuals/mged/mug_camo.jpg
  manuals/mged/mug_green.gif
  manuals/mged/mug_green.jpg
  manuals/mged/peewee.gif
  manuals/mged/peewee.jpg
  manuals/mged/preface.html
  manuals/mged/prims.gif
  manuals/mged/rim.gif
  manuals/mged/rim.jpg
  manuals/mged/shaders.html
  manuals/mged/tex-html.tcl
)
add_doc(manuals_mged_HTML html/manuals/mged REQUIRED "mged")

add_doc(manuals/shaders/camo.html html/manuals/shaders REQUIRED "liboptical")

set(
  relnotes_HTML
  ReleaseNotes/email2.0.html
  ReleaseNotes/email3.0.html
  ReleaseNotes/email3.1.html
  ReleaseNotes/email4.0.html
  ReleaseNotes/email4.4.html
  ReleaseNotes/email5.0.html
  ReleaseNotes/index.html
)
add_doc(relnotes_HTML html/ReleaseNotes REQUIRED "librt")

set(
  relnotes_5_HTML
  ReleaseNotes/Rel5.0/deprecated.html
  ReleaseNotes/Rel5.0/index.html
  ReleaseNotes/Rel5.0/new_cmds.html
  ReleaseNotes/Rel5.0/new_libs.html
)
add_doc(relnotes_5_HTML html/ReleaseNotes/Rel5.0 REQUIRED "librt")

set(
  relnotes_5_summary_HTML
  ReleaseNotes/Rel5.0/Summary/activem.gif
  ReleaseNotes/Rel5.0/Summary/activep.gif
  ReleaseNotes/Rel5.0/Summary/collapse.gif
  ReleaseNotes/Rel5.0/Summary/expand.gif
  ReleaseNotes/Rel5.0/Summary/first.gif
  ReleaseNotes/Rel5.0/Summary/home.gif
  ReleaseNotes/Rel5.0/Summary/ielogo.gif
  ReleaseNotes/Rel5.0/Summary/img001.jpg
  ReleaseNotes/Rel5.0/Summary/img002.jpg
  ReleaseNotes/Rel5.0/Summary/img003.jpg
  ReleaseNotes/Rel5.0/Summary/img004.jpg
  ReleaseNotes/Rel5.0/Summary/img005.jpg
  ReleaseNotes/Rel5.0/Summary/img006.jpg
  ReleaseNotes/Rel5.0/Summary/img007.jpg
  ReleaseNotes/Rel5.0/Summary/img008.jpg
  ReleaseNotes/Rel5.0/Summary/img009.jpg
  ReleaseNotes/Rel5.0/Summary/img010.jpg
  ReleaseNotes/Rel5.0/Summary/img011.jpg
  ReleaseNotes/Rel5.0/Summary/img012.jpg
  ReleaseNotes/Rel5.0/Summary/img013.jpg
  ReleaseNotes/Rel5.0/Summary/img014.jpg
  ReleaseNotes/Rel5.0/Summary/img015.jpg
  ReleaseNotes/Rel5.0/Summary/img016.jpg
  ReleaseNotes/Rel5.0/Summary/img017.jpg
  ReleaseNotes/Rel5.0/Summary/img018.jpg
  ReleaseNotes/Rel5.0/Summary/img019.jpg
  ReleaseNotes/Rel5.0/Summary/index.html
  ReleaseNotes/Rel5.0/Summary/info.gif
  ReleaseNotes/Rel5.0/Summary/last.gif
  ReleaseNotes/Rel5.0/Summary/next.gif
  ReleaseNotes/Rel5.0/Summary/pptani.gif
  ReleaseNotes/Rel5.0/Summary/prev.gif
  ReleaseNotes/Rel5.0/Summary/sld001.htm
  ReleaseNotes/Rel5.0/Summary/sld002.htm
  ReleaseNotes/Rel5.0/Summary/sld003.htm
  ReleaseNotes/Rel5.0/Summary/sld004.htm
  ReleaseNotes/Rel5.0/Summary/sld005.htm
  ReleaseNotes/Rel5.0/Summary/sld006.htm
  ReleaseNotes/Rel5.0/Summary/sld007.htm
  ReleaseNotes/Rel5.0/Summary/sld008.htm
  ReleaseNotes/Rel5.0/Summary/sld009.htm
  ReleaseNotes/Rel5.0/Summary/sld010.htm
  ReleaseNotes/Rel5.0/Summary/sld011.htm
  ReleaseNotes/Rel5.0/Summary/sld012.htm
  ReleaseNotes/Rel5.0/Summary/sld013.htm
  ReleaseNotes/Rel5.0/Summary/sld014.htm
  ReleaseNotes/Rel5.0/Summary/sld015.htm
  ReleaseNotes/Rel5.0/Summary/sld016.htm
  ReleaseNotes/Rel5.0/Summary/sld017.htm
  ReleaseNotes/Rel5.0/Summary/sld018.htm
  ReleaseNotes/Rel5.0/Summary/sld019.htm
  ReleaseNotes/Rel5.0/Summary/space.gif
  ReleaseNotes/Rel5.0/Summary/text.gif
  ReleaseNotes/Rel5.0/Summary/tsld001.htm
  ReleaseNotes/Rel5.0/Summary/tsld002.htm
  ReleaseNotes/Rel5.0/Summary/tsld003.htm
  ReleaseNotes/Rel5.0/Summary/tsld004.htm
  ReleaseNotes/Rel5.0/Summary/tsld005.htm
  ReleaseNotes/Rel5.0/Summary/tsld006.htm
  ReleaseNotes/Rel5.0/Summary/tsld007.htm
  ReleaseNotes/Rel5.0/Summary/tsld008.htm
  ReleaseNotes/Rel5.0/Summary/tsld009.htm
  ReleaseNotes/Rel5.0/Summary/tsld010.htm
  ReleaseNotes/Rel5.0/Summary/tsld011.htm
  ReleaseNotes/Rel5.0/Summary/tsld012.htm
  ReleaseNotes/Rel5.0/Summary/tsld013.htm
  ReleaseNotes/Rel5.0/Summary/tsld014.htm
  ReleaseNotes/Rel5.0/Summary/tsld015.htm
  ReleaseNotes/Rel5.0/Summary/tsld016.htm
  ReleaseNotes/Rel5.0/Summary/tsld017.htm
  ReleaseNotes/Rel5.0/Summary/tsld018.htm
  ReleaseNotes/Rel5.0/Summary/tsld019.htm
)
add_doc(relnotes_5_summary_HTML html/ReleaseNotes/Rel5.0/Summary REQUIRED "mged")

add_doc(ReleaseNotes/Rel6.0/index.html html/ReleaseNotes/Rel6.0 REQUIRED "mged")

set(
  html_ignore_files
  CMakeLists.txt
  ReleaseNotes/Rel6.0/index.html
  manuals/shaders/camo.html
  manuals/small-eagleCAD.ico
)
cmakefiles(${html_ignore_files})

# Local Variables:
# tab-width: 8
# mode: cmake
# indent-tabs-mode: t
# End:
# ex: shiftwidth=2 tabstop=8
