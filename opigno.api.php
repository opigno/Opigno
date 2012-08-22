<?php

/**
 * @file
 * Defines the Opigno LMS specific API and hooks
 */

/**
 * Implements hook_opigno_widget_info()
 *
 * Defines widgets that can be used on the OG homepage dashboard.
 * Must return an array identical to that of hook_block_info()
 *
 * @return array
 */
function hook_opigno_widgets_info() {
  return array(
    'og_tools' => array(
      'info' => t("Opigno: OG tools"),
      'cache' => DRUPAL_CACHE_PER_PAGE
    ),
    'add_og_content' => array(
      'info' => t("Opigno: Add OG content"),
      'cache' => DRUPAL_CACHE_PER_USER
    )
  );
}