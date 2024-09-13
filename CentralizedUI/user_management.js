// Dummy Data
const userData = [
    { name: "Workstation-01", status: "green", policyGroup: "xyz grp 1", alerts: 2, groups: "Group A" },
    { name: "Workstation-02", status: "red", policyGroup: "xyz grp 2", alerts: 0, groups: "Group B" },
    { name: "Workstation-03", status: "green", policyGroup: "xyz grp 3", alerts: 1, groups: "Group C" }
];

// Function to dynamically populate the table
function populateUserTable(data) {
    const tableBody = document.querySelector("#userTable tbody");
    tableBody.innerHTML = ""; // Clear any existing rows

    data.forEach(user => {
        const row = document.createElement("tr");

        row.innerHTML = `
            <td>${user.name}</td>
            <td><span style="color: ${user.status === "green" ? "green" : "red"};">●</span></td>
            <td>${user.policyGroup}</td>
            <td>${user.alerts}</td>
            <td>${user.groups}</td>
            <td>
                <button class="view-details-btn">View Details</button>
                <button class="apply-policy-btn">Apply Policy</button>
            </td>
        `;
        
        tableBody.appendChild(row);
    });
}

// Initialize table with dummy data
populateUserTable(userData);

// Logout button functionality
document.getElementById("logoutButton").addEventListener("click", function() {
    window.location.href = "index.html";
});
