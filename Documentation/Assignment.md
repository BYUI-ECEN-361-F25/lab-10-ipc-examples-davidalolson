# ECEN-361 Lab-10: IPC-Examples
     Student Name: David Olson  ___________________________________

## Introduction and Objectives of the Lab

This lab shows how various independent tasks can communicate through the
RTOS-controlled signaling mechanisms. It also shows the good practice of
handling interrupts by using semaphores to signal their respective
tasks.

This lab will demonstrate the following Interprocess Communication
mechanisms:

1.  Semaphores and Task-Notifications

2.  Mutexes

3.  Protected Global Variables

4.  Process Notification via Software Timers

In the provided FreeRTOS system, tasks are set up to use these
mechanisms to communicate with each other. The mechanisms and associated
tasks (and assignments) are described in the respective sections of the
lab.

The API's for [CMSIS-RTOS V2](https://arm-software.github.io/CMSIS_5/RTOS2/html/rtos_api2.html) and [FreeRTOS](https://www.freertos.org/a00106.html) are great resources to look for function information. Keep in mind we are using CMSIS-RTOS V2 but we included task notification functions from FreeRTOS. You can easily tell what API the function is from by its prefix, x is from FreeRTOS, os is from CMSIS-RTOS V2.
___
<!--------------------------------------------------------------------------------->
## Part 1: RTOS-friendly debounced buttons -- Notifications and Semaphores
<!--------------------------------------------------------------------------------->

As discussed in class, all mechanical buttons come with the mechanically
generated issue of 'bouncing' which can create multiple input requests
to a system where only a single entry is required. Methods to alleviate
this problem include external RC circuits on the button input,
dedicating a timer to the incoming button, and software counting loops
to debounce the input. Note that the 'bounce' characteristics of the
switch may vary, but in general they are much less than a human response
time. Typically, a debounce time on the order of 10 -30 milliseconds is
long enough to get rid of the mechanical flaw and not miss a human
reaction time.

In an RTOS environment, the timeout necessary to debug a switch should
not come within the ISR servicing the button event. Leaving the RTOS to
'wait' for a debounce inside of the ISR is bad practice as it can be
blocking. One simple solution is to use notification mechanisms in the
RTOS and trigger them from within the ISR. Two such mechanisms are
'**Semaphores'** and '**Task-Notifications'**. **Semaphores** can be
binary or counting and can track numbers of events from multiple
sources. Task-Notifications are simply a signal from one process to
another.

By using a signaling mechanism this way, we create a 'Thread-safe'
interrupt service routine. This means that the ISR will not create
unexpected behavior in a multi-process environment and supports scalable
grow with more/less processes. It's 'safe' to threads.

For a task(s) waiting for a clean, debounced button event, the initial
button press can't trigger the task(s), it must first trigger the
debounce wait, that then starts the task(s):

<img src="media/safe_debounce1.png" alt="safe-debounce" width="85%" style="display: block; margin: 0 auto;" :w
/>

<!-------------------------------------------------------------------------------->
### Part 1:  Instructions and Questions (2 pts)


>**Create a Task**<br>
(Either use the GUI -- *Under Middleware/FreeRTOS* or manually in the code) that will toggle LED_D4 each time Button_1 is pressed by acquiring the semaphore.

**NOTE**: When you create a task with the .IOC file (via the GUI), there are two fields:
<img src="media/Create_Task.png" alt="safe-debounce" width="85%" style="display: block; margin: 0 auto;" :w
/>

<br>

Now make sure to write the code inside of the Semaphore_Toggle_Task function that uses the provided semaphore "button_1_semaphore" to toggle LED_D4

<br>
1. How did your task ‘wait’ for the debounced button? <br>
<mark>The task uses osSemaphoreAquire which waits until the button is pressed. This is followed by a delay at the end of the task. </mark>
<br>
<br><br>

2.)	How long is the time between the button interrupt coming in and it being enabled again? <br>
<mark>The delay between toggles is 20ms to ensure effective debouncing. </mark>
><br>
> <br>

>**Second Task Creation**<br>
Now create a second task (semaphore_Toggle_D3) -- <p>
>Modify this so that it that also waits for the same Button_1_Semaphore.  It should then toggle LED_D3 each time Button_1 is pressed.  Note that they should both have been created with the same Priority (Normal).<br><br>



3.)	Do both of (D4 and D3) toggle with a single button press?  Describe the behavior?  <br>
<mark>When the button is pressed, one LED is illuminatined. Pressing the button again illuminates the other LED. They also turn off in the same pattern. <br><br>

4.)	Now change one of the priorities of these two tasks, re-compile,  and re-run.
How has the behavior changed?
<mark>Giving LED3 high priority made it the only task that runs. The other LED does not toggle.  <br><br>


## Part 2: Mutexes


Mutex's are used in embedded systems to guarantee exclusive access to a
resource that more than one process may need to use. Examples include
hardware devices or shared memory locations that could be requested
simultaneously.

