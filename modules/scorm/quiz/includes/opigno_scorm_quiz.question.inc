<?php

/**
 * @file
 * Question type class definition.
 */

class OpignoScormQuizQuestion extends QuizQuestion {

  /**
   * @copydoc QuizQuestion::validateNode()
   */
  public function validateNode(array &$form) {
    // No validation required
  }

  /**
   * @copydoc QuizQuestion::getCreationForm()
   */
  public function getCreationForm(array &$form_state = NULL) {
    // No special fields.
    return array();
  }

  /**
   * @copydoc QuizQuestion::getAnsweringForm()
   */
  public function getAnsweringForm(array $form_state = NULL, $rid) {
    return array(
      'tries' => array(
        '#type' => 'hidden',
        '#default_value' => 1,
      ),
    );
  }

  /**
   * @copydoc QuizQuestion::getMaximumScore()
   */
  public function getMaximumScore() {
    return variable_get('opigno_scorm_quiz_max_score', 50);
  }

  /**
   * @copydoc QuizQuestion::saveNodeProperties()
   */
  public function saveNodeProperties($is_new = FALSE) {
    // No properties to save.
  }

}
