
# Unit tests
- And other automatic tests
  * (mostly unit tests)


## Why have unit tests

### Make better design
- This works better if the tests drive the design, write tests first
- Encourages dependency injection
- Encourages less dependencies

### Specify what should happen
- Test the requirements
- Understand what the code should do, write tests to verify that it does so

### Document how to use
- The tests basically documents how to use the unit, as that is what they do
- The tests do this by providing a lot of small examples

### Avoid debugger
- Starting the debugger takes longer than running the tests
- Moving to the correct place/state takes even longer
- Stepping through the code and checking the state is slow
- Debug only when you cannot write tests to answer the question

### Avoid starting application
- This is faster than the debugger, but provides less information
- In some cases starting the application can take several minutes
- When the application is started, it may take a while to maneuver to where to test
- You need to remember to test everything relevant

### Allow refactoring with confidence
- If something is broken, the tests will tell immediately
- Most refactoring will be internal things, not affecting the tests

### Catch bugs
- Sometimes
- Especially when modifying code later, and something unexpected broke

### Faster development
- Less time from coding to coding
- Reducing this time wasted is an important factor in coding speed
- Theoretically could be immediate, but tools not there yet, at least not for C++
- Later in the project, the time to make changes will grow since there is more potential for bugs due to unexpected sideeffects
  * Having unittests for all functionality makes this inefficiency grow much slower
  * While it may seem like writing and maintaining all the tests slows down development this does not happen in practice


## How to write tests

### Test one thing, if it fails it failed for one reason
- If the test can fail for several reasons, more checking/debugging is needed to pinpoint the problem if it fails
- This should also keep the testcase small and easy to understand and reason about

### Name the test according to what it tests, if it fails the name basically tells you what is wrong
- Ideally just seeing the name of the failed testcase should be enough to know what is wrong

### This makes for many testcases, which is a good thing
- Thousands is good
- Avoids the problem of forgetting to test everything manually (just changed this, so only need to test that, missing sideeffects)
- Requires that each case runs fast

### Test either a free function or a class' behaviour, not a method
- Testing free functions is easy, call them with different arguments and check the return value
- Free functions may have more effects than this, but strive for pure functions to avoid spooky action functions
- If there are free functions that come from external API, encapsulate them in a IO or business logic type class with an interface so that they can be stubbed out when testing client code
- Classes are tested by behaviour
  * A new X is empty, create an X and check that X.empty() == true
  * A new X has no elements, create an X and check that X.begin() == X.end()
  * A new X has zero size, create an X and check that X.size() == 0

### Group testcases logically
- This makes them easier to read and manage mentally
- UnitTest/XTest/AfterCreation/is_empty
- UnitTest/XTest/AfterCreation/has_no_elements
- UnitTest/XTest/AfterCreation/has_zero_size

### Only test the public interface of classes
- Internal state is just an implementation detail, and can change
- Implementation details should be open to complete rewrite without affecting tests
- Private methods that need tests should be considered refactored into free functions
- Looking at internal state for a testcase is not OK

### Test corner cases as well as normal cases
- If the range of valid input is 1-10, test 0, 1, 10, 11
- Make the tests go through all paths
  * Even complex boolean expressions where short-circuiting creates a lot of paths
- Make sure you test all legal combinations of input/state
  * Not necessary to test all values, just examples of valid/invalid values

### Test error cases
- Exercise the error handling
  * This may be the only time it is run before deployment, there could be a crash in there even if it compiles
- Check that nothing is produced if nothing should be produced
  * Much easier and faster in unittests than manual testing

### Tests shall either succeed, every time, or fail, every time
- Do not let tests fail sometimes, it is a sign of bad code design
- Tests that are unstable are hard to know when are fixed
- Tests that fail makes the developers insensitive and indifferent to the tests, so always make sure that the tests are OK

### What does a unit test test case look like
- It is placed in a hierarchy of test suites
- It has three distinct parts, marked with comments
  * Arrange, setting up the start state
  * Act, performing the act to test
  * Assert, checking that the resulting state is correct
  * Sometimes the parts "given, when, then" are used instead, this is BDD, a subset of TDD
- Arrange should be approximately one statement
- Act shall be one statement
- Assert should be approximately one statement
- Sometimes it is useful to have some common setup/teardown code
- No control statements (if, switch, loops, etc.), purely communistic code

### Example test case (unit test)
~~~{.cpp .numberLines}
BOOST_AUTO_TEST_CASE( AdderAddingOneIncreasesTheStateByOne )
{
    // Arrange
    int state { 4 };
    Adder sut { &state };

    // Act
    sut.add( 1 );

    // Assert
    BOOST_CHECK_EQUAL( state, 5 );
}
~~~
- Here the Arrange part has two statements, had the Adder object been just a collection of free functions it could have been one
- Sometimes the Arrange part is empty, if there is nothing to do there, the comment should be there though
- Sometimes the method/function called in Act returns a value that shall be checked, it must then be captured in a local value
- If the expected change/no-change in state is more than one thing, the Assert part may have several statements
- If a unit test test case is more complex than this, something is wrong with the design of the code

