/**
 * @file
 * Test suite for the SCORM 2004 RTE API.
 */

;(function($, Drupal, API_1484_11, OpignoScormUI2004API, undefined) {

  /**
   * Fixture object.
   *
   * Stub AJAX responses, so the API doesn't tie to the actual server.
   */
  var fixture = {
    paths: {
      'api/init': {
        id: '1234'
      }
    },
    ajaxResponse: function(path) {
      if (fixture.paths[path] !== undefined) {
        return fixture.paths[path];
      }
      else {
        throw new URIError('Path ' + path + ' not stubbed.');
      }
    }
  };

  Drupal.tests.OpignoScormUI2004API = {
    getInfo: function() {
      return {
        name: 'Opigno SCORM 2004 API',
        description: 'Test the SCORM 2004 3rd Edition RTE API',
        group: 'Opigno'
      };
    },
    test: function() {
      module('API detection and method implementations.');
      ok(API_1484_11,                         'REQ_2.5: A global API_1484_11 object exists.');
      ok(API_1484_11.version,                 'REQ_2.6: The API implements a version property.');
      ok(/^1\.0\./.test(API_1484_11.version), 'REQ_2.6.1, REQ_2.6.2: The API version is correctly formatted.');
      ok(API_1484_11.Initialize,              'REQ_4.1: The API implements the Initialize() method.');
      ok(API_1484_11.Terminate,               'REQ_5.1: The API implements the Terminate() method.');
      ok(API_1484_11.GetValue,                'The API implements the GetValue() method.');
      ok(API_1484_11.SetValue,                'The API implements the SetValue() method.');
      ok(API_1484_11.Commit,                  'The API implements the Commit() method.');
      ok(API_1484_11.GetLastError,            'The API implements the GetLastError() method.');
      ok(API_1484_11.GetErrorString,          'The API implements the GetErrorString() method.');
      ok(API_1484_11.GetDiagnostic,           'The API implements the GetDiagnostic() method.');

      // Get a new API implementation, so we can easily reset its internals.
      var api = new OpignoScormUI2004API();

      module('API, initial state.');
      equal(api.GetLastError(), '0', 'REQ_3.1: The original error code is "0".');


      module('API::Initialize()');
      api = new OpignoScormUI2004API();
      equal(api.Initialize(), 'false',             'REQ_3.2: Initializing the API without a parameter fails.');
      equal(api.GetLastError(), '201',             'REQ_3.2: Initializing the API without a parameter gives a 201 error.');
      equal(api.Initialize('any string'), 'false', 'REQ_3.2: Initializing the API with any string fails.');
      equal(api.GetLastError(), '201',             'REQ_3.2: Initializing the API with any string gives a 201 error.');
      equal(api.Initialize(''), 'true',            'REQ_4.1.1, REQ_4.5: Initializing the API with an empty string succeeds.');
      equal(api.GetLastError(), '0',               'REQ_4.5: Initializing the API with an empty string gives no error.');
      equal(api.Initialize(''), 'false',           'REQ_4.3: Initializing the API twice fails.');
      equal(api.GetLastError(), '103',             'REQ_4.3: Initializing the API twice gives a 103 error.');
      // Terminate the communication.
      api.Terminate('');
      equal(api.Initialize(''), 'false',           'REQ_4.4: Initializing the API after termination fails.');
      equal(api.GetLastError(), '104',             'REQ_4.4: Initializing the API after termination gives a 104 error.');


      module('API::Terminate()');
      api = new OpignoScormUI2004API();
      equal(api.Terminate(''), 'false',            'REQ_5.4: Terminating the API before initializing fails.');
      equal(api.GetLastError(), '112',             'REQ_5.4: Terminating the API before initializing gives a 112 error.');
      // Initialize the communication.
      api.Initialize('');
      equal(api.Terminate(), 'false',              'REQ_3.2: Terminating the API without a parameter fails.');
      equal(api.GetLastError(), '201',             'REQ_3.2: Terminating the API without a parameter gives a 201 error.');
      equal(api.Terminate('any string'), 'false',  'REQ_3.2: Terminating the API with any string fails.');
      equal(api.GetLastError(), '201',             'REQ_3.2: Terminating the API with any string gives a 201 error.');
      equal(api.Terminate(''), 'true',             'REQ_5.1.1, REQ_5.2: Terminating the API with an empty string succeeds.');
      equal(api.GetLastError(), '0',               'REQ_5.2: Terminating the API with an empty string gives no error.');
      equal(api.Terminate(''), 'false',            'REQ_5.5: Terminating the API twice.');
      equal(api.GetLastError(), '113',             'REQ_5.5: Terminating the API twice gives a 113 error.');
    }
  };

})(jQuery, Drupal, API_1484_11, OpignoScormUI2004API);
