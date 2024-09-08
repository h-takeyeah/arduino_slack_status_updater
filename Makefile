# TO USE VSCODE EXTENSION IS STRONGLY RECOMMENDED

PIO := pio # pio must be in $PATH

.PHONY: all
all:
	$(PIO) run --target upload

.PHONY: listboard
listboard:
	$(PIO) device list
