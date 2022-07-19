
# Functional programming
- Functional programming is about managing complexity
- Functions may use eachother
- Types
- Values
- Functional programming frameworks
- Function template


## Functional programming is about managing complexity
- Through hiding it in functions with a small and controlled responsability
- The functions can be passed into other functions to build more complex functions
- Template meta-programming is basically functional in nature

### Through hiding it in functions with a small and controlled responsability
- These tend to be very focussed in what they do
- They are pure, so they always return the same result from the same arguments

### The functions can be passed into other functions to build more complex functions
- Shared interfaces allows functions to be treated similarily even though that may lead to very different results
- Having the same interface can make higher order functions more customizable

### Template meta-programming is basically functional in nature
- While this is advanced, it can be nice to know if needed
- Template meta programming works on types rather than values
- Types cannot change into other types, they are immutable

## Functions may use eachother
- Can be passed as arguments to eachother
- Can be used as return values
- Composes so that it is easy to use them together
- C++ does not have a full support for this, but it can do a lot of it

### Can be passed as arguments to eachother
- Higher order functions
- Allows customization of functions
- Allows functions to be more general

### Can be used as return values
- Higher order functions
- Functions can create function-like objects with customizations

### Composes so that it is easy to use them together
- Build complex functions by composing simpler functions

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

### Functions are overloaded on argument type
- By handwritten for the type
- By templates, if the function will allways look the same


## Values
- Functional programming often uses immutable data
- This leads to very threadsafe code
- It is often slower than imperative programming
- C++ allows imperative programming mixed with functional for best of several worlds

### Functional programming often uses immutable data
- Can not be changed after creation
- Can be shared between datasets
- Causes more creation of data, even for minor changes

### This leads to very threadsafe code
- Data that does not change is only read, this is threadsafe
- Once a thread received ownership of the data, it will not change

### It is often slower than imperative programming
- Mostly due to extra creation of data and copying
- Many functional languages use garbage collection since it makes for easier programming
- These features slow down the code compared to common imperative programming
- Heap allocation and deallocation are expensive compared to stack allocation

### C++ allows imperative programming mixed with functional for best of several worlds
- This allows for selecting the model that makes sense in the code at hand
- Often the object orientation is used for large scale (overall program shape)
- Functional programming is then used on small scale (function/method and smaller features)
- Some features are common, like benefits from types

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


## Function template
- Basically a way to make it easy to add types
- Comes with the cost that the types must support the same interface
- Comes with the cost that the function will be duplicated for each type
