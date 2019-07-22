
# lasercut.svg

This creates the shapes required to lasercut a loom from 3mm
material. I used baltic birch plywood, MDF would likely do just as
well.

# Other things needed

- 4mm diameter metal rod
- 25mm diameter wooden rod
- Solenoids - "DC 12V 10mm Push-Pull Actuator Frame Type Electromagnet
  Solenoid Electric Magnet" - can be found for about £2
  each. With this design, I recommend getting either 12 or 16.
- Relays - I used a board with 16 of them. "16-channel Relay Module
  Board 5v12v".
- Arduino (and 'jumper jerky' wire) to control the relays
- PSU for the relays
- PSU for the solenoids - This needs some power. I used "RS PRO 102W
  Embedded Switch Mode Power Supply SMPS, 8.5A, 12V dc". (You might
  need even more amps to power all your relays at once)
- Loom weights, one per thread - I recommend small plastic weighted
  kumihimo bobbins for this, then you can easily store the warp
  inside, but you can also use smallish bolts and wrap the warp around..
- Screws to attach the solenoids, I think I needed M2.5 screws for
  mine.
- wire to connect up the solenoids etc. This needs to be fairly heavy
  duty, to take the amps!
- diodes to protect everything against kickback from the
  solenoids. I still need to add these myself!

# Rough instructions

My plywood was bendier in one direction - basically if there is a
grain have it go horizontal otherwise E1/E2 will bend too much with
the weight of the solenoids.

A* and B* are the main legs and supports. D0 slots into the support
and is where I screw my relay switch board.

Below you see 'lollypop stick' heddles. Each has a 'mouth', two of the
things that look like capital As slot into each 'mouth', creating a
nice strike area for the solenoid. Half of the heddles have bends, so
the solenoids can be on two axes (doubling the number you can have in
a given amount of space).

The circles above the heddles are wooden washers to space the lollypop
sticks out so they line up with the solenoids, I woodglue one to each
heddle. These are then threaded onto a 4mm metal rod. I add a small
metal washer or two to line everything up with the solenoids.

E1 and E2 are what the solenoids are screwed on to.

C* are assembled with wood glue, and then slotted into the front of
the legs A4, with E1 and E2 and the 4mm rod slotted in using the
provided holes.

Contact alex@slab.org with any questions - I'd be happy offer advice
to get you up and running.
