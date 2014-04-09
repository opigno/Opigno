<?php

/**
 * @file
 * This file contains module hooks definitions and documentation.
 */

/**
 * Implements hook_opigno_scorm_cmi_set_alter().
 *
 * This hook allows modules to alter CMI data that is about to
 * be stored. If the module wants to take over the persistence
 * entirely, it can set the value to NULL. Opigno Scorm will then
 * skip the persisting of the data and assume some other module
 * took over.
 *
 * @param mixed $value
 *        The data to be persisted.
 * @param string $cmi_key
 *        The CMI data indentifier for the value.
 * @param array $context
 *        Context array, with at least the following keys:
 *         - uid: The user ID
 *         - sco_id: The SCO ID.
 *         - original_value: The original value, in case some
 *           module alters it.
 */
function hook_opigno_scorm_sco_cmi_set_alter(&$value, $cmi_key, $context) {
  if ($cmi_key === 'cmi.last_location') {
    // Store this somewhere else.
    // db_insert(...)

    // Let Opigno Scorm know we stored this.
    $value = NULL;
  }
}

/**
 * Implements hook_opigno_scorm_cmi_get_alter().
 *
 * This hook allows modules to alter CMI data that is about to
 * be sent back to the SCO.
 *
 * @param mixed $value
 *        The data that will be returned. Can be NULL if not
 *        previously persisted in the database.
 * @param string $cmi_key
 *        The CMI data indentifier for the value.
 * @param array $context
 *        Context array, with at least the following keys:
 *         - uid: The user ID
 *         - sco_id: The SCO ID.
 *         - original_value: The original value, in case some
 *           module alters it.
 */
function hook_opigno_scorm_sco_cmi_get_alter(&$value, $cmi_key, $context) {
  if ($cmi_key === 'cmi.learner_name') {
    $profile = profile_load($context['uid']);
    $value = "{$profile->first_name} {$profile->last_name}";
  }
}

/**
 * Implements hook_opigno_scorm_cmi_set_alter().
 *
 * This hook allows modules to alter CMI data that is about to
 * be stored. If the module wants to take over the persistence
 * entirely, it can set the value to NULL. Opigno Scorm will then
 * skip the persisting of the data and assume some other module
 * took over.
 *
 * @param mixed $value
 *        The data to be persisted.
 * @param string $cmi_key
 *        The CMI data indentifier for the value.
 * @param array $context
 *        Context array, with at least the following keys:
 *         - uid: The user ID
 *         - scorm_id: The SCORM ID.
 *         - original_value: The original value, in case some
 *           module alters it.
 */
function hook_opigno_scorm_scorm_cmi_set_alter(&$value, $cmi_key, $context) {
  if ($cmi_key === 'cmi.last_location') {
    // Store this somewhere else.
    // db_insert(...)

    // Let Opigno Scorm know we stored this.
    $value = NULL;
  }
}

/**
 * Implements hook_opigno_scorm_cmi_get_alter().
 *
 * This hook allows modules to alter CMI data that is about to
 * be sent back to the SCORM.
 *
 * @param mixed $value
 *        The data that will be returned. Can be NULL if not
 *        previously persisted in the database.
 * @param string $cmi_key
 *        The CMI data indentifier for the value.
 * @param array $context
 *        Context array, with at least the following keys:
 *         - uid: The user ID
 *         - scorm_id: The SCORM ID.
 *         - original_value: The original value, in case some
 *           module alters it.
 */
function hook_opigno_scorm_scorm_cmi_get_alter(&$value, $cmi_key, $context) {
  if ($cmi_key === 'cmi.learner_name') {
    $profile = profile_load($context['uid']);
    $value = "{$profile->first_name} {$profile->last_name}";
  }
}
