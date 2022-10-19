# Function/method design

## Naming
- Naming things is hard but important
- Functions should do one thing and be named accordingly
- Name functions for what they do, not how
~~~{.cpp .numberLines}
// Good
auto radiansFromDegrees( double degrees );
auto pythagoras( double x, double y, double z );

// Bad
auto to_radians( double degrees );
auto rootOfSumOfSquares( double x, double y, double z );
~~~

## Type
- The type of a function is composed of the argument types and the return type
- The type of a method also contains the class
  * And a CV-qualifier (const methods, mostly)
  * And ref-qualifier (& and &&, for making them applicable to only certain objects, this is rare)
- When the type of the function is good, it does tell a lot about the function
~~~{.cpp .numberLines}
auto funcA( function< B(const A&) >, const vector< A >& ) -> vector< B >;

auto funcB( const vector< A >& ) -> size_t;

auto funcC( const vector< A >& ) -> bool;

auto funcD( function< B( B, const A& ) >, B, const vector< A >& ) -> B;
~~~

## Arguments

### Pass by value
- When small and fast to copy
- When you are going to move the value to take ownership of data
~~~{.cpp .numberLines}
auto func( int a, double b );
auto C::C( string newValue )
    : m_value( move( newValue ) )
{ }
~~~
- Never pass the base class by value, use pointer or reference for this scenario (slicing)

### Pass by reference
- When large or otherwise slow to copy
- When you need access to the actual object passed
- When there has to be an object there (the function requires the object)
- This should be preferred over pointer, unless the optional capability of a pointer is needed
~~~{.cpp .numberLines}
auto func( vector< string >& lines )
{
    lines.push_back( "EndOfFile" );
}
~~~

### Pass by pointer
- When large and slow to copy
- When you need access to the actual object passed
- When you accept that there might not be an object (nullptr)
- If the object is not optional, use a reference
~~~{.cpp .numberLines}
auto func( vector< string >* lines )
{
    if ( lines != nullptr )
        lines->push_back( "EndOfFile" );
}
~~~

### When to const arguments
- As much as possible
- As long as you do not need to modify the argument
- As long as the argument type is const correct
~~~{.cpp .numberLines}
auto func( const vector< string >& lines )
{
    // lines.push_back( "EndOfFile" ); <- error, lines is const
}
~~~

### Hidden argument "this", and the dependency implications
- Methods have access to full object, consider using free functions where possible
- Methods must preserve the invariant of the object at all times
- Free functions only need to produce some result from some arguments
- Methods requires the class and usually an object
- Free functions are much easier to reuse, test and compose

## Return value

### Return by value
- Mostly
- When the function constructs the returned value
  * This is typically a calculation
- To protect against exposing internals to the application
  * The class may want to have a stable interface, independant of inner representation
~~~{.cpp .numberLines}
auto func( ) -> vector< string >
{
    vector< string > result;
    result.push_back( "StartOfFile" );

    return result;
}
~~~
- Never return the base class by value, use pointer or reference for this scenario (slicing)

### Return by reference
- When the function returns access to a member or something guaranteed to survive the call, so no local variables
- When the function returns access to an object held by this object, like in containers
~~~{.cpp .numberLines}
class C
{
public:
    auto func( ) -> vector< string >& // Bad, as it allows clients to change internal data
    {
        return m_myStrings;
    }
private:
    vector< string > m_myStrings;
}
~~~

### Return by pointer
- When the function returns a potential access to a member or something guaranteed to survive the call, so no local variables
- When the function returns a potential access to an object held by this object, like in containers
- Note that this can be done by returning an optional<>, which makes for clearer code
~~~{.cpp .numberLines}
class C
{
public:
    auto func( ) -> vector< string >* // Bad, as it allows clients to change internal data
    {
        if ( m_myStrings.empty() )
            return nullptr;

        return &m_myStrings;
    }
private:
    vector< string > m_myStrings;
}
~~~

### When to const returned value
- When returning access to internals, by reference or pointer
  * This is to avoid having clients use the access to internals to break the class invariant
  * This still reveals inner structure, which *may* be a problem later
