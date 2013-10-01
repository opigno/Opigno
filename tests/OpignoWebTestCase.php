<?php

/**
 * @file
 * Defines the base class for Opigno unit testing.
 * This base class contains re-usable logic that will make it easier and faster to
 * write Opigno-specific unit tests.
 * This class will also configure all modules to work correctly for Opigno.
 *
 * @todo - default settings should be done on install of features !
 */

class OpignoWebTestCase extends DrupalWebTestCase {

  /**
   * Create a course and assign members to it.
   *
   * @param  string $title = NULL
   * @param  object $creator = NULL
   * @param  array $members = array()
   *          A 2-dimensional array, where the key is the user ID and the value an
   *          array of roles.
   *          Ex:
   *            array(12 => array('manager', 'teacher'))
   *
   * @return object
   */
  protected function createCourse($title = NULL, $creator = NULL, $members = array()) {
    $settings = array(
      'type' => OPIGNO_COURSE_BUNDLE,
      'title' => $title ? $title : $this->randomName(8),
      'body' => array(
        LANGUAGE_NONE => array(
          array('value' => $this->randomName(16)),
        ),
      ),
    );
    $node = $this->drupalCreateNode($settings);

    $this->assertTrue(!empty($node->nid), 'Created a new course.');

    if (!empty($members)) {
      foreach ($members as $uid => $roles) {
        og_membership_create('node', $node->nid, 'user', $uid, 'og_user_node');
        foreach ($roles as $role) {
          $rid = $this->getRoleId($role);
          if (!empty($rid)) {
            og_role_grant('node', $node->nid, $uid, $rid);
          }
          else {
            $this->fail("Could not find the role '$role'.");
          }
        }
      }
    }
  }

  /**
   * Fetch the role ID by name.
   *
   * @param  string $role_name
   *
   * @return int
   */
  protected function getRoleId($role_name) {
    $rid  = db_select('og_role', 'r')
              ->fields('r', array('rid'))
              ->condition('r.name', $role_name)
              ->condition('group_bundle', OPIGNO_COURSE_BUNDLE)
              ->execute()
              ->fetchField();
    return !empty($rid) ? $rid : 0;
  }
}
