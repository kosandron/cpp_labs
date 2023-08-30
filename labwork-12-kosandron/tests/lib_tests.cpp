#include "MyCoolDB/MyCoolDB.h"

#include <gtest/gtest.h>

TEST(Methods, load) {
    MyCoolDB db;
    //db.save();
    db.load("cash.txt");
    ASSERT_TRUE(true);
}

TEST(Methods, create) {
    MyCoolDB db;
    table t = db.request(R"(CREATE TABLE Persons (
    PersonID INT PRIMARY KEY NOT NULL,
    LastName VARCHAR,
    FirstName VARCHAR,
    Address VARCHAR,
    City VARCHAR
    );)");
    openTable test = t.getData();
    ASSERT_EQ(test.types[test.index("PersonID")], Column::DataType::Int);
    ASSERT_EQ(test.types[test.index("City")], Column::DataType::Varchar);
    ASSERT_EQ(test.primary_key[test.index("PersonID")], true);
    ASSERT_EQ(test.foreign_key[test.index("PersonID")], false);
    ASSERT_EQ(test.is_null[test.index("PersonID")], true);
}

TEST(Methods, drop) {
    MyCoolDB db;
    table t = db.request(R"(CREATE TABLE Persons (
    PersonID INT PRIMARY KEY NOT NULL,
    LastName VARCHAR,
    FirstName VARCHAR,
    Address VARCHAR,
    City VARCHAR
    );)");
    bool flag = false;
    try {
        //t = db.request(R"(DROP TABLE Persons;)");
        t = db.request(R"(CREATE TABLE Persons (
    PersonID INT PRIMARY KEY NOT NULL,
    LastName VARCHAR,
    FirstName VARCHAR,
    Address VARCHAR,
    City VARCHAR
    );)");
    } catch (DBException& t) {
        std::cout << t.what();
        flag = true;
    }
    ASSERT_TRUE(flag);
    t = db.request(R"(DROP TABLE Persons;)");
    t = db.request(R"(CREATE TABLE Persons (
    PersonID INT PRIMARY KEY NOT NULL,
    LastName VARCHAR,
    FirstName VARCHAR,
    Address VARCHAR,
    City VARCHAR
    );)");
    openTable test = t.getData();
    ASSERT_EQ(test.types[test.index("PersonID")], Column::DataType::Int);
    ASSERT_EQ(test.types[test.index("City")], Column::DataType::Varchar);
    ASSERT_EQ(test.primary_key[test.index("PersonID")], true);
    ASSERT_EQ(test.foreign_key[test.index("PersonID")], false);
    ASSERT_EQ(test.is_null[test.index("PersonID")], true);
}

TEST(Methods, insert) {
    MyCoolDB db;
    table t = db.request(R"(CREATE TABLE Persons (
    PersonID INT PRIMARY KEY NOT NULL,
    LastName VARCHAR,
    FirstName VARCHAR,
    Address VARCHAR,
    City VARCHAR
    );)");
    t = db.request(R"(INSERT INTO Persons (PersonID, FirstName, LastName, Address, City)
VALUES (5, 'tree', 'Lunn', '4006', 'Moscow');)");
    t = db.request(R"(INSERT INTO Persons (PersonID, FirstName, LastName, Address, City)
VALUES (9, 'treyte', 'Lutrnn', '40067', 'Omsk');)");
    try {
        t = db.request(R"(INSERT INTO Persons (PersonID, FirstName, LastName, Address, City)
VALUES (9, 'treyte', 'Lutrnn', '40067', 'Omsk');)");
    } catch (DBException& ex) {
        std::cout << ex.what() << '\n';
    }

    openTable test = t.getData();
    ASSERT_EQ(test.number_of_notes, 2);
    ASSERT_EQ(test.columns[test.index("PersonID")][0], "5");
    ASSERT_EQ(test.columns[test.index("PersonID")][1], "9");
    ASSERT_EQ(test.columns[test.index("City")][0], "Moscow");
    ASSERT_EQ(test.columns[test.index("City")][1], "Omsk");
}

