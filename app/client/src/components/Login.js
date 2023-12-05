import React, { useState } from 'react';
import axios from 'axios';
import { Card, Input, Button } from 'antd';
import './login.css';

const { Password } = Input;

const Login = () => {
    const [username, setUsername] = useState('');
    const [password, setPassword] = useState('');

    const handleLogin = async () => {
        try {
            const response = await axios.post('http://localhost:3001/api/login', {
                username,
                password,
            });

            console.log(response.data); // Successful login message
        } catch (error) {
            console.error('Login failed:', error.response.data);
        }
    };
     
    return (
        <div>
            <div style={{ backgroundColor: 'darkblue', padding: '10px', color: 'white' }}>
                <h1>Login</h1>
            </div>
            <Card>
                <label>
                    Username:
                    <Input type="text" value={username} onChange={(e) => setUsername(e.target.value)} />
                </label>
                <br />
                <label>
                    Password:
                    <Password value={password} onChange={(e) => setPassword(e.target.value)} />
                </label>
                <br />
                <Button type="primary" onClick={handleLogin}>Login</Button>
            </Card>
            <div style={{ backgroundColor: 'red', padding: '10px', color: 'white' }}>
                <p>Footer</p>
            </div>
        </div> 
    );
};

export default Login;
