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
}