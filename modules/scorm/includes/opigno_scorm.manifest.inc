<?php

/**
 * @file
 * Manifest file extraction logic.
 *
 * Contains functions for parsing the manifest XML, treating the data recursively and
 * preparing the data for storage. Only data that is relevant to a SCORM player is stored.
 * For example, resource information is not stored.
 */

// XML tag names
define('OPIGNO_SCORM_MANIFEST_METADATA',               'METADATA');
define('OPIGNO_SCORM_MANIFEST_ORGANIZATIONS',          'ORGANIZATIONS');
define('OPIGNO_SCORM_MANIFEST_ORGANIZATION',           'ORGANIZATION');
define('OPIGNO_SCORM_MANIFEST_RESOURCES',              'RESOURCES');
define('OPIGNO_SCORM_MANIFEST_RESOURCE',               'RESOURCE');
define('OPIGNO_SCORM_MANIFEST_TITLE',                  'TITLE');
define('OPIGNO_SCORM_MANIFEST_ITEM',                   'ITEM');
define('OPIGNO_SCORM_MANIFEST_SEQUENCING',             'IMSSS:SEQUENCING');
define('OPIGNO_SCORM_MANIFEST_CTRL_MODE',              'IMSSS:CONTROLMODE');
define('OPIGNO_SCORM_MANIFEST_OBJECTIVES',             'IMSSS:OBJECTIVES');
define('OPIGNO_SCORM_MANIFEST_OBJECTIVE',              'IMSSS:OBJECTIVE');
define('OPIGNO_SCORM_MANIFEST_PRIMARY_OBJECTIVE',      'IMSSS:PRIMARYOBJECTIVE');
define('OPIGNO_SCORM_MANIFEST_MIN_NORMALIZED_MEASURE', 'IMSSS:MINNORMALIZEDMEASURE');
define('OPIGNO_SCORM_MANIFEST_MAX_NORMALIZED_MEASURE', 'IMSSS:MAXNORMALIZEDMEASURE');

// XML attributes
define('OPIGNO_SCORM_MANIFEST_DEFAULT_ATTR',      'DEFAULT');
define('OPIGNO_SCORM_MANIFEST_ID_ATTR',           'IDENTIFIER');
define('OPIGNO_SCORM_MANIFEST_REFID_ATTR',        'IDENTIFIERREF');
define('OPIGNO_SCORM_MANIFEST_LAUNCH_ATTR',       'LAUNCH');
define('OPIGNO_SCORM_MANIFEST_PARAM_ATTR',        'PARAMETERS');
define('OPIGNO_SCORM_MANIFEST_TYPE_ATTR',         'TYPE');
define('OPIGNO_SCORM_MANIFEST_SCORM_TYPE_ATTR',   'ADLCP:SCORMTYPE');
define('OPIGNO_SCORM_MANIFEST_HREF_ATTR',         'HREF');
define('OPIGNO_SCORM_MANIFEST_MANIFEST_ATTR',     'IDENTIFIER');
define('OPIGNO_SCORM_MANIFEST_CHOICE_ATTR',       'CHOICE');
define('OPIGNO_SCORM_MANIFEST_FLOW_ATTR',         'FLOW');
define('OPIGNO_SCORM_MANIFEST_CHOICE_EXIT_ATTR',  'CHOICEEXIT');
define('OPIGNO_SCORM_MANIFEST_OBJECTIVE_ID_ATTR', 'OBJECTIVEID');
define('OPIGNO_SCORM_MANIFEST_OBJECTIVE_SATISFIED_BY_MEASURE_ATTR', 'SATISFIEDBYMEASURE');

/**
 * Extract the SCORM package from the file.
 *
 * Given a file ID (which should be a valid SCORM ZIP file), this function extracts
 * the ZIP to OPIGNO_SCORM_DIRECTORY, finds the manifest file and parses it.
 * Once parsed, it stores the manifest data.
 *
 * Returns the id of the extracted SCORM package, or FALSE if an error occurs.
 *
 * @param int $fid
 *        The file ID that we'll extract.
 *
 * @return int|false
 */
