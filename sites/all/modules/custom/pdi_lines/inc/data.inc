<?php
/**
 * Get the actions and percentages per line.
 * @Params: $node -> integer
 * @Return: $output -> array( aid => integer, percentage => integer, status => boolean )
 **/
function get_action_per_line($nid){
  $node = node_load($nid);
  $actions = $node->field_topic[LANGUAGE_NONE];
  $output = array();
  foreach ($actions as &$action) {
    $line_loaded = field_collection_item_load($action['value']);
    $output[$action['value']]['aid'] = $action['value'];
    $output[$action['value']]['percentage'] = $line_loaded->field_percentage[LANGUAGE_NONE][0]['value'];
    $output[$action['value']]['status'] = $line_loaded->field_finished[LANGUAGE_NONE][0]['value'];
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
 * Get progress widget of the user area or global progress
 * @Params $uid -> int
 * @Return $progress -> html
 **/
function pdi_lines_get_progress($uid){
  $duser = user_load($uid);

  $duser_roles = $duser->roles;

  $progress = null;

  if (in_array('administrator', $duser_roles)) {
    $uarea = $duser->field_area[LANGUAGE_NONE][0]['tid'];
    $term = taxonomy_term_load($uarea);

    $progress = create_progress_widget($term->field_progress_area[LANGUAGE_NONE][0]['value'], $term->name);

    return $progress;

  } elseif (in_array('Titular', $duser_roles) or in_array('Operador', $duser_roles)) {
    $uarea = $duser->field_area[LANGUAGE_NONE][0]['tid'];
    $term = taxonomy_term_load($uarea);

    $progress = create_progress_widget($term->field_progress_area[LANGUAGE_NONE][0]['value'], $term->name);

    return $progress;
  }

  return $progress;
}

/**
 * Create progress widget  
 **/
function create_progress_widget($percent, $tname){

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

  $progress_widget .= '<div class="card_footer"><p>'. $tname .'</p></div>';

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
  $tid = $duser->field_area[LANGUAGE_NONE][0]['tid'];

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
    $actions .= '<div class="btn"><a href="/node/add/action-line">Nueva Linea de Accíon?</a></div>';
    $actions .= '<div class="btn"><a href="/pdi/lines/list">Lista de Lineas de Accíones</a></div>';
    $actions .= '</div>';
  } else {
    $search = module_invoke('search', 'block_view');
    $search_box = render($search);
    $actions = '<div class="actions">';
    $actions .= '<div class="search_box">'.$search_box.'</div>';
    $actions .= '<div class="btn"><a href="/pdi/lines/list">Lista de Lineas de Accíones</a></div>';
    $actions .= '</div>';
  }

  return $actions;
}
