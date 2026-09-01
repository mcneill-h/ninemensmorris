// NINE MEN'S MORRIS - MINIMAX AI (depth 2-3)
// Copyright (c) 2025-2026 henrymcneill, anagorisis
// Licensed under the MIT License - See the LICENSE document,
// more details on GitHub at https://github.com/mcneill-h/ninemensmorris

// BOARD REPRESENTATION:
//
// Outer ring  : positions 0 to 7
// Middle ring : positions 10 to 17
// Inner ring  : positions 20 to 27
//
// Positions 8, 9, 18 and 19 do not exist.
// They are gaps between the three rings.
//
// EMPTY_POSITION (-2) means that no piece is stored there.


// --------------------------------------------------------
// OPTIONAL VALUE TO CHANGE: changes if we are using the minimax for the beginning phase of the game (placing pieces) 
//                            or the mid/late game when we move pieces from tile to tile
// --------------------------------------------------------
bool inBeginning = true;
// true  = pieces are still being placed on the board.
// false = all pieces have been placed and players move their existing pieces.
// Change this value when the game enters the movement phase.



// --------------------------------------------------------
// CONSTANTS:
// --------------------------------------------------------

// -2 = no piece is present / no value is selected.
const int EMPTY_POSITION = -2;

// 9 = maximum number of pieces owned by one player.
const int MAX_PIECES = 9;

// 4 = maximum number of movement possibilities for one piece:
// next position, previous position, inner ring, outer ring.
const int MAX_MOVES = 4;

// -100 = initial score.
// Any valid move should normally be better than this value.
const int INITIAL_SCORE = -100;


// --------------------------------------------------------
// OPTIONAL VALUE TO CHANGE: change to set up different board positions!!!!!!
// --------------------------------------------------------
// Stores the positions of the bot's pieces:
int botPositions[MAX_PIECES] = {
  0, 2, 11, 
  EMPTY_POSITION, EMPTY_POSITION, EMPTY_POSITION, 
  EMPTY_POSITION, EMPTY_POSITION, EMPTY_POSITION
};
// Each index represents one of the bot's pieces.
// Example: botPositions[0] = 0
// means that bot piece #0 is on board position 0.
// -2 = this piece is not currently placed on the board.

// --------------------------------------------------------
// OPTIONAL VALUE TO CHANGE: change to set up different board positions!!!!!!
// --------------------------------------------------------
int botPieceCount = 3;

// --------------------------------------------------------
// OPTIONAL VALUE TO CHANGE: change to set up different board positions!!!!!!
// --------------------------------------------------------
// Stores the positions of the player's pieces:
int playerPositions[MAX_PIECES] = {
  3, 15, 14,
  EMPTY_POSITION, EMPTY_POSITION, EMPTY_POSITION,
  EMPTY_POSITION, EMPTY_POSITION, EMPTY_POSITION
};



bool playerJustMoved = true;

// Stores the possible destinations for the currently tested piece:
int possibleMoves[MAX_MOVES] = {
  EMPTY_POSITION,
  EMPTY_POSITION,
  EMPTY_POSITION,
  EMPTY_POSITION
};


// Temporary copy of the pieces currently being analysed:
int positionsToCalculate[MAX_PIECES] = {
  EMPTY_POSITION, EMPTY_POSITION, EMPTY_POSITION,
  EMPTY_POSITION, EMPTY_POSITION, EMPTY_POSITION,
  EMPTY_POSITION, EMPTY_POSITION, EMPTY_POSITION
};


// Temporary list used when checking whether a position creates a mill:
int positionsForAlignment[MAX_PIECES] = {
  EMPTY_POSITION, EMPTY_POSITION, EMPTY_POSITION,
  EMPTY_POSITION, EMPTY_POSITION, EMPTY_POSITION,
  EMPTY_POSITION, EMPTY_POSITION, EMPTY_POSITION
};


// Index (from the list) of the bot's piece that will be moved:
// 0 to 8 = one of the bot's nine possible pieces.
int bestStartIndex;

// Destination position of the best move:
// 0 to 7   = outer ring
// 10 to 17 = middle ring
// 20 to 27 = inner ring
int bestDestination;

// Index of the player's piece that should be removed:
// 0 to 8 = one of the player's nine possible pieces.
int bestPieceToRemove;

// Score of the best move found so far:
int bestScore = INITIAL_SCORE;
// = -100 --> no move has been selected yet

