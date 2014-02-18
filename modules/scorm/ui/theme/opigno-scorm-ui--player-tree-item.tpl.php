<li class="scorm-ui-sco scorm-ui-player-tree-item" data-sco-id="<?php print $sco->id; ?>" data-sco-can-launch="<?php print (int) !empty($sco->launch); ?>">
  <?php print check_plain($sco->title); ?>
  <?php if (!empty($sco->children)): ?>
    <?php print theme('opigno_scorm_ui__player_tree', array('tree' => $sco->children)); ?>
  <?php endif; ?>
</li>