<?php
/**
 * @todo
 */
?>
<ul class="opigno-add-content optigno-add-content-to-<?php print $og_type; ?>">
  <?php foreach ($tools as $tool): ?>
    <?php if (isset($tool['create_label']) && isset($tool['create_url'])): ?>
      <li><a href="<?php print $tool['create_url']; ?>"><?php print $tool['create_label']; ?></a></li>
    <?php endif; ?>
  <?php endforeach; ?>
</ul>