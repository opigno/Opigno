<?php

/**
 * @file
 * Rules implementation.
 */


/**
 * Implements hook_rules_action_info().
 */
function opigno_og_prereq_rules_action_info() {
  $items['opigno_og_prereq_rules_get_required_og'] = array(
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
    'base' => 'opigno_og_prereq_rules_get_required_og',
  );
  $items['opigno_og_prereq_gpc_with_required_course'] = array(
    'label' => t("Get passed courses with required course"),
    'group' => t('Opigno'),
    'parameter' => array(
      'course' => array(
        'type' => 'node',
        'label' => t('node'),
        'description' => t('The course'),
      ),
    ),
    'provides' => array(
      'entities_fetched' => array(
        'type' => 'list<node>',
        'label' => t('Requiring courses'),
      ),
    ),
    'base' => 'opigno_og_prereq_gpc_with_required_course',
  );
  return $items;
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

function opigno_og_prereq_gpc_with_required_course($course) {
  Global $user;
  $nodes = node_load_multiple(array(), array('type' => 'course'));
  $required_courses = array();
  foreach ($nodes as $node) {
    if (isset($node->course_required_course_ref[LANGUAGE_NONE][0]['target_id'])) {
      $hastheone = FALSE;
      $completed_all = TRUE;
      foreach ($node->course_required_course_ref[LANGUAGE_NONE] as $index => $target) {
        if ($target['target_id'] != $course->nid) {
          if (!opigno_quiz_app_user_passed($target['target_id'], $user->uid)) //passed it
          {
            $completed_all = FALSE;
          }
        }
        else {
          $hastheone = TRUE;
        }
      }
      if (($completed_all == TRUE) && ($hastheone == TRUE)) {
        $required_courses[] = $node;
      }
    }
  }
  return array('entities_fetched' => $required_courses);
}
