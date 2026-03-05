import asyncio
from bleak import BleakClient, BleakScanner
import struct

FTMS_SERVICE_UUID = "00001826-0000-1000-8000-00805f9b34fb"
INDOOR_BIKE_UUID = "00002ad2-0000-1000-8000-00805f9b34fb"

class TrainerBLE:
    def __init__(self, data_queue):
        self.client = None
        self.queue = data_queue

    async def scan_and_connect(self):
        print("Scanning for Elite Suito...")
        devices = await BleakScanner.discover()

        trainer = None
        for d in devices:
            if d.name and "Suito" in d.name:
                trainer = d
                break

        if not trainer:
            print("Trainer not found")
            return

        print(f"Connecting to {trainer.name}")
        self.client = BleakClient(trainer.address)
        await self.client.connect()

        await self.client.start_notify(
            INDOOR_BIKE_UUID,
            self.notification_handler
        )

        print("Connected and subscribed.")

        while True:
            await asyncio.sleep(1)

    def notification_handler(self, sender, data):
        metrics = self.decode_indoor_bike_data(data)
        if metrics:
            asyncio.create_task(self.queue.put(metrics))

    def decode_indoor_bike_data(self, data):
        flags = struct.unpack_from("<H", data, 0)[0]
        offset = 2

        speed = cadence = power = 0

        # Speed present
        if flags & 0x0001:
            raw_speed = struct.unpack_from("<H", data, offset)[0]
            speed = raw_speed / 100.0
            offset += 2

        # Cadence present
        if flags & 0x0002:
            raw_cadence = struct.unpack_from("<H", data, offset)[0]
            cadence = raw_cadence / 2.0
            offset += 2

        # Power present
        if flags & 0x0010:
            power = struct.unpack_from("<h", data, offset)[0]
            offset += 2

        return {
            "power": power,
            "cadence": cadence,
            "speed": speed
        }
