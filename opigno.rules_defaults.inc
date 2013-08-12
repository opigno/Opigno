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
      "REQUIRES" : [ "og", "rules", "rules_conditional" ],
      "USES VARIABLES" : {
        "course" : { "label" : "Course", "type" : "node" },
        "user" : { "label" : "User", "type" : "user" }
      },
      "IF" : [
        { "og_entity_is_group" : { "entity" : [ "course" ] } },
        { "og_entity_in_group" : { "entity" : [ "user" ], "group" : [ "course" ] } }
      ],
      "DO" : [
        { "entity_query" : {
            "USING" : {
              "type" : "og_membership",
              "property" : "entity",
              "value" : [ "user" ],
              "limit" : "100"
            },
            "PROVIDE" : { "entity_fetched" : { "entity_fetched" : "Fetched entity" } }
          }
        },
        { "LOOP" : {
            "USING" : { "list" : [ "entity-fetched" ] },
            "ITEM" : { "list_item" : "Current list item" },
            "DO" : [
              { "CONDITIONAL" : [
                  {
                    "IF" : { "data_is" : { "data" : [ "list-item:group" ], "value" : [ "course" ] } },
                    "DO" : [
                      { "data_set" : { "data" : [ "list-item:state" ], "value" : "1" } },
                      { "drupal_message" : { "message" : "Your membership for the next courses has been approved." } }
                    ]
                  }
                ]
              }
            ]
          }
        }
      ]
    }
  }');
  return $items;
}