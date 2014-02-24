Opigno Scorm
============

Opigno Scorm exposes an API for extracting and handling SCORM packages (ZIP files, also known as PIFs).
It's important to realize the Opigno Scorm **does not offer any UI components as is**! The Opigno Scorm UI submodule exposes a custom field, a player, etc. Opigno Scorm, on the other hand, is mainly an API for extracting SCORM data (from the manifest XML file).

Opigno SCORM is compatible with SCORM 2004 (also known as SCORM 1.3). Previous versions of SCORM could work, but might contain specificities that are incompatible.



The way SCORM works
===================

This is a *very* simple introduction to the way SCORM packages work. For more information, check out http://scorm.com/scorm-explained/technical-scorm/content-packaging.


The manifest file
-----------------

Each SCORM package contains a *imsmanifest.xml* XML file. This file contains information about the package content, the way it's supposed to be used, etc.

We don't need to use all the data from the manifest file, as we implement a *player*. The *player* is an iframe that contains the SCORM content - we don't "import" anything, or convert it to our own "format".

For instance, the manifest contains an entire section called *resources*. Most of these entries can be ignored (unless explicitly referenced by an item - more below), as we will never use the resources outside of the player context.

The big (and very important) part is the *organization* elements. Each *organization* contains a list of *items*. Each *item* defines a "step" in the course (think of them as PowerPoint slides). When an *item* contains child items, it is called an *aggregation*. Usually, *aggregations* do not "display" anything on their own - they just define a group of items. This gives the typical tree structure we find with certain SCORM packages. There is no limit to the depth of *aggregations*: *aggregations* can contain other *aggregations*, and so forth. The *organization* is a special type of *aggregation*: it's the root of the tree.

Each *item* has several attributes, the most important being "identifierref" and "launch":

 1. If the "launch" attribute is set, it will reference a file (usually HTML) that must be launched in the player iframe when the student comes to this item.
 2. If the "identifierref" attribute is set, the *item* references a *resource*, with an identical *identifier* attribute. The *resource*, in turn, will have an "href" attribute. This "href" attribute is to be treated as the "launch" attribute of the item. This is why, in `opigno_scorm_extract_manifest_data()`, the *items* (called SCOs) and *resources* are combined in `opigno_scorm_combine_manifest_sco_and_resources()`.


Assets and SCOs (Shareable Content Object)
------------------------------------------

An *item* in the manifest file represents either an "asset" or an "sco". There's no difference from the player perspective - they are both treated the same way for simplicity. The difference is that an "asset" does not communicate with the LMS, and an SCO does. So, for example, if a slide contains only text and images, it's called an "asset". If it contains a quiz, it's an SCO.



The SCORM player
================

The actual player is defined by opigno_scorm_ui. Opigno Scorm UI defines a custom file field and field formatter, that allow users to upload SCORM PIFs and display them inside the player.


Simple navigation
-----------------

The player contains a tree of items, each item having a unique ID (corresponding to its ID in the database). When the player is constructed, a "start" item is defined. This is either where the player last left off (to be done) or simple the first item in the tree.

The player has an iframe that opens a URL like `opigno-scorm/ui/player/sco/{sco_id}`. Note that we call the item a SCO, where technically it could be an "asset" - there's no difference for the player. Behind the scene, this URL is authenticated (like any Drupal path), and if the user has access, the SCO is shown. Different ways to show content:

 1. In case of an HTML SCO, a 301 redirect is performed (still inside the iframe) to the extracted SCORM package SCO file. This file is determined by the "launch" attribute (or linked resource "href" attribute). Because this file contains relative assets (like CSS or JS), these files will get included correctly. It is important to note that, because of this, **the actual SCORM content must be publicly available**. The URL is "hidden" from the user, thanks to the iframe. But the actual content must be publicly accessible.
 2. In case of a different format (video, audio), the player wraps the content in a correct markup (like a VideoJS player) and pushes this back to the user (to be done).

When a user clicks on an item in the navigation tree, the URL of the iframe is simply updated with the correct SCO ID. This refreshes its content, displaying the next (or previous) SCO.


Restricted navigation
---------------------

Todo.


LMS Communication
-----------------






