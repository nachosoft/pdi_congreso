<?php
/**
 * Implements hook_block_info.
 */
function pdi_lines_block_info() {
  $blocks['pdi_calculate_progress'] = array(
    'info' => t('Calculate Progress'), 
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
