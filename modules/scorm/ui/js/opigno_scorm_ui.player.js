/**
 * @file
 * JS logic for SCORM player.
 */

;(function($, Drupal, window, undefined) {

  Drupal.behaviors.opignoScormUIPlayer = {

    attach: function(context, settings) {
      var $players = $('.scorm-ui-player', context);
      if ($players.length) {
        $players.each(function() {
          var element = this,
              $element = $(element),
              player = new OpignoScormUIPlayer(element);

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

          $element.addClass('js-processed');
        });
      }
    }

  };

  /**
   * Representation of the SCORM player object.
   *
   * @param element
   * @constructor
   */
  var OpignoScormUIPlayer = function(element) {
    this.el = element;
    this.$el = $(element);
    this.$iframe = this.$el.find('.scorm-ui-player-iframe-wrapper iframe');
    this.iframe = this.$iframe[0];
  }

  // @const SCO integration path pattern.
  OpignoScormUIPlayer.PATH_PATTERN = 'opigno-scorm/ui/player/sco/%sco_id';

  /**
   * Launch another SCO.
   *
   * @param scoID
   */
  OpignoScormUIPlayer.prototype.launch = function(scoID) {
    this.iframe.src = Drupal.settings.basePath + OpignoScormUIPlayer.PATH_PATTERN.replace('%sco_id', scoID);
  }

  /**
   * Implementation of the SCORM API.
   *
   * @constructor
   */
  var OpignoScormUI2004API = function() { };

  /**
   * Implements Initialize().
   */
  OpignoScormUI2004API.prototype.Initialize = function() {
    console.log('Initialize');
  }

  /**
   * Implements Finish().
   */
  OpignoScormUI2004API.prototype.Finish = function() {
    console.log('Finish');
  }

  /**
   * Implements GetValue().
   */
  OpignoScormUI2004API.prototype.GetValue = function() {
    console.log('GetValue');
  }

  /**
   * Implements SetValue().
   */
  OpignoScormUI2004API.prototype.SetValue = function() {
    console.log('SetValue');
  }

  /**
   * Implements Commit().
   */
  OpignoScormUI2004API.prototype.Commit = function() {
    console.log('Commit');
  }

  /**
   * Implements GetLastError().
   */
  OpignoScormUI2004API.prototype.GetLastError = function() {
    console.log('GetLastError');
  }

  /**
   * Implements GetErrorString().
   */
  OpignoScormUI2004API.prototype.GetErrorString = function() {
    console.log('GetErrorString');
  }

  /**
   * Implements GetDiagnostic().
   */
  OpignoScormUI2004API.prototype.GetDiagnostic = function() {
    console.log('GetDiagnostic');
  }

  // Make it globally available.
  window.API = window.API_1484_11 = new OpignoScormUI2004API();

})(jQuery, Drupal, window);