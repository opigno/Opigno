<?php

/**
 * @file
 * Manifest file extraction logic.
 */

// XML tag names
define('OPIGNO_SCORM_MANIFEST_METADATA',      'METADATA');
define('OPIGNO_SCORM_MANIFEST_ORGANIZATIONS', 'ORGANIZATIONS');
define('OPIGNO_SCORM_MANIFEST_ORGANIZATION',  'ORGANIZATION');
define('OPIGNO_SCORM_MANIFEST_RESOURCES',     'RESOURCES');
define('OPIGNO_SCORM_MANIFEST_RESOURCE',      'RESOURCE');
define('OPIGNO_SCORM_MANIFEST_TITLE',         'TITLE');
define('OPIGNO_SCORM_MANIFEST_ITEM',          'ITEM');

// XML attributes
define('OPIGNO_SCORM_MANIFEST_DEFAULT_ATTR',    'DEFAULT');
define('OPIGNO_SCORM_MANIFEST_ID_ATTR',         'IDENTIFIER');
define('OPIGNO_SCORM_MANIFEST_REFID_ATTR',      'IDENTIFIERREF');
define('OPIGNO_SCORM_MANIFEST_LAUNCH_ATTR',     'LAUNCH');
define('OPIGNO_SCORM_MANIFEST_PARAM_ATTR',      'PARAMETERS');
define('OPIGNO_SCORM_MANIFEST_TYPE_ATTR',       'TYPE');
define('OPIGNO_SCORM_MANIFEST_SCORM_TYPE_ATTR', 'ADLCP:SCORMTYPE');
define('OPIGNO_SCORM_MANIFEST_HREF_ATTR',       'HREF');
define('OPIGNO_SCORM_MANIFEST_MANIFEST_ATTR',   'IDENTIFIER');

/**
 * Helper function to extract the SCORM package.
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

    $manifest_file = $extract_dir . '/imsmanifest.xml';

    if (file_exists($manifest_file)) {
      $scorm = (object) array(
        'fid' => $fid,
        'extracted_dir' => $extract_dir,
        'manifest_file' => $manifest_file,
        'manifest_id' => '',
        'metadata' => '',
      );

      $manifest_data = opigno_scorm_extract_manifest_data($manifest_file);

      if (!empty($manifest_data['manifest_id'])) {
        $scorm->manifest_id = $manifest_data['manifest_id'];
      }
      if (!empty($manifest_data['metadata'])) {
        $scorm->metadata = serialize($manifest_data['metadata']);
      }

      if (opigno_scorm_scorm_save($scorm)) {
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
    $error = '';
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
    dpm($error, 'ZipArchive');
  }

  return FALSE;
}

/**
 * Extract the manifest data.
 *
 * @param string $manifest_file
 *
 * @return array
 */
function opigno_scorm_extract_manifest_data($manifest_file) {
  $data = array();

  $manifest_string = file_get_contents($manifest_file);
  $parser = new XML2Array();
  $manifest = $parser->parse($manifest_string);
  $manifest = array_shift($manifest);
  //dpm($manifest, 'XML');

  if (!empty($manifest['attrs'][OPIGNO_SCORM_MANIFEST_MANIFEST_ATTR])) {
    $data['manifest_id'] = $manifest['attrs'][OPIGNO_SCORM_MANIFEST_MANIFEST_ATTR];
  }
  else {
    $data['manifest_id'] = '';
  }

  $data['metadata'] = opigno_scorm_extract_manifest_metadata($manifest);
  $data['scos'] = opigno_scorm_extract_manifest_scos($manifest);
  $data['resources'] = opigno_scorm_extract_manifest_resources($manifest);
  $data['scos']['items'] = opigno_scorm_combine_manifest_sco_and_resources($data['scos']['items'], $data['resources']);

  //dpm($data, 'extracted data');
  return $data;
}

/**
 * Extract the manifest metadata.
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
 * @param array $manifest
 *
 * @return array
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
        $parameters = $item['attrs'][OPIGNO_SCORM_MANIFEST_PARAM_ATTR];
      }
      else {
        $parameters = '';
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

      foreach ($item['children'] as $child) {
        if ($child['name'] == OPIGNO_SCORM_MANIFEST_TITLE) {
          $title = $child['tagData'];
          break;
        }
      }

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
        'parameters' => $parameters,
        'type' => $type,
        'scorm_type' => $scorm_type,
        'weight' => $weight,
      );

      if (empty($organization) && $item['name'] == OPIGNO_SCORM_MANIFEST_ORGANIZATION) {
        $organization = $identifier;
      }

      $items = array_merge(_opigno_scorm_extract_manifest_scos_items($item['children'], $identifier, $organization), $items);
    }
    $weight++;
  }

  return $items;
}

/**
 * Extract the manifest SCO resources.
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
 * Combine resources and SCO data. Update SCO data to include resource information (if necessary).
 *
 * @param array $scos
 * @param array $resources
 *
 * @return array
 */
function opigno_scorm_combine_manifest_sco_and_resources($scos, $resources) {
  foreach ($scos as &$sco) {
    if (!empty($sco['resource_identifier'])) {
      foreach ($resources as $resource) {
        if (!empty($resource['identifier']) && $resource['identifier'] == $sco['resource_identifier']) {
          if (!empty($resource['href']) && empty($sco['launch'])) {
            $sco['launch'] = $resource['href'];
          }

          if (!empty($resource['type']) && empty($sco['type'])) {
            $sco['type'] = $resource['type'];
          }

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
