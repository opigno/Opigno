<?php

/**
 * @file
 * Defines the XML2Array class, for easy XML manipulation.
 * Based on http://ch2.php.net/xml_parse#52567.
 */

class XML2Array {

  private $output = array();
  private $xml_parser;
  private $xml_string;

  public function parse($xlm_string) {
    $this->xml_parser = xml_parser_create();
    xml_set_object($this->xml_parser, $this);
    xml_set_element_handler($this->xml_parser, "tagOpen", "tagClosed");

    xml_set_character_data_handler($this->xml_parser, "tagData");

    $this->xml_string = xml_parse($this->xml_parser, $xlm_string);
    if (!$this->xml_string) {
      watchdog('opigno_scorm', sprintf(
        "XML error: %s at line %d",
        xml_error_string(xml_get_error_code($this->xml_parser)),
        xml_get_current_line_number($this->xml_parser)
      ), array(), WATCHDOG_ERROR);
    }

    xml_parser_free($this->xml_parser);

    return $this->output;
  }

  public function tagOpen($parser, $name, $attrs) {
    $tag = array("name" => $name, "attrs" => $attrs);
    array_push($this->output, $tag);
  }

  public function tagData($parser, $tagData) {
    if (trim($tagData)) {
      if (isset($this->output[count($this->output) - 1]['tagData'])) {
        $this->output[count($this->output) - 1]['tagData'] .= $tagData;
      }
      else {
        $this->output[count($this->output) - 1]['tagData'] = $tagData;
      }
    }
  }

  public function tagClosed($parser, $name) {
    $this->output[count($this->output) - 2]['children'][] = $this->output[count($this->output) - 1];
    array_pop($this->output);
  }
}