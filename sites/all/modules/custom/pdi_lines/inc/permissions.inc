<?php
/**
 * Implements hook_node_access. 
 **/
function pdi_lines_node_access($node, $op, $account) {
  $type = is_string($node) ? $node : $node->type;

  // if its a action_line content type, user content types machine name.
  if ($type == 'actividad' && $op != 'create') {
    // Get the ot fields of node and user 
    global $user;

    if (in_array('Operador', $user->roles)) {
      if (isset($node->field_activity_finished[LANGUAGE_NONE][0]['value']) &&  $node->field_activity_finished[LANGUAGE_NONE][0]['value'] == 1) {
        if ($op == 'update') {
          return NODE_ACCESS_DENY;
        }
      }
    }
    
    $obj_user = user_load($user->uid);
    $user_ot = $obj_user->field_organo_tecnico[LANGUAGE_NONE][0]['tid'];
    $node_ot = $node->field_organo_tecnico[LANGUAGE_NONE][0]['tid'];

    // If the user has the same technicall organ can see the node else no.
    if ($user_ot == $node_ot){
      return NODE_ACCESS_ALLOW;
    }else{
      return NODE_ACCESS_DENY;
    }
  }
  return NODE_ACCESS_IGNORE;
}

/**
 * Callback that decides if the user can calcule the progress or not.
 */
function user_can_calcule_progress($permission = null) {
  global $user;
  if ( $user->uid == 0 ) {
    return false;
  }

  $duser = user_load($user->uid);
  
  if ($duser->status == 0) {
    return false;
  }

  if ( in_array('administrator', $duser->roles) ) {
    return true;
  }

  return false;
}
/**
 * Callback that decide  if the user can see the dashboard.
 **/
function user_can_see_dashboard($permission = null){
  global $user;
  if ( $user->uid == 0 ) {
    return false;
  }

  $duser = user_load($user->uid);
  
  if ($duser->status == 0) {
    return false;
  }

  return true;
}

/**
 * Implements hook_permission().
 */
function pdi_lines_permission() {
  return array(
    'ver listado de actividades' =>  array(
      'title' => t('Ver listado de actividades'),
      'description' => t('Ver el listado de las actividades de cada linea de accion.'),
    ),
  );
}

?>