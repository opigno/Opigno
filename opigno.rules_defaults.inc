<?php

/**
 * @file
 * Default rules.
 */

/**
 * Implements hook_default_rules_configuration().
 */
function opigno_default_rules_configuration() {
  $items = array();
  $items['rules_activate_group_membership'] = entity_import('rules_config', '{ "rules_activate_group_membership" : {
      "LABEL" : "Activate group membership",
      "PLUGIN" : "rule",
      "TAGS" : [ "og", "opigno" ],
      "REQUIRES" : [ "og", "opigno", "rules" ],
      "USES VARIABLES" : {
        "course" : { "label" : "Course", "type" : "node" },
        "user" : { "label" : "User", "type" : "user" }
      },
      "IF" : [
        { "og_entity_is_group" : { "entity" : [ "course" ] } },
        { "og_user_in_group" : {
            "account" : [ "user" ],
            "group" : [ "course" ],
            "states" : { "value" : { "2" : "2" } }
          }
        }
      ],
      "DO" : [
        { "opigno_load_og_membership" : {
            "USING" : { "user" : [ "user" ], "group" : [ "course" ] },
            "PROVIDE" : { "og_membership" : { "og_membership" : "OG Membership" } }
          }
        },
        { "data_set" : { "data" : [ "og-membership:state" ], "value" : "1" } }
      ]
    }
  }');
  return $items;
}
