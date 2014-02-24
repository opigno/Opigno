/**
 * @file
 * Defines the SCORM 2004 API object.
 *
 * This is the Opigno SCORM UI implementation of the SCORM API
 * object, used for communicating with the Opigno LMS.
 */

;(function($, Drupal, window, undefined) {

  /**
   * Implementation of the SCORM API.
   *
   * @constructor
   */
  var OpignoScormUI2004API = function() { };

  /**
   * Implements Initialize().
   *
   * @returns {String} 'true' or 'false'
   *        The SCO expects boolean values to be returned as strings.
   */
  OpignoScormUI2004API.prototype.Initialize = function() {
    console.log('Initialize');
    return 'true';
  }

  /**
   * Implements Terminate().
   *
   * @returns {String} 'true' or 'false'
   *        The SCO expects boolean values to be returned as strings.
   */
  OpignoScormUI2004API.prototype.Terminate = function() {
    console.log('Terminate');
    return 'true';
  }

  /**
   * Implements GetValue().
   *
   * @param {Object} cmiElement
   *
   * @returns {String}
   */
  OpignoScormUI2004API.prototype.GetValue = function(cmiElement) {
    console.log('GetValue', cmiElement);
  }

  /**
   * Implements SetValue().
   *
   * @param {Object} cmiElement
   * @param {String} value
   *
   * @return {String}
   */
  OpignoScormUI2004API.prototype.SetValue = function(cmiElement, value) {
    console.log('SetValue', cmiElement, value);
  }

  /**
   * Implements Commit().
   *
   * @returns {String} 'true' or 'false'
   *        The SCO expects boolean values to be returned as strings.
   */
  OpignoScormUI2004API.prototype.Commit = function() {
    console.log('Commit');
    return 'true';
  }

  /**
   * Implements GetLastError().
   *
   * @returns {String}
   */
  OpignoScormUI2004API.prototype.GetLastError = function() {
    console.log('GetLastError');
    return '0';
  }

  /**
   * Implements GetErrorString().
   *
   * @param {String} cmiErrorCode
   *
   * @return {String}
   */
  OpignoScormUI2004API.prototype.GetErrorString = function(cmiErrorCode) {
    console.log('GetErrorString', cmiErrorCode);
  }

  /**
   * Implements GetDiagnostic().
   *
   * @param {String} cmiErrorCode
   *
   * @return {String}
   */
  OpignoScormUI2004API.prototype.GetDiagnostic = function(cmiErrorCode) {
    console.log('GetDiagnostic', cmiErrorCode);
  }

  // Export.
  window.API_1484_11 = new OpignoScormUI2004API();

})(jQuery, Drupal, window);