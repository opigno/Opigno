/**
 * @file
 * JS Quiz logic for SCORM player.
 */

;(function($, Drupal, window, undefined) {

  Drupal.behaviors.opignoScormQuiz = {

    attach: function(context, settings) {
      if (window.API_1484_11 !== undefined) {
        // Listen on commit event, so we can store our data.
        window.API_1484_11.bind('commit', function() {
          // @todo
        });
      }
    }

  };

})(jQuery, Drupal, window);