## How to make code testable

### Separate concerns into separate entities
- Code not really part of the class/function should be separated and tested separately
  * Do not make the God-object type of code
  * Smaller units are easier to understand
  * Units that handle a specific area of concern are easier to reuse
- IO, time and timers are separate entities from the class under test
  * These tend to make testing slow or difficult if embedded
  * IO shall always be behind an interface
  * Time shall be provided from the outside to units that need it
  * Timers shall be external to the unit, and just call a method, pass an argument that can be used to know how much time has passed
  * Doing this should provide a unit that can be tested with no delays
  * Doing this also allows for fast negative test, where no result is produced
- Business logic shall not own the state
  * A lot of code mixes the state of the system with the operations on that state
  * Schools tend to teach this, especially to teach polymorphism
  * This makes for fragile tests or breaking encapsulation
  * Keeping the data outside the operations makes it possible to have more robust tests
  * Classes operating on data may know about the data, but shall not own it

### Inject dependencies
- This allows for faster and more complete tests by allowing fake objects to be used
- Also easy test of dependency usage
- Allows faking errors so that error handling code is also tested
- Better design in general

### No global/static/singleton data unless actually constant over all runs
- These make testcases talk to eachother
  * This makes the order of testcases important
  * This makes running testcases in parallel impossible
- They make for weird and hard to find bugs (access order dependencies)
- Code becomes hard to reason about since there can be spooky action at a distance

## Speed of running translates to number of runs
- Tests should run in < 1ms per testcase
  * Expect the number of testcases to grow throughout the life of the application
- Tests should be independent of other tests (order of execution)
  * Then they can run in parallel, which increases performance for each new core used
- There should be many (hundreds or thousands) of testcases for medium sized projects
  * There should eventually be thousands of cases, maybe hundred thousands for larger applications
  * Running the tests shall not feel like waste of time
- Tests should ideally be part of the normal build (a buildstep)
  * Failing tests are bugs found before starting application -> Big win
  * Even better if they can be part of the editor and run in realtime while typing, but tools not there yet

## The tests are part of the system
- Not an afterthought
  * Writing tests after the code encourages code to be hard to test and tests to be hard to write
- Best practice is to write test before production code
  * Only write code that is tested
  * Make the test fail, and check that it does, before writing the production code so that it tests what it should
- Must be maintained with the rest of the code
  * As requirements of the code changes, the functionality changes, and therefore the tests
  * If tests are not maintained, the benefits are quickly lost and code degrades

## Other automatic tests

### Component tests
- These test full subsystems/components
- Can test several configurations of the component
- Tests simulate the part of the system needed by the component
  * Other components this component communicates with
  * May also include external systems
  * May also include hardware
  * May also include users
- If the unit tests are very good, the component tests are basically checking if all wiring between units is in place
- Negative testing (where no result shall be produced) shall be done in unit tests
- Error testing (where errors shall be produced) shall be done in unit tests
 
### System tests
- These test the full system
- Can test several configurations of the system
- Tests simulate the external agents to the system
  * External systems
  * Hardware
  * Users
- If the unit tests are very good, the system tests are basically checking if all wiring between components is in place
 
### Other tests that can be automated
- Many other tests can be automated, use the imagination
- Automating more tests can help avoiding rot
  * Performance - can the system deliver the performance needed
  * Visibility of data - can the system display all data
  * Common user operations - has the workflow been disrupted
  * Memory leaks - does the system leak memory
  * Stability - can the system operate without failure for a long time
  * Text completeness - are all texts there, for all languages and all formats
- This list is not complete, any testing/feature that can be checked automatically may qualify
- Testing all the things will be a huge endeavour, select what gives most value
- Look for common problems that occur during development, can they be tested automatically so that they are detected early?

### All tests shall be minimal to check what needs to be checked at this level
- This means that large scale tests do not check what smaller scale tests have checked
- Unit tests checks all the details of the unit, including quality and error handling
- Component tests checks that units talk OK with eachother
- System tests checks that components talk OK with eachother
- This allows the tests to be easy to maintain
- This allows the tests to run fast
- The other tests may be slow, so those are more likely to be run outside normal developer workflow
  * Scheduled to run every night/week
  * May be run on separate servers or the actual hardware
  * May also need to be started manually as part of other tests
- Status of tests should be visible to the developers, testers, managers
  * Not on some webpage, that is too easy to miss
  * Suggestion: Put it on monitors in the hallways for all to see
  * Teams should feel that the quality of testing for their components is important