TEST(Methods, deleteTest) {
    MyCoolDB db;
    table t = db.request(R"(CREATE TABLE Persons (
    PersonID INT PRIMARY KEY NOT NULL,
    LastName VARCHAR,
    FirstName VARCHAR,
    Address VARCHAR,
    City VARCHAR
    );)");
    t = db.request(R"(INSERT INTO Persons (PersonID, FirstName, LastName, Address, City)
VALUES (5, 'tree', 'Lunn', '4006', 'Moscow');)");
    t = db.request(R"(INSERT INTO Persons (PersonID, FirstName, LastName, Address, City)
VALUES (9, 'treyte', 'Lutrnn', '40067', 'Omsk');)");
    t = db.request(R"(INSERT INTO Persons (PersonID, FirstName, LastName, Address, City)
VALUES (2, 'Lana', 'Petrovna', '40037', 'Belgorod');)");
    t = db.request(R"(DELETE FROM Persons WHERE City=Moscow;)");

    openTable test = t.getData();
    ASSERT_EQ(test.number_of_notes, 2);
    ASSERT_EQ(test.columns[test.index("PersonID")][1], "2");
    ASSERT_EQ(test.columns[test.index("PersonID")][0], "9");
    ASSERT_EQ(test.columns[test.index("City")][1], "Belgorod");
    ASSERT_EQ(test.columns[test.index("City")][0], "Omsk");
}

TEST(Methods, InnerJoin) {
    MyCoolDB db;
    table t = db.request(R"(CREATE TABLE Persons (
    PersonID INT PRIMARY KEY NOT NULL,
    LastName VARCHAR,
    FirstName VARCHAR,
    Address VARCHAR,
    City VARCHAR
    );)");
    t = db.request(R"(INSERT INTO Persons (PersonID, FirstName, LastName, Address, City)
VALUES (5, 'tree', 'Lunn', '4006', 'Moscow');)");
    t = db.request(R"(INSERT INTO Persons (PersonID, FirstName, LastName, Address, City)
VALUES (9, 'treyte', 'Lutrnn', '40067', 'Omsk');)");
    t = db.request(R"(INSERT INTO Persons (PersonID, FirstName, LastName, Address, City)
VALUES (3, 'ger', 'Finn', '12234', 'Piter');)");
    t = db.request(R"(CREATE TABLE Prof (
    PersonID INT PRIMARY KEY NOT NULL,
    Profession VARCHAR,
    Workhours INT
    );)");
    t = db.request(R"(INSERT INTO Prof (PersonID, Profession, Workhours)
VALUES (3, 'programmer', 40);)");
    t = db.request(R"(INSERT INTO Prof (PersonID, Profession, Workhours)
VALUES (7, 'driver', 60);)");
    t = db.request(R"(INSERT INTO Prof (PersonID, Profession, Workhours)
VALUES (5, 'teacher', 20);)");
    t = db.request(R"(SELECT Persons.PersonID, Prof.Profession
FROM Persons
INNER JOIN Prof
ON Persons.PersonID=Prof.PersonID;)");

    openTable test = t.getData();
    ASSERT_EQ(test.number_of_notes, 2);
    ASSERT_EQ(test.columns[0][0], "5");
    ASSERT_EQ(test.columns[0][1], "3");
    ASSERT_EQ(test.columns[1][0], "teacher");
    ASSERT_EQ(test.columns[1][1], "programmer");
}

