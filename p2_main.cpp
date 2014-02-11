#include "Ordered_list.h"
#include "Record.h"
#include "Collection.h"
#include "Utility.h"
#include <iostream>
#include <fstream>
#include <cstring>

using std::cin;


struct Compare_Record_ID {
    bool operator() (const Record* p1, const Record* p2) const {return p1->get_ID() < p2->get_ID();}
};

static Ordered_list<Collection> catalog;
static Ordered_list<Record *, Less_than_ptr<Record *>> library_ordered_by_title;
static Ordered_list<Record *, Compare_Record_ID> library_ordered_by_id;


/* functions to handle command */
void find_Record_match_title(void);
void print_Record_match_id(void);
void print_Collection_match_name(void);
void print_Records(void);
void print_Catalog(void);
void print_memory_allocation(void);
void add_Record(void);
void add_Collection(void);
void add_Record_to_Collection(void);
void modify_Record_rating(void);
void delete_Record_from_Library(void);
void delete_Collection_from_Catalog(void);
void delete_Record_from_Collection(void);
void clear_Library(void);
void clear_Catalog(void);
void clear_all_data(void);
void save_all_data(void);
void restore_all_data(void);
void quit(void);

/* compare functions for OC_comp_fp_t */
int compare_Record_id(const void* data_ptr1, const void* data_ptr2);
int compare_Collection_name(const void* data_ptr1, const void* data_ptr2);
int compare_id_with_Record(const void* arg_ptr, const void* data_ptr);
int compare_name_with_Collection(const void* arg_ptr, const void* data_ptr);

/* implementation for OC_apply_if_arg_fp_t */
bool check_record_in_Collection (Collection collection, Record *arg_ptr);

/* implementation for OC_apply_if_fp_t */
bool check_Collection_empty (Collection collection);

/* implementation for OC_apply_fp_t*/
void print_Collection (Collection &collection_data, std::ostream& os);
void free_Record(Record *record_ptr);
void OC_destory_Collection(void *data_ptr);

/* implementation for OC_apply_arg_fp_t*/
void save_Collection_item(Collection& collection, std::ostream& os);
void save_Record_item(Record *record_ptr, std::ostream& os);

/* helper functions */
void handle_invalid_command_error(void);
void read_check_title(String &title);
FILE *open_file(const char *mode);
void read_collection_name(char *collection_name);
Ordered_list<Collection>::Iterator find_collection_iterator(void);
void handle_restore_all_data_invalid(FILE *fp);
void discard_input_remainder(void);
void read_and_check_integer(int *id_input);
int check_Record_ptr_find_by_id(void *find_ptr);
int trim_title(String &title);

int main ()
{
    while (1) {
        char action, object;
        printf("\nEnter command: ");
        cin >> action;
        cin >> object;
        try {
            switch (action) {
                case 'f':
                    switch (object) {
                        case 'r':
                            find_Record_match_title();
                            break;
                        default:
                            handle_invalid_command_error();
                            break;
                    }
                    break;
                case 'p':
                    switch (object) {
                        case 'r':
                            print_Record_match_id();
                            break;
                        case 'c':
                            print_Collection_match_name();
                            break;
                        case 'L':
                            print_Records();
                            break;
                        case 'C':
                            print_Catalog();
                            break;
                        case 'a':
                            print_memory_allocation();
                            break;
                        default:
                            handle_invalid_command_error();
                            break;
                    }
                    break;
                case 'a':
                    switch (object) {
                        case 'r':
                            add_Record();
                            break;
                        case 'c':
                            add_Collection();
                            break;
                        case 'm':
                            add_Record_to_Collection();
                            break;
                        default:
                            handle_invalid_command_error();
                            break;
                    }
                    break;
                case 'm':
                    switch (object) {
                        case 'r':
                            modify_Record_rating();
                            break;
                        default:
                            handle_invalid_command_error();
                            break;
                    }
                    break;
                case 'd':
                    switch (object) {
                        case 'r':
                            delete_Record_from_Library();
                            break;
                        case 'c':
                            delete_Collection_from_Catalog();
                            break;
                        case 'm':
                            delete_Record_from_Collection();
                            break;
                        default:
                            handle_invalid_command_error();
                            break;
                    }
                    break;
                case 'c':
                    switch (object) {
                        case 'L':
                            clear_Library();
                                printf("All records deleted\n");
                            break;
                        case 'C':
                            clear_Catalog();
                            printf("All collections deleted\n");
                            break;
                        case 'A':
                            clear_all_data();
                            printf("All data deleted\n");
                            break;
                        default:
                            handle_invalid_command_error();
                            break;
                    }
                    break;
                case 's':
                    switch (object) {
                        case 'A':
                            save_all_data();
                            break;
                        default:
                            handle_invalid_command_error();
                            break;
                    }
                    break;
                case 'r':
                    switch (object) {
                        case 'A':
                            restore_all_data();
                            break;
                        default:
                            handle_invalid_command_error();
                            break;
                    }
                    break;
                case 'q':
                    switch (object) {
                        case 'q':
                            quit();
                            return 0;
                            break;
                        default:
                            handle_invalid_command_error();
                            break;
                    }
                default:
                    handle_invalid_command_error();
                    break;
            }
        } catch (Error& error) {
            cout << error.msg << endl;
            discard_input_remainder();
        } catch (String_exception& exception) {
            cout << exception.msg <<endl;
        } catch (exception e) {
            cout << "Unknown exception caught" <<endl;
        }
    }
    return 0;
}