function opigno_scorm_extract($fid) {
  $file = file_load($fid);
  $path = drupal_realpath($file->uri);
  $zip = new ZipArchive();
  $result = $zip->open($path);
  if ($result === TRUE) {
    $extract_dir = OPIGNO_SCORM_DIRECTORY . '/scorm_' . $fid;
    $zip->extractTo($extract_dir);
    $zip->close();

    // This is a standard: the manifest file will always be here.
    $manifest_file = $extract_dir . '/imsmanifest.xml';

    if (file_exists($manifest_file)) {
      // Prepare the Scorm DB entry.
      $scorm = (object) array(
        'fid' => $fid,
        'extracted_dir' => $extract_dir,
        'manifest_file' => $manifest_file,
        'manifest_id' => '',
        'metadata' => '',
      );

      // Parse the manifest file and extract the data.
      $manifest_data = opigno_scorm_extract_manifest_data($manifest_file);

      // Get the manifest ID, if it's given.
      if (!empty($manifest_data['manifest_id'])) {
        $scorm->manifest_id = $manifest_data['manifest_id'];
      }

      // If the file contains (global) metadata, serialize it.
      if (!empty($manifest_data['metadata'])) {
        $scorm->metadata = serialize($manifest_data['metadata']);
      }

      // Try saving the SCORM to the DB.
      if (opigno_scorm_scorm_save($scorm)) {
        // Store each SCO.
        if (!empty($manifest_data['scos']['items'])) {
          foreach ($manifest_data['scos']['items'] as $i => $sco_item) {
            $sco = (object) array(
              'scorm_id' => $scorm->id,
              'organization' => $sco_item['organization'],
              'identifier' => $sco_item['identifier'],
              'parent_identifier' => $sco_item['parent_identifier'],
              'launch' => $sco_item['launch'],
              'type' => $sco_item['type'],
              'scorm_type' => $sco_item['scorm_type'],
              'title' => $sco_item['title'],
              'weight' => empty($sco_item['weight']) ? $sco_item['weight'] : 0,
              'attributes' => $sco_item['attributes'],
            );

            if (opigno_scorm_sco_save($sco)) {
              // @todo Store SCO attributes.
            }
            else {
              watchdog('opigno_scorm', "An error occured when saving an SCO.", array(), WATCHDOG_ERROR);
            }
          }
        }
        return TRUE;
      }
      else {
        watchdog('opigno_scorm', "An error occured when saving the SCORM package data.", array(), WATCHDOG_ERROR);
      }
    }
  }
  else {
    $error = 'none';
    switch ($result) {
      case ZipArchive::ER_EXISTS:
        $error = 'ER_EXISTS';
        break;

      case ZipArchive::ER_INCONS:
        $error = 'ER_INCONS';
        break;

      case ZipArchive::ER_INVAL:
        $error = 'ER_INVAL';
        break;

      case ZipArchive::ER_NOENT:
        $error = 'ER_NOENT';
        break;

      case ZipArchive::ER_NOZIP:
        $error = 'ER_NOZIP';
        break;

      case ZipArchive::ER_OPEN:
        $error = 'ER_OPEN';
        break;

      case ZipArchive::ER_READ:
        $error = 'ER_READ';
        break;

      case ZipArchive::ER_SEEK:
        $error = 'ER_SEEK';
        break;
    }
    watchdog('opigno_scorm', "An error occured when unziping the SCORM package data. Error: !error", array('!error' => $error), WATCHDOG_ERROR);
  }

  return FALSE;
}

/**
 * Extract the manifest data.
 *
 * Parse the manifest XML with XML2Array (located in includes/XML2Array.php)
 * and extract data that is relevant to us.
 *
 * @param string $manifest_file
 *
 * @return array
 */
function opigno_scorm_extract_manifest_data($manifest_file) {
  $data = array();

  // Get the XML as a string.
  $manifest_string = file_get_contents($manifest_file);

  // Parse it as an array.
  $parser = new XML2Array();
  $manifest = $parser->parse($manifest_string);

  // The parser returns an array of arrays - skip the first element.
  $manifest = array_shift($manifest);

  // Get the manifest ID, if any.
  if (!empty($manifest['attrs'][OPIGNO_SCORM_MANIFEST_MANIFEST_ATTR])) {
    $data['manifest_id'] = $manifest['attrs'][OPIGNO_SCORM_MANIFEST_MANIFEST_ATTR];
  }
  else {
    $data['manifest_id'] = '';
  }

  // Extract the global metadata information.
  $data['metadata'] = opigno_scorm_extract_manifest_metadata($manifest);

  // Extract the SCOs (course items). Gets the default SCO and a list of all SCOs.
  $data['scos'] = opigno_scorm_extract_manifest_scos($manifest);

  // Extract the resources, so we can combine the SCOs and resources.
  $data['resources'] = opigno_scorm_extract_manifest_resources($manifest);

  // Combine the resources and SCOs.
  $data['scos']['items'] = opigno_scorm_combine_manifest_sco_and_resources($data['scos']['items'], $data['resources']);

  return $data;
}

