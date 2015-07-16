<?php
/**
 * Get the actions and percentages per line.
 * @Params: $node -> integer
 * @Return: $output -> array( nid => integer, percentage => integer, status => boolean )
 **/
function get_action_per_line($nid){
  $node = node_load($nid);

  $sql = sprintf("SELECT entity_id FROM field_data_field_action_line WHERE field_action_line_target_id = %d AND bundle = 'actividad'" , $nid);
  $results = db_query($sql);
  $actions = array(
    'n_elements' => 0, 
    'n_percentage' => 100, 
  );

  while(($row = $results->fetchAssoc())) {
    $node = node_load($row['entity_id']); 
    $actions['activities'][$row['entity_id']]['nid'] = $row['entity_id'];
    $actions['activities'][$row['entity_id']]['title'] = $node->title;
    $actions['activities'][$row['entity_id']]['finish'] = $node->field_activity_finished[LANGUAGE_NONE][0]['value'];
    if (isset($node->field_porcentaje_de_actividad[LANGUAGE_NONE][0]['value'])){
      if ($node->field_porcentaje_de_actividad[LANGUAGE_NONE][0]['value'] > 0 ) {
        $actions['n_percentage'] = $actions['n_percentage'] - $node->field_porcentaje_de_actividad[LANGUAGE_NONE][0]['value'];
      }
      $actions['activities'][$row['entity_id']]['percent'] = $node->field_porcentaje_de_actividad[LANGUAGE_NONE][0]['value'];
    } else {
      $actions['activities'][$row['entity_id']]['percent'] = 0;
      $actions['n_elements'] = $actions['n_elements'] + 1;
    }
  } 

  if ($actions['n_percentage'] <= 0) {
    $message = 'El porcentaje de los elementos ingresados es mayor al 100%  >>>>>> ';

    foreach ($actions['activities'] as $key => $activity) {
      $message .= '['.$activity['title'] . ' -> ' .$activity['percent'].'%  ]  ';
    }
    
    drupal_set_message(t($message), 'error', FALSE);
    return; 
  }

  $output = array(
    'progress' => 0, 
  );

  if ($actions['n_elements'] > 0) {
    $nactions = $actions['n_percentage']/$actions['n_elements'];
  } else {
    $nactions = 0;
  }
  

  foreach ($actions['activities'] as $key => $activity) {
    $node = node_load($activity['nid']);
    if ($activity['finish'] == 1) {
      if ($activity['percent'] > 0){
        $output['progress'] = $output['progress'] + $activity['percent'];
      }else{
        $output['progress'] = $output['progress'] + $nactions;
      } 
    }
    $output[$activity['nid']]['status'] = $node->field_activity_finished[LANGUAGE_NONE][0]['value'];
    $output[$activity['nid']]['area'] = $node->field_area[LANGUAGE_NONE][0]['tid'];
  }

  return $output;
}

/**
 * Get the nodes of an strategy and return the progress of the strategy.
 * @Params $tnodes -> array([] => nid)
 * @Return $tprogress -> int 
 **/
function pdi_strategy_menu_calcule_progress($tnodes){
  $nprogress = array(
    'progress' => 0, 
  );
  // the percentage that represents each node on decimal notation(50% -> 0.5).
  $tnode_percentage_decimal = 1 / count($tnodes); 

  foreach ($tnodes as $key => $nid) {
    $node = node_load($nid);
    $nprogress[$nid]['nid'] = $nid;
    $nprogress[$nid]['real_percentage'] = $node->field_percentage_action[LANGUAGE_NONE][0]['value'] * $tnode_percentage_decimal;
    $nprogress['progress'] = $nprogress['progress'] + $nprogress[$nid]['real_percentage'] ;
  }

  return $nprogress['progress'];
}

/**
 * Get the tid of a premise.
 * @Params $tid -> int
 * @Return $tnodes -> array([] => nid)
 **/
