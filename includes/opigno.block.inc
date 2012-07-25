<?php

/**
 * @file
 * Contains all block definitions
 */

/**
 * Course Tools block.
 * Displays all available course tools, with links ot the appropriate overview pages.
 *
 * @return array
 */
function opigno_course_tools_block() {
  $html = '';
  
  $node = menu_get_object('node');
  
  if (isset($node) && $node->type == 'course') {  
    $tools = module_invoke_all('opigno_course_tools', $node);
    
    foreach ($tools as $machine_name => $tool) {
      $html .= theme('opigno_og_tool', array(
        'og_type' => 'course',
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