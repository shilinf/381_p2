#include "String.h"
#include <iostream>

using namespace std;

// this function outputs the number and memory usage of all strings
void print_String_info();

void test_fn1();
String test_fn2();
String test_fn3(String s);


int main ()
{
	String::set_messages_wanted(true);

    print_String_info();
	// call a function that will create and destroy a couple of strings
	cout << "\nWatch them go in and out of scope" << endl;
	test_fn1();
	print_String_info();
    	
    {
    cout << "\nDemonstrate initialization and copy and move construction" << endl;
    cout << "Initialize a with String a = \"Hello\";" << endl;

/* Technically, initializing a variable has to be done with something of the same type. For example,
    int i = 3.14;
requires that the compiler convert 3.14 into an integer, and create the machine instructions to initialize
i with that integer value.  Compare this with:
    String a = "Hello";
where the initial value is a C-string, not a String. The compiler has to first convert the C-string into a String. It does this by taking
advantage of the fact that a constructor that takes a single argument of a particular other type is a way to convert that type into the
class type. So the compiler first constructs an unnamed String using the C-string "Hello" as a constructor parameter, and then 
copy or move constructs String a from it.  
 
However, almost all compilers can short-cut this and simply initialize String a using "Hello" as a constructor parameter, 
leaving out the unnamed String construction and copy/move construction from it. Most compilers are agressive about eliminating 
these logically unnecessary constructor calls, even if it can change the side effects produced by the code. 

This is called "constructor elision" - from "elide", meaning "to omit", from the Latin "elidere", meaning "to crush out".
*/
	String a = "Hello";
    cout << "Construct b from a" << endl;
	String b(a);
    cout << "Construct c from a + b" << endl;
    String c(a + b);
    cout << "a:" << a << " b:" << b << " c:" << c << endl;
    cout << "Construct d from function return value" << endl;
    String d(test_fn2());
    cout << "d:" << d << endl;
    print_String_info();
    }
	
	{
    cout << "\nDemonstrate assignment operators" << endl;
    String a, b, c;
	cout << "Assign from a C-string" << endl;
	// assign from a C-string
	a = "Good Morning!";
    cout << "Assign from another String" << endl;
	b = a;
    cout << "Assign from a + b" << endl;
    c = a + b;
    cout << "a:" << a << " b:" << b << " c:" << c << endl;
    print_String_info();
    }
	
	{
    cout << "\nDemonstrate call and return by value" << endl;
    String a("initial a"), b("initial b"), c("initial c");
	cout << "Call a function with a call-by-value argument that returns that same variable" << endl;
	b = test_fn3(a);
	cout << "function call result: " << b << endl;
    cout << "Call the same function with an argument of a + b" << endl;
    c = test_fn3(a + b);
    cout << "function call result: " << c << endl;
    cout << "a:" << a << " b:" << b << " c:" << c << endl;
    print_String_info();
    }

    {
    cout << "\nDemonstrate move construction and assignment using std::move" << endl;
    String a{"aaa"}; // C+11 style initialization
    cout << "a has " << a << endl;
    print_String_info();
    cout << "move construct b from a" << endl;
    String b{std::move(a)};
    cout << "a now has \"" << a << "\", b has \"" << b << "\"" << endl;
    print_String_info();
    cout << "now move assign a from b" << endl;
    a = std::move(b);
    cout << "a now has \"" << a << "\", b has \"" << b << "\"" << endl;
    print_String_info();
    }

    {
    cout << "\nDemonstrate swaping" << endl;
    String a{"aaa"}; // C+11 style initialization
    String b{"bbb"};
    print_String_info();
    cout << "a now has \"" << a << "\", b has \"" << b << "\"" << endl;
    cout << "let's swap their guts with String::swap!" << endl;
    a.swap(b);
    cout << "a now has \"" << a << "\", b has \"" << b << "\"" << endl;
    cout << "let's swap their guts back with std::swap which uses std::move through a temporary String!" << endl;
    std::swap(a, b);
    cout << "a now has \"" << a << "\", b has \"" << b << "\"" << endl;
    print_String_info();
    }
}


void test_fn1()
{
	cout << "in test_fn1:" << endl;
	String s1("Good morning!");
	String s2("Good night!");
	cout << s1 << s2 << endl;
	// check number of strings now existing
	print_String_info();
}

/*
In addition to eliding constructor calls in initialization, almost all compilers will also perform the Return Value Optimization (RVO)
to eliminate the copy or move construction of the returned value;
instead of building the return value in a local variable and then copying it out into the unnamed returned
variable in the the caller's stack space, the compiler builds the local variable in that same
returned variable space, thereby saving a copy or move construction. This happens in the function below.
 */

String test_fn2()
{
    cout << "in test_fn2" << endl;
    String s("xxx");
    cout << "test_fn2 returning: " << s << endl;
    return s;
}

/*
But in the type of function shown below, where the returned value is the same variable as the input parameter,
The RVO cannot be done! The variable s can't be placed both in the local stack space of the parameter and in the caller's stack space 
for the returned value. This forces the compiler to call a copy or move constructor to get the returned value into 
the returned variable space.
*/

String test_fn3(String s)
{
	cout << "test_fn3 got:" << s << endl;
	// check number of strings now existing
	print_String_info();
    s = "!!!";
	cout << "test_fn3 returning: " << s << endl;
	print_String_info();
	return s;
}

void print_String_info()
{
	cout << String::get_number() << " Strings using " << String::get_total_allocation() << " bytes"  << endl;
}