/**
 * Extract the manifest metadata.
 *
 * Find the metadata of this manifest file and return it.
 * We only treat global metadata - we don't parse metadata on SCOs or
 * resources.
 *
 * @param array $manifest
 *
 * @return array
 */
function opigno_scorm_extract_manifest_metadata($manifest) {
  foreach ($manifest['children'] as $child) {
    if ($child['name'] == OPIGNO_SCORM_MANIFEST_METADATA) {
      $meta = array();
      foreach ($child['children'] as $metadata) {
        $meta[strtolower($metadata['name'])] = $metadata['tagData'];
      }
      return $meta;
    }
  }
  return array();
}

/**
 * Extract the manifest SCO items.
 *
 * @see _opigno_scorm_extract_manifest_scos_items().
 *
 * @param array $manifest
 *
 * @return array
 *        'items' => array of SCOs
 *        'default' => default SCO identifier
 */
function opigno_scorm_extract_manifest_scos($manifest) {
  $items = array('items' => array());
  foreach ($manifest['children'] as $child) {
    if ($child['name'] == OPIGNO_SCORM_MANIFEST_ORGANIZATIONS) {
      if (!empty($child['attrs'][OPIGNO_SCORM_MANIFEST_DEFAULT_ATTR])) {
        $items['default'] = $child['attrs'][OPIGNO_SCORM_MANIFEST_DEFAULT_ATTR];
      }
      else {
        $items['default'] = '';
      }

      $items['items'] = array_merge(_opigno_scorm_extract_manifest_scos_items($child['children']), $items['items']);
    }
  }
  return $items;
}

/**
 * Helper function to recursively extract the manifest SCO items.
 *
 * The data is extracted as a flat array - it contains to hierarchy. Because of this,
 * the items are not extracted in logical order. However, each "level" is given a weight
 * which allows us to know how to organize them.
 *
 * @param array $manifest
 * @param string|int $parent_identifier = 0
 * @param string $organization = ''
 *
 * @return array
 */
function _opigno_scorm_extract_manifest_scos_items($manifest, $parent_identifier = 0, $organization = '') {
  $items = array();
  $weight = 0;

  foreach ($manifest as $item) {
    if (in_array($item['name'], array(OPIGNO_SCORM_MANIFEST_ORGANIZATION, OPIGNO_SCORM_MANIFEST_ITEM)) && !empty($item['children'])) {
      $attributes = array();
      if (!empty($item['attrs'][OPIGNO_SCORM_MANIFEST_ID_ATTR])) {
        $identifier = $item['attrs'][OPIGNO_SCORM_MANIFEST_ID_ATTR];
      }
      else {
        $identifier = uniqid();
      }

      if (!empty($item['attrs'][OPIGNO_SCORM_MANIFEST_LAUNCH_ATTR])) {
        $launch = $item['attrs'][OPIGNO_SCORM_MANIFEST_LAUNCH_ATTR];
      }
      else {
        $launch = '';
      }

      if (!empty($item['attrs'][OPIGNO_SCORM_MANIFEST_REFID_ATTR])) {
        $resource_identifier = $item['attrs'][OPIGNO_SCORM_MANIFEST_REFID_ATTR];
      }
      else {
        $resource_identifier = '';
      }

      if (!empty($item['attrs'][OPIGNO_SCORM_MANIFEST_PARAM_ATTR])) {
        $attributes['parameters'] = $item['attrs'][OPIGNO_SCORM_MANIFEST_PARAM_ATTR];
      }

      if (!empty($item['attrs'][OPIGNO_SCORM_MANIFEST_TYPE_ATTR])) {
        $type = $item['attrs'][OPIGNO_SCORM_MANIFEST_TYPE_ATTR];
      }
      else {
        $type = '';
      }

      if (!empty($item['attrs'][OPIGNO_SCORM_MANIFEST_SCORM_TYPE_ATTR])) {
        $scorm_type = $item['attrs'][OPIGNO_SCORM_MANIFEST_SCORM_TYPE_ATTR];
      }
      else {
        $scorm_type = '';
      }

      // Find the title, which is also a child node.
      foreach ($item['children'] as $child) {
        if ($child['name'] == OPIGNO_SCORM_MANIFEST_TITLE) {
          $title = $child['tagData'];
          break;
        }
      }

      // Find any sequencing control modes, which are also child nodes.
      $control_modes = array();
      foreach ($item['children'] as $child) {
        if ($child['name'] == OPIGNO_SCORM_MANIFEST_SEQUENCING) {
          $control_modes = opigno_scorm_extract_item_sequencing_control_modes($child);
          $attributes['objectives'] = opigno_scorm_extract_item_sequencing_objectives($child);
        }
      }

      // Failsafe - we cannot have elements without a title.
      if (empty($title)) {
        $title = 'NO TITLE';
      }

      $items[] = array(
        'manifest' => '', // @deprecated
        'organization' => $organization,
        'title' => $title,
        'identifier' => $identifier,
        'parent_identifier' => $parent_identifier,
        'launch' => $launch,
        'resource_identifier' => $resource_identifier,
        'type' => $type,
        'scorm_type' => $scorm_type,
        'weight' => $weight,
        'attributes' => $control_modes + $attributes,
      );

      // The first item is not an "item", but an "organization" node. This is the organization
      // for the remainder of the tree. Get it, and pass it along, so we know to which organization
      // the SCOs belong.
      if (empty($organization) && $item['name'] == OPIGNO_SCORM_MANIFEST_ORGANIZATION) {
        $organization = $identifier;
      }

      // Recursively get child items, and merge them to get a flat list.
      $items = array_merge(_opigno_scorm_extract_manifest_scos_items($item['children'], $identifier, $organization), $items);
    }
    $weight++;
  }

  return $items;
}