/* Print error message for invalid comamnd and read to new line */
void handle_invalid_command_error(void)
{
    printf("%s", "Unrecognized command!\n");
    discard_input_remainder();
}


/* find and print the specified record with the matching title */
void find_Record_match_title(void)
{
    String title;
    read_check_title(title);
    Record find_temp(title);
    auto find_item_iterator = library_ordered_by_title.find(&find_temp);

    if (find_item_iterator != library_ordered_by_title.end())
        cout << *find_item_iterator;
    else
        throw Error("No record with that title!\n");
}


/* print the specified record with the matching ID number */
void print_Record_match_id(void)
{
    int id_input;
    read_and_check_integer(&id_input);
    Record find_temp(id_input);
    auto find_item_iterator = library_ordered_by_id.find(&find_temp);

    if (find_item_iterator != library_ordered_by_id.end())
        cout << *find_item_iterator;
    else
        throw Error("No record with that ID!\n");
}


/* print collection - print each record in the collection with the
 specified name */
void print_Collection_match_name(void)
{
    auto find_item_iterator = find_collection_iterator();
    cout << *find_item_iterator;
}



/* print all the records in the Library */
void print_Records(void)
{
    if (library_ordered_by_title.empty())
        cout << "Library is empty" << endl;
    else {
        cout << "Library contains " << library_ordered_by_title.size() << " records:" <<endl;
        apply_arg_ref(library_ordered_by_title.begin(), library_ordered_by_title.end(), print_Record, cout);
    }
}

/* print the Catalog - print all the collections in the Catalog */
void print_Catalog(void)
{
    if (catalog.empty()) {
        cout << "Catalog is empty" <<endl;
    }
    else {
        cout << "Catalog contains " << catalog.size() << " collections:" <<endl;
        apply_arg_ref(catalog.begin(), catalog.end(), print_Collection, cout);
    }
}


/* print the collection passed in */
void print_Collection (Collection &collection_data, std::ostream& os)
{
    os << collection_data;
}




/* print memory allocations  */
void print_memory_allocation(void)
{
    cout << "Memory allocations:" <<endl;
    cout << "Records: " << library_ordered_by_title.size() <<endl;
    cout << "Collections: " << catalog.size() << endl;
    cout << "Lists: " << g_Ordered_list_count << endl;
    cout << "List Nodes: " << g_Ordered_list_Node_count << endl;
    cout << "Strings: " << String::get_number() << " with " << String::get_total_allocation() << " bytes total" << endl;
    
    //is this the right way to call static function?
}




/* add a record to the Library */
void add_Record(void)
{
    String medium, title;
    cin >> medium;
    read_check_title(title);
    
    Record find_temp(title);
    auto find_item_iterator = library_ordered_by_title.find(&find_temp);//duplicate code, check that afterwords
    if (find_item_iterator == library_ordered_by_title.end()) {
        Record *new_record = new Record(medium, title);
        library_ordered_by_title.insert(new_record);
        library_ordered_by_id.insert(new_record);
        cout << "Record " << new_record->get_ID() << " added" <<endl;
    }
    else
        throw Error("Library already has a record with this title!");
}



/* add a collection with the specified name */
void add_Collection(void)
{
    String collection_name;
    cin >> collection_name;
    Collection find_temp(collection_name);
    auto find_item_iterator = catalog.find(find_temp);
    if (find_item_iterator == catalog.end()) {
        Collection new_collection(collection_name);
        catalog.insert(new_collection);
        cout << "Collection " << collection_name << " added" <<endl;
    }
    else
        throw Error("Catalog already has a collection with this name!");
    
    
    
    // consider whether we can simplify the endl;
}



