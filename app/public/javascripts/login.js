// Function to handle the login form submission
function handleLogin() {
    // Get the values from the input fields
    var username = document.getElementById("username").value;
    var password = document.getElementById("password").value;

    // Perform validation on the input values
    if (username === "" || password === "") {
        alert("Please enter both username and password");
        return;
    }

    // Make an API call to authenticate the user
    // Replace the API endpoint with your actual authentication endpoint
    fetch("/api/login", {
        method: "POST",
        headers: {
            "Content-Type": "application/json",
        },
        body: JSON.stringify({
            username: username,
            password: password,
        }),
    })
        .then(function (response) {
            if (response.ok) {
                // User is authenticated, redirect to the dashboard page
                window.location.href = "/dashboard.html";
            } else {
                // Authentication failed, display error message
                alert("Invalid username or password");
            }
        })
        .catch(function (error) {
            console.error("Error:", error);
        });
}

// Attach event listener to the login form submit button
document.getElementById("loginForm").addEventListener("submit", function (event) {
    event.preventDefault(); // Prevent form submission
    handleLogin(); // Call the handleLogin function
});
