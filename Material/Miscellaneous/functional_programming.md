
# Functional programming
- Functional programming is about managing complexity
- Functions may use each other
- Types
- Values
- Functional programming frameworks
- Function template


## Functional programming is about managing complexity
- Make code easier to reason about by removing assignments
- Functions can be passed into and out of other functions
- Template meta-programming is basically functional in nature

### Make code easier to reason about by removing assignments
- This makes the value of variables constant
- Less need to maintain current state in your head while reading code
- Initialization is *not* assignment in this context
- Tend to lead to a lot of recursion to make loops
  * This can be slow, and hard to come up with reasonable solutions
  * For C++ it is often better to use assignments, but limit them to as few as possible
- Leads to pure functions, since they cannot change anything
  * Pure functions are good for several reasons
  * Testability
  * Optimizer friendly
  * Easy to understand the effects/results when used
- Functional languages often use special tricks to emulate assignments
  * C++ has assignment and does not need these tricks
  * The general ideas around these tricks can be interesting though

### Functions can be passed into and out of other functions
- This is Higher Order Functions, a useful technique
- Shared interfaces allows functions to be treated polymorphically
- Having the same interface make higher order functions more customizable

### Shared interface
~~~{.cpp .numberLines}
int isSpace( char a_ch ) { return a_ch == ' ' ? 1 : 0; }
int isDigit( char a_ch ) { return a_ch >= '0' && a_ch <= '9' ? 1 : 0; }

template <typename FUNC>
int count_char( FUNC a_func, const std::string_view a_str )
{
    // This function actually exists in STL: std::count_if(),
    // and it would be better to call that
    int result { 0 };
    
    for ( char ch : a_str )
        result += a_func( ch ); // Note assignment here, avoids recursion
    
    return result;
}

int count_space( const std::string_view a_str )
{
    return count_char( isSpace, a_str );
}

int count_digits( const std::string_view a_str )
{
    return count_char( isdigit, a_str );
}
~~~

### Template meta-programming is basically functional in nature
- While this is advanced, it can be nice to know if needed
- Template meta programming works on types rather than values
- Types cannot change into other types, they are immutable


## Functions may use each other
- Can be passed as arguments
- Can be used as return values
- Composes so that it is easy to use them together
- C++ does not have a full support for this, but it can do a lot of it

### Can be passed as arguments
- Higher order functions
- Allows customization of functions
- Allows functions to be more general
- STL algorithms are based on this idea

### Can be used as return values
- Higher order functions
- Functions can create function-like objects with customizations

~~~{.cpp .numberLines}
constexpr auto char_counter( char a_ch )
{
    return [a_ch]( const std::string_view a_str )
    {
        int result { 0 };
    
        for ( char ch : a_str )
            if ( a_ch == ch )
                ++result;
    
        return result;
    };
}

int count_space( const std::string_view a_str )
{
    return char_counter( ' ' )( a_str );
}
const auto countSpace { char_counter( ' ' ) };

int count_zero( const std::string_view a_str )
{
    return char_counter( '0' )( a_str );
}
const auto countZero { char_counter( '0' ) };
~~~

### Composes so that it is easy to use them together
- Build complex functions by composing simpler functions
- Writing fully composable functions require some skill and language support

### C++ does not have a full support for this, but it can do a lot of it
- Lambdas are useful for return values
- Function templates can take function-like objects as arguments
- For specific prototypes, std::function can be used
- Does sometimes create a bit verbose code, due to lack of language support for things like currying


## Types
- Functional programming is typically very type oriented
- Functions are overloaded on argument type

### Functional programming is typically very type oriented
- Encode the application data model precisely
- Make illegal state impossible
- C++ can do a lot of this, but it is verbose due to being very flexible
- Code normally does not take advantage of this, which creates more bugs

### Functions are overloaded on argument type
- By handwritten for the type, if the types require special handling
- By templates, if the function will always look the same


## Values
- Functional programming uses immutable data
- This leads to very threadsafe code
- It is often slower than imperative programming
- C++ allows imperative programming mixed with functional for best of several worlds

### Functional programming uses immutable data
- Can not be changed after creation (no assignments, only initialization)
- Easier to reason about, can be emulated in functions by const variables
- Can be shared between datastructures, may need special datastructures for this
- Causes more creation of data, even for minor changes

### This leads to very threadsafe code
- Data that does not change is only read, this is threadsafe
- Once a thread receives ownership of the data, it will not change
- No locking needed, so no deadlocks
  * Locking still needed while transferring data between threads, but only there
- Functions should be pure, no dependencies to anything but arguments
- No side-effects, no spooky action at a distance

### It is often slower than imperative programming
- Mostly due to extra creation and copying of data
- Many functional languages use garbage collection since it makes for easier programming
- Heap allocation and deallocation are expensive compared to stack allocation
- These features slow down the code compared to common imperative programming

### C++ allows imperative programming mixed with functional for best of several worlds
- This allows for selecting the model that makes sense in the code at hand
- Often object orientation is used for large scale (overall program shape)
- Functional programming is then used on small scale (function/method and smaller features)
- Some features are common, like benefits from types and pure functions


## Functional programming frameworks
- Hard to add types
- Easy to add functionality

### Hard to add types
- They need to be supported by all functions they will be used in
- Framework built to have all supported types
- Clients can typically not extend with new types
- Adding new types to framework causes all clients to have to modify their functions

### Easy to add functionality
- Clients can add any number of new functions
- Adding new functions to framework does not affect clients

## Example: Shapes
~~~{.cpp .numberLines}
struct Position
{
    double x;
    double y;
};

class Circle
{
public:
    ...
    double radius() const;
    Position position() const;
};

class Rectangle
{
public:
    ...
    double height() const;
    double width() const;
    Position position() const;
};
~~~

## Example: Shapes

~~~{.cpp .numberLines}
using Shape = std::variant< Circle, Rectangle >;

void draw( Canvas& a_canvas, const Circle& a_shape )
{
    ...
}

void draw( Canvas& a_canvas, const Rectangle& a_shape )
{
    ...
}

void drawShapes( Canvas& a_canvas, std::vector< Shape >& a_shapes )
{
    auto DrawVisitor { [&a_canvas]( auto& shape ) { draw( a_canvas, a_shape ); } };

    for ( auto& shape : a_shapes )
        std::visit( DrawVisitor, shape );
}
~~~


## Function template
- Basically a way to make it easy to add types
- Comes with the cost that the types must support the same interface
- Comes with the cost that the function will be duplicated for each type
