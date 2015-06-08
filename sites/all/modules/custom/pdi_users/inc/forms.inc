<?php 
/**
 * Implements form_hook.
 **/
function pdi_users_add_operator_form($form, &$form_state){
  $form ['operator_username'] = array(
    '#title' => 'Nombre de usuario del Operador',
    '#type' => 'textfield',
    '#placeholder' => t('Nombre de usuario del Operador'), 
    '#attributes' => array('placeholder' => t('Nombre de usuario del Operador')), 
    '#required' => TRUE,
  );

  $form ['operator_email'] = array(
    '#title' => 'Email del Operador',
    '#type' => 'textfield',
    '#placeholder' => t('Email del Operador'), 
    '#attributes' => array('placeholder' => t('Email del Operador')), 
    '#required' => TRUE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Guardar'),
  );

  return $form;
}

function pdi_users_add_operator_form_submit($form,&$form_state) {
  global $user;
  $duser = user_load($user->uid);

  // Set Operador role.
  $roles = array(
    2 => 'authenticated user', 
    5 => 'Operador', 
  );


  // Create a random password.
  $upass = user_password();

  // Get the user info.
  $nuser = array(
    'name' => $form_state['values']['operator_username'], 
    'roles' => $roles,
    'pass' => $upass,
    'mail' => $form_state['values']['operator_email'],
    'init' => $form_state['values']['operator_email'], 
    'field_area' => $duser->field_area,
    'status' => 1, 
    'access' => REQUEST_TIME, 
  );

  // Save the user.
  user_save(null, $nuser);

  // Setup the email.
  $to = $form_state['values']['operator_email'];
  $subject = 'New account created';
  $headers = "From: congreso@congreso.org\nContent-type: text/html";
  $body = 'Una nueva cuenta de operador ha sido creada para ti.<br /> Tu informacion de inicio de sesion es:<br />Usuario: <b>'.$form_state['values']['operator_username'].'</b><br /> contraseña: <b>'.$upass.'</b> <br /><br /> ';
  
  // Send a mail with the password.
  mail($to,$subject,$body,$headers);  

  drupal_goto('pdi/users/add/confirm');
}