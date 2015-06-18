<?php
/**
 * Implements hook_block_info.
 */
function pdi_lines_block_info() {
  $blocks['pdi_calculate_progress'] = array(
    'info' => t('Calculate Progress'), 
    'cache' => DRUPAL_NO_CACHE,
  );
  $blocks['pdi_progress'] = array(
    'info' => t('Progreso'), 
    'cache' => DRUPAL_NO_CACHE,
  );
  return $blocks;
}

/**
 * Implements hook_block_view.
 */
function pdi_lines_block_view($delta = '') {
  $block = array(
    'content' => false, 
  );
  switch($delta) {
    case 'pdi_calculate_progress':
      if ( user_can_calcule_progress() ) {
        $block['subject'] = t('Recalcular Progreso?');
        $block['content'] = pdi_calculate_progress_block_content();
      }
      break;

    case 'pdi_progress':
      $block['subject'] = t('Progreso');
      $block['content'] = pdi_progress_block_content();
      break;
  }
  return $block;
}

/**
 * Create a button to recalcule the progress.
 **/
function pdi_calculate_progress_block_content(){
  $link = '';

  if ( arg(0) == 'taxonomy' && arg(1) == 'term' && intval(arg(2)) > 0) {
    $term = taxonomy_term_load(arg(2));
    $term_vname = $term->vocabulary_machine_name;

    switch ($term_vname) {
      case 'premisa':
        $link = '<div class="progress_calcule"><div class="cta"><a href="/pdi/premise/calcule/'. arg(2) .'">Recalcular Progreso?</a></div></div>';
        break;

      case 'estrategia':
        $link = '<div class="progress_calcule"><div class="cta"><a href="/pdi/strategy/calcule/'. arg(2) .'">Recalcular Progreso?</a></div></div>';
        break;
      
      default:
        $link = '';
        break;
    }
  }

  if (arg(0) == 'node' && intval(arg(1)) > 0) {
    $node = node_load(arg(1));

    if ($node->type == 'action_line') {
      $link = '<div class="progress_calcule"><div class="cta"><a href="/pdi/line/calcule/'. arg(1) .'">Recalcular Progreso?</a></div></div>';
    } 
  }

  return $link;
}

/**
 * Return progress bar for each element.
 **/
function pdi_progress_block_content(){
  $output = '';

  if (arg(0) == 'taxonomy' && arg(1) == 'term' && intval(arg(2)) > 0) {
    $output = pdi_progress_block_content_taxonomy(arg(2));
  } else if (arg(0) == 'node' && intval(arg(1)) > 0) {

  }

  return $output;
}

/**
 * return progress per taxonomy term 
 **/
function pdi_progress_block_content_taxonomy($tid){
  $term = taxonomy_term_load($tid);
  $tnodes = taxonomy_select_nodes($tid);

  if (!isset($tnodes)) {
    return;
  }

  $progress = 0;

  $rnodes = array();

  foreach ($tnodes as $key => $nid) {
    $node = node_load($nid);

    if ($node->type == 'action_line') {
      $rnodes[$nid]['progress'] = $node->field_percentage_action[LANGUAGE_NONE][0]['value'];
    }
  }
  if (count($rnodes) == 0) {
    $npercentage = 0;
  } else {
    $npercentage = 1/count($rnodes);
  }
  
  foreach ($rnodes as $key => $node) {
    $progress = $progress + $node['progress'] * $npercentage ;
  }

  $term->field_progreso_p[LANGUAGE_NONE][0]['value'] = $progress;
  taxonomy_term_save($term);

  $output = create_progress_widget($progress, $term->name);

  return $output;
}

