<?php
/**
 * @file
 * opigno_features.features.inc
 */

/**
 * Implements hook_views_api().
 */
function opigno_features_views_api() {
  return array("version" => "3.0");
}

/**
 * Implements hook_node_info().
 */
function opigno_features_node_info() {
  $items = array(
    'calendar_entry' => array(
      'name' => t('Calendar entry'),
      'base' => 'node_content',
      'description' => t('<em>Calendar entries</em>'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'course' => array(
      'name' => t('Course'),
      'base' => 'node_content',
      'description' => t('A <em>course</em> can contain <em>quizzes</em>, <em>wikis</em> and other tools necessary for teaching a certain topic.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'wiki' => array(
      'name' => t('Wiki page'),
      'base' => 'node_content',
      'description' => t('<em>Wikis</em> have a built-in hierarchical navigation. Use for handbooks or tutorials.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}
