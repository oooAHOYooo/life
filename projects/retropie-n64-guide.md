# Raspberry Pi RetroPie N64 Guide

Turning your Raspberry Pi into a retro gaming console is a fantastic project! Nintendo 64 (N64) is one of the more challenging retro systems to emulate smoothly, but with the right setup, you can definitely play classics like *Super Smash Bros*, *Mario Kart 64*, *Super Mario 64*, and *The Legend of Zelda: Majora's Mask*.

Here is a step-by-step guide to get you up and running so you can review and execute it later.

## 1. Hardware Requirements

To play N64 games smoothly, you need the right Raspberry Pi hardware. Older models will struggle.
* **Raspberry Pi 4 (or Pi 5):** N64 emulation requires some processing power. A Pi 4 (2GB RAM or higher) works well, but a **Raspberry Pi 5** provides a massive performance boost and is highly recommended for stable 30-60 FPS on N64 games.
* **Power Supply:** The official Raspberry Pi USB-C power supply for your specific model.
* **MicroSD Card:** A high-quality MicroSD card (32GB or 64GB is plenty for N64 games; SanDisk or Samsung are recommended).
* **Micro HDMI to HDMI cable** (to connect to your TV/Monitor).
* **USB Controller:** Any USB controller works (Xbox, PlayStation, or an N64-style USB controller).
* **Cooling:** A case with a fan or a good heatsink. N64 emulation makes the Pi run hot, and if it overheats, it will throttle performance and cause lag.

---

## 2. Choose and Install your Emulator OS

There are dedicated "Operating Systems" built specifically for retro gaming. The most popular are **RetroPie**, **Recalbox**, and **Batocera**. We'll focus on **RetroPie** as it is the most popular and customizable.

### How to install RetroPie:
1. Download the **[Raspberry Pi Imager](https://www.raspberrypi.com/software/)** to your PC.
2. Insert your MicroSD card into your PC.
3. Open the Pi Imager app.
4. Click **Choose Device** and select your Raspberry Pi model (e.g., Raspberry Pi 4 or 5).
5. Click **Choose OS**. Go to **Emulation and game OS** -> **RetroPie** -> Select the version for your Pi model.
6. Click **Choose Storage** and select your MicroSD card.
7. Click **Next** and wait for it to finish flashing and verifying. (This wipes the SD card, so make sure there's nothing important on it!).

---

## 3. First Boot & Controller Setup

1. Put the flashed MicroSD card into your Raspberry Pi.
2. Plug in your HDMI, your USB Controller, and finally the Power Cable to turn it on.
3. RetroPie will boot up (it might take a minute or two the first time as it expands the file system).
4. **Configure Controller:** The screen will say "1 Gamepad Detected". Press and hold any button on your controller. It will then ask you to map your buttons (D-Pad Up, Down, A, B, etc.). 
   * *Note: When it asks for a "Hotkey" at the very end, press the `Select` button. You will use `Select + Start` together to exit games later.*

---

## 4. Getting Games (ROMs) onto the Pi

You will need the digital game files, known as **ROMs** (e.g., `.z64` or `.n64` files). For legal reasons, you must source your own ROM backups of the games you legally own.

Once you have your N64 ROMs, you need to transfer them to the Pi. The easiest way is over your home network (Wi-Fi):

1. **Connect the Pi to Wi-Fi:** In the RetroPie main menu, go to the `RetroPie` configuration menu -> `WiFi` -> Connect to your home network.
2. **Transfer via PC:** Leave the Pi turned on. Go to your PC (which must be on the same Wi-Fi network).
3. Open your File Explorer.
4. In the address bar at the top, type `\\RETROPIE` and press Enter.
5. You should see a folder named `roms`. Open it, and scroll down to the `n64` folder.
6. Copy your N64 ROM files (like `Mario Kart 64.z64`) into this `n64` folder.
7. Restart your Raspberry Pi (Press Start -> Quit -> Restart EmulationStation).
8. The Nintendo 64 logo will now appear on your main menu, and your games will be listed!

---

## 5. Optimizing N64 Performance (Crucial Step)

N64 emulation is notoriously picky. Some games run flawlessly on the default emulator, while others might have graphical glitches or lag. RetroPie comes with multiple N64 "emulators" installed, and you can switch between them on a per-game basis.

**If a game like *Majora's Mask* or *Super Smash Bros* is lagging or looks glitchy:**
1. Select the game from the menu and press `A` to launch it.
2. Immediately, a grey box will appear that says "Press a button to configure". **Press `A` rapidly** before the game starts.
3. A blue menu will appear (the Runcommand menu).
4. Select **"Select emulator for ROM"**.
5. You will see a list of emulators. The default is usually `lr-mupen64plus-next`. If a game runs poorly, try selecting **`mupen64plus-GLideN64`** or **`mupen64plus-gles2rice`**. (Standalone emulators without the `lr-` prefix usually run much faster on the Pi than the RetroArch `lr-` cores).
6. Select "Launch" to test the game. 

Experiment with those different emulators if a specific game gives you trouble. Enjoy your retro gaming setup!
