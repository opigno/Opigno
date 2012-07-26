<?php

/**
 * @file
 * Contains all block definitions
 */

/**
 * OG Tools block.
 * Displays all available OG tools, with links ot the appropriate overview pages.
 *
 * @return array
 */
function opigno_og_tools_block() {
  $html = '';
  
  $node = menu_get_object('node');
  
  if (isset($node) && opigno_is_og_type($node->type)) {  
    $tools = opigno_get_og_tools($node);
    
    foreach ($tools as $machine_name => $tool) {
      $html .= theme('opigno__og_tool', array(
        'og_type' => $node->type,
        'machine_name' => $machine_name,
        'name' => $tool['tool_name'],
        'url' => $tool['tool_url']
      ));
    }
  }
  
  return array(
    'subject' => t("Tools"),
    'content' => $html
  );
}

/**
 * OG Add content block.
 * Display links to create content for the current OG.
 *
 * @return array
 */
function opigno_og_add_content_block() {
  $html = '';
  
  $node = menu_get_object('node');
  
  if (isset($node) && opigno_is_og_type($node->type)) {  
    $tools = opigno_get_og_tools($node);
    
    $html = theme('opigno__add_content_to_og_list', array(
      'og_type' => $node->type,
      'tools' => $tools
    ));
  }
  
  return array(
    'subject' => t("Add content"),
    'content' => $html
  );
}