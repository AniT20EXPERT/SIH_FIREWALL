import React, { useState, useEffect } from 'react';
import '../Style/MonitoringUser.css';

const MonitoringUser = () => {
  const [userId, setUserId] = useState('userABC');  // Dummy data 
  // Fetch userId from the database here (e.g., via API call)

  return (
    <div className="monitoring-box">
      <p>Currently Monitoring User ID:</p>
      <h2>{userId}</h2>
    </div>
  );
};

export default MonitoringUser;
