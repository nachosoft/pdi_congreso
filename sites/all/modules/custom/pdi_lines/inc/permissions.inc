<?php
/**
 * Implements hook_node_access. 
 **/
function pdi_lines_node_access($node, $op, $account) {
  $type = is_string($node) ? $node : $node->type;
  // if its a action_line content type, user content types machine name.
  if ($type == 'action_line' && $op != 'create') {
    if ($op == 'update') {
      return NODE_ACCESS_IGNORE;
    }
    // Get the area fields of node and user 
    global $user;
    $obj_user = user_load($user->uid);
    $user_area = $obj_user->field_area[LANGUAGE_NONE][0]['tid'];
    $node_area = $node->field_area[LANGUAGE_NONE][0]['tid'];
    // If the user has the same area can see the node else no.
    if ($user_area == $node_area){
      return NODE_ACCESS_ALLOW;
    }
    else {
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

?>