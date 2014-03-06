<?php

/**
 * @file
 * Provides the AJAX callbacks for the SCORM API.
 */

/**
 * AJAX callback: CMI communication.
 *
 * @param string $action 
 *        Either 'get' or 'set'.
 * @param int $sco_id
 * @param string $cmi_key
 */
function opigno_scorm_ajax_sco_cmi($action, $sco_id, $cmi_key) {
  global $user;
  if ($action == 'set') {
    opigno_scorm_sco_cmi_set($user->uid, $sco_id, $cmi_key, $_POST['value']);
    drupal_json_output(array('success' => 1));
  }
  else {
    $data = opigno_scorm_sco_cmi_get($user->uid, $sco_id, $cmi_key);
    drupal_json_output(array('data' => $data));
  }
}