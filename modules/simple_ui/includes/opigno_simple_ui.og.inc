<?php

/**
 * @file
 * OG specific enhancements.
 */

/**
 * Implements hook_menu().
 */
function opigno_simple_ui_og_menu() {
  return array(
    'node/%/group/remove/%' => array(
      'title' => "Remove member",
      'page callback' => 'drupal_get_form',
      'page arguments' => array('opigno_simple_ui_remove_member_confirm', 1, 4),
      'access arguments' => array(0, 1, 'manage members'),
      'access callback' => 'og_user_access',
      'file' => 'includes/opigno_simple_ui.og.inc',
      'type' => MENU_CALLBACK,
    ),
  );
}

/**
 * Implements hook_form_og_ui_edit_membership_alter().
 */
function opigno_simple_ui_form_og_ui_edit_membership_alter(&$form, $form_state) {
  $og_membership = og_membership_load($form['id']['#value']);
  $form['og_user']['state'] = array(
    '#type' => 'radios',
    '#title' => t("State"),
    '#options' => og_group_content_states(),
    '#default_value' => $og_membership->state,
  );

  $form['actions']['remove'] = array(
    '#type' => 'submit',
    '#value' => t("Remove user from group"),
    '#submit' => array('opigno_simple_ui_form_og_ui_edit_membership_remove_from_group_submit'),
  );

  // We need to set the state *before* changing the roles, because some Rules or some Opigno apps
  // can change the status based on permissions automatically. And many react on role change.
  $form['#submit'] = array_merge(array('opigno_simple_ui_form_og_ui_edit_membership_submit'), $form['#submit']);
}

/**
 * Submit callback for hook_form_og_ui_edit_membership_alter().
 * Update membership state.
 */
function opigno_simple_ui_form_og_ui_edit_membership_submit($form, $form_state) {
  $form_state['og_membership']->state = $form_state['values']['state'];
  og_membership_save($form_state['og_membership']);
}

/**
 * Submit callback for hook_form_og_ui_edit_membership_alter().
 * Remove a user from the group. Redirect to confirmation form.
 */
function opigno_simple_ui_form_og_ui_edit_membership_remove_from_group_submit($form, &$form_state) {
  unset($_GET['destination']);
  drupal_goto("node/{$form_state['values']['gid']}/group/remove/{$form_state['values']['id']}");
}

/**
 * Implements hook_form_og_ui_add_users_alter().
 */
function opigno_simple_ui_form_og_ui_add_users_alter(&$form, $form_state) {
  $form['og_user']['state'] = array(
    '#type' => 'radios',
    '#title' => t("State"),
    '#options' => og_group_content_states(),
    '#default_value' => OG_STATE_ACTIVE,
  );
}

/**
 * Implements hook_form_og_massadd_massadd_form_alter().
 */
function opigno_simple_ui_form_og_massadd_massadd_form_alter(&$form, $form_state) {
  $gid = current($form['group_ids']['#value']);
  $node = node_load($gid);

  $form['massadd'] = array(
    '#type' => 'multiselect',
    '#title' => t('Select students to add to the group'),
    '#default_value' => NULL,
    '#size' => 15,
    '#required' => TRUE,
  );

  // Exclude current members.
  $exclude = array();
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'og_membership', '=')
    ->propertyCondition('gid', $gid, '=')
    ->propertyCondition('entity_type', 'user', '=');
  $result = $query->execute();
  if (!empty($result['og_membership'])) {
    foreach ($result['og_membership'] as $id => $info) {
      $og_membership = og_membership_load($id);
      $exclude[] = $og_membership->etid;
    }
  }

  $users = entity_load('user');
  foreach ($users as $uid => $account) {
    if ($uid && !in_array($uid, $exclude)) {
      $form['massadd']['#options'][$account->uid] = $account->name;
    }
  }

  $form['massadd']['#weight'] = -50;

  $form['roles'] = array(
    '#type' => 'checkboxes',
    '#title' => t("Roles"),
    '#options' => og_roles($form['group_type']['#value'], $node->type, $gid, FALSE, FALSE),
    '#default_value' => OG_STATE_ACTIVE,
    '#weight' => -40,
  );

  $form['state'] = array(
    '#type' => 'radios',
    '#title' => t("State"),
    '#options' => og_group_content_states(),
    '#default_value' => OG_STATE_ACTIVE,
    '#weight' => -30,
  );

  $form['submit']['#weight'] = 0;
  // We validate our own way.
  unset($form['#validate']);
  $form['#submit'][] = 'opigno_simple_ui_form_og_massadd_massadd_form_add_roles';
  $form['#submit'][] = 'opigno_simple_ui_form_og_massadd_massadd_form_add_state';
  $form['#submit'] = array_merge(array('opigno_simple_ui_form_og_massadd_massadd_form_prepare'), $form['#submit']);
}

