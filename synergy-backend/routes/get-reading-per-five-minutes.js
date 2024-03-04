var express = require('express');
var router = express.Router();
const moment = require('moment-timezone');

const { PrismaClient } = require('@prisma/client');
const prisma = new PrismaClient();

router.get('/', async function (req, res, next) {

    const startTime = moment().subtract(5, 'minutes').format();
    let reading = await prisma.readingPerHour.findMany({
        where: {
            createdAt: {
                gte: new Date(startTime)
            }
        },
        take: 3
    });
    if (reading.length === 0) {
        reading = await prisma.readingPerHour.findMany({
            orderBy: {
                createdAt: 'desc'
            },
            take: 3
        });
    }
    console.log(reading);
    // let utcDate = reading.createdAt;
    // let localDate = moment(utcDate).tz('Asia/Dhaka');
    // console.log(localDate.format('HH:mm:ss'));

    res.json({ socketData: reading });
});

module.exports = router;