/**
 * Extract the manifest SCO item sequencing objective.
 *
 * This extracts sequencing objectives from an item. Objectives allow the system
 * to know how to "grade" the SCORM object.
 *
 * @param array $item_manifest
 *
 * @return array
 */
function opigno_scorm_extract_item_sequencing_objectives($item_manifest) {
  $objectives = array();
  foreach ($item_manifest['children'] as $child) {
    if ($child['name'] == OPIGNO_SCORM_MANIFEST_OBJECTIVES) {
      foreach ($child['children'] as $child_objective) {
        if (!empty($child_objective['attrs'][OPIGNO_SCORM_MANIFEST_OBJECTIVE_ID_ATTR])) {
          $id = $child_objective['attrs'][OPIGNO_SCORM_MANIFEST_OBJECTIVE_ID_ATTR];
        }
        else {
          $id = uniqid();
        }

        if ($child_objective['name'] == OPIGNO_SCORM_MANIFEST_PRIMARY_OBJECTIVE) {
          // Note: boolean attributes are stored as a strings. PHP does not know
          // how to cast 'false' to FALSE. Use string comparisons to bypass
          // this limitation by PHP. See below.
          $satisfied_by_measure = FALSE;
          if (!empty($child_objective['attrs'][OPIGNO_SCORM_MANIFEST_OBJECTIVE_SATISFIED_BY_MEASURE_ATTR])) {
            $satisfied_by_measure = strtolower($child_objective['attrs'][OPIGNO_SCORM_MANIFEST_OBJECTIVE_SATISFIED_BY_MEASURE_ATTR]) === 'true';
          }

          $objective = array(
            'primary' => TRUE,
            'secondary' => FALSE,
            'id' => $id,
            'satisfied_by_measure' => $satisfied_by_measure,
          );

          foreach ($child_objective['children'] as $primary_obj_child) {
            if ($primary_obj_child['name'] == OPIGNO_SCORM_MANIFEST_MIN_NORMALIZED_MEASURE) {
              $objective['min_normalized_measure'] = $primary_obj_child['tagData'];
            }
            elseif ($primary_obj_child['name'] == OPIGNO_SCORM_MANIFEST_MAX_NORMALIZED_MEASURE) {
              $objective['max_normalized_measure'] = $primary_obj_child['tagData'];
            }
          }

          $objectives[] = $objective;
        }
        elseif ($child_objective['name'] == OPIGNO_SCORM_MANIFEST_OBJECTIVE) {
          $objectives[] = array(
            'primary' => FALSE,
            'secondary' => TRUE,
            'id' => $id,
          );
        }
      }
    }
  }

  return $objectives;
}

/**
 * Extract the manifest SCO item sequencing control modes.
 *
 * This extracts sequencing control modes from an item. Control modes
 * describe how the user can navigate around the course
 * (e.g.: display the tree or not, skip SCOs, etc).
 *
 * @param array $item_manifest
 *
 * @return array
 */
