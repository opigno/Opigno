<ul class="scorm-ui-player-tree">
  <?php foreach($tree as $sco): ?>
    <?php print theme('opigno_scorm_ui__player_tree_item', array('sco' => $sco)); ?>
  <?php endforeach; ?>
</ul>