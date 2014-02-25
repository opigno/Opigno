/**
 * @file
 * Test suite for the SCORM 2004 RTE API.
 */

;(function($, Drupal, window, OpignoScormUI2004API, undefined) {

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
        ok(window.API_1484_11,                         'REQ_2.5: A global API_1484_11 object exists.');
        ok(window.API_1484_11.version,                 'REQ_2.6: The API implements a version property.');
        ok(/^1\.0\./.test(window.API_1484_11.version), 'REQ_2.6.1, REQ_2.6.2: The API version is correctly formatted.');
        ok(window.API_1484_11.Initialize,              'REQ_4.1: The API implements the Initialize() method.');
        ok(window.API_1484_11.Terminate,               'REQ_5.1: The API implements the Terminate() method.');
        ok(window.API_1484_11.GetValue,                'REQ_6.1: The API implements the GetValue() method.');
        ok(window.API_1484_11.SetValue,                'The API implements the SetValue() method.');
        ok(window.API_1484_11.Commit,                  'The API implements the Commit() method.');
        ok(window.API_1484_11.GetLastError,            'The API implements the GetLastError() method.');
        ok(window.API_1484_11.GetErrorString,          'The API implements the GetErrorString() method.');
        ok(window.API_1484_11.GetDiagnostic,           'The API implements the GetDiagnostic() method.');

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
        // @todo Missing specs for REQ_5.2.1m REQ_5.2.1.1 and REQ_5.3.
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


      module('API::GetValue()');
        // @todo Missing specs for REQ_6.5 and REQ_6.7.
        api = new OpignoScormUI2004API();
        equal(api.GetValue(''), '',                      'REQ_6.8: Requesting a value before initializing fails.');
        equal(api.GetLastError(), '122',                 'REQ_6.8: Requesting a value before initializing gives a 122 error.');
        // Initialize the communication.
        api.Initialize('');
        equal(api.GetValue('cmi.__value__'), 'value',    'REQ_6.2: Requesting a recognized value succeeds.');
        equal(api.GetLastError(), '0',                   'REQ_6.2: Requesting a recognized value gives no error.');
        equal(api.GetValue('cmi.__unknown__'), '',       'REQ_6.3: Requesting an unknown value fails.');
        equal(api.GetLastError(), '401',                 'REQ_6.3: Requesting an unknown value gives a 401 error.');
        equal(api.GetValue('__unknown__'), '',           'REQ_6.3: Requesting an unknown value fails.');
        equal(api.GetLastError(), '401',                 'REQ_6.3: Requesting an unknown value gives a 401 error.');
        equal(api.GetValue('cmi.__unimplemented__'), '', 'REQ_6.4: Requesting an unimplemented value fails.');
        equal(api.GetLastError(), '402',                 'REQ_6.4: Requesting an unimplemented value gives a 402 error.');
        equal(api.GetValue('cmi.__write_only__'), '',    'REQ_6.6: Requesting a write-only value fails.');
        equal(api.GetLastError(), '405',                 'REQ_6.6: Requesting an write-only value gives a 405 error.');
        equal(api.GetValue(''), '',                      'REQ_6.10: Requesting an empty string key fails.');
        equal(api.GetLastError(), '301',                 'REQ_6.10: Requesting an empty string key gives a 301 error.');
        equal(api.GetValue(), '',                        'REQ_6.10: Requesting a "null" key fails.');
        equal(api.GetLastError(), '301',                 'REQ_6.10: Requesting a "null" key gives a 301 error.');
        equal(api.GetValue('cmi.__test__.1.child'), '',  'REQ_6.11: Requesting an inexistent child fails.');
        equal(api.GetLastError(), '301',                 'REQ_6.11: Requesting an inexistent child gives a 301 error.');
        equal(api.GetValue('cmi.__test__._count'), '',   'REQ_6.12: Requesting the count property of an element that is not an array fails.');
        equal(api.GetLastError(), '301',                 'REQ_6.12: Requesting the count property of an element that is not an array gives a 301 error.');
        // Set some array value to test array properties.
        api.SetValue('cmi.__test__.0.child', 'value');
        equal(api.GetValue('cmi.__test__.0.child'), 'value', 'REQ_6.2: Requesting a recognized child value succeeds.');
        equal(api.GetLastError(), '0',                       'REQ_6.2: Requesting a recognized child value gives no error.');
        equal(api.GetValue('cmi.__test__._count'), 1,        'REQ_6.12: Requesting the count property of an element that is an array succeeds.');
        equal(api.GetLastError(), '0',                       'REQ_6.12: Requesting the count property of an element that is an array gives no error.');
        // Terminate the communication.
        api.Terminate('');
        equal(api.GetValue(''), '',                          'REQ_6.9: Requesting a value after termination fails.');
        equal(api.GetLastError(), '123',                     'REQ_6.9: Requesting a value after termination gives a 123 error.');


      module('API::SetValue()');
        // @todo Missing specs for REQ_7.7, REQ_7.8, REQ_7.11, REQ_7.12, REQ_7.15.
        api = new OpignoScormUI2004API();
        equal(api.SetValue('', ''), 'false',                     'REQ_7.9: Setting a value before initializing fails.');
        equal(api.GetLastError(), '132',                         'REQ_7.8: Setting a value before initializing gives a 132 error.');
        // Initialize the communication.
        api.Initialize('');
        equal(api.SetValue('cmi.__value__', 'value'), 'true',    'REQ_7.2: Setting a recognized value succeeds.');
        equal(api.GetLastError(), '0',                           'REQ_7.2: Setting a recognized value gives no error.');
        equal(api.SetValue('cmi.__unknown__', 'value'), 'false', 'REQ_7.3: Setting an unknown value fails.');
        equal(api.GetLastError(), '401',                         'REQ_7.3: Setting an unknown value gives a 401 error.');
        equal(api.SetValue('__unknown__', 'value'), 'false',       'REQ_7.3: Setting an unknown value fails.');
        equal(api.GetLastError(), '401',                           'REQ_7.3: Setting an unknown value gives a 401 error.');
        equal(api.SetValue('cmi.__unimplemented__', 'value'), 'false',       'REQ_7.4: Setting an unimplemented value fails.');
        equal(api.GetLastError(), '402',                           'REQ_7.4: Setting an unimplemented value gives a 402 error.');
        equal(api.SetValue('cmi.__read_only__', 'value'), 'false',       'REQ_7.5: Setting a read-only value fails.');
        equal(api.GetLastError(), '404',                           'REQ_7.5: Setting a read-only value gives a 404 error.');
        equal(api.SetValue('cmi.__test__._count', 2), 'false',   'REQ_7.5: Setting a read-only value fails.');
        equal(api.GetLastError(), '404',                 'REQ_7.5: Setting a read-only value gives a 404 error.');
        equal(api.SetValue('cmi.__value__', { key: 'value' }), 'false',   'REQ_7.6: Setting a non-string value fails.');
        equal(api.GetLastError(), '406',                 'REQ_7.6: Setting a non-string value gives a 406 error.');
        equal(api.SetValue('', 'value'), 'false',   'REQ_7.13: Providing an empty string path fails.');
        equal(api.GetLastError(), '351',                 'REQ_7.13: Providing an empty string path gives a 351 error.');
        equal(api.SetValue(null, 'value'), 'false',   'REQ_7.13: Providing an non-string path fails.');
        equal(api.GetLastError(), '351',                 'REQ_7.13: Providing an non-string path gives a 351 error.');
        equal(api.SetValue(89, 'value'), 'false',   'REQ_7.13: Providing an non-string path fails.');
        equal(api.GetLastError(), '351',                 'REQ_7.13: Providing an non-string path gives a 351 error.');
        equal(api.SetValue('cmi.__test__.0.child', 'value'), 'true',   'REQ_7.2: Setting a valid array value succeeds.');
        equal(api.GetLastError(), '0',                 'REQ_7.2: Setting a valid array value gives no error.');
        equal(api.SetValue('cmi.__test__.2.child', 'value'), 'false',   'REQ_7.14: Setting an array key that is out of bounds fails.');
        equal(api.GetLastError(), '351',                 'REQ_7.14: Setting an array key that is out of bounds gives a 351 error.');
        // Terminate the communication.
        api.Terminate('');
        equal(api.SetValue('cmi.__value__', 'value'), 'false',  'REQ_7.10: Setting a value after termination fails.');
        equal(api.GetLastError(), '133',                     'REQ_7.10: Setting a value after termination gives a 123 error.');
    }
  };

})(jQuery, Drupal, window, OpignoScormUI2004API);