- Also when operating on const data, including const methods
- If found in a table from some optimized class storage (when the object has optimized space, f.ex. holding an enum, representing a string)
- If the value is fast to create (small, no allocations) it can be returned by value instead
~~~{.cpp .numberLines}
class C
{
public:
    auto func( ) const -> const vector< string >& //  Better, clients not allowed to change internal data, but structure revealed
    {
        return m_myStrings;
    }

    auto operator+( const C& other ) const -> const C; // Disallow a + b = c

private:
    vector< string > m_myStrings;
}
~~~

### Return value optimization (RVO and NRVO)
- This basically makes a lot of copying go away
- Construct the object to return in the return statement
- Or construct one object that is returned from all paths
- Does not work for returning members, as they must be copied
- Never std::move() in a return statement

### Returning more than one thing
- Output parameters
  * Do not use this, as a general rule
  * In some cases can be faster, but this should be tested and measured and the need for speed should be documented for the function
- Return a struct
  * This creates a single entity that can be returned
  * Can have any amount of data
  * Keep small, to avoid having to deal with large objects
  * Gives names to each field
- Return a class
  * Basically the same as for structs, but also implies some invariant
  * More natural to have query methods on the returned class for checking the returned value(s)
  * Example: std::string
- std::pair and std::tuple
  * Basically structs with unnamed fields, though pair has first and second
  * These are useful as they allow for structured binding
  * std::tuple also allows for some iteration over fields, which can be useful in templates, but that is advanced


## Argument order
- When deciding the order of arguments a few things should be considered
  * Default arguments
    - These, by language rules, must be last
  * Common use
    - The arguments that would be "fixed" in a currying should be first
    - This makes the STL incorrect, as the range of data to operate on would vary, rather than the operation to perform
      * Not really a problem, it is always possible to "convenience-function" around this
    - There is a std::bind_front() that basically provides currying, in C++20
    - There is a std::bind_back() to remedy the reversed currying, in C++23
  * Overloads
    - Should have the same order for arguments that are the same
    - If possible the less-arguments function should call the more-argument function
    - This makes the difference in arguments potential default arguments and the less-argument function can be removed

## Const methods
- Const all methods that do not change the observable state of the object
- Cached calculations can be "mutable" so that they can be modified in const methods
- Do not change any observable state in a const method, ever
- Mutexes must be mutable, since it is necessary to lock them even in const methods
  * They are not really observable outside the class anyway, so this is ok
  * If the method returns access to some internal data, the mutex must remain locked
    - This calls for a locked_ptr type of return value
- Only const methods can be called on const objects
- Const is an important part of the contract, and should not be ignored

## Static methods
- Useful or just a complicated name for a free function
- Gives the function a strong relation to the class
  * Easy for intellisense
  * Ok for constructor-like functions
- Often abused
  * Singleton anti-pattern
  * Unrelated operations
  * Java programming

## Do or do not
- This is about surprises, surprises are bad
- Any function should either succeed or do nothing (observable)
  * Especially important for methods
  * Do not allow the system to get into a "halfway state"
  * Remember that exceptions can occur at nearly any point
  * Copying of native types and pointers do not throw, so this can be done after the results are ready to set the new state
  * Basic order of operations then becomes
    - Check arguments and state
      * If these are bad or not compatible, no further processing, report the problem
    - Perform operation on temporary data
      * If this fails, only temporary data have been affected and these will be cleaned up, report the problem
    - Swap in results from temporary data to members
      * This should not be able to fail (only simple copying of non-throwing data)
  * This is often simplified, but should be considered for all methods modifying the object
      * Ending up in a halfway state is potentially difficult to figure out and debug
      * Program is basically undefined behaviour from that point
  * This will often be more lines, but correct code is better than cute code

## Pure functions
- A pure function
  * is a function that only reads its arguments and returns a result
  * always returns the same value when given the same arguments, allows memoization
  * is allowed to change local variables, including pass by value arguments
  * has no side effects
  * can be called at any time
  * can be called from any thread
  * is often easy to compose with other functions
  * is easy to test
  * is easy to reason about
  * can be implemented as a table lookup
- Make functions pure if possible
  * This may mean that the pure part is moved to another function and then sent to the side-effect part later
