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
    ->fields('sco', array())
    ->condition('sco.scorm_id', $scorm_id)
    ->condition('sco.parent_identifier', $parent_identifier)
    ->execute();

  while ($sco = $result->fetchObject()) {
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
 * @param array $tree
 *
 * @return object
 */
function opigno_scorm_ui_player_start_sco($tree) {
  $flat_tree = opigno_scorm_ui_player_scorm_flatten_tree($tree);

  foreach ($flat_tree as $sco) {
    if (!empty($sco->launch)) {
      return $sco;
    }
  }

  // Failsafe.
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

  if (!empty($sco->launch)) {
    $scorm = opigno_scorm_scorm_load($sco->scorm_id);
    $sco_location = "{$scorm->extracted_dir}/{$sco->launch}";
    $sco_path = file_create_url($sco_location);
    drupal_goto($sco_path);
  }
  else {
    drupal_not_found();
  }
}