<?php

/**
 * @file
 * Default rules.
 */

/**
 * Implements hook_default_rules_configuration().
 */
function opigno_og_prereq_default_rules_configuration() {
  $items = array();
  $items['rules_get_course_required_quizzes'] = entity_import('rules_config', '{ "rules_get_course_required_courses" : {
      "LABEL" : "Get course required courses",
      "PLUGIN" : "rule",
      "TAGS" : [ "og", "opigno" ],
      "REQUIRES" : [ "rules" ],
      "USES VARIABLES" : {
        "course" : { "label" : "Course", "type" : "node" },
        "required_courses" : {
          "label" : "Required courses",
          "type" : "list\u003Cnode\u003E",
          "parameter" : false
        }
      },
      "IF" : [
        { "entity_has_field" : { "entity" : [ "course" ], "field" : "course_required_course" } },
        { "NOT data_is_empty" : { "data" : [ "course:course-required-course" ] } }
      ],
      "DO" : [
        { "LOOP" : {
            "USING" : { "list" : [ "course:course-required-course" ] },
            "ITEM" : { "list_item" : "Current list item" },
            "DO" : [
              { "list_add" : {
                  "list" : [ "required-courses" ],
                  "item" : [ "list-item" ],
                  "unique" : 1
                }
              }
            ]
          }
        }
      ],
      "PROVIDES VARIABLES" : [ "required_courses" ]
    }
  }');
  return $items;
}












