/**
 * @file
 * Test suite for the SCORM 2004 RTE API.
 */

;(function($, Drupal, window, OpignoScorm2004API, undefined) {

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

  Drupal.tests.OpignoScorm2004API = {
    getInfo: function() {
      return {
        name: 'Opigno SCORM 2004 API',
        description: 'Test the SCORM 2004 3rd Edition RTE API',
        group: 'Opigno'
      };
    },
    test: function() {
      /*
      module('API detection and method implementations.');
        ok(window.API_1484_11,                         'REQ_2.5: A global API_1484_11 object exists.');
        ok(window.API_1484_11.version,                 'REQ_2.6: The API implements a version property.');
        ok(/^1\.0\./.test(window.API_1484_11.version), 'REQ_2.6.1, REQ_2.6.2: The API version is correctly formatted.');
        ok(window.API_1484_11.Initialize,              'REQ_4.1: The API implements the Initialize() method.');
        ok(window.API_1484_11.Terminate,               'REQ_5.1: The API implements the Terminate() method.');
        ok(window.API_1484_11.GetValue,                'REQ_6.1: The API implements the GetValue() method.');
        ok(window.API_1484_11.SetValue,                'REQ_7.1: The API implements the SetValue() method.');
        ok(window.API_1484_11.Commit,                  'REQ_8.1: The API implements the Commit() method.');
        ok(window.API_1484_11.GetLastError,            'REQ_9.1: The API implements the GetLastError() method.');
        ok(window.API_1484_11.GetErrorString,          'REQ_10.1: The API implements the GetErrorString() method.');
        ok(window.API_1484_11.GetDiagnostic,           'REQ_11.1: The API implements the GetDiagnostic() method.');
      */

      // Get a new API implementation, so we can easily reset its internals.
      var api = new OpignoScorm2004API(), value = '';

      /**
       * @defgroup scorm_2004_object_specs Object specs
       * @{
       * Specs for the OpignoScorm2004API custom methods.
       *
       * OpignoScorm2004API provides several methods for easy integration
       * with other libraries. This is different from the SCORM API methods,
       * which are defined by the ADL Conformance Requirements.
       */

      /**
       * We use a event powered system for abstracting communication
       * between the SCO and the LMS. Test the events.
       */
      module('Events');
        api = new OpignoScorm2004API();

        var spy = {};
        api.bind('initialize', function() {
          spy.called = true;
          spy.that = this;
        });
        api.Initialize('');
        ok(spy.called, 'Spy got called for the "initialize" event.');
        equal(spy.that, api, 'Spy called with correct context (API object).');

        spy = {};
        api.bind('pre-getvalue', function(cmiPath) {
          spy.preGetValueCalled = true;
          spy.preGetValueThat = this;
          spy.preGetValueCMIPath = cmiPath;
        });
        api.bind('post-getvalue', function(cmiPath, value) {
          spy.postGetValueCalled = true;
          spy.postGetValueThat = this;
          spy.postGetValueCMIPath = cmiPath;
          spy.postGetValueValue = value;
        });
        api.GetValue('cmi._version');
        ok(spy.preGetValueCalled, 'Spy got called for the "pre-getvalue" event.');
        equal(spy.preGetValueThat, api, 'Spy called with correct context (API object).');
        equal(spy.preGetValueCMIPath, 'cmi._version', 'Spy got called for the "pre-getvalue" event with the correct cmi path.');
        ok(spy.postGetValueCalled, 'Spy got called for the "post-getvalue" event.');
        equal(spy.postGetValueThat, api, 'Spy called with correct context (API object).');
        equal(spy.postGetValueCMIPath, 'cmi._version', 'Spy got called for the "post-getvalue" event with the correct cmi path.');
        equal(spy.postGetValueValue, api.data.cmi._version, 'Spy got called for the "post-getvalue" event with the correct value.');

        spy = {};
        api.bind('pre-setvalue', function(cmiPath, value) {
          spy.preSetValueCalled = true;
          spy.preSetValueThat = this;
          spy.preSetValueCMIPath = cmiPath;
          spy.preSetValueValue = value;
        });
        api.bind('post-setvalue', function(cmiPath, value) {
          spy.postSetValueCalled = true;
          spy.postSetValueThat = this;
          spy.postSetValueCMIPath = cmiPath;
          spy.postSetValueValue = value;
        });
        api.SetValue('cmi.__value__', 'value');
        ok(spy.preSetValueCalled, 'Spy got called for the "pre-setvalue" event.');
        equal(spy.preSetValueThat, api, 'Spy called with correct context (API object).');
        equal(spy.preSetValueCMIPath, 'cmi.__value__', 'Spy got called for the "pre-setvalue" event with the correct cmi path.');
        equal(spy.preSetValueValue, 'value', 'Spy got called for the "pre-setvalue" event with the correct value.');
        ok(spy.postSetValueCalled, 'Spy got called for the "post-setvalue" event.');
        equal(spy.postSetValueThat, api, 'Spy called with correct context (API object).');
        equal(spy.postSetValueCMIPath, 'cmi.__value__', 'Spy got called for the "post-setvalue" event with the correct cmi path.');
        equal(spy.postSetValueValue, 'value', 'Spy got called for the "post-setvalue" event with the correct value.');

        spy = {};
        api.bind('pre-commit', function() {
          spy.preCommitCalled = true;
          spy.preCommitThat = this;
        });
        api.bind('commit', function() {
          spy.commitCalled = true;
          spy.commitThat = this;
        });
        api.bind('post-commit', function() {
          spy.postCommitCalled = true;
          spy.postCommitThat = this;
        });
        api.Commit('');
        ok(spy.preCommitCalled, 'Spy got called for the "pre-commit" event.');
        equal(spy.preCommitThat, api, 'Spy called with correct context (API object).');
        ok(spy.commitCalled, 'Spy got called for the "commit" event.');
        equal(spy.commitThat, api, 'Spy called with correct context (API object).');
        ok(spy.postCommitCalled, 'Spy got called for the "pre-commit" event.');
        equal(spy.postCommitThat, api, 'Spy called with correct context (API object).');

        spy = {};
        api.bind('terminate', function() {
          spy.called = true;
          spy.that = this;
        });
        api.Terminate('');
        ok(spy.called, 'Spy got called for the "terminate" event.');
        equal(spy.that, api, 'Spy called with correct context (API object).');


      /**
       * We allow third party JS to register CMI paths, flag them as write only, read only, etc.
       */
      module('Register paths');
        api = new OpignoScorm2004API();

        ok(!api._implementedCMIDataPath('cmi.write_only_path'), 'Path cmi.write_only_path is not registered.');
        ok(!api._implementedCMIDataPath('cmi.read_only_path'), 'Path cmi.read_only_path is not registered.');
        ok(!api._implementedCMIDataPath('cmi.read_write_path'), 'Path cmi.read_write_path is not registered.');
        ok(!api._implementedCMIDataPath('cmi.read_write_path_2'), 'Path cmi.read_write_path_2 is not registered.');

        api.registerCMIPaths({
          'cmi.write_only_path': {
            writeOnly: true
          },
          'cmi.read_only_path': {
            readOnly: true
          },
          'cmi.read_write_path': { },
          'cmi.read_write_path_2': {
            writeOnly: false,
            readOnly: false
          }
        });

        ok(!api._implementedCMIDataPath('cmi.__unkown__'), "Registering paths don't make all paths magically available.");
        ok(api._implementedCMIDataPath('cmi.write_only_path'), 'Path cmi.write_only_path got registered correctly.');
        ok(api._implementedCMIDataPath('cmi.read_only_path'), 'Path cmi.read_only_path got registered correctly.');
        ok(api._implementedCMIDataPath('cmi.read_write_path'), 'Path cmi.read_write_path got registered correctly.');
        ok(api._implementedCMIDataPath('cmi.read_write_path_2'), 'Path cmi.read_write_path_2 got registered correctly.');


      /**
       * We allow third party JS to register data needed for the SCO at startup.
       */
      module('Register data');
        api = new OpignoScorm2004API();

        notEqual(api._getCMIData('cmi.__data_value__', true), 'value', 'Data cmi.__data_value__ got correctly registered.');
        notDeepEqual(api._getCMIData('cmi.__tree_value__', true), { tree: { leaf: 1 } }, 'Data cmi.__tree_value__ got correctly registered.');
        notDeepEqual(api._getCMIData('cmi.__array_value__', true), [{ key: 'value' }, { key: 'value2' }], 'Data cmi.__array_value__ got correctly registered.');

        api.registerCMIData('cmi.__data_value__', 'value');
        api.registerCMIData('cmi.__tree_value__', { tree: { leaf: 1 } });
        api.registerCMIData('cmi.__array_value__', [{ key: 'value' }, { key: 'value2' }]);

        equal(api._getCMIData('cmi.__data_value__', true), 'value', 'Data cmi.__data_value__ got correctly registered.');
        deepEqual(api._getCMIData('cmi.__tree_value__', true), { tree: { leaf: 1 } }, 'Data cmi.__tree_value__ got correctly registered.');
        deepEqual(api._getCMIData('cmi.__array_value__', true), [{ key: 'value' }, { key: 'value2' }], 'Data cmi.__array_value__ got correctly registered.');

      /**
       * @} End of "defgroup scorm_2004_object_specs".
       */



      /**
       * IMPORTANT NOTE:
       * REQ_9.x are all covered by the following specs, as GetLastError() is called many times
       * throughout the test suite.
       */


      module('API, initial state.');
        equal(api.GetLastError(), '0', 'REQ_3.1: The original error code is "0".');


      module('API::Initialize()');
        api = new OpignoScorm2004API();
        equal(api.Initialize(), 'false',             'REQ_3.2: Initializing the API without a parameter fails.');
        equal(api.GetLastError(), '201',             'REQ_3.2: Initializing the API without a parameter gives a 201 error.');
        equal(api.Initialize('any string'), 'false', 'REQ_3.2: Initializing the API with any string fails.');
        equal(api.GetLastError(), '201',             'REQ_3.2: Initializing the API with any string gives a 201 error.');
        equal(api.Initialize(654), 'false',          'REQ_3.2: Initializing the API with a parameter that is not a string fails.');
        equal(api.GetLastError(), '201',             'REQ_3.2: Initializing the API with a parameter that is not a string gives a 201 error.');
        equal(api.Initialize(null), 'false',         'REQ_3.2: Initializing the API with a parameter that is not a string fails.');
        equal(api.GetLastError(), '201',             'REQ_3.2: Initializing the API with a parameter that is not a string gives a 201 error.');
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
        api = new OpignoScorm2004API();
        equal(api.Terminate(''), 'false',            'REQ_5.4: Terminating the API before initializing fails.');
        equal(api.GetLastError(), '112',             'REQ_5.4: Terminating the API before initializing gives a 112 error.');
        // Initialize the communication.
        api.Initialize('');
        equal(api.Terminate(), 'false',              'REQ_3.2: Terminating the API without a parameter fails.');
        equal(api.GetLastError(), '201',             'REQ_3.2: Terminating the API without a parameter gives a 201 error.');
        equal(api.Terminate('any string'), 'false',  'REQ_3.2: Terminating the API with any string fails.');
        equal(api.GetLastError(), '201',             'REQ_3.2: Terminating the API with any string gives a 201 error.');
        equal(api.Terminate(654), 'false',           'REQ_3.2: Terminating the API with a parameter that is not a string fails.');
        equal(api.GetLastError(), '201',             'REQ_3.2: Terminating the API with a parameter that is not a string gives a 201 error.');
        equal(api.Terminate(null), 'false',          'REQ_3.2: Terminating the API with a parameter that is not a string fails.');
        equal(api.GetLastError(), '201',             'REQ_3.2: Terminating the API with a parameter that is not a string gives a 201 error.');
        equal(api.Terminate(''), 'true',             'REQ_5.1.1, REQ_5.2: Terminating the API with an empty string succeeds.');
        equal(api.GetLastError(), '0',               'REQ_5.2: Terminating the API with an empty string gives no error.');
        equal(api.Terminate(''), 'false',            'REQ_5.5: Terminating the API twice.');
        equal(api.GetLastError(), '113',             'REQ_5.5: Terminating the API twice gives a 113 error.');


      module('API::GetValue()');
        // @todo Missing specs for REQ_6.5 and REQ_6.7.
        api = new OpignoScorm2004API();
        equal(api.GetValue(''), '',                          'REQ_6.8: Requesting a value before initializing fails.');
        equal(api.GetLastError(), '122',                     'REQ_6.8: Requesting a value before initializing gives a 122 error.');
        // Initialize the communication.
        api.Initialize('');
        equal(api.GetValue('cmi.__value__'), 'value',        'REQ_6.2: Requesting a recognized value succeeds.');
        equal(api.GetLastError(), '0',                       'REQ_6.2: Requesting a recognized value gives no error.');
        equal(api.GetValue('cmi.__unknown__'), '',           'REQ_6.3: Requesting an unknown value fails.');
        equal(api.GetLastError(), '401',                     'REQ_6.3: Requesting an unknown value gives a 401 error.');
        equal(api.GetValue('__unknown__'), '',               'REQ_6.3: Requesting an unknown value fails.');
        equal(api.GetLastError(), '401',                     'REQ_6.3: Requesting an unknown value gives a 401 error.');
        equal(api.GetValue('cmi.__unimplemented__'), '',     'REQ_6.4: Requesting an unimplemented value fails.');
        equal(api.GetLastError(), '402',                     'REQ_6.4: Requesting an unimplemented value gives a 402 error.');
        equal(api.GetValue('cmi.__write_only__'), '',        'REQ_6.6: Requesting a write-only value fails.');
        equal(api.GetLastError(), '405',                     'REQ_6.6: Requesting an write-only value gives a 405 error.');
        equal(api.GetValue(''), '',                          'REQ_6.10: Requesting an empty string key fails.');
        equal(api.GetLastError(), '301',                     'REQ_6.10: Requesting an empty string key gives a 301 error.');
        equal(api.GetValue(), '',                            'REQ_6.10: Requesting a "null" key fails.');
        equal(api.GetLastError(), '301',                     'REQ_6.10: Requesting a "null" key gives a 301 error.');
        equal(api.GetValue('cmi.__test__.1.child'), '',      'REQ_6.11: Requesting an nonexistent child fails.');
        equal(api.GetLastError(), '301',                     'REQ_6.11: Requesting an nonexistent child gives a 301 error.');
        equal(api.GetValue('cmi.__test__._count'), '',       'REQ_6.12: Requesting the count property of an element that is not an array fails.');
        equal(api.GetLastError(), '301',                     'REQ_6.12: Requesting the count property of an element that is not an array gives a 301 error.');
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
        api = new OpignoScorm2004API();
        equal(api.SetValue('', ''), 'false',                            'REQ_7.9: Setting a value before initializing fails.');
        equal(api.GetLastError(), '132',                                'REQ_7.8: Setting a value before initializing gives a 132 error.');
        // Initialize the communication.
        api.Initialize('');
        equal(api.SetValue('cmi.__value__', 'value'), 'true',           'REQ_7.2: Setting a recognized value succeeds.');
        equal(api.GetLastError(), '0',                                  'REQ_7.2: Setting a recognized value gives no error.');
        equal(api.SetValue('cmi.__unknown__', 'value'), 'false',        'REQ_7.3: Setting an unknown value fails.');
        equal(api.GetLastError(), '401',                                'REQ_7.3: Setting an unknown value gives a 401 error.');
        equal(api.SetValue('__unknown__', 'value'), 'false',            'REQ_7.3: Setting an unknown value fails.');
        equal(api.GetLastError(), '401',                                'REQ_7.3: Setting an unknown value gives a 401 error.');
        equal(api.SetValue('cmi.__unimplemented__', 'value'), 'false',  'REQ_7.4: Setting an unimplemented value fails.');
        equal(api.GetLastError(), '402',                                'REQ_7.4: Setting an unimplemented value gives a 402 error.');
        equal(api.SetValue('cmi.__read_only__', 'value'), 'false',      'REQ_7.5: Setting a read-only value fails.');
        equal(api.GetLastError(), '404',                                'REQ_7.5: Setting a read-only value gives a 404 error.');
        equal(api.SetValue('cmi.__test__._count', 2), 'false',          'REQ_7.5: Setting a read-only value fails.');
        equal(api.GetLastError(), '404',                                'REQ_7.5: Setting a read-only value gives a 404 error.');
        equal(api.SetValue('cmi.__value__', { key: 'value' }), 'false', 'REQ_7.6: Setting a non-string value fails.');
        equal(api.GetLastError(), '406',                                'REQ_7.6: Setting a non-string value gives a 406 error.');
        equal(api.SetValue('', 'value'), 'false',                       'REQ_7.13: Providing an empty string path fails.');
        equal(api.GetLastError(), '351',                                'REQ_7.13: Providing an empty string path gives a 351 error.');
        equal(api.SetValue(null, 'value'), 'false',                     'REQ_7.13: Providing an non-string path fails.');
        equal(api.GetLastError(), '351',                                'REQ_7.13: Providing an non-string path gives a 351 error.');
        equal(api.SetValue(89, 'value'), 'false',                       'REQ_7.13: Providing an non-string path fails.');
        equal(api.GetLastError(), '351',                                'REQ_7.13: Providing an non-string path gives a 351 error.');
        equal(api.SetValue('cmi.__test__.0.child', 'value'), 'true',    'REQ_7.2: Setting a valid array value succeeds.');
        equal(api.GetLastError(), '0',                                  'REQ_7.2: Setting a valid array value gives no error.');
        equal(api.SetValue('cmi.__test__.2.child', 'value'), 'false',   'REQ_7.14: Setting an array key that is out of bounds fails.');
        equal(api.GetLastError(), '351',                                'REQ_7.14: Setting an array key that is out of bounds gives a 351 error.');
        // Terminate the communication.
        api.Terminate('');
        equal(api.SetValue('cmi.__value__', 'value'), 'false',          'REQ_7.10: Setting a value after termination fails.');
        equal(api.GetLastError(), '133',                                'REQ_7.10: Setting a value after termination gives a 123 error.');


      module('API::Commit()');
        // @todo Missing specs for REQ_8.2.1, REQ_8.3.
        api = new OpignoScorm2004API();
        equal(api.Commit(''), 'false',           'REQ_8.4: Committing the API before initializing fails.');
        equal(api.GetLastError(), '142',         'REQ_8.4: Committing the API before initializing gives a 142 error.');
        // Initialize the communication.
        api.Initialize('');
        equal(api.Commit(), 'false',             'REQ_3.2: Committing the API without a parameter fails.');
        equal(api.GetLastError(), '201',         'REQ_3.2: Committing the API without a parameter gives a 201 error.');
        equal(api.Commit('any string'), 'false', 'REQ_3.2: Committing the API with any string fails.');
        equal(api.GetLastError(), '201',         'REQ_3.2: Committing the API with any string gives a 201 error.');
        equal(api.Commit(123), 'false',          'REQ_3.2: Committing the API with a parameter that is not a string fails.');
        equal(api.GetLastError(), '201',         'REQ_3.2: Committing the API with a parameter that is not a string gives a 201 error.');
        equal(api.Commit(null), 'false',         'REQ_3.2: Committing the API with a parameter that is not a string fails.');
        equal(api.GetLastError(), '201',         'REQ_3.2: Committing the API with a parameter that is not a string gives a 201 error.');
        equal(api.Commit(''), 'true',            'REQ_8.2: Committing the API after initializing succeeds.');
        equal(api.GetLastError(), '0',           'REQ_8.2: Committing the API after initializing gives no error.');
        // Terminate the communication.
        api.Terminate('');
        equal(api.Commit(''), 'false',           'REQ_8.5: Committing the API after termination fails.');
        equal(api.GetLastError(), '143',         'REQ_8.5: Committing the API after termination gives a 143 error.');


      module('RTE Data Model Conformance');
        api = new OpignoScorm2004API();
        api.Initialize('');

        // REQ_55
        equal(api.GetValue('cmi._version'), '1.0', 'REQ_55.1, REQ_55.2, REQ_55.3: Requesting cmi._version succeeds and returns "1.0".');
/*
        // REQ_57
        equal(api.GetValue('cmi.comments_from_learner._children'), 'comment,location,timestamp',
          'REQ_57.1, REQ_57.1.2, REQ_75.1.3: Requesting cmi.comments_from_learner._children succeeds and returns a list of properties.');
        equal(api.SetValue('cmi.comments_from_learner._children', 'value'), 'false',
          'REQ_57.1.1: cmi.comments_from_learner._children is read-only.');
        equal(api.GetLastError(), '404',
          'REQ_57.1.1: cmi.comments_from_learner._children is read-only.');
        equal(api.GetValue('cmi.comments_from_learner._count'), 0,
          'REQ_57.2, REQ_57.1.2, REQ_75.2.3: Requesting cmi.comments_from_learner._children succeeds and returns a list of properties.');
        equal(api.SetValue('cmi.comments_from_learner._count', 'value'), 'false',
          'REQ_57.2.1: cmi.comments_from_learner._count is read-only.');
        equal(api.GetLastError(), '404',
          'REQ_57.2.1: cmi.comments_from_learner._count is read-only.');

        // REQ_72
      equal(api.GetValue('cmi.objectives._children'), 'id,score,success_status,completion_status,progress_measure,description',
        'REQ_72.1: Requesting cmi.objectives._children succeeds and returns "id,score,success_status,completion_status,progress_measure,description".');
      equal(api.GetValue('cmi.objectives._children'), 'id,score,success_status,completion_status,progress_measure,description',
        'REQ_72.1.1, REQ_72.1.3: Requesting cmi.objectives._children succeeds and returns "id,score,success_status,completion_status,progress_measure,description".');
      equal(api.SetValue('cmi.objectives._children', 'value'), 'false',
        'REQ_72.1.2: cmi.objectives._children is read only.');
      equal(api.GetValue('cmi.objectives._count'), 0,
        'REQ_72.2, REQ_72.2.3: Requesting cmi.objectives._count succeeds and returns the number of objectives.');
      equal(api.SetValue('cmi.objectives._count', 1), 'false',
        'REQ_72.2.1: cmi.objectives._count is read-only.');

      // Set some objectives.
      api.data.cmi.objectives.push({
        id: 'primary_obj',
        score: {},
        success_status: 'failed',
        completion_status: 'completed',
        progress_measure: '',
        description: ''
      });
      api.data.cmi.objectives.push({
        id: 'secondary_obj',
        score: {},
        success_status: 'passed',
        completion_status: 'completed',
        progress_measure: '',
        description: ''
      });

      equal(api.GetValue('cmi.objectives.0.id'), 'primary_obj',
        'REQ_72.3, REQ_72.3.1: Requesting cmi.objectives.n.id succeeds and returns the identifier.');
      equal(api.GetValue('cmi.objectives.1.id'), 'secondary_obj',
        'REQ_72.3, REQ_72.3.1: Requesting cmi.objectives.n.id succeeds and returns the identifier.');
      equal(api.SetValue('cmi.objectives.0.id', 'new id'), 'true',
        'REQ_72.3.1: cmi.objectives.n.id is also writable.');
      equal(api.GetValue('cmi.objectives.0.id'), 'new id',
        'REQ_72.3.1: cmi.objectives.n.id is also writable.');*/



    }
  };

})(jQuery, Drupal, window, OpignoScorm2004API);
