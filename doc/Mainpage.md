

\mainpage

![That is the robot](the_robot.jpg "That is the robot")


\section intro_sec Introduction
Welcome to a brief introduction of a project executed by a
student of the university of Kassel. \n The goal of this project is to create reliable 
software that controlls the robot to drive three laps on a track (<a href="track_img.jpg" target="_blank"><b>example track</b></a>) consisting of a black line, autonomously. \n
There is a black square placed on the track which acts as the startfield. \n 
The robot has three sensors which enable it to detect the black line aswell as the startfield so it can take action accordingly. \n\n
In addition to that the robot starts driving only after being placed on the startfield which initiates a 15s countdown. \n
Upon being moved the countdown shall be interrupted and the robot has to look for the startfield again. \n 
The countdown has the robot blinking three leds with 5hz which are placed on top of the roboter. \n
When the robots completes 3 laps it stops and a 60s countdown starts, analog to the first countdown.


\section install_sec Installation
How to compile and install the firmware on the roboter:
* Before the firmware can be flashed, the robot has to be <a href="flashing_process.jpg" target="_blank"><b>connected</b></a> to a pc containing the firmware.
* The of process compiling and flashing is automated using Makefile. \n
  To install the default configuration it is sufficient to navigate to the projects directory in the console and run **make all**
  
  \subsection Explanation
  In order to install new firmware on the robot. We first need to translate the sourcecode for the computer/ hardware to understand. \n
  This is done by compiling the sourcecode into bytecode. Since the project consists of multiple sourcefiles these have to be linked together.

* Open the terminal and navigate to the file with the code using "cd"
  (= change directory), or open the terminal in the desired file folder
  by clicking on the right mouse pad and choosing "Open Terminal Here".
* Depending on which option you wish, chose one of the following
  compiler options:
  
The <a href="transition_map.pdf" target="_blank"><b>Transition Map</b></a>.
  
\section Hardware

\section Table_Of_Contents
\subsection States descrption
\how to implement new states
  
\section Calibration

\section compiler_options Compiler Options
* **make**: compiles, flashes and cleans up files that are no longer
  useful. Debug modus is disabled.
* **make debug**: debug modus is enabled. \n
* **make remote**: This modus enabled the remote control of the robot via
  cutecom. Debug modus is disabled. \n
* **make documentation**: This modus only updates the doxygen
  documentation. . \n **Attention** if you received the file, usually a
  doxyfile should already exist. However, if there is no html or latex
  file in the file you received, first type 'doxygen -g doxyfile' in the
  terminal. After that open the doxyfile in nano
  ('nano doxyfile' in the terminal) and search for
  'OPTIMIZE_OUTPUT_FOR_C' (use the explanation at the bottom of the screen
  in nano for short cuts). Now you have to type 'YES' behind the
  '='-symbol and save the doxyfile.

\section Amazing_features Extra features
The robot not only drives three rounds on a circular track, has two
reliable and exact countdowns using different timer and prescaler to
show the robot's (and coder's :) ) full potential, but also
**only** uses timer instead of delays and has a working remote control! \n
To use the remote control, you first have to enable it with the compile
option 'make remote'. \n Open cutecom and create a bluetooth
connection. \n Then type a direction with the desired
time in ms in the text box and send it. \n The robot will follow your
order as long as the time you sent is not expired or you have not
send a new order. Here is a list of the directions available: \n
* F: forwards
* B : backwards
* l : soft left
* r : soft right
* L : hard right
* R : hard left
* Q : do not move
  \n For example, if you type and send 'F100' the robot will move
  forwards for 100 ms.

\section About_me About the author
To tell you a little bit more about myself: My name is Klara M. Gutekunst,
i am currently at the end of my second semester studying computer
science at the university of Kassel. \n
This semester we had the chance to choose a project in a variety of
hands-on coding training, including programming a robot. As I have never
worked with code before I started studying, \n I wanted to bring what I
have learned so far to good use and actually see what some lines of
code can do in real life.  I have been not disappointed, as you can see: \n
The work really paid off and at the end my robot slowely but confidently
moves around on the track!

\section further_information Optionial information
If you wish to read more about the code and why certain code is used
the way it is, just explore the option given above in the main page. \n
If you have any suggestions for improvement, do not hesitate and contact
me: klara.gutekunst.kg@gmail.com \n
I hope you enjoy this project as much as I did

\section Roadmap
