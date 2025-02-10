# Steam Killer

When trying to exit Steam from the tray icon, it opens an annoying window that steals focus and lasts for a few seconds before actually shutting down Steam.

This program creates another tray icon to allow you to quickly terminate Steam's process. You currently can do that by either right-clicking the icon and selecting "Exit" or by simply left-clicking the icon.

# Installing

Download the [release](https://github.com/jiura/steamkiller/releases/tag/v1.0) and run *steamkiller.exe* whenever you run Steam. You can do that in several ways, but for the sake of convenience the release also includes a batch file to run both Steam and Steam Killer. Just edit the paths and you're good to go.

Personally, I've created a shortcut with Steam's icon and "cmd /c "path/to/run_steam_stk.bat" as target.

# Building

Run *build.bat*.

# Side Notes

Inspired by [NoSteamWebHelper](https://github.com/Aetopia/NoSteamWebHelper), I might try to make this a DLL and have an option to toggle CEF in the future. Aetopia's program works like a charm though.