function pdi_premise_get_nodes($tid){

  $sql = sprintf("SELECT entity_id FROM field_revision_field__estrategia_premisa WHERE field__estrategia_premisa_tid = %d", $tid);
  $results = db_query($sql);
  $tids = array();

  while(($row = $results->fetchAssoc())) {
    $tids[$row['entity_id']] = $row['entity_id'];
  } 

  return $tids;
}

/**
 * Get the list of term ids of a premise and returns the progress percentage.
 * @Params $tids -> array( nid => nid)
 * @Return $tprogress -> int
 **/
function pdi_premise_menu_calcule_progress($tids){
  $tids_percentage = array(
    'progress' => 0, 
  );

  $titem_percentage_decimal = 1 / count($tids_percentage); 

  foreach ($tids as $key => $tid) {
    $term_loaded = taxonomy_term_load($tid);
    $tids_percentage[$tid] = $term_loaded->field_strategy_progress[LANGUAGE_NONE][0]['value'] * $titem_percentage_decimal;
    $tids_percentage['progress'] = $tids_percentage['progress'] + $tids_percentage[$tid];
  }  

  return $tids_percentage['progress'];
}

/**
 * Get the tid of an area.
 * @Params $tid -> int
 * @Return $tnodes -> array([] => nid)
 **/
function pdi_area_get_nodes($tid){

  $sql = sprintf("SELECT entity_id FROM field_data_field_premisa_area WHERE field_premisa_area_tid = %d AND bundle = 'premisa'", $tid);
  $results = db_query($sql);
  $tids = array();

  while(($row = $results->fetchAssoc())) {
    $tids[$row['entity_id']] = $row['entity_id'];
  } 

  return $tids;
}


/**
 * Create progress widget  
 **/
function create_progress_widget($percent, $name){

  $progress_widget = '<h2 class="card_title">Progreso</h2>';
  $progress_widget .= '<div class="bar_container">
    <div id="main_container">
    <div id="pbar" class="progress-pie-chart" data-percent="0">
    <div class="ppc-progress">
    <div class="ppc-progress-fill"></div>
    </div>
    <div class="ppc-percents">
    <div class="pcc-percents-wrapper">
    <span>%</span>
    </div>
    </div>
    </div>';

  $progress_widget .= '<progress style="display: none" id="progress_bar" value="'  . intval($percent) . '" max="'. intval($percent) .'"></progress>';

  $progress_widget .= '</div></div>';

  $progress_widget .= '<div class="card_footer"><p>'. $name .'</p></div>';

  drupal_add_js(drupal_get_path('module', 'pdi_lines') . '/js/pdi_lines-progress.bar.js');

  drupal_add_css(drupal_get_path('module', 'pdi_lines') . '/css/circle_chart.css');

  return $progress_widget;
}

/**
 * Create a list of the users in the area.
 **/
function create_users_list($tid){

  $sql = sprintf("SELECT entity_id FROM field_data_field_area WHERE field_area_tid = %d AND bundle = 'user'", $tid);
  $results = db_query($sql);
  $uids = array();

  while(($row = $results->fetchAssoc())) {
    $uids[] = $row['entity_id'];
  } 

  $users = '<h2 class="card_title">Miembros del Area</h2>';
  $users .= '<div class="users_list">';
  foreach ($uids as $key => $uid) {
    $user = user_load($uid);
    if (in_array('Titular', $user->roles)) {
      $users .= '<div class="user_item"><h3>'.$user->name.'</h3><p class="user_mail">'.$user->mail.'</p><p class="user_role">Titular</p></div>';
    } elseif (in_array('Operador', $user->roles)){
      $users .= '<div class="user_item"><h3>'.$user->name.'</h3><p class="user_mail">'.$user->mail.'</p><p class="user_role">Operador</p></div>';
    }
  }

  $users .= '</div>';

  return $users;
}

/**
 * Create a list of the premises in the area.
 **/
