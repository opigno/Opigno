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
    this.$lis = this.$el.find('.scorm-ui-player-tree-wrapper li');
  }

  // @const SCO integration path pattern.
  OpignoScormUIPlayer.PATH_PATTERN = 'opigno-scorm/ui/player/sco/%sco_id';

  /**
   * Initialize the player.
   */
  OpignoScormUIPlayer.prototype.init = function() {
    this.initNavigation();
    this.initEvents();
  }

  /**
   * Initialize the navigation logic.
   */
  OpignoScormUIPlayer.prototype.initNavigation = function() {
    // Tree navigation.
    // Register the tree click handlers.
    this.registerTreeClick();

    // Handle control modes.
    this.handleTreeControlModes();
  }

  /**
   * Register navigation tree click events.
   */
  OpignoScormUIPlayer.prototype.registerTreeClick = function() {
    var player = this;

    // Register click events for each <li> in the navigation tree.
    this.$lis.click(function(e) {
      e.stopPropagation();

      var $this = $(this);

      // Exit if this has been disabled.
      if ($this.hasClass('disable-click')) {
        return;
      }

      if ($this.data('sco-can-launch')) {
        player.launch($this.data('sco-id'));
      }
      else {
        // Trigger click on child item, if any.
        // This will happen recursively until one item can be launched.
        $this.find('li:eq(0)').click();
      }
    });
  }

  /**
   * Handle control modes.
   *
   * SCORM defines several ways the user can navigate between SCOs.
   * This data is stored directly in the tree as data attributes.
   * Make sure the user can only navigate the way the SCORM intended
   * her to.
   */
  OpignoScormUIPlayer.prototype.handleTreeControlModes = function() {
    var player = this;
    this.$lis.each(function() {
      var $li = $(this);

      // If this is the root element, don't check any control mode.
      if ($li.hasClass('root') || $li.parent().parent().hasClass('scorm-ui-player-tree-wrapper')) {
        $li.addClass('root');
        return;
      }

      // Control modes are only for aggregations.
      if ($li.hasClass('scorm-ui-sco-aggregation')) {
        // If this is an aggregation, and the control mode "choice" is false,
        // disable child lis.
        if (!$li.data('sco-control-mode-choice')) {
          // @todo
          // $li.find('> ul').hide().find('li').addClass('disable-click');
        }

        // If this is an aggregation, and the control mode "flow" is false,
        // disable the flow navigation (forward-backward navigation).
        if (!$li.data('sco-control-mode-flow')) {
          // @todo
          // $li.find('> ul').hide().find('li').addClass('disable-click');
        }
      }
    });
  }

  /**
   * Register events.
   *
   * Use events to update the navigation and UI.
   */
  OpignoScormUIPlayer.prototype.initEvents = function() {
    var player = this;

    // Each time a SCO has finished loading, trigger the sco:loaded event on the corresponding SCO
    // item in the tree.
    this.$iframe.load(function() {
      player.$lis.filter('[data-sco-id="' + player.$iframe.data('sco-id') + '"]').trigger('sco:loaded');
    });

    // When a SCO is loading, add a "loading" class to the SCO tree item for styling.
    this.$lis.bind('sco:loading', function() {
      $(this).addClass('loading');
    });


    // When a SCO has finished loading, remove the "loading" class from the SCO tree item
    // and check what control modes are available.
    this.$lis.bind('sco:loaded', function() {
      $(this).removeClass('loading');

      // Check if flow is allowed.
    });
  }

  /**
   * Launch a SCO.
   *
   * @param scoID
   */
  OpignoScormUIPlayer.prototype.launch = function(scoID) {
    // Set the SCO ID as an attribute on the iframe.
    this.$iframe.data('sco-id', scoID);

    // Load the SCO.
    this.iframe.src = Drupal.settings.basePath + OpignoScormUIPlayer.PATH_PATTERN.replace('%sco_id', scoID);
    
    // Trigger a loading event on the tree item.
    this.$lis.filter('[data-sco-id="' + scoID + '"]').trigger('sco:loading');
  }

  // Export.
  window.OpignoScormUIPlayer = OpignoScormUIPlayer;

})(jQuery, Drupal, window);