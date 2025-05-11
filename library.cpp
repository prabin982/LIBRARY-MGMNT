#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <limits>

class Date {
private:
    int day;
    int month;
    int year;

public:
    Date() : day(1), month(1), year(2023) {}
    
    Date(int d, int m, int y) : day(d), month(m), year(y) {}
    
    void setDate(int d, int m, int y) {
        day = d;
        month = m;
        year = y;
    }
    
    std::string toString() const {
        return std::to_string(day) + "/" + std::to_string(month) + "/" + std::to_string(year);
    }
    
    // Add days to the current date
    void addDays(int days) {
        // Simple implementation - doesn't handle month/year changes properly
        day += days;
        // In a real application, you would need proper date arithmetic
    }
};

class Book {
private:
    std::string id;
    std::string title;
    std::string author;
    std::string category;
    bool isIssued;
    std::string issuedTo;
    Date issueDate;
    Date returnDate;

public:
    Book(const std::string& i, const std::string& t, const std::string& a, const std::string& c)
        : id(i), title(t), author(a), category(c), isIssued(false), issuedTo("") {}
    
    std::string getId() const { return id; }
    std::string getTitle() const { return title; }
    std::string getAuthor() const { return author; }
    std::string getCategory() const { return category; }
    bool getIsIssued() const { return isIssued; }
    std::string getIssuedTo() const { return issuedTo; }
    Date getIssueDate() const { return issueDate; }
    Date getReturnDate() const { return returnDate; }
    
    void issueBook(const std::string& student) {
        isIssued = true;
        issuedTo = student;
        
        // Set current date as issue date
        issueDate = Date();
        
        // Set return date as 14 days from now
        returnDate = issueDate;
        returnDate.addDays(14);
    }
    
    void returnBook() {
        isIssued = false;
        issuedTo = "";
    }
};

class Library {
private:
    std::vector<Book> books;

public:
    void addBook(const Book& book) {
        books.push_back(book);
    }
    
    bool removeBook(const std::string& id) {
        auto it = std::find_if(books.begin(), books.end(), 
                              [&id](const Book& b) { return b.getId() == id; });
        
        if (it != books.end()) {
            books.erase(it);
            return true;
        }
        return false;
    }
    
    Book* findBook(const std::string& id) {
        auto it = std::find_if(books.begin(), books.end(), 
                              [&id](const Book& b) { return b.getId() == id; });
        
        if (it != books.end()) {
            return &(*it);
        }
        return nullptr;
    }
    
    std::vector<Book*> searchBooks(const std::string& query) {
        std::vector<Book*> results;
        
        for (auto& book : books) {
            if (book.getTitle().find(query) != std::string::npos ||
                book.getAuthor().find(query) != std::string::npos ||
                book.getCategory().find(query) != std::string::npos) {
                results.push_back(&book);
            }
        }
        
        return results;
    }
    
    std::vector<Book>& getAllBooks() {
        return books;
    }
};

class LibraryUI {
private:
    Library library;
    
    void clearScreen() {
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif
    }
    
    void waitForKeyPress() {
        std::cout << "\nPress Enter to continue...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();
    }
    
    void displayHeader() {
        clearScreen();
        std::cout << "========================================\n";
        std::cout << "       LIBRARY MANAGEMENT SYSTEM        \n";
        std::cout << "========================================\n\n";
    }
    
    void addBookUI() {
        displayHeader();
        std::cout << "ADD NEW BOOK\n\n";
        
        std::string id, title, author, category;
        
        std::cout << "Enter Book ID: ";
        std::cin >> id;
        std::cin.ignore();
        
        // Check if book with this ID already exists
        if (library.findBook(id) != nullptr) {
            std::cout << "\nError: A book with this ID already exists!\n";
            waitForKeyPress();
            return;
        }
        
        std::cout << "Enter Title: ";
        std::getline(std::cin, title);
        
        std::cout << "Enter Author: ";
        std::getline(std::cin, author);
        
        std::cout << "Enter Category: ";
        std::getline(std::cin, category);
        
        Book newBook(id, title, author, category);
        library.addBook(newBook);
        
        std::cout << "\nBook added successfully!\n";
        waitForKeyPress();
    }
    
    void removeBookUI() {
        displayHeader();
        std::cout << "REMOVE BOOK\n\n";
        
        std::string id;
        std::cout << "Enter Book ID to remove: ";
        std::cin >> id;
        
        if (library.removeBook(id)) {
            std::cout << "\nBook removed successfully!\n";
        } else {
            std::cout << "\nError: Book not found!\n";
        }
        
        waitForKeyPress();
    }
    
    void searchBooksUI() {
        displayHeader();
        std::cout << "SEARCH BOOKS\n\n";
        
        std::string query;
        std::cout << "Enter search term (title/author/category): ";
        std::cin.ignore();
        std::getline(std::cin, query);
        
        auto results = library.searchBooks(query);
        
        if (results.empty()) {
            std::cout << "\nNo books found matching your search.\n";
        } else {
            std::cout << "\nSearch Results:\n";
            std::cout << "----------------------------------------------------------------------\n";
            std::cout << std::left << std::setw(10) << "ID" 
                      << std::setw(30) << "Title" 
                      << std::setw(20) << "Author" 
                      << std::setw(15) << "Category" 
                      << "Status\n";
            std::cout << "----------------------------------------------------------------------\n";
            
            for (const auto& book : results) {
                std::cout << std::left << std::setw(10) << book->getId() 
                          << std::setw(30) << book->getTitle() 
                          << std::setw(20) << book->getAuthor() 
                          << std::setw(15) << book->getCategory() 
                          << (book->getIsIssued() ? "Issued" : "Available") << "\n";
            }
            std::cout << "----------------------------------------------------------------------\n";
        }
        
        waitForKeyPress();
    }
    
