<li class="scorm-ui-sco scorm-ui-player-tree-item<?php if (!empty($sco->children)) print ' scorm-ui-sco-aggregation scorm-ui-player-tree-item-has-children'; ?>" <?php print opigno_scorm_ui_player_sco_attributes($sco); ?>>
  <span class="scorm-ui-sco-title scorm-ui-player-tree-item-title"><?php print check_plain($sco->title); ?></span>
  <?php if (!empty($sco->children)): ?>
    <?php print theme('opigno_scorm_ui__player_tree', array('tree' => $sco->children)); ?>
  <?php endif; ?>
</li>