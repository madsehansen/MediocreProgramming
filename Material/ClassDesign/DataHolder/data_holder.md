# Data holders

## struct vs class
- The difference is spelling and intent
- And default access level
  * struct is default public
  * class is default private

### struct
- In a struct the expectation is that is can be treated as a collection of fields that are independant and can be modified at will
- All fields are public, and the struct will not contain any methods, unless constructors are needed by members to ensure legal default values
- The fields are not dependant on eachother
- Each field can receive the full range of values for its type
- Thus, no combination of values are actually illegal
- They may be wrong, but the system should handle any combination gracefully

### class
- In a class the expectation is that it has a limited set of legal values, and this is enforced by the class: invariant
- No fields are public, and the class will contain the methods needed to ensure the fields are always valid
- The fields depend on eachother and can often not be set independently of eachother
- The class object can only hold a subset of the range of its members' ranges
- Many, usually most, of the potential values are illegal and prohibited by the class
- Therefore the class objects should never be in an illegal state
  * Error states can be valid states, but should be avoided for most types


## Minimal but complete

### The class interface should not be larger than necessary
- Only provide the methods that are needed to operate the object in the application
- Give a complete interface, so that all basic actions can be performed
  * This does not include convenience methods
  * This does include all methods to access functionality
  * Typically getters and setters of fields are problematic for keeping the invariant
    - Must not give clients the ability to set the object in an invalid state
    - They also tend to expose the internal implementation, thus breaking encapsulation
- Extra functionality should be outside the class
  * C++ is a multi-paradigm language
  * Free functions are legal and good

### There should not be more data than necessary
- For some objects it may be desirable to cache some calculations, this is OK
- Do not hold data that can be easily calculated from other data
- Do not hold data that couples the object to other objects that are actually external, do not force a data structure that is not naturally a part of the object


## Copy and move special functions
- Data holders should support copy and move operations
  * These objects are often created in large numbers, copied around and generally used a lot
  * They may also be stored by value in collections, which may need to reallocate


## Value semantics

### These objects should be similar to int, string, vector type objects
- This does not include overloading all operators, only those that make sense for the type
- The uses for these objects is that they hold some data, this is the same as e.g. int
- Objects will often be held on stack, or as parts of other objects

### Constructors
- These classes should have copy and move constructors
  * The compiler generated ones should be OK
  * If a destructor is included, these constructors probably could be = default
- These classes probably should have a default constructor
  * This might not be OK for all, but for most
  * Ommitting a default constructor can cause usage-problems for collections
- These classes probably should have some constructors for setting a proper value
  * There might be only one, for simple classes
  * There may be several, if there are several ways to construct a valid object
- Constructors that fail to construct a valid object should prefer to throw
  * This makes it impossible to construct an invalid object
  * Setting some error-state and carrying on leaves a burden on the user of the class
  * Consider making some "will this work" type check available. This may be just documenting that the exception is possible
- If several ways to create an object from the same types are needed, use static methods
  * Then these can perform the necessary operations to make a valid object
  * They might call a private constructor

### Operators
- These classes should have copy and move assignment operators
  * The compiler generated ones should be OK
- Other operators can be added, follow the general expectation for what they should do
  * For symmetric operators, make them free functions
  * Most operators should be implementable as free functions

### Methods
- These classes should have methods for basic manipulation
- A good set of methods will make it relatively easy to make convenience functions as free functions
- A good set of methods do not have overlapping responsabilities
- Methods should prefer to throw rather than allowing an invalid object to exist
- Methods may return an error rather than throwing, but still not allow invalid object to exist

### Destructor
- Only add this if needed
- Design data holder classes primarily so that the destructor is not needed
  * Thus the compiler generated is OK
  * Sometimes it is adviseable to add a destructor so that the compiler does NOT inline it
    - It should then be = default in the cpp file
    - This happens when the destructor is large and/or inlined a lot of places

## Field order
- To avoid wasting a lot of space in the object, some care must be taken
- Data holder objects will often be held in collections, and size of the object is important for performance
- Do not use larger fields than needed
  * If an int is enough for the data, do not use a string
  * Most data types will have a very limited range, so a smaller data type can be used
  * This should not be a problem, as the inner representation should not be visible outside class
  * For some objects the extra computation to translate from a small representation
to an actual value is less then the extra time needed to fetch more cache lines
- Order the fields by alignment
  * This typically means by size
    - At least for 1-8 byte fields
    - Enums can have several sizes, unless specified at declaration
    - Allocated size is not included in size (this is on the heap)
    - Arrays are aligned the same as the elements
    - Composite fields (classes and structs) are aligned by largest alignment field
  * Order the fields so that the larger alignment fields are first, and then smaller and smaller
    - This can, in some cases, allow for using inheritance to add fields without increasing object size
  * For some objects proper ordering of fields can drastically reduce the memory requirements

## Typical data holders
- Business objects, that hold data flowing through the application
- Collections of objects
- Objects that are parts of business objects

## Example
- Circle
- Employee

## Task
- Create a data holder for tracks on a battlefield
  * A track has an ID
  * A track has a position (lat, lon, alt)
  * A track has a velocity (north, east, up)
  * A track has a class (Unknown, Civilian, Bomber, Fighter, Helicopter)
  * Position, velocity and class can be modified
  * Tracks should be transferrable over networks
    - It must be possible to extract all data fields
    - It must be possible to recreate from extracted data fields

## Provide a suggested solution
