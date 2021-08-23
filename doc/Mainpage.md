

\mainpage

\htmlonly <style>div.image img[src="the_robot.jpg"]{width:700px;}</style> \endhtmlonly
@image html the_robot.jpg "The Robot"


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
  This is done by compiling the sourcecode into bytecode. Since the project consists of multiple sourcefiles these have to be linked together so the output is one object file. \n
  

* Open the terminal and navigate to the file with the code using "cd"
  (= change directory), or open the terminal in the desired file folder
  by clicking on the right mouse pad and choosing "Open Terminal Here".
* Depending on which option you wish, chose one of the following
  compiler options:
  
The <a href="transition_map.pdf" target="_blank"><b>Transition Map</b></a>.
  
\section Hardware
Microcontroller Arduino Modell: ATMEGA328P \n
The <a href="pin_layout.pdf" target="_blank"><b>Pin layout</b></a> can be looked up here.


\subsection States descriptions

\section Features
The hole implementation is based on the api fsm.h which is a finite state machine. \n
This has several advantages. The robot knows at all times in which state it currently is in. \n
This allows for more accurate state transitions which results in better driving behaviour. \n
Each state can have its own logic and transitions which can be completly independant of each other. \n
This makes it incredibly easy to debug and implement new states. \n When adding, a new state the logic of the allready existing transitions does not have to be modified because 
only the information relevant to the current state has to be interpreted.

  \subsection FSM_explanation
  * Each State implements an **enter function** and an **update function** which defines the behaviour of the State. \n
  * The **enter function** is called once for every transition into an other state which invokes the new current state's **enter function** implementation. \n
    E.g. When the State Forward is entered the motor settings can be adjusted accordingly in the enter_forward() function as this only need to happen once in this state.
  * The **update function** is called repeadetly in start_fsm_cycle() which invokes the current_state **update function**. \n
    Each State uses this function to do task that have to take place over a period of time and to determine wether to stay or to transition into another State if a certain condition is met.
  * To transition to another state transition_to_state() is called. This is typically done inside of an **update function** as it has the needed parameters.
    
  * Note: Not every State has to implement an **enter function** but each State has to implement an **update function**. 

  \subsection how_to_implement_new_states
  Descrition of how to add a new state. 
  * Add a new state in State
  * Create function enter_ + "name_of_state" and update_ + "name_of_state"
  * Call to add_state() passing a pointer to the States's enter and update function
  
\section Additional_Features
The ATMEGA328p comes with 3 onboard timers which are used in order to achieve delays.



\section Calibration
Before letting the robot drive it can be wise. To calibrate the motors. /n
Sometimes one motor can be stronger than the other. So the robot is thinking its driving straight \n
but in reality driving to the site. The same can be said about the sensors. \n
The sensors are not always 100% accurate so it can be reasonable to adjust the
Threshold enum as needed. Each of the three sensors has its own value which can be adjusted.

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
  
\section About_me About the author
To tell you a little bit more about myself: My name is Fabian Indrunas,
I am currently at the end of my second semester studying computer
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
