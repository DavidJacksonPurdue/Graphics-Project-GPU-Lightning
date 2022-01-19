Wave Simulation Program made by David Jackson, jacks668, 3/16/21

I implemented option A of the wave simulation problem--a summation of four seperate sine waves.

====================================HOW TO USE======================================================================

To control the parameters of the 4 sine waves, use the alternative window opened up called
"GPGPU Settings". Press the "Restart" button to apply the changes. These values can be changed on the fly without
the need for you to restart the application (Extra Credit). You can also click on the screen to reset the 
simulation, however this will not apply any of the changes made to the settings. In order to do that, you must
press the "Restart" button in the "GPGPU Settings" window.

All 4 waves start with the same direction, speed, amplitude, and wavelength. You can modify these values
in the "GPGPU Settings" window within certain limits. To "turn a wave off", simply set its amplitude to 0. If 
all 4 waves' amplitudes are 0, the screen will just be a still deep blue. With the initial settings, the simulation
does not greatly resemble water as all four waves have the same values. From my testing, I found that changing the
x direction of one wave to -1, changing the y direction of another wave to -1, and keeping all other settings the
same generated a visually interesting water effect. You can use whatever settings you like, but these values gave
good looking results for me.

You can additionally "control" the lighting of the simulation. You can't change the intensity of the lighting as
that is hardcoded, but you can select an option to place the light in any of the four corners of the screen. By 
default, the light projects from the bottom left corner of the simulation.

Finally, the "island" of the simulation is just a simple green circle placed in the middle of the screen that is
more designed to demonstrate the visual effect of waves bouncing off of it. You can toggle the inclusion of this
island with the check box "Include Island?" of the "GPGPU Settings" window. By default the island is not present.

====================================KNOWN BUGS======================================================================

There is a known slight disconnect between the default variable values used by the simulation and those stored in
the "GPGPU Settings" window. This bug has no real effect on the functionality of the simulation. The only thing to
note is that pressing "Restart" with the default values in "GPGPU Settings" will result in a different looking
scene to what is shown on boot up.