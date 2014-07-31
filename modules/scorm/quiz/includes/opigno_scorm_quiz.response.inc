<?php

/**
 * @file
 * Response type class definition.
 */

class OpignoScormQuizResponse extends QuizQuestionResponse {

  /**
   * @copydoc QuizQuestionResponse::save()
   */
  public function save() {
    global $user;

    if (!empty($this->question->opigno_scorm_package[LANGUAGE_NONE][0]['fid'])) {
      $scorm = opigno_scorm_scorm_load_by_fid($this->question->opigno_scorm_package[LANGUAGE_NONE][0]['fid']);

      // We get the latest result. The way the SCORM API works always overwrites attempts
      // for the global CMI storage. The result stored is always the latest. Get it,
      // and presist it again in the user results table so we can track results through
      // time.
      module_load_include('module', 'opigno_scorm_ui');
      $scaled = opigno_scorm_ui_scorm_cmi_get($user->uid, $scorm->id, 'cmi.score.scaled', '');
    }

    $completion=opigno_scorm_ui_scorm_cmi_get($user->uid, $scorm->id, 'cmi.completion_status', '');
    $raw=opigno_scorm_ui_scorm_cmi_get($user->uid, $scorm->id, 'cmi.score.raw', '');
    $max=opigno_scorm_ui_scorm_cmi_get($user->uid, $scorm->id, 'cmi.score.max', '');
    $success_status=opigno_scorm_ui_scorm_cmi_get($user->uid, $scorm->id, 'cmi.success_status', '');
	

    if (empty($completion)) {
      $scaled = 0;
    }
    if (($completion == "completed") && (empty($raw)) && (!is_numeric($raw))) {
      $scaled = 1;
    }
    if (($completion == "incomplete") && (empty($raw)) && (!is_numeric($raw))) {
      $scaled = 0;
    }

    // Something went wrong. Set a score of -1.
    if (!isset($scaled) || !is_numeric($scaled)) {
      $scaled = -1;
    }

    db_insert('opigno_scorm_quiz_user_results')
      ->fields(array(
        'question_nid' => $this->question->nid,
        'question_vid' => $this->question->vid,
        'result_id' => $this->rid,
        'score_scaled' => $scaled,
      ))
      ->execute();
  }

  /**
   * @copydoc QuizQuestionResponse::delete()
   */
  public function delete() {
    db_delete('opigno_scorm_quiz_user_results')
      ->condition('question_nid', $this->question->nid)
      ->condition('question_vid', $this->question->vid)
      ->condition('result_id', $this->rid)
      ->execute();
  }

  /**
   * @copydoc QuizQuestionResponse::delete()
   */
  public function score() {
    $scaled = db_select('opigno_scorm_quiz_user_results', 'o')
      ->fields('o', array('score_scaled'))
      ->condition('question_nid', $this->question->nid)
      ->condition('question_vid', $this->question->vid)
      ->condition('result_id', $this->rid)
      ->execute()
      ->fetchField();

    $round_method = !empty($this->question->opigno_scorm_round_method[LANGUAGE_NONE][0]['value']) ? $this->question->opigno_scorm_round_method[LANGUAGE_NONE][0]['value'] : 'round';

    $scaled *= $this->getMaxScore();

    switch ($round_method) {
      case 'ceil':
        $scaled = ceil($scaled);
        break;

      case 'floor':
        $scaled = floor($scaled);
        break;
      
      default:
        $scaled = round($scaled);
        break;
    }

    return $scaled;
  }

  /**
   * @copydoc QuizQuestionResponse::getResponse()
   */
  public function getResponse() {
    return 'In SCORM package';
  }

  /**
   * @copydoc QuizQuestionResponse::getReportFormResponse()
   */
  /*public function getReportFormResponse($showpoints = TRUE, $showfeedback = TRUE, $allow_scoring = FALSE) {
    if (empty($this->question->answers)) {
      return array(
        '#markup' => t('Missing question.'),
      );
    }
    

    // @todo ......
    return array(
      '#markup' => 'SCORE',
    );
  }*/

  /*public function getReportFormScore($showfeedback = TRUE, $showpoints = TRUE, $allow_scoring = FALSE) {
    return array(
      '#markup' => $this->score(),
    );
  }*/

  /**
   * Implementation of getReportForm
   *
   * @see QuizQuestionResponse#getReportForm($showpoints, $showfeedback, $allow_scoring)
   */
  public function getReportForm($showpoints = TRUE, $showfeedback = TRUE, $allow_scoring = FALSE) {
    return array(
      '#no_report' => TRUE,
    );
  }
	

  public function getReportFormAnswerFeedback($showpoints = TRUE, $showfeedback = TRUE, $allow_scoring = FALSE) {
    return FALSE;
  }

}
