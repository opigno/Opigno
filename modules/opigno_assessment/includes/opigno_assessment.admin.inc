<?php

/**
 * @file
 * This file contains all admin page callbacks and forms.
 */

/**
 * Global settings form
 */
function opigno_assessment_settings_form($form, $form_state) {
  $form['opigno_assessment_grade_scale'] = array(
    '#type' => 'textfield',
    '#title' => t("Assessment grades scale"),
    '#description' => t("Assessments are based on this scale. It must be numerical, E.g.: 10, 20, 100, etc. Only integers are allowed, no floating point numbers."),
    '#default_value' => variable_get('opigno_assessment_grade_scale', 10),
    '#required' => TRUE,
  );
  
  return system_settings_form($form);
}

/**
 * Global settings validation callback
 */
function opigno_assessment_settings_form_validate($form, $form_state) {
  if (strlen(preg_replace('/[0-9]+/', '', trim($form_state['values']['opigno_assessment_grade_scale'])))) {
    form_set_error('opigno_assessment_grade_scale', t("Only integers are allowed. No letters or floating point numbers (6.5, 10.2, etc)."));
  }
}