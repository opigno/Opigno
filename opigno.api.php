<?php

/**
 * @file
 * Defines the Opigno LMS specific API and hooks
 */

/**
 * Implements hook_opigno_tool().
 *
 * @param stdClass $node = NULL
 *        (optional) This hook can be called in context of a specific group. In that case, the
 *        $node parameter will contain the OG node.
 *        If called with no specific context (like lilsting available tools on the platform)
 *        the $node parameter will be NULL.
 *
 * @return array
 */
function hook_opigno_tool($node = NULL) {
  /**
   * Return
   */
  return array(
    'tool_machine_name' => array(
      'name' => t("Quiz import"),
      'path' => isset($node) ? "node/{$node->nid}/import-quiz" : 'admin/quiz/import/xls',
      'access_arguments' => array('import quiz questions xls'),
      'description' => t("Import Excel files containing multiple choice questions as a quiz."),
      'actions' => array(
        'import_quiz' => array(
          'title' => t("Import a new Quiz"),
          'href' => isset($node) ? "node/{$node->nid}/import-quiz" : 'admin/quiz/import/xls',
          'access_arguments' => array('import quiz questions xls'),
        ),
      ),
    ),
  );
}