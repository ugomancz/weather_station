# Weather Station
This project is a very simple weather station based on the NodeMCU 0.9 microcontroller and an e-ink display.

## How it works
Once the MCU boots, it connects to the pre-set WiFi and fetches the current time and weather in Ostrava, CZ from publicly available APIs.

Weather API gives updated information every hour, so depending on the current time, the MCU then turns itself off for a calculated period of time and wakes up again at the beginning of the next hour to fetch the updated data.

## TODOs

- Add an e-ink display
- Add a battery-based power supply
- Hang it on the wall :)