    void displayAllBooksUI() {
        displayHeader();
        std::cout << "ALL BOOKS\n\n";
        
        auto& books = library.getAllBooks();
        
        if (books.empty()) {
            std::cout << "No books in the library.\n";
        } else {
            std::cout << "----------------------------------------------------------------------\n";
            std::cout << std::left << std::setw(10) << "ID" 
                      << std::setw(30) << "Title" 
                      << std::setw(20) << "Author" 
                      << std::setw(15) << "Category" 
                      << "Status\n";
            std::cout << "----------------------------------------------------------------------\n";
            
            for (const auto& book : books) {
                std::cout << std::left << std::setw(10) << book.getId() 
                          << std::setw(30) << book.getTitle() 
                          << std::setw(20) << book.getAuthor() 
                          << std::setw(15) << book.getCategory() 
                          << (book.getIsIssued() ? "Issued" : "Available") << "\n";
            }
            std::cout << "----------------------------------------------------------------------\n";
        }
        
        waitForKeyPress();
    }
    
    void issueBookUI() {
        displayHeader();
        std::cout << "ISSUE BOOK\n\n";
        
        std::string id, student;
        
        std::cout << "Enter Book ID: ";
        std::cin >> id;
        
        Book* book = library.findBook(id);
        
        if (book == nullptr) {
            std::cout << "\nError: Book not found!\n";
            waitForKeyPress();
            return;
        }
        
        if (book->getIsIssued()) {
            std::cout << "\nError: Book is already issued to " << book->getIssuedTo() << "!\n";
            waitForKeyPress();
            return;
        }
        
        std::cout << "Enter Student Name: ";
        std::cin.ignore();
        std::getline(std::cin, student);
        
        book->issueBook(student);
        
        std::cout << "\nBook issued successfully!\n";
        std::cout << "Issue Date: " << book->getIssueDate().toString() << "\n";
        std::cout << "Return Date: " << book->getReturnDate().toString() << "\n";
        
        waitForKeyPress();
    }
    
    void returnBookUI() {
        displayHeader();
        std::cout << "RETURN BOOK\n\n";
        
        std::string id;
        
        std::cout << "Enter Book ID: ";
        std::cin >> id;
        
        Book* book = library.findBook(id);
        
        if (book == nullptr) {
            std::cout << "\nError: Book not found!\n";
            waitForKeyPress();
            return;
        }
        
        if (!book->getIsIssued()) {
            std::cout << "\nError: Book is not issued to anyone!\n";
            waitForKeyPress();
            return;
        }
        
        std::cout << "\nBook Details:\n";
        std::cout << "Title: " << book->getTitle() << "\n";
        std::cout << "Issued To: " << book->getIssuedTo() << "\n";
        std::cout << "Issue Date: " << book->getIssueDate().toString() << "\n";
        std::cout << "Return Date: " << book->getReturnDate().toString() << "\n";
        
        char confirm;
        std::cout << "\nConfirm return (Y/N): ";
        std::cin >> confirm;
        
        if (toupper(confirm) == 'Y') {
            book->returnBook();
            std::cout << "\nBook returned successfully!\n";
        } else {
            std::cout << "\nReturn cancelled.\n";
        }
        
        waitForKeyPress();
    }
    
public:
    void run() {
        int choice;
        
        // Add some sample books
        library.addBook(Book("B001", "The Great Gatsby", "F. Scott Fitzgerald", "Fiction"));
        library.addBook(Book("B002", "To Kill a Mockingbird", "Harper Lee", "Fiction"));
        library.addBook(Book("B003", "1984", "George Orwell", "Science Fiction"));
        library.addBook(Book("B004", "The Hobbit", "J.R.R. Tolkien", "Fantasy"));
        library.addBook(Book("B005", "Pride and Prejudice", "Jane Austen", "Romance"));
        
        while (true) {
            displayHeader();
            std::cout << "MAIN MENU\n\n";
            std::cout << "1. Add a New Book\n";
            std::cout << "2. Remove a Book\n";
            std::cout << "3. Search Books\n";
            std::cout << "4. Display All Books\n";
            std::cout << "5. Issue a Book\n";
            std::cout << "6. Return a Book\n";
            std::cout << "0. Exit\n\n";
            std::cout << "Enter your choice: ";
            
            std::cin >> choice;
            
            switch (choice) {
                case 1:
                    addBookUI();
                    break;
                case 2:
                    removeBookUI();
                    break;
                case 3:
                    searchBooksUI();
                    break;
                case 4:
                    displayAllBooksUI();
                    break;
                case 5:
                    issueBookUI();
                    break;
                case 6:
                    returnBookUI();
                    break;
                case 0:
                    displayHeader();
                    std::cout << "Thank you for using the Library Management System!\n";
                    return;
                default:
                    std::cout << "\nInvalid choice. Please try again.\n";
                    waitForKeyPress();
            }
        }
    }
};

int main() {
    LibraryUI ui;
    ui.run();
    return 0;
}