<?php

/**
 * @file
 * Rules implementation.
 */


/**
 * Implements hook_rules_action_info().
 */
function opigno_og_prereq_rules_action_info() {
  return array(
    'opigno_og_prereq_rules_get_required_og' => array(
      'group' => 'Opigno',
      'label' => t('Get requiring courses'),
      'arguments' => array(
        'node' => array(
          'type' => 'node',
          'label' => t('Course node'),
        ),
      ),
      'provides' => array(
        'entities_fetched' => array(
          'type' => 'list<node>',
          'label' => t('Requiring courses'),
        ),
      ),
    ),
  );
}

/**
 * Action: Get a property from a quiz.
 */
function opigno_og_prereq_rules_get_required_og($node, $settings) {
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node')
    ->propertyCondition('status', 1)
    ->entityCondition('bundle', 'course')
    ->fieldCondition('course_required_course_ref', 'target_id', $node->nid, '=')
    ->addMetaData('account', user_load(1)); // Run as admin.

  $result = $query->execute();
  $courses = array();
  if (isset($result['node'])) {
    $nids = array_keys($result['node']);
    foreach ($nids as $nid) {
      $courses[$nid] = node_load($nid);
    }
  }

  return array('entities_fetched' => $courses);
}
