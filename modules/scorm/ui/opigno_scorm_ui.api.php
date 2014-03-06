<?php

/**
 * @file
 * This file contains module hooks definitions and documentation.
 */

/**
 * Implements hook_opigno_scorm_ui_register_cmi_paths().
 *
 * Allows modules to register CMI data paths. The SCORM API object provided by
 * Opigno Scorm ignores un-implemented CMI paths when a SCO tries to store/retrieve
 * that data (as per ADL Requirements).
 *
 * This hook allows module to let the API know it's ok to let the SCO use these paths.
 * Every key is a CMI path. Each key can have a 'readOnly' or a 'writeOnly' value, which
 * should easily be castable to a boolean.
 *
 * @return array
 */
function hook_opigno_scorm_ui_register_cmi_paths() {
  return array(
    'cmi.location' => array(),
    'cmi.objectives._count' => array('readOnly' => 1),
    'cmi.objectives._children' => array('writeOnly' => 1),
  );
}

/**
 * Implements hook_opigno_scorm_ui_register_cmi_data().
 *
 * If the SCORM package or some of it SCOs require data, module can provide it
 * here. This should be used in conjunction with hook_opigno_scorm_ui_register_cmi_paths()
 * to let the SCO know what CMI data is available to it.
 *
 * @param object $scorm
 * @param array $scos
 *
 * @return array
 */
function hook_opigno_scorm_ui_register_cmi_data($scorm, $scos) {
  $data = array(
    'cmi.location' => 0,
  );

  return $data;
}

/**
 * Implements hook_opigno_scorm_ui_add_assets().
 *
 * Allows module to easily provide assets (CSS or JS) when a player
 * is about to be rendered on the page.
 */
function hook_opigno_scorm_ui_add_assets() {
  $path = drupal_get_path('module', 'opigno_scorm_ui');
  drupal_add_js("$path/js/script.js");
}

/**
 * Implements hook_opigno_scorm_ui_commit().
 *
 * Allows module to persist data provided by the SCORM package SCOs.
 * The $data parameter contains a complete representation of the data
 * set on the API object.
 *
 * Important note! Unlike hook_opigno_scorm_ui_register_cmi_data(), which accepts
 * either associate arrays or objects, the $data variable is an object.
 *
 * @param object $scorm
 * @param object $data
 */
function hook_opigno_scorm_ui_commit($scorm, $data) {
  global $user;

  // Store the last position.
  if (!empty($data->cmi->location)) {
    opigno_scorm_scorm_cmi_set($user->uid, $scorm->id, 'cmi.location', $data->cmi->location);
  }
}
