<?php
/**
 * @file
 * Tool template file. Renders the tool on the tools page.
 * Available variables:
 *  - $machine_name (filtered)
 *  - $name (filtered)
 *  - $path (escaped)
 *  - $description (escaped)
 *  - $tool (array containing any information the tool module defines. Unfiltered).
 */
?>
<div class="opigno-tool opigno-<?php print str_replace('_', '-', $machine_name); ?>-tool">
  <h4><?php print l($name, $path); ?></h4>
  <p><?php print $description; ?></p>
</div>