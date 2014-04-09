/**
 * @file
 * Defines the SCORM 1.2 API object.
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
  var OpignoScorm12API = function() { };

  /**
   * Implements LMSInitialize().
   */
  OpignoScorm12API.prototype.LMSInitialize = function() {
    console.log('LMSInitialize');
  }

  /**
   * Implements LMSFinish().
   */
  OpignoScorm12API.prototype.LMSFinish = function() {
    console.log('LMSFinish');
  }

  /**
   * Implements LMSGetValue().
   */
  OpignoScorm12API.prototype.LMSGetValue = function() {
    console.log('LMSGetValue');
  }

  /**
   * Implements LMSSetValue().
   */
  OpignoScorm12API.prototype.LMSSetValue = function() {
    console.log('LMSSetValue');
  }

  /**
   * Implements LMSCommit().
   */
  OpignoScorm12API.prototype.LMSCommit = function() {
    console.log('LMSCommit');
  }

  /**
   * Implements LMSGetLastError().
   */
  OpignoScorm12API.prototype.LMSGetLastError = function() {
    console.log('LMSGetLastError');
    return '0';
  }

  /**
   * Implements LMSGetErrorString().
   */
  OpignoScorm12API.prototype.LMSGetErrorString = function() {
    console.log('LMSGetErrorString');
  }

  /**
   * Implements LMSGetDiagnostic().
   */
  OpignoScorm12API.prototype.LMSGetDiagnostic = function() {
    console.log('LMSGetDiagnostic');
  }

  // Export.
  window.API = new OpignoScorm12API();

})(jQuery, Drupal, window);