#if ENABLED(BED_MESH_VIEWER)
  #if !HAS_MARLINUI_U8GLIB
    #error "BED_MESH_VIEWER richiede un display grafico compatibile con U8glib (es. MINI12864)."
  #endif
  #if !HAS_LEVELING
    #error "BED_MESH_VIEWER richiede che sia abilitato almeno un sistema di livellamento (MESH, BILINEAR o UBL)."
  #endif
#endif