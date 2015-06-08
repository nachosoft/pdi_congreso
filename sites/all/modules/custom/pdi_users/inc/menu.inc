<?php
/**
 * Implements menu_hook.
 **/
function pdi_users_menu(){
  $items['pdi/users/add'] = array(
    'title' => 'Agregar Nuevo Operador', 
    'page callback' => 'drupal_get_form', 
    'page arguments' => array('pdi_users_add_operator_form'),
    'access callback' => 'user_can_create_operator', 
    'access arguments' => array('Crear operadores'), 
    'type' => MENU_CALLBACK, 
  );
  $items['pdi/users/add/confirm'] = array(
    'title' => 'Nuevo Operador Agregado', 
    'page callback' => 'pdi_users_menu_add_confirm', 
    'access callback' => 'user_can_create_operator', 
    'access arguments' => array('Crear operadores'), 
    'type' => MENU_CALLBACK, 
  );
  return $items;
}

// Return a confirmation page for the new operator
function pdi_users_menu_add_confirm(){
  $output = '<div class="card centered"><h2>El nuevo operador fue agregado con exito.</h2>';
  $output .= '<p>El nuevo operador recibira un email con las instrucciones para iniciar sesion.</p></br>';
  $output .= '<div class="cta">
  <a href="/pdi/users/add">Agregar otro operador?</a>
</div></div>';

  return $output;
}

?>