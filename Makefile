include variables.mk

ARDUINO := arduino-cli
CACHE_PATH := cache

DEFINES := "-DPORT=\"$(PORT)\" -DWIFI_SSID=\"$(WIFI_SSID)\" -DWIFI_PASS=\"$(WIFI_PASS)\" -DSLACK_USER_TOKEN=\"$(SLACK_USER_TOKEN)\""

.PHONY: all
all:
	$(ARDUINO) compile --build-cache-path $(CACHE_PATH) \
		--build-property build.defines=$(DEFINES) --upload --port $(PORT)

.PHONY: listboard
listboard:
	$(ARDUINO) board list --format yaml
