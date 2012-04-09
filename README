Uploads
=======

By Rob Myers <rob@robmyers.org>

Uploads is a DIY Transhumanist system for creating low-resolution personality uploads using currently available consumer-level technology.


Required Hardware
=================

To run Uploads you will need a Microsoft Kinect and a Neurosky Mindwave connected to a computer with more than 2GB of RAM.


Building The Software
=====================

Uploads has been developed on a GNU/Linux system using Python 2.7 and gcc 4.x . 

The Python code requires Puzzlebox Synapse to access the Mindwave:

http://brainstorms.puzzlebox.info/static.php?page=downloads

The c++ code requires OpenFrameworks (currently v007):

http://www.openframeworks.cc/download/

Other libraries, particularly Freenect, can be installed using your distro's package manager.

upload_capture can simply be run from the command line (see the next section).

upload_app can be built using make. e.g.:

cd uploads/upload_app
make


Running The Software
====================

upload_capture
--------------

upload_capture is a Python application that captures Mindwave eeg data and Kinect depth image data. To use it, connect the Mindwave dongle, put on the Mindwave head set and switch it on, and start Puzzlebox Synapse. Then connect the Kinect (you can check that the Kinect is working using glview). Finally type something like:

cd uploads/upload_capture
python upload_capture.py me-20120415.upload

This will prompt you to (pretend to) feel various emotions, and capture the resulting data to a folder called me-20120415.upload.

upload_app
----------

upload_app is a c++ application that re-animates the upload data. It needs an Internet connection to run in order to access the Twitter Streaming API, which it uses to create the emotional state of the upload.

To run upload_app on the output of upload_capture, type something like:

cd uploads/upload_app/bin
./upload_app --data \ 	    
  /home/someuser/projects/uploads/upload_capture/me-20120415.upload \ 
  --twitter_auth twitteruser:password123

There are many other flags that can be passed to upload_app, see the --help option for more information on these.


Theory
======

Cyberpunk
---------

[Uploading human consciousness](https://secure.wikimedia.org/wikipedia/en/wiki/Mind_uploading) to a computer was a common trope of [Cyberpunk](https://secure.wikimedia.org/wikipedia/en/wiki/Cyberpunk) literature. [Pat Cadigan's(https://secure.wikimedia.org/wikipedia/en/wiki/Pat_Cadigan) "Synners]", the TV series "[Max Headroom](https://secure.wikimedia.org/wikipedia/en/wiki/Max_Headroom_%28character%29)", the movie "[The Lawnmower Man](https://secure.wikimedia.org/wikipedia/en/wiki/The_Lawnmower_Man_%28film%29)", and William Gibson's short story "[The Winter Market](https://secure.wikimedia.org/wikipedia/en/wiki/The_Winter_Market)" all feature uploads. But there is a more serious side to uploading, exemplified by [Hans Moravec](https://secure.wikimedia.org/wikipedia/en/wiki/Hans_Moravec)'s book "Mind Children".

The titular character of the "Max Headroom" TV series is the uploaded consciousness of a television reporter. Max is a [glitch art](https://secure.wikimedia.org/wikipedia/en/wiki/Glitch_art) upload, an example of noise signifying authenticity that predates Trip-Hop. In the episode "[Detities](http://www.maxheadroom.com/mh_episode_22.html)" an [Extropian](https://secure.wikimedia.org/wikipedia/en/wiki/Extropianism) televangelist church wishes to replace their primitive uploads, essentially video loops of the dead who intone a familiar phrase to reassure their visitors, with the more advanced Max Headroom process.

Transhumanism
-------------

Moravec proposes that the brain be scanned, layer by layer, by a destructive system that would remove neurons whose values had already been transferred to computer. The brainless body would then be left to die. "The Winter Market" dramatises the question of whether the resulting data is really the person whose physical existence was terminated in order to create it.

This [Transhumanist](https://secure.wikimedia.org/wikipedia/en/wiki/Transhumanism) resolution to the [mind/body problem](https://secure.wikimedia.org/wikipedia/en/wiki/Mind_body_problem) ironically separates the mind from the body by asserting that the mind is purely a localized product of part of the body. The technology required to do so is not yet available. But people's social lives are atrophying as they become simply vehicles for maintaining digital social media simulacra of their lives.

There is a new trend in Transhumanism, if not inspired by [Maker culture](https://secure.wikimedia.org/wikipedia/en/wiki/Maker_subculture) then reflecting it, to not wait for future technological or economic developments before getting on with upgrading oneself. This trend is exemplified by sensory enhancements such as [neodymium magnet implants](http://www.stevehaworth.com/wordpress/index.php/welcome-from-steve-haworth/magnetic-faq) and [haptic direction senses](http://sensebridge.net/projects/northpaw/). This kitchen table Transhumanism realises some of the content of Transhumanism at the expense of disappointing its fetishised military-industrial-complex inspired form.

The quantum free will confections of Roger Penrose's "[The Emperor's new mind](https://secure.wikimedia.org/wikipedia/en/wiki/The_Emperor%27s_New_Mind)" and the role of [embodiment](https://secure.wikimedia.org/wikipedia/en/wiki/Embodied_cognition) and the rest of the nervous system in consciousness aside, for uploading the state of the brain (which in Moravec's scheme is coterminous with the mind) to be a successful transcription of personhood we must know the level of detail at which further detail would be irrelevant. We do not, but it is certainly below the level of [Electroencephalography](https://secure.wikimedia.org/wikipedia/en/wiki/Eeg) (EEG).

The Project
-----------

"Uploads" uses [consumer EEG headset equipment](http://store.neurosky.com/products/mindwave-1) to digitize the least possible record of the state of the human brain in different emotional states (chosen from [Descartes' emotional categories](https://secure.wikimedia.org/wikipedia/en/wiki/Emotion_classification#Basic_and_Complex_Emotions)). It is emotion that makes us human, after all. A physical expression of emotion is digitized using the [Kinect](http://www.xbox.com/kinect). The brainwave and facial states are rendered in software in response to the emotional environment of [Twitter](https://twitter.com/). Using consumer-grade equipment ties the project to Maker culture and kitchen table Transhumanism.

I expect this project to be misunderstood in two ways, both failures of irony. Firstly as a demonstration that uploading is practical in a limited way now so its full imagined form will be possible soon. Secondly as an unreflective embodiment of that claim. This is in itself part of the work, which is art by virtue of being a reflexive intervention in perception.

Dedication
==========

This project is dedicated to the memory of Internet VR pioneer Evie Matthieson.
