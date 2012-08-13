<?php

/**
 * @file
 */

/**
 * Implements hook_menu_alter()
 */
function opigno_simple_ui_quiz_menu_alter(&$items) {
  $items['node/%node/edit/questions'] = $items['node/%node/questions'];
  
  $items['node/%node/edit/edit'] = array(
    'title' => 'Edit',
    'type' => MENU_DEFAULT_LOCAL_TASK,
  );
    
  // Remove "Manage questions" tab, but keep it existing in case
  $items['node/%node/questions']['type'] = MENU_CALLBACK;
  
  // Remove "Take" tab (already button)
  $items['node/%node/take']['type'] = MENU_CALLBACK;
}

/**
 * Implements hook_form_alter()
 */
function opigno_simple_ui_quiz_form_alter(&$form, &$form_state, $form_id) {  
  if ($form_id == 'quiz_questions_form') {
    $form['question_list']['browser']['#title'] = t("Browse existing questions to add them to this quiz");
    $form['question_list']['browser']['#collapsible'] = TRUE;
    $form['question_list']['browser']['#collapsed'] = TRUE;
  }
}
