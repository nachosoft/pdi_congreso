<?php 

/**
 * Callback that decides if the user can create an operator or not.
 */
function user_can_create_operator($permission = null) {
  if ( !is_null($permission) ) {
    if ( !user_access($permission) ) {
      return false; 
    }
  }
  
  global $user;
  if ( $user->uid == 0 ) {
    return false;
  }
  
  $duser = user_load($user->uid);
  
  if ($duser->status == 0) {
    return false;
  }
  
  if ( in_array('Titular', $duser->roles) ) {
    return true;
  }
  

  return false;
}