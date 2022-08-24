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
- Tip: implement the game in a static library that is linked from a driver executable

## IntraCom middleware
- This simplified middleware allows several components to send messages to each other within one executable
- It is a many to many communication
- There are writers that sends data and readers that receives data
- All data sent by a writer can be read by all readers connected at the time of sending
- Data does not have a key, the same data can be sent several times, all instances will be delivered to available readers
- Data are automatically removed when all readers have received them, readers are not notified

### IntraCom usage
- There shall be a single instance of an IntraCom object, all objects that are going to communicate must use this object
- The IntraCom object can produce readers and writers for any copyable type
- Readers and writers are not shared between objects, but any number of readers and writers for any type can be created
- When a writer writes a message, this message is copied to a queue where readers can get it
- When a reader detects that there are data in the queue, it makes a callback call with an argument that is a pointer to the reader
- IntraCom has an internal thread, reader callbacks are made from this thread
- The callback must then check for which reader to read from, read all the data and process them
- A reader will not read the same data twice, so process all data when it has data

### IntraCom example
 ~~~{.cpp .numberLines}
    // In main()
    {
        IntraCom::IntraCom intracom;
        Class obj( &intracom );
        intracom.start();
        // Wait for game to end
    }

    // Class implementation
    Class::Class( IntraCom::IntraCom* a_intraCom)
        : m_reader1 { a_intraCom->createReader< Msg1 >( [this]( IntraCom::Reader* a_reader ) { readData( a_reader ); } ) };
        , m_reader2 { a_intraCom->createReader< Msg2 >( [this]( IntraCom::Reader* a_reader ) { readData( a_reader ); } ) };
        , m_writer1 { a_intraCom->createWriter< Msg3 >( ) }
    { 
        Msg3 sample;
        // init sample
        m_writer1->write( sample );
    }

    void Class::readData( IntraCom::Reader* a_reader )
    {
        if ( a_reader == m_reader1 )
        {
            for ( Msg1& sample : m_reader1->read() )
                handleMsg1( sample );
        }
        if ( a_reader == m_reader2 )
        {
            for ( Msg2& sample : m_reader2->read() )
                handleMsg2( sample );
        }
    }
~~~


## Design the classes and data types this game needs
- Use OOP for the design
- The players should have different strategies, but actual AI is not needed, random and first available square is OK
- The players should present themselves to the referee, and the referee decides if their token is X or O
- Once the players have presented themselves and received their tokens the board is created and the game is on

## One possible design
[TicTacToe_1]

## Write the code
- Remember that there must be some termination criteria
- Create a reader in main() that sets a flag when the game is finished
- Since IntraCom operates in its own thread, main() can just sleep until the flag is set

## One solution for the possible design
[TicTacToe_1]


## Automatic tests
- You have tested the system manually by running it and observing the behaviour
- The code should have automatic tests to verify that it actually does what we want
- Add a test-project (BOOST test or Google test or whatever)
- Add tests for each class and free function in the game
- Do not modify the game code, that could introduce bugs

## One solution to tests for the possible design
[TicTacToe_1]
