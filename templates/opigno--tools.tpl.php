<?php
/**
 * @file
 * Render a list of tools. Each tool is a themed tool item.
 */
?>
<div class="opigno-tools">
  <?php foreach ($tools as $tool): ?>
    <?php print render($tool); ?>
  <?php endforeach; ?>
</div>