function create_premises_list($tids){

  $premises = '<h2 class="card_title">Premisas del Area</h2>';
  $premises .= '<div class="strategies_list">'; 

  foreach ($tids as $key => $tid) {
    $term = taxonomy_term_load($tid);

    if (isset($term->field_premisa_progreso[LANGUAGE_NONE][0]['value'])) {
      $premises .= '<div class="strategy_item double"><p class="name">'.$term->name.'</p><p class="progress">'.intval($term->field_premisa_progreso[LANGUAGE_NONE][0]['value']).'%</p></div>';
    }
    $premises .= '<div class="strategy_item single"><p class="name">'.$term->name.'</p></div>';
  }

  $premises .= '</div>';

  return $premises;
  
}

/**
 * Get the strategies per user
 * @Params $uid -> int
 * @Return $user_strategy -> array( [tid] => tname )
 **/
function pdi_lines_get_user_strategies($uid){
  $duser = user_load($uid);

  $uarea = $duser->field_area[LANGUAGE_NONE][0]['tid']; 

  $premises = pdi_line_get_area_premises($uarea);

  $strategies = array(
     '_none' => '- Seleccione un valor -', 
  );
  foreach ($premises as $key => $premise) {
    $strategy_list = pdi_line_get_premise_strategies($premise);
    foreach ($strategy_list as $tid=> $tname) {
      $strategies[$tid] = $tname;
    }
  }
  
  return $strategies;
}

/**
 * Get the premises per area
 * @Params $tid -> int
 * @Return $premises -> array( [] => tid )
 **/ 
function pdi_line_get_area_premises($tid){
  $sql = sprintf("SELECT entity_id FROM field_data_field_premisa_area WHERE field_premisa_area_tid = %d AND bundle = 'premisa'", $tid);
  $results = db_query($sql);
  $premises = array();

  while(($row = $results->fetchAssoc())) {
    $premises[] = $row['entity_id'];
  } 

  return $premises;
}

/**
 * Get the strategies per premise
 * @Params $tid -> int
 * @Return $strategies -> array( [] => tid )
 **/ 
function pdi_line_get_premise_strategies($tid){
  $sql = sprintf("SELECT entity_id FROM field_revision_field__estrategia_premisa WHERE field__estrategia_premisa_tid = %d AND bundle = 'estrategia'", $tid);
  $results = db_query($sql);
  $strategies = array();

  while(($row = $results->fetchAssoc())) {
    $term = taxonomy_term_load($row['entity_id']);
    $strategies[$row['entity_id']] = $term->name;
  } 

  return $strategies;
}

/**
 * Calcule the progress of the area.
 * @Params $nid -> int
 * @Return $status -> boolean
 **/
function pdi_lines_progress_calcule($nid){
  if (!isset($nid)){
    return false;
  }
  $node = node_load($nid);
  // Calcule the line progress
  pdi_lines_calcule_progress($nid);

  return true;
}

/**
 * Get user  structure 
 * @Params $uid -> int
 * @Return $structure -> array();
 **/
function pdi_lines_get_structure($uid){
  if (is_int($uid)) {
    return false;
  }

  $structure = array();

  $duser = user_load($uid);

  $structure['organo_tecnico']['tid'] = $duser->field_organo_tecnico[LANGUAGE_NONE][0]['tid'];

  $structure['dependencia']['tid'] = $duser->field_dependencia[LANGUAGE_NONE][0]['tid'];

  // Aqui se hace la magia.
  $structure['area']['tid'] = $duser->field_area[LANGUAGE_NONE][0]['tid'];

  return $structure;
}

/**
 * Return the nodes of the of that ot.
 * @Params $tid -> int;
 * @Return $activities -> array();
 **/

