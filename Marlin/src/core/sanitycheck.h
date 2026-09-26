#if ENABLED(BED_MESH_VIEWER)
  #if !HAS_MARLINUI_U8GLIB
    #error "BED_MESH_VIEWER requires a graphical LCD compatible with U8glib (e.g., MINI12864)."
  #endif
  #if !HAS_LEVELING
    #error "BED_MESH_VIEWER requires at least one leveling system enabled (MESH, BILINEAR, or UBL)."
  #endif
#endif