TEST(Methods, LeftJoin) {
    MyCoolDB db;
    table t = db.request(R"(CREATE TABLE Persons (
    PersonID INT PRIMARY KEY NOT NULL,
    LastName VARCHAR,
    FirstName VARCHAR,
    Address VARCHAR,
    City VARCHAR
    );)");
    t = db.request(R"(INSERT INTO Persons (PersonID, FirstName, LastName, Address, City)
VALUES (5, 'tree', 'Lunn', '4006', 'Moscow');)");
    t = db.request(R"(INSERT INTO Persons (PersonID, FirstName, LastName, Address, City)
VALUES (9, 'treyte', 'Lutrnn', '40067', 'Omsk');)");
    t = db.request(R"(INSERT INTO Persons (PersonID, FirstName, LastName, Address, City)
VALUES (3, 'ger', 'Finn', '12234', 'Piter');)");
    t = db.request(R"(CREATE TABLE Prof (
    PersonID INT PRIMARY KEY NOT NULL,
    Profession VARCHAR,
    Workhours INT
    );)");
    t = db.request(R"(INSERT INTO Prof (PersonID, Profession, Workhours)
VALUES (3, 'programmer', 40);)");
    t = db.request(R"(INSERT INTO Prof (PersonID, Profession, Workhours)
VALUES (7, 'driver', 60);)");
    t = db.request(R"(INSERT INTO Prof (PersonID, Profession, Workhours)
VALUES (5, 'teacher', 20);)");
    t = db.request(R"(SELECT Persons.PersonID, Prof.Profession
FROM Persons
LEFT JOIN Prof
ON Persons.PersonID=Prof.PersonID;)");

    openTable test = t.getData();
    ASSERT_EQ(test.number_of_notes, 3);
    ASSERT_EQ(test.columns[0][0], "5");
    ASSERT_EQ(test.columns[0][1], "9");
    ASSERT_EQ(test.columns[0][2], "3");
    ASSERT_EQ(test.columns[1][0], "teacher");
    ASSERT_EQ(test.columns[1][1], "NULL");
    ASSERT_EQ(test.columns[1][2], "programmer");
}

TEST(Methods, RightJoin) {
    MyCoolDB db;
    table t = db.request(R"(CREATE TABLE Persons (
    PersonID INT PRIMARY KEY NOT NULL,
    LastName VARCHAR,
    FirstName VARCHAR,
    Address VARCHAR,
    City VARCHAR
    );)");
    t = db.request(R"(INSERT INTO Persons (PersonID, FirstName, LastName, Address, City)
VALUES (5, 'tree', 'Lunn', '4006', 'Moscow');)");
    t = db.request(R"(INSERT INTO Persons (PersonID, FirstName, LastName, Address, City)
VALUES (9, 'treyte', 'Lutrnn', '40067', 'Omsk');)");
    t = db.request(R"(INSERT INTO Persons (PersonID, FirstName, LastName, Address, City)
VALUES (3, 'ger', 'Finn', '12234', 'Piter');)");
    t = db.request(R"(CREATE TABLE Prof (
    PersonID INT PRIMARY KEY NOT NULL,
    Profession VARCHAR,
    Workhours INT
    );)");
    t = db.request(R"(INSERT INTO Prof (PersonID, Profession, Workhours)
VALUES (3, 'programmer', 40);)");
    t = db.request(R"(INSERT INTO Prof (PersonID, Profession, Workhours)
VALUES (7, 'driver', 60);)");
    t = db.request(R"(INSERT INTO Prof (PersonID, Profession, Workhours)
VALUES (5, 'teacher', 20);)");
    t = db.request(R"(SELECT Persons.PersonID, Prof.Profession, Persons.FirstName
FROM Persons
RIGHT JOIN Prof
ON Persons.PersonID=Prof.PersonID;)");
    openTable test = t.getData();
    std::cout << "\nResult: \n";
    for (size_t i = 0; i < test.names.size(); i++) {
        std::cout << test.names[i] << " ";
    }
    std::cout << '\n';
    for (int j = 0; j < test.number_of_notes; j++) {
        for (int i = 0; i < test.columns.size(); i++) {
            std::cout << test.columns[i][j] << " ";
        }
        std::cout << '\n';
    }

    ASSERT_EQ(test.number_of_notes, 3);
    ASSERT_EQ(test.columns[0][0], "ger");
    ASSERT_EQ(test.columns[0][1], "NULL");
    ASSERT_EQ(test.columns[0][2], "tree");
    ASSERT_EQ(test.columns[1][0], "3");
    ASSERT_EQ(test.columns[1][1], "NULL");
    ASSERT_EQ(test.columns[1][2], "5");
    ASSERT_EQ(test.columns[2][2], "teacher");
    ASSERT_EQ(test.columns[2][1], "driver");
    ASSERT_EQ(test.columns[2][0], "programmer");
}

