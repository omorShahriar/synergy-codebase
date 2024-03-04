
var express = require('express');
var path = require('path');
var cors = require('cors')
var cookieParser = require('cookie-parser');
var logger = require('morgan');
const startCronJobs = require('./crons/hourlyJob');

var indexRouter = require('./routes/index');
var readingPerFiveMinutesRouter = require('./routes/get-reading-per-five-minutes');
const mqttHandler = require('./mqtt/mqttHandler');


var app = express();



app.use(logger('dev'));
app.use(express.json());
app.use(express.urlencoded({ extended: false }));
app.use(cookieParser());
app.use(express.static(path.join(__dirname, 'public')));
app.use(cors())

startCronJobs();


var mqttClient = new mqttHandler();
mqttClient.connect();


app.use('/', indexRouter);
app.use('/get-reading-per-five-minutes', readingPerFiveMinutesRouter);



module.exports = app;