/* add a record to a specified collection */
void add_Record_to_Collection(void)
{
    auto find_Collection_item_iterator = find_collection_iterator();
    int id_input;
    read_and_check_integer(&id_input);
    
    // this part should be simplified
    Record temp(id_input);
    auto find_Record_item_iterator = library_ordered_by_id.find(&temp);
    
    if (find_Record_item_iterator == library_ordered_by_id.end())
        throw Error("No record with that ID!");
    find_Collection_item_iterator->add_member(*find_Record_item_iterator);
    cout << "Member " << id_input << " " << (*find_Record_item_iterator)->get_title() <<" added" << endl;
}

/* modify the rating of the specified record with the matching ID number */
void modify_Record_rating(void)
{
    int id_input, rating_input;
    read_and_check_integer(&id_input);
    Record temp(id_input);
    auto find_Record_item_iterator = library_ordered_by_id.find(&temp);
    
    if (find_Record_item_iterator == library_ordered_by_id.end())
        throw Error("No record with that ID!"); // also considering simplify this part.
    
    read_and_check_integer(&rating_input);
    cout << "Rating for record " << id_input << " changed to " << rating_input <<endl;
}


/* delete the specified record from the Library */
void delete_Record_from_Library(void)
{
    String title;
    read_check_title(title);
    Record temp(title);
    auto find_Record_item_iterator = library_ordered_by_title.find(&temp);
    if (find_Record_item_iterator == library_ordered_by_title.end())
        throw Error("No record with that title!");
    if (apply_if_arg(catalog.begin(), catalog.end(), check_record_in_Collection, *find_Record_item_iterator))
        throw Error("Cannot delete a record that is a member of a collection!");
    int record_ID = (*find_Record_item_iterator)->get_ID();
    library_ordered_by_title.erase(find_Record_item_iterator);
    Record temp_id(record_ID);
    library_ordered_by_id.erase(library_ordered_by_id.find(&temp_id));
    delete (*find_Record_item_iterator);
    cout << "Record " << record_ID << " " << title << " deleted" <<endl;
}





/* delete the specified collection from the Catalog */
void delete_Collection_from_Catalog(void)
{
    auto find_Collection_item_iterator = find_collection_iterator();
    catalog.erase(find_Collection_item_iterator);
    cout << "Collection " << find_Collection_item_iterator->get_name() << " deleted" << endl;
}



/* delete the specified record as member of the a specified collection */
void delete_Record_from_Collection(void)
{
    
    // very similar to add record to collection, find way to simplify it
    auto find_Collection_item_iterator = find_collection_iterator();
    int id_input;
    read_and_check_integer(&id_input);
    
    // this part should be simplified
    Record temp(id_input);
    auto find_Record_item_iterator = library_ordered_by_id.find(&temp);
    
    if (find_Record_item_iterator == library_ordered_by_id.end())
        throw Error("No record with that ID!");
    find_Collection_item_iterator->remove_member(*find_Record_item_iterator);
    cout << "Member " << id_input << " " << (*find_Record_item_iterator)->get_title() <<" deleted" << endl;

}



/* Clear the Library. If collections are not empty, return 0. Otherwise,
 return 1. */
void clear_Library(void)
{
    if (apply_if(catalog.begin(), catalog.end(), check_Collection_empty))
        throw Error("Cannot clear all records unless all collections are empty!");
    apply(library_ordered_by_title.begin(), library_ordered_by_title.end(), free_Record);
    library_ordered_by_title.clear();
    library_ordered_by_id.clear();
    Record::reset_ID_counter();
}




/* clear the Catalog: destroy all of the collections in the Catalog,
 and clear the Catalog */
void clear_Catalog(void)
{
    catalog.clear();
}


/*  clear all data: first clear the Catalog as in cC,
 then clear the Library as in cL */
void clear_all_data(void)
{
    clear_Catalog();
    clear_Library();
}




/* save all data: write the Library and Catalog data to the named file */
void save_all_data(void)
{
    String file_name;
    cin >> file_name;
    ofstream output_file(file_name.c_str());
    if (!output_file.is_open())
        throw Error("Could not open file!");
    output_file << library_ordered_by_title.size() << endl;
    apply_arg_ref(library_ordered_by_title.begin(), library_ordered_by_title.end(), save_Record_item, output_file);
    output_file << catalog.size() << endl;
    apply_arg_ref(catalog.begin(), catalog.end(), save_Collection_item, output_file);
    output_file.close();
    cout << "Data saved" <<endl;
}




