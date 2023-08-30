#include "MyCoolDB/MyCoolDB.h"

int main() {
    MyCoolDB a;
    a.load();
    /*table t = a.request(R"(CREATE TABLE Persons (
    PersonID INT PRIMARY KEY NOT NULL,
    LastName VARCHAR,
    FirstName VARCHAR,
    Address VARCHAR,
    City VARCHAR
    );)");
    t = a.request(R"(INSERT INTO Persons (PersonID, FirstName, LastName, City, Address)
    VALUES (5, 'Tom B.', 'Eriksen', 'Moscow', 'Norway');)");
    t = a.request(R"(UPDATE Customers
    SET ContactName = 'Alfred Schmidt', City= 'Frankfurt'
    WHERE CustomerID = 1;)");
    std::cout << t.getNotes();*/
    return 0;
}
