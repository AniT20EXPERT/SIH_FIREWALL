import React, { useState, useContext } from 'react';
import { AuthContext } from '../AuthContext';
import '../Style/Login.css';
import logo from '../assets/logo.png';

const Login = () => {
  const { login } = useContext(AuthContext);
  const [username, setUsername] = useState('');
  const [password, setPassword] = useState('');

  const handleLogin = () => {
    login(username, password);
  };

  return (
    <div className="login-page"> {/* Added class here */}
      <div className="login-container">
        <img src={logo} alt="Website Logo" />
        <h2>Login</h2>
        <input
          type="text"
          placeholder="Username"
          value={username}
          onChange={(e) => setUsername(e.target.value)}
        />
        <input
          type="password"
          placeholder="Password"
          value={password}
          onChange={(e) => setPassword(e.target.value)}
        />
        <button className="login-btn" onClick={handleLogin}>
          Login
        </button>
        <div className="forgot-password">
          <a href="#">Forgot your password?</a>
        </div>
      </div>
    </div>
  );
};

export default Login;
