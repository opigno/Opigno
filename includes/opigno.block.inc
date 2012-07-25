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
  
  $tools = module_invoke_all('opigno_course_tools');
  
  foreach ($tools as $i => $tool) {
    $html .= theme('opigno_og_tool', array(
      'og_type' => 'course',
      'machine_name' => isset($tool['machine_name']) ? $tool['machine_name'] : "tool-$i",
      'name' => $tool['name'],
      'uri' => $tool['uri']
    ));
  }
  
  return array(
    'subject' => t("Tools"),
    'content' => $html
  );
}