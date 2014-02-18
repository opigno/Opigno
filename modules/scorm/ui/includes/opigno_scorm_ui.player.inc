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

    $tree[] = array(
      'title' => $sco->title,
      'identifier' => $sco->identifier,
      'id' => $sco->id,
      'children' => $children,
    );
  }

  return $tree;
}