For the hardware set of our labs, we have only a single 4-digit
seven-segment display. If there were multiple producers trying to change
the value of this display, one way to guarantee its integrity would be
to protect it with a mutex: only one process at a time gets to send
output to it.

For this example, the following picture shows how a mutex could protect
a value being display on the seven-segment:

<img src="media/mutex.png" alt="safe-debounce" width="85%" style="display: block; margin: 0 auto;" />

The three processes on the left are all competing for access to write
to the protected variable. The **mutex_protected_count** is continuously
updated to the seven-segment display on the right. The CountDown and
CountUp are both trying at random intervals, and the Reset_MutexCount
waits for the Button_3 (via its associated semaphore), then resets the
current count. The first two processes are done for you "Mutex_CountDownTask" and "Mutex_CountUpTask"


### Part 2:  Instructions and Questions (4 pt)

6.) Add the third process that uses the mutex to take control and put a “—” on the Two_Digit UpDown SevenSegment Display.

<br>Use the following code in your process:<br>
><br>
```c
    for(;;) 
        {
        /* This doesn't change the value, it just clears the display  */
        /* If asked to display a negative number, the function displays a "--"
        */
        osMutexWait(UpDownMutexHandle,osWaitForever);
        MultiFunctionShield_Display_Two_Digits(-1);
        osDelay(200);
        osMutexRelease(UpDownMutexHandle);
        osDelay(2);
        }
```
>
><br>
>7.)	Comment on the Up/Down/ ”—” display that you see.  <br><br>
><mark>The line appears after pressing button three occassionaly and deflects the reset task, however, sometimes the reset task runs instesad. <br><br><p>


>8.)	Is there a ‘priority’ associated with the Mutex?  If so, how can it be changed?
><br>  
><mark>There does not appear to be a priority related to mutexes.<br><br>
<p>

><br>
>9.)	Button_3 resets the mutex-protected global variable to “50.”  It too, has to wait for the mutex to be granted.<br>

>  Change the priority of the Reset to be osPriorityIdle.  This is the lowest priority available. Note that you will not find this priority type listed in the .ioc configuration, as it is intended to be used for idle threads. This priority must be manually set in the code.<br>
><br> Did you see any effect on the ability of Button_3 to reset the count?<br><br>
><mark>Changing the priority of the reset task did not effect its ability to run.<br><br>
>
---
<!--------------------------------------------------------------------------------->
##  Part 3: Software Timers
<!--------------------------------------------------------------------------------->

In a previous lab, we learned how to setup any of the hardware timer
blocks to generate periodic interrupts. This is limited by the number of
actual timers available on the silicon, and still requires careful
coding to insure RTOS-friendly servicing, as discussed with the buttons
above. Software timers are easy to deploy, flexible to use, and have no
concerns operating in the RTOS environment.

Software timers don't generate an interrupt, they simply execute a
procedure when they expire.

Button_2 is used to toggle the SW_Timer_7Seg between running/stopped.
When the timer expires, it calls a routine to decrement the left-most
display digit.


### Part 3:  Instructions and Questions (2 pts)

>
>9.)	 Change the timer period from the current “200” mS to something different.
>Verify that the decrementing count changes accordingly.

>
>10.) This timer was created via the GUI  (.IOC file).  It’s type is *“osTimerPeriodic”* which means it repeats over and over.<br><br>
What other options can a Software Timer take to change its Type and operation? <br>
><mark>The other option for the software timer is to have it only run once instead of repeat (periodic).<br><br>

>11).	The debounce for the switches here used an osDelay() call (non-blocking).  Is there any advantage to using a SWTimer here instead?<br>
> Explain why or why not?
>
><mark>Since osDelay is already non-blocking, it seems that there is no advantage to using software timers for  a debouncing system.<br><br>


<!--------------------------------------------------------------------------------->
## Ideas for Credit to get to 'A' & Extra-Credit (2 pts for any)
<!--------------------------------------------------------------------------------->

>1.	The Seven-Seg Display is currently refreshed with a hardware (TIM17) timer.  Make this more thread-safe by changing the refresh as a process that is based off a S/W timer.
>
>Write about how you did it, and what the slowest period could be to keep the persistence looking good:
><mark>Added a SW timer named SW_Timer_Refresh and set it to begin the moment the schedular initialized. In the callback is the multi-function shield display refresh method followeed by a 1ms delay. The minimum period that maintained persistence was 5ms.<br><br>

>2.	We only used a binary semaphore in this lab for the switch presses.  Change it so that presses are accumulated through a counting semaphore and then handled as they are taken off.<br><br>
>Describe any issues with this approach
>
><mark>___________________________________________________________________________________________________________<br><br>
>

>


>
>3.	Any other relevant uses for semaphores, mutexes, or S/W timers ?   Describe what you’ve done and why?
>
><mark>___________________________________________________________________________________________________________<br><br>
>
><br>



