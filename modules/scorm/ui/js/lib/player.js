/**
 * @file
 * Defines the SCORM player object.
 *
 * The SCORM player has simple methods for navigating between SCOs.
 * It does not communicate with the SCOs.
 */

;(function($, Drupal, window, undefined) {

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

  // Export.
  window.OpignoScormUIPlayer = OpignoScormUIPlayer;

})(jQuery, Drupal, window);