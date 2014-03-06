/**
 * @file
 * JS Quiz logic for SCORM player.
 */

;(function($, Drupal, window, undefined) {

  Drupal.behaviors.opignoScormQuiz = {

    attach: function(context, settings) {
      if (window.API_1484_11 !== undefined) {
        try {
          // Add '_children' properties, as we cannot set them server-side through PHP.
          window.API_1484_11.data.cmi.objectives._children = 'id,score,success_status,completion_status,progress_measure,description';
        }
        catch (e) { }
      }
    }

  };

})(jQuery, Drupal, window);