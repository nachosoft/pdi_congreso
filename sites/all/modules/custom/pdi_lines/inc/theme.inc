<?php
/**
 * Implements hook_theme.
 */
function pdi_lines_theme() {
  return array(
    'pdi_lines_dashboard' => array(
      'arguments' => array(
        'progress' => NULL, 
        'actions' => NULL, 
        'users' => NULL, 
        'premises' => NULL, 
      ), 
      'template' => 'theme/pdi_lines_dashboard', 
    ),
  );
}
