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