- Methods can also be pure functions
  * This is not about pure virtuals, which is a different thing
  * Pure methods shall be const
    - Remember that the object itself is an argument
  * Pure methods do not have the same flexibility as pure functions
    - They need the implicit argument "this"

## Variables

### Name variables after what they represent
- Not the type
- Do not use abbreviations, generally
- It is OK to use abbreviations or short names that are
  * Commonly known and unambigous in the business field
  * Loop index variables (i, j, k...)
  * Limited to a very small scope
- Most coding standards place some extra requirements on names, like
  * a_ prefix for arguments
  * m_ prefix for members
  * no prefix for locals
  * also statics and globals may have prefixes

### Use a variable for a single purpose
- There is some temptation to reuse variables for new purposes later in functions, avoid this
- It is possible to limit the scope of some variables
  * This makes them easier to reason about
  * Stack space can be reused by later variables, if this is a concern, but it usually is not
  * While this does free up the name, avoid using the same name for another variable

### Const variables where possible
- Do not declare variables until they can be given a value
  * It is OK to call a function to get the value
  * Sometimes this function can be a lambda
- Do not change value of variables unless absolutely necessary
  * Mostly they do not need changing
  * Some do, mostly those that are used for iterations/loops
  * Subexpressions can be given their own names if a complex result is needed
- So most variables can be const...
- Const variables are easier to reason about
- Even for the compiler, so sometimes it may allow for better optimization as well

### Stack vs heap
- Stack is much faster than heap
  * Avoid memory allocation
  * Hot in cache, heap is often cold
- Stack does not get fragmented
  * Fragmentation of the heap can lead to serious problems
  * Unable to allocate more memory
  * C++ can have, but nobody gave it, a garbage collector, so memory lost is lost
  * There may be plenty of memory free, but only in small chunks
- Do not use the heap unless necessary
- Stack is limited in size, much smaller than heap
  * Stacks tend to be close to 1 MB though, so most local variables should reside on stack
  * Some objects may need the heap
    - Strings and collections tend to use the heap for storage even though they themselves can be on the stack
    - Arrays tend to be on the stack unless explicitly on the heap
    - Some objects that are to be used polymorphically will live naturally on the heap
    - Objects that must survive the returning from the function

## General

### Size of function
- There is no real consensus on correct function size, beyond "small"
- One study suggested about 40 lines for less bugs
- The more modern ideas are smaller than this
- I suggest "about 7 statements" as a "limit"
  * This is just a gut feeling
  * A method properly written with testing of arguments and exception safety can often be larger than this
  * Most algorithms can be described in 7 steps or less
    - Some of those steps may be complex and require separate functions
  * Many functions will be smaller, one to three statements
  * Some functions will be larger
    - Functions that initialize data
    - Functions that operates on large structures with many fields
    - Functions that are just complex
  * Guideline, not rule
- Large functions can be split into smaller functions, with descriptive names

### Functions that do different things depending on an argument
- Such arguments are often constant at the call-site
- Such arguments are often bool or some small enum
- This should normally be avoided
- Split into several functions that then do not need the argument
- If called with varying values for the argument, make a convenience function that handles the selecting of the function to call

### Function decorations (specifiers)
- Pure virtuals must have "= 0", as per language rules
- Overriding a virtual method shall have "override" to catch bugs, but not "virtual"
- The leaf class shall declare overridden virtuals as "final", still not "virtual"
- Functions that do not throw shall be "noexcept", this can help the optimizer
- Destructors shall be "noexcept", never let exceptions escape destructors
- All methods that do not change the visible state of the object shall be "const"

## constexpr
- Can be run at compiletime, or runtime, compiler decides
- Does not allow undefined behaviour, less bugs
- More capabilities as the language continues to grow
- When run at compiletime, can result in massive optimizations
  * At the cost of compile speed
- When possible, use "constexpr" on functions and methods, including constructors and destructors (C++20)
- consteval is guaranteed to be run/evaluated at compiletime, this can be used as well
- This can be very useful for generating tables and other data that does not change at runtime
  * Generating these with code can be more clear or maintainable than adding huge tables by hand
