

\mainpage

<a href="transition_map.pdf" target="_blank"><b>Transition Map</b></a>

\section intro_sec Introduction
Welcome to a short introduction of a heartwarming project executed by a
student of the university Kassel. \n The aim of this project is to create
a code that programs a robot to drive three rounds on a circular track,
as well as starting and ending with a countdown.\n
Even though the robot is adorable to observe while following on its own
it is also possible to control the robot via a bluetooth connection! \n
If you want to know how to start and setup the robot, just keep reading
and enjoy!

\section install_sec Installation
Installation and handelling is not as compilcated as one might think!
* First, one has to create a USB connection between the computer and
  the robot.
* Open the terminal and navigate to the file with the code using "cd"
  (= change directory), or open the terminal in the desired file folder
  by clicking on the right mouse pad and choosing "Open Terminal Here".
* Depending on which option you wish, chose one of the following
  compiler options:
  

\section Table\ Of\ Contents
\subsection States descrption
\how to implement new states
  
\section Calibration

@sa Beginners

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