// --------------------------------------------------------
// ARDUINO SETUP:
// --------------------------------------------------------
void setup() {

  Serial.begin(9600); // to be able to print values

  Serial.println("------------- START -------------");

  // A4 = analog input used as a source of randomness.
  randomSeed(analogRead(A4));
}


// --------------------------------------------------------
// MINIMAX ALGORITHM: CALCULATE POSSIBLE MOVES, where can each piece move to? 
// --------------------------------------------------------

// pieceIndex = index of the piece inside positionsToCalculate:
// 0 to 8 = one of the nine possible pieces.
void calculatePossibleMoves(int pieceIndex) {

  // Clear the previous results:
  for (int i = 0; i < MAX_MOVES; i++) {

    possibleMoves[i] = EMPTY_POSITION;
  }


  int currentPosition = positionsToCalculate[pieceIndex];
  // get the current board position of the piece

  int destination = getNextPosition(currentPosition + 1);
  // move to the next position


  // false = the destination is not occupied.
  if (isPositionOccupied(destination) == false) {

    // 0 = first slot of the possible-moves array.
    possibleMoves[0] = destination;
  }


  // Move to previous positon:
  destination = getPreviousPosition(currentPosition - 1);


  // false = the destination is not occupied.
  if (isPositionOccupied(destination) == false) {

    // 1 = second slot of the possible-moves array
    possibleMoves[1] = destination;
  }


  // --------------------------------------------------------
  // Move between rings:
  // There are 3 rings:
  // 0 = outer ring
  // 1 = middle ring
  // 2 = inner ring
  for (int ring = 0; ring < 3; ring++) {

    int localPosition =
      currentPosition - (ring * 10);


    // 1, 3, 5 and 7 = positions that connect two rings.
    if (localPosition == 1 ||
        localPosition == 3 ||
        localPosition == 5 ||
        localPosition == 7) {

      // Move towards the inner ring.
      //
      // 2 = inner ring index.

      if (ring != 2) {

        // 10 = numerical distance between two rings.
        destination =
          currentPosition + 10;

        // false = destination is free.
        if (isPositionOccupied(destination) == false) {

          // 2 = third slot of the possible-moves array.
          possibleMoves[2] = destination;
        }
      }


      // Move towards the outer ring.
      // 0 = outer ring index.
      if (ring != 0) {

        // 10 = numerical distance between two rings.
        destination =
          currentPosition - 10;

        // false = destination is free.
        if (isPositionOccupied(destination) == false) {

          // 3 = fourth slot of the possible-moves array.
          possibleMoves[3] = destination;
        }
      }
    }
  }
}


// --------------------------------------------------------
// Get next postion:
// --------------------------------------------------------
// Each ring contains 8 positions.
// 0 to 7   = outer ring
// 10 to 17 = middle ring
// 20 to 27 = inner ring
int getNextPosition(int position) {

  // 8  = position immediately after the outer ring.
  // 18 = position immediately after the middle ring.
  // 28 = position immediately after the inner ring.

  if (position == 8 ||
      position == 18 ||
      position == 28) {

    // 8 = number of positions on one ring.
    position = position - 8;
  }

  return position;
}


// Get previous position:
int getPreviousPosition(int position) {

  // -1 = position immediately before position 0.
  // 9  = position immediately before position 10.
  // 19 = position immediately before position 20.

  if (position == -1 ||
      position == 9 ||
      position == 19) {

    // 8 = number of positions on one ring.
    position = position + 8;
  }


  return position;
}


// Check whether a position is occupied
// Returns:
// true  = position is occupied.
// false = position is free.
bool isPositionOccupied(int position) {

  // false = no piece has been found at this position yet.
  bool occupied = false;

  // 9 = maximum number of pieces for one player.
  for (int i = 0; i < MAX_PIECES; i++) {

    if (position == botPositions[i] ||
        position == playerPositions[i]) {

      // true = the position contains a piece.
      occupied = true;
    }
  }


  return occupied;
}


