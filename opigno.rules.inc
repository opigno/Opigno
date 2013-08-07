<?php

function opigno_rules_action_info() {
  return array(
    'opigno_rules_add_og_role_to_user' => array(
      'label' => t('Set user as role'),
      'group' => t('OG membership'),
      'arguments' => array(
        'user' => array(
          'type' => 'user',
          'label' => t('user'),),
        'node' => array(
          'type' => 'node',
          'label' => t('group'),),),
    ),
  );
}

function opigno_rules_add_og_role_to_user($user,$node) {
// Load the user we want to add to the group
  og_role_grant('node',$node->nid, $user->uid, 5);
}