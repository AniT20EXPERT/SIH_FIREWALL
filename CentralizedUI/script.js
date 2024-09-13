function validateLogin() {
    const username = document.getElementById("username").value;
    const password = document.getElementById("password").value;
    const errorMessage = document.getElementById("error-message");

    // Dummy check for username and password
    if (username === "user" && password === "password") {
        // Redirect to user management page
        window.location.href = "user_management.html";
        return false; // Prevent form submission
    } else {
        errorMessage.textContent = "Incorrect username or password. Please try again.";
        return false; // Prevent form submission
    }
}
