import serial
import wave
import requests
import time
import os

# =====================================================
# CONFIGURATION
# =====================================================

SERIAL_PORT = "COM3"

BAUD_RATE = 921600

RECORD_SECONDS = 6

SAMPLE_RATE = 16000

CHANNELS = 1

SAMPLE_WIDTH = 4       # 32-bit

OUTPUT_FILE = "recording.wav"

DEEPGRAM_API_KEY = "d8dfb78c5a32497682c4fa716f5306274aa4a51c"


# =====================================================
# OPEN SERIAL
# =====================================================

print("Connecting to ESP32...")

ser = serial.Serial(
    SERIAL_PORT,
    BAUD_RATE,
    timeout=5
)

time.sleep(2)

print("Connected!")
print()


# =====================================================
# WAIT FOR ESP32 READY
# =====================================================

while True:

    line = ser.readline().decode(
        errors="ignore"
    ).strip()

    if line:

        print("ESP32:", line)

    if line == "READY":
        break


# =====================================================
# START RECORDING
# =====================================================

input("Press ENTER to record...")

print()
print("🎤 Recording...")

ser.write(b"RECORD\n")


# =====================================================
# WAIT FOR START
# =====================================================

while True:

    line = ser.readline().decode(
        errors="ignore"
    ).strip()

    if line == "START":
        break


print("Receiving audio...")


# =====================================================
# RECEIVE EXACT AUDIO SIZE
# =====================================================

total_bytes = (
    SAMPLE_RATE
    * RECORD_SECONDS
    * CHANNELS
    * SAMPLE_WIDTH
)

audio_data = bytearray()

while len(audio_data) < total_bytes:

    remaining = (
        total_bytes
        - len(audio_data)
    )

    chunk = ser.read(
        min(4096, remaining)
    )

    if chunk:

        audio_data.extend(chunk)

        percent = (
            len(audio_data)
            / total_bytes
            * 100
        )

        print(
            f"\rReceived: {percent:.1f}%",
            end=""
        )

print()

print(
    f"Received {len(audio_data)} bytes"
)


# =====================================================
# SAVE WAV
# =====================================================

print("Saving WAV...")

with wave.open(
    OUTPUT_FILE,
    "wb"
) as wav:

    wav.setnchannels(CHANNELS)

    wav.setsampwidth(SAMPLE_WIDTH)

    wav.setframerate(SAMPLE_RATE)

    wav.writeframes(audio_data)


print(
    f"Saved: {os.path.abspath(OUTPUT_FILE)}"
)


# =====================================================
# SEND TO DEEPGRAM
# =====================================================

print()
print("Uploading to Deepgram...")


url = (
    "https://api.deepgram.com/v1/listen"
    "?model=nova-3"
    "&smart_format=true"
    "&language=en-US"
)


headers = {
    "Authorization":
        f"Token {DEEPGRAM_API_KEY}",

    "Content-Type":
        "audio/wav"
}


with open(
    OUTPUT_FILE,
    "rb"
) as audio_file:

    response = requests.post(
        url,
        headers=headers,
        data=audio_file,
        timeout=60
    )


# =====================================================
# CHECK RESPONSE
# =====================================================

print(
    "HTTP status:",
    response.status_code
)


if response.status_code == 200:

    result = response.json()

    transcript = (
        result["results"]
        ["channels"][0]
        ["alternatives"][0]
        ["transcript"]
    )

    print()
    print("==============================")
    print("       TRANSCRIPTION")
    print("==============================")

    print(transcript)

    print("==============================")


else:

    print()
    print("❌ Deepgram error:")
    print(response.text)


ser.close()