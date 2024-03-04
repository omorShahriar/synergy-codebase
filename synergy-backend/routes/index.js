var express = require('express');
var router = express.Router();
const moment = require('moment-timezone');
const { PrismaClient } = require('@prisma/client');
const prisma = new PrismaClient();
/* GET home page. */
router.get('/', async function (req, res, next) {
  const reading = await prisma.readingPerFiveSecond.findUnique({
    where: {
      id: '65e335e5e2617032cb3eced2'
    }
  });
  let utcDate = reading.createdAt;
  let localDate = moment(utcDate).tz('Asia/Dhaka');
  console.log(localDate.format('HH:mm:ss'));
  res.json({ title: 'Express' });
});

module.exports = router;