function pdi_lines_get_nodes_ot($tid){
  $sql = sprintf("SELECT entity_id FROM field_data_field_organo_tecnico WHERE field_organo_tecnico_tid = %d AND bundle = 'actividad'", $tid);
  $results = db_query($sql);
  $nodes = array();

  while(($row = $results->fetchAssoc())) {
    $nodes[$row['entity_id']]['nid'] = $row['entity_id'];
  } 

  $activities = array();
  foreach ($nodes as $key => $node) {
    $loaded_node = node_load($node['nid']);
    if (isset($loaded_node->title)) {
      $node['nombre'] = $loaded_node->title;
    }
    if (isset($loaded_node->field_action_line[LANGUAGE_NONE][0]['target_id'])) {
      $node['linea'] = $loaded_node->field_action_line[LANGUAGE_NONE][0]['target_id'];
    }
    if (isset($loaded_node->field_eje_estrategico[LANGUAGE_NONE][0]['tid'])) {
      $node['eje_estrategico'] = $loaded_node->field_eje_estrategico[LANGUAGE_NONE][0]['tid'];
    }
    if (isset($loaded_node->field_premisa[LANGUAGE_NONE][0]['tid'])) {
      $node['premisa'] = $loaded_node->field_premisa[LANGUAGE_NONE][0]['tid'];
    }
    if (isset($loaded_node->field_estrategia[LANGUAGE_NONE][0]['tid'])) {
      $node['estrategia'] = $loaded_node->field_estrategia[LANGUAGE_NONE][0]['tid'];
    }

    $activities[$node['nid']] = $node;
  }

  return $activities;
}

/**
 * Create the data accordions for the homepage
 * @Params $activities -> array();
 * @Returns $content -> html;
 **/
function pdi_lines_create_content($activities){

  $strategic_axis = pdi_lines_clean_stratetegic_axis($activities);

  $premises = pdi_lines_clean_premises($activities);

  $strategy = pdi_lines_clean_strategy($activities);

  $lines = pdi_lines_clean_lines($activities);

  $structure = pdi_lines_create_structure($activities, $strategic_axis, $premises, $strategy, $lines);

  $content = pdi_lines_create_home_page($structure);

  return $content;

}

// Get all the stratecig axis and clean the result 
function pdi_lines_clean_stratetegic_axis($activities){

  $strategic_axis = array();

  foreach ($activities as $key => $activity) {
    if (isset($activity['eje_estrategico'])){
      $strategic_axis[$activity['eje_estrategico']] = $activity['eje_estrategico'];
    }
  }

  return $strategic_axis;
}

// Clean all the premises 
function pdi_lines_clean_premises($activities){

  $premises = array();
  foreach ($activities as $key => $activity) {
    if (isset($activity['premisa'])){
      $term = taxonomy_term_load($activity['premisa']);
      $premises[$activity['premisa']]['tid'] = $activity['premisa'];
      if ($term->field_premisa_eje[LANGUAGE_NONE][0]['tid'] > 0){
        $premises[$activity['premisa']]['eje'] = $term->field_premisa_eje[LANGUAGE_NONE][0]['tid'];
      }
    }
  }

  return $premises;
}

// Clean all the strategies 
function pdi_lines_clean_strategy($activities){
  $strategies = array();

  foreach ($activities as $key => $activity) {
    if (isset($activity['estrategia'])){
      $term = taxonomy_term_load($activity['estrategia']);
      $strategies[$activity['estrategia']]['tid'] = $activity['estrategia'];
      if ($term->field__estrategia_premisa[LANGUAGE_NONE][0]['tid'] > 0){
        $strategies[$activity['estrategia']]['premisa'] = $term->field__estrategia_premisa[LANGUAGE_NONE][0]['tid'];
      }
    }
  }

  return $strategies;
}

