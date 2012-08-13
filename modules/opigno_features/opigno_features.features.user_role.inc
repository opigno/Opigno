<?php
/**
 * @file
 * opigno_features.features.user_role.inc
 */

/**
 * Implements hook_user_default_roles().
 */
function opigno_features_user_default_roles() {
  $roles = array();

  // Exported role: student.
  $roles['student'] = array(
    'name' => 'student',
    'weight' => '4',
  );

  // Exported role: teacher.
  $roles['teacher'] = array(
    'name' => 'teacher',
    'weight' => '3',
  );

  return $roles;
}
