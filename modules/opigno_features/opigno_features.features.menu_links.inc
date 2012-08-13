<?php
/**
 * @file
 * opigno_features.features.menu_links.inc
 */

/**
 * Implements hook_menu_default_menu_links().
 */
function opigno_features_menu_default_menu_links() {
  $menu_links = array();

  // Exported menu link: main-menu:courses
  $menu_links['main-menu:courses'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'courses',
    'router_path' => 'courses',
    'link_title' => 'Courses',
    'options' => array(),
    'module' => 'system',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '20',
  );
  // Exported menu link: main-menu:dashboard
  $menu_links['main-menu:dashboard'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'dashboard',
    'router_path' => 'dashboard',
    'link_title' => 'Dashboard',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
    ),
    'module' => 'menu',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '0',
  );
  // Exported menu link: main-menu:workgroups
  $menu_links['main-menu:workgroups'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'workgroups',
    'router_path' => 'workgroups',
    'link_title' => 'Workgroups',
    'options' => array(),
    'module' => 'system',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '30',
  );
  // Translatables
  // Included for use with string extractors like potx.
  t('Courses');
  t('Dashboard');
  t('Workgroups');


  return $menu_links;
}
