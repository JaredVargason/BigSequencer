# BigSequencer

The Big Sequencer is a big grid of notes. You can change its width and height.

It has multiple "cursors" that iterate along the grid. Each cursor has multiple parameters:
- active (whether the cursor will play notes)
- interval (quarter note, half note)
- note length (percentage of the interval that the note should play)
- pitch offset (adjust the note pitch by this amount)

With planned parameters:
- offset (how off beat the note will be played)
- start position (where on the grid the note will start)
- transition rule (which note the cursor should move to next)