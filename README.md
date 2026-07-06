<h1 align="center">📚 Magic Library</h1>

<h3 align="center">Mini Projeto 03 — Game Backend in C</h3>

<p align="center">
  <em>Structs, Pointers, Dynamic Allocation and File Handling</em>
</p>

<p align="center">
  <img src="https://img.shields.io/badge/language-C-blue">
  <img src="https://img.shields.io/badge/status-finished-brightgreen">
  <img src="https://img.shields.io/badge/build-GCC-orange">
  <img src="https://img.shields.io/badge/platform-Linux%20%7C%20Windows-lightblue">
</p>

---

## 📌 About the Project

**Magic Library** is a terminal-based backend system for managing magical books in a fictional RPG inventory.

The project was developed in C and uses:

- `struct`
- pointers
- dynamic memory allocation with `malloc` and `free`
- array of pointers
- file handling
- string encryption

The system stores books in memory while the program is running and saves them to a file when the user exits.

---

## ✨ Features

| # | Feature | Description |
|---|---|---|
| 1 | Register book | Creates a new magical book |
| 2 | Delete book | Removes a book by its ID |
| 3 | Display book | Shows all data from a specific book |
| 4 | Update book | Allows selective editing of book fields |
| 5 | List book titles | Lists all registered book titles |
| 6 | Save and exit | Saves the library to a file and exits |

---

## 🧙 RPG Attributes

Each book may optionally provide RPG attributes.

Available attributes:

| Code | Attribute |
|---|---|
| FOR | Strength |
| DES | Dexterity |
| CON | Constitution |
| INT | Intelligence |
| SAB | Wisdom |
| CAR | Charisma |
| MAG | Magic |

Each attribute can have a value from `1` to `20`.

A book does not need to have all attributes. For example, one book may provide only `MAG`, while another may provide `FOR` and `CON`.

---

## 🧱 Data Structures

### Date

Stores day, month and year.

```c
typedef struct
{
    int day;
    int month;
    int year;
} Date;
