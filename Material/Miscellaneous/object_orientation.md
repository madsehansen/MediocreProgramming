
# Object orientation
- Object orientation is about managing complexity
- Classes may inherit eachother
- Class hierarchies
- Object orientated frameworks
- Visitor pattern


## Object orientation is about managing complexity
- Make code easier to reason about by providing common interfaces
- Make objects with a small and controlled interface
- Interfaces can be shared between many classes of objects

### Make code easier to reason about by providing common interfaces
- Allows objects to be substituted for other functionality
- Less clutter of code with special treatment for each type

### Make objects with a small and controlled interface
- Should follow the Open/Close principle
- Open for extension, let clients add functionality freely by adding new classes that inherit
- Closed for modification, do not let clients change the class itself
- All access to the object shall go through the public interface

  Avoid using 'friend', and never ever #define private public
- Must be reasonably complete for the type, convenience can be built from interface

  Thus the interface shall allow all operations that are needed

  It does not need to have all operations imaginable

### Example class layout

~~~{.cpp .numberLines}
class MyClass : public BaseClass, public IMyInterface
{
public: // Interface starts here
    // Constructors
    // Destructor
    // Operators
    // Methods
protected: // Base classes only
    // Interface for subclasses, typically constructors
    // Virtual methods that subclasses shall override
private: // Interface stops here
    // Members
    // Other complexity, hidden from users
};
~~~

### Interfaces can be shared between many classes of objects
- Sharing the interface makes the objects look the same to client code
- Having different classes share a useful interface makes writing general code easier
- Also allows for collections to hold pointers to different types
- Thus this makes polymorphism possible

### Shared interface
~~~{.cpp .numberLines}
class IShape
{
public:
    virtual ~IShape() = default;
    virtual void draw() = 0;
};

class Circle : public IShape
{
public:
    ...
    void draw() final;
};

class Rectangle : public IShape
{
public:
    ...
    void draw() final;
};
~~~


## Classes may inherit eachother
- Should follow the Liskov principle
- Inherit, not to reuse code, but to allow other, existing code, to use this class
- C++ allows multiple inheritance

### Should follow the Liskov principle
- Allows subclasses to be used as if they were the superclass, without surprises
- This may make the hierarchy different in code from math or modelled world

  Square is not a special case of rectangle but may be a property of rectangle

### Inherit, not to reuse code, but to allow other, existing code, to use this class
- Simple code reuse can be implemented via composition
- Composed objects can hide the dependency, with inheritance this is impossible
- This is what polymorphism is about, allowing old code to use new code

### C++ allows multiple inheritance
- This can cause problems with shared/common base classes
- Only inherit from at most one base class
- Inherit from any number of interface classes
- Interface classes have only pure virtual methods, and a virtual destructor, no data


## Class hierarchies
- Hierarchies shall be shallow
- There is a very strong dependency on the base class(es)

### Hierarchies shall be shallow
- A base class can hold basic, common functionality for the subtree
- Deep trees tend to arise with too fine level of distinctions
- Deep trees are a sign of bad design
- Often a deep hierarchy can be flattened by moving things out into other classes
- Try to avoid creating hierarchies more than 3 levels deep, interfaces not counted

### There is a very strong dependency on the base class(es)
- This also causes longer build-times since the base class must be known at class definition
- Dependencies may come with more dependencies
- Dependencies becomes visible to clients
- May be hard to refactor away later


## Object orientated frameworks
- Easy to add types
- Hard to add functionality

### Easy to add types
- They need to inherit something and then they can be used in the system
- Framework built to handle this
- Clients can add any number of types

### Hard to add functionality
- Results in having to modify all the types affected
- Framework must support the functionality
- Affects all clients as well

## Example: Shape hierarchy, traditional
~~~{.cpp .numberLines}
class IShape
{
public:
    virtual ~IShape() = default;
    virtual void draw( Canvas& a_canvas ) = 0;
    virtual void read( std::istream& a_stream ) = 0;
    virtual void write( std::ostream& a_stream ) = 0;
};

class Circle : public IShape
{
public:
    ...
    void draw( Canvas& a_canvas ) final;
    void read( std::istream& a_stream ) final;
    void write( std::ostream& a_stream ) final;
};

class Rectangle : public IShape
{
public:
    ...
    void draw( Canvas& a_canvas ) final;
    void read( std::istream& a_stream ) final;
    void write( std::ostream& a_stream ) final;
};

void drawShapes( Canvas& a_canvas, std::vector< IShape* >& a_shapes )
{
    for ( auto* shape : a_shapes )
        shape->draw( a_canvas );
}
~~~

## Visitor pattern
- Basically a way to make it easy to add functionality
- Comes with the cost that it will be hard to add types

## Example: Shape hierarchy, visitor
~~~{.cpp .numberLines}
class IVisitor;
class IShape
{
public:
    virtual ~IShape() = default;
    virtual void accept( IVisitor& a_visitor ) = 0;
};

class Circle : public IShape
{
public:
    ...
    void accept( IVisitor& a_visitor ) final;
};

class Rectangle : public IShape
{
public:
    ...
    void accept( IVisitor& a_visitor ) final;
};
~~~

## Example: Shape hierarchy, visitor

~~~{.cpp .numberLines}
class IVisitor
{
public:
    virtual ~IVisitor() = default;
    virtual void visit( Circle& a_element ) = 0;
    virtual void visit( Rectangle& a_element ) = 0;
};

class DrawVisitor : public IVisitor
{
public:
    ...
    void visit( Circle& a_element ) final;
    void visit( Rectangle& a_element ) final;
};

void forAllShapes( IVisitor& a_visitor, std::vector< IShape* >& a_shapes )
{
    for ( auto* shape : a_shapes )
        shape->accept( a_visitor );
}

void drawShapes( Canvas& a_canvas, std::vector< IShape* >& a_shapes )
{
    forAllShapes( DrawVisitor{ a_canvas }, a_shapes );
}
~~~

## Task
Make an Angle type/hierarchy.

Angles are important in a lot of code but often causes headaches,
sometimes they are victims of unit-mistakes,
sometimes they are just mixed with other things, like distances.
This happens because they are typically encoded as just free doubles.

~~~
double angle = 0; // Radians clockwise from true north
double distance = 700; // Meters
double someCalc = angle + distance; // Makes no sense but is allowed
~~~

The Angle should be a separate type, that cannot be abused in such manner.

It should support conversion to and from

- degrees (-180 to +180)
- degrees (0 to 360)
- radians (-PI to +PI)
- radians (0 to 2*PI)
- mills (0 to 6400)
- positive in clockwise direction
- positive in counter clockwise direction

It should support trigonomerty functions sin() and cos(). Also the basic operations (+, -, *, /).

In a practical application, angles do not need infinite precicion nor range. Does this affect your design?

Make and document some assumptions about the Angles and their use.

## Possible solution
