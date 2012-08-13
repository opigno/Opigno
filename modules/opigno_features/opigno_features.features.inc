<?php
/**
 * @file
 * opigno_features.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function opigno_features_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "page_manager" && $api == "pages_default") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function opigno_features_views_api() {
  return array("version" => "3.0");
}

/**
 * Implements hook_homebox().
 */
function opigno_features_homebox() {
  $homeboxes = array();

  $homeboxes['user_dashboard'] = array (
    'regions' => 3,
    'cache' => 1,
    'color' => 1,
    'colors' => 
    array (
      0 => '#E4F0F8',
      1 => '#E4F0F8',
      2 => '#E4F0F8',
      3 => '#E4F0F8',
      4 => '#E4F0F8',
      5 => '#E4F0F8',
    ),
    'blocks' => 
    array (
      'forum_active' => 
      array (
        'module' => 'forum',
        'delta' => 'active',
        'region' => 1,
        'movable' => 1,
        'status' => 1,
        'open' => 1,
        'closable' => 1,
        'title' => '',
        'weight' => -15,
      ),
      'homebox_custom' => 
      array (
        'module' => 'homebox',
        'delta' => 'custom',
        'region' => 2,
        'movable' => 1,
        'status' => 1,
        'open' => 1,
        'closable' => 1,
        'title' => '',
        'weight' => -15,
      ),
      'poll_recent' => 
      array (
        'module' => 'poll',
        'delta' => 'recent',
        'region' => 3,
        'movable' => 1,
        'status' => 1,
        'open' => 1,
        'closable' => 1,
        'title' => '',
        'weight' => -15,
      ),
    ),
    'widths' => 
    array (
      1 => 25,
      2 => 50,
      3 => 25,
    ),
    'title' => 'Dashboard',
    'path' => 'dashboard',
    'menu' => 0,
    'enabled' => 1,
    'auto_save' => 1,
    'full' => 0,
    'roles' => 
    array (
      0 => 'authenticated user',
    ),
  );


  return $homeboxes;
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
    'workgroup' => array(
      'name' => t('Workgroup'),
      'base' => 'node_content',
      'description' => t('A <em>workgroup</em> allows students and/or teachers to work together on a specific task. <em>Workgroups</em> are created inside <em>courses</em> or even other <em>workgroups</em>.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}
