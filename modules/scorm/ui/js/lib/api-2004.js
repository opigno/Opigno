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
   */
  OpignoScormUI2004API.prototype.Initialize = function() {
    console.log('Initialize:', arguments);
  }

  /**
   * Implements Finish().
   */
  OpignoScormUI2004API.prototype.Finish = function() {
    console.log('Finish', arguments);
  }

  /**
   * Implements GetValue().
   */
  OpignoScormUI2004API.prototype.GetValue = function() {
    console.log('GetValue', arguments);
  }

  /**
   * Implements SetValue().
   */
  OpignoScormUI2004API.prototype.SetValue = function() {
    console.log('SetValue', arguments);
  }

  /**
   * Implements Commit().
   */
  OpignoScormUI2004API.prototype.Commit = function() {
    console.log('Commit', arguments);
  }

  /**
   * Implements Terminate().
   */
  OpignoScormUI2004API.prototype.Terminate = function() {
    console.log('Terminate', arguments);
  }

  /**
   * Implements GetLastError().
   */
  OpignoScormUI2004API.prototype.GetLastError = function() {
    console.log('GetLastError', arguments);
    return '0';
  }

  /**
   * Implements GetErrorString().
   */
  OpignoScormUI2004API.prototype.GetErrorString = function() {
    console.log('GetErrorString', arguments);
  }

  /**
   * Implements GetDiagnostic().
   */
  OpignoScormUI2004API.prototype.GetDiagnostic = function() {
    console.log('GetDiagnostic', arguments);
  }

  // Export.
  window.API_1484_11 = new OpignoScormUI2004API();

})(jQuery, Drupal, window);