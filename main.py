import asyncio
import threading
from kivy.app import App
from kivy.uix.screenmanager import Screen
from kivy.properties import NumericProperty
from kivy.clock import Clock
from ble_client import TrainerBLE

class Dashboard(Screen):
    power = NumericProperty(0)
    cadence = NumericProperty(0)
    speed = NumericProperty(0)

class TrainerApp(App):
    def build(self):
        self.dashboard = Dashboard()
        self.queue = asyncio.Queue()
        self.start_ble_thread()
        Clock.schedule_interval(self.update_ui, 0.1)
        return self.dashboard

    def start_ble_thread(self):
        def ble_loop():
            asyncio.run(self.ble_main())

        thread = threading.Thread(target=ble_loop, daemon=True)
        thread.start()

    async def ble_main(self):
        ble = TrainerBLE(self.queue)
        await ble.scan_and_connect()

    def update_ui(self, dt):
        try:
            while not self.queue.empty():
                data = self.queue.get_nowait()
                self.dashboard.power = data["power"]
                self.dashboard.cadence = data["cadence"]
                self.dashboard.speed = data["speed"]
        except:
            pass

if __name__ == "__main__":
    TrainerApp().run()
