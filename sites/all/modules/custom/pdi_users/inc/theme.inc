<?php
/**
 * Implements hook_theme.
 */
function pdi_users_theme() {
  return array(
    'pdi_users_user-add-confirmation' => array(
      'template' => 'theme/pdi_users_user-add-confirmation', 
    ),
  );
}
