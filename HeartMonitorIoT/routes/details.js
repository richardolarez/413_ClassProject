var express = require('express');
var router = express.Router();

/* GET home page. */
router.get('/details', function(req, res, next) {
  res.render('details', { title: 'Express' });
});

module.exports = router;
