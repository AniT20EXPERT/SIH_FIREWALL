# SIH_FIREWALL


## Overview
This project implements a firewall that performs application context-aware blocking. It features a local running software component that applies firewall rules based on the contents of a `rule_file.json` file. The system includes a Node.js server for rule management and a React-based admin UI for updating firewall rules. The project uses a centralized management system with hash-based version control for updating firewall rules across all components.
(Path of rules_json: SIH_RULES_UPDATOR/cpp_filechecker/instruction_file/rule_file.json)
## Project Structure
### 1. WFP_LOCAL_SOFTWARE
- This folder contains the **Windows Filtering Platform (WFP)** driver code written in C++ using Visual Studio.
- The local software applies firewall rules from the `rule_file.json` file to block/allow application-level traffic based on context-aware policies.

### 2. SIH_RULES_UPDATER
- This folder includes two main components:
  - **C++ Hash-based Version Controller**: Ensures that the latest version of the `rule_file.json` file is always used by calculating the hash of the file and checking for changes.
  - **Node.js Server**: Hosts a REST API that allows the frontend to push and update rules in the database. The updated `rules_json` file is fetched by the local C++ software when changes are detected.

### 3. SIH_frontend
- A React-based admin UI for managing the firewall rules.
- The UI allows administrators to create, update, and delete rules, which are sent to the Node.js server for storage in the database and eventual deployment to the local firewall software.


## How it Works

1. **Frontend (SIH_frontend)**: Administrators interact with the React UI to manage firewall rules. The UI communicates with the Node.js server via HTTP requests (PUT, POST) to update the `rule_file.json` file stored in the MongoDB database.
   
2. **Node.js Server (SIH_RULES_UPDATER)**: This server processes rule updates from the frontend and stores them in the database. It also provides a JSON file for the local software to fetch and apply updates.

3. **Hash-based Version Control (SIH_RULES_UPDATER)**: A C++ application calculates the hash of the local `rule_file.json` file and compares it with the server version. If the hash differs, the new rules file is fetched and applied locally.

4. **Local Firewall Software (WFP_LOCAL_SOFTWARE)**: The WFP driver applies the rules from the `rule_file.json` file to enforce context-aware blocking on applications.

## Security of the `rule_file.json` File

Ensuring the integrity and confidentiality of the `rule_file.json` file is critical to the security of the firewall. In future iterations of this project, we plan to implement the following measures to ensure that the `rule_file.json` file is accessed and modified only by the **SIH_RULES_UPDATER**:

1. **File Permissions and Access Control**: 
   The `rule_file.json` file will have restricted file permissions, allowing only the local **SIH_RULES_UPDATER** process to read or write to the file. Unauthorized processes will be denied access to prevent tampering or accidental modification.

2. **Authentication and Authorization**:
   The **SIH_RULES_UPDATER** will be authenticated using process-level access control, ensuring only the trusted application can make changes. A security token or certificate-based authentication will be implemented to verify that the requestor is the legitimate SIH_RULES_UPDATER process.

3. **Encryption at Rest**:
   The `rules_json` file will be encrypted while stored locally. This ensures that even if the file is accessed by an unauthorized party, the contents will remain unreadable without the correct decryption key, which will only be available to **SIH_RULES_UPDATER**.

4. **Hash Integrity Checks**:
   In addition to controlling access, hash-based integrity checks will be used to detect any unauthorized modifications to the `rules_json` file. Any changes that have not been initiated by **SIH_RULES_UPDATER** will trigger an alert, and the original rules will be restored from the server.