// --------------------------------------------------------
// Check two positions of a mill:
// --------------------------------------------------------
// Checks whether both required positions are already occupied
// by the player represented in positionsForAlignment.
bool checkAlignment(
  int firstPosition,
  int secondPosition,
  bool millFound
) {
  // Number of required positions that have been found.
  int matchingPositions = 0;

  // 9 = maximum number of pieces in the array.
  for (int i = 0; i < MAX_PIECES; i++) {

    if (positionsForAlignment[i] == firstPosition ||
        positionsForAlignment[i] == secondPosition) {

      matchingPositions++;


      // 2 = both required positions have been found.
      if (matchingPositions == 2) {

        // true = the move completes a mill.
        millFound = true;
      }
    }
  }

  return millFound;
}


// --------------------------------------------------------
//Check whether a move creates a mill
// --------------------------------------------------------
// testedPosition = position where the piece was just placed or moved.
// Returns:
// true  = the move creates a mill.
// false = no mill is created.
bool createsMill(int testedPosition) {

  // false = no mill has been detected yet.
  bool millFound = false;

  // Even Position:
  // Even positions are the corners of a ring.
  // A corner can belong to two different mills on the same ring.

  if (testedPosition % 2 == 0) {

    // Check the two positions after the tested position.

    int firstPosition = getNextPosition(testedPosition + 1);

    int secondPosition = getNextPosition(firstPosition + 1);

    millFound =
      checkAlignment(
        firstPosition,
        secondPosition,
        millFound
      );

    // Check the two positions before the tested position
    firstPosition = getPreviousPosition(testedPosition - 1);

    secondPosition = getPreviousPosition(firstPosition - 1);

    millFound =
      checkAlignment(
        firstPosition,
        secondPosition,
        millFound
      );
  }


  // --------------------------------------------------------
  // Odd positions:
  // Odd positions are intersections between rings.
  // They can belong to:
  // 1. a mill on their current ring;
  // 2. a vertical mill connecting the three rings.
  else {

    // Check the mill on the same ring.

    int firstPosition = getNextPosition(testedPosition + 1);

    int secondPosition = getPreviousPosition(testedPosition - 1);


    millFound =
      checkAlignment(
        firstPosition,
        secondPosition,
        millFound
      );

    // Find the corresponding intersection.
    // 1, 3, 5 and 7 = possible vertical connections.

    int movedPosition = testedPosition;

    // -2 = no valid intersection has been found yet.
    int verticalAlignment = EMPTY_POSITION;

    // 3 = number of rings.
    for (int ring = 0; ring < 3; ring++) {

      // 1, 3, 5 and 7 = positions that connect rings.
      for (int position = 1;
           position <= 7;
           position += 2) {

        if (movedPosition == position) {

          verticalAlignment = position;
        }
      }


      // 10 = distance between two rings.
      movedPosition =
        movedPosition - 10;
    }

    // Find the two other positions of the vertical mill:
    // -1 = intentionally used to indicate that the first
    // position has not been found yet.
    int firstVerticalPosition = -1;

    // true = the first position has not been found yet.
    bool firstPositionFound = true;

    // 3 = check all three rings.
    for (int ring = 0; ring < 3; ring++) {

      // 10 = distance between two rings.
      int position = verticalAlignment + (ring * 10);

      if (position != testedPosition) {

        if (firstPositionFound == true) {

          firstVerticalPosition = position;

          // false = the first position has now been found.
          firstPositionFound = false;
        }


        else {

          // This is the second position of the vertical mill.
          secondPosition = position;
        }
      }
    }


    millFound =
      checkAlignment(
        firstVerticalPosition,
        secondPosition,
        millFound
      );
  }

  return millFound;
}


