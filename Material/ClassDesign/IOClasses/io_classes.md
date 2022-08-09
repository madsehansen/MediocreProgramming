# IO classes
- These are the classes that implement the IO functionality of the system
- Keeping these as separate objects allows them to be easily shared between components and applications
- IO should not really be considered part of the business of the application, even though it is very important
- IO should only be concerned with getting data into the application and sending data out from the application


## IO classes shall always implement some interface
- There shall always be at least two implementations, the program's implementation and the test's implementation
- This is probably one of the most efficient ways to make writing tests easy and efficient
- This also opens up the possibility to easily change the way the application does IO

## Reference semantics

### These classes should not be like ints or strings
- The typical use for these objects are create one and keep it for the duration of the program.
They are usually sent to and used by business logic classes
- Any class/function that needs these objects shall receive a reference or pointer to the interface of the IO type
- Sometimes there need to be several of one of these (they may need to have some different settings),
but this should be avoided, keep such state in a data holder instead
- Some state/dependencies may be needed/desired for ease of use, typically ResourceManagers for files/sockets/etc

### Constructors
- These object should have one or more constructors that sets up the different ways to create the object
- Copy/default constructors are not wanted. Sometimes move contructor can be used.
These objects do not copy naturally, but in a few cases they may move
- Constructors that fail to construct a valid object should prefer to throw
  * This makes it impossible to construct an invalid object
  * No invalid object means the IO is at least theoretically possible

### Operators
- No operators needed
- But sometimes they can help with readability, like insert and extract operators for streams
- Delete the copy assignment operator (and usually move assignment), as they make no sense

### Methods
- All methods should be public
- These classes shall always implement an interface, only the interface shall be used by client code
- In the case of errors, either an error is returned or an exception is thrown.
This is part of the interface, and shall be documented there. Do not mix these for any interface implementation.
- All methods not part of the interface are for setup/teardown/diagnostics only

### Destructor
- Add a virtual destructor, should probably be = default in the cpp-file.
- These objects are often used in some polymorphic context
- Since any cleanup needed shall be handled by a ResourceManager type, this is not needed in the IO-class destructor

## Typical IO classes
- Database connections
- Network connections
- File IO
- DDS readers and writers
- Test IO, very useful for unit tests

## Example
- Publishing data through DDS data writer
- Receiving data from file

## Task
- Look at the TicTacToe code and try to identify IO objects
- Can their use be improved?
- If so, how?
- Try making the IO objects into proper IO class(es), with useful interfaces
- Change the automatic tests to take advantage of this

## <Provide a suggested solution>
