<?php
function pdi_preprocess_html(&$variables){
  print_r($variables); exit;
  if ($variables['user']) { 
    if(in_array('admin', $variables['user']->roles)){
      $variables['classes_array'][] = 'archive-active'; 
    }
    if(in_array('developer', $variables['user']->roles)){
      $variables['classes_array'][] = 'archive-active'; 
    }
  }
}