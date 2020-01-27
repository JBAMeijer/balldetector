# Ball Detector
The ball detector is very fond of detecting orange balls. So place a ball or an orange on the Stewart Platform to make him excited.

## Ball Detection
You can start the ball detection in the terminal with the following command ./balldetector

You can also start the ball detection with the ballexec.sh executable.

Prior to operation or debugging, one shell executable has to be run. This executable turns off the auto-exposure, making the exposure time invariant to lighting conditions. The other shell executable sets the camera settings back to default.

The algorithm uses HSV-thresholding, which segments the blobs based on thresholds in the HSV spectrum.
The parameter settings of the HSV-thresholding can be adjusted in real-time by setting the bool dbg = true.

Furthermore, once the Raspberry Pi has found a ball, it sends its location to the platform controller, the Arduino Due. This information is transmitted via a UART communication line.

## Communication
The raspberry communicates with the Arduino through a UART line. To make sure all of our coordinates are send in one piece 


## Built With
* [OpenCV](https://opencv.org/) - **OpenCV ** (_Open source computer vision_) is a [library of programming functions](https://en.wikipedia.org/wiki/Library_(computing) "Library (computing)") mainly aimed at real-time [computer vision](https://en.wikipedia.org/wiki/Computer_vision "Computer vision").[[1]](https://en.wikipedia.org/wiki/OpenCV#cite_note-1) Originally developed by [Intel](https://en.wikipedia.org/wiki/Intel_Corporation "Intel Corporation"), it was later supported by [Willow Garage](https://en.wikipedia.org/wiki/Willow_Garage "Willow Garage") then Itseez (which was later acquired by Intel[[2]](https://en.wikipedia.org/wiki/OpenCV#cite_note-2)). The library is [cross-platform](https://en.wikipedia.org/wiki/Cross-platform "Cross-platform") and free for use under the [open-source](https://en.wikipedia.org/wiki/Open-source_software "Open-source software") [BSD license](https://en.wikipedia.org/wiki/BSD_license "BSD license").

