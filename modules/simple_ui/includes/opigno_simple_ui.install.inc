<?php

/**
 * @file
 * Define customizations that are called on module install or other module installs.
 */

/**
 * Rename Quiz questions.
 * Some Quiz questions are not always intuitively named. Give them better names and descriptions.
 */
function opigno_simple_ui_change_quiz_question_bundles_names() {
  // Quiz directions
  $type = node_type_load('quiz_directions');
  if (!empty($type)) {
    $type->name = "Slide";
    $type->description = "Provide info for a scenario.";
    node_type_save($type);
  
    $instance = field_info_instance('node', 'body', 'quiz_directions');
    if (!empty($instance)) {
      $instance['label'] = "Content";
      field_update_instance($instance);
    }  
  }
}