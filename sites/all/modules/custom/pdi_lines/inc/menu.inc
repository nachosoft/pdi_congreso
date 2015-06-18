<?php
/**
 * Implements menu_hook.
 **/
function pdi_lines_menu(){
  $items['pdi/line/calcule/%'] = array(
    'title' => 'Calcular progreso linea de accion', 
    'page callback' => 'pdi_lines_menu_calcule', 
    'page arguments' => array(3), 
    'access callback' => 'user_can_calcule_progress', 
    'type' => MENU_CALLBACK, 
  );
  $items['pdi/strategy/calcule/%'] = array(
    'title' => 'Calcular progreso estrategia', 
    'page callback' => 'pdi_strategy_menu_calcule', 
    'page arguments' => array(3), 
    'access callback' => 'user_can_calcule_progress', 
    'type' => MENU_CALLBACK, 
  );
  $items['pdi/premise/calcule/%'] = array(
    'title' => 'Calcular progreso premisa', 
    'page callback' => 'pdi_premise_menu_calcule', 
    'page arguments' => array(3), 
    'access callback' => 'user_can_calcule_progress', 
    'type' => MENU_CALLBACK, 
  );
  $items['pdi/home'] = array(
    'title' => 'Resumen de Progreso', 
    'page callback' => 'pdi_premise_menu_dashboard', 
    'access callback' => 'user_can_see_dashboard', 
    'type' => MENU_CALLBACK, 
  );
  return $items;  
}

/**
 * Calcule the percentaje of each line.
 **/
function pdi_lines_menu_calcule($nid){
  // Get Actions per line 
  pdi_lines_calcule_progress($nid);

  // Add message.
  drupal_set_message(t('El progreso de la linea de accion ha sido actualizado'), 'pdi_message');

  // Go to the node
  drupal_goto('node/'.$nid);

}

/**
 * Calcule the progress of the line
 * @Params $nid -> int
 * @Return nothing
 **/ 
function pdi_lines_calcule_progress($nid){
  $actions = get_action_per_line($nid);
  $node = node_load($nid);
  
  // Set the new progress.
  $node->field_percentage_action[LANGUAGE_NONE][0]['value'] = $actions['progress'];
  node_save($node);

  return true;
}

/**
 * Calcule the percentage of the strategy progress.
 **/ 
function pdi_strategy_menu_calcule($tid){
  // Get the nodes of the therm.
  $tnodes = taxonomy_select_nodes($tid); 

  // Calculate the progress.
  $tprogress = pdi_strategy_menu_calcule_progress($tnodes);

  // Get the term object
  $term = taxonomy_term_load($tid);

  // Set the new progress.
  $term->field_strategy_progress[LANGUAGE_NONE][0]['value'] = $tprogress;

  // Save the term.
  taxonomy_term_save($term);

  // Add message.
  drupal_set_message(t('El progreso de la estrategia ha sido actualizado'), 'pdi_message');
  // Got to the term page
  drupal_goto('taxonomy/term/' . $tid);
}

/**
 * Calcule the percentage of the strategy progress.
 * no redirectios :)
 **/ 
function pdi_strategy_calcule_progress($tid){
  // Get the nodes of the therm.
  $tnodes = taxonomy_select_nodes($tid); 

  // Calculate the progress.
  $tprogress = pdi_strategy_menu_calcule_progress($tnodes);

  // Get the term object
  $term = taxonomy_term_load($tid);

  // Set the new progress.
  $term->field_strategy_progress[LANGUAGE_NONE][0]['value'] = $tprogress;

  // Save the term.
  taxonomy_term_save($term);

  return;
}

/**
 * Calculate the percentage of the premise progress.
 **/
function pdi_premise_menu_calcule($tid){

  // Get the terms of the term.
  $tids = pdi_premise_get_nodes($tid); 

  // Calcule the percentage progress of the premise.
  $tprogress = pdi_premise_menu_calcule_progress($tids);

  // Load the premise tid.
  $term = taxonomy_term_load($tid);

  // Set new progress.
  $term->field_premisa_progreso[LANGUAGE_NONE][0]['value'] = $tprogress;

  // Save the term.
  taxonomy_term_save($term);

  // Add message.
  drupal_set_message(t('El progreso de la premisa ha sido actualizado'), 'pdi_message');

  // Got to the term page
  drupal_goto('taxonomy/term/' . $tid);
}
/**
 * Calculate the percentage of the premise progress.
 * No redirects :)
 **/
function pdi_premise_calcule_progress($tid){
  // Get the terms of the term.
  $tids = pdi_premise_get_nodes($tid); 

  // Calcule the percentage progress of the premise.
  $tprogress = pdi_premise_menu_calcule_progress($tids);

  // Load the premise tid.
  $term = taxonomy_term_load($tid);

  // Set new progress.
  $term->field_premisa_progreso[LANGUAGE_NONE][0]['value'] = $tprogress;

  // Save the term.
  taxonomy_term_save($term);

  return;
}

/**
 * Calculate the percentage of the area progress.
 * No redirects :)
 **/
function pdi_area_calcule_progress($tid){
  // Get the terms of the term.
  $tids = pdi_area_get_nodes($tid); 

  // Calcule the percentage progress of the area.
  $tprogress = pdi_premise_menu_calcule_progress($tids);

  // Load the area tid.
  $term = taxonomy_term_load($tid);

  // Set new progress.
  $term->field_premisa_progreso[LANGUAGE_NONE][0]['value'] = $tprogress;

  // Save the term.
  taxonomy_term_save($term);

  return;
}

/**
 * Create homepage to see the resume of the information.
 **/
function pdi_premise_menu_dashboard(){
  global $user;

  $progress = pdi_lines_get_progress($user->uid);

  $users = pdi_lines_get_users($user->uid);

  $premises = pdi_lines_get_premises($user->uid);

  $actions = pdi_lines_get_actions($user->uid);

  $vars = array(
    'progress' => $progress, 
    'actions' => $actions, 
    'users' => $users, 
    'premises' => $premises, 
  );

  $output = theme('pdi_lines_dashboard', $vars);
  return $output;
}
