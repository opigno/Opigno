<?php

/**
 * @file
 * Contains all block definitions
 */

/**
 * OG Tools block.
 * Displays all available OG tools, with links ot the appropriate overview pages.
 *
 * @param object $group
 *        The current group from og_context
 * 
 * @return array
 */
function opigno_og_tools_block($group) {
  $html = '';
  
  if (isset($group->gid)) {  
    $tools = opigno_get_og_tools($group);
    
    foreach ($tools as $machine_name => $tool) {
      $html .= theme('opigno__og_tool', array(
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
 * @param object $group
 *        The current group from og_context
 *
 * @return array
 */
function opigno_og_add_content_block($group) {
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

/**
 *
 */
function opigno_og_description_block($group) {
  $node = opigno_get_og_node($group);
      
  return array(
    'subject' => t("Description"),
    'content' => isset($node->body[$node->language][0]['safe_value']) ? $node->body[$node->language][0]['safe_value'] :
                    (isset($node->body[LANGUAGE_NONE][0]['safe_value']) ? $node->body[LANGUAGE_NONE][0]['safe_value'] : '')
  );
}

/**
 *
 */
function opigno_og_course_progress_block($group) {
  // @todo get quiz progress, only for students
  return array(
    'subject' => t("Progress"),
    'content' => theme('progress_bar', array('percent' => 50, 'message' => t("Hi")))
  );
}