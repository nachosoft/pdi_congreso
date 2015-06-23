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
  $actions = array();

  while(($row = $results->fetchAssoc())) {
    $actions[$row['entity_id']] = $row['entity_id'];
  } 
  $output = array(
    'progress' => 0, 
  );

  $nactions = 100/count($actions);

  foreach ($actions as $key => $action) {
    $activity = node_load($action); 
    if ($activity->field_activity_finished[LANGUAGE_NONE][0]['value'] == 1) {
      $output['progress'] = $output['progress'] + $nactions;
    }
    $output[$action]['status'] = $activity->field_activity_finished[LANGUAGE_NONE][0]['value'];
    $output[$action]['area'] = $activity->field_area[LANGUAGE_NONE][0]['tid'];
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
 * Get progress widget of the user area or global progress
 * @Params $uid -> int
 * @Return $progress -> html
 **/
function pdi_lines_get_progress($uid){
  $duser = user_load($uid);

  $duser_roles = $duser->roles;

  $progress = null;

  if (in_array('administrator', $duser_roles)) {
    $uarea = $duser->field_organo_tecnico[LANGUAGE_NONE][0]['tid'];
    $term = taxonomy_term_load($uarea);
    $progress = create_progress_widget($term->field_progreso_p[LANGUAGE_NONE][0]['value'], $term->name);

    return $progress;

  } elseif (in_array('Titular', $duser_roles) or in_array('Operador', $duser_roles)) {
    $uarea = $duser->field_area[LANGUAGE_NONE][0]['tid'];
    $term = taxonomy_term_load($uarea);
    

    $progress = create_progress_widget($term->field_organo_tecnico[LANGUAGE_NONE][0]['value'], $term->name);

    return $progress;
  }

  return $progress;
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

  $progress_widget .= '<progress style="display: none" id="progress_bar" value="'  . $percent . '" max="'. $percent .'"></progress>';

  $progress_widget .= '</div></div>';

  $progress_widget .= '<div class="card_footer"><p>'. $name .'</p></div>';

  drupal_add_js(drupal_get_path('module', 'pdi_lines') . '/js/pdi_lines-progress.bar.js');

  drupal_add_css(drupal_get_path('module', 'pdi_lines') . '/css/circle_chart.css');

  return $progress_widget;
}

/**
 * Get the users per area.
 * @Params $uid -> int
 * @Return $users -> html
 **/
function pdi_lines_get_users($uid){
  $duser = user_load($uid);

  $duser_roles = $duser->roles;

  $users = null;

  if (in_array('administrator', $duser_roles)) {
    $tid = $duser->field_area[LANGUAGE_NONE][0]['tid'];

    $users = create_users_list($tid);

  } elseif (in_array('Titular', $duser_roles) or in_array('Operador', $duser_roles)) {
    $tid = $duser->field_area[LANGUAGE_NONE][0]['tid'];

    $users = create_users_list($tid);

  }

  return $users;
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
 * Get the premises per area.
 * @Params $uid -> int
 * @Return $premises -> html
 **/
function pdi_lines_get_premises($uid){
  $duser = user_load($uid);
  $otid = $duser->field_organo_tecnico[LANGUAGE_NONE][0]['tid'];
  $term = taxonomy_term_load($otid);

  $sql = sprintf("SELECT entity_id FROM field_data_field_premisa_area WHERE field_premisa_area_tid = %d AND bundle = 'premisa'", $tid);
  $results = db_query($sql);
  $tids = array();

  while(($row = $results->fetchAssoc())) {
    $tids[] = $row['entity_id'];
  } 

  $premises = create_premises_list($tids);

  return $premises;

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
 * Get actios per user.
 * @Params $uid -> int
 * @Return $action -> html
 **/
function pdi_lines_get_actions($uid){
  $duser = user_load($uid);

  $actions = NULL;
  if (!in_array('Operador', $duser->roles)) {
    $search = module_invoke('search', 'block_view');
    $search_box = render($search);
    $actions = '<div class="actions">';
    $actions .= '<div class="search_box">'.$search_box.'</div>';
    $actions .= '<div class="btn"><a href="/node/add/actividad">Nueva Actividad?</a></div>';
    $actions .= '<div class="btn"><a href="/pdi/activity/list">Lista de Actividades</a></div>';
    $actions .= '</div>';
  } else {
    $search = module_invoke('search', 'block_view');
    $search_box = render($search);
    $actions = '<div class="actions">';
    $actions .= '<div class="search_box">'.$search_box.'</div>';
    $actions .= '<div class="btn"><a href="/pdi/activity/list">Lista de Actividades</a></div>';
    $actions .= '</div>';
  }

  return $actions;
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