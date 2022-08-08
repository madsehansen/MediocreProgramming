# Task: Distributed Tic-Tac-Toe
A lot of the systems we write are distributed, with several executables cooperating to solve the task.

For this task the executables are going to be simulated in one executable, for simplicity.
We will simulate the communication in a way that is similar to how this would look if there was separate executables.

The system to design, and then implement is the game of Tic-Tac-Toe. This is also known as Naughts and Crosses.

## Rules of the game
- There is a board of 3x3 squares
- There are two players, denoted by their token X and O
- The players take turn on placing their token on one square
- The goal of the game is to get three in a row, horizontally, vertically or diagonally
- If a player succeeds in getting three in a row, that player wins the game
- If no player has three in a row when there are no more free squares, the game is a draw

## Architecture
- There needs to be a referee, that holds the board
- The referee decides which player shall play next move
- The referee checks for winner/draw after each move
- Each player makes a move when allowed, only one move
- There are two players, they do not talk to each other
- Communication between the referee and the players will be done using IntraCom

## IntraCom middleware
- This simplified middleware allows several components to send messages to each other within one executable
- It is a many to many communication
- There are writers that sends data and readers that receives data
- All data sent by a writer can be read by all readers connected at the time of sending
- Data does not have a key, the same data can be sent several times, all instances will be delivered to available readers
- Data are automatically removed when all readers have received them, readers are not notified

## Design the classes and data types this game needs
- Use OOP for the design
- The players should have different strategies, but actual AI is not needed, random and first available square is OK
- The players should present themselves to the referee, and the referee decides if their token is X or O
- Once the players have presented themselves and received their tokens the board is created and the game is on

## One possible design
[Link to images of design]

## Write the code

## One solution for the possible design
[Link to the code]


## Automatic tests
- You have tested the system manually by running it and observing the behaviour
- The code should have automatic tests to verify that it actually does what we want
- Add a test-project (BOOST test or Google test or whatever)
- Add tests for each class and free function in the game
- Do not modify the game code, that could introduce bugs

## One solution to tests for the possible design
[Link to test-code]
