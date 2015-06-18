<?php

/**
 * Implements hook_preprocess_html().
 */
function pdi_lines_preprocess_html(&$variables) {
  // Add classes for each role.
  if ($variables['user']) { 
    if(in_array('Titular', $variables['user']->roles)){
      $variables['classes_array'][] = 'titular'; 
    }
    if(in_array('Operador', $variables['user']->roles)){
      $variables['classes_array'][] = 'operador'; 
    }
  }
}

/**
 * Implements hook_preprocess_page().
 */
function pdi_lines_preprocess_page(&$vars) {
  
}