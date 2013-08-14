<?php

function og_rules_add_role_to_user($role,$user,EntityDrupalWrapper $group) {
// Load the user we want to add to the group
  $all_roles=og_roles('node',$group->getBundle(),0,TRUE,TRUE);
  foreach($all_roles as $role_id => $a_role)
  {
    if ($role==$a_role)
    {
      og_role_grant('node',$group->getIdentifier(), $user->uid, $role_id);
    }
  }
}

function og_rules_revoke_role_to_user($role,$user,EntityDrupalWrapper $group) {
// Load the user we want to add to the group
  $all_roles=og_roles('node',$group->getBundle(),0,TRUE,TRUE);
  foreach($all_roles as $role_id => $a_role)
  {
    if ($role==$a_role)
    {
      og_role_revoke('node',$group->getIdentifier(), $user->uid, $role_id);
    }
  }
}

function og_rules_user_has_roles_options_list() {
  $all_bundles=og_get_all_group_bundle();
  foreach($all_bundles['node'] as $bundle_machine_name => $bundle_name)
  {
    $all_roles=og_roles('node',$bundle_machine_name,0,TRUE,TRUE);
    foreach($all_roles as $role)
    {
      $roles[$role]=$role;
    }
  }
  return $roles;
}


function opigno_rules_action_info() {
  $items = array();
  $items['og_get_members'] = array(
    'label' => t('Get group members from group audience'),
    'group' => t('Organic groups'),
    'parameter' => array(
      'group_content' => array(
        'type' => 'entity',
        'label' => t('Group content'),
        'description' => t('The group content determining the group audience.'),
      ),
      // @todo: Add option to filter by member-state?
    ),
    'provides' => array(
      'group_members' => array('type' => 'list<user>', 'label' => t('List of group members')),
    ),
    'base' => 'og_rules_get_members',
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
        'options list' => 'og_rules_user_has_roles_options_list',),
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
    'base' => 'og_rules_add_role_to_user',
    'access callback' => 'og_rules_integration_access',
  );
  return $items;
}

function opigno_rules_condition_info()
{
  $items = array();
$items['og_user_has_role'] = array(
  'label' => t('User has role'),
  'group' => t('Organic groups'),
  'parameter' => array(
    'role' => array(
      'type' => 'text',
      'label' => t('Role'),
      'description' => t('The role to check for.'),
      'options list' => 'og_rules_user_has_roles_options_list',
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
  'base' => 'og_rules_user_has_roles',
  'access callback' => 'og_rules_integration_access',
);

return $items;
}

function og_og_role_grant($type,$node_id,$user_id,$role_id)
{
  dpm($type);
  dpm($node_id);
  dpm($user_id);
  dpm($role_id);
  if (($type=="node")&&(module_exists('rules')))
  {
    rules_invoke_event('og_user_was_granted_role', $type, $node_id,$user_id,$role_id);
  }
}

function og_og_role_revoke($type,$node_id,$user_id,$role_id)
{
  dpm($type);
  dpm($node_id);
  dpm($user_id);
  dpm($role_id);
  if (($type=="node")&&(module_exists('rules'))&&($role_id!=7)&&($role_id!=8))
  {
    rules_invoke_event('og_user_was_revoked_role', $type, $node_id,$user_id,$role_id);
  }
}