<?php
/**
 * Implements hook_block_info().
 */
function pdi_admin_block_info() {
  $blocks['admin_menu'] = array(
    'info' => t('Menu Administrativo'),
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
    
  }
  return $block;
}

function pdi_admin_block_menu(){
  GLOBAL $user; 

  if ($user->uid == 0) {
    $output = '';
  }
  $output = '';
  if (in_array('admin', $user->roles)) {
    $menu = pdi_admin_block_menu_construct();
    $output = $menu;
  }

  return $output;
}


