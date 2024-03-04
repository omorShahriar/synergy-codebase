const cron = require('node-cron');
const { PrismaClient } = require('@prisma/client');
const moment = require('moment');

const prisma = new PrismaClient();

function startCronJobs() {
    cron.schedule('*/5 * * * *', async () => {
        // const endTime = moment().startOf('hour').format();
        // const startTime = moment().subtract(1, 'hours').startOf('hour').format();

        // for testing last 5 minutes
        const endTime = moment().format();
        const startTime = moment().subtract(5, 'minutes').format();



        const readings = await prisma.readingPerFiveSecond.findMany({
            where: {
                createdAt: {
                    gte: new Date(startTime),
                    lt: new Date(endTime)
                }
            }
        });

        let power1 = 0, power2 = 0, power3 = 0;
        readings.forEach(reading => {
            power1 += reading.power1;
            power2 += reading.power2;
            power3 += reading.power3;
        });

        const numberOfReadings = readings.length;
        console.log('Number of readings:', numberOfReadings);
        const elementsNum = numberOfReadings ? numberOfReadings : 45;
        try {
            await prisma.readingPerHour.createMany({
                data: [
                    { socketId: 1, power: power1 / elementsNum, startHour: startTime, endHour: endTime },
                    { socketId: 2, power: power2 / elementsNum, startHour: startTime, endHour: endTime },
                    { socketId: 3, power: power3 / elementsNum, startHour: startTime, endHour: endTime }
                ]
            });
        } catch (e) {
            console.log(e);
        } finally {
            console.log('cron done');
        }


    });
}

module.exports = startCronJobs;