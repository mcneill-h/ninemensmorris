# Minimax-Algorithm-for-Mills-board-game
A C++ implementation of a Minimax AI for Nine Men's Morris (Mills board game), capable of evaluating board positions and calculating the best move up to 2-3 moves ahead. It is designed to work on Arduino microcontrollers and the online simulator Tinkercad; Built from scratch, no external code, and minimal set of libraries.

The algorithm is designed to work for Arduino microcontrollers, designed to detect moves via dozens of electromagnetic sensors, so it is best to use the online simulator Tinkercad to try the program out. We just need to create a “Circuits” project, add a microcontroller to it, and copy paste the C++ code. By default, the program runs the opening-phase minimax; switching “in_beginning = true” to “false” activates the mid-game version.

Here is a graph on how the algorithm determines the best position: 
<p align="center">
<img width="905" height="314" alt="image" src="https://github.com/user-attachments/assets/36fed8bc-38c6-417a-a0cf-b5f8e3487d88" />
</p>
Here is how the circuit looks in real life, along with a digital PCB version that can be manufactured:
<p align="center">
<img width="288" height="428" alt="image" src="https://github.com/user-attachments/assets/f7187b93-0a9d-4cb1-b4f7-5df7a055a10f" /> <img width="630" height="428" alt="image" src="https://github.com/user-attachments/assets/e5ea97cc-8e90-4151-a580-445d88fca891" />
</p>
This is the digital representation I used to know the position of each pieces:
<p align="center">
<img width="1093" height="1127" alt="image" src="https://github.com/user-attachments/assets/59d75276-9d27-458b-ba0b-3838831da0c3" />
</p>
