# Unreal Engine C++ Gameplay Systems & Data Integration

This repository contains C++ source code examples from my Unreal Engine project. It demonstrates event-driven gameplay mechanics, optimized logic, and database integration.

**Gameplay Demo Video:** [https://youtu.be/tpZdvFR5vSc](https://youtu.be/tpZdvFR5vSc)

## 1. Dynamic Day-Night Cycle & Event System
A highly optimized, time-scalable day-night cycle system controlled entirely via C++.

* **Custom Time Scaling:** The `DayLength` variable (exposed to the Editor and must be entered a value in the Editor) dictates the real-world minutes it takes to complete a full 24-hour in-game cycle.
* **Optimized Execution:** Instead of running complex calculations every single frame, the system executes exactly 720 ticks over the defined `DayLength`. Each tick rotates the Directional Light by 0.5 degrees and advances the in-game clock by 2 minutes, ensuring a smooth visual transition with minimal performance cost.
* **Event-Driven Architecture (Delegates):** To prevent other actors from constantly polling the current time, this class utilizes Unreal's Delegate system. It broadcasts events at specific in-game milestones (`OnMorning`, `OnNoon`, `OnEvening`, `OnNight`). Other classes simply bind to these delegates to trigger their own logic, ensuring a decoupled and performant architecture.

* ##Usage:
1. **Add to Source:** Copy `DayManager.h` and `DayManager.cpp` into your Unreal Engine project's `Source/[YourProjectName]/` folder.
2. **After recompiling the project open the Unreal Editor.
3. **Drag and drop the `DayManager` actor to the level viewport.
4. **In details panel select the `DirectionalLight` actor which is sun. And in details panel enter a desired value for `DayLength` variable to adjust length of the day in game. (for exapmle if you enter 5 here. Day length in the game will be 5 minutes in real time.)



## 2. SQLite Database Integration for UI Word Game
A backend data management system designed for a menu-based word game.

* **Database Management:** Built a custom C++ class utilizing an SQLite plugin to handle data persistence, dynamic queries, and word retrieval. This completely eliminates the need for hardcoded strings.
* **UI/Backend Separation:** The C++ backend securely handles all database interactions and logic, seamlessly passing the required data to the User Interface (built with UMG / User Widgets). This showcases a clean separation of concerns between backend data handling and frontend presentation.

* ### 📦 Database Management (SQLite)
* **CRUD Operations:** Seamlessly Read, Add, Update, and Delete words in your tables.
* **Schema Management:** Create, Rename, or Drop tables safely at runtime.
* **Defensive Programming:** Built-in checks to prevent duplicate entries and SQL injection (using Prepared Statements).
* **Identifier Safety:** Handles table names with special characters and safely validates alphanumeric naming conventions.

### 🇹🇷 Advanced String Handling & "Turkish I" Fix
* **Turkish-Aware Lowercase:** Completely solves the notorious "Turkish I" problem (`İ` -> `i`, `I` -> `ı`). It manually sanitizes `Ç, Ş, Ğ, Ü, Ö` ensuring your string comparisons are 100% accurate regardless of the user's OS language settings.
* **Case-Insensitive Equality:** Compare non-ASCII strings without dealing with localized collation errors.
* **String Parsing:** Split strings easily by delimiters (e.g., separating "Apple = Elma").

### 🎲 Array & Gameplay Utilities
* **Non-Destructive Randomization:** Generates shuffled index arrays (Fisher-Yates algorithm) to pull random data without modifying your original arrays.
* **Multiple-Choice Generator:** Input a correct answer's index and pool size, and get a perfectly shuffled 4-option array (1 correct, 3 unique random distractors) for quiz games.

* ## 🛠️ Installation & Setup

1. **Add to Source:** Copy `MySQLLibrary.h` and `MySQLLibrary.cpp` into your Unreal Engine project's `Source/[YourProjectName]/` folder.
2. **Update Build.cs:** To use Unreal's built-in SQLite features, you must include the SQLite modules in your project's `.Build.cs` file. Open `[YourProjectName].Build.cs` and add `"SQLiteCore"` and `"SQLiteSupport"` to your public dependencies:
   ```csharp
   PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "SQLiteCore", "SQLiteSupport" });
3. **Then you can use this Library functions in your blueprint codes or in your C++ codes.
