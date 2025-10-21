# CPP-Parking-Management-System
## Top-Down Designed Parking Management System in C++

This project is a console-based Parking Management System implemented in **C++** (using C++20 standard) and was developed as a software design exercise (**APF03-A2**) for an Advanced Programming course. [cite_start]The primary educational goal was to practice **Top-Down Design** principles and effectively manage large-scale program complexity by breaking the problem into smaller, manageable components[cite: 7, 8].

---

### Key Objectives

The main goals of this assignment were to demonstrate proficiency in:

* [cite_start]**Top-Down Design:** Breaking the complex parking problem into smaller sub-problems and distributing responsibilities among different parts of the program[cite: 7, 8].
* [cite_start]**Data Handling:** Reading and processing external data from three separate **CSV files** (Cars, Parking Spots, and Pricing) at startup[cite: 15].
* **Clean Code:** Ensuring clean, well-structured code with proper modularization.

### Features

The program simulates a real-world parking lot and handles the following commands:

* [cite_start]**`request_spot <Car's name>`:** Displays a list of suitable and vacant parking spots based on the car's size[cite: 55]. [cite_start]Each suggestion includes the spot's ID, type, static price, and price-per-hour[cite: 65].
* [cite_start]**`assign_spot <Spot's ID>`:** Reserves the specified parking spot for a car[cite: 68].
* **`checkout <Spot's ID>`:** Calculates the total fee and frees the spot. [cite_start]The total cost is calculated based on a static fee, a price-per-day rate, and potential surcharges for premium spots (Covered or CCTV)[cite: 83, 91, 95, 96].
* [cite_start]**`pass_time <Number of days>`:** Advances the system's internal clock by the specified number of days, which affects the total parking cost[cite: 115, 119].

### Prerequisites

* A C++20 compliant compiler (e.g., G++).
* A Linux environment (as required by the assignment).
