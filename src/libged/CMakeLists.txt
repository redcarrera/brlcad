# LIBGED is set up based on a plugin architecture, with common functionality
# defined in a core library.

brlcad_find_package(LMDB REQUIRED)
brlcad_find_package(PNG REQUIRED)
brlcad_find_package(REGEX REQUIRED)

add_custom_command(
  OUTPUT "${CMAKE_BINARY_DIR}/ged_plugins.sentinel"
  COMMAND ${CMAKE_COMMAND} -E touch "${CMAKE_BINARY_DIR}/ged_plugins.sentinel"
)
add_custom_target(ged_plugins_sentinel DEPENDS ${CMAKE_BINARY_DIR}/ged_plugins.sentinel)
set_target_properties(ged_plugins_sentinel PROPERTIES FOLDER "BRL-CAD Plugins/ged")

function(ged_plugin_library name)
  add_library(${name} ${ARGN})
  set(dep_includes)
  foreach(ll ${libslist})
    if(TARGET ${ll})
      get_target_property(IDIRS ${ll} INTERFACE_INCLUDE_DIRECTORIES)
      if(IDIRS)
        list(APPEND dep_includes ${IDIRS})
      endif(IDIRS)
    endif(TARGET ${ll})
  endforeach(ll ${libslist})
  brlcad_include_dirs(${name} dep_includes PRIVATE)
  add_dependencies(ged_plugins_sentinel ${name})
endfunction(ged_plugin_library name)

# This target is supplied so applications wanting to run libged
# commands at build time can depend on the plugins being built as well as
# libged itself (otherwise LIBGED commands might fail due to their plugins not
# yet having been built.)
add_custom_target(ged_plugins ALL DEPENDS ged_plugins_sentinel)
set_target_properties(ged_plugins PROPERTIES FOLDER "BRL-CAD Plugins")

# In addition to the standard command plugins, there are plugins for the
# ged_exec subprocess executable, which are run as independent processes
# to allow them to be terminated by the parent command without bringing
# down the parent application
add_custom_command(
  OUTPUT "${CMAKE_BINARY_DIR}/ged_subprocesses.sentinel"
  COMMAND ${CMAKE_COMMAND} -E touch "${CMAKE_BINARY_DIR}/ged_subprocesses.sentinel"
)
add_custom_target(ged_subprocesses_sentinel DEPENDS ${CMAKE_BINARY_DIR}/ged_subprocesses.sentinel)
set_target_properties(ged_subprocesses_sentinel PROPERTIES FOLDER "BRL-CAD Plugins/ged_process")

function(ged_subprocess_library name)
  add_library(${name} ${ARGN})
  set_target_properties(${name} PROPERTIES PREFIX "")
  set(dep_includes)
  foreach(ll ${libslist})
    if(TARGET ${ll})
      get_target_property(IDIRS ${ll} INTERFACE_INCLUDE_DIRECTORIES)
      if(IDIRS)
        list(APPEND dep_includes ${IDIRS})
      endif(IDIRS)
    endif(TARGET ${ll})
  endforeach(ll ${libslist})
  brlcad_include_dirs(${name} dep_includes PRIVATE)
  add_dependencies(ged_subprocesses_sentinel ${name})
endfunction(ged_subprocess_library name)

# This target is supplied so applications wanting to run libged
# commands at build time can depend on the subprocesses being built as well as
# libged itself (otherwise LIBGED commands might fail due to their subprocesses not
# yet having been built.)
add_custom_target(ged_subprocesses ALL DEPENDS ged_subprocesses_sentinel)
set_target_properties(ged_subprocesses PROPERTIES FOLDER "BRL-CAD Plugins")

set(
  LIBGED_SOURCES
  exec.cpp
  exec_mapping.cpp
  ged_init.cpp
  columns.c
  dbi_state.cpp
  display_list.c
  draw.cpp
  facedef.c
  ged.cpp
  ged_util.cpp
  get_obj_bounds.c
  get_solid_kp.c
  inside.c
  path.c
  pipe.c
  pnts_util.c
  points_eval.c
  polyclip.cpp
  qray.c
  rotate_eto.c
  rotate_extrude.c
  rotate_hyp.c
  rotate_tgc.c
  tab_complete.cpp
  trace.c
  track.c
  translate_extrude.c
  translate_tgc.c
  vutil.c
  view/data_lines.c
  wdb_importFg4Section.c
  wireframe_eval.c
)

set_property(
  SOURCE ged_init.cpp
  APPEND
  PROPERTY COMPILE_DEFINITIONS "GED_PLUGIN_SUFFIX=\"${CMAKE_SHARED_LIBRARY_SUFFIX}\""
)

