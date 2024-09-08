# Slack status changer built on Arduino

for seeed xiao esp32c3. Windows

## build
1. install [platformio-ide](https://platformio.org/platformio-ide)
2. copy `variables.ini.sample` to `variables.ini` and fill vars properly (see section below)
3. upload

## variables
- **PORT**: serial port to upload bin to your board
- **WIFI\_SSID**, **WIFI_PASS**: wifi ssid and its password
- **SLACK\_USER_TOKEN**: required scope is *[users.profile:write](https://api.slack.com/scopes/users.profile:write)*

<!--## troubleshooting
### arduino-cli not found
extension may be required. override var like this: `make -e ARDUINO=arduino-cli.exe`

### find board
connect your board and run `make listboard`. port.address is the port

## notes
push A button to reset and exit deep-sleep-->
