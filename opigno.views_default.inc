<?php

/**
 * @file
 * Default views.
 */

/**
 * Implements hook_views_default_views().
 */
function opigno_views_default_views() {
  $export = array();

  /**
   * My courses
   */
  $view = new view();
  $view->name = 'opigno_my_courses';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'og_membership';
  $view->human_name = 'My courses';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'My courses';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'role';
  $handler->display->display_options['access']['role'] = array(
    2 => '2',
  );
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['disable_sql_rewrite'] = TRUE;
  $handler->display->display_options['query']['options']['distinct'] = TRUE;
  $handler->display->display_options['query']['options']['pure_distinct'] = TRUE;
  $handler->display->display_options['exposed_form']['type'] = 'better_exposed_filters';
  $handler->display->display_options['exposed_form']['options']['autosubmit'] = TRUE;
  $handler->display->display_options['exposed_form']['options']['bef'] = array(
    'general' => array(
      'allow_secondary' => 0,
      'secondary_label' => 'Advanced options',
    ),
    'opigno_course_categories_tid' => array(
      'bef_format' => 'default',
      'more_options' => array(
        'bef_select_all_none' => FALSE,
        'bef_collapsible' => 0,
        'is_secondary' => 0,
        'any_label' => '',
        'bef_filter_description' => '',
        'tokens' => array(
          'available' => array(
            0 => 'global_types',
          ),
        ),
        'rewrite' => array(
          'filter_rewrite_values' => '',
        ),
      ),
    ),
  );
  $handler->display->display_options['exposed_form']['options']['input_required'] = 0;
  $handler->display->display_options['exposed_form']['options']['text_input_required_format'] = 'html';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '30';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['style_options']['grouping'] = array(
    0 => array(
      'field' => 'title_1',
      'rendered' => 1,
      'rendered_strip' => 0,
    ),
  );
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Relationship: OG membership: Group Node from OG membership */
  $handler->display->display_options['relationships']['og_membership_related_node_group']['id'] = 'og_membership_related_node_group';
  $handler->display->display_options['relationships']['og_membership_related_node_group']['table'] = 'og_membership';
  $handler->display->display_options['relationships']['og_membership_related_node_group']['field'] = 'og_membership_related_node_group';
  /* Relationship: Entity Reference: Referencing entity */
  $handler->display->display_options['relationships']['reverse_opigno_class_courses_node']['id'] = 'reverse_opigno_class_courses_node';
  $handler->display->display_options['relationships']['reverse_opigno_class_courses_node']['table'] = 'node';
  $handler->display->display_options['relationships']['reverse_opigno_class_courses_node']['field'] = 'reverse_opigno_class_courses_node';
  $handler->display->display_options['relationships']['reverse_opigno_class_courses_node']['relationship'] = 'og_membership_related_node_group';
  $handler->display->display_options['relationships']['reverse_opigno_class_courses_node']['label'] = 'Class referencing the course';


  /* Field: Content: course_image */
  $handler->display->display_options['fields']['opigno_course_image']['id'] = 'opigno_course_image';
  $handler->display->display_options['fields']['opigno_course_image']['table'] = 'field_data_opigno_course_image';
  $handler->display->display_options['fields']['opigno_course_image']['field'] = 'opigno_course_image';
  $handler->display->display_options['fields']['opigno_course_image']['relationship'] = 'og_membership_related_node_group';
  $handler->display->display_options['fields']['opigno_course_image']['label'] = '';
  $handler->display->display_options['fields']['opigno_course_image']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['opigno_course_image']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['opigno_course_image']['settings'] = array(
    'image_style' => 'thumbnail',
    'image_link' => '',
  );

  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['relationship'] = 'og_membership_related_node_group';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;

  /* Field: Content: Description */
  $handler->display->display_options['fields']['body']['id'] = 'body';
  $handler->display->display_options['fields']['body']['table'] = 'field_data_body';
  $handler->display->display_options['fields']['body']['field'] = 'body';
  $handler->display->display_options['fields']['body']['relationship'] = 'og_membership_related_node_group';
  $handler->display->display_options['fields']['body']['label'] = 'Description';
  $handler->display->display_options['fields']['body']['type'] = 'text_summary_or_trimmed';
  $handler->display->display_options['fields']['body']['settings'] = array(
    'trim_length' => '600',
  );

  /* Field: Global: PHP */
  $handler->display->display_options['fields']['php_teachers']['id'] = 'php_teachers';
  $handler->display->display_options['fields']['php_teachers']['table'] = 'views';
  $handler->display->display_options['fields']['php_teachers']['field'] = 'php';
  $handler->display->display_options['fields']['php_teachers']['label'] = 'Teachers';
  $handler->display->display_options['fields']['php_teachers']['hide_empty'] = TRUE;
  $handler->display->display_options['fields']['php_teachers']['use_php_setup'] = 0;
  $handler->display->display_options['fields']['php_teachers']['php_output'] = '<?php print opigno_get_teacher_html($data->node_og_membership_nid);
?>';
  $handler->display->display_options['fields']['php_teachers']['use_php_click_sortable'] = '0';
  $handler->display->display_options['fields']['php_teachers']['php_click_sortable'] = '';

  /* Field: OG membership: Created */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'og_membership';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['label'] = 'Member since';
  $handler->display->display_options['fields']['created']['date_format'] = 'raw time ago';
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['relationship'] = 'og_membership_related_node_group';
  $handler->display->display_options['fields']['nid']['label'] = '';
  $handler->display->display_options['fields']['nid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nid']['element_label_colon'] = FALSE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title_1']['id'] = 'title_1';
  $handler->display->display_options['fields']['title_1']['table'] = 'node';
  $handler->display->display_options['fields']['title_1']['field'] = 'title';
  $handler->display->display_options['fields']['title_1']['relationship'] = 'reverse_opigno_class_courses_node';
  $handler->display->display_options['fields']['title_1']['label'] = '';
  $handler->display->display_options['fields']['title_1']['exclude'] = TRUE;
  $handler->display->display_options['fields']['title_1']['element_label_colon'] = FALSE;
  /* Contextual filter: OG membership: Entity id */
  $handler->display->display_options['arguments']['etid']['id'] = 'etid';
  $handler->display->display_options['arguments']['etid']['table'] = 'og_membership';
  $handler->display->display_options['arguments']['etid']['field'] = 'etid';
  $handler->display->display_options['arguments']['etid']['default_action'] = 'default';
  $handler->display->display_options['arguments']['etid']['default_argument_type'] = 'current_user';
  $handler->display->display_options['arguments']['etid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['etid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['etid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['etid']['specify_validation'] = TRUE;
  $handler->display->display_options['arguments']['etid']['validate']['type'] = 'user';
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['relationship'] = 'og_membership_related_node_group';
  $handler->display->display_options['filters']['type']['value'] = array(
    'course' => 'course',
  );
  /* Filter criterion: OG membership: State */
  $handler->display->display_options['filters']['state']['id'] = 'state';
  $handler->display->display_options['filters']['state']['table'] = 'og_membership';
  $handler->display->display_options['filters']['state']['field'] = 'state';
  $handler->display->display_options['filters']['state']['value'] = array(
    1 => '1',
  );
  /* Filter criterion: Content: Course categories (opigno_course_categories) */
  $handler->display->display_options['filters']['opigno_course_categories_tid']['id'] = 'opigno_course_categories_tid';
  $handler->display->display_options['filters']['opigno_course_categories_tid']['table'] = 'field_data_opigno_course_categories';
  $handler->display->display_options['filters']['opigno_course_categories_tid']['field'] = 'opigno_course_categories_tid';
  $handler->display->display_options['filters']['opigno_course_categories_tid']['relationship'] = 'og_membership_related_node_group';
  $handler->display->display_options['filters']['opigno_course_categories_tid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['opigno_course_categories_tid']['expose']['operator_id'] = 'opigno_course_categories_tid_op';
  $handler->display->display_options['filters']['opigno_course_categories_tid']['expose']['label'] = 'Course category';
  $handler->display->display_options['filters']['opigno_course_categories_tid']['expose']['operator'] = 'opigno_course_categories_tid_op';
  $handler->display->display_options['filters']['opigno_course_categories_tid']['expose']['identifier'] = 'opigno_course_categories_tid';
  $handler->display->display_options['filters']['opigno_course_categories_tid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
  );
  $handler->display->display_options['filters']['opigno_course_categories_tid']['type'] = 'select';
  $handler->display->display_options['filters']['opigno_course_categories_tid']['vocabulary'] = 'course_categories';
  /* Filter criterion: OG membership: Entity_type */
  $handler->display->display_options['filters']['entity_type']['id'] = 'entity_type';
  $handler->display->display_options['filters']['entity_type']['table'] = 'og_membership';
  $handler->display->display_options['filters']['entity_type']['field'] = 'entity_type';
  $handler->display->display_options['filters']['entity_type']['value'] = 'user';

  /* Display: My courses */
  $handler = $view->new_display('page', 'My courses', 'page_my_courses');
  $handler->display->display_options['path'] = 'my-courses';

  /* Display: Entity Reference */
  $handler = $view->new_display('entityreference', 'Entity Reference', 'entityreference');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'entityreference_style';
  $handler->display->display_options['style_options']['search_fields'] = array(
    'nid' => 'nid',
  );
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'entityreference_fields';
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $translatables['opigno_my_courses'] = array(
    t('Master'),
    t('My courses'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('Items per page'),
    t('- All -'),
    t('Offset'),
    t('« first'),
    t('‹ previous'),
    t('next ›'),
    t('last »'),
    t('Group node from OG membership'),
    t('Class referencing the course'),
    t('Title'),
    t('Member since'),
    t('All'),
    t('Course category'),
    t('Entity Reference'),
  );



  $export['opigno_my_courses'] = $view;

  /**
   * Course catalogue
   */
  $view = new view();
  $view->name = 'opigno_course_catalgue';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Training catalogue';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Training catalogue';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'role';
  $handler->display->display_options['access']['role'] = array(
    2 => '2',
  );
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['disable_sql_rewrite'] = TRUE;
  $handler->display->display_options['query']['options']['distinct'] = TRUE;
  $handler->display->display_options['query']['options']['query_comment'] = 'exclude_own_groups';
  $handler->display->display_options['exposed_form']['type'] = 'better_exposed_filters';
  $handler->display->display_options['exposed_form']['options']['autosubmit'] = TRUE;
  $handler->display->display_options['exposed_form']['options']['bef'] = array(
    'general' => array(
      'allow_secondary' => 0,
      'secondary_label' => 'Advanced options',
    ),
    'opigno_course_categories_tid' => array(
      'bef_format' => 'default',
      'more_options' => array(
        'bef_select_all_none' => FALSE,
        'bef_collapsible' => 0,
        'is_secondary' => 0,
        'any_label' => '',
        'bef_filter_description' => '',
        'tokens' => array(
          'available' => array(
            0 => 'global_types',
          ),
        ),
        'rewrite' => array(
          'filter_rewrite_values' => '',
        ),
      ),
    ),
  );
  $handler->display->display_options['exposed_form']['options']['input_required'] = 0;
  $handler->display->display_options['exposed_form']['options']['text_input_required_format'] = 'html';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '30';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['style_options']['grouping'] = array(
    0 => array(
      'field' => 'group_group_1',
      'rendered' => 1,
      'rendered_strip' => 0,
    ),
  );
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Relationship: Entity Reference: Referencing entity */
  $handler->display->display_options['relationships']['reverse_opigno_class_courses_node']['id'] = 'reverse_opigno_class_courses_node';
  $handler->display->display_options['relationships']['reverse_opigno_class_courses_node']['table'] = 'node';
  $handler->display->display_options['relationships']['reverse_opigno_class_courses_node']['field'] = 'reverse_opigno_class_courses_node';
  $handler->display->display_options['relationships']['reverse_opigno_class_courses_node']['relationship'] = 'og_membership_related_node_group';
  $handler->display->display_options['relationships']['reverse_opigno_class_courses_node']['label'] = 'Class referencing the course';

  /* Field: Content: Group */
  $handler->display->display_options['fields']['group_group']['id'] = 'group_group';
  $handler->display->display_options['fields']['group_group']['table'] = 'field_data_group_group';
  $handler->display->display_options['fields']['group_group']['field'] = 'group_group';
  $handler->display->display_options['fields']['group_group']['label'] = '';
  $handler->display->display_options['fields']['group_group']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['group_group']['type'] = 'og_group_subscribe';
  $handler->display->display_options['fields']['group_group']['settings'] = array(
    'field_name' => '0',
  );

  /* Field: Content: course_image */
  $handler->display->display_options['fields']['opigno_course_image']['id'] = 'opigno_course_image';
  $handler->display->display_options['fields']['opigno_course_image']['table'] = 'field_data_opigno_course_image';
  $handler->display->display_options['fields']['opigno_course_image']['field'] = 'opigno_course_image';
  $handler->display->display_options['fields']['opigno_course_image']['label'] = '';
  $handler->display->display_options['fields']['opigno_course_image']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['opigno_course_image']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['opigno_course_image']['settings'] = array(
    'image_style' => 'thumbnail',
    'image_link' => '',
  );
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;

  /* Field: Content: Body */
  $handler->display->display_options['fields']['body']['id'] = 'body';
  $handler->display->display_options['fields']['body']['table'] = 'field_data_body';
  $handler->display->display_options['fields']['body']['field'] = 'body';
  $handler->display->display_options['fields']['body']['label'] = 'Description';
  $handler->display->display_options['fields']['body']['type'] = 'text_summary_or_trimmed';
  $handler->display->display_options['fields']['body']['settings'] = array(
    'trim_length' => '600',
  );

  if (module_exists("opigno_course_quota_app"))
  {
    /* Field: Global: PHP */
    $handler->display->display_options['fields']['php_available_places']['id'] = 'php_available_places';
    $handler->display->display_options['fields']['php_available_places']['table'] = 'views';
    $handler->display->display_options['fields']['php_available_places']['field'] = 'php';
    $handler->display->display_options['fields']['php_available_places']['label'] = 'Available places';
    $handler->display->display_options['fields']['php_available_places']['hide_empty'] = TRUE;
    $handler->display->display_options['fields']['php_available_places']['use_php_setup'] = 0;
    $handler->display->display_options['fields']['php_available_places']['php_output'] = '<?php
$node=node_load($data->nid);
$info=opigno_course_quota_app_students_info($node);
if ((isset($info[\'places\']))&&($info[\'places\']!=-1))
{
 print $info[\'available_places\'];
}
?>';
    $handler->display->display_options['fields']['php_available_places']['use_php_click_sortable'] = '0';
    $handler->display->display_options['fields']['php_available_places']['php_click_sortable'] = '';
  }

  /* Field: Global: PHP Teachers */
  $handler->display->display_options['fields']['php_course_teachers']['id'] = 'php_course_teachers';
  $handler->display->display_options['fields']['php_course_teachers']['table'] = 'views';
  $handler->display->display_options['fields']['php_course_teachers']['field'] = 'php';
  $handler->display->display_options['fields']['php_course_teachers']['label'] = 'Teachers';
  $handler->display->display_options['fields']['php_course_teachers']['hide_empty'] = TRUE;
  $handler->display->display_options['fields']['php_course_teachers']['use_php_setup'] = 0;
  $handler->display->display_options['fields']['php_course_teachers']['php_output'] = '<?php
print opigno_get_teacher_html($data->nid);
?>';
  $handler->display->display_options['fields']['php_course_teachers']['use_php_click_sortable'] = '0';
  $handler->display->display_options['fields']['php_course_teachers']['php_click_sortable'] = '';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title_1']['id'] = 'title_1';
  $handler->display->display_options['fields']['title_1']['table'] = 'node';
  $handler->display->display_options['fields']['title_1']['field'] = 'title';
  $handler->display->display_options['fields']['title_1']['relationship'] = 'reverse_opigno_class_courses_node';
  $handler->display->display_options['fields']['title_1']['label'] = '';
  $handler->display->display_options['fields']['title_1']['exclude'] = TRUE;
  $handler->display->display_options['fields']['title_1']['element_label_colon'] = FALSE;

  if (module_exists("opigno_in_house_training_app"))
  {
    /* Field: Global: PHP */
    $handler->display->display_options['fields']['php_in_house_trainings']['id'] = 'php_in_house_trainings';
    $handler->display->display_options['fields']['php_in_house_trainings']['table'] = 'views';
    $handler->display->display_options['fields']['php_in_house_trainings']['field'] = 'php';
    $handler->display->display_options['fields']['php_in_house_trainings']['label'] = 'In house training';
    $handler->display->display_options['fields']['php_in_house_trainings']['hide_empty'] = TRUE;
    $handler->display->display_options['fields']['php_in_house_trainings']['use_php_setup'] = 0;
    $handler->display->display_options['fields']['php_in_house_trainings']['php_output'] = '<?php
print opigno_in_house_training_app_get_all_iht_unrestricted_view($data->nid);
?>';
    $handler->display->display_options['fields']['php_in_house_trainings']['use_php_click_sortable'] = '0';
    $handler->display->display_options['fields']['php_in_house_trainings']['php_click_sortable'] = '';
  }

  if (module_exists("opigno_webex_app"))
  {
    /* Field: Global: PHP */
    $handler->display->display_options['fields']['php_webex']['id'] = 'php_webex';
    $handler->display->display_options['fields']['php_webex']['table'] = 'views';
    $handler->display->display_options['fields']['php_webex']['field'] = 'php';
    $handler->display->display_options['fields']['php_webex']['label'] = 'Webex meetings';
    $handler->display->display_options['fields']['php_webex']['hide_empty'] = TRUE;
    $handler->display->display_options['fields']['php_webex']['use_php_setup'] = 0;
    $handler->display->display_options['fields']['php_webex']['php_output'] = '<?php
print opigno_webex_app_get_all_webex_unrestricted_view($data->nid);
?>';
    $handler->display->display_options['fields']['php_webex']['use_php_click_sortable'] = '0';
    $handler->display->display_options['fields']['php_webex']['php_click_sortable'] = '';
  }

  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'course' => 'course',
  );
  /* Filter criterion: Content: Nid */
  $handler->display->display_options['filters']['nid']['id'] = 'nid';
  $handler->display->display_options['filters']['nid']['table'] = 'node';
  $handler->display->display_options['filters']['nid']['field'] = 'nid';
  $handler->display->display_options['filters']['nid']['value']['value'] = '0';
  /* Filter criterion: Content: Course categories (opigno_course_categories) */
  $handler->display->display_options['filters']['opigno_course_categories_tid']['id'] = 'opigno_course_categories_tid';
  $handler->display->display_options['filters']['opigno_course_categories_tid']['table'] = 'field_data_opigno_course_categories';
  $handler->display->display_options['filters']['opigno_course_categories_tid']['field'] = 'opigno_course_categories_tid';
  $handler->display->display_options['filters']['opigno_course_categories_tid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['opigno_course_categories_tid']['expose']['operator_id'] = 'opigno_course_categories_tid_op';
  $handler->display->display_options['filters']['opigno_course_categories_tid']['expose']['label'] = 'Course category';
  $handler->display->display_options['filters']['opigno_course_categories_tid']['expose']['operator'] = 'opigno_course_categories_tid_op';
  $handler->display->display_options['filters']['opigno_course_categories_tid']['expose']['identifier'] = 'opigno_course_categories_tid';
  $handler->display->display_options['filters']['opigno_course_categories_tid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
  );
  $handler->display->display_options['filters']['opigno_course_categories_tid']['type'] = 'select';
  $handler->display->display_options['filters']['opigno_course_categories_tid']['vocabulary'] = 'course_categories';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'course-catalogue';
  $translatables['opigno_course_catalgue'] = array(
    t('Master'),
    t('Training catalogue'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('Items per page'),
    t('- All -'),
    t('Offset'),
    t('« first'),
    t('‹ previous'),
    t('next ›'),
    t('last »'),
    t('Class referencing the course'),
    t('Title'),
    t('Subscribe'),
    t('Course category'),
    t('Page'),
  );

  $export['opigno_course_catalgue'] = $view;

  return $export;
}
