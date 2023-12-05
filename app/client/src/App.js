import { BrowserRouter as Router, Routes, Route, BrowserRouter } from 'react-router-dom';
import logo from './logo.svg';
import './App.css';
import ApplicationRoutes from './/config/ApplicationRoutes';

function App() {
  return (
    <BrowserRouter>
    <ApplicationRoutes />
    </BrowserRouter>
  );
}

export default App;

