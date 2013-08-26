<?php

/**
 * @file
 * Quiz specific enhancements.
 */

/**
 * Implements hook_form_quiz_directions_node_form_alter().
 */
function opigno_simple_ui_form_quiz_directions_node_form_alter(&$form, $form_state) {
  $form['add_directly']['#access'] = FALSE;
}

/**
 * Implements hook_form_quiz_questions_form_alter().
 */
function opigno_simple_ui_form_quiz_questions_form_alter(&$form, $form_state) {
  $form['question_list']['browser']['#collapsible'] = TRUE;
  $form['question_list']['browser']['#collapsed'] = TRUE;

  // If this quiz is a 'theory' quiz, hide questions.
  $nid = arg(1);
  if ($nid && is_numeric($nid)) {
    $quiz = node_load($nid);
    if (!empty($quiz) && !empty($quiz->quiz_type[LANGUAGE_NONE][0]['value']) && $quiz->quiz_type[LANGUAGE_NONE][0]['value'] == 'theory' && isset($form['additional_questions']['quiz_directions'])) {
      $form['add_slide'] = array(
        '#type' => 'fieldset',
        '#title' => t("Add a slide to the lesson"),
        '#description' => t("A lesson of type 'theory' can only have slides as content."),
        '#weight' => -50,
      );
      $form['add_slide']['add_slide'] = $form['additional_questions']['quiz_directions'];

      $form['question_list']['#title'] = t("Slides inside this lesson");
      $form['question_list']['browser']['#access'] = FALSE;

      $form['additional_questions']['#attributes']['class'][] = 'element-hidden';
    }
  }
}

/**
 * Implements hook_form_quiz_node_form_alter().
 */
function opigno_simple_ui_form_quiz_node_form_alter(&$form, $form_state) {
  foreach ($form['resultoptions'] as $key => &$child) {
    if ($child['#type'] == 'fieldset') {
      $child['#collapsed'] = TRUE;
    }
  }
  $form['#validate'][] = 'opigno_simple_ui_form_quiz_node_form_alter_validate';
}

/**
 * Validate callback for .
 */
function opigno_simple_ui_form_quiz_node_form_alter_validate($form, &$form_state) {
  // If of type theory, deactivate Allow skipping questions".
  if (!empty($form_state['values']['quiz_type'][LANGUAGE_NONE][0]['value']) && $form_state['values']['quiz_type'][LANGUAGE_NONE][0]['value'] == 'theory') {
    $form_state['values']['allow_skipping'] = 0;
  }
}