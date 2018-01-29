LightsUp!

--- Release Notes ---

* Version 1.1 Release
** Fixed a couple bugs in when the fade to next/prev buttons were enabled.

* Version 1.1rc1
** Automatically unfreeze DMX when Fade to Next or Prev is selected

* Version 1.1b4
** Allow fade to End of Show, like any other cue
** Suppress fade requests when a fade is already in progress
** Fix Cancel fade to stop in place and allow continue in either direction
** Change hotkey for New Cue to be Insert instead of Return/Enter.  This allows Return/Enter to act per Windows standard behavior.

* Version 1.1b3
** Removed unused (and usually invisble) keylock flags from the main window status bar
** Fixed range check on fade time boxes to happen on killfocus, not on edit
** Save active cue when "DMX Freeze" is clicked and restore it when "DMX Freeze" is unclicked
** Confirm overwrite of existing Look name
** Apply results of channel map changes immediately when map editor is closed with OK button
** Validate ranges of DMX addresses and percentages in Channel Map editor

* Version 1.1b2
** Fixed old reverse fade time bug
** Changed window titles
** Allowed resizing of the Scene Control window
** Other minor tweaks.

* Version 1.1b1
** Removed non-functional UI controls
** Added "DMX Freeze" button to allow editing of cues without affecting onstage lights
** Extended fade max time to 300 seconds to match industry standard
** Changed channel mixing model to add inputs instead of taking the max.  This improves the look of most fades.