function opigno_scorm_extract_item_sequencing_control_modes($item_manifest) {
  $defaults = array(
    'control_mode_choice' => TRUE,
    'control_mode_flow' => FALSE,
    'control_mode_choice_exit' => TRUE,
    'control_mode_forward_only' => FALSE,
  );

  $control_modes = array();

  foreach ($item_manifest['children'] as $child) {
    if ($child['name'] == OPIGNO_SCORM_MANIFEST_CTRL_MODE) {
      // Note: boolean attributes are stored as a strings. PHP does not know
      // how to cast 'false' to FALSE. Use string comparisons to bypass
      // this limitation by PHP. See below.

      if (!empty($child['attrs'][OPIGNO_SCORM_MANIFEST_CHOICE_ATTR])) {
        $control_modes['control_mode_choice'] = strtolower($child['attrs'][OPIGNO_SCORM_MANIFEST_CHOICE_ATTR]) === 'true';
      }

      if (!empty($child['attrs'][OPIGNO_SCORM_MANIFEST_FLOW_ATTR])) {
        $control_modes['control_mode_flow'] = strtolower($child['attrs'][OPIGNO_SCORM_MANIFEST_FLOW_ATTR]) === 'true';
      }

      if (!empty($child['attrs'][OPIGNO_SCORM_MANIFEST_CHOICE_EXIT_ATTR])) {
        $control_modes['control_mode_choice_exit'] = strtolower($child['attrs'][OPIGNO_SCORM_MANIFEST_CHOICE_EXIT_ATTR]) === 'true';
      }
    }
  }

  return $control_modes + $defaults;
}

/**
 * Extract the manifest SCO resources.
 *
 * We only extract resource information that is relevant to us. We don't care about
 * references files, dependencies, etc. Only about the href attribute, type and
 * identifier.
 *
 * @param array $manifest
 *
 * @return array
 */
function opigno_scorm_extract_manifest_resources($manifest) {
  $items = array();
  foreach ($manifest['children'] as $child) {
    if ($child['name'] == OPIGNO_SCORM_MANIFEST_RESOURCES) {
      foreach ($child['children'] as $resource) {
        if ($resource['name'] == OPIGNO_SCORM_MANIFEST_RESOURCE) {
          if (!empty($resource['attrs'][OPIGNO_SCORM_MANIFEST_ID_ATTR])) {
            $identifier = $resource['attrs'][OPIGNO_SCORM_MANIFEST_ID_ATTR];
          }
          else {
            $identifier = uniqid();
          }

          if (!empty($resource['attrs'][OPIGNO_SCORM_MANIFEST_HREF_ATTR])) {
            $href = $resource['attrs'][OPIGNO_SCORM_MANIFEST_HREF_ATTR];
          }
          else {
            $href = '';
          }

          if (!empty($resource['attrs'][OPIGNO_SCORM_MANIFEST_TYPE_ATTR])) {
            $type = $resource['attrs'][OPIGNO_SCORM_MANIFEST_TYPE_ATTR];
          }
          else {
            $type = '';
          }

          if (!empty($resource['attrs'][OPIGNO_SCORM_MANIFEST_SCORM_TYPE_ATTR])) {
            $scorm_type = $resource['attrs'][OPIGNO_SCORM_MANIFEST_SCORM_TYPE_ATTR];
          }
          else {
            $scorm_type = '';
          }

          $items[] = array(
            'identifier' => $identifier,
            'href' => $href,
            'type' => $type,
            'scorm_type' => $scorm_type,
          );
        }
      }
    }
  }
  return $items;
}

/**
 * Combine resources and SCO data.
 *
 * Update SCO data to include resource information (if necessary). Return the updated
 * SCO list.
 *
 * @param array $scos
 * @param array $resources
 *
 * @return array
 */
function opigno_scorm_combine_manifest_sco_and_resources($scos, $resources) {
  foreach ($scos as &$sco) {
    // If the SCO has a resource identifier ("identifierref"),
    // we need to combine them.
    if (!empty($sco['resource_identifier'])) {
      // Check all resources, and break when the correct one is found.
      foreach ($resources as $resource) {
        if (!empty($resource['identifier']) && $resource['identifier'] == $sco['resource_identifier']) {
          // If the SCO has no launch attribute, get the resource href.
          if (!empty($resource['href']) && empty($sco['launch'])) {
            $sco['launch'] = $resource['href'];
          }

          // Set the SCO type, if available.
          if (!empty($resource['type']) && empty($sco['type'])) {
            $sco['type'] = $resource['type'];
          }

          // Set the SCO scorm type, if available.
          if (!empty($resource['scorm_type']) && empty($sco['scorm_type'])) {
            $sco['scorm_type'] = $resource['scorm_type'];
          }
          break;
        }
      }
    }
  }
  return $scos;
}
