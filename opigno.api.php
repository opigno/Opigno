<?php

/**
 * @file
 * Defines the Opigno LMS specific API and hooks
 */

/**
 * Implements hook_opigno_widget_info()
 *
 * Defines widgets that can be used on the OG homepage dashboard.
 * Must return an array identical to that of hook_block_info()
 *
 * @return array
 */
function hook_opigno_widget_info() {
  return array(
    'og_tools' => array(
      'info' => t("Opigno: OG tools"),
      'cache' => DRUPAL_CACHE_PER_PAGE
    ),
    'add_og_content' => array(
      'info' => t("Opigno: Add OG content"),
      'cache' => DRUPAL_CACHE_PER_USER
    )
  );
}

/**
 * Implements hook_opigno_widget_view()
 *
 * Returns the widget subject and content.
 * Must return an array identical to that of hook_block_view()
 *
 * @return array
 */
function hook_opigno_widget_view($group, $delta) {
  // First check our block
  switch ($delta) {
    case 'og_tools':
      $html = '';
  
      if (isset($group->gid)) {  
        $tools = opigno_get_og_tools($group);
        
        $html = theme('opigno__add_content_to_og_list', array(
          'tools' => $tools
        ));
      }
      
      return array(
        'subject' => t("Add content"),
        'content' => $html
      );
  }
}

/**
 * Implements hook_opigno_tools()
 *
 * Defines the tools that can be used inside a group.
 * The resulting array must be keyed by tool-id.
 * Each entry can contain the following keys:
 *  - tool_name: (required) A human readable tool name
 *  - tool_url: (required) A url to a tool overview page (usually a view)
 *  - create_label: (optional) A label for the "create tool" link. User access check should be done by the
 *                  module. Opigno does not verify the user access.
 *  - create_url: (optional) A url to the create page
 *
 * @return array
 */
function hook_opigno_tools($node) {
  $tools = array();
  
  $tools['wikis'] = array(
    'tool_name' => t("Wikis"),
    'tool_url'  => url("node/{$node->nid}/wikis")
  );
  
  if (user_access('create wiki content')) {
    $tools['wikis']['create_label'] = t("Add a !type", array('!type' => 'wiki'));
    $tools['wikis']['create_url']   = url('node/add/wiki', array('query' => array('gids_node[]' => $node->nid)));
  }
  
  return $tools;
}