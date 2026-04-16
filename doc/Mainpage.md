

\mainpage

\htmlonly <style>div.image img[src="the_robot.jpg"]{width:700px;}</style> \endhtmlonly
@image html the_robot.jpg "The Robot"

\section intro_sec Introduction
Welcome to a brief introduction of a project executed by a
student of the university of Kassel. \n The goal of this project is to create reliable
software that controls the robot so it drives three laps on a track (<a href="track_img.jpg" target="_blank"><b>example track</b></a>) consisting of a black line, autonomously. \n
There is a black square placed on the track which acts as the starfield. \n
The robot has three sensors which enable it to detect the black line as well as the starfield so it can take action accordingly. \n\n
In addition to that the robot starts driving only after being placed on the starfield which initiates a 15s countdown. \n
Upon being moved the countdown shall be interrupted and the robot has to look for the starfield again. \n
During the countdown three on board leds are blinking with 5hz. \n
When the robots completes 3 laps it stops and a 60s countdown starts, analog to the first countdown.


\section install_sec Installation
How to compile and install the firmware on the roboter:
* Before the firmware can be flashed, the robot has to be <a href="flashing_process.jpg" target="_blank"><b>connected</b></a> to a pc containing the firmware.
* The of process compiling and flashing is automated using Makefile. \n
  To install the default configuration it is sufficient to navigate to the projects directory in the console and run **make**

  \subsection Explanation
  * In order to install new firmware on the robot. We first need to "translate" the sourcecode for the computer/ hardware to understand. \n
    This is done by compiling the sourcecode into bytecode. Since the project consists of multiple sourcefiles these also have to be linked together so the output is one object file. \n
  * The project can be compilated in different ways for more information view section **Compilation Modes**. \n
  * The **ATMEGA328P** uses a different encoding so the object file has to converted to a hex file \n
  * Now the correct hex file is on the computer and only has to be installed on the microcontroller.
  * This is done by flashing. 
  * The hex and flash commands are listed in the **Makefile**.
  
  \subsection comp Compilation Modes
  The Compilation modes can be adjusted as wanted by setting the values of the  **- D Flags**. \n
  in the **Makefile** under **CFLAGS**.
  If a value is set to 0 the corrosponding mode is disabled otherwise it is enabled. \n
  * **DEBUG_MODE**: \n
    Turns on debug prints that are send to the **serial port** via transmit_debug_msg() \n
    The data send contains the current State, the sensor values and speed of the engines.
  * **COUNTDONW_MODE**: \n 
    Enables the 15s Countdown at the start. \n
    Otherwise the Roboter starts with the linefollowing algorithm as soon as it is turned on.
  * **LAPCOUNTER_MODE**: \n
    If enabled the robot counts the laps otherwise it follows the line till it's batteries die.
  * **LAPS**: \n
    Determines how many laps the robot has to drive. \n
    **Note**: This is only relevant if **LAPCOUNTER_MODE** is enabled.
    
\section Hardware
Microcontroller Arduino Modell: **ATMEGA328P** \n
The <a href="pin_layout.pdf" target="_blank"><b>Pin layout</b></a> can be looked up here.

