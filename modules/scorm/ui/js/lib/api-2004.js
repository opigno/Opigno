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
  var OpignoScormUI2004API = function() {
    this.version = '1.0.0';
    this.error = '0';
    this.isInitialized = false;
    this.isTerminated = false;
  };

  /**
   * Implements Initialize().
   *
   * @param {String} value
   *        Expected to be an empty string. If not, will
   *        return a 201 error.
   *
   * @returns {String} 'true', 'false' or '201'.
   *        The SCO expects boolean values to be returned as strings.
   */
  OpignoScormUI2004API.prototype.Initialize = function(value) {
    console.log('Initialize', value);

    // The value MUST be an empty string (per SCORM.2004.3ED.ConfReq.v1.0).
    // If it's not empty, don't bother initializing the package.
    if (value !== '') {
      return '201';
    }

    if (!this.isInitialized) {
      // @todo Establish communication with Opigno.
      // If the communication fails, set the error to 102
      // and return 'false'.
      if (false) {
        this.error = '102';
        return 'false';
      }
      else {
        this.isInitialized = true;
      }
    }
    // If already initialized, set the error to 103 and return 'false'.
    else if (this.isInitialized) {
      this.error = '103';
      return 'false';
    }
    // If terminated, set the error to 104 and return 'false'.
    if (this.isTerminated) {
      this.error = '104';
      return 'false';
    }

    // Successfully initialized the package.
    return 'true';
  }

  /**
   * Implements Terminate().
   *
   * @param {String} value
   *        Expected to be an empty string. If not, will
   *        return a 201 error.
   *
   * @returns {String} 'true', 'false' or '201'
   *        The SCO expects boolean values to be returned as strings.
   */
  OpignoScormUI2004API.prototype.Terminate = function(value) {
    console.log('Terminate', value);

    // The value MUST be an empty string (per SCORM.2004.3ED.ConfReq.v1.0).
    // If it's not empty, don't bother terminating the package.
    if (value !== '') {
      return '201';
    }

    // Can only terminate if the session was initialized. Else, set error to
    // 112 and return 'false'.
    if (!this.isInitialized) {
      this.error = '112';
      return 'false';
    }
    // If already terminated, set the error to 113 and return 'false'.
    else if (this.isTerminated) {
      this.error = '113';
      return 'false';
    }
    // Terminate must call Commit to persist all data.
    else if (this.Commit('') === 'false') {
      this.error = '391';
      return 'false';
    }
    else {
      // Terminate the communication.
      // If the termination fails, set the error to 111 end return 'false'.
      if (false) {
        this.error = '111';
        return 'false';
      }
    }

    return value === '' ? 'true' : '201';
  }

  /**
   * Implements GetValue().
   *
   * @param {String} cmiElement
   *
   * @returns {String}
   */
  OpignoScormUI2004API.prototype.GetValue = function(cmiElement) {
    console.log('GetValue', cmiElement);
  }

  /**
   * Implements SetValue().
   *
   * @param {String} cmiElement
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
   * @param {String} value
   *        Expected to be an empty string. If not, will
   *        return a 201 error.
   *
   * @returns {String} 'true' or 'false'
   *        The SCO expects boolean values to be returned as strings.
   */
  OpignoScormUI2004API.prototype.Commit = function(value) {
    console.log('Commit', value);
    return value === '' ? 'true' : '201';
  }

  /**
   * Implements GetLastError().
   *
   * @returns {String}
   */
  OpignoScormUI2004API.prototype.GetLastError = function() {
    console.log('GetLastError');
    return this.error;
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