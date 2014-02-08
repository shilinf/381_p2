/* 
A Record ontains a unique ID number, a rating, and a title and medium name as Strings.
When created, a Record is assigned a unique ID number. The first Record created
has ID number == 1.
*/

/* *** NOTE: If after a function header is a comment "fill this in" remove the comment and replace
it with the proper code here in the header file.  All other functions should be defined
in the .cpp file. 
Comments starting with "***" are instructors to you - remove them from your finished code.
Remove this comment too. */


public:
	// Create a Record object, giving it a unique ID number by first incrementing
	// a static member variable then using its value as the ID number. The rating is set to 0.
	Record(const String& medium_, const String& title_);

	// Create a Record object suitable for use as a probe containing the supplied
	// title. The ID and rating are set to 0, and the medium is an empty String.
	Record(const String& title_);

	// Create a Record object suitable for use as a probe containing the supplied
	// ID number - the static member variable is not modified.
	// The rating is set to 0, and the medium and title are empty Strings.
	Record(int ID_);

	// Construct a Record object from a file stream in save format.
	// Throw Error exception if invalid data discovered in file.
	// No check made for whether the Record already exists or not.
	// Input string data is read directly into the member variables.
	// The record number will be set from the saved data. 
	// The static member variable used for new ID numbers will be set to the saved
	// record ID if the saved record ID is larger than the static member variable value.
	Record(std::ifstream& is);
	
	// These declarations help ensure that Record objects are unique
	Record(const Record&) = delete;	// disallow copy construction
	Record(Record&&) = delete;	// disallow move construction
	Record& operator= (const Record&) = delete; // disallow copy assignment
	Record& operator= (Record&&) = delete; // disallow move assignment

	// Accessors
	int get_ID() const
		{/*** fill this in */}
	String get_title() const
		{/*** fill this in */}
	// reset the ID counter
	static void reset_ID_counter() 
		{/*** fill this in */}
	// save the ID counter in another static member variable
	static void save_ID_counter() 
		{/*** fill this in */}
	// restore the ID counter from the value in the other static member variable
	static void restore_ID_counter() 
		{/*** fill this in */}
	// if the rating is not between 1 and 5 inclusive, an exception is thrown
	void set_rating(int rating_);
	
	// Write a Record's data to a stream in save format with final endl.
	// The record number is saved.
	void save(std::ostream& os) const;

	// This operator defines the order relation between Records, based just on the last title
	bool operator< (const Record& rhs) const
		{/*** fill this in */}

	/* *** fill in a friend declaration for the output operator */

private:
	static int ID_counter; // must be initialized to zero.
	/* *** another static member variable for the backup value of iD_counter; name is your choice */
	/* *** other private members are your choice */

	


// Print a Record's data to the stream without a final endl. 
// Output order is ID number followed by a ':' then medium, rating, title, separated by one space.
// If the rating is zero, a 'u' is printed instead of the rating.
std::ostream& operator<< (std::ostream& os, const Record& record);
