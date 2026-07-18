# Minimax-Algorithm-for-Mills-board-game
<p align="center">
<img src="https://img.shields.io/badge/C%2B%2B-00599C?style=for-the-badge&logo=cplusplus&logoColor=white" alt="C++">  <img src="https://img.shields.io/badge/Arduino-Microcontrollers-00979D?style=for-the-badge&logo=arduino&logoColor=white" alt="Arduino Microcontrollers">  <img src="https://img.shields.io/badge/License-MIT-blue?style=for-the-badge" alt="MIT License">  <img src="https://img.shields.io/badge/Release-v2.0.0-blue?style=for-the-badge" alt="Release">  <img src="https://img.shields.io/badge/Open_Source-Yes-success?style=for-the-badge" alt="Open Source">
</p>


# What is this algorithm?
A C++ implementation of a Minimax AI for Nine Men's Morris (Mills board game), capable of evaluating board positions and calculating the best move up to 2-3 moves ahead. It is designed to work on Arduino microcontrollers and the online simulator Tinkercad; Built from scratch, no external code, and minimal set of libraries.

The algorithm is designed to work for Arduino microcontrollers, designed to detect moves via dozens of electromagnetic sensors, so it is best to use the online simulator Tinkercad to try the program out. We just need to create a “Circuits” project, add a microcontroller to it, and copy paste the C++ code. 

# Broad Program Explanation:
Because pieces may only move to adjacent positions, I needed an intuitive way for the program to understand board connectivity. I solved this by numbering the possible positions on each ring from 0 to 7 and adding a multiple of ten to indicate the ring. For example, a piece at position “1” can move to “0”, “2”, or “11”. The program recognises that “1” and “11” are connected because they share the same first digit.

This is the digital representation I used to know the position of each pieces:
<p align="center">
<img width="1093" height="1127" alt="image" src="https://github.com/user-attachments/assets/59d75276-9d27-458b-ba0b-3838831da0c3" />
</p>

We create two lists, one for the bot and one for the player, each containing the positions of their nine pieces. A value of “-2” means that the piece is not on the board. These lists are the “input” values to the algorithm which returns a following “output” move.

The most challenging part was to create the minimax algorithm. Differently from games like chess where we have to maximise our position then minimise the opponents’, I had to do it differently because of the ruleset. In the Nine Men’s Morris game, it is possible that a player can play two times in a row, as it can move a piece of its own and then delete any of the opponents’. So the minimax program would rather look like the graph below. Depending on what happens, we may have to maximise two times the possible reward a player can get, and then minimise the opponents reward. 

Here is a graph on how the algorithm determines the best position: 
<p align="center">
<img width="905" height="419" alt="598748437-36fed8bc-38c6-417a-a0cf-b5f8e3487d88" src="https://github.com/user-attachments/assets/de71b23d-07db-490c-92b3-6a91731c71ba" />
</p>


**Creation of the electric circuit**

I designed the electric circuit. The idea was to place an electromagnetic sensor beneath each of the 24 positions, and then use magnets embedded pieces to detect the user’s moves.

The first challenge was that the Arduino microcontroller we had didn’t have enough ports for the dozens of electromagnetic captors. To overcome this, we used two microcontrollers that would communicate information to each other. My friend and I called the circuit the “bomb” due to its complex electric cable system. I have also created a digital PCB version of the circuit, which can be manufactured and shipped rather than having to make by hand.

Here is how the circuit looks in real life, along with a digital PCB version that can be manufactured:
<p align="center">
<img width="288" height="427" alt="598748397-f7187b93-0a9d-4cb1-b4f7-5df7a055a10f" src="https://github.com/user-attachments/assets/894e80e7-948d-4efa-806e-3e8ee582fb77" /> <img width="630" height="429" alt="598748369-e5ea97cc-8e90-4151-a580-445d88fca891" src="https://github.com/user-attachments/assets/ec260458-c6d5-43b4-89c8-ddb4f2e98dc4" />
</p>


However, although all individual components worked as intended, the flow of information between microcontrollers and captors was unstable. In fact, it seemed that our circuit required more voltage to make it work; the information received and sent was too great to handle. Despite that, the algorithm itself works perfectly on the computer.


# How to use the program?

The algorithm is designed to work for Arduino microcontrollers, designed to detect moves via dozens of electromagnetic sensors, so it is best to use the online simulator Tinkercad to try the program out. We just need to create a “Circuits” project, add a microcontroller to it, and copy paste the C++ code. 


# How to install and edit the project?

By default, the program runs the opening-phase minimax; switching “in_beginning = true” to “false” activates the mid-game version.


# License
Licensed under the MIT License - See the LICENSE document




