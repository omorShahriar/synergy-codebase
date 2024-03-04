
const mqtt = require('mqtt');
const { PrismaClient } = require('@prisma/client');
const prisma = new PrismaClient();

class MqttHandler {
    constructor() {
        this.mqttClient = null;
        this.host = 'mqtt://broker.emqx.io:1883';
        this.username = 'emqx'; // mqtt credentials if these are needed to connect
        this.password = 'public';
    }

    connect() {
        // Connect mqtt with credentials (in case of needed, otherwise we can omit 2nd param)
        this.mqttClient = mqtt.connect(this.host, { username: this.username, password: this.password });

        // Mqtt error calback
        this.mqttClient.on('error', (err) => {
            console.log(err);
            this.mqttClient.end();
        });

        // Connection callback
        this.mqttClient.on('connect', () => {
            console.log(`mqtt client connected`);
        });

        // mqtt subscriptions
        this.mqttClient.subscribe('synergy/value', { qos: 0 });

        // When a message arrives, console.log it
        this.mqttClient.on('message', async function (topic, message) {
            if (topic === 'synergy/value') {

                const payload = JSON.parse(message.toString());
                const { voltage, power1, power2, power3, crnt1, crnt2, crnt3 } = payload;
                await prisma.readingPerFiveSecond.create({
                    data: {
                        voltage,
                        power1,
                        power2,
                        power3,
                        crnt1,
                        crnt2,
                        crnt3
                    }
                });
            }


        });

        this.mqttClient.on('close', () => {
            console.log(`mqtt client disconnected`);
        });
    }

    // Sends a mqtt message to topic: mytopic
    sendMessage(topic, message) {
        this.mqttClient.publish(topic, message);
    }
}

module.exports = MqttHandler;