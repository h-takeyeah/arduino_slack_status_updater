# Slack status changer built on Arduino

for M5Stack. Windows

## build
1. install arduino-cli, make
2. copy `variables.mk.sample` to `variables.mk` and fill vars properly (see section below)
3. run `make`

## variables
- **PORT**: serial port to upload bin to your board; `make listboard` helps find appropriate value
- **WIFI\_SSID**, **WIFI_PASS**: wifi ssid and its password
- **SLACK\_USER_TOKEN**: required scope is *[users.profile:write](https://api.slack.com/scopes/users.profile:write)*

## troubleshooting
### arduino-cli not found
extension may be required. override var like this: `make -e ARDUINO=arduino-cli.exe`

### find board
connect your board and run `make listboard`. port.address is the port
