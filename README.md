<h1>Raspberry Webcap Motion project</h1>

Software requirement:
- Qt version > 5.3
- opencv  (in project uses version 2.4.9)

This is client/server app for capture video from webcam and detection motions on the image from webcam and recognition faces on image.

Server app using opencv for capture from webcam and motion detection on image. When we have detections on the image we recognition faces. If we find some one we send image for all conections client. 

server and client using ssl certification for transmit data.

For generate certification you can use openssl:

<b>openssl req -x509 -newkey rsa:1024 -keyout key.key -out key.pem -days 365 -nodes</b> 

the server app uses motions filter for skip small contours. You can change value for more sensitivity. Go to videocapture.h and change <br/>
<pre><code>
/* the minimum area of the circuit for filtering small changes */
static constexpr double MIN_CONTOUR_AREA = 200.0;
</code></pre>

in file videocapture.cpp in startCapture() you can see where using this MIN_CONTOUR_AREA:

<pre>
<code>
 /* We go through all the found contours and filter excess */
for (int i=0; i < contours.size(); i++) {
    /* We calculate the area of the contour */
    double area = cv::contourArea(contours[i]);
    /* if the area is less than the minimum circuit then go to the next contour found*/
    <b>if (area < MIN_CONTOUR_AREA) {
        continue;
    } </b>
    /* We define a rectangle which includes a circuit */
    cv::Rect rect = cv::boundingRect(contours[i]);
    isDetection = true;
}
</code></pre>

If client have connection with server they can view image from webcam. For less traffic sent each FRAME_OFFSET frame in  <b>void VideoCapture::startCapture()</b>. for default 
<pre><code>int FRAME_OFFSET=3</code></pre>  