/**
 * Submit callback for hook_form_og_massadd_massadd_form_alter().
 * Prepare for submission by changing the format of the field value.
 */
function opigno_simple_ui_form_og_massadd_massadd_form_prepare($form, &$form_state) {
  $csv = '';
  foreach ($form_state['values']['massadd'] as $uid) {
    $csv .= $form['massadd']['#options'][$uid] . "\n";
  }
  $form_state['values']['_massadd'] = $form_state['values']['massadd'];
  $form_state['values']['massadd'] = $csv;
}

/**
 * Submit callback for hook_form_og_massadd_massadd_form_alter().
 * Add roles.
 */
function opigno_simple_ui_form_og_massadd_massadd_form_add_roles($form, $form_state) {
  foreach ($form_state['values']['_massadd'] as $uid) {
    foreach ($form_state['values']['roles'] as $rid => $selected) {
      if ($selected) {
        og_role_grant($form_state['values']['group_type'], current($form_state['values']['group_ids']), $uid, $rid);
      }
    }
  }
}

/**
 * Submit callback for hook_form_og_massadd_massadd_form_alter().
 * Add state.
 */
function opigno_simple_ui_form_og_massadd_massadd_form_add_state($form, $form_state) {
  foreach ($form_state['values']['_massadd'] as $uid) {
    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', 'og_membership', '=')
      ->propertyCondition('gid', current($form_state['values']['group_ids']), '=')
      ->propertyCondition('entity_type', 'user', '=')
      ->propertyCondition('etid', $uid, '=');
    $result = $query->execute();

    $og_membership = og_membership_load(current($result['og_membership'])->id);
    $og_membership->state = $form_state['values']['state'];
    og_membership_save($og_membership);
  }
}

/**
 * Confirmation form for deleting an OG membership.
 */
function opigno_simple_ui_remove_member_confirm($form, $form_state, $gid, $og_membership_id) {
  $og_membership = og_membership_load($og_membership_id);
  $entity = current(entity_load($og_membership->entity_type, array($og_membership->etid)));
  $form['og_membership_id'] = array(
    '#type' => 'value',
    '#value' => $og_membership_id,
  );

  $form['gid'] = array(
    '#type' => 'value',
    '#value' => $gid,
  );

  // @todo - we process the entity as a user (hence $entity->name), but this is a bit restrictive.
  $form['entity'] = array(
    '#type' => 'value',
    '#value' => $entity->name,
  );
  return confirm_form($form, t("Are you sure you want to remove %entity from this group ?", array('%entity' => $entity->name)), "node/$gid/group");
}

/**
 * Submit callback for deleting an OG membership.
 */
function opigno_simple_ui_remove_member_confirm_submit($form, &$form_state) {
  og_membership_delete($form_state['values']['og_membership_id']);
  drupal_set_message(t("%entity was removed from the group", array('%entity' => $form_state['values']['entity'])));
  $form_state['redirect'] = "node/{$form_state['values']['gid']}/group";
}