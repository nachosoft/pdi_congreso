<?php
/**
 * Implements hook_permission().
 */
function pdi_admin_permission() {
  return array(
    /*
     *'administer my module' =>  array(
     *  'title' => t('Administer my module'),
     *  'description' => t('Perform administration tasks for my module.'),
     *),
     */
    'ver estructura pdi' =>  array(
      'title' => t('Ver la estructura del pdi'),
      'description' => t('Crea barra lateral para mostrar la estuctura del pdi en forma de folders.'),
    ),
  );
}