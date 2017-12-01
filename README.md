# **Udacity Term 2 Project 4 PID-controll**

## Theoretical discussion
The PID controll consists of 3 parts. P, I and D controller parameters. I use this controller to control the steering angle of the car while it drives throug the track.

The P(proportional)-parameter (Kp) controlls the action of the car according to the distance to center of the road (the desired place or "cte"). The highter this parameter is, the more the car will steer neer the center to minimize that error. The problem with using only a P-controller ist, that the car will overshoot its goal and then have to steer in the other direction. This leads to an oscillating behavior.

That's why there is also the D (differential) -parameter (Kd). The d-Paramter uses the distance between the current cte and the previous one to dampen the action produced by the P-controller. That is, when the car approactes the goal position very fast this parameter lowers the amplituce of the action. So the car will approach the target position more gradually.

So with those components, the car will steer the corrent ammount accroding to it's distance to the goal position. But it still can settle not completely at the target position but a little bit off to the side (due to not completely correct alignment of the tires to the car or some thing similar). To reduce this offset also, the I (integrational) -parameter (Ki) is introduced. It is used to controll the steering action according to the cumulative error of the cars position. So the the car drives constantly next to the target position this value will get more and more influence and therefore steer the car gradually neer the target.

## Practical observations
You want to have a decent high Kp value to steer the car towards the target but you have to be carefull not to overshoot so much. So not only the values of the parameter matter, also the distance between them. The bigger the distance between the Kp and Kd parameter is, the less shaky the control becomes. However with a high Kd the steering in a turn gets lower and therfore it's possible that the car doesn't steer enough to savely take the turn. On the other hand if the Kp parameter is too high the car steerds very suddenly which would make the drive less plesent for a possible human driver.

So you have to find a sweet spot of parameters for Kd and Kp so the car doesn't oscillate that much and doesn't steer so suddenly, but still steers enough in the corners.

The i-parameter is nearly irrelevant because it's a simulation and the car has no noticable bias during controll. However, even though one can assume there is no bias error, it doesn't hurt to use a small value for Ki if this assumption is wrong.

## Finding the best parameters
To find the correct parameters you can use different approaches. Stochastic gradient descent (SGD). Twiddle, manual search and muc more. In this implementation I chose the twiddle algorithm to search for optimal parameters. In twiddle you change the PID-parameters according to an update sceme until they settle or until a certain amount of iterations are over. The change of the PID-parameters is done by adding or subtracting a "twiddle-value". Which value that is and how much it changes if the algorithm finds a "good" direction can be set with a parameter in the twiddle algorithm. It's important to set these values very carefully because if they are too high the update may change the PID-parameter too much to drive the car off the track at the very beginning. If they are too low the inital PID-parameters have to be set very precisely because thex are not changes that much at the beginning.

So my approach was to find inital PID-parameter value that are roughly correct and then choose a rather low update parameter for the twiddle algorithm that adjusts the PID-parameter to their optimum. I stop twiddle-ing after a fixed number of iterations.

---

# CarND-Controls-PID
Self-Driving Car Engineer Nanodegree Program

---

## Dependencies

* cmake >= 3.5
 * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1(mac, linux), 3.81(Windows)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools]((https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)
* [uWebSockets](https://github.com/uWebSockets/uWebSockets)
  * Run either `./install-mac.sh` or `./install-ubuntu.sh`.
  * If you install from source, checkout to commit `e94b6e1`, i.e.
    ```
    git clone https://github.com/uWebSockets/uWebSockets 
    cd uWebSockets
    git checkout e94b6e1
    ```
    Some function signatures have changed in v0.14.x. See [this PR](https://github.com/udacity/CarND-MPC-Project/pull/3) for more details.
* Simulator. You can download these from the [project intro page](https://github.com/udacity/self-driving-car-sim/releases) in the classroom.

There's an experimental patch for windows in this [PR](https://github.com/udacity/CarND-PID-Control-Project/pull/3)

## Basic Build Instructions

1. Clone this repo.
2. Make a build directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./pid`. 

Tips for setting up your environment can be found [here](https://classroom.udacity.com/nanodegrees/nd013/parts/40f38239-66b6-46ec-ae68-03afd8a601c8/modules/0949fca6-b379-42af-a919-ee50aa304e6a/lessons/f758c44c-5e40-4e01-93b5-1a82aa4e044f/concepts/23d376c7-0195-4276-bdf0-e02f1f3c665d)

## Editor Settings

We've purposefully kept editor configuration files out of this repo in order to
keep it as simple and environment agnostic as possible. However, we recommend
using the following settings:

* indent using spaces
* set tab width to 2 spaces (keeps the matrices in source code aligned)

## Code Style

Please (do your best to) stick to [Google's C++ style guide](https://google.github.io/styleguide/cppguide.html).

## Project Instructions and Rubric

Note: regardless of the changes you make, your project must be buildable using
cmake and make!

More information is only accessible by people who are already enrolled in Term 2
of CarND. If you are enrolled, see [the project page](https://classroom.udacity.com/nanodegrees/nd013/parts/40f38239-66b6-46ec-ae68-03afd8a601c8/modules/f1820894-8322-4bb3-81aa-b26b3c6dcbaf/lessons/e8235395-22dd-4b87-88e0-d108c5e5bbf4/concepts/6a4d8d42-6a04-4aa6-b284-1697c0fd6562)
for instructions and the project rubric.

## Hints!

* You don't have to follow this directory structure, but if you do, your work
  will span all of the .cpp files here. Keep an eye out for TODOs.

## Call for IDE Profiles Pull Requests

Help your fellow students!

We decided to create Makefiles with cmake to keep this project as platform
agnostic as possible. Similarly, we omitted IDE profiles in order to we ensure
that students don't feel pressured to use one IDE or another.

However! I'd love to help people get up and running with their IDEs of choice.
If you've created a profile for an IDE that you think other students would
appreciate, we'd love to have you add the requisite profile files and
instructions to ide_profiles/. For example if you wanted to add a VS Code
profile, you'd add:

* /ide_profiles/vscode/.vscode
* /ide_profiles/vscode/README.md

The README should explain what the profile does, how to take advantage of it,
and how to install it.

Frankly, I've never been involved in a project with multiple IDE profiles
before. I believe the best way to handle this would be to keep them out of the
repo root to avoid clutter. My expectation is that most profiles will include
instructions to copy files to a new location to get picked up by the IDE, but
that's just a guess.

One last note here: regardless of the IDE used, every submitted project must
still be compilable with cmake and make./

## How to write a README
A well written README file can enhance your project and portfolio.  Develop your abilities to create professional README files by completing [this free course](https://www.udacity.com/course/writing-readmes--ud777).

