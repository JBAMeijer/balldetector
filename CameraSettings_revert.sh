# Set Auto exposure to manual mode
v4l2-ctl -d /dev/video0 -c auto_exposure=0
# Set Exposure to 1000 microseconds
v4l2-ctl -d /dev/video0 -c exposure_time_absolute=0