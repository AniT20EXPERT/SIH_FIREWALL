import React from 'react';
import '../Style/BlacklistTable.css';

const BlacklistTable = ({ blacklistData, onDeleteEntry }) => {
  return (
    <div className="table-container">
      <h4>Blacklist Table</h4>
      <table>
        <thead>
          <tr>
            <th>App Name</th>
            <th>Location</th>
            <th>URLs</th>
            <th>Action</th>
          </tr>
        </thead>
        <tbody>
          {blacklistData.map((data, index) => (
            <tr key={index}>
              <td>{data.appName}</td>
              <td>{data.location}</td>
              <td>
                {data.urls.map((url, urlIndex) => (
                  <div key={urlIndex}>{url}</div>
                ))}
              </td>
              <td>
                <button
                  className="delete-entry-btn"
                  onClick={() => {
                    console.log(`Deleting entry at index: ${index}`);
                    onDeleteEntry(index);
                  }}
                >
                  Delete
                </button>
              </td>
            </tr>
          ))}
        </tbody>
      </table>
    </div>
  );
};

export default BlacklistTable;
