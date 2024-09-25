import React, { useState } from 'react';
import { FontAwesomeIcon } from '@fortawesome/react-fontawesome';
import { faTrashAlt } from '@fortawesome/free-solid-svg-icons';
import '../Style/BlacklistForm.css';

const BlacklistForm = ({ onSubmit }) => {
  const [appName, setAppName] = useState('');
  const [location, setLocation] = useState('');
  const [url, setUrl] = useState('');
  const [urlList, setUrlList] = useState([]);

  const handleAddUrl = () => {
    if (url) {
      setUrlList([...urlList, url]);
      setUrl(''); // Clear the URL input after adding
    }
  };

  const handleDeleteUrl = (indexToDelete) => {
    setUrlList(urlList.filter((_, index) => index !== indexToDelete));
  };

  const handleSubmit = () => {
    if (appName && location && urlList.length > 0) {
      const newEntry = { appName, location, urls: urlList };
      
      // Retrieve existing data from local storage
      const existingData = JSON.parse(localStorage.getItem('blacklist')) || [];
      
      // Add the new entry to the existing data
      existingData.push(newEntry);
      
      // Save updated data back to local storage
      localStorage.setItem('blacklist', JSON.stringify(existingData));
      
      // Clear form after submission
      setAppName('');
      setLocation('');
      setUrlList([]);

      // Optional: Call the onSubmit prop if needed
      if (onSubmit) {
        onSubmit(newEntry);
      }
    }
  };

  return (
    <div className="blacklist-box">
      <h3>Blacklist</h3>
      <input
        type="text"
        placeholder="App Name"
        value={appName}
        onChange={(e) => setAppName(e.target.value)}
      />
      <input
        type="text"
        placeholder="Location"
        value={location}
        onChange={(e) => setLocation(e.target.value)}
      />
      <div className="url-input-container">
        <input
          type="text"
          placeholder="URL"
          value={url}
          onChange={(e) => setUrl(e.target.value)}
        />
        <button className="add-btn" onClick={handleAddUrl}> +</button>
      </div>
      <div className="url-list-box">
        {urlList.map((url, index) => (
          <div className="url-item" key={index}>
            {url}
            <FontAwesomeIcon
              icon={faTrashAlt}
              className="delete-icon"
              onClick={() => handleDeleteUrl(index)}
            />
          </div>
        ))}
      </div>
      <button
        className="submit-btn"
        onClick={handleSubmit}
        disabled={!appName || !location || urlList.length === 0}
      >
        Submit
      </button>
    </div>
  );
};

export default BlacklistForm;
