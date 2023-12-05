import React from 'react';
import { BrowserRouter as Router, Routes, Route, BrowserRouter } from 'react-router-dom';
import Login from '../components/Login';



const ApplicationRoutes = () => {
    return (
        
            <Routes>
                <Route path="/login" element={<Login />} />
            </Routes>
      
    );
}   

export default ApplicationRoutes;