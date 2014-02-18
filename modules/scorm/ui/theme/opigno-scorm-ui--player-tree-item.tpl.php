<li class="scorm-ui-sco scorm-ui-player-tree-item<?php if (!empty($sco->children)) print ' scorm-ui-sco-aggregation scorm-ui-player-tree-item-has-children'; ?>" data-sco-id="<?php print $sco->id; ?>" data-sco-can-launch="<?php print (int) !empty($sco->launch); ?>">
  <span class="scorm-ui-sco-title scorm-ui-player-tree-item-title"><?php print check_plain($sco->title); ?></span>
  <?php if (!empty($sco->children)): ?>
    <?php print theme('opigno_scorm_ui__player_tree', array('tree' => $sco->children)); ?>
  <?php endif; ?>
</li>