<?php

/**
 * @file
 * Defines the Opigno LMS specific API and hooks
 */

/**
 * Implements hook_opigno_tool().
 *
 * This hook allows modules to define course tools.
 * Course tools provide functionality inside the context of a course. Usually, each tool has
 * it's own, specific "page", or interface. For modules like Crumb to pickup the relation with the
 * course for the breadcrumbs, it's usually a good idea to construct a URL like node/:nid/tool-name.
 *
 * @param stdClass $node = NULL
 *        (optional) This hook can be called in context of a specific group. In that case, the
 *        $node parameter will contain the OG node.
 *        If called with no specific context (like listing available tools on the platform)
 *        the $node parameter will be NULL.
 *
 * @return array
 */
function hook_opigno_tool($node = NULL) {
  /**
   * Return an array, keyed by tool machine name.
   * This array can (and should) be different whether it is invoked in context of a course
   * or for the global platform.
   */
  return array(
    'tool_machine_name' => array(
      // The human-readable name of the tool.
      'name' => t("Quiz import"),
      // The path of the tool.
      'path' => isset($node) ? "node/{$node->nid}/import-quiz" : 'admin/quiz/import/xls',
      // Access arguments for this tool. Defaults to array('access content').
      'access_arguments' => isset($node) ? array('node', $node->nid, 'create quiz content') : array('create quiz content'),
      // Custom access callback. Note that it is advised to use OG access control and permissions whenever possible.
      'access_callback' => isset($node) ? 'og_user_access' : 'user_access',
      // Description of the tool. This is shown to the end users.
      'description' => t("Import Excel files containing multiple choice questions as a quiz."),
      /**
       * You may provide a list of actions authorized users can take for this tool.
       * Actions are listed as links inside an "Actions" block Opigno core provides.
       * Each action link is passed as-is to the theme_link() function. For more information
       * on formatting a link object, see: https://api.drupal.org/api/drupal/includes%21theme.inc/function/theme_links/7.
       * The only exception are the access_arguments and access_callback keys, which gives
       * you fine-grained access control.
       */
      'actions' => array(
        // Keyed by the action name.
        'import_quiz' => array(
          'title' => t("Import a new Quiz"),
          'href' => isset($node) ? "node/{$node->nid}/import-quiz" : 'admin/quiz/import/xls',
          // Access control is done in the same way as the parent level.
          'access_arguments' => array('import quiz questions xls'),
        ),
      ),
    ),
  );
}

/**
 * Implements hook_opigno_tool_alter().
 *
 * @param  array $tools
 *        An array of all available tools. These can be updated here.
 */
function hook_opigno_tool_alter(&$tools) {
  $tools['tool_machine_name']['name'] = t("Updated Quiz name");
}
