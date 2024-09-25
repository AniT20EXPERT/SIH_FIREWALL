import React, { useContext } from 'react';
import '../Style/Sidebar.css';
import { AuthContext } from '../AuthContext'; 
import logo from '../assets/logo.png'; 


const Sidebar = () => {
  const { logout } = useContext(AuthContext); 

  const handleLogout = () => {
    logout(); // Call the logout function to log the user out
  };

  return (
    <div className="sidebar">
      <div className="logo-container">
        <img src={logo} alt="Logo" className="logo" />
      </div>
      <button className="sidebar-btn">Monitor</button>
      <button className="sidebar-btn" onClick={handleLogout}>Logout</button>
    </div>
  );
};

export default Sidebar;