/* restore all data - restore the Library and Catalog data from the file */
void restore_all_data(void)
{
    String file_name;
    cin >> file_name;
    ifstream input_file(file_name.c_str());
    if (!input_file.is_open())
        throw Error("Could not open file!");
    Ordered_list<Collection> local_catalog(catalog);
    Ordered_list<Record *, Less_than_ptr<Record *>> local_library_ordered_by_title(library_ordered_by_title);
    Ordered_list<Record *, Compare_Record_ID> local_library_ordered_by_id(library_ordered_by_id);
    catalog.clear();
    library_ordered_by_id.clear();
    library_ordered_by_title.clear();
    Record::save_ID_counter();
    Record::reset_ID_counter();
    try {
        int num_record;
        if (!(input_file >> num_record))
            throw Error("Invalid data found in file!");
        for (int i=0; i < num_record; i++) {
            Record *new_record = new Record(input_file);
            library_ordered_by_title.insert(new_record);
            library_ordered_by_id.insert(new_record);
        }
        int num_collection;
        if (!(input_file >> num_collection))
            throw Error("Invalid data found in file!");
        for (int i = 0; i < num_collection; i++) {
            Collection new_collection(input_file, library_ordered_by_title);
            catalog.insert(new_collection);
        }
        input_file.close();
        apply(local_library_ordered_by_title.begin(), local_library_ordered_by_title.end(), free_Record);
        cout << "Data loaded" <<endl;
    } catch (Error &error) {
        clear_all_data();
        library_ordered_by_title.swap(local_library_ordered_by_title);
        library_ordered_by_id.swap(local_library_ordered_by_id);
        catalog.swap(local_catalog);
        Record::restore_ID_counter();
        throw;
    }
}



/* clear all data (as in cA), and also destroy the Library and Catalog
 containers themselves, so that all memory is deallocated, and then terminate */
void quit(void)
{
    clear_all_data();
    cout << "All data deleted" << endl;
    cout << "Done" << endl;
}





/* arg_ptr is a FILE pointer. Save Record to that file. */
void save_Record_item(Record *record_ptr, std::ostream& os)
{
    record_ptr->save(os);
}

/* arg_ptr is a FILE pointer. Save Record to that file. */
void save_Collection_item(Collection& collection, std::ostream& os)
{
    collection.save(os);
}



void free_Record(Record *record_ptr)
{
    delete record_ptr;
}

bool check_Collection_empty (Collection collection)
{
    return !collection.empty();
}



bool check_record_in_Collection (Collection collection, Record *arg_ptr)
{
    return collection.is_member_present(arg_ptr);
}









/* Find the collection pointer from catalog with the specified collection name.
 Print error message and read to new line if the collection doesn't exist*/
Ordered_list<Collection>::Iterator find_collection_iterator(void)
{
    String collection_name;
    cin >> collection_name;
    Collection find_temp(collection_name);
    auto find_item_iterator = catalog.find(find_temp);
    if (find_item_iterator == catalog.end())
        throw Error("No collection with that name!\n");
    return find_item_iterator;
}










/* check Record ptr find by ID and print error message if find_ptr is null
 return 0 if find_ptr is null, 1 if not
int check_Record_ptr_find_by_id(void *find_ptr)
{
    if (!find_ptr) {
        printf("No record with that ID!\n");
        discard_input_remainder();
        return 0;
    }
    else
        return 1;
}*/








/* Read in an integer and check whether scanf succeeds.
 Return 1 if scanf succeeds, 0 if not*/
void read_and_check_integer(int *id_input)
{
    if (!(cin >> *id_input))
        throw Error("Could not read an integer value!\n");
}






/* Read title and check whether its valid.
 return 1 if the title is valie, 0 if not.*/
void read_check_title(String &title)
{
    getline(cin, title);
    if (!trim_title(title))
        throw Error("Could not read a title!\n");
}



int trim_title(String &title)
{
    int valid = 0, i = 0;
    while (i < title.size()) {
        while (isspace(title[i]))
            title.remove(i, 1);
        while (i < title.size() && !isspace(title[i])) {
            ++i;
            valid = 1;
        }
        /* If title doesn't touch '\0', add one space to end of current string*/
        if (i < title.size())
            title[i++] = ' ';
    }
    /* If the last character before '\0' is space and there exists non-sapce
     character in the string, put the '\0' one position before to
     occupy the trailing whitespace. */
    if (isspace(title[title.size()-1]) && valid)
        title.remove(title.size()-1, 1);
    return valid;
}





/* Read to new line */
void discard_input_remainder(void)
{
    while (cin.get() != '\n') {
        ;
    }
}


















