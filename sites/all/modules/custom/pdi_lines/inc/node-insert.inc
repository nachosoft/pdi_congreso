<?php
/**
 * Implements hook_node_presave.
 */
function pdi_lines_node_presave($node) {
  switch ($node->type) {
    case 'actividad':
      $duser = user_load($node->uid);

      // Assign ot to the activity.
      $uot = $duser->field_organo_tecnico[LANGUAGE_NONE][0]['tid'];
      $node->field_organo_tecnico[LANGUAGE_NONE][0]['tid'] = $uot;

      // Set dependecia 
      $udp = $duser->field_dependencia;
      $node->field_dependencia = $udp;

      // Set area.
      $uarea = $duser->field_area;
      $node->field_area = $uarea;


      // Action Line. 
      $action_line = $node->field_action_line[LANGUAGE_NONE][0]['target_id'];
      $node_al = node_load($action_line);

      // Estrategia 

      $strat = $node_al->field_estrategia;
      $node->field_estrategia = $strat;

      // Premisa
      $strat_term = taxonomy_term_load($strat[LANGUAGE_NONE][0]['tid']);
      $premise = $strat_term->field__estrategia_premisa;
      $node->field_premisa = $premise;

      // Eje  estrat 
      $premise_term = taxonomy_term_load($premise[LANGUAGE_NONE][0]['tid']);
      $eje = $premise_term->field_premisa_eje;
      $node->field_eje_estrategico = $eje;




      break;

    case 'action_line':
      $strategy = $node->field_estrategia[LANGUAGE_NONE][0]['tid'];
      $strategy_term = taxonomy_term_load($strategy);

      // set premise.
      $premise = $strategy_term->field__estrategia_premisa;
      $node->field_premisa = $premise;

      // Set eje.
      $premise_term = taxonomy_term_load($premise[LANGUAGE_NONE][0]['tid']);
      $eje = $premise_term->field_premisa_eje;
      $node->field_eje_estrategico = $eje;

      break;
    
    default:
      return;

      break;
  }

}

/**
 * Implements hook_node_insert.
 */
function pdi_lines_node_insert($node) {
  // Disregard other content types.
  if ( $node->type != 'actividad' ) {
    return;
  }
  
  pdi_lines_progress_calcule($node->field_action_line[LANGUAGE_NONE][0]['target_id']);
}

/**
 * Implements hook_node_update.
 */
function pdi_lines_node_update($node) {
  // Disregard other content types.
  if ( $node->type != 'actividad' ) {
    return;
  }
  
  pdi_lines_progress_calcule($node->field_action_line[LANGUAGE_NONE][0]['target_id']);

}