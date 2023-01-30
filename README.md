# 2D Platformer - Kobayashi Studios

## Description

The Pipe Maze is a 2D Platformer developed by Kobayashi Studios, founded by Héctor Báscones Zamora @Hekbas Hekbas (https://github.com/Hekbas) and Júlia Serra Trujillo @softdrawss (https://github.com/softdrawss).

![logo](https://user-images.githubusercontent.com/99959289/200201246-e9a57554-41d6-4304-8de3-caa10a13133a.png)

Link to the Alpha release: [https://github.com/softdrawss/Game_Development_Platformer/releases/tag/Alpha] (https://github.com/softdrawss/Game_Development_Platformer/releases/tag/Alpha)

Link to the Beta release: [https://github.com/softdrawss/Game_Development_Platformer/releases/tag/Beta] (https://github.com/softdrawss/Game_Development_Platformer/releases/tag/Beta)

Link to the Gold release: [github.com/softdrawss/Game_Development_Platformer/releases/tag/Gold] (https://github.com/softdrawss/Game_Development_Platformer/releases/tag/Gold)

### NOTE ON THIS VERSION!

- Added from Assigment 1: XML config file loading - read modules and entity parameters from XML
- Added from Assigment 1: Load/Save game state using XML file

## Key Features
 - DELTATIME: Game runs at constant time for all framerates
 - COIN COUNTER: Collect the coins of the map to increase the counter and the score!
 - DASH: With cooldown
 - GOD MODE: Inmortal, move freely, turns off gravity
 - HEALTH BAR: Collect extra lives to increase the health bar
 - SCARE THE PLAYER: ENTER scares the player, who runs away and starts game automatically
 - SCORE: Collecting coins increases the Score
 - TIMER: Timer starts when entering the level. If player does not end the level before the timeline, the game will end automatically kill the player and show the endScreen

## GUI Manager

With color and audio feedback. 

### Buttons
 - PLAY
 - CONTINUE
 - SETTINGS
 - CREDITS
 - BACK
 - RESUME
 - BACK TO TITLE
 - EXIT

### Sliders
 -  MUSIC VOLUME
 -  FX VOLUME

### Checkboxes
 - FULLSCREEN MODE
 - VSYNC

## Controls

 - WASD: Movement
 - Q: Attack
 - SPACE: Jump
 - SHIFT: Dash
 - ENTER: Scare the player and enter the game
 - ESC: Pause Menu
 
 ### Debug
 Press TAB to enter Debug Mode.
 
 - C: Show Camera Limits
 - F: Free Camera (to move, use arrows)
 - V: Show Variables
 - F1: Start from the first level
 - F2: Start from the second level
 - F3: Start from the beginning of the current level
 - F5: Save the current game state
 - F6: Load the previous state (even across levels)
 - F8: View GUI bounds rectangles and state in different colors
 - F9: View colliders / logic / paths
 - F10: God Mode (fly around, cannot be killed, etc)
 - F11: Enable/Disable FPS cap to xml set value

## Developers

 - Héctor Báscones - Programmer, Designer
 - Júlia Serra Trujillo - Designer, Audio Engineer, Programmer

## License

This project is licensed under an unmodified MIT license, which is an OSI-certified license that allows static linking with closed source software. Check [LICENSE](LICENSE) for further details.
