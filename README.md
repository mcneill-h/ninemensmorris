# Minimax-Algorithm-for-Mills-board-game
<p align="center">
<img width="288" height="427" alt="598748397-f7187b93-0a9d-4cb1-b4f7-5df7a055a10f" src="https://github.com/user-attachments/assets/894e80e7-948d-4efa-806e-3e8ee582fb77" /> <img width="630" height="429" alt="598748369-e5ea97cc-8e90-4151-a580-445d88fca891" src="https://github.com/user-attachments/assets/ec260458-c6d5-43b4-89c8-ddb4f2e98dc4" />
</p>

<p align="center">
<img src="https://img.shields.io/badge/C%2B%2B-00599C?style=for-the-badge&logo=cplusplus&logoColor=white" alt="C++">  <img src="https://img.shields.io/badge/Arduino-Microcontrollers-00979D?style=for-the-badge&logo=arduino&logoColor=white" alt="Arduino Microcontrollers">  <img src="https://img.shields.io/badge/License-MIT-blue?style=for-the-badge" alt="MIT License">  <img src="https://img.shields.io/badge/Release-v2.0.0-blue?style=for-the-badge" alt="Release">  <img src="https://img.shields.io/badge/Open_Source-Yes-success?style=for-the-badge" alt="Open Source">
</p>


# What is this algorithm?
This project is a C++ implementation of a Minimax AI for Nine Men's Morris (Mills board game), capable of evaluating board positions and calculating the best move up to 2-3 moves ahead. It is designed to work on Arduino microcontrollers and the online simulator Tinkercad.

We were two people to work on this project, our goal was to build a physical version of the Nine Men's Morris board game, where an Al could play against a human user. The system relied on a circuit, linked to Arduino microcontrollers, designed to detect moves via dozens of electromagnetic sensors, and a self-made minimax algorithm to create the AI. After the player makes a move, the algorithm would then evaluate the positiona and respond to it with another move. We would use the small screen to display the response. 

I was responsible for designing and building the minimax program and the electrical circuit, so I will focus on those components. I will only release the code of the minimax on this repository, as it was my personal work. The other reason for not posting the full code is that the final product didn't succeed to work (even though all individual components did). The microcontrollers would output false signals and break the program. It was likely due to the sheer number of components used. 

The minimax algorithm from this repository was built from scratch, with no external code, and using a minimal set of libraries.


# Broad Program Explanation:

In order to store data of the board, we needed an intuitive way for the program to understand board connectivity. Therefore, we solved this by numbering the possible positions on each ring from 0 to 7 and adding a multiple of ten to indicate the n-th ring. This greatly helps program optimisation. For example, a piece at position “1” can move to “0”, “2”, or “11”. The program recognises that “1” and “11” are connected because they both share the same first digit.

This is the digital representation I used to know the position of each pieces:
<p align="center">
<img width="1093" height="1127" alt="image" src="https://github.com/user-attachments/assets/59d75276-9d27-458b-ba0b-3838831da0c3" />
</p>

Furthermore, we stored data in two lists of 9 slots, one list for the bot and the other for the player, each of them containing the positions of their nine pieces. A piece from the player on position number 12 would have a "12" in his list. Values of -2 are also used to indicate that the piece is not played on the board. 

The most challenging part was to create the minimax algorithm. Differently from games like chess where we have to maximise our position then minimise the opponents’, we had to do it differently because of the Nine Men's Morris ruleset. In the game, it is possible that a player can "play" two times in a row, as it can move a piece of its own and then delete any of the opponents’ in one turn. Depending on what happens, we may have to maximise two times the possible reward a player can get, and then minimise the opponents reward. So the minimax program would rather look like the graph below.

Here is a graph on how the algorithm determines the best position: 
<p align="center">
<img width="905" height="419" alt="598748437-36fed8bc-38c6-417a-a0cf-b5f8e3487d88" src="https://github.com/user-attachments/assets/de71b23d-07db-490c-92b3-6a91731c71ba" />
</p>

Finally, the algorithm of the Nine Men's Morris AI is in reality composed of 2 minimax algotihms, because the Nine Men's Morris Game is divided in 2 distinctive parts. The first phase is when oponents place pieces on the board one at a time, the second is when they move the placed pieces. in_beginning is the variable that indicates in which of the two phases the game is. If it is true, then we are at the starting phase, otherwise we are at the "playing" phase (mid and late game).

**Creation of the electric circuit**

Our idea was to place an electromagnetic sensor beneath each of the 24 positions, and then use magnets embedded pieces to detect the user’s moves.

The first challenge was that the Arduino microcontroller we used didn’t have enough ports for the dozens of electromagnetic captors. To overcome this, we used two microcontrollers that would communicate information to each other. We had also created a digital PCB version of the circuit, which can be manufactured and shipped rather than having to wire it by hand.

Here is how the circuit looks in real life, along with a digital PCB version that can be manufactured:
<p align="center">
<img width="288" height="427" alt="598748397-f7187b93-0a9d-4cb1-b4f7-5df7a055a10f" src="https://github.com/user-attachments/assets/894e80e7-948d-4efa-806e-3e8ee582fb77" /> <img width="630" height="429" alt="598748369-e5ea97cc-8e90-4151-a580-445d88fca891" src="https://github.com/user-attachments/assets/ec260458-c6d5-43b4-89c8-ddb4f2e98dc4" />
</p>

However, although all individual components worked as intended, the flow of information between microcontrollers and captors was unstable. In fact, it seemed that our circuit required more voltage to make it work. Moreover, the information received and sent was too great to handle. Despite that, the separate algorithms like the minimax program work perfectly on computer.


# How to use the program?

The code in the repository works as a position analyser, similar to Stockfish in Chess. It will give you the best move to play in a given position (in a depth 2-3 search). 

You have to manually indicate the position of each player's pieces, by writting it yourself in the lists. You have to do this process turn by turn as the lists don't automatically update.

By default, the program runs the opening phase minimax. Replacing “in_beginning = true” to “false” activates the mid-game version.


# How to install and edit the project?

The algorithm is designed to work for Arduino microcontrollers, designed to detect moves via dozens of electromagnetic sensors. Therefore, to try out the algorithm you can: 
- Launch the program on an Arduino microcontroller.
- Use the online simulator Tinkercad to try the program out. We just need to create a “Circuits” project, add a microcontroller to it, and copy paste the C++ code.

  
# License

Licensed under the MIT License - See the LICENSE document