\section Features
The hole implementation is based on the api fsm.h which is a finite state machine. \n
This has several advantages. The robot knows at all times in which state it currently is in. \n
This allows for more accurate state transitions which results in better driving behaviour. \n
Each state can have its own logic and transitions which can be completly independant of each other. \n
This makes it incredibly easy to debug and implement new states. \n When adding, a new state the logic of the allready existing transitions does not have to be modified because 
only the information relevant to the current state has to be interpreted.

  \subsection fsm_explanation FSM Explanation
  * Each State implements an **enter function** and an **update function** which defines the behaviour of the State. \n
  * The **enter function** is called once for every transition into an other state which invokes the new current state's **enter function** implementation. \n
    E.g. When the State Forward is entered the motor settings can be adjusted accordingly in the enter_forward() function as this only need to happen once in this state.
  * The **update function** is called repeadetly in start_fsm_cycle() which invokes the current_state **update function**. \n
    Each State uses this function to do task that have to take place over a period of time and to determine wether to stay or to transition into another State if a certain condition is met.
  * To transition to another state transition_to_state() is called. This is typically done inside of a States's **update function** as it has the needed parameters.
  * **Note**: Not every State has to implement an **enter function** but each State has to implement an **update function**. 
  
  \subsection implement_new_states How to implement new States
  Descrition of how to add a new state. 
  * Add a new state in State
  * Create function enter_ + "name_of_state" and update_ + "name_of_state" for better readabilty.
  * Call to add_state() passing a pointer to the States's enter and update function
  * Implement the enter and update function as wanted, achieving the desired behaviour for that State.

\section additional_features Additional Features
The **ATMEGA328p** comes with 3 onboard timers which are all used in order to prevent software delays. \n
All delays including the delay for the start and end countdown are implemented via the usage of interrupts. \n
This is more efficient as the microcontroller does not have to waist energy doing nothing.
Another advantage is that the programm can still be running normally during the delays which 
enables us to take measurements as per usuall and take action accordingly.

\section States
The State transitions are implemented as shown in <a href="transition_map.pdf" target="_blank"><b>Transition Map</b></a>.
* **INIT**: \n
  Inilizes all the modules.
* **CHECK_STARTPOS**: \n
  Lights leds based on their status and transition to CHECK_STARTPOS when the startfield is detected.
* **COUNTDOWN**: \n
  Starts the 15s countdown. If the robot is moved during this time the countdown is interrupted. \n
  If the countdown finished successfully the roboter transitions to LEAVE_START
* **LEAVE_START**: \n
  In this state the robot  starts to drive forward until it leaves the startfield. \n
  This State is used so the Robot doesn't count the startfield multiple times per pass.
* **FORWARD**: \n
  The Robot drives forward by setting both engines forward and to the same speed. \n
  This state is left only if one or both of the side sensors detect a line.
* **LEFT_SOFT**: \n
  In this state the robot performes a slight turn by making the left eng speed slower then the right via set_direction() \n
  This state is used mainly to react to slight inaccuracies when the robot wants to drive forward but \n
  is not doing this perfectly. There is always a small margin. This keep the robot on track.
* **RIGHT_SOFT**: \n
  Analog to LEFT_SOFT
* **LEFT_HARD**: \n
  This state is used for the sharp turnes. By setting the left eng backwards and the right eng to a high speed \n
  the robot performs a sharp turn. 
* **RIGHT_HARD**:  \n
  Analog to LEFT_HARD
* **CHECK_LAP**:  \n
  State that is entered when all 3 sensors see black. \n
  In this case the robot checks if it is indeed on the startfield by checking if all 3 sensors
  detect black for an extended amount of time. /n
  It is not sufficient to assume it is on the startfield automatically as it can occure in the sharp turns \n
  that all 3 sensors see black for a very short amount of time. \n
  This state is used to differentiate between these two cases.
* **GOAL_REACHED**: \n 
  The goal is reached when the robot completed all laps. A 60s countdown is initiated.

\section Calibration
Before letting the robot drive it can be wise to calibrate it's motors and sensors. \n
Sometimes one motor can be stronger than the other. So the robot is thinking its driving straight \n
but in reality driving to the site. The same can be said about the sensors. \n
The sensors are not always 100% accurate so it can be reasonable to adjust the
enum Threshold enum as needed. \n Each of the three sensors has its own value. \n
<a href="https://youtu.be/au92yQbTNe0" target="_blank"><b>Here</b></a>
 is an example how the robot should drive when it is calibrated properly.

\section further_information Additional information
If you wish to read more about the code and why certain code is used
the way it is, just explore the option given above in the main page. \n
If you have any suggestions for improvement, do not hesitate and contact
me: findrunas@protonmail.com \n

