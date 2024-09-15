document.addEventListener("DOMContentLoaded", function () {
    const userTable = document.querySelector("#userTable tbody");

    // Dummy data, this should come from your API
    const users = [
        {
            name: "Workstation-01",
            status: "Active",
            policyGroup: "xyz grp 1",
            alerts: 2,
            group: "Admin",
            ipAddress: "192.168.1.10",
            operatingSystem: "Windows 10",
            location: "Building A - 2nd Floor",
        },
        {
            name: "Workstation-02",
            status: "Inactive",
            policyGroup: "xyz grp 2",
            alerts: 0,
            group: "User",
            ipAddress: "192.168.1.15",
            operatingSystem: "Ubuntu 20.04",
            location: "Building B - 1st Floor",
        },
        {
            name: "Workstation-03",
            status: "Active",
            policyGroup: "xyz grp 3",
            alerts: 1,
            group: "IT",
            ipAddress: "192.168.1.25",
            operatingSystem: "macOS Monterey",
            location: "Building C - 3rd Floor",
        },
        
    ];

    // Render table rows
    users.forEach((user, index) => {
        const row = document.createElement("tr");
        row.innerHTML = `
            <td>${user.name}</td>
            <td>${user.status === "Active" ? "<span style='color:green'>●</span>" : "<span style='color:red'>●</span>"}</td>
            <td>${user.policyGroup}</td>
            <td>${user.alerts}</td>
            <td>${user.group}</td>
            <td>
                <button class="view-details-btn" data-index="${index}">View Details</button>
                <button class="apply-policy-btn" data-index="${index}">Apply Policy</button>
            </td>
        `;
        userTable.appendChild(row);
    });

    // Modal elements for "View Details"
    const modal = document.getElementById("detailsModal");
    const closeBtn = document.querySelector(".close-btn");

    const wsName = document.getElementById("ws-name");
    const wsStatus = document.getElementById("ws-status");
    const wsPolicy = document.getElementById("ws-policy");
    const wsAlerts = document.getElementById("ws-alerts");
    const wsGroup = document.getElementById("ws-group");

    // Modal elements for "Apply Policy"
    const applyPolicyModal = document.getElementById("applyPolicyModal");
    const closeBtnApply = document.querySelector(".close-btn-apply");

    const applyWsName = document.getElementById("apply-ws-name");
    const policyGroupIdInput = document.getElementById("policyGroupId");
    const applyPolicyForm = document.getElementById("applyPolicyForm");

    // Function to open "View Details" modal
    function openModal(user) {
        wsName.textContent = user.name;
        wsStatus.textContent = user.status;
        wsPolicy.textContent = user.policyGroup;
        wsAlerts.textContent = user.alerts;
        wsGroup.textContent = user.group;

        modal.style.display = "block";
    }

    // Function to open "Apply Policy" modal
    function openApplyPolicyModal(user) {
        applyWsName.textContent = user.name;
        policyGroupIdInput.value = ""; // Clear previous input
        applyPolicyModal.style.display = "block";
    }

    // Function to close the modal
    closeBtn.onclick = function () {
        modal.style.display = "none";
    };

    // Function to close the "Apply Policy" modal
    closeBtnApply.onclick = function () {
        applyPolicyModal.style.display = "none";
    };

    // Close modal when clicking outside the modal
    window.onclick = function (event) {
        if (event.target === modal) {
            modal.style.display = "none";
        }
        if (event.target === applyPolicyModal) {
            applyPolicyModal.style.display = "none";
        }
    };

    // Attach event listeners to "View Details" buttons
    document.querySelectorAll(".view-details-btn").forEach(button => {
        button.addEventListener("click", function () {
            const userIndex = this.getAttribute("data-index");
            openModal(users[userIndex]);
        });
    });

    // Attach event listeners to "Apply Policy" buttons
    document.querySelectorAll(".apply-policy-btn").forEach(button => {
        button.addEventListener("click", function () {
            const userIndex = this.getAttribute("data-index");
            openApplyPolicyModal(users[userIndex]);
        });
    });

    // Handle form submission for "Apply Policy"
    applyPolicyForm.addEventListener("submit", function (event) {
        event.preventDefault(); // Prevent form from submitting

        // Get the policy group ID
        const policyGroupId = policyGroupIdInput.value;

        // You can send this data to your API here (e.g., with fetch or Axios)
        console.log("Submitting policy group ID:", policyGroupId);

        // Close the modal after submission
        applyPolicyModal.style.display = "none";
    });
});







document.getElementById('logoutButton').addEventListener('click', function() {
    // Handle the logout logic here
    window.location.href = 'index.html'; // Redirect to login page
});
