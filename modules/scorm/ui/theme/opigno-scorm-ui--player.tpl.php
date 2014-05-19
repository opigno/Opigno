<div class="scorm-ui-player" id="scorm-ui-player-scorm-<?php print $scorm_id; ?>" data-scorm-id="<?php print $scorm_id; ?>">
  <?php if (!empty($tree)): ?>
    <div class="scorm-ui-player-tree-wrapper">
      <?php print theme('opigno_scorm_ui__player_tree', array('tree' => $tree)); ?>
    </div>
  <?php endif; ?>

  <div class="scorm-ui-player-iframe-wrapper">
    <iframe src="<?php print url("opigno-scorm/ui/player/sco/{$start_sco->id}"); ?>"></iframe>
  </div>
</div>