# Note - libged_deps is defined by ${BRLCAD_SOURCE_DIR}/src/source_dirs.cmake
set(
  GED_LIBS
  ${libged_deps}
  ${PNG_LIBRARIES}
  ${LMDB_LIBRARIES}
  ${REGEX_LIBRARIES}
  ${WINSOCK_LIB}
  ${M_LIBRARY}
)

# Local include directories
# includes from plugins
set(
  GED_LOCAL_INCLUDE_DIRS
  ${PNG_INCLUDE_DIRS}
  ${REGEX_INCLUDE_DIRS}
  ${LMDB_INCLUDE_DIRS}
  ${CMAKE_CURRENT_SOURCE_DIR}/../libbg
  ${CMAKE_CURRENT_BINARY_DIR}
)

brlcad_addlib(libged "${LIBGED_SOURCES}" "${GED_LIBS}" "" "${GED_LOCAL_INCLUDE_DIRS}")
set_target_properties(libged PROPERTIES VERSION 20.0.1 SOVERSION 20)

# This executable is used to launch processes from LIBGED commands that may
# need to be terminated due to excessive runtime length or infinite looping.
# It is NOT intended be run directly by users (although it CAN be so run
# for debugging purposes if necessary.)
brlcad_addexec(ged_exec ged_exec.cpp libged NO_MAN)

add_subdirectory(tests)
add_subdirectory(simulate/tests)

brlcad_addexec(test_help help/test_help.c libged TEST)

