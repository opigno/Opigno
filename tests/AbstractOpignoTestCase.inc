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

class AbstractOpignoTestCase extends DrupalWebTestCase {
  protected $admin_user;

 /**
  *
  */
  protected function configureOpigno() {
    if (!isset($this->admin_user)) {
      // Create admin user
      $permissions = array();

      foreach (array('node', 'opigno', 'og', 'quiz') as $module) {
        $permissions = array_merge(array_keys(module_invoke($module, 'permission')), $permissions);
      }

      $this->admin_user = $this->drupalCreateUser($permissions);
    }

    // Login admin user
    $this->drupalLogin($this->admin_user);

    // Configure group types
    /* Done in features module
    foreach (array('course', 'workgroup') as $type) {
      $this->configureOGGroupType($type);
    }
    */

    // Configure group content types
    foreach (array('calendar_entry', 'glossary', 'quiz', 'wiki', 'workgroup') as $type) {
      $this->configureOGContentType($type);
    }

    // Logout admin user
    $this->drupalLogout($this->admin_user);
  }

  /**
   *
   */
  protected function configureOGGroupType($type) {
    $edit = array();

    $edit['og_group_type'] = 'group';

    $this->drupalPost('admin/structure/types/manage/' . str_replace('-', '_', $type), $edit, t("Save content type"));
  }

  /**
   *
   */
  protected function configureOGContentType($type) {
    $edit = array();

    $edit['og_group_content_type'] = 'og_content';

    $this->drupalPost('admin/structure/types/manage/' . str_replace('-', '_', $type), $edit, t("Save content type"));
  }

  /**
   *
   */
  protected function createOGNode($type, $user = NULL, $edit = array()) {
    if (!isset($user)) {
      $user = $this->admin_user;
    }

    // Login the node author
    $this->drupalLogin($user);

    // Set the form fields
    $base = array(
      'title' => $this->randomName(8),
      'body[und][0][value]' => $this->randomName(16),
    );
    $edit = $base + $edit;
    $this->drupalGet('node/add/' . str_replace('_', '-', $type));

    // Add node
    $this->drupalPost($this->getURL(), $edit, 'Save');

    // Get new node nid
    $matches = array();
    if (preg_match_all('/node\/([0-9]+)/', $this->getURL(), $matches)) {
      $nid = $matches[1];
      $node = node_load($nid);

      // Return group gid and nid
      return array($node->group_group[LANGUAGE_NONE][0]['value'], $nid);
    }
    else {
      $this->assertTrue(FALSE, 'Could not find the new node nid in the URL.');
    }
  }

  /**
   *
   */
  protected function createOGContent($type, $gid, $user = NULL, $edit = array()) {
    if (!isset($user)) {
      $user = $this->admin_user;
    }

    // Login the node author
    $this->drupalLogin($user);

    // Set the form fields
    $base = array(
      'title' => $this->randomName(8),
      'body[und][0][value]' => $this->randomName(16),
      'group_audience[und][]' => $gid
    );
    $edit = $base + $edit;
    $this->drupalGet('node/add/' . str_replace('_', '-', $type));

    // Add node
    $this->drupalPost($this->getURL(), $edit, 'Save');

    // Get new node nid
    $matches = array();
    if (preg_match_all('/node\/([0-9]+)/', $this->getURL(), $matches)) {
      $nid = $matches[1];

      // Return nid
      return $nid;
    }
    else {
      $this->assertTrue(FALSE, 'Could not find the new node nid in the URL.');

      return 0;
    }
  }
}
