#Implementation PID-controll

##Theoretical discussion
The PID controll consists of 3 parts. P, I and D controller parameters. I use this controller to control the steering angle of the car while it drives throug the track.

The P-parameter (Kp) controlls the action of the car according to the distance to center of the road (the desired place or "cte"). The highter this parameter is, the more the car will steer neer the center to minimize that error. The problem with using only a P-controller ist, that the car will overshoot its goal and then have to steer in the other direction. This leads to an oscillating behavior.
That's why there is also the D-parameter (Kd). The d-Paramter uses the distance between the current cte and the previous one to dampen the action produced by the P-controller. That is, when the car approactes the goal position very fast this parameter lowers the amplituce of the action. So the car will approach the target position more gradually.
So with those components, the car will steer the corrent ammount accroding to it's distance to the goal position. But it still can settle not completely at the target position but a little bit off to the side (due to not completely correct alignment of the tires to the car or some thing similar). To reduce this offset also, the I-parameter (Ki) is introduced. It is used to controll the steering action according to the cumulative error of the cars position. So the the car drives constantly next to the target position this value will get more and more influence and therefore steer the car gradually neer the target.

##Practical observations
You want to have a decent high Kp value to steer the car towards the target but you have to be carefull not to overshoot so much. So not only the values of the parameter matter, also the distance between them. The bigger the distance between the Kp and Kd parameter is, the less shaky the control becomes. However with a high Kd the steering in a turn gets lower and therfore it's possible that the car doesn't steer enough to savely take the turn. On the other hand if the Kp parameter is too high the car steerds very suddenly which would make the drive less plesent for a possible human driver.

So you have to find a sweet spot of parameters for Kd and Kp so the car doesn't oscillate that much and doesn't steer so suddenly, but still steers enough in the corners.

The i-parameter is nearly irrelevant because it's a simulation and the car has no noticable bias during controll. However, even though one can assume there is no bias error, it doesn't hurt to use a small value for Ki if this assumption is wrong.

##Finding the best parameters
To find the correct parameters you can use different approaches. Stochastic gradient descent (SGD). Twiddle, manual search and muc more. In this implementation I chose the twiddle algorithm to search for optimal parameters. In twiddle you change the PID-parameters according to an update sceme until they settle or until a certain amount of iterations are over. The change of the PID-parameters is done by adding or subtracting a "twiddle-value". Which value that is and how much it changes if the algorithm finds a "good" direction can be set with a parameter in the twiddle algorithm. It's important to set these values very carefully because if they are too high the update may change the PID-parameter too much to drive the car off the track at the very beginning. If they are too low the inital PID-parameters have to be set very precisely because thex are not changes that much at the beginning.

So my approach was to find inital PID-parameter value that are roughly correct and then choose a rather low update parameter for the twiddle algorithm that adjusts the PID-parameter to their optimum. I stop twiddle-ing after a fixed number of iterations.


