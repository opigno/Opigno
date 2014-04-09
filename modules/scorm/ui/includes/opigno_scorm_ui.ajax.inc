<?php

/**
 * @file
 * Provides the AJAX callbacks for the SCORM UI module.
 */

/**
 * AJAX callback: SCORM committed data.
 *
 * @param object $scorm
 */
function opigno_scorm_ui_ajax_commit($scorm) {
  if (!empty($_POST['data'])) {
    module_invoke_all('opigno_scorm_ui_commit', $scorm, json_decode($_POST['data']));
    drupal_json_output(array('success' => 1));
  }
  else {
    drupal_json_output(array('error' => 1, 'message' => 'no data received'));
  }
}
