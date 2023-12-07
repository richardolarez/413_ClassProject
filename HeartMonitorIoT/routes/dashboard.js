var express = require('express');
const { path } = require('../app');
var router = express.Router();

/* GET home page. */
router.get('/', function(req, res, next) {
  // res.sendFile(path.join(__dirname, '../public/login.html'));
  res.render('dashboard', { title: 'Express' });
});

module.exports = router;
 