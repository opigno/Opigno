<?php

/**
 * @file
 * OG specific enhancements.
 */

/**
 * Implements hook_form_og_ui_edit_membership_alter().
 */
function opigno_simple_ui_form_og_ui_edit_membership_alter(&$form, $form_state) {
  dpm($form);
}