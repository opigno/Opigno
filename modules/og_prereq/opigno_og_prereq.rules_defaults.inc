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
  $items['rules_course_has_required_courses'] = entity_import('rules_config', '{ "rules_course_has_required_courses" : {
      "LABEL" : "Course has required courses",
      "PLUGIN" : "action set",
      "TAGS" : [ "og", "opigno" ],
      "REQUIRES" : [ "rules", "rules_conditional" ],
      "USES VARIABLES" : {
        "course" : { "label" : "Course", "type" : "node" },
        "has_required_courses" : {
          "label" : "Has required courses",
          "type" : "boolean",
          "parameter" : false
        }
      },
      "ACTION SET" : [
        { "component_rules_get_course_required_courses" : {
            "USING" : { "course" : [ "course" ] },
            "PROVIDE" : { "required_courses" : { "required_courses" : "Required courses" } }
          }
        },
        { "CONDITIONAL" : [
            {
              "IF" : { "data_is_empty" : { "data" : [ "required-courses" ] } },
              "DO" : [ { "data_set" : { "data" : [ "has-required-courses" ], "value" : 0 } } ]
            },
            { "ELSE" : [ { "data_set" : { "data" : [ "has-required-courses" ], "value" : 1 } } ] }
          ]
        }
      ],
      "PROVIDES VARIABLES" : [ "has_required_courses" ]
    }
  }');
  $items['rules_course_has_required_courses_cond_ruleset'] = entity_import('rules_config', '{ "rules_course_has_required_courses_cond_ruleset" : {
      "LABEL" : "Course has required courses",
      "PLUGIN" : "rule condition set",
      "TAGS" : [ "og", "opigno" ],
      "REQUIRES" : [ "rules_conditional", "rules" ],
      "USES VARIABLES" : {
        "course" : { "label" : "Course", "type" : "node" },
        "has_required_courses" : { "label" : "Course has required courses", "type" : "boolean" }
      },
      "DO" : [
        { "component_rules_course_has_required_courses" : {
            "USING" : { "course" : [ "course" ] },
            "PROVIDE" : { "has_required_courses" : { "has_req" : "Has required courses" } }
          }
        },
        { "data_set" : { "data" : [ "has-required-courses" ], "value" : [ "has-req" ] } }
      ],
      "RESULT" : [  ]
    }
  }');
  if (module_exists('opigno_quiz_app')) {
    foreach (_opgino_og_prereq_install_opigno_quiz_app_rules() as $key => $rule) {
      $items[$key] = entity_import('rules_config', $rule);
    }
  }
  return $items;
}












