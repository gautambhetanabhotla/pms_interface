# IoT-based patient monitoring system

## Hardware
We have sensors on an ESP32 collecting patient data like SpO2, heart rate, and blood pressure. The ESP publishes these values to a Thingspeak channel at regular intervals.

## Software
We have a simple webpage which fetches data from `thingspeak.com` then displays it with the help of `chart.js`. The website also displays analytics of the patient's biological data. Alerts in case of abnormal health conditions are processed and sent by the ESP32 itself.