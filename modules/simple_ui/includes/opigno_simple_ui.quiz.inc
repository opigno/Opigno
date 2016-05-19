<?php

/**
 * @file
 * Quiz specific enhancements.
 */

/**
 * Implements hook_theme_registry_alter().
 */
function opigno_simple_ui_quiz_theme_registry_alter(&$registry) {
  $registry['quiz_question_navigation_form']['function'] = 'theme_opigno_simple_ui_quiz_question_navigation_form';
}

/**
 * Implements hook_menu_local_tasks_alter().
 */
function opigno_simple_ui_quiz_menu_local_tasks_alter(&$data, $router_item, $root_path) {
  if ($root_path == 'node/%/quizzes') {
    if (!empty($data['actions']['output'])) {
      foreach ($data['actions']['output'] as $key => $theme_array) {
        if (!empty($theme_array['#link']['path'])) {
          if ($theme_array['#link']['path'] == 'node/add/quiz') {
            $data['actions']['output'][$key]['#link']['title'] = t("Add new lesson");
          }
          elseif ($theme_array['#link']['path'] == 'node/%/sort-quizzes') {
            $data['actions']['output'][$key]['#link']['title'] = t("Sort lessons");
          }
        }
      }
    }
  }
}

/**
 * Implements hook_form_form_alter() for all question types.
 */
function opigno_simple_ui_question_typesform_alter(&$form, $form_state) {  
  // Remove the "Add to Quiz" options. This is for very advanced users, and rarely
  // used.
  unset($form['add_directly']['#group']);
  $form['add_directly']['#access'] = FALSE;
  $form['add_directly']['#attributes']['style'][] = 'display:none;';

  // Change the order of the fields. Put the title on top, always.
  $form['title']['#weight'] = -30;
}

/**
 * Implements hook_form_quiz_report_form_alter().
 */
function opigno_simple_ui_form_quiz_report_form_alter(&$form, $form_state) {
  $no_report = TRUE;
  drupal_set_title(t("Lesson completed."));
  foreach ($form as $key => $value) {
    if (is_numeric($key)) {
      if (!$form[$key]['#no_report']) {
        $no_report = FALSE;
      }
    }
  }

  if ($no_report) {
    $form = array('#markup' => t("There are no results to display."));
    drupal_set_message(t("You successfully completed this lesson."));
  }
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

      $form['additional_questions']['#attributes']['class'][] = 'element-hidden';
    }
  }
}

/**
 * Implements hook_form_quiz_node_form_alter().
 */
function opigno_simple_ui_form_quiz_node_form_alter(&$form, $form_state) {
  foreach ($form['resultoptions'] as $key => &$child) {
    if (!empty($child['#type']) && $child['#type'] == 'fieldset') {
      $child['#collapsed'] = TRUE;
    }
  }
}

/**
 * Theme Quiz navigation.
 */
function theme_opigno_simple_ui_quiz_question_navigation_form($vars) {
  $form = $vars['form'];
  if (!isset($form['#last'])) {
    return drupal_render_children($form);
  }
  else {
    $submit = drupal_render($form['submit']) . drupal_render($form['op']);
    return drupal_render_children($form) . $submit;
  }
}

/**
 * Update the quiz fields and settings and name them "Lesson".
 */
function opigno_simple_ui_update_quiz_labels() {
  variable_set('quiz_name', t("Lesson"));

  $instance = field_info_instance('node', 'quiz_weight', 'quiz');
  if (!empty($instance)) {
    $instance['label'] = "Lesson weight";
    field_update_instance($instance);
  }

  $instance = field_info_instance('node', 'quiz_type', 'quiz');
  if (!empty($instance)) {
    $instance['label'] = "Lesson type";
    $instance['description'] = "Sets the type of this lesson. Can be <em>theory</em> (not shown in results by default), <em>quiz</em> (always shown in results) and <em>mix</em>.";
    field_update_instance($instance);
  }

  $instance = field_info_instance('node', 'body', 'quiz');
  if (!empty($instance)) {
    $instance['label'] = "Introduction";
    field_update_instance($instance);
  }

  foreach (array(
    'long_answer' => array(
      'body' => -1,
      'og_group_ref' => 1,
    ),
    'matching' => array(
      'body' => -5,
      'og_group_ref' => 1,
    ),
    'multichoice' => array(
      'body' => -5,
      'og_group_ref' => 1,
    ),
    'quizfileupload' => array(
      'body' => -4,
    ),
    'quiz_ddlines' => array(
      'body' => -4,
    ),
    'quiz_directions' => array(
      'body' => -4,
    ),
    'scale' => array(
      'body' => -4,
    ),
    'short_answer' => array(
      'body' => -5,
    ),
    'truefalse' => array(
      'body' => -5,
    ),
  ) as $type => $fields) {
    foreach ($fields as $field => $weight) {
      $instance = field_read_instance('node', $field, $type);
      if (!empty($instance)) {
        $instance['widget']['weight'] = $weight;
        field_update_instance($instance);
      }
    }
  }

  $settings = variable_get('field_bundle_settings_node__multichoice', NULL);
  if ($settings) {
    $settings['extra_fields']['form']['title']['weight'] = -10;
    variable_set('field_bundle_settings_node__multichoice', $settings);
  }
  
  /*
  array('long-answer', 'quizfileupload', 'quiz-directions');
  foreach (array_keys(_quiz_question_get_implementations()) as $type) {
    $instance = field_read_instance('node', 'body', $type);
    if (!empty($instance)) {
      $instance['widget']['weight'] = -6;
      field_update_instance($instance);
    }
  }
  */

  $instance = field_info_instance('node', 'course_required_quiz_ref', OPIGNO_COURSE_BUNDLE);
  if (!empty($instance)) {
    $instance['label'] = "Lesson required for the course validation";
    field_update_instance($instance);
  }
}
