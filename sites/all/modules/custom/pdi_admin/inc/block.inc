<?php
/**
 * Implements hook_block_info().
 */
function pdi_admin_block_info() {
  $blocks['admin_menu'] = array(
    'info' => t('Menu Administrativo'),
  );
  $blocks['admin_navigation'] = array(
    'info' => t('Navegacion Administrativa'),
  );

  return $blocks;
}

/**
 * Implements hook_block_view().
 */
function pdi_admin_block_view($delta = '') {
  $block = array();

  switch ($delta) {
    case 'admin_menu':
      $block['subject'] = t('Admin Menu');
      $block['content'] = pdi_admin_block_menu();
      break;

    case 'admin_navigation':
      $block['subject'] = t('Admin Navigation');
      $block['content'] = pdi_admin_block_navigation();
      break;
    
  }
  return $block;
}

function pdi_admin_block_menu(){
  GLOBAL $user; 

  if ($user->uid == 0) {
    $output = '';
  }


  $output = '';
  if (user_access('ver estructura pdi')) {
    $menu = pdi_admin_block_menu_construct();
    $output = $menu;
  } else {
    $output = t('No tienes permisos para ver el archivo.');
  }

  return $output;
}

// Returns the navigation for the activitites 
function pdi_admin_block_navigation(){
  $case = 0;
  if (arg(0) == 'linea_accion' && arg(1) > 0 && arg(2) == 'actividades'){
    $case = 1;
  }

  if (arg(0) == 'node' && arg(1) > 0) {
    $case = 2;
  }

  if (arg(0) == 'admin' && arg(1) != 'node') {
    $case = 3;
  }

  $output = '<div class="admin_nav"><ul>';
  switch ($case) {
    case 0:
        $output .= '<li><a href="/">Inicio</a></li>';
      break;

    case 1:
        $node = node_load(arg(1));
        //print_r($node); exit;
        $output .= '<li><a class="back" href="/taxonomy/term/'.$node->field_estrategia[LANGUAGE_NONE][0]['tid'].'">'.$node->field_estrategia[LANGUAGE_NONE][0]['taxonomy_term']->name.'</a></li>';
      break;
    
    case 2:
      $node = node_load(arg(1));
      $output .= '<li><a class="back" href="/linea_accion/'.$node->field_action_line[LANGUAGE_NONE][0]['target_id'].'/actividades">Regresar a la Linea de Accion</a></li>';  
      break;

    case 3:
      $output .= '';
      break;

    default:
        
      break;
  }

  drupal_add_css(drupal_get_path('module', 'pdi_admin') . '/css/navigation.css');

  $output .= '</ul></div>';

  return $output;

}