// Clean all actions lines
function pdi_lines_clean_lines($activities){
  $lines = array();

  foreach ($activities as $key => $activity) {
    if ($activity['linea'] > 0){
      $node = node_load($activity['linea']);
      //print_r($node); exit;
      $lines[$activity['linea']]['nid'] = $activity['linea'];
      if (isset($node->field_percentage_action[LANGUAGE_NONE][0]['value'])){
        $lines[$activity['linea']]['progreso'] = $node->field_percentage_action[LANGUAGE_NONE][0]['value'];
      }
      if (isset($node->field_eje_estrategico[LANGUAGE_NONE][0]['tid'])) {
        $lines[$activity['linea']]['eje'] = $node->field_eje_estrategico[LANGUAGE_NONE][0]['tid'];
      }
      if (isset($node->field_premisa[LANGUAGE_NONE][0]['tid'])) {
        $lines[$activity['linea']]['premisa'] = $node->field_premisa[LANGUAGE_NONE][0]['tid'];
      }
      if (isset($node->field_estrategia[LANGUAGE_NONE][0]['tid'])) {
        $lines[$activity['linea']]['estrategia'] = $node->field_estrategia[LANGUAGE_NONE][0]['tid'];
      }
    }
  }

  return $lines;
}

// Create structure of the data; 
function pdi_lines_create_structure($activities, $strategic_axis, $premises, $strategy, $lines){
  $structure = array();

  // set first and second level   
  foreach ($strategic_axis as $key => $axi) {
    $structure[$axi]['tid'] = $axi;

    // inception time!!
    foreach ($premises as $key => $premise) {
      if ($axi == $premise['eje']){
        $structure[$axi]['premisas'][$premise['tid']]['tid'] = $premise['tid'];
      }
    }
  }

  // Set  third level
  foreach ($structure as $key => $struct) {
    foreach ($struct['premisas'] as $key => $premisa) {
      foreach ($strategy as $key => $strat) {
        if ($strat['premisa'] == $premisa['tid']){
          $structure[$struct['tid']]['premisas'][$premisa['tid']]['estrategias'][$strat['tid']]['tid'] = $strat['tid'];
        }
      }
    }
  }

  // Set fourth level 
  foreach ($structure as $key => $struct) {
    foreach ($struct['premisas'] as $key => $premisa) {
      foreach ($premisa['estrategias'] as $key => $strat) {
        foreach ($lines as $key => $line) {
          if ($line['estrategia'] == $strat['tid']) {
            $structure[$struct['tid']]['premisas'][$premisa['tid']]['estrategias'][$strat['tid']]['lineas'][$line['nid']]['nid'] = $line['nid'];
            $structure[$struct['tid']]['premisas'][$premisa['tid']]['estrategias'][$strat['tid']]['lineas'][$line['nid']]['progreso'] = $line['progreso'];
          }
        }
      }
    }
  }

  //print_r($activities); exit; 
  // Set fifth level 
  foreach ($structure as $key => $struct) {
    foreach ($struct['premisas'] as $key => $premisa) {
      foreach ($premisa['estrategias'] as $key => $strat) {
        foreach ($strat['lineas'] as $key => $line) {
          foreach ($activities as $key => $activity) {
            if ($activity['linea'] == $line['nid']){
              $structure[$struct['tid']]['premisas'][$premisa['tid']]['estrategias'][$strat['tid']]['lineas'][$line['nid']]['actividades'][$activity['nid']]['nid'] = $activity['nid'];
              $structure[$struct['tid']]['premisas'][$premisa['tid']]['estrategias'][$strat['tid']]['lineas'][$line['nid']]['actividades'][$activity['nid']]['nombre'] = $activity['nombre'];
            }
          }
        }
      }
    }
  }

  return $structure;
}

/**
 * Returns page with the lines data.
 **/

function pdi_lines_create_home_page($structure){
  if (!isset($structure)) {
    return false;
  }

  $content = '<div class="accordions level_1" data-level="1"><ul data-level="1">';
  foreach ($structure as $key => $axi) {
    $term = taxonomy_term_load($axi['tid']);
    $content .= '<li class="accordion accordion-1" data-level="1">';
    $content .= '<h2><span>Eje Estrategico: </span>'.$term->name.'</h2>'; 
    if ($term->field_progreso_p[LANGUAGE_NONE][0]['value']) {
      $content .= '<div class="progress">'.$term->field_progreso_p[LANGUAGE_NONE][0]['value'].'</div>'; 
    }

    $content .= '<ul class="l-2" data-level="2">';
    foreach ($axi['premisas'] as $key => $premisa) {
      $level_2 = pdi_lines_create_home_page_level_2($premisa);
      $content .= $level_2;
    }

    $content .= '</ul>';
    $content .= '</li>';
  }
  $content .='</div>';

  return $content;
}

