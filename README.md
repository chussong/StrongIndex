# StrongIndex

Have you ever worked with code where multiple similar objects are tracked by IDs that are just raw integers with vaguely suggestive names?
For example, consider a project for managing students at a school. 
They may have ID numbers issued by the school as well as separate ones issued by your system.
If you keep these as fields of the form `int student_id;` and `int user_id`, someone will eventually end up using one in place of the other.
If an argument to a random function is just `int user_id`, how is a reader supposed to know that a "user_id" is a specific concept, rather than just "the ID of this user"?
Using a strong type for these IDs makes it obvious that there are multiple distinct kinds. 
Now you can pass `UserId user_id` and a reader (perhaps your future self) will immediately know that a `UserId` is a specific type of identifier.

The entire library is a few templates contained in `strong-index.hpp`.
The header provides three types of strong index: `StrongIndex::Basic`, `StrongIndex::Incrementable`, and `StrongIndex::FullArithmetic`.
In most cases you should probably use the Basic one, which can not be modified without casting to and from the underlying type.
Incrementable can be incremented and decremented, as well as adding or subtracting the underlying type.
FullArithmetic can additionally add or subtract other instances of itself, as well as multiply, divide, and mod by the underlying type.

To create a StrongIndex type within your own code, you will typically need to do something like `using UserId = StrongIndex::Basic<struct UserIdTag>;`.
The tag is necessary because C++ does not yet have metaclasses. The tag struct does not need to be defined anywhere, and each StrongIndex type should use a different tag struct.
The header also has some macros that can be used instead of explicitly writing a tag, but you probably shouldn't use them because macros are bad (they're commented out in the repository version).
Once the type is instantiated, you can use it by casting back and forth to the underlying type (by default it's `std::size_t`, but you can specify it as a template argument).

For some short examples, please see the simple program in `example.cpp`.
It includes comments explaining what each part does.

There are also some unit tests. If you're curious, you can download the repository and build `tests.cpp`, which is a binary file you can run.
They're built with the lovely [doctest](https://github.com/onqtam/doctest).
