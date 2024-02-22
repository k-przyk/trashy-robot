from motor_controller import MotorController
import sys
import os 
import curses

def main(): 
    mc = MotorController() 
    mc.init_driver() #Defaults to 50, channel to 14  
    if sys.argv[1] == 1: 
        mc.arm() 
    mc.print_speed() 

    # get the curses screen window
    screen = curses.initscr()

    # turn off input echoing
    curses.noecho()

    # respond to keys immediately (don't wait for enter)
    curses.cbreak()

    # map arrow keys to special values
    screen.keypad(True)

    try:
        while True:
            char = screen.getch()
            if char == ord('q'):
                break
            elif char == ord('a'):
                mc.turn_left() 
                screen.addstr(0, 0, f'left   {hex(mc._angle)}')
            elif char == ord('d'):
                mc.turn_right() 
                screen.addstr(0, 0, f'left   {hex(mc._angle)}')
            elif char == ord('w'):
                mc.fwd()
                screen.addstr(0, 0, f'up   {hex(mc.speed)}')
            elif char == curses.KEY_DOWN:
                mc.bck() 
                screen.addstr(0, 0, f'down {hex(mc.speed)}')
    finally:
        # shut down cleanly
        curses.nocbreak()
        screen.keypad(0)
        curses.echo()
        curses.endwin()
        mc.exit() 



if  __name__ == "__main__": 
    #Determine if calibration is needed store it in the first arg 
    if len(sys.argv) != 2: 
        print("Need Calibration arg") 
        exit() 
    main() 
