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

      // Initiate the API.
      if (window.API_1484_11 === undefined) {
        window.API_1484_11 = new OpignoScorm2004API(settings.scorm_data || {});
      }

      // Register CMI paths.
      if (settings.opignoScormUIPlayer && settings.opignoScormUIPlayer.cmiPaths) {
        window.API_1484_11.registerCMIPaths(settings.opignoScormUIPlayer.cmiPaths);
      }

      // Register default CMI data.
      if (settings.opignoScormUIPlayer && settings.opignoScormUIPlayer.cmiData) {
        for (var item in settings.opignoScormUIPlayer.cmiData) {
          window.API_1484_11.registerCMIData(item, settings.opignoScormUIPlayer.cmiData[item]);
        }
      }

      // Listen on commit event, and send the data to the server.
      window.API_1484_11.bind('commit', function(value, data) {

      });

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

          // Add a class to the player, so the CSS can style it differently if needed.
          $element.addClass('js-processed');
        });
      }
    }

  };

})(jQuery, Drupal, window);