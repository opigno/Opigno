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
  module_invoke_all('opigno_scorm_ui_commit', $scorm, json_decode($_POST['data']));
}