// Return level 2 of homepage
function pdi_lines_create_home_page_level_2($premisa){
  $term = taxonomy_term_load($premisa['tid']);
  $content_l2 = ''; 
  $content_l2 .= '<li class="accordion accordion-2" data-level="2">';
  $content_l2 .= '<h2><span>Premisa: </span>'.$term->name.'</h2>';
  if ($term->field_progreso[LANGUAGE_NONE][0]['value']) {
    $content_l2 .= '<div class="progress">'.$term->field_progreso[LANGUAGE_NONE][0]['value'].'</div>'; 
  }

  $content_l2 .= '<ul class="l-3" data-level="3">';
  foreach ($premisa['estrategias'] as $key => $estrategia) {
    $level_3 = pdi_lines_create_home_page_level_3($estrategia);
    $content_l2 .= $level_3;
  }
  $content_l2 .= '</ul';
  $content_l2 .= '</li>';
  
  return $content_l2;
}

// Return level 3 of homepage
function pdi_lines_create_home_page_level_3($estrategia){
  $term = taxonomy_term_load($estrategia['tid']);
  $content_l3 = ''; 
  $content_l3 .= '<li class="accordion accordion-3" data-level="3">';
  $content_l3 .= '<h2><span>Estrategia: </span>'.$term->name.'</h2>';
  if ($term->field_progreso_p[LANGUAGE_NONE][0]['value']) {
    $content_l3 .= '<div class="progress">'.$term->field_progreso_p[LANGUAGE_NONE][0]['value'].'</div>'; 
  }

  $content_l3 .= '<ul class="l-4" data-level="4">';
  foreach ($estrategia['lineas'] as $key => $linea) {
    $level_4 = pdi_lines_create_home_page_level_4($linea);
    $content_l3 .= $level_4;
  }

  $content_l3 .= '</ul>';

  $content_l3 .='</li>';

  return $content_l3;
}

// Return level 4 of homepage
function pdi_lines_create_home_page_level_4($linea){
  $node = node_load($linea['nid']);
  $content_l4 = ''; 
  $content_l4 .= '<li class="accordion accordion-4" data-level="4">';
  $content_l4 .= '<h2><span>Linea de Accion: </span>'.$node->title.'</h2>';
  if ($node->field_percentage_action[LANGUAGE_NONE][0]['value']) {
    $content_l4 .= '<div class="progress">'.$node->field_percentage_action[LANGUAGE_NONE][0]['value'].'</div>'; 
  }

  foreach ($linea['actividades'] as $key => $actividad) {
    $level_5 = pdi_lines_create_home_page_level_5($actividad);
    $content_l4 .= $level_5;
  }

  $content_l4 .= '</li>';

  return $content_l4;
}

// Return level 5 of homepage 
function pdi_lines_create_home_page_level_5($actividad){
  $node = node_load($actividad['nid']);
  
  $content_l5 = '<div class="card activity">';
  if ($node->field_activity_finished[LANGUAGE_NONE][0]['value'] == 0){
    $content_l5 .= '<div class="edit"><a href="node/'.$actividad['nid'].'/edit">Editar</a></div>';
  }else if($node->field_activity_finished[LANGUAGE_NONE][0]['value'] == 1){
    $content_l5 .= '<div class="finished"><label>Terminada</label></div>';
  }
  $content_l5 .= '<h2 class="title">'.$node->title.'</h2>';
  if (isset($node->field_activity_comments[LANGUAGE_NONE][0]['value'])){
    $content_l5 .= '<div class="comments"><p>'.$node->field_activity_comments[LANGUAGE_NONE][0]['value'].'</p></div>';
  }

  $content_l5 .= '</div>';

  return $content_l5;
}

