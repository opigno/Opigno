<div class="scorm-ui-player">
  <div class="scorm-ui-player-tree-wrapper">
    <?php print theme('opigno_scorm_ui__player_tree', array('tree' => $tree)); ?>
  </div>

  <div class="scorm-ui-player-iframe-wrapper">
    <iframe src="<?php print url("opigno-scorm/ui/player/sco/{$start_sco->id}"); ?>"></iframe>
  </div>
</div>