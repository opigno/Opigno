/**
 * @file
 * JS Quiz logic for SCORM player.
 */

;(function($, Drupal, window, undefined) {

  Drupal.behaviors.opignoScormQuiz = {

    attach: function(context, settings) {
      if (window.API_1484_11 !== undefined) {
        // Register our own paths.
        if (settings.opignoScormQuiz && settings.opignoScormQuiz.cmiPaths) {
          window.API_1484_11.registerCMIPaths(settings.opignoScormQuiz.cmiPaths);
        }

        // Set default data.
        if (settings.opignoScormQuiz && settings.opignoScormQuiz.cmiData) {
          for (var item in settings.opignoScormQuiz.cmiData) {
            window.API_1484_11.registerCMIData(item, settings.opignoScormQuiz.cmiData[item]);
          }
        }

        // Listen on commit event, so we can store our data.
        window.API_1484_11.bind('commit', function() {
          // @todo
        });
      }
    }

  };

})(jQuery, Drupal, window);