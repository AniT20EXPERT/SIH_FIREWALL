import React, { useContext, useState, useEffect } from 'react';
import Sidebar from './Components/Sidebar';
import MonitoringUser from './Components/MonitoringUser';
import BlacklistForm from './Components/BlacklistForm';
import BlacklistTable from './Components/BlacklistTable';
import './Style/App.css';
import { AuthProvider, AuthContext } from './AuthContext';
import Login from './Components/Login'; 

const App = () => {
  return (
    <AuthProvider>
      <MainAppContent />
    </AuthProvider>
  );
};

const MainAppContent = () => {
  const { isAuthenticated } = useContext(AuthContext);
  const [blacklistData, setBlacklistData] = useState([]);

  // Load data from local storage when the component mounts
  useEffect(() => {
    const storedData = JSON.parse(localStorage.getItem('blacklist')) || [];
    setBlacklistData(storedData);
  }, []);

  // Save data to local storage when blacklistData changes
  useEffect(() => {
    if (blacklistData.length > 0) {
      localStorage.setItem('blacklist', JSON.stringify(blacklistData));
    } else {
      localStorage.removeItem('blacklist');  // Remove the key if there's no data
    }
  }, [blacklistData]);

  const handleBlacklistSubmit = (data) => {
    const updatedData = [...blacklistData, data];
    setBlacklistData(updatedData);  // Update state
    localStorage.setItem('blacklist', JSON.stringify(updatedData));  // Save to local storage
  };

  const handleDeleteEntry = (indexToDelete) => {
    const updatedData = blacklistData.filter((_, index) => index !== indexToDelete);
    setBlacklistData(updatedData);  // Update state
    localStorage.setItem('blacklist', JSON.stringify(updatedData));  // Save updated data to local storage
  };

  return (
    <div className="app-container">
      {isAuthenticated ? (
        <>
          <Sidebar />
          <div className="main-content">
            <MonitoringUser />
            <BlacklistForm onSubmit={handleBlacklistSubmit} />
            <BlacklistTable blacklistData={blacklistData} onDeleteEntry={handleDeleteEntry} />
          </div>
        </>
      ) : (
        <Login />
      )}
    </div>
  );
};

export default App;
