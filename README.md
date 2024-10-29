# BigSequencer
The BigSequencer is a randomly populated grid of notes that get sent over MIDI.
You can use it to quickly generate new ideas or to create easy ambient music.

Check out a video demo [here](https://www.youtube.com/watch?v=dx0lfoL8MUg)
![Big Sequencer Demo Image](resource/sequencer.png)

It has multiple **cursors** that iterate along the grid. Each cursor has multiple parameters:
- active (whether the cursor will play notes)
- interval (quarter note, half note, etc)
- note length (percentage of the interval that the note should play)
- pitch offset (adjust the note pitch by this many semitones)
- velocity (how hard the key press will be)
- probability (chance that the cursor will play the current note)

Use the **retrigger** parameter to make the sequencer restart at the beginning on playback.

## Usage
On Windows, move the VST folder to your VST plug-in location. The default is usually `C:\Program Files\Common Files\VST3`.

Generally, once you have the VST installed and opened in your DAW of choice, set the plugin's MIDI output channel to match an instrument's input channel.

Turn cursors on and off to have more "hands" playing notes at a time.

Changing the width, height, min note, max note, root note, scale, or fill chance will regenerate the note grid.

### Usage in FLStudio
Open up BigSequencer in a channel and click the gear in the plugin window for detailed settings. Then click the VST wrapper settings button with the plug & gear.
Set your output port to match the either the input port you set in another synth plugin, or send it to the output port you configured in Options -> MIDI Settings.

### Usage in Ableton
Open up BigSequencer in a new audio track. In a MIDI track under the "MIDI From" section, select BigSequencer for both the "Input Type" and "Input Channel".
Under "MIDI To", select a plugin or configured external output to send to. Make sure to set Monitor on BigSequencer to "In" on to arm the track so the signals will be sent.

### Usage in Reaper
Load BigSequencer as a virtual instrument in a new track. Under the track settings at the bottom, open up BigSequencer's "Sends, Receives, and Hardware Output Options".
If you have another virtual instrument you want to send to, add it under "Sends". If you have external hardware, add it under "Midi Hardware Output".

## Future Plans
- There are more cursor parameters that should be implemented:
    - offset (how off beat the note will be played)
    - start position (where on the grid the note will start)
    - transition rule (which note the cursor should move to next)
        - this allows for varying patterns on our grid
        - but we need to find out a good way to do it
- More note generation methods (other noise types?)

## Building
To build the app, you will need the Steinberg VST3 SDK:

```
git submodule update --init --recursive
```

Then in the BigSequencer project, we can build using CMake:
```
mkdir build
cd build
cmake ..
cmake --build .
```

## Support Me
If you like my work, consider supporting me:

[![paypal](https://www.paypalobjects.com/en_US/i/btn/btn_donateCC_LG.gif)](https://www.paypal.com/ncp/payment/UBXNLMZ39X6G6)
