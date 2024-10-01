# SIH_FIREWALL


## Overview
This project implements a firewall that performs application context-aware blocking. It features a local running software component that applies firewall rules based on the contents of a `rules_json` file. The system includes a Node.js server for rule management and a React-based admin UI for updating firewall rules. The project uses a centralized management system with hash-based version control for updating firewall rules across all components.
(Path of rules_json: SIH_RULES_UPDATOR/cpp_filechecker/instruction_file/rules_json)
## Project Structure
### 1. WFP_LOCAL_SOFTWARE
- This folder contains the **Windows Filtering Platform (WFP)** driver code written in C++ using Visual Studio.
- The local software applies firewall rules from the `rules_json` file to block/allow application-level traffic based on context-aware policies.

### 2. SIH_RULES_UPDATER
- This folder includes two main components:
  - **C++ Hash-based Version Controller**: Ensures that the latest version of the `rules_json` file is always used by calculating the hash of the file and checking for changes.
  - **Node.js Server**: Hosts a REST API that allows the frontend to push and update rules in the database. The updated `rules_json` file is fetched by the local C++ software when changes are detected.

### 3. SIH_frontend
- A React-based admin UI for managing the firewall rules.
- The UI allows administrators to create, update, and delete rules, which are sent to the Node.js server for storage in the database and eventual deployment to the local firewall software.


## How it Works

1. **Frontend (SIH_frontend)**: Administrators interact with the React UI to manage firewall rules. The UI communicates with the Node.js server via HTTP requests (PUT, POST) to update the `rules_json` file stored in the MongoDB database.
   
2. **Node.js Server (SIH_RULES_UPDATER)**: This server processes rule updates from the frontend and stores them in the database. It also provides a JSON file for the local software to fetch and apply updates.

3. **Hash-based Version Control (SIH_RULES_UPDATER)**: A C++ application calculates the hash of the local `rules_json` file and compares it with the server version. If the hash differs, the new rules file is fetched and applied locally.

4. **Local Firewall Software (WFP_LOCAL_SOFTWARE)**: The WFP driver applies the rules from the `rules_json` file to enforce context-aware blocking on applications.



