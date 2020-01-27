# Set Auto exposure to manual mode
v4l2-ctl -d /dev/video0 -c auto_exposure=1

# Set Exposure to 1000 microseconds
v4l2-ctl -d /dev/video0 -c exposure_time_absolute=1000

# Set resolution to 720p
v4l2-ctl -d /dev/video0 -v width=640,height=480,pixelformat=BGR3

# Set framerate to 30
v4l2-ctl -d /dev/video0 -p 30