# Plugins
add_subdirectory(3ptarb)
add_subdirectory(adc)
add_subdirectory(adjust)
add_subdirectory(ae2dir)
add_subdirectory(analyze)
add_subdirectory(annotate)
add_subdirectory(arb)
add_subdirectory(arced)
add_subdirectory(arot)
add_subdirectory(attr)
add_subdirectory(bb)
add_subdirectory(bev)
add_subdirectory(bigE)
add_subdirectory(blast)
add_subdirectory(bo)
add_subdirectory(bot)
add_subdirectory(brep)
add_subdirectory(cat)
add_subdirectory(cc)
add_subdirectory(check)
add_subdirectory(clone)
add_subdirectory(close)
add_subdirectory(coil)
add_subdirectory(color)
add_subdirectory(comb)
add_subdirectory(comb_color)
add_subdirectory(comb_std)
add_subdirectory(combmem)
add_subdirectory(concat)
add_subdirectory(constraint)
add_subdirectory(copy)
add_subdirectory(copyeval)
add_subdirectory(copymat)
add_subdirectory(cpi)
add_subdirectory(dbip)
add_subdirectory(debug)
add_subdirectory(debugbu)
add_subdirectory(debugdir)
add_subdirectory(debuglib)
add_subdirectory(debugnmg)
add_subdirectory(decompose)
add_subdirectory(delay)
add_subdirectory(dir2ae)
add_subdirectory(dm)
add_subdirectory(draw)
add_subdirectory(dsp)
add_subdirectory(dump)
add_subdirectory(dup)
add_subdirectory(eac)
add_subdirectory(echo)
add_subdirectory(edarb)
add_subdirectory(edcodes)
add_subdirectory(edcomb)
add_subdirectory(edit)
add_subdirectory(editit)
add_subdirectory(edmater)
add_subdirectory(env)
add_subdirectory(erase)
add_subdirectory(exists)
add_subdirectory(expand)
add_subdirectory(eye_pos)
add_subdirectory(facetize)
add_subdirectory(fb2pix)
add_subdirectory(fbclear)
add_subdirectory(find)
add_subdirectory(form)
add_subdirectory(fracture)
add_subdirectory(garbage_collect)
add_subdirectory(gdiff)
add_subdirectory(get)
add_subdirectory(get_autoview)
add_subdirectory(get_comb)
add_subdirectory(get_eyemodel)
add_subdirectory(get_type)
add_subdirectory(glob)
add_subdirectory(gqa)
add_subdirectory(graph)
add_subdirectory(grid)
add_subdirectory(grid2model_lu)
add_subdirectory(grid2view_lu)
add_subdirectory(group)
add_subdirectory(heal)
add_subdirectory(help)
add_subdirectory(hide)
add_subdirectory(how)
add_subdirectory(human)
add_subdirectory(illum)
add_subdirectory(importFg4Section)
add_subdirectory(inside)
add_subdirectory(instance)
add_subdirectory(isize)
add_subdirectory(item)
add_subdirectory(joint)
add_subdirectory(joint2)
add_subdirectory(keep)
add_subdirectory(keypoint)
add_subdirectory(kill)
add_subdirectory(killall)
add_subdirectory(killrefs)
add_subdirectory(killtree)
add_subdirectory(label)
add_subdirectory(lc)
add_subdirectory(libfuncs)
add_subdirectory(lint)
add_subdirectory(list)
add_subdirectory(lod)
add_subdirectory(log)
add_subdirectory(ls)
add_subdirectory(lt)
add_subdirectory(m2v_point)
add_subdirectory(make)
add_subdirectory(make_name)
add_subdirectory(man)
add_subdirectory(match)
add_subdirectory(mater)
add_subdirectory(material)
add_subdirectory(metaball)
add_subdirectory(mirror)
add_subdirectory(model2grid_lu)
add_subdirectory(model2view)
add_subdirectory(model2view_lu)
add_subdirectory(move)
add_subdirectory(move_all)
add_subdirectory(move_arb_edge)
add_subdirectory(move_arb_face)
add_subdirectory(mrot)
add_subdirectory(nirt)
add_subdirectory(nmg)
add_subdirectory(npush)
add_subdirectory(ocenter)
add_subdirectory(open)
add_subdirectory(orient)
add_subdirectory(orotate)
add_subdirectory(oscale)
add_subdirectory(otranslate)
add_subdirectory(overlay)
add_subdirectory(pathlist)
add_subdirectory(pathsum)
add_subdirectory(perspective)
add_subdirectory(pipe)
add_subdirectory(pix2fb)
add_subdirectory(plot)
add_subdirectory(pmat)
add_subdirectory(pmodel2view)
add_subdirectory(png)
add_subdirectory(png2fb)
add_subdirectory(pnts)
add_subdirectory(prcolor)
add_subdirectory(prefix)
add_subdirectory(process)
add_subdirectory(protate)
add_subdirectory(ps)
add_subdirectory(pscale)
add_subdirectory(pset)
add_subdirectory(ptranslate)
add_subdirectory(pull)
add_subdirectory(push)
add_subdirectory(put)
add_subdirectory(put_comb)
add_subdirectory(putmat)
add_subdirectory(qray)
add_subdirectory(rcodes)
add_subdirectory(rect)
add_subdirectory(red)
add_subdirectory(regdef)
add_subdirectory(region)
add_subdirectory(remove)
add_subdirectory(rfarb)
add_subdirectory(rmap)
add_subdirectory(rmat)
add_subdirectory(rmater)
add_subdirectory(rot)
add_subdirectory(rot_point)
add_subdirectory(rrt)
add_subdirectory(rt)
add_subdirectory(rtabort)
add_subdirectory(rtcheck)
add_subdirectory(rtwizard)
add_subdirectory(savekey)
add_subdirectory(scale)
add_subdirectory(search)
add_subdirectory(select)
add_subdirectory(set_output_script)
add_subdirectory(set_transparency)
add_subdirectory(set_uplotOutputMode)
add_subdirectory(setview)
add_subdirectory(shaded_mode)
add_subdirectory(shader)
add_subdirectory(shells)
add_subdirectory(showmats)
add_subdirectory(simulate)
add_subdirectory(slew)
add_subdirectory(solid_report)
add_subdirectory(solids_on_ray)
add_subdirectory(sphgroup)
add_subdirectory(stat)
add_subdirectory(summary)
add_subdirectory(sync)
add_subdirectory(tables)
add_subdirectory(tire)
add_subdirectory(title)
add_subdirectory(tol)
add_subdirectory(tops)
add_subdirectory(tra)
add_subdirectory(track)
add_subdirectory(tree)
add_subdirectory(typein)
add_subdirectory(unhide)
add_subdirectory(units)
add_subdirectory(v2m_point)
add_subdirectory(vdraw)
add_subdirectory(version)
add_subdirectory(view)
add_subdirectory(view2grid_lu)
add_subdirectory(view2model)
add_subdirectory(view2model_lu)
add_subdirectory(view2model_vec)
add_subdirectory(voxelize)
add_subdirectory(vrot)
add_subdirectory(wcodes)
add_subdirectory(whatid)
add_subdirectory(which)
add_subdirectory(which_shader)
add_subdirectory(who)
add_subdirectory(wmater)
add_subdirectory(xpush)
add_subdirectory(zap)
add_subdirectory(zoom)

set(
  ged_ignore_files
  CMakeLists.txt
  NOTES
  README
  TODO
  alphanum.h
  bot/ged_bot.h
  brep/ged_brep.h
  check/check_private.h
  ged_private.h
  joint/joint.h
  osg.cpp
  pscale/pscale.h
  pnts_util.h
  qray.h
  simulate/NOTES
  simulate/rt_collision_algorithm.hpp
  simulate/rt_collision_shape.hpp
  simulate/rt_debug_draw.hpp
  simulate/rt_instance.hpp
  simulate/rt_motion_state.hpp
  simulate/simulation.hpp
  simulate/utility.hpp
  include/plugin.h
)
cmakefiles(${ged_ignore_files})

# Local Variables:
# tab-width: 8
# mode: cmake
# indent-tabs-mode: t
# End:
# ex: shiftwidth=2 tabstop=8
