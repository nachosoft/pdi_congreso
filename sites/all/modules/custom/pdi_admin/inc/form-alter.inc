<?php
/**
 * Implements hook_form_alter().
 */
function pdi_admin_form_alter(&$form, &$form_state, $form_id) {
  if ($form_id == 'taxonomy_form_term'){
    switch ($form['#term']['vocabulary_machine_name']) {
      case 'premisa':
        if (isset($_GET['axis'])){
          $axis = intval($_GET['axis']);
        }
        print_r($axis); exit;
        if (isset($axis) && is_int($axis) && $axis > 0){
          if (array_key_exists($axis, $form['field_premisa_eje'][LANGUAGE_NONE]['#options'])){
            $form['field_premisa_eje'][LANGUAGE_NONE]['#default_value'][0] = $axis ;
          }
        }
        break;
      
      case 'estrategia':
        if (isset($_GET['premise'])){
          $premise = intval($_GET['premise']);
        }
        if (isset($premise) && is_int($premise) && $premise > 0){
          if (array_key_exists($premise, $form['field__estrategia_premisa'][LANGUAGE_NONE]['#options'])){
            $form['field__estrategia_premisa'][LANGUAGE_NONE]['#default_value'][0] = $premise ;
          }
          //print_r($form['field__estrategia_premisa'][LANGUAGE_NONE]); exit;
        }
        break;

      default:
        
        break;
    }
  }
  //$form['field_premisa_eje'][LANGUAGE_NONE]['#default_value'][0] = 13 ;
  //print_r($form['field_premisa_eje'][LANGUAGE_NONE]['#default_value']); exit;
} 