// --------------------------------------------------------
// Evaluate the Oponents possible moves:
// Returns:
// -2 = opponent can create a mill.
// -1 = opponent can block one of the bot's mills.
//  0 = no important threat detected.
// A higher value is better for the bot.
int evaluateOpponentMove() {

  // 0 = neutral score; no threat detected yet.
  int result = 0;

  // 9 = maximum number of opponent pieces.
  for (int pieceIndex = 0;
       pieceIndex < MAX_PIECES;
       pieceIndex++) {


    // -2 = this piece is not currently on the board.
    if (playerPositions[pieceIndex] == EMPTY_POSITION) {
      continue;
    }

    // --------------------------------------------------------
    // Copy the opponent's positions.
    // 9 = number of positions to copy.
    for (int i = 0; i < MAX_PIECES; i++) {

      positionsToCalculate[i] = playerPositions[i];
    }


    calculatePossibleMoves(pieceIndex);

    // Save the original position before simulating moves.
    int originalPosition =
      playerPositions[pieceIndex];


    // false = opponent has not found a mill yet.
    bool canCreateMill = false;

    // false = opponent has not found a blocking move yet.
    bool canBlockMill = false;


    // 4 = maximum number of possible destinations.
    for (int moveIndex = 0;
         moveIndex < MAX_MOVES;
         moveIndex++) {


      // -2 = no destination available in this slot.
      if (possibleMoves[moveIndex] == EMPTY_POSITION) {
        continue;
      }


      // Simulate the opponent's move.
      playerPositions[pieceIndex] = possibleMoves[moveIndex];

      // --------------------------------------------------------
      // Can the opponent create a mill?

      // 9 = number of pieces to copy.
      for (int i = 0; i < MAX_PIECES; i++) {

        positionsForAlignment[i] = playerPositions[i];
      }


      if (createsMill(possibleMoves[moveIndex]) == true) {

        // true = opponent can create a mill.
        canCreateMill = true;
      }


      // --------------------------------------------------------
      // Can the opponent block one of the bot's mills?

      // 9 = number of bot pieces to copy.
      for (int i = 0; i < MAX_PIECES; i++) {

        positionsForAlignment[i] = botPositions[i];
      }


      if (createsMill(possibleMoves[moveIndex]) == true) {

        // true = opponent can block a bot mill.
        canBlockMill = true;
      }
    }


    // Restore the original position.
    playerPositions[pieceIndex] = originalPosition;

    // --------------------------------------------------------
    // A mill is the most dangerous possible response.

    if (canCreateMill == true) {

      // -2 = opponent can create a mill.
      return -2;
    }


    // --------------------------------------------------------
    // Blocking is the second level of danger.
    if (canBlockMill == true) {

      // -1 = opponent can block a bot mill.
      result = -1;
    }
  }

  // 0 = no important threat detected.
  return result;
}


// --------------------------------------------------------
// Save the best move:
// --------------------------------------------------------
void saveBestMove(
  int startIndex,
  int destination,
  int score,
  int pieceToRemove
) {

  // 0 to 8 = index of one of the bot's pieces.
  bestStartIndex = startIndex;

  // Board position where the piece should move.
  bestDestination = destination;

  // Score associated with this move.
  bestScore = score;

  // 0 to 8 = index of the opponent's piece to remove.
  // -2 = no piece should be removed.
  bestPieceToRemove = pieceToRemove;
}


// --------------------------------------------------------
// PRINT THE POSITION OF THE PIECE TO REMOVE:
// --------------------------------------------------------
void printPieceToRemove() {

  Serial.println(
    "Opponent piece position to remove:"
  );

  // -2 = no opponent piece should be removed.
  if (bestPieceToRemove == EMPTY_POSITION) {

    Serial.println("NONE");
  }


  else {

    // bestPieceToRemove = index of the player's piece.
    //
    // playerPositions[bestPieceToRemove] =
    // actual board position of that piece.
    Serial.println(
      playerPositions[bestPieceToRemove]
    );
  }
}


