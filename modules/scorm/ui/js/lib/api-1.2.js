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
  var OpignoScormUI12API = function() { };

  /**
   * Implements LMSInitialize().
   */
  OpignoScormUI12API.prototype.LMSInitialize = function() {
    console.log('LMSInitialize');
  }

  /**
   * Implements LMSFinish().
   */
  OpignoScormUI12API.prototype.LMSFinish = function() {
    console.log('LMSFinish');
  }

  /**
   * Implements LMSGetValue().
   */
  OpignoScormUI12API.prototype.LMSGetValue = function() {
    console.log('LMSGetValue');
  }

  /**
   * Implements LMSSetValue().
   */
  OpignoScormUI12API.prototype.LMSSetValue = function() {
    console.log('LMSSetValue');
  }

  /**
   * Implements LMSCommit().
   */
  OpignoScormUI12API.prototype.LMSCommit = function() {
    console.log('LMSCommit');
  }

  /**
   * Implements LMSGetLastError().
   */
  OpignoScormUI12API.prototype.LMSGetLastError = function() {
    console.log('LMSGetLastError');
  }

  /**
   * Implements LMSGetErrorString().
   */
  OpignoScormUI12API.prototype.LMSGetErrorString = function() {
    console.log('LMSGetErrorString');
  }

  /**
   * Implements LMSGetDiagnostic().
   */
  OpignoScormUI12API.prototype.LMSGetDiagnostic = function() {
    console.log('LMSGetDiagnostic');
  }

  // Export.
  window.API = new OpignoScormUI12API();

})(jQuery, Drupal, window);