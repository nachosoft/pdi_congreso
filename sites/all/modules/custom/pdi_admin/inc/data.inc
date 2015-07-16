<?php
// Create menu for users
function pdi_admin_block_menu_construct(){
  $structure_menu = pdi_admin_menu_structure('organo_tecnico');
  $pdi_menu = pdi_admin_menu_pdi('eje_estrategico');
  $add_content_menu = pdi_admin_menu_content();

  $menu  = '<div id="pdi_admin_menu"><ul class="menu clean l1">';
  $menu .= '<li><span>Agregar Contenido</span>'.$add_content_menu.'</li>';
  $menu .= '<li><span>PDI</span>'.$pdi_menu.'</li></ul>';
  $menu .= '<li><span>Estructura</span>'.$structure_menu.'</li>';
  $menu .= '</ul></div>';

  return $menu;
}

// Create estructure menu.
function pdi_admin_menu_structure($term_name){
  $terms = pdi_admin_taxonomy_vocabulary_load($term_name);
  $structure = '<ul class="l2"><li class="name"><span>Órganos Técnicos</span></li><li><a class="first_link" href="/admin/structure/taxonomy/organo_tecnico/add">Agregar Órgano Técnico</a></li>';
  foreach ($terms as $key => $term) {
    $structure .= '<li><a href="/taxonomy/term/'.$term->tid.'">'.$term->name.'</a><ul>';

    $structure .= pdi_admin_menu_structure_2($term->tid);
    $structure .= '</ul></li>';
  }
  

  return $structure; 
}
// Create second leve structure menu
function pdi_admin_menu_structure_2($tid){

  $sql = sprintf("SELECT entity_id FROM field_data_field_organo_tecnico WHERE field_organo_tecnico_tid = %d AND bundle = 'dependencia'" , $tid);
  $results = db_query($sql);
  $structure_l2 = '<li class="name"><span>Dependencias</span></li><li><a class="first_link" href="/admin/structure/taxonomy/dependencia/add">Agregar Dependencia</a></li>';

  while(($row = $results->fetchAssoc())) {
    $term = taxonomy_term_load($row['entity_id']);
    $structure_l2 .= '<li><a href="/taxonomy/term/'.$row['entity_id'].'">'.$term->name.'</a><ul>';

    $structure_l2 .= pdi_admin_menu_structure_3($row['entity_id']);
    $structure_l2 .= '</ul></li>';
  } 

  return $structure_l2;
}

// Create third leve structure menu
function pdi_admin_menu_structure_3($tid){

  $sql = sprintf("SELECT entity_id FROM field_data_field_area_dependencia WHERE field_area_dependencia_tid = %d AND bundle = 'area'" , $tid);
  $results = db_query($sql);
  $structure_l3 = '<li class="name"><span>Areas</span></li><li><a class="first_link" href="/admin/structure/taxonomy/area/add">Agregar Area</a></li>';

  while(($row = $results->fetchAssoc())) {
    $term = taxonomy_term_load($row['entity_id']);
    $structure_l3 .= '<li><a href="/taxonomy/term/'.$row['entity_id'].'">'.$term->name.'</a></li>';
  } 

  return $structure_l3;
}

// Contruct  pdi menu. 
function pdi_admin_menu_pdi($term_name){
  // Ejes estrategicos
  $terms = pdi_admin_taxonomy_vocabulary_load($term_name);
  $structure = '<ul class="l2"><li class="name"><span>Ejes Estrategicos</span></li><li><a class="first_link" href="/admin/structure/taxonomy/eje_estrategico/add">Agregar eje Estrategico</a></li>';
  foreach ($terms as $key => $term) {
    $structure .= '<li><a href="/taxonomy/term/'.$term->tid.'">'.$term->name.'</a><ul>';

    $structure .= pdi_admin_menu_pdi_2($term->tid);
    $structure .= '</ul></li>';
  }
  
  return $structure; 
}

// Create second leve pdi menu
function pdi_admin_menu_pdi_2($tid){
  // Premisas
  $sql = sprintf("SELECT entity_id FROM field_data_field_premisa_eje WHERE field_premisa_eje_tid = %d AND bundle = 'premisa'" , $tid);
  $results = db_query($sql);
  $pdi_l2 = '<li class="name"><span>Premisas</span></li><li><a class="first_link" href="/admin/structure/taxonomy/premisa/add?axis='.$tid.'">Agregar Premisa</a></li>';
  //$pdi_l2 = '<li class="name"><span>Premisas</span></li><li><a class="first_link" href="/admin/structure/taxonomy/premisa/add">Agregar Premisa</a></li>';

  while(($row = $results->fetchAssoc())) {
    $term = taxonomy_term_load($row['entity_id']);
    $pdi_l2 .= '<li><a href="/taxonomy/term/'.$row['entity_id'].'">'.$term->name.'</a><ul>';

    $pdi_l2 .= pdi_admin_menu_pdi_3($row['entity_id']);
    $pdi_l2 .= '</ul></li>';
  } 

  return $pdi_l2;
}

// Create second leve structure menu
function pdi_admin_menu_pdi_3($tid){
  // Estrategias
  $sql = sprintf("SELECT entity_id FROM field_data_field__estrategia_premisa WHERE field__estrategia_premisa_tid = %d AND bundle = 'estrategia'" , $tid);
  $results = db_query($sql);
  $pdi_l3 = '<li class="name"><span>Estrategias</span></li><li><a class="first_link" href="/admin/structure/taxonomy/estrategia/add?premise='.$tid.'">Agregar Estrategia</a></li>';
  //$pdi_l3 = '<li class="name"><span>Estrategias</span></li><li><a class="first_link" href="/admin/structure/taxonomy/estrategia/add">Agregar Estrategia</a></li>';

  while(($row = $results->fetchAssoc())) {
    $term = taxonomy_term_load($row['entity_id']);
    $pdi_l3 .= '<li><a href="/taxonomy/term/'.$row['entity_id'].'">'.$term->name.'</a><ul>';

    $pdi_l3 .= pdi_admin_menu_pdi_4($row['entity_id']);
    $pdi_l3 .= '</ul></li>';
  } 

  return $pdi_l3;
}

// Create second leve structure menu
function pdi_admin_menu_pdi_4($tid){
  // Lineas de accion
  //$tid = 33;
  $sql = sprintf("SELECT entity_id FROM field_revision_field_estrategia WHERE field_estrategia_tid = %d AND bundle = 'action_line'" , $tid);
  $results = db_query($sql);

  $pdi_l4 = '<li class="name"><span>Lineas de Acción</span></li><li><a class="first_link" href="/node/add/actividad">Agregar Linea de Accion </a></li>';

  while(($row = $results->fetchAssoc())) {
    $node = node_load($row['entity_id']);
    $pdi_l4 .= '<li><a href="/node/'.$row['entity_id'].'">'.$node->title.'</a></li>';
  } 

  return $pdi_l4;
}

// Create content menu.
function pdi_admin_menu_content(){
  $content  = '<ul class="l2">';
  $content .= '<li><a href="/node/add/action-line">Agregar Linea de Acción</a></li>';
  $content .= '<li><a href="/node/add/actividad">Agregar Actividad</a></li>';
  $content .= '</ul>';

  return $content;
}

// Contruct structure.
function pdi_admin_taxonomy_vocabulary_load($taxonomi_name){
  $vocabulary = taxonomy_vocabulary_machine_name_load($taxonomi_name);
  $terms = entity_load('taxonomy_term', FALSE, array('vid' => $vocabulary->vid));

  return $terms;
}

