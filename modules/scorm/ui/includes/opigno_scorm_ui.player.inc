<?php

/**
 * @file
 * Player logic.
 */

/**
 * Traverse the SCORM package data and construct a SCO tree.
 *
 * @param object $scorm
 *
 * @return array
 */
function opigno_scorm_ui_player_scorm_tree($scorm) {
  return _opigno_scorm_ui_player_scorm_tree($scorm->id);
}

/**
 * Helper function to recursively create the SCO tree.
 *
 * @param int $scorm_id
 * @param int $parent_identifier = 0
 *
 * @return array
 */
function _opigno_scorm_ui_player_scorm_tree($scorm_id, $parent_identifier = 0) {
  $tree = array();

  $result = db_select('opigno_scorm_package_scos', 'sco')
    ->fields('sco', array('id'))
    ->condition('sco.scorm_id', $scorm_id)
    ->condition('sco.parent_identifier', $parent_identifier)
    ->execute();

  while ($sco_id = $result->fetchField()) {
    $sco = opigno_scorm_sco_load($sco_id);
    
    $children = _opigno_scorm_ui_player_scorm_tree($scorm_id, $sco->identifier);

    $sco->children = $children;

    $tree[] = $sco;
  }

  return $tree;
}

/**
 * Helper function to flatten the SCORM tree.
 *
 * @param array $tree
 *
 * @return array
 */
function opigno_scorm_ui_player_scorm_flatten_tree($tree) {
  $items = array();

  if (!empty($tree)) {
    foreach ($tree as $sco) {
      $items[] = $sco;
      if (!empty($sco->children)) {
        $items = array_merge($items, opigno_scorm_ui_player_scorm_flatten_tree($sco->children));
      }
    }
  }

  return $items;
}

/**
 * Determine the start SCO for the SCORM package.
 *
 * @todo Get last viewed SCO.
 *
 * @param array $flat_tree
 *
 * @return object
 */
function opigno_scorm_ui_player_start_sco($flat_tree) {
  foreach ($flat_tree as $sco) {
    if (!empty($sco->launch)) {
      return $sco;
    }
  }

  // Failsafe. Just get the first element.
  return array_shift($flat_tree);
}

/**
 * Integrate a SCO object and return it (wrapped if necessary).
 *
 * This page callback bypasses the Drupal page rendering and includes the SCO directly.
 *
 * @param object $sco
 */
function opigno_scorm_ui_player_integrate_sco($sco) {
  // @todo Use hooks for different SCO types.

  // Does the SCO have a launch property ?
  if (!empty($sco->launch)) {
    $query = array();

    // Load the SCO data.
    $scorm = opigno_scorm_scorm_load($sco->scorm_id);

    // Remove the URL parameters from the launch URL.
    if (!empty($sco->attributes['parameters'])) {
      $sco->launch .= $sco->attributes['parameters'];
    }
    $parts = explode('?', $sco->launch);
    $launch = array_shift($parts);

    if (!empty($parts)) {
      // Failsafe - in case a launch URL has 2 or more '?'.
      $parameters = implode('&', $parts);
    }

    // Get the SCO location on the filesystem
    $sco_location = "{$scorm->extracted_dir}/$launch";
    $sco_path = file_create_url($sco_location);

    // Where there any parameters ? If so, prepare them for Drupal.
    if (!empty($parameters)) {
      foreach (explode('&', $parameters) as $param) {
        list($key, $value) = explode('=', $param);
        $query[$key] = !empty($value) ? $value : '';
      }
    }

    drupal_goto($sco_path, array('query' => $query));
  }
  else {
    drupal_not_found();
  }
}

/**
 * Parse the SCO attributes and return them in an HTML ready format.
 *
 * @param object $sco
 *
 * @return string
 */
function opigno_scorm_ui_player_sco_attributes($sco) {
  $attributes = array();

  // Some default "attributes".
  $attributes["data-sco-id"] = $sco->id;
  $attributes["data-sco-can-launch"] = (int) !empty($sco->launch);

  if (!empty($sco->attributes)) {
    foreach ($sco->attributes as $key => $value) {
      if (is_bool($value)) {
        $value = (int) $value;
      }
      elseif (is_array($value) || is_object($value)) {
        $value = drupal_json_encode($value);
      }
      
      $key = str_replace('_', '-', $key);

      $attributes["data-sco-$key"] = $value;
    }
  }

  return drupal_attributes($attributes);
}

/**
 * Helper function to add JS and CSS assets for the SCORM player.
 */
function opigno_scorm_ui_add_assets() {
  $path = drupal_get_path('module', 'opigno_scorm');
  $ui_path = drupal_get_path('module', 'opigno_scorm_ui');
  drupal_add_js("$path/js/lib/api-2004.js");
  drupal_add_js("$path/js/lib/api-1.2.js");
  drupal_add_css("$ui_path/css/opigno_scorm_ui.player.css");
  drupal_add_js("$ui_path/js/lib/player.js");
  drupal_add_js("$ui_path/js/opigno_scorm_ui.player.js");

  module_invoke_all('opigno_scorm_ui_add_assets');
}

/**
 * Get the CMI data for the SCORM player.
 *
 * Invokes the hook_opigno_scorm_ui_register_cmi_data() on all implementing modules
 * to retrieve data to pass to the SCORM player.
 *
 * @param object $scorm
 * @param array $scos
 *
 * @return array
 */
function opigno_scorm_ui_add_cmi_data($scorm, $scos) {
  $data = module_invoke_all('opigno_scorm_ui_register_cmi_data', $scorm, $scos);
  drupal_alter('opigno_scorm_ui_register_cmi_data', $data, $scorm, $scos);
  return $data;
}

/**
 * Get the available CMI paths for the SCORM player.
 *
 * Invokes the hook_opigno_scorm_ui_register_cmi_paths() on all implementing modules
 * to retrieve data to pass to the SCORM player.
 *
 * @return array
 */
function opigno_scorm_ui_add_cmi_paths() {
  $paths = module_invoke_all('opigno_scorm_ui_register_cmi_paths');
  drupal_alter('opigno_scorm_ui_register_cmi_paths', $paths);
  return $paths;
}