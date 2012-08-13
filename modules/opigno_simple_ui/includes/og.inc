<?php

/**
 * @file
 */

/**
 * Implements hook_menu_alter()
 */
function opigno_simple_ui_og_menu_alter(&$items) {
  // Add Members tabs
  $items['node/%/members'] = array_merge($items['group/%/%/admin/people'], array(
    'title' => 'Members',
    'title callback' => 't',
    'title arguments' => array(),
    'page arguments' => array('og_ui_user_admin_account', 0, 1),
    'access arguments' => array('approve and deny subscription', 0, 1),
    'type' => MENU_LOCAL_TASK,
  ));
  
  $items['node/%/members/view'] = array(
    'title' => 'Members',
    'type' => MENU_DEFAULT_LOCAL_TASK,
  );
  
  $items['node/%/members/add'] = array_merge($items['group/%/%/admin/people/add-user'], array(
    'page arguments' => array('og_ui_add_users', 0, 1),
    'access arguments' => array('add user', 0, 1),
    'type' => MENU_LOCAL_TASK,
  ));
  
  // Remove Group tab, but leave it usable, just in case
  $items['node/%/group']['type'] = MENU_CALLBACK;
}

/**
 * Implements hook_form_alter()
 */
function opigno_simple_ui_og_form_alter(&$form, &$form_state, $form_id) {
  global $language;
  
  if (strpos($form_id, '_node_form')) {
    if (og_is_group_type('node', $form['#node']->type)) {
      $form['group_group']['#type'] = 'hidden';
      $form['group_group'][$form['#node']->language]['#type'] = 'hidden';
      $form['group_group'][$form['#node']->language]['#value'] = 1;
    }
    elseif (og_is_group_content_type('node', $form['#node']->type)) {
      if (isset($_GET['gids_node'])) {
        $form['group_audience']['#attributes']['class'][] = 'element-hidden';
      }
    }
  }
}
