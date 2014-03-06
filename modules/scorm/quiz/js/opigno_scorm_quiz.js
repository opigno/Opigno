/**
 * @file
 * JS Quiz logic for SCORM player.
 */

;(function($, Drupal, window, undefined) {

  Drupal.behaviors.opignoScormQuiz = {

    attach: function(context, settings) {
      // Register our own paths.
      if (window.API_1484_11 !== undefined) {
        window.API_1484_11.registerCMIPath({

        });
      }
    }

  };

})(jQuery, Drupal, window);