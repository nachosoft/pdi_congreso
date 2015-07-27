<?php
// Implements hook_form_alter.
function pdi_lines_form_node_form_alter(&$form, $form_state, $form_id) {
  // if you are targeting a specific content type then 
  // you can access the type:

  if ($form_id == 'user_register') {
    print_r($form); exit;
  }

  if ($form['#node']->type != 'action_line') {
    return;
  }

  global $user;

  if (in_array('administrator', $user->roles)) {
    return;
  }
  
  //$user_strategy_field = pdi_lines_get_user_strategies($user->uid);
  //$form['field_estrategia'][LANGUAGE_NONE]['#options'] = $user_strategy_field;
}