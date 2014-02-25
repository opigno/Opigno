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
    this.eventCallbacks = {
      initialize: [],
      terminate: [],
      commit: []
    };
    this.data = {
      cmi: {
        _version: '1.0',
        comments_from_learner: []
      }
    };
    this.data.cmi.comments_from_learner._children = 'comment,location,timestamp';
  };

  /**
   * @const Requested CMI value is currently not available.
   */
  OpignoScormUI2004API.VALUE_NOT_AVAILABLE = 'VALUE_NOT_AVAILABLE';

  /**
   * @const Requested CMI value is invalid.
   */
  OpignoScormUI2004API.CMI_NOT_VALID = 'CMI_NOT_VALID';

  /**
   * @const Requested CMI value is not yet implemented by Opigno.
   */
  OpignoScormUI2004API.CMI_NOT_IMPLEMENTED = 'CMI_NOT_IMPLEMENTED';

  /**
   * @const Requested CMI value is write-only.
   */
  OpignoScormUI2004API.VALUE_WRITE_ONLY = 'VALUE_WRITE_ONLY';

  /**
   * @const Requested CMI value is read-only.
   */
  OpignoScormUI2004API.VALUE_READ_ONLY = 'VALUE_READ_ONLY';

  /**
   * @const Requested CMI child value does not exist.
   */
  OpignoScormUI2004API.CHILD_DOES_NOT_EXIST = 'CHILD_DOES_NOT_EXIST';

  /**
   * Implements Initialize().
   *
   * @param {String} value
   *        Expected to be an empty string. If not, will
   *        return a 201 error.
   *
   * @returns {String} 'true' or 'false'
   *        The SCO expects boolean values to be returned as strings.
   */
  OpignoScormUI2004API.prototype.Initialize = function(value) {
    console.log('Initialize', value);

    // The value MUST be an empty string (per SCORM.2004.3ED.ConfReq.v1.0).
    // If it's not empty, don't bother initializing the package.
    if (value !== '') {
      this.error =  '201';
      return 'false';
    }

    if (!this.isInitialized) {
      // If the communication fails, set the error to 102
      // and return 'false'.
      if (!this._initCommunication()) {
        this.error = '102';
        return 'false';
      }
      else {
        this.isInitialized = true;
      }
    }
    // If terminated, set the error to 104 and return 'false'.
    else if (this.isTerminated) {
      this.error = '104';
      return 'false';
    }
    // If already initialized, set the error to 103 and return 'false'.
    else if (this.isInitialized) {
      this.error = '103';
      return 'false';
    }

    // Successfully initialized the package.
    this.error = '0';
    return 'true';
  }

  /**
   * Implements Terminate().
   *
   * @param {String} value
   *        Expected to be an empty string. If not, will
   *        return a 201 error.
   *
   * @returns {String} 'true' or 'false'
   *        The SCO expects boolean values to be returned as strings.
   */
  OpignoScormUI2004API.prototype.Terminate = function(value) {
    console.log('Terminate', value);

    // The value MUST be an empty string (per SCORM.2004.3ED.ConfReq.v1.0).
    // If it's not empty, don't bother terminating the package.
    if (value !== '') {
      this.error =  '201';
      return 'false';
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
      else {
        this.isTerminated = true;
      }
    }

    this.error =  '0';
    return 'true';
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

    // Cannot get a value if not initialized.
    // Set the error to 122 end return ''.
    if (!this.isInitialized) {
      this.error = '122';
      return '';
    }
    // Cannot get a value if terminated.
    // Set the error to 123 end return ''.
    else if (this.isTerminated) {
      this.error = '123';
      return '';
    }

    // Must provide a cmiElement. If no valid identifier is provided,
    // set the error to 301 and return ''.
    if (cmiElement === undefined || cmiElement === null || cmiElement === '') {
      this.error = '301';
      return '';
    }

    // Find the CMI value.
    try {
      if (/^cmi\./.test(cmiElement)) {
        var result = this._getCMIData(cmiElement);

        // If the value is not available, set the error to 403
        // and return ''.
        if (result === OpignoScormUI2004API.VALUE_NOT_AVAILABLE) {
          this.error = '403';
          return '';
        }
        // If the value does not exist, set the error to 401
        // and return ''.
        else if (result === OpignoScormUI2004API.CMI_NOT_VALID) {
          this.error = '401';
          return '';
        }
        // If the value is supposed to be a child value, but the parent
        // doesn't have it, set the error to 301 and return ''.
        else if (result === OpignoScormUI2004API.CHILD_DOES_NOT_EXIST) {
          this.error = '301';
          return '';
        }
        // If the value is write-only, set the error to 405 and
        // return ''.
        else if (result === OpignoScormUI2004API.VALUE_WRITE_ONLY) {
          this.error = '405';
          return '';
        }
        // For currently unimplemented values, set the error to 402
        // and return ''.
        else if (result === OpignoScormUI2004API.CMI_NOT_IMPLEMENTED) {
          this.error = '402';
          return '';
        }
        // If the value was found, return it and set the error to '0'.
        else {
          this.error = '0';
          return result;
        }
      }
      // For unknown values, set the error to 401 and return ''.
      else {
        this.error = '401';
        return '';
      }
    }
    catch (e) {
      // If anything fails, for whatever reason, set the error to 301 and
      // return ''.
      this.error = '301';
      return '';
    }
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

    // Cannot get a value if not initialized.
    // Set the error to 122 end return ''.
    if (!this.isInitialized) {
      this.error = '132';
      return 'false';
    }
    // Cannot get a value if terminated.
    // Set the error to 123 end return ''.
    else if (this.isTerminated) {
      this.error = '133';
      return 'false';
    }

    // Must provide a cmiElement. If no valid identifier is provided,
    // set the error to 301 and return ''.
    if (cmiElement === undefined || cmiElement === null || cmiElement === '' || typeof cmiElement !== 'string') {
      this.error = '351';
      return 'false';
    }

    // The value must either be a String or a number. All other values have to be rejected.
    // Return 'false' and set the error to 406.
    if (typeof value !== 'string' && typeof value !== 'number') {
      this.error = '406';
      return 'false';
    }

    // Find the CMI value.
    try {
      if (/^cmi\./.test(cmiElement)) {
        var result = this._setCMIData(cmiElement, value);

        // If the value does not exist, set the error to 401
        // and return 'false'.
        if (result === OpignoScormUI2004API.CMI_NOT_VALID) {
          this.error = '401';
          return 'false';
        }
        // For currently unimplemented values, set the error to 402
        // and return 'false'.
        else if (result === OpignoScormUI2004API.CMI_NOT_IMPLEMENTED) {
          this.error = '402';
          return 'false';
        }
        // For read-only values, set the error to 404 and return 'false'.
        else if (result === OpignoScormUI2004API.VALUE_READ_ONLY) {
          this.error = '404';
          return 'false';
        }
      }
      // For unknown values, set the error to 401 and return ''.
      else {
        this.error = '401';
        return 'false';
      }
    }
    catch (e) {
      // If anything fails, for whatever reason, set the error to 351 and
      // return ''.
      this.error = '351';
      return 'false';
    }

    this.error = '0';
    return 'true';
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

    // The value MUST be an empty string (per SCORM.2004.3ED.ConfReq.v1.0).
    // If it's not empty, don't bother terminating the package.
    if (value !== '') {
      this.error =  '201';
      return 'false';
    }

    // Can only commit if the session was initialized. Else, set error to
    // 142 and return 'false'.
    if (!this.isInitialized) {
      this.error = '142';
      return 'false';
    }
    // If already terminated, set the error to 143 and return 'false'.
    else if (this.isTerminated) {
      this.error = '143';
      return 'false';
    }

    try {
      // Persist the data.
    }
    catch (e) {
      // If anything fails, for whatever reason, set the error to 391 and
      // return ''.
      this.error = '391';
      return 'false';
    }

    this.error =  '0';
    return 'true';
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

    // @todo
    return '';
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

    // @todo
    return '';
  }

  /**
   * Initialize the communication between the SCORM package and Opigno.
   *
   * @return {Boolean}
   */
  OpignoScormUI2004API.prototype._initCommunication = function() {
    // The SCORM 2004 edition does not provide any asynchronous logic, or the concept
    // of promises. This means establishing the communication between the SCORM
    // and Opigno must always be considered "active", and can never "fail".
    // We return true in any case.
    return true;
  }

  /**
   * Bind an event listener to the API.
   *
   * @param {String} event
   * @param {Function} callback
   */
  OpignoScormUI2004API.prototype.bind = function(event, callback) {
    if (this.eventCallbacks[event] === undefined) {
      throw { name: "ScormAPIUnknownEvent", message: "Can't bind/trigger event '" + event + "'" };
    }
    else {
      this.eventCallbacks[event].push(callback);
    }
  }

  /**
   * Trigger the passed event. All parameters (except the event name) are passed
   * to the registered callback.
   *
   * @param {String} event
   */
  OpignoScormUI2004API.prototype.trigger = function() {
    var event = arguments.shift();

    if (this.eventCallbacks[event] === undefined) {
      throw { name: "ScormAPIUnknownEvent", message: "Can't bind/trigger event '" + event + "'" };
    }
    else {
      for (var i = 0, len = this.eventCallbacks[event].length; i < len; i++) {
        this.eventCallbacks[event][i].apply(this, arguments);
      }
    }
  }

  /**
   * Fetch the CMI data by recursively checking the CMI data tree.
   *
   * @param {String} cmiPath
   *
   * @returns {String}
   */
  OpignoScormUI2004API.prototype._getCMIData = function(cmiPath) {
    // Special test values.
    if (cmiPath === 'cmi.__value__') {
      return 'value';
    }
    else if (cmiPath === 'cmi.__write_only__') {
      return OpignoScormUI2004API.VALUE_WRITE_ONLY;
    }
    else if (cmiPath === 'cmi.__unimplemented__') {
      return OpignoScormUI2004API.CMI_NOT_IMPLEMENTED;
    }
    else if (cmiPath === 'cmi.__unknown__') {
      return OpignoScormUI2004API.CMI_NOT_VALID;
    }

    // Check if the CMI path is valid. If not, return CMI_NOT_VALID.
    if (!this._validCMIDataPath(cmiPath)) {
      return OpignoScormUI2004API.CMI_NOT_VALID;
    }
    // Check if the CMI path is write-only. If so, return VALUE_WRITE_ONLY.
    else if (this._writeOnlyCMIDataPath(cmiPath)) {
      return OpignoScormUI2004API.VALUE_WRITE_ONLY;
    }
    // Check if the CMI path is implemented. If not, return CMI_NOT_IMPLEMENTED.
    else if (!this._implementedCMIDataPath(cmiPath)) {
      return OpignoScormUI2004API.CMI_NOT_IMPLEMENTED;
    }

    // Recursively walk the data tree and get the requested leaf.
    var pathTree = cmiPath.split('.'),
      // Get the first path element, usually 'cmi'.
      path = pathTree.shift(),
      // Get the root element data.
      data = this.data[path] !== undefined ? this.data[path] : null,
      // Are there more parts ? If so, flag this as looking for children.
      checkChildren = pathTree.length  > 1;

    // Recursively walk the tree.
    while (data && pathTree.length) {
      path = pathTree.shift();

      // Special case: if we request the length of an array, check if the current
      // data is an array. If so, get its length and break out of the loop.
      // If not, throw an error.
      if (path === '_count') {
        if (data.length !== undefined) {
          data = data.length;
          break;
        }
        else {
          console.log(data);
          throw new EvalError("Can only get the '_count' property for array data. CMI path: " + cmiPath);
        }
      }
      else {
        data = data[path] !== undefined ? data[path] : null;
      }
    }

    if (data !== null) {
      return data;
    }
    else {
      // If we were looking for an element children, return CHILD_DOES_NOT_EXIST.
      if (checkChildren) {
        return OpignoScormUI2004API.CHILD_DOES_NOT_EXIST;
      }
      // Else, return VALUE_NOT_AVAILABLE.
      else {
        return OpignoScormUI2004API.VALUE_NOT_AVAILABLE;
      }
    }
  }

  /**
   * Set the CMI data by recursively checking the CMI data tree.
   * Create elements in the tree that do not exist yet.
   *
   * @param {String} cmiPath
   * @param {String} value
   *
   * @returns {String}
   */
  OpignoScormUI2004API.prototype._setCMIData = function(cmiPath, value) {
    // Check if the CMI path is valid. If not, return CMI_NOT_VALID.
    if (!this._validCMIDataPath(cmiPath)) {
      return OpignoScormUI2004API.CMI_NOT_VALID;
    }
    // Check if the CMI path is implemented. If not, return CMI_NOT_IMPLEMENTED.
    else if (!this._implementedCMIDataPath(cmiPath)) {
      return OpignoScormUI2004API.CMI_NOT_IMPLEMENTED;
    }
    // Check if the CMI path is read-only. If so, return VALUE_READ_ONLY.
    else if (this._readOnlyCMIDataPath(cmiPath)) {
      return OpignoScormUI2004API.VALUE_READ_ONLY;
    }

    // @todo DO not set _count or _children properties. Read-only

    // Recursively walk the data tree and get the requested leaf.
    var pathTree = cmiPath.split('.'),
      // Get the first path element, usually 'cmi'.
      path = pathTree.shift(),
      // Get the last path element.
      leaf = pathTree.length ? pathTree.pop() : false;

    // If the root does not exist, initialize an empty object.
    if (this.data[path] === undefined) {
      this.data[path] = {};
    }

    // Get the root element data.
    var data = this.data[path];

    // If the leaf is not set, we don't need to walk any tree. Set the value immediately.
    if (!leaf) {
      data = value;
    }
    // Else, we walk the tree recursively creating all elements if needed.
    else {
      var prevPaths = [path];
      // Recursively walk the tree.
      while (pathTree.length) {
        path = pathTree.shift();

        // If the property does not exist yet, create it.
        if (data[path] === undefined) {
          // If the property is numerical, we're dealing with an array.
          if (/^[0-9]+$/.test(path)) {
            // If the key is 0, and the parent is not an array, reset the parent to an array object.
            // Push an empty element onto the array.
            if (path === '0' && data.length === undefined) {
              // Just resetting data to [] loses it's relationship with this.data. We have no choice
              // but to use eval() here.
              eval('this.data.' + prevPaths.join('.') + ' = [];');
              eval('data = this.data.' + prevPaths.join('.') + ';');
              data.push({});
            }
            // If the parent is an array object, but the given key is out of bounds, throw an error.
            else if (data.length < path) {
              throw { name: "CMIDataOutOfBounds", message: "Out of bounds. Cannot set [" + path + "] on " + cmiPath + ", as it contains only " + data.length + " elements." };
            }
            // Finally, if this is an array, and the key is valid, but there's no element yet,
            // push an empty element onto the array.
            else if (data[path] === undefined) {
              data.push({});
            }
          }
          // Else, we're dealing with a hash.
          else {
            data[path] = {};
          }
        }

        data = data[path];
        prevPaths.push(path);
      }

      data[leaf] = value;
    }
  }

  /**
   * Check if the given CMI path is valid and usable.
   *
   * @param {String} cmiPath
   *
   * @returns {Boolean}
   */
  OpignoScormUI2004API.prototype._validCMIDataPath = function(cmiPath) {
    var keys = [
      // Special test paths.
      'cmi.__value__',
      'cmi.__read_only__',
      'cmi.__unimplemented__',
      'cmi.__test__',
      'cmi.__test__._count',
      'cmi.__test__.n.child',

      // Real CMI paths.
      'cmi._version',
      'cmi.comments_from_learner',
      'cmi.comments_from_learner._children',
      'cmi.comments_from_learner._count',
      'cmi.comments_from_learner.n.comment',
      'cmi.comments_from_learner.n.location',
      'cmi.comments_from_learner.n.timestamp'
    ];

    // Replace all ".[0-9]." values with ".n.".
    return keys.indexOf(cmiPath.replace(/\.[0-9]+\./g, '.n.')) !== -1;
  }

  /**
   * Check if the given CMI path is write-only.
   *
   * @param {String} cmiPath
   *
   * @returns {Boolean}
   */
  OpignoScormUI2004API.prototype._writeOnlyCMIDataPath = function(cmiPath) {
    // @todo
    return false;

    var keys = [

    ];

    return keys.indexOf(cmiPath) !== -1;
  }

  /**
   * Check if the given CMI path is read-only.
   *
   * @param {String} cmiPath
   *
   * @returns {Boolean}
   */
  OpignoScormUI2004API.prototype._readOnlyCMIDataPath = function(cmiPath) {
    // Array properties are always read-only.
    if (/\._(count|children)/.test(cmiPath)) {
      return true;
    }

    var keys = [
      // Special test paths.
      'cmi.__read_only__'
    ];

    return keys.indexOf(cmiPath) !== -1;
  }

  /**
   * Check if the given CMI path is implemented by Opigno.
   *
   * @param {String} cmiPath
   *
   * @returns {Boolean}
   */
  OpignoScormUI2004API.prototype._implementedCMIDataPath = function(cmiPath) {
    var keys = [
      // Special test paths.
      'cmi.__value__',
      'cmi.__read_only__',
      'cmi.__test__',
      'cmi.__test__._count',
      'cmi.__test__.n.child',

      // Real CMI paths.
      'cmi._version',
      'cmi.comments_from_learner',
      'cmi.comments_from_learner._children',
      'cmi.comments_from_learner._count',
      'cmi.comments_from_learner.n.comment',
      'cmi.comments_from_learner.n.location',
      'cmi.comments_from_learner.n.timestamp',
      'cmi.comments_from_lms._children',
      'cmi.comments_from_lms._count',
      // 'cmi.comments_from_lms.n.comment',
      // 'cmi.comments_from_lms.n.location',
      // 'cmi.comments_from_lms.n.timestamp',

      '' // Dummy value to prevent trailing comma errors in this particular list.
    ];

    return keys.indexOf(cmiPath.replace(/\.[0-9]+\./g, '.n.')) !== -1;
  }

  // Export.
  window.OpignoScormUI2004API = OpignoScormUI2004API;
  window.API_1484_11 = new OpignoScormUI2004API();

})(jQuery, Drupal, window);