set(
  lessons_ES
  mged01_crear_figuras_primitivas.xml
  mged02_opciones_vistas.xml
  mged03_utilizar_comando_in.xml
  mged04_prop_materiales.xml
  mged05_expresiones_booleanas.xml
  mged06_crear_una_copa.xml
  mged07_prop_materiales_copa.xml
  mged08_prop_materiales_copa2.xml
  mged09_esfera_en_caja_exhibidora.xml
  mged10_crear_taza.xml
  mged11_refinar_taza.xml
  mged12_taza_desde_gui.xml
  mged13_ubicar_figuras_en_3d.xml
  mged14_ubicar_figuras_en_3d_2.xml
  mged15_crear_camion_de_juguete.xml
  mged16_mod_tech_struct.xml
)

set(
  lessons_ES_IMAGES
  images/mged01_commandwindow.png
  images/mged01_graphicswindow.png
  images/mged01_terminal.png
  images/mged02_coordsys.png
  images/mged02_defaultradio.png
  images/mged02_flower_zooomin.png
  images/mged02_flower_zooomout.png
  images/mged02_multipane.png
  images/mged02_radio_35_25.png
  images/mged02_radio_front.png
  images/mged02_radio_right.png
  images/mged02_radio_top.png
  images/mged02_truck_0_0_14.png
  images/mged02_truck_270_-90.png
  images/mged02_truck_270_90.png
  images/mged02_truck_35_0.png
  images/mged02_truck_35_25.png
  images/mged02_truck_35_60.png
  images/mged02_truck_35_90.png
  images/mged02_truck_front.png
  images/mged02_truck_left.png
  images/mged02_truck_rear.png
  images/mged02_truck_right.png
  images/mged04_commandwindow.png
  images/mged04_fb_overlay_mode.png
  images/mged04_raytracedshapes.png
  images/mged05_combinationeditor.png
  images/mged05_intersectionspheres.png
  images/mged05_raytraceddome.png
  images/mged05_raytracedpart1.png
  images/mged05_raytracedpart2.png
  images/mged05_raytracedpart3.png
  images/mged05_subtractionspheres.png
  images/mged05_twoprimitivespheres.png
  images/mged05_unionspheres.png
  images/mged06_base_substracted_vol.png
  images/mged06_goblet1stsphere.png
  images/mged06_gobletbase.png
  images/mged06_gobletbasestembasin.png
  images/mged06_gobletstem.png
  images/mged06_rtgoblet.png
  images/mged06_rtgobletaz_35_el_25.png
  images/mged07_goblet_complete_window.png
  images/mged07_goblet_mater_complete.png
  images/mged07_goblet_mirror.png
  images/mged07_goblet_semitransparent.png
  images/mged08_comb_editor_window_mater_prop.png
  images/mged08_goblet_combeditor.png
  images/mged08_goblet_reflectivity_1.png
  images/mged08_goblet_reflectivity_2.png
  images/mged08_refractive_index.png
  images/mged08_spec_vs_diff_reflectivity.png
  images/mged09_globe_base_box_raytraced.png
  images/mged09_globe_base_box_wireframe.png
  images/mged09_globe_inside_outside_box.png
  images/mged09_globe_outside_display_box.png
  images/mged09_globe_raytraced.png
  images/mged09_globe_wireframe.png
  images/mged10_mug_wireframe.png
  images/mged11_mug_raytraced_norim.png
  images/mged11_mug_raytraced_rim.png
  images/mged12_cyls_frontview.png
  images/mged12_mug_gui_finished_raytraced.png
  images/mged12_mug_handle_frontview.png
  images/mged12_mug_handle_topview.png
  images/mged12_mug_rim_frontview.png
  images/mged13_candle_comb_ed_checker.png
  images/mged13_candle_complete_front.png
  images/mged13_candle_design.png
  images/mged13_candle_raytraced_overlay.png
  images/mged13_candle_tabletop_base_candle_front.png
  images/mged13_candle_tabletop_base_front.png
  images/mged13_candle_tabletop_eto1_rcc_wireframe_front.png
  images/mged13_candle_tabletop_eto1_wireframe_front.png
  images/mged13_candle_tabletop_wireframe_front.png
  images/mged14_candle_base_sphere_topview.png
  images/mged14_candle_multipane.png
  images/mged14_candle_spheres_az35el25.png
  images/mged14_candle_spheres_frontview.png
  images/mged14_candle_spheres_raytraced.png
  images/mged14_candle_spheres_topview.png
  images/mged14_primitive_editor.png
  images/mged14_primitive_selection_dropdown.png
  images/mged15_cameo_shader.png
  images/mged15_prim_ed_wheel_well.png
  images/mged15_truck_2nd_wheel_wheel_place.png
  images/mged15_truck_ambient_lighting.png
  images/mged15_truck_cab1s.png
  images/mged15_truck_cab_body.png
  images/mged15_truck_cab_cutoff.png
  images/mged15_truck_default_lighting.png
  images/mged15_truck_design.png
  images/mged15_truck_multipane_1stwheel.png
  images/mged15_truck_wheel_wells_wireframe.png
  images/mged15_truck_wheels.png
  images/mged16_radio_casecut_raytrace.png
  images/mged16_radio_cut_raytrace.png
  images/mged16_radio_cut_wireframe.png
  images/mged16_radio_cutaways.png
  images/mged16_radio_default_components_props.png
  images/mged16_radio_new_design.png
  images/mged16_radio_structure_1.png
  images/mged16_radio_structure_2.png
  images/mged16_radio_structure_3.png
  images/mged16_radio_transparent.png
  images/mged16_radio_wireframe_circ_board.png
  images/mged16_radio_with_mater_props.png
)
if(TARGET mged OR TARGET qged OR TARGET archer)
  if(BRLCAD_EXTRADOCS)
    add_doc(lessons_ES_IMAGES html/lessons/es/images)
    add_docbook("HTML;PHP;PDF" lessons_ES lessons/es lessons_ES_IMAGES_cp)
  endif(BRLCAD_EXTRADOCS)
endif(TARGET mged OR TARGET qged OR TARGET archer)

cmakefiles(
  CMakeLists.txt
  ${lessons_ES}
  ${lessons_ES_IMAGES}
)

# Local Variables:
# tab-width: 8
# mode: cmake
# indent-tabs-mode: t
# End:
# ex: shiftwidth=2 tabstop=8