// --------------------------------------------------------
// BOT TURN
// --------------------------------------------------------
void moveBot() {

  delay(50);

  // --------------------------------------------------------
  // Only play if the player has just finished their turn.

  // false = the bot has already played this turn.
  if (playerJustMoved == false) {

    return;
  }


  // Reset the score before searching for the best move.
  // -100 = deliberately very low starting score.
  bestScore = INITIAL_SCORE;

  // -2 = no opponent piece selected for removal yet.
  bestPieceToRemove = EMPTY_POSITION;

  // --------------------------------------------------------
  // true  = placement phase.
  // false = movement phase.
  // This is intentionally controlled manually.
  // --------------------------------------------------------
  if (inBeginning == true) {

    // --------------------------------------------------------
    // Placement phase

    // --------------------------------------------------------
    // Test every possible board position.

    // 27 = highest valid board position.
    //
    // The loop therefore checks positions 0 through 27.
    for (int position = 0;
         position <= 27;
         position++) {

      // 8 and 9 = invalid positions between outer
      // and middle rings.
      //
      // 18 and 19 = invalid positions between middle
      // and inner rings.
      if (position == 8 ||
          position == 9 ||
          position == 18 ||
          position == 19) {

        continue;
      }


      // false = position is free.
      if (isPositionOccupied(position) == false) {

        // 0 = neutral starting score for this move.
        int score = 0;

        // --------------------------------------------------------
        // Check whether this placement creates a mill.

        // 9 = number of bot pieces to copy.
        for (int i = 0; i < MAX_PIECES; i++) {

          positionsForAlignment[i] = botPositions[i];
        }


        // true = this placement creates a mill.
        bool createsNewMill = createsMill(position);


        // -2 = no opponent piece selected yet.
        int pieceToRemove = EMPTY_POSITION;

        // --------------------------------------------------------
        // If a mill is created, consider removing an
        // opponent piece.
        // --------------------------------------------------------

        if (createsNewMill == true) {

          // +10 = very strong priority for creating
          // a mill during the placement phase.
          score += 10;

          // Temporarily place the new bot piece.
          botPositions[botPieceCount] = position;

          // -100 = initial opponent-response score.
          int opponentMoveScore =
            INITIAL_SCORE;

          // --------------------------------------------------------
          // Test removing every opponent piece.

          // 9 = maximum number of opponent pieces.
          for (int opponentIndex = 0;
               opponentIndex < MAX_PIECES;
               opponentIndex++) {


            // -2 = no opponent piece at this index.
            if (playerPositions[opponentIndex] == EMPTY_POSITION) {

              continue;
            }


            int savedOpponentPosition = playerPositions[opponentIndex];


            // -2 = temporarily remove this opponent piece.
            playerPositions[opponentIndex] = EMPTY_POSITION;


            // 100 = deliberately high starting value.
            // We are looking for the worst response the
            // opponent can produce after this capture.
            int worstOpponentScore = 100;


            // 27 = highest board position.
            for (int opponentPosition = 0;
                 opponentPosition <= 27;
                 opponentPosition++) {


              // 8, 9, 18 and 19 = invalid board positions.
              if (opponentPosition == 8 ||
                  opponentPosition == 9 ||
                  opponentPosition == 18 ||
                  opponentPosition == 19) {

                continue;
              }

              // false = destination is free.
              if (isPositionOccupied(opponentPosition) == false) {


                // 0 = neutral response score.
                int responseScore = 0;

                // ------------------------------------------------
                // Can the opponent create a mill?

                for (int i = 0;
                     i < MAX_PIECES;
                     i++) {

                  positionsForAlignment[i] = playerPositions[i];
                }


                if (createsMill(opponentPosition) == true) {

                  // -2 = opponent can create a mill.
                  responseScore = -2;
                }


                // ------------------------------------------------
                // Can the opponent block a bot mill?

                // 9 = number of bot pieces.
                for (int i = 0;
                     i < MAX_PIECES;
                     i++) {

                  positionsForAlignment[i] = botPositions[i];
                }


                if (createsMill(opponentPosition) == true) {

                  // -1 = opponent can block a bot mill.
                  responseScore =
                    responseScore - 1;
                }


                // Keep the worst possible response.
                if (responseScore < worstOpponentScore) {

                  worstOpponentScore =
                    responseScore;
                }
              }
            }


            // Keep the best capture according to the future
            // position of the bot.
            if (opponentMoveScore < worstOpponentScore) {

              opponentMoveScore =
                worstOpponentScore;

              pieceToRemove =
                opponentIndex;
            }


            // Restore the opponent's piece.
            playerPositions[opponentIndex] = savedOpponentPosition;
          }


          score += opponentMoveScore;


          // Remove the temporary bot piece.
          botPositions[botPieceCount] = EMPTY_POSITION;
        }


        // ----------------------------------------------------
        // Check whether this position blocks an opponent mill.

        // 9 = number of opponent pieces.
        for (int i = 0; i < MAX_PIECES; i++) {

          positionsForAlignment[i] = playerPositions[i];
        }


        // true = the position blocks an opponent mill.
        bool blocksMill = createsMill(position);


        if (blocksMill == true) {

          // +1 = bonus for blocking an opponent mill.
          score += 1;
        }


        // ----------------------------------------------------
        // Compare with the best move found so far.
        // ----------------------------------------------------

        if (score > bestScore) {

          saveBestMove(
            botPieceCount,
            position,
            score,
            pieceToRemove
          );
        }


        // Equal scores can occasionally result in a random
        // choice to make the AI less predictable.
        else if (score == bestScore) {

          // random(1, 15) generates values from 1 to 14.
          // 1 = approximately 1/14 chance of selecting
          // this equally-scored move.
          if (random(1, 15) == 1) {

            saveBestMove(
              botPieceCount,
              position,
              score,
              pieceToRemove
            );
          }
        }
      }
    }


    // ------------------------------------------------
    // PLAY THE BEST PLACEMENT

    Serial.println(
      "According to the AI, the best placement is:"
    );

    Serial.println(
      bestDestination
    );

    // Print the ACTUAL BOARD POSITION of the piece
    // the AI wants to remove.
    printPieceToRemove();


    // Actually place the piece on the board.
    botPositions[bestStartIndex] = bestDestination;

    // -2 = no capture is required.
    if (bestPieceToRemove != EMPTY_POSITION) {

      // Remove the selected opponent piece.
      playerPositions[bestPieceToRemove] = EMPTY_POSITION;
    }


    // false = the bot has finished its turn.
    playerJustMoved = false;

    // Increase the number of placed bot pieces.
    //
    // 1 = one additional piece has been placed.
    botPieceCount =
      botPieceCount + 1;
  }


  // ------------------------------------------------
  // MOVEMENT PHASE
  // ------------------------------------------------

  else {

    // false = the game is now in the movement phase.

    // --------------------------------------------------------
    // Test every bot piece.

    // 9 = maximum number of bot pieces.
    for (int pieceIndex = 0;
         pieceIndex < MAX_PIECES;
         pieceIndex++) {


      // -2 = this piece is not currently on the board.
      if (botPositions[pieceIndex] == EMPTY_POSITION) {
        continue;
      }

      // ------------------------------------------------------
      // Copy the bot's positions.

      // 9 = number of pieces to copy.
      for (int i = 0;
           i < MAX_PIECES;
           i++) {

        positionsToCalculate[i] = botPositions[i];
      }


      // Calculate all possible destinations.
      calculatePossibleMoves(pieceIndex);

      // Save the original position before simulation.
      int originalPosition =
        botPositions[pieceIndex];


      // Store the possible moves for this piece.
      int pieceMoves[MAX_MOVES];

      // 4 = maximum number of possible moves.
      for (int i = 0;
           i < MAX_MOVES;
           i++) {

        pieceMoves[i] = possibleMoves[i];
      }


      // ------------------------------------------------
      // TEST EVERY POSSIBLE MOVE
      // ------------------------------------------------

      // 4 = maximum number of destinations.
      for (int moveIndex = 0;
           moveIndex < MAX_MOVES;
           moveIndex++) {

        // -2 = no destination available.
        if (pieceMoves[moveIndex] == EMPTY_POSITION) {
          continue;
        }

        // -2 = no opponent piece selected yet.
        int pieceToRemove =
          EMPTY_POSITION;

        // 0 = neutral starting score.
        int score = 0;


        // ----------------------------------------------------
        // Temporarily move the bot piece.

        botPositions[pieceIndex] = pieceMoves[moveIndex];


        Serial.println("Testing move:");

        Serial.println(originalPosition);

        Serial.println("-->");

        Serial.println(botPositions[pieceIndex]);


        // ----------------------------------------------------
        // Check whether this move creates a mill.

        // 9 = number of bot pieces.
        for (int i = 0;
             i < MAX_PIECES;
             i++) {

          positionsForAlignment[i] = botPositions[i];
        }


        bool createsNewMill =
          createsMill(pieceMoves[moveIndex]);


        // ------------------------------------------------
        // THE MOVE CREATES A MILL

        if (createsNewMill == true) {

          // +2 = positive score for creating a mill.
          score = 2;


          Serial.println("MILL CREATED");


          // -100 = initial opponent-response score.
          int opponentMoveScore =
            INITIAL_SCORE;


          // --------------------------------------------------
          // Test removing every opponent piece.

          // 9 = maximum number of opponent pieces.
          for (int opponentIndex = 0;
               opponentIndex < MAX_PIECES;
               opponentIndex++) {


            // -2 = no opponent piece at this index.
            if (playerPositions[opponentIndex] == EMPTY_POSITION) {

              continue;
            }


            Serial.println(
              "Testing opponent piece removal:"
            );


            // Print the actual board position rather than
            // only the array index.
            Serial.println(
              playerPositions[opponentIndex]
            );

            int savedOpponentPosition = playerPositions[opponentIndex];


            // -2 = temporarily remove the opponent piece.
            playerPositions[opponentIndex] = EMPTY_POSITION;

            // Evaluate the opponent's possible response.
            int opponentResponse = evaluateOpponentMove();

            if (opponentMoveScore < opponentResponse) {

              // Store the index internally.
              //
              // The board position can be obtained with:
              // playerPositions[opponentIndex]
              pieceToRemove =
                opponentIndex;

              opponentMoveScore =
                opponentResponse;
            }


            // Restore the opponent piece.
            playerPositions[opponentIndex] = savedOpponentPosition;
          }


          score += opponentMoveScore;

          Serial.println("Move score:");
          Serial.println(score);

          // Print the actual position of the piece
          // that this candidate move would remove.
          Serial.println(
            "Candidate opponent piece position to remove:"
          );


          // -2 = no piece selected.
          if (pieceToRemove == EMPTY_POSITION) {

            Serial.println("NONE");
          }

          else {

            // Convert the array index into the board position.
            Serial.println(
              playerPositions[pieceToRemove]
            );
          }
        }


        // ------------------------------------------------
        // THE MOVE DOES NOT CREATE A MILL
        // ------------------------------------------------

        else {

          Serial.println("No mill created");


          // Evaluate the opponent's possible response.
          int opponentMoveScore = evaluateOpponentMove();


          score += opponentMoveScore;


          Serial.println("Move score:");
          Serial.println(score);
        }


        // ------------------------------------------------
        // COMPARE WITH THE CURRENT BEST MOVE
        // ------------------------------------------------

        if (score > bestScore) {

          saveBestMove(
            pieceIndex,
            pieceMoves[moveIndex],
            score,
            pieceToRemove
          );
        }


        else if (score == bestScore) {


          // -2 = no piece would be removed.
          // Give priority to a move that allows a capture.
          if (pieceToRemove != EMPTY_POSITION &&
              bestPieceToRemove == EMPTY_POSITION) {

            saveBestMove(
              pieceIndex,
              pieceMoves[moveIndex],
              score,
              pieceToRemove
            );
          }


          // --------------------------------------------------
          // SPECIAL "0 - 0" CASE
          // --------------------------------------------------
          // 0 = neutral score.
          // This preserves the original behaviour that gives
          // the AI an additional decision in certain tied
          // situations.

          else if (bestScore == 0 && score == 0) {

            // Restore the original position.
            botPositions[pieceIndex] = originalPosition;

            // 9 = number of bot pieces.
            for (int i = 0;
                 i < MAX_PIECES;
                 i++) {

              positionsForAlignment[i] = botPositions[i];
            }


            // true = the original position was already part
            // of a mill.
            if (createsMill(originalPosition) == true) {

              saveBestMove(
                pieceIndex,
                pieceMoves[moveIndex],
                score,
                pieceToRemove
              );
            }
          }


          // --------------------------------------------------
          // RANDOM TIE BREAKER
          // --------------------------------------------------

          else {

            // random(1, 15) generates values from 1 to 14.
            //
            // 1 = approximately 1/14 chance of selecting
            // this equally-scored move.
            if (random(1, 15) == 1) {

              saveBestMove(
                pieceIndex,
                pieceMoves[moveIndex],
                score,
                pieceToRemove
              );
            }
          }
        }
      }


      // ------------------------------------------------------
      // Restore the original position of the bot piece.
      // ------------------------------------------------------

      botPositions[pieceIndex] = originalPosition;
    }


    // ------------------------------------------------
    // PLAY THE BEST MOVE
   // ------------------------------------------------

    Serial.println(
      "According to the AI, the best move is:"
    );


    // Print the starting board position.
    Serial.println(
      botPositions[bestStartIndex]
    );


    Serial.println("-->");

    // Print the destination board position.
    Serial.println(
      bestDestination
    );


    // Print the ACTUAL BOARD POSITION of the opponent's
    // piece that the AI wants to remove.
    printPieceToRemove();

    // Actually perform the selected movement.
    botPositions[bestStartIndex] =
      bestDestination;


    // -2 = no capture is required.
    if (bestPieceToRemove != EMPTY_POSITION) {

      // Remove the selected opponent piece.
      playerPositions[bestPieceToRemove] =
        EMPTY_POSITION;
    }


    // false = the bot has finished its turn.
    playerJustMoved = false;
  }
}


// ------------------------------------------------
// Main Loop
// ------------------------------------------------
void loop() {

  // Execute the bot's turn.
  moveBot();
}

