<?php

/**
 * @file
 */

/**
 * Implements hook_menu_alter()
 */
function opigno_simple_ui_user_menu_alter(&$items) {
  $admin_root_path = opigno_get_admin_root_path('', 'user');

  $items["$admin_root_path/profiles"] = $items['admin/config/people/accounts'];
  $items["$admin_root_path/blocking"] = $items['admin/config/people/ip-blocking'];
  $items["$admin_root_path/list"]     = $items['admin/people'];
}

