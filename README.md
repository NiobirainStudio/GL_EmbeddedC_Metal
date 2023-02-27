# GL_EmbeddedC_Metal
A set of tasks for C/Embedded on STM32F4DISCOVERY board and GL Extension board.



# Layouts

### LED layout
![LEDs](resources/general/LEDs.png)

### Button layout (tasks 1 & 2)
![Buttons_1_2](resources/general/Buttons_1_2.png)

### Button layout (task 3)
![Buttons_3](resources/general/Buttons_3.png)

# Visualisation

### LED blinking modes
![Blinking_modes](resources/general/Blinking_modes.png)



# Task_1 results:

<p>For the 1st task I have implemented the "Round cycle" blink mode.</p>

### Raw output
![task_1_raw](resources/signal_sample_snippets/task_1_raw.jpg)

### Single cycle
![task_1_single_cycle](resources/signal_sample_snippets/task_1_single_cycle.jpg)

### Demonstration
[Task 1 video link](https://drive.google.com/file/d/1ZAXTzUsTWDgzGQvHig82hiqvSF-8zatT/view?usp=sharing)



# Task_2 results:

<p>For the 2nd task all 3 blink modes were implemented.</p>
<p>Button controls can be seen in the layout section.<p>

### Demonstration
[Task 2 video link](https://drive.google.com/file/d/1ogUyGjm35eDtBWMlxWRxrAiMj8aUB_tE/view?usp=sharing)



# Task_3 results:

### Switching to High Speed External clock (frequency set to 16MHz)
![task_3_clock](resources/other/task_3_clock.jpg)

<p>After the clock is set, set the Prescaler (PSC) to 16000, resulting in an input frequency of 1 kHz.<p>

### Blink frequency formula
![task_3_blink_frequency_formula](resources/formulas/task_3_blink_frequency_formula.jpg)

<p>Since we cannot change the Input signal frequency by adding an Additional frequency to it, we will find the formula for calculating the new Counter period value.<p>

### System of equations of for Blink frequency with new and old Counter period values
![task_3_arr_calculation_1](resources/formulas/task_3_arr_calculation_1.jpg)

### New Counter period value formula
![task_3_arr_calculation_2](resources/formulas/task_3_arr_calculation_2.jpg)

### Duty cycle calculation

<p>Calculating the duty cycle is very easy once you know the ARR value.<p>

![task_3_duty_cycle](resources/formulas/task_3_duty_cycle_calculation.jpg)

### Signal analyzer result table

| Input frequency | Additional frequency | ARRold | ARRnew | Duty cycle | Blink frequency | Signal Capture                                                         |
|:---------------:|:--------------------:|:------:|:------:|:----------:|:---------------:|:---------------------------------------------------------------------- |
| 1000 Hz         | 0 Hz                 | 250    | 250    | 50%        | 2 Hz            | ![task_3_d50_s4](resources/signal_sample_snippets/task_3_d50_b2.jpg)   |
| 1000 Hz         | 5000 Hz              | 250    | ~42    | 50%        | 12 Hz           | ![task_3_d50_s24](resources/signal_sample_snippets/task_3_d50_b12.jpg) |
| 1000 Hz         | 10000 Hz             | 250    | ~23    | 50%        | 22 Hz           | ![task_3_d50_s44](resources/signal_sample_snippets/task_3_d50_b22.jpg) |
| 1000 Hz         | 0 Hz                 | 250    | 250    | 55%        | 2 Hz            | ![task_3_d55_s4](resources/signal_sample_snippets/task_3_d55_b2.jpg)   |
| 1000 Hz         | 0 Hz                 | 250    | 250    | 60%        | 2 Hz            | ![task_3_d60_s4](resources/signal_sample_snippets/task_3_d60_b2.jpg)   |
| 1000 Hz         | 0 Hz                 | 250    | 250    | 70%        | 2 Hz            | ![task_3_d65_s4](resources/signal_sample_snippets/task_3_d70_b2.jpg)   |

### Demonstration
[Task 3 video link](https://drive.google.com/file/d/1F5gNPX0-D6QYBlSHwgrwUIJH9fl9PKEO/view?usp=sharing)
