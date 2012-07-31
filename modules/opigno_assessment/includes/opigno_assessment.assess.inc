<?php

/**
 * @file
 * This file contains all assess functions and logic
 */

/**
 *
 */
function opigno_assessment_assess_course($gid, $uid) {
  $node = node_load($gid);
  
  if (!isset($node->opigno_assessment)) {
    return;
  }
  
  if ($node->opigno_assessment['option'] !== 'no') {
    $nids = array();
    
    // Get quizzes to assess
    if ($node->opigno_assessment['option'] === 'some') {
      foreach ($node->opigno_assessment['quizzes'] as $nid => $value) {
        if ($value) {
          $nids[] = $nid;
        }
      }
    }
    else {
      $query = db_select('node', 'n');
      
      $query->leftJoin('og_membership', 'om', 'om.etid = n.nid');
      
      $query->leftJoin('og', 'o', 'o.gid = om.gid');
      
      $result = $query->fields('n', array('nid'))
                      ->condition('n.type', 'quiz')
                      ->condition('om.state', 1)
                      ->condition('o.entity_type', 'node')
                      ->condition('o.gid', $gid)
                      ->execute();
      
      while ($nid = $result->fetchField(0)) {
        $nids[] = $nid;
      }
    }
    
    // Check each quiz result
    $results = array();
    
    foreach ($nids as $nid) {
      $result = db_select('quiz_node_results', 'r')
                  ->fields('r', array('score'))
                  ->condition('r.uid', $uid)
                  ->condition('r.nid', $nid)
                  ->orderBy('r.result_id', 'DESC')
                  ->range(0,1)
                  ->execute()
                  ->fetchField(0);
      
      if (isset($result)) {
        $results[$nid] = $result;
      }
      else {
        // No result ! No certificate !
        return;
      }
    }
    
    $total = 0;
    
    $divide = 0;
    
    $min = 100;
    
    foreach ($results as $nid => $result) {
      $min = min($min, $result);
      
      $options = variable_get("opigno_assessment_nid_$nid", NULL);

      $coef = isset($options['ponderation']) ? $options['ponderation'] : 1;
      
      $total += ($result * $coef);
      
      $divide += $coef;
    }
    
    if ($total && $divide) {
      $average = $total / $divide;
    }
    
    // Get average in regular scale
    $average = (variable_get('opigno_assessment_grade_scale', 10) * $average) / 100;
    
    // Get min in regular scale
    $min = (variable_get('opigno_assessment_grade_scale', 10) * $min) / 100;
    
    if ($average >= $node->opigno_assessment['average']) {
      if ($node->opigno_assessment['rule'] == 'average_neg' && $node->opigno_assessment['min'] > $min) {
        drupal_set_message(t("You completed the course. However, you have a grade that's lower than the minimum allowed. Lowest allowed: @settings_min, your lowest: !min", array('@settings_min' => $node->opigno_assessment['min'], '!min' => $min)), 'warning');
      }
      else {
        drupal_set_message(t("You have successfuly finished the course ! Your grade: !average / @max", array('!average' => $average, '@max' => variable_get('opigno_assessment_grade_scale', 10))));
      }
    }
    else {
      drupal_set_message(t("You completed the course. However, your average grade is lower than the minimum allowed. Minimum to pass: @settings_average, your grade: !average", array('@settings_average' => $node->opigno_assessment['average'], '!average' => $average)), 'warning');
    }
  }
}