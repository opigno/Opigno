<?php

//*
function opigno_rules_add_role_to_user($role, $user, EntityDrupalWrapper $group) {
  // Load the user we want to add to the group
  $all_roles = og_roles('node', $group->getBundle(), $group->getIdentifier(), TRUE, TRUE);
  foreach ($all_roles as $role_id => $a_role) {
    if ($role == $role_id) {
      og_role_grant('node', $group->getIdentifier(), $user->uid, $role_id);
    }
  }
}

function opigno_rules_revoke_role_to_user($role, $user, EntityDrupalWrapper $group) {
// Load the user we want to add to the group
  $all_roles = og_roles('node', $group->getBundle(), $group->getIdentifier(), TRUE, TRUE);
  foreach ($all_roles as $role_id => $a_role) {
    if ($role == $role_id) {
      og_role_revoke('node', $group->getIdentifier(), $user->uid, $role_id);
    }
  }
}

/*function opigno_rules_user_has_roles_options_list() {
  $all_bundles = og_get_all_group_bundle();
  foreach ($all_bundles['node'] as $bundle_machine_name => $bundle_name) {
    $all_roles = og_roles('node', $bundle_machine_name, 0, TRUE, TRUE);
    foreach ($all_roles as $role_id => $role) {
      $roles[$bundle_name][$role_id] = $role;
    }
  }
  return $roles;
}*/


function opigno_rules_user_has_roles($role, $group, $account) {
  $all_roles = og_get_user_roles('node', $group->nid, $account->uid, TRUE);

  foreach ($all_roles as $a_role => $a_rolename) {
    if ($role == $a_role) {
      return TRUE;
    }
  }
  return FALSE;
}

function opigno_rules_action_info() {
  $items = array();
  $items['opigno_load_og_membership'] = array(
    'label' => t("Load OG Membership"),
    'group' => t('Organic groups'),
    'parameter' => array(
      'user' => array(
        'type' => 'user',
        'label' => t('User'),
        'description' => t('The user who get the role'),
      ),
      'group' => array(
        'type' => array_keys(og_get_all_group_entity()),
        'label' => t('Group'),
        'wrapped' => TRUE,
      ),
    ),
    'provides' => array(
      'og_membership' => array(
        'type' => 'og_membership',
        'label' => t('OG Membership'),
      ),
    ),
    'base' => 'opigno_load_og_membership',
  );

  $items['og_revoke_og_role_to_user'] = array(
    'label' => t('Revoke user a og role'),
    'group' => t('Organic groups'),
    'parameter' => array(
      'role' => array(
        'type' => 'text',
        'label' => t('Role'),
        'description' => t('The role to be removed'),
        //'options list' => 'opigno_rules_user_has_roles_options_list',
      ),
      'user' => array(
        'type' => 'user',
        'label' => t('User'),
        'description' => t('The user who get the role'),
      ),
      'group' => array(
        'type' => array_keys(og_get_all_group_entity()),
        'label' => t('Group'),
        'wrapped' => TRUE,
      ),
      // @todo: Add membership-type setting + add in the membership-entity
      // fields via the info_alter callback + reload the form once the
      // membership-type has been chosen.
      // Then, we probably also want to provide the newly created membership
      // entity.
    ),
    'base' => 'opigno_rules_revoke_role_to_user',
    'access callback' => 'og_rules_integration_access',
  );

  $items['og_give_og_role_to_user'] = array(
    'label' => t('Give user a og role'),
    'group' => t('Organic groups'),
    'parameter' => array(
      'role' => array(
        'type' => 'text',
        'label' => t('Role'),
        'description' => t('The role to add'),
        //'options list' => 'opigno_rules_user_has_roles_options_list',
      ),
      'user' => array(
        'type' => 'user',
        'label' => t('User'),
        'description' => t('The user who get the role'),
      ),
      'group' => array(
        'type' => array_keys(og_get_all_group_entity()),
        'label' => t('Group'),
        'wrapped' => TRUE,
      ),
      // @todo: Add membership-type setting + add in the membership-entity
      // fields via the info_alter callback + reload the form once the
      // membership-type has been chosen.
      // Then, we probably also want to provide the newly created membership
      // entity.
    ),
    'base' => 'opigno_rules_add_role_to_user',
    'access callback' => 'og_rules_integration_access',
  );
  return $items;
}

/*function opigno_rules_condition_info() {
  $items = array();
  $items['og_user_has_role'] = array(
    'label' => t('User has role'),
    'group' => t('Organic groups'),
    'parameter' => array(
      'role' => array(
        'type' => 'text',
        'label' => t('Role'),
        'description' => t('The role to check for.'),
        'options list' => 'opigno_rules_user_has_roles_options_list',
        'restriction' => 'input',
      ),
      'group' => array(
        'type' => array_keys(og_get_all_group_entity()),
        'label' => t('Group'),
        'description' => t('The group for which permission should be checked.'),
      ),
      'account' => array(
        'type' => 'user',
        'label' => t('User'),
      ),
    ),
    'base' => 'opigno_rules_user_has_roles',
    'access callback' => 'og_rules_integration_access',
  );

  return $items;
}*/

function opigno_rules_event_info() {
  return array(
    'og_user_was_granted_role' => array(
      'variables' => array(
        'entity_type' => array(
          'type' => 'token',
          'label' => t("The type of the entity the user is getting role to"),
        ),
        'node_id' => array(
          'type' => 'integer',
          'label' => t('Node id'),
        ),
        'user_id' => array(
          'type' => 'integer',
          'label' => t('User id'),
        ),
        'role_id' => array(
          'type' => 'integer',
          'label' => t('Role id'),
        ),
      ),
      'group' => t('OG membership'),
      'label' => t('User has been granted a role'),
      'help' => t("A user has been granted a role"),
    ),
    'og_user_was_revoked_role' => array(
      'variables' => array(
        'entity_type' => array(
          'type' => 'token',
          'label' => t("The type of the entity the user is getting the role taken from"),
        ),
        'node_id' => array(
          'type' => 'integer',
          'label' => t('Node id'),
        ),
        'user_id' => array(
          'type' => 'integer',
          'label' => t('User id'),
        ),
        'role_id' => array(
          'type' => 'integer',
          'label' => t('Role id'),
        ),
      ),
      'group' => t('OG membership'),
      'label' => t('User has been revoked a role'),
      'help' => t("A user has been revoked a role"),
    ),
  );
}
//*/

function opigno_load_og_membership($user, EntityDrupalWrapper $group, $settings) {
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'og_membership', '=')
    ->propertyCondition('gid', $group->getIdentifier(), '=')
    ->propertyCondition('group_type', $group->type(), '=')
    ->propertyCondition('entity_type', 'user', '=')
    ->propertyCondition('etid', $user->uid, '=');
  $result = $query->execute();

  if (!empty($result['og_membership'])) {
    $og_membership = og_membership_load(current($result['og_membership'])->id);
  }
  else {
    $og_membership = NULL;
  }
  return array('og_membership' => $og_membership);
}