TEST(Methods, Update) {
    MyCoolDB db;
    table t = db.request(R"(CREATE TABLE Persons (
    PersonID INT PRIMARY KEY NOT NULL,
    LastName VARCHAR,
    FirstName VARCHAR,
    Address VARCHAR,
    City VARCHAR
    );)");
    t = db.request(R"(INSERT INTO Persons (PersonID, FirstName, LastName, Address, City)
VALUES (5, 'tree', 'Lunn', '4006', 'Moscow');)");
    t = db.request(R"(INSERT INTO Persons (PersonID, FirstName, LastName, Address, City)
VALUES (9, 'treyte', 'Lutrnn', '40067', 'Omsk');)");
    t = db.request(R"(INSERT INTO Persons (PersonID, FirstName, LastName, Address, City)
VALUES (3, 'ger', 'Finn', '12234', 'Piter');)");
    t = db.request(R"(UPDATE Persons
SET LastName = 'Alfred', City= 'Frankfurt'
WHERE NOT 9 < PersonID AND NOT FirstName = "tree";)");

    openTable test = t.getData();
    std::set<std::string> primary_keys;
    for(size_t i = 0; i < test.number_of_notes; i++) {
        primary_keys.insert(test.columns[test.index("City")][i]);
    }
    std::set<std::string> check = {"Frankfurt", "Moscow"};
    std::set<std::string>::iterator tmp = check.begin();
    for(auto& u : primary_keys) {
        ASSERT_EQ(*tmp, u);
        tmp++;
    }
    std::set<std::string> last_name_keys;
    for(size_t i = 0; i < test.number_of_notes; i++) {
        last_name_keys.insert(test.columns[test.index("LastName")][i]);
    }
    std::set<std::string> check1 = {"Alfred", "Lunn"};
    std::set<std::string>::iterator tmp2 = check1.begin();
    for(auto& u : last_name_keys) {
        ASSERT_EQ(*tmp2, u);
        tmp2++;
    }
}

//DONE
TEST(Methods, Select) {
    MyCoolDB db;
    table t = db.request(R"(CREATE TABLE Persons (
    PersonID INT PRIMARY KEY NOT NULL,
    LastName VARCHAR,
    FirstName VARCHAR,
    Address VARCHAR,
    City VARCHAR
    );)");
    t = db.request(R"(INSERT INTO Persons (PersonID, FirstName, LastName, Address, City)
VALUES (5, 'tree', 'Lunn', '4006', 'Moscow');)");
    t = db.request(R"(INSERT INTO Persons (PersonID, FirstName, LastName, Address, City)
VALUES (9, 'treyte', 'Lutrnn', '40067', 'Omsk');)");
    t = db.request(R"(INSERT INTO Persons (PersonID, FirstName, LastName, Address, City)
VALUES (3, 'ger', 'Finn', '12234', 'Piter');)");
    t = db.request(R"(SELECT * FROM Persons
WHERE NOT 9 < PersonID AND NOT FirstName = "tree";)");
    openTable test = t.getData();
    ASSERT_EQ(test.number_of_notes, 2);
    ASSERT_EQ(test.columns.size(), 5);
    std::set<std::string> primary_keys;
    for(size_t i = 0; i < test.number_of_notes; i++) {
        primary_keys.insert(test.columns[test.index("PersonID")][i]);
    }
    std::set<std::string> check = {"3", "9"};
    std::set<std::string>::iterator tmp = check.begin();
    for(auto& u : primary_keys) {
        ASSERT_EQ(*tmp, u);
        tmp++;
    }
}
