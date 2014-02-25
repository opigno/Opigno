/**
 * @file
 * JS UI logic for SCORM player.
 *
 * @see js/lib/player.js
 * @see js/lib/api.js
 */

;(function($, Drupal, window, undefined) {

  Drupal.behaviors.opignoScormUIPlayer = {

    attach: function(context, settings) {
      // Get all SCORM players in our context.
      var $players = $('.scorm-ui-player', context);

      // If any players were found...
      if ($players.length) {
        // Register each player.
        $players.each(function() {
          var element = this,
              $element = $(element),
              // Create a new OpignoScormUIPlayer().
              player = new OpignoScormUIPlayer(element);

          player.init();

          // Bind event listeners for different API events.
          API_1484_11.bind('commit', function(value, data) {

          });

          // Add a class to the player, so the CSS can style it differently if needed.
          $element.addClass('js-processed');
        });
      }
    }

  };

})(jQuery, Drupal, window);