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
        // Register events for each player.
        $players.each(function() {
          var element = this,
              $element = $(element),
              // Create a new OpignoScormUIPlayer().
              player = new OpignoScormUIPlayer(element);

          // Register click events for each <li> in the navigation tree.
          $element.find('li').click(function(e) {
            e.stopPropagation();

            var $this = $(this);

            if ($this.data('sco-can-launch')) {
              player.launch($this.data('sco-id'));
            }
            else {
              // Trigger click on child item, if any.
              // This will happen recursively until one item can be launched.
              $this.find('li:eq(0)').click();
            }
          });

          // Add a class to the player, so the CSS can style it differently if needed.
          $element.addClass('js-processed');
        });
      }
    }

  };

})(jQuery, Drupal, window);